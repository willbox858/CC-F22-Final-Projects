#include <sstream>
#include <random>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

//The Shader.h and Mesh.h files were both written by the professor to make working with OpenGL shaders/buffers/uniforms easier 
//ShapeGen.h was also written by the professor, and handles automatically generating the vertex data for certain shapes given 
//a number of starting values (height, width, depth, position, etc)
#include "EW/Shader.h"
#include "EW/Mesh.h"
#include "EW/ShapeGen.h"

//
#include "WBox/Lights.h"
#include "WBox/Math.h"
#include "WBox/Camera.h"
#include "WBox/Transform.h"

void processInput(GLFWwindow* window);
void resizeFrameBufferCallback(GLFWwindow* window, int width, int height);
void keyboardCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods);
float randomRange(float min, float max);
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void mousePosCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

glm::vec3 getPointOnSphere(float radius);

const int NUM_OF_POINT_LIGHTS = 2;

float lastFrameTime;
float deltaTime;

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 640;

float moveSpeed = 1.0f;
float rotateSpeed = 3.0f;
float scaleSpeed = 1.0f;

double prevMouseX;
double prevMouseY;
bool firstMouseInput = false;

/* Button to lock / unlock mouse
* 1 = right, 2 = middle
* Mouse will start locked. Unlock it to use UI
* */
const int MOUSE_TOGGLE_BUTTON = 1;
const float MOUSE_SENSITIVITY = 0.1f;

