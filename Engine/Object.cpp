#include "pch.h"
#include "Object.h"

Object::Object(OBJECT_TYPE type) : _objectType(type)
{
	static uint32 idGenerator = 1;		// 1������ �����Ҷ����� ��� �׳� �þ.
	_id = idGenerator;
	idGenerator++;
}

Object::~Object()
{
}
