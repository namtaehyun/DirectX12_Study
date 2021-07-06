#pragma once
#include "Object.h"

enum class SHADER_TYPE : uint8
{
	DEFERRED,
	FORWARD,
	LIGHTING,
};

enum class RASTERIZER_TYPE : uint8
{
	CULL_NONE,			// 둘다 안무시
	CULL_FRONT,			// 와인딩 오더 (시계방향 무시)
	CULL_BACK,				// 와인딩 오더 (반시계방향 무시)
	WIREFRAME,			// 뼈대만 보이는거
};

// 깊이 테스트 Skybox가 절두체를 기준으로 무조건 보여야 하는데, 깊이값이 다른 오브젝트에 밀려서 안보이면 안되니까
// 이를 위한 처리를 해줄 필요가 있음. 이를 위해 사용하는게 DEPTH_STENCIL_BUFFER인데,
enum class DEPTH_STENCIL_TYPE :uint8
{
	LESS,										// 작은 놈만 그려준다.
	LESS_EQUAL,							// 작을 때랑 같을 떄도 그려준다.
	GREATER,								// 큰 놈만 그려준다.
	GREATER_EQUAL,						// 클 때랑 같을 떄도 그려준다.
	NO_DEPTH_TEST,						// 깊이 테스트(X) + 깊이 기록(O)
	NO_DEPTH_TEST_NO_WRITE,		// 깊이 테스트(X) + 깊이 기록(X)
	LESS_NO_WRITE,						// 깊이 테스트(O) + 깊이 기록(X)
};

enum class BLEND_TYPE : uint8		// 결과물에 대한 혼합 색상 
{
	DEFAULT,
	ALPHA_BLEND,							// Alpha Blending
	ONE_TO_ONE_BLEND,
	END
};

struct ShaderInfo
{
	SHADER_TYPE shaderType = SHADER_TYPE::FORWARD;
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
	BLEND_TYPE blendType = BLEND_TYPE::DEFAULT;
	D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
};

// [일감 기술서] 외주 인력들이 뭘 해야할지 기술
class Shader : public Object
{
public:
	Shader();
	virtual ~Shader();

	void													Init(const wstring& path, ShaderInfo info = ShaderInfo(), const string& vs = "VS_Main", const string& ps = "PS_Main");
	void													Update();

	SHADER_TYPE										GetShaderType() { return _info.shaderType; }

private:
	void													CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void													CreateVertexShader(const wstring& path, const string& name, const string& version);
	void													CreatePixelShader(const wstring& path, const string& name, const string& version);
	
private:
	ShaderInfo												_info;

	ComPtr<ID3DBlob>									_vsBlob;
	ComPtr<ID3DBlob>									_psBlob;
	ComPtr<ID3DBlob>									_errBlob;

	ComPtr<ID3D12PipelineState>						_pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC		_pipelineDesc = {};
};

