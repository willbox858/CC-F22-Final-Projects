#pragma once

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ew {
	glm::mat4 translate(const glm::vec3& t) {
		return glm::mat4{
			1.0, 0.0, 0.0, 0.0,  //col 0
			0.0, 1.0, 0.0, 0.0,  //col 1 
			0.0, 0.0, 1.0, 0.0,  //col 2
			t.x, t.y, t.z, 1.0   //col 3
		};
	}

	glm::mat4 rotateX(float a) {
		return glm::mat4{
			1.0,  0.0, 0.0, 0.0,
			0.0, cos(a), sin(a), 0.0,
			0.0, -sin(a), cos(a), 0.0,
			0.0, 0.0, 0.0, 1.0
		};
	}

	glm::mat4 rotateY(float a) {
		return glm::mat4{
			cos(a),  0.0, sin(a), 0.0,
			0.0,         1.0, 0.0,       0.0,
			-sin(a), 0.0, cos(a), 0.0,
			0.0,         0.0, 0.0,       1.0
		};
	}

	glm::mat4 rotateZ(float a) {
		return glm::mat4{
			cos(a), sin(a), 0.0, 0.0,  //col 0
			-sin(a), cos(a),  0.0, 0.0,//col 1 
			0.0, 0.0,               1.0, 0.0, //col 2
			0.0, 0.0,               0.0, 1.0  //col 3
		};
	}

	glm::mat4 scale(const glm::vec3& s) {
		return glm::mat4{
			s.x, 0.0, 0.0, 0.0, //col 0
			0.0, s.y, 0.0, 0.0,	//col 1 
			0.0, 0.0, s.z, 0.0,	//col 2
			0.0, 0.0, 0.0, 1.0	//col 3
		};
	}

	float dot(const glm::vec3& a, const glm::vec3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	glm::vec3 cross(const glm::vec3& a, const glm::vec3& b) {
		return glm::vec3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		);
	}

	float magnitude(const glm::vec3& v) {
		return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	glm::vec3 normalize(const glm::vec3& v) {
		return v / magnitude(v);
	}

	glm::mat4 lookAt(const glm::vec3& eyePos, const glm::vec3& centerPos, const glm::vec3& up) {
		glm::vec3 f = normalize(eyePos - centerPos);
		glm::vec3 r = normalize(cross(up, f));
		glm::vec3 u = normalize(cross(f, r));

		return glm::mat4(
			r.x, u.x, f.x, 0.0,
			r.y, u.y, f.y, 0.0,
			r.z, u.z, f.z, 0.0,
			0.0, 0.0, 0.0, 1.0
		) * translate(-eyePos);
	}

	glm::mat4 ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
		glm::mat4 m(1);
		m[0][0] = 2.0f / (right - left);
		m[1][1] = 2.0f / (top - bottom);
		m[2][2] = -2.0f / (farPlane - nearPlane);
		m[3][0] = -(right + left) / (right - left);
		m[3][1] = -(top + bottom) / (top - bottom);
		m[3][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
		return m;
	}

	glm::mat4 perspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
		const float c = tan(fov / 2.0f);
		glm::mat4 m(1);
		m[0][0] = 1.0f / (aspectRatio * c);
		m[1][1] = 1.0f / (c);
		m[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
		m[2][3] = -1.0f;
		m[3][2] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);

		return m;
	}
}
