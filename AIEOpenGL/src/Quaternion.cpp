#include "Quaternion.h"



Quaternion::Quaternion::Quaternion()
{
}

Quaternion::Quaternion(const vec3& eulerAngles)
{
	*this = eulerAngles;
}

Quaternion::Quaternion(const quat& _quaternion)
{
	quaternion = _quaternion;
}

vec3 Quaternion::Euler()
{
	return glm::eulerAngles(*((quat*)this)) * (glm::pi<float>() / 180.f);
}

float Quaternion::GetX()
{
	return quaternion.x;
}

float Quaternion::GetY()
{
	return quaternion.y;
}

float Quaternion::GetZ()
{
	return quaternion.z;
}

void Quaternion::Set(vec3 eulerAngles)
{
	quaternion = quat(eulerAngles * (glm::pi<float>() / 180.f));
}

void Quaternion::Set(float x, float y, float z)
{
	*this = vec3(x, y, z);
}

void Quaternion::SetX(float x)
{
	Set(x, GetY(), GetZ());
}

void Quaternion::SetY(float y)
{
	Set(GetX(), y, GetZ());
}

void Quaternion::SetZ(float z)
{
	Set(GetX(), GetY(), z);
}

#pragma region Adder Functions

void Quaternion::Add(float x, float y, float z)
{
	quaternion *= quat(vec3(x, y, z) * (glm::pi<float>() / 180.f));
}

void Quaternion::AddX(float x)
{
	Add(x, 0, 0);
}

void Quaternion::AddY(float y)
{
	Add(0, y, 0);
}

void Quaternion::AddZ(float z)
{
	Add(0, 0, z);
}

void Quaternion::Add(Quaternion _quaternion)
{
	quaternion *= _quaternion.quaternion;
}

#pragma endregion

#pragma region Operator Overloads

const Quaternion& Quaternion::operator=(const vec3& eulerAngles)
{
	quaternion = quat(eulerAngles);

	return *this;
}

const Quaternion& Quaternion::operator=(const quat& eulerAngles)
{
	quaternion = quat(eulerAngles);

	return *this;
}

#pragma endregion