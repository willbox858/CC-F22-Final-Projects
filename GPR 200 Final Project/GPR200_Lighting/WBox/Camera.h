#pragma once

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Math.h"

const glm::vec3 WORLD_UP = glm::vec3(0, 1, 0);

enum Projection
{
	orthographic,
	perspective
};
namespace WB {
	class Camera
	{
	public:
		Camera(float aspectRatio, float fov = 60.0f, float nearPlane = 0.001f, float farPlane = 1000.0f, float orthoSize = 7.5f, float moveSpeed = 5.0f, float zoomSpeed = 3.0f)
		{
			mYaw = -90.0f;
			mPitch = 0.0f;

			mFov = fov;

			mNearPlane = nearPlane;
			mFarPlane = farPlane;

			mOrthoSize = orthoSize;
			mProj = Projection::perspective;

			mMoveSpeed = moveSpeed;
			mZoomSpeed = zoomSpeed;

			mAspectRatio = aspectRatio;
		}

		void changeProjection(Projection proj)
		{
			mProj = proj;
		}

		//Position setter/getter/adder

		glm::vec3 getPosition()
		{
			return mPosition;
		}

		void setPosition(glm::vec3 newPosition)
		{
			mPosition = newPosition;
		}

		void addPosition(glm::vec3 positionAdd)
		{
			mPosition += positionAdd;
		}

		//Aspect-Ratio setter/getter

		float getAspectRatio()
		{
			return mAspectRatio;
		}

		void setAspectRatio(float newAspectRatio)
		{
			mAspectRatio = newAspectRatio;
		}

		//Yaw setter/getter/adder

		float getYaw()
		{
			return mYaw;
		}

		void setYaw(float newYaw)
		{
			mYaw = newYaw;
		}

		void addYaw(float addYaw)
		{
			mYaw += addYaw;
		}

		//Pitch setter/getter/adder

		float getPitch()
		{
			return mPitch;
		}

		void setPitch(float newPitch)
		{
			mPitch = newPitch;
		}

		void addPitch(float addPitch)
		{
			mPitch += addPitch;
		}

		//FOV setter/getter/adder

		float getFOV()
		{
			return mFov;
		}

		void addFOV(float addFOV)
		{
			mFov += addFOV;
		}

		void setFOV(float newFOV)
		{
			mFov = newFOV;
		}

		//Camera Move Speed setter/getter

		float getMoveSpeed()
		{
			return mMoveSpeed;
		}

		void setMoveSpeed(float newMoveSpeed)
		{
			mMoveSpeed = newMoveSpeed;
		}

		//Zoom Speed setter/getter

		float getZoomSpeed()
		{
			return mZoomSpeed;
		}

		void setZoomSpeed(float newZoomSpeed)
		{
			mZoomSpeed = newZoomSpeed;
		}

		glm::mat4 getProjectionMatrix()
		{
			switch (mProj)
			{
			case Projection::orthographic:
				return WB::ortho(mOrthoSize, mAspectRatio, mNearPlane, mFarPlane);
			case Projection::perspective:
				return WB::perspective(mFov, mAspectRatio, mNearPlane, mFarPlane);
			}
		}

		glm::mat4 getViewMatrix()
		{
			return WB::lookAt(mPosition, mPosition + getForward(), WORLD_UP);
		}

		glm::vec3 getForward()
		{
			float yawRadians = glm::radians(mYaw);
			float pitchRadians = glm::radians(mPitch);

			glm::vec3 forward;

			forward.x = cos(yawRadians) * cos(pitchRadians);
			forward.y = sin(pitchRadians);
			forward.z = sin(yawRadians) * cos(pitchRadians);

			return forward;
		}

	private:

		glm::vec3 mPosition = glm::vec3(0, 0, 5);

		float mYaw;
		float mPitch;

		float mFov;
		float mNearPlane;
		float mFarPlane;

		float mOrthoSize;
		Projection mProj;

		float mMoveSpeed;
		float mZoomSpeed;

		float mAspectRatio;
	};
}