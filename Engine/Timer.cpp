#include "pch.h"
#include "Timer.h"

void Timer::Init()
{
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount)); // CPU Ŭ��
	// Counter�� ����ϴµ� �̰� CPUŬ���� ������ ����.
}

void Timer::Update()
{
	uint64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	// ���������� ���� �ð��� ���ϱ� ���� ������ ���� �������� deltaTime�� ����
	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);

	_prevCount = currentCount;

	_frameCount++;					// 1�ʵ��� frame�� �󸶳� ����Ǵ°��� ���
	_frameTime += _deltaTime;	// frameTime�� �����ð�

	if (_frameTime > 1.f)			// 1�ʸ��� �ٽ� ���
	{
		_fps = static_cast<uint32>(_frameCount / _frameTime);	// Frame Per Second

		_frameTime = 0.f;
		_frameCount = 0;
	}
}
