#pragma once
class TableDescriptorHeap
{
public:
	void											Init(uint32 count);

	void											Clear();
	void											SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg);
	void											SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg);
	void											CommitTable();

	ComPtr<ID3D12DescriptorHeap>			GetDescriptorHeap() { return _descHeap; }

	D3D12_CPU_DESCRIPTOR_HANDLE		GetCPUHandle(CBV_REGISTER reg);
	D3D12_CPU_DESCRIPTOR_HANDLE		GetCPUHandle(SRV_REGISTER reg);
private:
	D3D12_CPU_DESCRIPTOR_HANDLE		GetCPUHandle(uint8 reg);
private:
	ComPtr<ID3D12DescriptorHeap>			_descHeap;
	uint64											_handleSize = 0;				// 각 핸들별로 몇칸 넘어가야되는지?
	uint64											_groupSize = 0;				// DescHeap의 그룹의 크기
	uint64											_groupCount = 0;			// DescHeap의 그룹의 갯수

	uint32											_currentGroupIndex = 0;
};

