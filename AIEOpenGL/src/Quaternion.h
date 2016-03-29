#pragma once
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/ext.hpp"

using glm::quat;
using glm::vec3;

class Quaternion
{
public:
	quat quaternion;

	Quaternion();

	Quaternion(const vec3& eulerAngles);

	Quaternion(const quat& _quaternion);

	vec3 Euler();

	float GetX();

	float GetY();

	float GetZ();

	void Set(vec3 eulerAngles);
	
	void Set(float x, float y, float z);
	
	void SetX(float x);
	
	void SetY(float y);
	
	void SetZ(float z);

#pragma region Adder Functions

	void Add(float x, float y, float z);
	
	void AddX(float x);
	
	void AddY(float y);
	
	void AddZ(float z);

	void Add(Quaternion _quaternion);

#pragma endregion

#pragma region Operator Overloads

	const Quaternion& operator=(const vec3& eulerAngles);

	const Quaternion& operator=(const quat& eulerAngles);

#pragma endregion
};

