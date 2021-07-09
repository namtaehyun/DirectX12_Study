#ifndef _PARTICLE_FX_
#define _PARTICLE_FX_

#include "params.fx"
#include "utils.fx"

struct Particle
{
	float3  worldPos;		// 월드 좌표계 기준 좌표
	float   curTime;			// 경과 시간
	float3  worldDir;		// 월드 좌표계 기준 방향
	float   lifeTime;			// 유지 시간
	int     alive;				// 생/사 Flag
	float3  padding;
};

StructuredBuffer<Particle> g_data : register(t9);

struct VS_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	uint id : SV_InstanceID;
};

struct VS_OUT
{
	float4 viewPos : POSITION;
	float2 uv : TEXCOORD;
	float id : ID;
};

// VS_MAIN
// g_float_0    : Start Scale
// g_float_1    : End Scale
// g_tex_0      : Particle Texture

VS_OUT VS_Main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0.f;

	float3 worldPos = mul(float4(input.pos, 1.f), g_matWorld).xyz;
	worldPos += g_data[input.id].worldPos;

	output.viewPos = mul(float4(worldPos, 1.f), g_matView);
	output.uv = input.uv;
	output.id = input.id;

	return output;
}

struct GS_OUT
{
	float4 position : SV_Position;
	float2 uv : TEXCOORD;
	uint id : SV_InstanceID;
};

[maxvertexcount(6)]
void GS_Main(point VS_OUT input[1], inout TriangleStream<GS_OUT> outputStream)
{
	//정점을 하나만 받아서 필요에 따라 그려줄지 안그려줄지 판단.
	GS_OUT output[4] =
	{
		(GS_OUT)0.f, (GS_OUT)0.f, (GS_OUT)0.f, (GS_OUT)0.f
	};

	VS_OUT vtx = input[0];
	uint id = (uint)vtx.id;
	if (0 == g_data[id].alive)		// alive가 0이면 그리질 않음. 그대로 리턴
		return;

	float ratio = g_data[id].curTime / g_data[id].lifeTime;
	float scale = ((g_float_1 - g_float_0) * ratio + g_float_0) / 2.f;

	// View Space
	output[0].position = vtx.viewPos + float4(-scale, scale, 0.f, 0.f);
	output[1].position = vtx.viewPos + float4(scale, scale, 0.f, 0.f);
	output[2].position = vtx.viewPos + float4(scale, -scale, 0.f, 0.f);
	output[3].position = vtx.viewPos + float4(-scale, -scale, 0.f, 0.f);

	// Projection Space
	output[0].position = mul(output[0].position, g_matProjection);
	output[1].position = mul(output[1].position, g_matProjection);
	output[2].position = mul(output[2].position, g_matProjection);
	output[3].position = mul(output[3].position, g_matProjection);

	output[0].uv = float2(0.f, 0.f);
	output[1].uv = float2(1.f, 0.f);
	output[2].uv = float2(1.f, 1.f);
	output[3].uv = float2(0.f, 1.f);

	output[0].id = id;
	output[1].id = id;
	output[2].id = id;
	output[3].id = id;

	outputStream.Append(output[0]);
	outputStream.Append(output[1]);
	outputStream.Append(output[2]);
	outputStream.RestartStrip();

	outputStream.Append(output[0]);
	outputStream.Append(output[2]);
	outputStream.Append(output[3]);
	outputStream.RestartStrip();
	// 기하셰이더에서 삼각형에 대한 View행렬, Proj행렬, uv좌표, 삼각형인덱스를 모두 초기화시켜준다.
	// 이걸 픽셀셰이더로 넘겨주면 그려준다.
}

float4 PS_Main(GS_OUT input) : SV_Target
{
	return g_tex_0.Sample(g_sam_0, input.uv);
}

struct ComputeShared
{
	int addCount;
	float3 padding;
};

RWStructuredBuffer<Particle> g_particle : register(u0);					// 얘를 이제 t레지스터로 넘겨줘서 계산하게 될 것.
RWStructuredBuffer<ComputeShared> g_shared : register(u1);		// 공용 버퍼

