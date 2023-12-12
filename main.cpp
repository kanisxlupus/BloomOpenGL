// Tutorial for setup from Victor Gordan (https://www.youtube.com/playlist?list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-)
// OpenGL Tutorials used as a reference (https://learnopengl.com)

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


#include "Model.h"

using namespace std;

// Constant Variables
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const char* WINDOW_TITLE = "Final Project - Sam Jones";

// Global Variables
float gamma = 2.2f;

// Framebuffer quad verts
float rectangleVertices[] =
{
	// COORDINATES		// TEXTURE COORDINATES
	 1.0f, -1.0f,		1.0f, 0.0f,
	-1.0f, -1.0f,		0.0f, 0.0f,
	-1.0f,  1.0f,		0.0f, 1.0f,

	 1.0f,  1.0f,		1.0f, 1.0f,
	 1.0f, -1.0f,		1.0f, 0.0f,
	-1.0f,  1.0f,		0.0f, 1.0f
};

// Vertices for plane with texture
vector<Vertex> vertices =
{
	Vertex{glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for plane with texture
vector<GLuint> indices =
{
	0, 1, 2,
	0, 2, 3
};

// Vertices for cube
Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
};

// Indices for cube
GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

int main()
{
	// WINDOW SETUP
	// ------------------------------------------------------------------------------------------
	// Set up GLFW to draw the window
	glfwInit();

	// Tell GLFW what version of OpenGl we're using (3.3 for tutorial)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell GLFW to use the CORE profile (different from other cs450 projects)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

	// Window error checking
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;

		return -1;
	}

	// Draw the window
	glfwMakeContextCurrent(window);

	// Load GLAD
	gladLoadGL();

	// Specify the viewport
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


	// SETUP SHADERS
	// ------------------------------------------------------------------------------------------
	Shader shaderProgram("default.vert", "default.frag");
	Shader framebufferProgram("framebuffer.vert", "framebuffer.frag");
	Shader lightProgram("light.vert", "light.frag");
	Shader bloomProgram("framebuffer.vert", "bloom.frag");


	// SETUP LIGHTING AND CONSTANT SHADER UNIFORMS
	// ------------------------------------------------------------------------------------------
	glm::vec4 lightColor0 = glm::vec4(100.f, 100.f, 100.f, 100.f);
	glm::vec3 lightPos0 = glm::vec3(.5f, .5f, .5f);

	glm::vec4 lightColor1 = glm::vec4(0.f, 0.f, 100.f, 1.f);
	glm::vec3 lightPos1 = glm::vec3(-.5f, .3f, .5f);

	glm::vec4 lightColor2 = glm::vec4(0.f, 100.f, 0.f, 1.f);
	glm::vec3 lightPos2 = glm::vec3(-.5f, .75f, -.5f);

	glm::vec4 lightColor3 = glm::vec4(100.f, 0.f, 0.f, 1.f);
	glm::vec3 lightPos3 = glm::vec3(.5f, .6f, -.5f);


	lightProgram.Activate();
	glUniform3f(glGetUniformLocation(lightProgram.ID, "lightPos"), lightPos0.x, lightPos0.y, lightPos0.z);
	glUniform4f(glGetUniformLocation(lightProgram.ID, "lightColor"), lightColor0.x, lightColor0.y, lightColor0.z, lightColor0.w);
	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor0"), lightColor0.x, lightColor0.y, lightColor0.z, lightColor0.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos0"), lightPos0.x, lightPos0.y, lightPos0.z);
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor1"), lightColor1.x, lightColor1.y, lightColor1.z, lightColor1.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos1"), lightPos1.x, lightPos1.y, lightPos1.z);
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor2"), lightColor2.x, lightColor2.y, lightColor2.z, lightColor2.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos2"), lightPos2.x, lightPos2.y, lightPos2.z);
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor3"), lightColor3.x, lightColor3.y, lightColor3.z, lightColor3.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos3"), lightPos3.x, lightPos3.y, lightPos3.z);
	framebufferProgram.Activate();
	glUniform1i(glGetUniformLocation(framebufferProgram.ID, "screenTexture"), 0);
	glUniform1i(glGetUniformLocation(framebufferProgram.ID, "bloomTexture"), 1);
	glUniform1f(glGetUniformLocation(framebufferProgram.ID, "gamma"), gamma); // Add Gamma Correction to custom framebuffer
	bloomProgram.Activate();
	glUniform1i(glGetUniformLocation(bloomProgram.ID, "screenTexture"), 0);

	// SET UP DEPTH BUFFER
	// ------------------------------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);


	// SET UP CAMERA
	// ------------------------------------------------------------------------------------------
	Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(0.f, 0.f, 2.f));


	// SET UP CUSTOM FRAMEBUFFERS
	// ------------------------------------------------------------------------------------------

	// create VBO and VAO for framebuffer quad
	unsigned int rectVAO, rectVBO;
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	
	// create framebuffer object
	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// create framebuffer color texture
	unsigned int framebufferTexture;
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

	// create bloom texture
	unsigned int bloomTexture;
	glGenTextures(1, &bloomTexture);
	glBindTexture(GL_TEXTURE_2D, bloomTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bloomTexture, 0);

	// tell openGL to use both textures
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);



	// create a renderbuffer object for the depth and stencil buffers (more efficient than a framebuffer object)
	// (but can't access in shaders)
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// Framebuffer error checking
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "Framebuffer Error: " << fboStatus << endl;
	}
	else
	{
		cout << "Framebuffer FBO successfully created" << endl;
	}

	// setup ping pong framebuffers
	unsigned int pingpongFBO[2];
	unsigned int pingpongBuffer[2];

	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongBuffer);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0);
	
		fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			cout << "PingPong(" << i << ") Framebuffer Error: " << fboStatus << endl;
		}
		else
		{
			cout << "PingPong Framebuffer " << i << " FBO successfully created" << endl;
		}
	}

	// IMPORT TEXTURES FOR OBJECTS
	// ------------------------------------------------------------------------------------------
	vector<Texture> textures =
	{
		Texture("planks.png", "diffuse", 0),
	};

	// PREP CUBE OBJECT
	// ------------------------------------------------------------------------------------------
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));


	// LOAD IN MODELS & MESHES
	// ------------------------------------------------------------------------------------------
	Model map("map/scene.gltf");

	Mesh plane(vertices, indices, textures);
	Mesh cube(lightVerts, lightInd, textures);

	// MAIN RENDER LOOP
	// ------------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// Activate custom framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		// Set the window background color
		glClearColor(pow(0.07f, gamma), pow(0.13f, gamma), pow(0.17f, gamma), 1.0f);

		// Clear the back buffer and assign new color, clear the depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);


		// Set up Camera
		camera.Inputs(window);
		camera.updateMatrix(45.f, .1f, 100.f);

		// Draw models
		plane.Draw(shaderProgram, camera);
		//map.Draw(shaderProgram, camera);
		cube.Draw(lightProgram, camera, glm::mat4(1.f), -lightPos0);
		
		glUniform3f(glGetUniformLocation(lightProgram.ID, "lightPos"), lightPos1.x, lightPos1.y, lightPos1.z);
		glUniform4f(glGetUniformLocation(lightProgram.ID, "lightColor"), lightColor1.x, lightColor1.y, lightColor1.z, lightColor1.w);
		cube.Draw(lightProgram, camera, glm::mat4(1.f), -lightPos1);

		glUniform3f(glGetUniformLocation(lightProgram.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
		glUniform4f(glGetUniformLocation(lightProgram.ID, "lightColor"), lightColor2.x, lightColor2.y, lightColor2.z, lightColor2.w);
		cube.Draw(lightProgram, camera, glm::mat4(1.f), -lightPos2);

		glUniform3f(glGetUniformLocation(lightProgram.ID, "lightPos"), lightPos3.x, lightPos3.y, lightPos3.z);
		glUniform4f(glGetUniformLocation(lightProgram.ID, "lightColor"), lightColor3.x, lightColor3.y, lightColor3.z, lightColor3.w);
		cube.Draw(lightProgram, camera, glm::mat4(1.f), -lightPos3);

		glUniform3f(glGetUniformLocation(lightProgram.ID, "lightPos"), lightPos0.x, lightPos0.y, lightPos0.z);
		glUniform4f(glGetUniformLocation(lightProgram.ID, "lightColor"), lightColor0.x, lightColor0.y, lightColor0.z, lightColor0.w);


		// Blur the bloom texture
		bool horizontal = true;
		bool first_iteration = true;
		int amount = 10;
		bloomProgram.Activate();
		for (unsigned int i = 0; i < amount; i++)
		{
			//cout << "horizontal? " << horizontal << endl;
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			glUniform1i(glGetUniformLocation(bloomProgram.ID, "horizontal"), horizontal);

			if (first_iteration)
			{
				//cout << "First Iteration" << endl;
				glBindTexture(GL_TEXTURE_2D, bloomTexture);
				first_iteration = false;
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
			}

			glBindVertexArray(rectVAO);
			glDisable(GL_DEPTH_TEST);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glEnable(GL_DEPTH_TEST);
			horizontal = !horizontal;
		}




		// Switch back to default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Draw framebuffer texture to rectangle
		framebufferProgram.Activate();
		glBindVertexArray(rectVAO);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, framebufferTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// swap buffers
		glfwSwapBuffers(window);

		// Process GLFW events
		glfwPollEvents();
	}


	// CLEAN UP
	// ------------------------------------------------------------------------------------------
	// Delete shader programs
	shaderProgram.Delete();

	// Close the window
	glfwDestroyWindow(window);

	// Terminate GLFW 
	glfwTerminate();

	return 0;
}