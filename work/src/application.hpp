
#pragma once

// glm
#include <glm/glm.hpp>

// project
#include "opengl.hpp"

// class to load and draw an obj file
#include "objfile.h"


// Main application class
class Application {
private:
	// window
	glm::vec2 m_windowsize;
	GLFWwindow *m_window;

	// basic shader
	GLuint m_shader;

	ObjFile m_model; // model to load and draw
	glm::vec3 m_modelColor = glm::vec3(1.0f, 1.0f, 1.0f); // white as default
	glm::vec3 m_lightDirection = glm::vec3(0.0f, -1.0f, -1.0f); // For directional light

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