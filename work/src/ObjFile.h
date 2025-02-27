// ObjFile.h
#pragma once
// std
#include <string>
#include <vector>
// glm
#include <glm/glm.hpp>
// project
#include "opengl.hpp"


class ObjFile {
private:

	// CPU-side data
	std::vector<glm::vec3> vertices; // vertex positions
	std::vector<glm::vec3> normals; // vertex normals
	std::vector<unsigned int> indices; // indices for drawing triangles

	// GPU-side data
	GLuint vao = 0; // vertex array object, stores information about how the buffers are set up
	GLuint vbo = 0; // vertex buffer object, stores the vertex data
	GLuint nbo = 0; // normal buffer object, stores the normal data
	// do i need it?????? GLuint tbo = 0; // texture buffer object, stores the texture data
	GLuint ebo = 0; // element buffer object, stores the indices that make up primitives

	// helper function to parse face data
	void parseFace(std::istringstream& ss);
	void parseVertex(const std::string& vertex);

public:

	// constructor & destructor
	ObjFile() {}
	~ObjFile() { destroy(); }

	// take single string argument of the filepath to the.objfile, that loads the data from the file into the private variables
	bool loadOBJ(const std::string& filepath);

	// set up mesh geometry data on the OpenGL side
	void build();

	// draw the mesh
	void draw();

	// clear the mesh geometry data
	void destroy();

	// print the mesh data
	void printMeshData();

};
