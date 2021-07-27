#include "pch.h"
#include "Input.h"

void Input::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}

// ������ ������ �ȿ����� Ű ���°� �����ϴٰ� �����ؾ��ϱ� ������ �� ������ Update�� ȣ���մϴ�.
void Input::Update()
{
	HWND hwnd = GetActiveWindow();
	// ���� Ȱ��ȭ�� ������ â�� �� â�� �ƴϸ� �����ִ� Ű�� �Է� ���¸� ��� none���� �ʱ�ȭ
	// �� ������ ����ǰ� ���� ���� Ű �Է��� �ް� �մϴ�.
	if (_hwnd != hwnd)
	{
		for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
		{
			_states[key] = KEY_STATE::NONE;
		}

		return;
	}

	BYTE asciiKeys[KEY_TYPE_COUNT] = {};

	//255���� ����Ű���� Ű ���¸� �޾ƿ´�.
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// window API Ű üũ�ϴ� �Լ�.
		// Ű�� ���� ������ true
		//if (::GetAsyncKeyState(key) & 0x8000)
		if (asciiKeys[key] & 0x80)
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

			// ���� �����ӿ� Ű�� ���� ���¶�� UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::UP;
			else
				state = KEY_STATE::NONE;
		}
	}
}