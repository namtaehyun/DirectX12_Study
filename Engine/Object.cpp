#include "pch.h"
#include "Object.h"

Object::Object(OBJECT_TYPE type) : _objectType(type)
{
	static uint32 idGenerator = 1;		// 1번부터 생성할때마다 계속 그냥 늘어남.
	_id = idGenerator;
	idGenerator++;
}

Object::~Object()
{
}
