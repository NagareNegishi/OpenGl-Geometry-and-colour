
#pragma once

// glm
#include <glm/glm.hpp>

// project
#include "opengl.hpp"

// replace the triangle model with the obj file model
#include "ObjFile.h"
//#include "triangle.hpp"

// Main application class
//
class Application {
private:
	// window
	glm::vec2 m_windowsize;
	GLFWwindow *m_window;

	// basic shader
	GLuint m_shader;

	// replace the triangle model with the obj file model
	ObjFile m_model;
	// test triangle model
	// TestTriangle m_model;

	/* !!!!!!!!  addition here  !!!!!!!*/
	glm::vec3 m_modelColor = glm::vec3(1.0f, 1.0f, 1.0f); // white as default

	/* light source properties */
	glm::vec3 m_lightDirection = glm::vec3(0.0f, -1.0f, -1.0f); // For directional light
	glm::vec3 m_lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	// Phong model properties
	float m_ambient = 0.1f;
	float m_diffuse = 0.7f;
	float m_specular = 0.2f;
	float m_shininess = 32.0f;


public:
	// setup
	Application(GLFWwindow *);

	// disable copy constructors (for safety)
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	// rendering callbacks (every frame)
	void render();
	void renderGUI();

	// input callbacks
	void cursorPosCallback(double xpos, double ypos);
	void mouseButtonCallback(int button, int action, int mods);
	void scrollCallback(double xoffset, double yoffset);
	void keyCallback(int key, int scancode, int action, int mods);
	void charCallback(unsigned int c);
};