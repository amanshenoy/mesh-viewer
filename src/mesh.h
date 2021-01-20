#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

namespace gl {
class Mesh{
private:
    GLuint vertex_array, vertex_buffer, normal_buffer, index_buffer;
public:
    std::string object_path; 

    std::vector <glm::vec3> objectVertices;
    std::vector <glm::vec3> objectNormals;
    std::vector <unsigned int> objectIndices;    

    void generate_buffers();
    void destroy_buffers();
    void parse_and_bind();
    inline void bind(){glBindVertexArray(vertex_array);}
};
}