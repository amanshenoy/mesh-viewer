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
	float x, y, z;
	int fx, fy, fz, ignore;
	int c1, c2;
	float minY = INFINITY, minZ = INFINITY;
	float maxY = -INFINITY, maxZ = -INFINITY;

	fp = fopen(object_path.c_str(), "rb");
	if (fp == NULL){
		std::cerr << "Error loading file: " << object_path << "; probably does not exist" << std::endl;
		exit(-1);
	}

	while (!feof(fp)){
		c1 = fgetc(fp);
		while (!(c1 == 'v' || c1 == 'f')) {
			c1 = fgetc(fp);
			if (feof(fp))
				break;
		}
		c2 = fgetc(fp);
		if ((c1 == 'v') && (c2 == ' ')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			objectVertices.push_back(vec3(x, y, z));
			if (y < minY) minY = y;
			if (z < minZ) minZ = z;
			if (y > maxY) maxY = y;
			if (z > maxZ) maxZ = z;
		}
		else if ((c1 == 'v') && (c2 == 'n')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			objectNormals.push_back(glm::normalize(vec3(x, y, z)));
		}
		else if (c1 == 'f'){
			fscanf(fp, "%d//%d %d//%d %d//%d", &fx, &ignore, &fy, &ignore, &fz, &ignore);
			objectIndices.push_back(fx - 1);
			objectIndices.push_back(fy - 1);
			objectIndices.push_back(fz - 1);
		}
	}
	fclose(fp); 
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