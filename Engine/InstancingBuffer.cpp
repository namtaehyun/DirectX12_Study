#include "pch.h"
#include "InstancingBuffer.h"
#include "Engine.h"
InstancingBuffer::InstancingBuffer()
{
}

InstancingBuffer::~InstancingBuffer()
{
}

void InstancingBuffer::Init(uint32 maxCount)		// instacing 갯수 - ex 100개만들꺼? 그럼 100개
{
	_maxCount = maxCount;

	const int32 bufferSize = sizeof(InstancingParams) * maxCount;
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);	// 버퍼만들어주고
	
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_buffer));
}

void InstancingBuffer::Clear()
{
	_data.clear();
}

void InstancingBuffer::AddData(InstancingParams& params)
{
	_data.push_back(params);			// 같은 물체들끼리 뭉쳐서 관리 해줘야 할 data들
}

// 최종적으로 Rendering 하기 전에 PushData실행.
void InstancingBuffer::PushData()
{
	const uint32 dataCount = GetCount();
	if (dataCount > _maxCount);
	Init(dataCount);

	const uint32 bufferSize = dataCount * sizeof(InstancingParams);

	void* dataBuffer = nullptr;
	D3D12_RANGE readRange{0, 0};
	_buffer->Map(0, &readRange, &dataBuffer);
	memcpy(dataBuffer, &_data[0], bufferSize);
	_buffer->Unmap(0, nullptr); 
	// 데이터를 싹 복사해준다.

	_bufferView.BufferLocation = _buffer->GetGPUVirtualAddress();
	_bufferView.StrideInBytes = sizeof(InstancingParams);
	_bufferView.SizeInBytes = bufferSize;
}
