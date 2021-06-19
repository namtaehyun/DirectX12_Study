#include "pch.h"
#include "Timer.h"

void Timer::Init()
{
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount)); // CPU 클럭
	// Counter를 계산하는데 이게 CPU클럭과 연관이 있음.
}

void Timer::Update()
{
	uint64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	// 실질적으로 지난 시간을 구하기 위해 다음과 같은 계산식으로 deltaTime을 구함
	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);

	_prevCount = currentCount;

	_frameCount++;					// 1초동안 frame이 얼마나 진행되는가를 계산
	_frameTime += _deltaTime;	// frameTime은 누적시간

	if (_frameTime > 1.f)			// 1초마다 다시 계산
	{
		_fps = static_cast<uint32>(_frameCount / _frameTime);	// Frame Per Second

		_frameTime = 0.f;
		_frameCount = 0;
	}
}