// CS_Main
// g_vec2_1 : DeltaTime / AccTime
// g_int_0  : Particle Max Count
// g_int_1  : AddCount
// g_vec4_0 : MinLifeTime / MaxLifeTime / MinSpeed / MaxSpeed
[numthreads(1024, 1, 1)]														// 이렇게 묶어두면 공통된 메모리를 같이 접근해 사용가능
void CS_Main(int3 threadIndex : SV_DispatchThreadID)				// 가장 먼저 Compute Shader 실행
{
	if (threadIndex.x >= g_int_0)
		return;

	int maxCount = g_int_0;
	int addCount = g_int_1;
	int frameNumber = g_int_2;
	float deltaTime = g_vec2_1.x;
	float accTime = g_vec2_1.y;
	float minLifeTime = g_vec4_0.x;
	float maxLifeTime = g_vec4_0.y;
	float minSpeed = g_vec4_0.z;
	float maxSpeed = g_vec4_0.w;

	// g_particle[threadIndex.x = g_particle[threadIndex.x - 1];
	// 이런식으로 이전의 것을 현재의 것으로 초기화하면 문제가 생김.
	// 병렬로 실행될 때 순서가 보장이 되지 않기 때문에. 순서 보장을 해줘야 함.
	// 이런 사항을 방지하기 위해 동기화를 해줘야 함.

	g_shared[0].addCount = addCount;			// 여기서 각 쓰레드간의 충돌이 일어날 수 있음.
	GroupMemoryBarrierWithGroupSync();		// xx를 막기 위해 대기를 해주는 것. 위에 구문이 
															// 실행이 될 때까지 대기하고 끝나면 밑으로 가도록

	if (g_particle[threadIndex.x].alive == 0)
	{
		while (true)										// xx. 밑에 동작을 하는 와중에 다른쓰레드가 같은 공유자원에 접근해서 
															// xx. 위와 같은 데이터를 써버리면 충돌 발생.
		{
			int remaining = g_shared[0].addCount;
			if (remaining <= 0)
				break;

			int expected = remaining;
			int desired = remaining - 1;
			int originalValue;
			InterlockedCompareExchange(g_shared[0].addCount, expected, desired, originalValue);
			// 인터락계열로 루프안의 데이터 초기화를 병렬로 했을 때 충돌이 나지 않도록 수행.
			// 한번에 무조건 한번만 실행되는 함수
			// expected와 addCount가 같으면 desired를 addCount에 넣어주고, addCount를 originalValue에 넣어주는 함수.
			// 밑의 함수와 같이 동작함. (병렬 충돌 방지로)
			/*
			if(g_shared[0].addCount == expected)
			{
				g_shared[0].addCount = desired;
			}
			originalValue = g_shared[0].addCount;
			*/
			if (originalValue == expected)
			{
				g_particle[threadIndex.x].alive = 1;
				break;
			}
		}

		if (g_particle[threadIndex.x].alive == 1)
		{
			float x = ((float)threadIndex.x / (float)maxCount) + accTime;

			// hlsl에서 Rand함수가 없음.
			float r1 = Rand(float2(x, accTime));
			float r2 = Rand(float2(x * accTime, accTime));
			float r3 = Rand(float2(x * accTime * accTime, accTime * accTime));

			// [0.5~1] -> [0~1]
			float3 noise =
			{
				2 * r1 - 1,
				2 * r2 - 1,
				2 * r3 - 1
			};

			// [0~1] -> [-1~1]
			float3 dir = (noise - 0.5f) * 2.f;

			g_particle[threadIndex.x].worldDir = normalize(dir);
			g_particle[threadIndex.x].worldPos = (noise.xyz - 0.5f) * 25;
			g_particle[threadIndex.x].lifeTime = ((maxLifeTime - minLifeTime) * noise.x) + minLifeTime;
			g_particle[threadIndex.x].curTime = 0.f;
		}
	}
	else
	{
		g_particle[threadIndex.x].curTime += deltaTime;
		if (g_particle[threadIndex.x].lifeTime < g_particle[threadIndex.x].curTime)
		{
			g_particle[threadIndex.x].alive = 0;
			return;
		}

		float ratio = g_particle[threadIndex.x].curTime / g_particle[threadIndex.x].lifeTime;
		float speed = (maxSpeed - minSpeed) * ratio + minSpeed;
		g_particle[threadIndex.x].worldPos += g_particle[threadIndex.x].worldDir * speed * deltaTime;
	}
}

#endif