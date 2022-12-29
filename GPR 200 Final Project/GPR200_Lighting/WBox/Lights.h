#pragma once
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum LightType
{
	ambient,
	diffuse,
	specular
};

class Light
{
public:

	Light();
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	glm::vec3 getLight(LightType type);

	void setLight(LightType type, glm::vec3 color);

private:

	glm::vec3 mAmbient;
	glm::vec3 mDiffuse;
	glm::vec3 mSpecular;

};

class PointLight :public Light
{
public:

	PointLight();
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,glm::vec3 position,float constant, float linear, float quadratic);

	glm::vec3 getPosition();
	float getConstant();
	float getLinear();
	float getQuadratic();

private:

	glm::vec3 mPosition;

	float mConstant;
	float mLinear;
	float mQuadratic;

};

class DirectionalLight :public Light
{
public:

	DirectionalLight();
	DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,glm::vec3 direction);

	glm::vec3 getDirection();

private:

	glm::vec3 mDirection;

};

struct Material
{
	glm::vec3 mAmbient;
	glm::vec3 mDiffuse;
	glm::vec3 mSpecular;
	float mShininess;
};