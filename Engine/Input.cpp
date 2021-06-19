#include "pch.h"
#include "Input.h"

void Input::Init(HWND hWnd)
{
	_hWnd = hWnd;
	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
	// �ƹ��͵� �ȴ��� ����
}

void Input::Update()	// �� ������ ���� Update
{
	HWND hWnd = ::GetActiveWindow();

	if (_hWnd != hWnd)
	{
		for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
			_states[key] = KEY_STATE::NONE;

		return;
	}

	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		if (::GetAsyncKeyState(key) & 0x8000)
		{
			KEY_STATE& state = _states[key];

			// ���� �����ӿ� Ű�� ���� ���¶�� PRESS
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::PRESS;
			else
				state = KEY_STATE::DOWN;
		}
		else
		{
			KEY_STATE& state = _states[key];

			// ���� �����ӿ� Ű�� �� ���¶�� UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::UP;
			else
				state = KEY_STATE::NONE;

		}
	}
}
