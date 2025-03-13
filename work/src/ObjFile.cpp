// ObjFile.cpp
#include "ObjFile.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> // Add this include for std::min

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
	textureIndices.clear();
	normalIndices.clear();
	drawIndices.clear();
	meshVertices.clear();


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
// texture indices are only collected for printing, not used in the mesh
void ObjFile::parseVertex(const std::string& vertex) {
	// format is "v/vt/vn"
	stringstream ss(vertex);
	string v, vt, vn;
	getline(ss, v, '/');
	getline(ss, vt, '/');
	getline(ss, vn, '/');
	// obj file indexes from 1, so subtract 1
	if (!v.empty()) {
		indices.push_back(stoi(v) - 1);
	}
	if (!vt.empty()) {
		textureIndices.push_back(stoi(vt) - 1);
	}
	if (!vn.empty()) {
		normalIndices.push_back(stoi(vn) - 1);
	}
}

/*
* must be called after the loadOBJ() function, to build the mesh data
* create the mesh data from the raw data, and store it in the GPU
*/
void ObjFile::build() {
	if (vao == 0) {
		// Create triangles from the original indices (Each triangle has 3 vertices, but possibly with different normals)
		for (auto i = 0; i < indices.size(); i++) {
			Vertex vertex;
			// Get the current vertex index (to tell which vertex to use)
			unsigned int vertexIndex = indices[i];
			vertex.position = vertices[vertexIndex]; // Set the vertex position
			unsigned int normalIndex;
			// Get the normal index (if it exists) or use the vertex index
			if (i < normalIndices.size()) {
				normalIndex = normalIndices[i];
			} else {
				normalIndex = vertexIndex;
			}
			vertex.normal = normals[normalIndex]; // Set the vertex normal
			meshVertices.push_back(vertex); // Add the vertex to our mesh
		}
		// Create a list of indices for drawing
		drawIndices.resize(meshVertices.size());
		for (auto i = 0; i < meshVertices.size(); i++) {
			drawIndices[i] = i;
		}

		// Generate buffers
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		// bind the VAO
		glBindVertexArray(vao);

		// bind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * meshVertices.size(), meshVertices.data(), GL_STATIC_DRAW);

		// set the vertex and normal attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3))); // offset by the size of the position

		// bind the EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * drawIndices.size(), drawIndices.data(), GL_STATIC_DRAW);

		// Unbind the VAO
		glBindVertexArray(0);
	}
}

/*
* draw the mesh data, must be called after the build() function
*/
void ObjFile::draw() {
	if (vao == 0) return;
	glBindVertexArray(vao); // bind our VAO which sets up all our buffers and data for us
	glDrawElements(GL_TRIANGLES, drawIndices.size(), GL_UNSIGNED_INT, 0); // tell opengl to draw our VAO using the draw mode and how many verticies to render
	glBindVertexArray(0); // unbind the VAO
}

// clear the mesh geometry data
void ObjFile::destroy() {
	if (vao != 0) {
		// delete the data buffers
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		// reset the variables
		vao = 0;
		vbo = 0;
		ebo = 0;
		// clear the CPU-side data (may not nessesary?)
		vertices.clear();
		normals.clear();
		indices.clear();
		textureIndices.clear();
		normalIndices.clear();
		drawIndices.clear();
		meshVertices.clear();
	}
}

/*
* prints out the current raw mesh data in the console.
* faces are reconstructed to the original format
* commented out version is limited to 10 vertices for faster debugging
*/
void ObjFile::printMeshData() {
	cout << "Vertices: " << vertices.size() << endl;
	for (const auto& v : vertices) {
		cout << "v " << v.x << " " << v.y << " " << v.z << endl;
	}
	cout << "Normals: " << normals.size() << endl;
	for (const auto& n : normals) {
		cout << "vn " << n.x << " " << n.y << " " << n.z << endl;
	}
	// reconstruct the faces from the indices
	if (indices.size() != textureIndices.size() || indices.size() != normalIndices.size()) {
		cerr << "Error: Indices, texture indices, and normal indices do not match" << endl;
		return;
	}
	if (indices.size() % 3 != 0) {
		cerr << "Error: Only triangles are supported" << endl;
		return;
	}
	cout << "Faces: " << indices.size() / 3 << endl; // a line is 3 sets of indices
	for (auto i = 0; i < indices.size(); i += 3) { // +1 to convert to 1-based index
		cout << "f " << indices[i] + 1 << "/" << textureIndices[i] + 1 << "/" << normalIndices[i] + 1 << " ";
		cout << indices[i + 1] + 1 << "/" << textureIndices[i + 1] + 1 << "/" << normalIndices[i + 1] + 1 << " ";
		cout << indices[i + 2] + 1 << "/" << textureIndices[i + 2] + 1 << "/" << normalIndices[i + 2] + 1 << endl;
	}

	// limit the output to 10 for faster debugging
	// 
	//cout << "Vertices: " << vertices.size() << endl;
	//for (size_t i = 0; i < vertices.size() && i < 10; i++) {
	//	cout << "v " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << endl;
	//}
	//cout << "Normals: " << normals.size() << endl;
	//for (size_t i = 0; i < normals.size() && i < 10; i++) {
	//	cout << "vn " << normals[i].x << " " << normals[i].y << " " << normals[i].z << endl;
	//}
	//cout << "Faces: " << indices.size() / 3 << endl;
	//for (size_t i = 0; i < indices.size() && i < 30; i += 3) {
	//	cout << "f " << indices[i] + 1 << "/" << textureIndices[i] + 1 << "/" << normalIndices[i] + 1 << " ";
	//	cout << indices[i + 1] + 1 << "/" << textureIndices[i + 1] + 1 << "/" << normalIndices[i + 1] + 1 << " ";
	//	cout << indices[i + 2] + 1 << "/" << textureIndices[i + 2] + 1 << "/" << normalIndices[i + 2] + 1 << endl;
	//}
}



