
// std
#include <iostream>
#include <string>
#include <chrono>

// glm
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "application.hpp"
#include "cgra/cgra_gui.hpp"
#include "cgra/cgra_shader.hpp"


using namespace std;
using namespace cgra;
using namespace glm;


// constructor & build the shader 
Application::Application(GLFWwindow *window) : m_window(window) {
	// build the shader
	shader_builder color_sb;
	color_sb.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//default_vert.glsl")); 
	color_sb.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//default_frag.glsl"));
	m_shader = color_sb.build();
}

// draw the model
void Application::render() {
	
	// retrieve the window hieght
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height); 
	m_windowsize = vec2(width, height); // update window size
	glViewport(0, 0, width, height); // set the viewport to draw to the entire window

	// clear the back-buffer
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	// enable flags for normal/forward rendering
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS);

	// calculate the projection and view matrix
	mat4 proj = perspective(1.f, float(width) / height, 0.1f, 1000.f);
	mat4 view = translate(mat4(1), vec3(0, -5, -20));

	// set shader and upload variables
	glUseProgram(m_shader);
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "uModelViewMatrix"), 1, false, value_ptr(view));

	// set the model color
	glUniform3fv(glGetUniformLocation(m_shader, "uColor"), 1, value_ptr(m_modelColor));
	/*
	* note for me:
	* glUniform3fv(location, count, value)
	* location: the location of the uniform variable to be modified -> color
	* count: the number of vec3 -> single color -> 1
	* value: pointer to the first element -> m_modelColor
	*/
	
	// set the directional light properties
	vec3 normalLightDir = normalize(m_lightDirection);
	glUniform3fv(glGetUniformLocation(m_shader, "uLightDirection"), 1, value_ptr(normalLightDir));

	// draw the model
	m_model.draw();
}

// render the GUI
void Application::renderGUI() {

	// setup window
	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(500, 170), ImGuiSetCond_Once);
	ImGui::Begin("Mesh loader", 0);

	// Loading buttons
	static char filename[512] = "";
	// if the filename is empty, set the default path
	if (filename[0] == '\0') {
		string defaultPath = CGRA_SRCDIR + string("//res//assets//teapot.obj");
		strcpy(filename, defaultPath.c_str());
	}

	ImGui::InputText("", filename, 512);
	ImGui::SameLine();
	if (ImGui::Button("Load")) {
		// load mesh from 'filename'
		if (m_model.loadOBJ(filename)) {
			m_model.build();
		}
		else {
			cout << "Error: Unable to load model" << endl;
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Print")) {
		// print mesh data
		m_model.printMeshData();
	}

	ImGui::SameLine();
	if (ImGui::Button("Unload")) {
		// unload mesh
		m_model.destroy();
	}

	// Color picker
	ImGui::ColorEdit3("Model Color", glm::value_ptr(m_modelColor));

	// Directional light properties
	ImGui::Separator();
	ImGui::Text("Light Direction");
	ImGui::SliderFloat("X", &m_lightDirection.x, -1.0f, 1.0f);
	ImGui::SliderFloat("Y", &m_lightDirection.y, -1.0f, 1.0f);
	ImGui::SliderFloat("Z", &m_lightDirection.z, -1.0f, 1.0f);

	// finish creating window
	ImGui::End();
}


void Application::cursorPosCallback(double xpos, double ypos) {
	(void)xpos, ypos; // currently un-used
}


void Application::mouseButtonCallback(int button, int action, int mods) {
	(void)button, action, mods; // currently un-used
}


void Application::scrollCallback(double xoffset, double yoffset) {
	(void)xoffset, yoffset; // currently un-used
}


void Application::keyCallback(int key, int scancode, int action, int mods) {
	(void)key, (void)scancode, (void)action, (void)mods; // currently un-used
}


void Application::charCallback(unsigned int c) {
	(void)c; // currently un-used
}