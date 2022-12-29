#pragma once
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ewMath.h"

const glm::vec3 WORLD_UP = glm::vec3(0, 1, 0);

class Camera {
public:
	glm::vec3 position = glm::vec3(0, 0, 5);
	float yaw = -90.0f;
	float pitch = 0.0f;
	float fov = 60.0f;
	float nearPlane = 0.001f;
	float farPlane = 1000.0f;
	float orthoSize = 7.5f;
	bool ortho = false;
	float moveSpeed = 5.0f;
	float zoomSpeed = 3.0f; //FovY degrees per scroll click
	float aspectRatio = 1.7777f;
public:
	Camera(float aspectRatio) : aspectRatio(aspectRatio) {
	}
	glm::mat4 getViewMatrix() {
		return ew::lookAt(position, position + getForward(), WORLD_UP);
	}
	glm::mat4 getProjectionMatrix() {
		if (ortho) {
			float halfSize = orthoSize * 0.5f;
			return ew::ortho(-halfSize * aspectRatio, halfSize * aspectRatio, -halfSize, halfSize, nearPlane, farPlane);
		}
		else {
			return ew::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
		}
	}
	glm::vec3 getForward() {
		float yawRad = glm::radians(yaw);
		float pitchRad = glm::radians(pitch);

		glm::vec3 forward;
		forward.x = cos(yawRad) * cos(pitchRad);
		forward.y = sin(pitchRad);
		forward.z = sin(yawRad) * cos(pitchRad);
		return forward;
	}
};