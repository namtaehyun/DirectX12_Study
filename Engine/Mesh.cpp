#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

void Mesh::Init(vector<Vertex>& vec)
{
	_vertexCount = static_cast<uint32>(vec.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);	

	D3D12_HEAP_PROPERTIES heapProperty	= CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc					= CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));
	// GPU에 공간을 할당하는 함수 - vertexBuffer

	// Copy the triangle data to the vertex buffer.
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);		// 뚜껑을 열어서
	::memcpy(vertexDataBuffer, &vec[0], bufferSize);				// GPU공간에 데이터를 밀어넣고
	_vertexBuffer->Unmap(0, nullptr);								// 뚜껑을 닫음.
	// -> 이렇게 하면 GPU에 데이터 전달 완료

	// CPU Memory -> GPU Memory 이동

	// Initialize the vertex buffer view.
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	_vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기	
}

void Mesh::Render()
{
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 정점 연결 형식(삼각형)
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView);								// Slot: (0~15)

	// TODO 인자 셋팅
	// 1) Buffer에 데이터 셋팅
	// 2) TableDescHeap에 CBV전달
	// 3) 모두 세팅이 끝났으면 TableDescHeap 커밋
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GEngine->GetConstantBuffer()->PushData(0, &_transform, sizeof(_transform));
		GEngine->GetTableDescriptorHeap()->SetCBV(handle, CBV_REGISTER::b0);
	}
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GEngine->GetConstantBuffer()->PushData(0, &_transform, sizeof(_transform));
		GEngine->GetTableDescriptorHeap()->SetCBV(handle, CBV_REGISTER::b1);
	}
	GEngine->GetTableDescriptorHeap()->CommitTable();

	CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);
}