#include "pch.h"
#include "Engine.h"
#include "Device.h"
#include "SwapChain.h"
#include "CommandQueue.h"


void Engine::Init(const WindowInfo& info)
{
	_window = info;

	// 그려질 화면 크기 설정
	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);

	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(info,_device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init();
	_constantBuffer->Init(sizeof(Transform), 256);	// Shader데이터와 맞춰주면됨.
	_TableDescriptorHeap->Init(256);
	_DepthStencilBuffer->Init(_window);

	_input->Init(info.hwnd);
	_timer->Init();

	ResizeWindow(info.width, info.height);
}

void Engine::Render()
{
	RenderBegin();

	// TODO : 나머지 물체를 그려준다.
	

	RenderEnd();

}

void Engine::Update()
{
	_input->Update();
	_timer->Update();

	ShowFPS();
}

void Engine::RenderBegin()
{
	_cmdQueue->RenderBegin(&_viewport, &_scissorRect);
}


void Engine::RenderEnd()
{
	_cmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)
{
	// Window 크기 설정
	_window.width = width;
	_window.height = height;

	RECT rect = { 0, 0, width, height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);	// 100,100 위치에 width, height 크기로 창을 띄운다.

	_DepthStencilBuffer->Init(_window);
}

void Engine::ShowFPS()
{
	uint32 fps = _timer->GetFPS();

	WCHAR text[100] = L"";
	::wsprintf(text, L"FPS : %d", fps);

	::SetWindowText(_window.hwnd, text);		// Window에 FPS 출력
}
