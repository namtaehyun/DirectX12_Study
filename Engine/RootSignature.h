#pragma once

// [��༭ / ����]

// CPU [        ]    GPU [        ]
// �ѱ� [        ]   ��Ʈ�� [       ]

class RootSignature
{
public:
	void													Init();

	ComPtr<ID3D12RootSignature>				GetGraphicsSignature() { return _graphicsRootsignature; }
	ComPtr<ID3D12RootSignature>				GetComputeSignature() { return _computeRootsignature; }

private:
	void													CreateGraphicsRootSignature();
	void													CreateComputeRootSignature();

private:
	D3D12_STATIC_SAMPLER_DESC				_samplerDesc;
	ComPtr<ID3D12RootSignature>				_graphicsRootsignature;
	ComPtr<ID3D12RootSignature>				_computeRootsignature;
};

