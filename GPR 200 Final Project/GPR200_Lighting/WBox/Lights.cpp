#include "Lights.h"

Light::Light()
{
	mAmbient = glm::vec3(0.0f);
	mDiffuse = glm::vec3(0.0f);
	mSpecular = glm::vec3(0.0f);
}

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	mAmbient = ambient;
	mDiffuse = diffuse;
	mSpecular = specular;
}

glm::vec3 Light::getLight(LightType type)
{
	switch (type)
	{
	case ambient:
		return mAmbient;
	case diffuse:
		return mDiffuse;
	case specular:
		return mSpecular;
	}
}

void Light::setLight(LightType type, glm::vec3 color)
{
	switch (type)
	{
	case ambient:
		mAmbient = color;
		return;
		break;
	case diffuse:
		mDiffuse = color;
		return;
		break;
	case specular:
		mSpecular = color;
		return;
		break;
	}
}

PointLight::PointLight():Light()
{
	mPosition = glm::vec3(0);
	mConstant = 0.0f;
	mLinear = 0.0f;
	mQuadratic = 0.0f;
}

PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, float constant, float linear, float quadratic):Light(ambient,diffuse,specular)
{
	mPosition = position;
	mConstant = constant;
	mLinear = linear;
	mQuadratic = quadratic;
}

glm::vec3 PointLight::getPosition()
{
	return mPosition;
}

float PointLight::getConstant()
{
	return mConstant;
}

float PointLight::getLinear()
{
	return mLinear;
}

float PointLight::getQuadratic()
{
	return mQuadratic;
}

DirectionalLight::DirectionalLight():Light()
{
	mDirection = glm::vec3(0.0f);
}

DirectionalLight::DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction):Light(ambient,diffuse,specular)
{
	mDirection = direction;
}

glm::vec3 DirectionalLight::getDirection()
{
	return mDirection;
}
