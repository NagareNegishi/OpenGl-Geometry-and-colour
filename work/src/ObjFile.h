// ObjFile.h
#pragma once
// std
#include <string>
#include <vector>
#include <sstream>
// glm
#include <glm/glm.hpp>
// project
#include "opengl.hpp"

// store combined vertex data
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
};

class ObjFile {
private:
	// CPU-side data
	std::vector<glm::vec3> vertices; // vertex positions
	std::vector<glm::vec3> normals; // vertex normals
	std::vector<unsigned int> indices; // indices for drawing triangles
	std::vector<unsigned int> textureIndices; // indices for texture coordinates
	std::vector<unsigned int> normalIndices; // indices for normals
	std::vector<unsigned int> drawIndices;  // indices for OpenGL drawing
	std::vector<Vertex> meshVertices; // processed vertices with aligned position and normal

	// GPU-side data
	GLuint vao = 0; // vertex array object, stores information about how the buffers are set up
	GLuint vbo = 0; // vertex buffer object, stores the vertex data
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
