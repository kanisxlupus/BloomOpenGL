#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderClass.h"

class Camera
{
	public:
		glm::vec3 Position;
		glm::vec3 Orientation = glm::vec3(0.f, 0.f, -1.f);
		glm::vec3 Up = glm::vec3(0.f, 1.f, 0.f);
		glm::mat4 cameraMatrix = glm::mat4(1.f);

		// stop the camera from jumping on the first click
		bool firstClick = true;

		// window dimensions
		int width;
		int height;

		// camera speed and sensitivity
		float speed = .001f;
		float sensitivity = 100.f;

		// constructor
		Camera(int width, int height, glm::vec3 position);

		void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
		void Matrix(Shader& shader, const char* uniform);
		
		// handles peripheral inputs
		void Inputs(GLFWwindow* window);
};

#endif 