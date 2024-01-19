#pragma once

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace WB
{
	enum class Axis
	{
		xAxis,
		yAxis,
		zAxis
	};

	glm::mat4 translate(const glm::vec3& trans)
	{
		glm::mat4 temp(1.0f);

		temp[3][0] = trans.x;
		temp[3][1] = trans.y;
		temp[3][2] = trans.z;

		return temp;
	}

	glm::mat4 translate(Axis axis, float distance)
	{
		glm::mat4 temp(1.0f);

		switch (axis)
		{
		case WB::Axis::xAxis:
			temp[3][0] = distance;
			return temp;
		case WB::Axis::yAxis:
			temp[3][1] = distance;
			return temp;
		case WB::Axis::zAxis:
			temp[3][2] = distance;
			return temp;
		}
	}

	glm::mat4 rotateX(float angle)
	{
		glm::mat4 temp(1.0f);

		temp[1][1] = cos(angle);
		temp[1][2] = sin(angle);
		temp[2][1] = -1 * sin(angle);
		temp[2][2] = cos(angle);

		return temp;
	}

	glm::mat4 rotateY(float angle)
	{
		glm::mat4 temp(1);

		temp[0][0] = cos(angle);
		temp[0][2] = -1 * sin(angle);
		temp[2][0] = sin(angle);
		temp[2][2] = cos(angle);

		return temp;
	}

	glm::mat4 rotateZ(float angle)
	{
		glm::mat4 temp(1);

		temp[0][0] = cos(angle);
		temp[0][1] = sin(angle);
		temp[1][0] = -1 * sin(angle);
		temp[1][1] = cos(angle);

		return temp;
	}

	glm::mat4 rotate(Axis axis, float angle)
	{
		switch (axis)
		{
		case WB::Axis::xAxis:
			return rotateX(angle);
		case WB::Axis::yAxis:
			return rotateY(angle);
		case WB::Axis::zAxis:
			return rotateZ(angle);
		}
	}

	glm::mat4 allAxisRotation(glm::vec3 eulerAngle)
	{
		return rotate(WB::Axis::xAxis, eulerAngle.x) * rotate(WB::Axis::yAxis, eulerAngle.y) * rotate(WB::Axis::zAxis, eulerAngle.z);
	}

	glm::mat4 scale(const glm::vec3& scale)
	{
		glm::mat4 temp(1.0f);

		temp[0][0] = scale.x;
		temp[1][1] = scale.y;
		temp[2][2] = scale.z;

		return temp;
	}

	glm::mat4 scale(Axis axis, float scale)
	{
		glm::mat4 temp(1.0f);

		switch (axis)
		{
		case WB::Axis::xAxis:
			temp[0][0] = scale;
			return temp;
		case WB::Axis::yAxis:
			temp[1][1] = scale;
			return temp;
		case WB::Axis::zAxis:
			temp[2][2] = scale;
			return temp;
		}
	}

	glm::mat4 ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane)
	{
		glm::mat4 temp(1);
		temp[0][0] = 2 / (right - left);
		temp[1][1] = 2 / (top - bottom);
		temp[2][2] = -2 / (farPlane - nearPlane);
		temp[3][0] = (-1 * (right + left)) / (right - left);
		temp[3][1] = (-1 * (top + bottom)) / (top - bottom);
		temp[3][2] = (-1 * (farPlane + nearPlane)) / (farPlane - nearPlane);

		return temp;
	}

	glm::mat4 ortho(float height, float aspectRatio, float nearPlane, float farPlane)
	{
		float bottom = height / -2;
		float top = height / 2;

		float width = aspectRatio * height;

		float left = width / -2; 
		float right = width / 2;

		return ortho(left, right, bottom, top, nearPlane, farPlane);
	}

	glm::mat4 perspective(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		float fovRad = glm::radians(fov);
		float c = glm::tan(fovRad / 2);
		glm::mat4 temp(1);
		temp[0][0] = 1 / (aspectRatio * c);
		temp[1][1] = 1 / c;
		temp[2][2] = -1 * ((farPlane + nearPlane) / (farPlane - nearPlane));
		temp[2][3] = -1;
		temp[3][2] = -1 * ((2 * farPlane * nearPlane) / (farPlane - nearPlane));

		return temp;
	}

	glm::mat4 lookAt(glm::vec3 eyePos, glm::vec3 targetPos, glm::vec3 up)
	{
		glm::vec3 f = normalize(eyePos - targetPos);
		glm::vec3 r = normalize(cross(up, f));
		glm::vec3 u = normalize(cross(f, r));

		glm::mat4 temp(1);

		temp[0][0] = r.x;
		temp[1][0] = r.y;
		temp[2][0] = r.z;

		temp[0][1] = u.x;
		temp[1][1] = u.y;
		temp[2][1] = u.z;

		temp[0][2] = f.x;
		temp[1][2] = f.y;
		temp[2][2] = f.z;

		glm::vec3 translationVector = eyePos;

		translationVector *= -1;

		temp *= translate(translationVector);

		return temp;
	}
}