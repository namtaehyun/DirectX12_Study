#pragma once
#include "Device.h"
#include "SwapChain.h"
#include "CommandQueue.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "Texture.h"
#include "DepthStencilBuffer.h"

#include "Input.h"
#include "Timer.h"

class Engine
{
public:

	void											Init(const WindowInfo& info);
	void											Render();

public:
	void											Update();

public:
	shared_ptr<Device>						GetDevice()						{ return _device; }
	shared_ptr<CommandQueue>			GetCmdQueue()					{ return _cmdQueue; }
	shared_ptr<SwapChain>					GetSwapChain()					{ return _swapChain; }
	shared_ptr<RootSignature>				GetRootSignature()				{ return _rootSignature; }
	shared_ptr<ConstantBuffer>				GetConstantBuffer()			{ return _constantBuffer; }
	shared_ptr<TableDescriptorHeap>		GetTableDescriptorHeap()		{ return _TableDescriptorHeap; }
	shared_ptr<DepthStencilBuffer>			GetDepthStencilBuffer()		{ return _DepthStencilBuffer; }

	shared_ptr<Input>							GetInput()						{ return _input; }
	shared_ptr<Timer>							GetTimer()						{ return _timer; }

public:
	void											RenderBegin();
	void											RenderEnd();

	void											ResizeWindow(int32 width, int32 height);

private:
	void											ShowFPS();

private:
	// �׷��� ȭ�� ũ�� ����
	WindowInfo									_window;
	D3D12_VIEWPORT							_viewport = {};
	D3D12_RECT									_scissorRect = {};

	shared_ptr<Device>						_device					= make_shared<Device>();
	shared_ptr<CommandQueue>			_cmdQueue				= make_shared<CommandQueue>();
	shared_ptr<SwapChain>					_swapChain				= make_shared<SwapChain>();
	shared_ptr<RootSignature>				_rootSignature			= make_shared<RootSignature>();
	shared_ptr<ConstantBuffer>				_constantBuffer			= make_shared<ConstantBuffer>();
	shared_ptr<TableDescriptorHeap>		_TableDescriptorHeap	= make_shared<TableDescriptorHeap>();
	shared_ptr<DepthStencilBuffer>			_DepthStencilBuffer		= make_shared<DepthStencilBuffer>();

	shared_ptr<Input>							_input						= make_shared<Input>();
	shared_ptr<Timer>							_timer						= make_shared<Timer>();
};

