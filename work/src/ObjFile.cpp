// ObjFile.cpp
#include "ObjFile.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace glm; // OpenGL Mathematics, for vec3 

/*
* take single string argument of the filepath to the.objfile, that loads the data from the file into the private variables
*
* Let the file name be a string specified in the ImGui input controls -> 
* in application.cpp -> renderGUI() -> InputText() is taking the file name as input
*/
bool ObjFile::loadOBJ(const std::string& filepath) { // const for read-only
	// clear existing data
	vertices.clear();
	normals.clear();
	indices.clear();

	// open the file
	ifstream fsIn;
	fsIn.open(filepath, ios::in);
	if (!fsIn.is_open()) {
		cerr << "Error: Unable to open file" << filepath << endl;
		return false;
	}

	// read file, ObjFile should be formatted, so use formatted I/O
	string line;
	while (getline(fsIn, line)) {
		// skip empty lines and comments
		if (line.empty() || line[0] == '#') {
			continue;
		}

		istringstream ss(line); // create a string stream from the line
		string type; // check OBJ format
		ss >> type; // read the first word from the line

		if (type == "v") { // vertex position
			vec3 v;
			ss >> v.x >> v.y >> v.z; // read the three floats to the vec3
			vertices.push_back(v); // add the vertex to the list
		}
		else if (type == "vn") { // vertex normal
			vec3 n;
			ss >> n.x >> n.y >> n.z;
			normals.push_back(n);
		}
		else if (type == "f") { // face
			parseFace(ss);
		}
		// ignore other line types
	}
	// close the file
	fsIn.close();
	// if all lists are not empty, return true
	return !vertices.empty() && !normals.empty() && !indices.empty();
}

// helper function to parse faces, only expecting triangles!!
// read the three vertex and parse them
void ObjFile::parseFace(std::istringstream& ss) {
	string v1, v2, v3;
	ss >> v1 >> v2 >> v3;
	parseVertex(v1);
	parseVertex(v2);
	parseVertex(v3);
}

// helper function to parse vertex data
void ObjFile::parseVertex(const std::string& vertex) {
	// format is "v/vt/vn", but only need vertex position
	stringstream ss(vertex);
	string v;
	getline(ss, v, '/');
	// obj file indexes from 1, so subtract 1
	if (!v.empty()) {
		indices.push_back(stoi(v) - 1);
	}
}

// set up mesh geometry data on the OpenGL side
void ObjFile::build() {
	if (vao == 0) {
		// generate buffers
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &nbo);
		glGenBuffers(1, &ebo);

		// bind the VAO
		glBindVertexArray(vao);

		// bind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo); // GL_ARRAY_BUFFER is binding point
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW); // upload the vertex data
		glEnableVertexAttribArray(0); // set up the vertex attribute, attribute 0 is usually the vertex position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr); // tell OpenGL how to interpret the data (3 floats per vertex)

		// bind the NBO
		glBindBuffer(GL_ARRAY_BUFFER, nbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1); // set up the vertex attribute, attribute 1 is usually the vertex normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr); // tell OpenGL how to interpret the data (3 floats per normal)

		// bind the EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // GL_ELEMENT_ARRAY_BUFFER is binding point
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW); // upload the indices

		// unbind the VAO
		glBindVertexArray(0);
	}
}

// draw the mesh
void ObjFile::draw() {
	if (vao == 0) return;
	glBindVertexArray(vao); // bind our VAO which sets up all our buffers and data for us
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); // tell opengl to draw our VAO using the draw mode and how many verticies to render
	glBindVertexArray(0); // unbind the VAO
}
// clear the mesh geometry data
void ObjFile::destroy() {
	if (vao != 0) {
		// delete the data buffers
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &nbo);
		glDeleteBuffers(1, &ebo);
		// reset the variables
		vao = 0;
		vbo = 0;
		nbo = 0;
		ebo = 0;
		// clear the CPU-side data (may not nessesary?)
		vertices.clear();
		normals.clear();
		indices.clear();
	}
}


/*

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!ask!!!!!!!!!!!!!!!!!!!!!!


*/
// prints out the current raw mesh data in the console. Link the printMeshData() function to a button in ImGui
// printing out every vertex, normal and index, may be better to limit the output to the first few
// but it is dependent on the requirements of the project
void ObjFile::printMeshData() {
	cout << "Vertices: " << vertices.size() << endl;
	for (const auto& v : vertices) {
		cout << "v " << v.x << " " << v.y << " " << v.z << endl;
	}
	cout << "Normals: " << normals.size() << endl;
	for (const auto& n : normals) {
		cout << "vn " << n.x << " " << n.y << " " << n.z << endl;
	}
	cout << "Indices: " << indices.size() << endl;
	for (size_t i = 0; i < indices.size(); i += 3) {
		cout << "f " << indices[i] << " " << indices[i + 1] << " " << indices[i + 2] << endl;
	}
}



