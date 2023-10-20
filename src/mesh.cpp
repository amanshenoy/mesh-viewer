#include <memory>
#include <iostream>

#include "mesh.h"
#include "transform.h"
#include "globals.h"

namespace gl {
void Mesh::generate_buffers(){
	glGenVertexArrays(1, &vertex_array);
	glGenBuffers(1, &vertex_buffer);
	glGenBuffers(1, &normal_buffer);
	glGenBuffers(1, &index_buffer);
}

void Mesh::destroy_buffers(){
	glDeleteVertexArrays(1, &vertex_array);
	glDeleteBuffers(1, &vertex_buffer);
	glDeleteBuffers(1, &normal_buffer);
	glDeleteBuffers(1, &index_buffer);
}

void Mesh::parse_and_bind(){
	FILE* fp;
	// fp >> 
	float minY = INFINITY, minZ = INFINITY;
	float maxY = -INFINITY, maxZ = -INFINITY;
	// judge the extension
	std::string extension = object_path.substr(object_path.find_last_of(".") + 1);
	fp = fopen(object_path.c_str(), "rb");
	// Error loading file
	if (fp == NULL) {
		std::cerr << "Error loading file: " << object_path << "; probably does not exist" << std::endl;
		exit(-1);
	}
	// open the file with obj extension
	if (extension == "obj")
	{
		float x, y, z;
		int fx, fy, fz, ignore;
		int c1, c2;
		
		//
		while (!feof(fp)) { // This loop continues until the end of the file (EOF) is reached.
			c1 = fgetc(fp); // get the first character of the line
			while (!(c1 == 'v' || c1 == 'f')) {
				c1 = fgetc(fp);
				if (feof(fp))
					break;
			}
			// c2 == 'v' or c2 == 'f'
			c2 = fgetc(fp);
			if ((c1 == 'v') && (c2 == ' ')) {
				// scan the 3 floating number and scan it to x,y and z.
				fscanf(fp, "%f %f %f", &x, &y, &z);
				objectVertices.push_back(vec3(x, y, z));
				if (y < minY) minY = y;
				if (z < minZ) minZ = z;
				if (y > maxY) maxY = y;
				if (z > maxZ) maxZ = z;
			}
			else if ((c1 == 'v') && (c2 == 'n')) {
				// the normal information 
				fscanf(fp, "%f %f %f", &x, &y, &z);
				objectNormals.push_back(glm::normalize(vec3(x, y, z)));
			}
			else if (c1 == 'f') {
				// the face information, depends on the format of the obj file.
				fscanf(fp, "%d//%d %d//%d %d//%d", &fx, &ignore, &fy, &ignore, &fz, &ignore);
				objectIndices.push_back(fx - 1);
				objectIndices.push_back(fy - 1);
				objectIndices.push_back(fz - 1);
			}
		}
	}
	// mesh extension
	else if (extension == "mesh" || extension == "gmsh")
	{
		int numVertices, numTriangles;
		//int vertexId, faceId;
		float x, y, z;
		char keyword[50]; // for section keywords

		// Skip headers until Vertices
		while (fscanf(fp, "%s", keyword) && strcmp(keyword, "Vertices") != 0) {}

		fscanf(fp, "%d", &numVertices);
		for (int i = 0; i < numVertices; ++i) {
			fscanf(fp, " %f %f %f", &x, &y, &z);
			objectVertices.push_back(vec3(x, y, z));
			float normal_x = 0.0;
			float normal_y = 0.0;
			float normal_z = 1.0;
			// here I assume every vertex has normal (0,0,1), the actual normal where be computed after introduced the openmesh structure
			objectNormals.push_back(glm::normalize(vec3(normal_x, normal_y, normal_z)));
			if (y < minY) minY = y;
			if (z < minZ) minZ = z;
			if (y > maxY) maxY = y;
			if (z > maxZ) maxZ = z;
		}

		// Skip sections until Triangles
		while (fscanf(fp, "%s", keyword) && strcmp(keyword, "Triangles") != 0) {
			continue;
		}

		fscanf(fp, "%d", &numTriangles);
		int v1, v2, v3;
		for (int i = 0; i < numTriangles; ++i) {
			fscanf(fp, "% d % d % d", &v1, &v2, &v3);
			objectIndices.push_back(v1 - 1); // Assuming indices in the mesh file are 1-based
			objectIndices.push_back(v2 - 1);
			objectIndices.push_back(v3 - 1);
		}
	}
	else {
		std::cerr << "Unsupported file format: " << extension << std::endl;
		exit(-1);
	}
	fclose(fp); // close the document
	// adjust the vertex position 
	float avgY = (minY + maxY) / 2.0f - 0.02f;
	float avgZ = (minZ + maxZ) / 2.0f;
	for (unsigned int i = 0; i < objectVertices.size(); ++i) {
		vec3 shiftedVertex = (objectVertices[i] - vec3(0.0f, avgY, avgZ)) * vec3(1.58f, 1.58f, 1.58f);
		objectVertices[i] = shiftedVertex;
	}

	glBindVertexArray(vertex_array);

	// Bind vertices to layout location 0
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer );
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * objectVertices.size(), &objectVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // This allows usage of layout location 0 in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind normals to layout location 1
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer );
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * objectNormals.size(), &objectNormals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // This allows usage of layout location 1 in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * objectIndices.size(), &objectIndices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

}