WB::Camera camera((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);

WB::Transform cubeTransform;
WB::Transform sphereTransform;
WB::Transform coneTransform;
WB::Transform lightTransform1;
WB::Transform lightTransform2;

glm::vec3 bgColor = glm::vec3(0);

glm::vec3 lightColor = glm::vec3(1.0f);
glm::vec3 diffuseColor = glm::vec3(0.0f, 0.0f, 0.1f);
glm::vec3 ambientColor = glm::vec3(0.0f, 0.0f, 0.25f);
glm::vec3 specularColor = glm::vec3(0.0f, 0.0f, 0.5f);

glm::vec3 orbitalDiffuseColor = glm::vec3(0.2f, 0.0f, 0.0f);
glm::vec3 orbitalAmbientColor = glm::vec3(0.5f, 0.0f, 0.0f);
glm::vec3 orbitalSpecularColor = glm::vec3(1.0f, 0.0f, 0.0f);

glm::vec3 pointLightFloats = glm::vec3(1.0f, 0.22f, 0.20f);

glm::vec3 spotLightDiffuseColor = glm::vec3(0.0f, 0.2f, 0.0f);
glm::vec3 spotLightAmbientColor = glm::vec3(0.0f, 0.5f, 0.0f);
glm::vec3 spotLightSpecularColor = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 spotLightFloats = glm::vec3(1.0f, 0.045f, 0.0075f);
float spotlightCutOffDegrees = 12.5f;

//float pointLightConstant = 1.0f;
//float pointLightLinear = 0.22f;
//float pointLightQuadratic = 0.20f;

glm::vec3 lightOrbit1Center = glm::vec3(0,0.0f,0);
float lightOrbit1Radius = 1.0f;
float lightOrbit1Speed = 1.0f;

glm::vec3 lightOrbit2Center = glm::vec3(0, 0.0f, 0);
float lightOrbit2Radius = 1.0f;
float lightOrbit2Speed = -1.0f;

bool drawAsPoints = false;

//TODO: Add material variables. HINT: A struct is helpful!

int main() {
	if (!glfwInit()) {
		printf("glfw failed to init");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lighting", 0, 0);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		printf("glew failed to init");
		return 1;
	}

	glfwSetFramebufferSizeCallback(window, resizeFrameBufferCallback);
	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);
	glfwSetCursorPosCallback(window, mousePosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// Setup UI Platform/Renderer backends
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Dark UI theme.
	ImGui::StyleColorsDark();

	//Used to draw shapes. This is the shader you will be completing.
	Shader litShader("shaders/defaultLit.vert", "shaders/defaultLit.frag");

	//Used to draw light
	Shader unlitShader("shaders/defaultLit.vert", "shaders/unlit.frag");

	MeshData cubeMeshData;
	createCube(1.0f, 1.0f, 1.0f, glm::vec3(1.0f), cubeMeshData);
	MeshData sphereMeshData;
	createSphere(0.5f, 64.0f, glm::vec3(1.0f), sphereMeshData);
	MeshData coneMeshData;
	createCone(0.75f, 1.0f, 64.0f, glm::vec3(1.0f), coneMeshData);

	Mesh cubeMesh(&cubeMeshData);
	Mesh sphereMesh(&sphereMeshData);
	Mesh coneMesh(&coneMeshData);

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glPointSize(3.0f);

	//Initialize positions
	cubeTransform.mPosition = glm::vec3(-2.0f, 0.0f, 0.0f);
	sphereTransform.mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	coneTransform.mPosition = glm::vec3(2.0f,0.0f,0.0f);

	lightTransform1.mScale = glm::vec3(0.5f);
	lightTransform2.mScale = glm::vec3(0.5f);

	Material testMaterial;
	testMaterial.mAmbient = glm::vec3(1.0f, 0.5f, 0.31f);
	testMaterial.mDiffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	testMaterial.mSpecular = glm::vec3(1.0f, 0.5f, 0.31f);
	testMaterial.mShininess = 32.0f;

	glm::vec3 directionalDirection = glm::vec3(-0.2f, -1.0f, -0.3f);

	glm::vec3 directionalAmbient = glm::vec3(0.0f);
	glm::vec3 directionalDiffuse = glm::vec3(0.0f);
	glm::vec3 directionalSpecular = glm::vec3(0.0f);
	
	DirectionalLight testDirLight(directionalAmbient,directionalDiffuse,directionalSpecular,directionalDirection);

	glm::vec3 testPointLightPositions[NUM_OF_POINT_LIGHTS];

	while (!glfwWindowShouldClose(window)) {

		testDirLight.setLight(LightType::ambient, ambientColor);
		testDirLight.setLight(LightType::diffuse, diffuseColor);
		testDirLight.setLight(LightType::specular, specularColor);

		processInput(window);
		glClearColor(bgColor.r,bgColor.g,bgColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		float time = (float)glfwGetTime();
		deltaTime = time - lastFrameTime;
		lastFrameTime = time;

		//Light position 
		lightTransform1.mPosition = lightOrbit1Center;
		lightTransform1.mPosition.x += cosf(time * lightOrbit1Speed) * lightOrbit1Radius;
		lightTransform1.mPosition.z += sinf(time * lightOrbit1Speed) * lightOrbit1Radius;

		lightTransform2.mPosition = lightOrbit2Center;
		lightTransform2.mPosition.y += cosf(time * lightOrbit2Speed) * lightOrbit2Radius;
		lightTransform2.mPosition.z += sinf(time * lightOrbit2Speed) * lightOrbit2Radius;

		//Draw
		litShader.use();
		litShader.setMat4("uProjection", camera.getProjectionMatrix());
		litShader.setMat4("uView", camera.getViewMatrix());

		//TODO: Set material uniforms, lightPos, eyePos
		litShader.setVec3("light.position", lightTransform1.mPosition);
 		litShader.setVec3("dirLight.direction", testDirLight.getDirection());
		litShader.setVec3("uEyePos", camera.getPosition());

		litShader.setVec3("uLightColor", lightColor);


		//Directional Lighting Stuff
		/*litShader.setVec3("dirLight.ambient", testDirLight.getLight(LightType::ambient));
		litShader.setVec3("dirLight.diffuse", testDirLight.getLight(LightType::diffuse));
		litShader.setVec3("dirLight.specular", testDirLight.getLight(LightType::specular));*/

		litShader.setVec3("dirLight.ambient", ambientColor);
		litShader.setVec3("dirLight.diffuse", diffuseColor);
		litShader.setVec3("dirLight.specular", specularColor);

		litShader.setVec3("material.ambient", testMaterial.mAmbient);
		litShader.setVec3("material.diffuse", testMaterial.mDiffuse);
		litShader.setVec3("material.specular", testMaterial.mSpecular);
		litShader.setFloat("material.shininess", testMaterial.mShininess);

		//Point Light Stuff
		testPointLightPositions[0] = lightTransform1.mPosition;
		testPointLightPositions[1] = lightTransform2.mPosition;

		for (int i = 0; i < NUM_OF_POINT_LIGHTS; i++)
		{
			std::stringstream ss1,ss2,ss3,ss4,ss5,ss6,ss7;
			ss1 << "pointLights[" << i << "].position";
			ss2 << "pointLights[" << i << "].ambient";
			ss3 << "pointLights[" << i << "].diffuse";
			ss4 << "pointLights[" << i << "].specular";
			ss5 << "pointLights[" << i << "].constant";
			ss6 << "pointLights[" << i << "].linear";
			ss7 << "pointLights[" << i << "].quadratic";

			litShader.setVec3(ss1.str(), testPointLightPositions[i]);

			litShader.setVec3(ss2.str(), orbitalAmbientColor);
			litShader.setVec3(ss3.str(), orbitalDiffuseColor);
			litShader.setVec3(ss4.str(), orbitalSpecularColor);

			litShader.setFloat(ss5.str(), pointLightFloats.x);
			litShader.setFloat(ss6.str(), pointLightFloats.y);
			litShader.setFloat(ss7.str(), pointLightFloats.z);
		}

		std::stringstream ss0, ss1, ss2, ss3, ss4, ss5, ss6, ss7, ss8, ss9;
		ss0 << "sptLight";

		litShader.setVec3(ss0.str()+".position", camera.getPosition());
		litShader.setVec3(ss0.str() + ".direction", camera.getForward());

		litShader.setVec3(ss0.str() + ".ambient", spotLightAmbientColor);
		litShader.setVec3(ss0.str() + ".diffuse", spotLightDiffuseColor);
		litShader.setVec3(ss0.str() + ".specular", spotLightSpecularColor);

		litShader.setFloat(ss0.str() + ".constant", spotLightFloats.x);
		litShader.setFloat(ss0.str() + ".linear", spotLightFloats.y);
		litShader.setFloat(ss0.str() + ".quadratic", spotLightFloats.z);

		litShader.setFloat(ss0.str() + ".cutoff", glm::cos(glm::radians(12.5f)));

		litShader.setVec3("uLightColor", lightColor);

		//Draw cube
		litShader.setMat4("uModel", cubeTransform.getModelMatrix());
		cubeMesh.draw(drawAsPoints);

		//Draw sphere
		litShader.setMat4("uModel", sphereTransform.getModelMatrix());
		sphereMesh.draw(drawAsPoints);

		//Draw cone
		litShader.setMat4("uModel", coneTransform.getModelMatrix());
		coneMesh.draw(drawAsPoints);

		//Draw light as a small sphere using unlit shader, ironically.
		unlitShader.use();
		unlitShader.setMat4("uProjection", camera.getProjectionMatrix());
		unlitShader.setMat4("uView", camera.getViewMatrix());
		unlitShader.setMat4("uModel", lightTransform1.getModelMatrix());
		unlitShader.setVec3("uColor", lightColor);
		sphereMesh.draw(drawAsPoints);

		//Draw second point light as a small sphere using the unlit shader
		unlitShader.use();
		unlitShader.setMat4("uProjection", camera.getProjectionMatrix());
		unlitShader.setMat4("uView", camera.getViewMatrix());
		unlitShader.setMat4("uModel", lightTransform2.getModelMatrix());
		unlitShader.setVec3("uColor", lightColor);
		sphereMesh.draw(drawAsPoints);

		//Draw UI
		ImGui::Begin("Settings");

		//TODO: Add material settings

		ImGui::ColorEdit3("Directional Ambient Color", &ambientColor.r);
		ImGui::ColorEdit3("Directional Diffuse Color", &diffuseColor.r);
		ImGui::ColorEdit3("Directional Specular Color", &specularColor.r);

		ImGui::ColorEdit3("Spot Light Ambient Color", &spotLightAmbientColor.r);
		ImGui::ColorEdit3("Spot Light Diffuse Color", &spotLightDiffuseColor.r);
		ImGui::ColorEdit3("Spot Light Specular Color", &spotLightSpecularColor.r);

		ImGui::SliderFloat("Spot Light Cutoff", &spotlightCutOffDegrees,0.0f,359.9f);
		ImGui::SliderFloat3("Spot Light Constant, Linear, Quadratic", &spotLightFloats.r, 0.0f, 1.0f);

		ImGui::ColorEdit3("Orbital Ambient Color", &orbitalAmbientColor.r);
		ImGui::ColorEdit3("Orbital Diffuse Color", &orbitalDiffuseColor.r);
		ImGui::ColorEdit3("Orbital Specular Color", &orbitalSpecularColor.r);

		ImGui::SliderFloat3("Point Light Constant, Linear, Quadratic", &pointLightFloats.r, 0.0f, 1.0f);

		ImGui::SliderFloat3("Light One Orbit Center",&lightOrbit1Center.r,-5.0f,5.0f);
		ImGui::SliderFloat("Light One Orbit Radius", &lightOrbit1Radius, 0.0f, 5.0f);
		ImGui::SliderFloat("Light One Orbit Speed", &lightOrbit1Speed, 0.0f, 3.0f);

		ImGui::SliderFloat3("Light Two Orbit Center", &lightOrbit2Center.r, -5.0f, 5.0f);
		ImGui::SliderFloat("Light Two Orbit Radius", &lightOrbit2Radius, 0.0f, 5.0f);
		ImGui::SliderFloat("Light Two Orbit Speed", &lightOrbit2Speed, 0.0f, -3.0f);

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwPollEvents();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void resizeFrameBufferCallback(GLFWwindow* window, int width, int height)
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	camera.setAspectRatio((float)SCREEN_WIDTH / SCREEN_HEIGHT);
	glViewport(0, 0, width, height);
}

void keyboardCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods)
{
	if (keycode == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (keycode == GLFW_KEY_1 && action == GLFW_PRESS) {
		camera.changeProjection(Projection::perspective);
	}
	if (keycode == GLFW_KEY_2 && action == GLFW_PRESS) {
		camera.changeProjection(Projection::orthographic);
	}
	//Toggle between drawing as points
	if (keycode == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		drawAsPoints = !drawAsPoints;
	}
	//Reset camera
	if (keycode == GLFW_KEY_R && action == GLFW_PRESS) {
		camera.setPosition(glm::vec3(0, 0, 5));
		camera.setYaw(-90.0f);
		camera.setPitch(0.0f);
		firstMouseInput = false;
	}
}

float randomRange(float min, float max)
{
	return min + (max - min) * ((float)rand() / (float)RAND_MAX);
}

void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (abs(yoffset) > 0) {
		camera.setFOV(-(float)yoffset * camera.getZoomSpeed());
		if (camera.getFOV() < 0.0f) 
		{
			camera.setFOV(0.0f);
		}
		else if (camera.getFOV() > 180.0f) 
		{
			camera.setFOV(180.0f);
		}
	}
}

void mousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) {
		return;
	}
	if (!firstMouseInput) {
		prevMouseX = xpos;
		prevMouseY = ypos;
		firstMouseInput = true;
	}
	camera.addYaw((float)(xpos - prevMouseX) * MOUSE_SENSITIVITY);
	camera.addPitch(-((float)(ypos - prevMouseY) * MOUSE_SENSITIVITY));

	if (camera.getPitch() < -89.9f) {
		camera.setPitch(-89.9f);
	}
	else if (camera.getPitch() > 89.9f) {
		camera.setPitch(89.9f);
	}
	prevMouseX = xpos;
	prevMouseY = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	//Toggle cursor lock
	if (button == MOUSE_TOGGLE_BUTTON && action == GLFW_PRESS) {
		int inputMode = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
		glfwSetInputMode(window, GLFW_CURSOR, inputMode);
		glfwGetCursorPos(window, &prevMouseX, &prevMouseY);
	}
}

float getAxis(GLFWwindow* window, int positiveKey, int negativeKey) {
	float axis = 0.0f;
	if (glfwGetKey(window, positiveKey)) {
		axis++;
	}
	if (glfwGetKey(window, negativeKey)) {
		axis--;
	}
	return axis;
}

void processInput(GLFWwindow* window) {
	float moveAmnt = camera.getMoveSpeed() * deltaTime;

	glm::vec3 forward = camera.getForward();
	glm::vec3 right = glm::normalize(glm::cross(forward, WORLD_UP));
	glm::vec3 up = glm::normalize(glm::cross(forward, right));
	camera.addPosition(forward * getAxis(window, GLFW_KEY_W, GLFW_KEY_S) * moveAmnt);
	camera.addPosition(right * getAxis(window, GLFW_KEY_D, GLFW_KEY_A) * moveAmnt);
	camera.addPosition(up * getAxis(window, GLFW_KEY_Q, GLFW_KEY_E) * moveAmnt);
}

glm::vec3 getPointOnSphere(float radius)
{
	glm::vec3 point;

	float min = 1;
	float max = 100;

	point.x = randomRange(min, max);
	point.y = randomRange(min, max);
	point.z = randomRange(min, max);

	point = glm::normalize(point);

	return radius * point;
}