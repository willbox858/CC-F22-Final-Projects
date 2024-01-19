#pragma once
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Math.h"

enum class direction
{
	forward,
	backwards,
	up,
	down,
	left,
	right,
};

enum rotation
{
	cwX,
	ccX,
	cwY,
	ccY,
	cwZ,
	ccZ
};

enum size
{
	grow,
	shrink
};


namespace WB
{
	struct Transform 
	{
		glm::vec3 mPosition = glm::vec3(0);
		glm::vec3 mRotation = glm::vec3(0);
		glm::vec3 mScale = glm::vec3(1);

		glm::mat4 getModelMatrix() {
			return WB::translate(mPosition) * WB::allAxisRotation(mRotation) * WB::scale(mScale);
		}
		void reset() {
			mPosition = glm::vec3(0);
			mRotation = glm::vec3(0);
			mScale = glm::vec3(1);
		}
	};

	struct projectionMatrix
	{
		float mFov;
		float mNearPlane;
		float mFarPlane;
		float mAspectRatio;
		int mHeight;
		int mWidth;

		glm::mat4 perspectiveMatrix;
		glm::mat4 orthoMatrix;

		projectionMatrix(float fov, float nearPlane, float farPlane, int width, int height)
		{
			mFov = fov;
			mNearPlane = nearPlane;
			mFarPlane = farPlane;
			mHeight = height;
			mWidth = width;
			mAspectRatio = width / height;

			perspectiveMatrix = WB::perspective(mFov, mAspectRatio, mNearPlane, mFarPlane);
			orthoMatrix = WB::ortho(mHeight, mAspectRatio, mNearPlane, mFarPlane);
		}

		void updateFov(float fovDelta)
		{
			mFov += fovDelta;

			perspectiveMatrix = WB::perspective(mFov, mAspectRatio, mNearPlane, mFarPlane);
		}
	};
};