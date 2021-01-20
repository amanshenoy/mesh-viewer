#include <iostream>
#include <string>

namespace gl {
class Shader{
public:
    std::string read_text_file(const char * filename);

    void program_errors (const GLint program);
    void shader_errors (const GLint shader);
    
    GLuint init_shaders (GLenum type, const char * filename);
    GLuint init_program (GLuint vertexshader, GLuint fragmentshader);
};
}