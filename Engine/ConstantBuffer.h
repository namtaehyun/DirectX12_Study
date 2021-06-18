#pragma once
class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void										Init(uint32 size, uint32 count);
	
	void										Clear();
	D3D12_CPU_DESCRIPTOR_HANDLE	PushData(int32 rootParamaterIndex, void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS		GetGpuVirtualAddress(uint32 index);
	D3D12_CPU_DESCRIPTOR_HANDLE	GetCpuHandle(uint32 index);
	
private:
	void										CreateBuffer();
	void										CreateView();

private:
	ComPtr<ID3D12Resource>				_cbvBuffer;							// 상수버퍼의 모음집
	BYTE*										_mappedBuffer	= nullptr;			// CPU에서 데이터 밀어넣는 버퍼
	uint32										_elementSize		= 0;				// 상수버퍼 각 요소의 크기
	uint32										_elementCount	= 0;				// 상수버퍼 각 요소의 갯수

	ComPtr<ID3D12DescriptorHeap>		_cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE	_cpuHandleBegin = {};				// 시작 핸들 주소
	uint32										_handleIncrementSize = 0;			// 몇 칸을 띄워야 다음 핸들로 넘어갈 수 있는지?

	uint32										_currentIndex	= 0;				// 지금 내가 사용하고 있는 버퍼의 인덱스
};