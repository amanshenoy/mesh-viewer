#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <memory>

#include "shaders.h"
#include "transform.h"
#include "mesh.h"

#include "dependancies/imgui/imgui.h"
#include "dependancies/imgui/imgui_impl_glfw.h"
#include "dependancies/imgui/imgui_impl_opengl3.h"
        
#define MAINPROGRAM 
#include "globals.h" 
 

void reshape(GLFWwindow* window, int width, int height){
    window_width = width;
    window_height = height;

    int vp = width / 4; 

    current_vp_height = height; 
    current_vp_width = vp;

    glViewport(vp, 0, width - vp, height);

    float aspect = (float) (width - vp) / (float) height;
    projection = glm::perspective(glm::radians(fovy), aspect, zNear, zFar); 
    glUniformMatrix4fv(projectionPos, 1, GL_FALSE, &projection[0][0]);
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS){
        switch(key){
            case GLFW_KEY_EQUAL:
                amount++;
                std::cout << "Sensitivity set to " << amount << "\n" ;
                break;
            case GLFW_KEY_MINUS:
                amount--;
                std::cout << "Sensitivity set to " << amount << "\n" ; 
                break;
            case GLFW_KEY_ESCAPE: 
                exit(0) ;
                break ;
            case GLFW_KEY_R:  
                eye = eyeinit ; 
                up = upinit ; 
                amount = amountinit ;
                transop = view ;
                sx = sy = 1.0 ; 
                tx = ty = 0.0 ; 
                break ;   
            case GLFW_KEY_LEFT: 
                if (keyboard_mode == 1) gl::Transform::left(amount, eye,  up);
                else if (keyboard_mode == 0) tx -= amount * 0.01 ; 
                break;
            case GLFW_KEY_UP: 
                if (keyboard_mode == 1) gl::Transform::up(-amount,  eye,  up);
                else if (keyboard_mode == 0) ty += amount * 0.01 ; 
                break;
            case GLFW_KEY_RIGHT:
                if (keyboard_mode == 1) gl::Transform::left(-amount, eye,  up);
                else if (keyboard_mode == 0) tx += amount * 0.01 ; 
                break;
            case GLFW_KEY_DOWN: 
                if (keyboard_mode == 1) gl::Transform::up(amount,  eye,  up);
                else if (keyboard_mode == 0) ty -= amount * 0.01 ; 
                break;
        }
    }
}

static void custom_mouse_callback(GLFWwindow* window, float sensitivity){
    bool clicked_state = true; 
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        clicked_state = false;
    }
    if (clicked_state){
        double x;
        double y;
        glfwGetCursorPos(window, &x, &y);
        if (x > current_vp_width){
            if (previous_y_position - y > 0){
                if (mouse_mode == 1) gl::Transform::up(-amount * sensitivity,  eye,  up);
                else if (mouse_mode == 0) ty += amount * 0.005;
                previous_y_position = y;
            }
            else if(previous_y_position - y < 0){
                if (mouse_mode == 1) gl::Transform::up(amount * sensitivity,  eye,  up);
                else if (mouse_mode == 0) ty -= amount * 0.005;
                previous_y_position = y;
            }
            if (previous_x_position - x > 0){
                if (mouse_mode == 1) gl::Transform::left(amount * sensitivity,  eye,  up);
                else if (mouse_mode == 0) tx -= amount * 0.005; 
                previous_x_position = x;
            }
            else if(previous_x_position - x < 0){
                if (mouse_mode == 1) gl::Transform::left(-amount * sensitivity,  eye,  up);
                else if (mouse_mode == 0) tx += amount * 0.005;
                previous_x_position = x;
            }
        }
    }
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    sx += yoffset * 0.01;
    sy += yoffset * 0.01; 
}

void initialise_shader_and_mesh(){
    // Initialize shaders
    gl::Shader shader; 
    vertexshader = shader.init_shaders(GL_VERTEX_SHADER, "res/shaders/vertex.glsl") ;
    fragmentshader = shader.init_shaders(GL_FRAGMENT_SHADER, "res/shaders/fragment.glsl") ;
    shaderprogram = shader.init_program(vertexshader, fragmentshader) ;

    // Get uniform locations 
    lightpos = glGetUniformLocation(shaderprogram,"light_posn") ;       
    lightcol = glGetUniformLocation(shaderprogram,"light_col") ;       
    ambientcol = glGetUniformLocation(shaderprogram,"ambient") ;       
    diffusecol =  glGetUniformLocation(shaderprogram,"diffuse") ;       
    specularcol = glGetUniformLocation(shaderprogram,"specular") ;       
    emissioncol = glGetUniformLocation(shaderprogram,"emission") ;       
    shininesscol = glGetUniformLocation(shaderprogram,"shininess") ;    
    projectionPos = glGetUniformLocation(shaderprogram, "projection");
    modelviewPos = glGetUniformLocation(shaderprogram, "modelview");

    // Initialize global mesh
    mesh.generate_buffers();
    mesh.parse_and_bind();   
}

void display(float& ambient_slider, float& diffuse_slider, float& specular_slider, float& shininess_slider, bool custom_color, float& light_position, float& light_color) {
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    modelview = glm::lookAt(eye,center,up); 
    glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);

    glUniform4fv(lightpos, numLights, &light_position);
    glUniform4fv(lightcol, numLights, &light_color);  

    // Transformations for objects, involving translation and scaling 
    mat4 sc(1.0) , tr(1.0), transf(1.0); 
    sc = gl::Transform::scale(sx,sy,1.0); 
    tr = gl::Transform::translate(tx,ty,0.0);   
    modelview = tr * sc * modelview;

    if (!custom_color){
        *(&ambient_slider + 1) = ambient_slider;
        *(&ambient_slider + 2) = ambient_slider; 

        *(&diffuse_slider + 1) = diffuse_slider;
        *(&diffuse_slider + 2) = diffuse_slider; 

        *(&specular_slider + 1) = specular_slider;
        *(&specular_slider + 2) = specular_slider; 
    }

    glUniform4fv(ambientcol, 1, &ambient_slider);
    glUniform4fv(diffusecol, 1, &diffuse_slider);
    glUniform4fv(specularcol, 1, &specular_slider);
    glUniform1f(shininesscol, shininess_slider);

    const GLfloat white[4] = {1, 1, 1, 1};
    const GLfloat black[4] = {0, 0, 0, 0};

    glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview * glm::scale(mat4(1.0f), vec3(2, 2, 2)))[0][0]);
    mesh.bind();
    if (render_mode == 0){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, mesh.objectIndices.size(), GL_UNSIGNED_INT, 0);
    }
    if (render_mode == 1){
        glLineWidth(1); 
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, mesh.objectIndices.size(), GL_UNSIGNED_INT, 0);
    } 
    if (render_mode == 2){
        glPointSize(2.5); 
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glDrawElements(GL_TRIANGLES, mesh.objectIndices.size(), GL_UNSIGNED_INT, 0);
    } 
    if (render_mode == 3){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, mesh.objectIndices.size(), GL_UNSIGNED_INT, 0);
        glUniform4fv(diffusecol, 1, black);
        glUniform4fv(specularcol, 1, white);

        glPointSize(2.5); 
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glDrawElements(GL_TRIANGLES, mesh.objectIndices.size(), GL_UNSIGNED_INT, 0);

        glLineWidth(2.5); 
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, mesh.objectIndices.size(), GL_UNSIGNED_INT, 0);
    } 
    glBindVertexArray(0);
}

int main(int argc, char* argv[]){
    // Initialise GLFW and GLEW; and parse path from command line  
    GLFWwindow* window;
    mesh.object_path = argv[1]; 

    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    window = glfwCreateWindow(1920, 1080, "Scene Viewer", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) std::cout << "There is a problem\n ";
    std::cout << glGetString(GL_VERSION) << "\n"; 

    // Initialize ImGui 
    const char* glsl_version = "#version 330";
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true); 
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::StyleColorsDark(); 

    // Initialise all variable initial values
    initialise_shader_and_mesh();
    eye = (eyeinit); 
    up = (upinit); 
    amount = amountinit;
    sx = sy = 1.0;   
    tx = ty = 0.0;  

    glEnable(GL_DEPTH_TEST);
    glfwSetWindowSizeCallback(window, reshape);

    glfwSetKeyCallback(window, keyboard);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    
    float ambient[4] = {0.05, 0.05, 0.05, 1};
    float diffusion[4] = {0.2, 0.2, 0.2, 1};
    float specular[4] = {0.5, 0.5, 0.5, 1};
    float light_positions[20] = {0.0f};
    float light_colors[20] = {0.0f};
    float shininess = 10; 
    bool custom_color = false; 
    
    light_positions[1] = 6.5f;
    light_colors[0] = 1.0f;
    light_colors[1] = 1.0f;
    light_colors[2] = 1.0f;

    previous_y_position = 0;
    previous_x_position = 0;

    current_vp_width = 1920/4;
    current_vp_height = 1080; 

    while (!glfwWindowShouldClose(window)){
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos({0, 0});
        ImGui::SetNextWindowSize({current_vp_width, current_vp_height}); 

        ////////////////////////////////////////////////////////////////////////////////////////////////

        ImGui::Begin("Object Properties"); 
        ImGui::Text("Application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text(" ");

        ////////////////////////////////////////////////////////////////////////////////////////////////        

        ImGui::Separator(); ImGui::TextColored({0.0f,1.0f,1.0f,1.0f}, "Render Mode"); ImGui::Separator();
        // ImGui::Text("Primitive object "); ImGui::SameLine();
        ImGui::RadioButton("Smooth", &render_mode, 0); ImGui::SameLine();
        ImGui::RadioButton("Lines", &render_mode, 1); ImGui::SameLine();
        ImGui::RadioButton("Point Cloud", &render_mode, 2); ImGui::SameLine();
        ImGui::RadioButton("Mesh", &render_mode, 3); 

        ////////////////////////////////////////////////////////////////////////////////////////////////        
        
        ImGui::Separator(); ImGui::TextColored({0.0f,1.0f,1.0f,1.0f}, "Object Properties"); ImGui::Separator();
        ImGui::Text("These set of boxes and sliders change the way the object reacts");
        ImGui::Text("with light giving us the impression of its material. The initial"); 
        ImGui::Text("sliders assume the object stays white, and the checkbox 'Custom");
        ImGui::Text("Colors' allows us to change the associated colors"); 
        ImGui::Separator();

        ImGui::Checkbox("Custom Colors", &custom_color); 
        if (custom_color){
            ImGui::SliderFloat3("Ambient R, G, B", &ambient[0], 0.0f, 1.0f);
            ImGui::SliderFloat3("Diffusion R, G, B", &diffusion[0], 0.0f, 1.0f);
            ImGui::SliderFloat3("Specular R, G, B", &specular[0], 0.0f, 1.0f);
            ImGui::SliderFloat("Shininess", &shininess, 0.0f, 150.0f);
        }
        if (!custom_color){
            ImGui::SliderFloat("Ambient", &ambient[0], 0.0f, 1.0f);
            ImGui::SliderFloat("Diffusion", &diffusion[0], 0.0f, 1.0f);
            ImGui::SliderFloat("Specular", &specular[0], 0.0f, 1.0f);         
            ImGui::SliderFloat("Shininess", &shininess, 0.0f, 150.0f);
        }
        ImGui::Text(" ");

        ////////////////////////////////////////////////////////////////////////////////////////////////
        
        ImGui::Separator(); ImGui::TextColored({0.0f,1.0f,1.0f,1.0f}, "Input Devices"); ImGui::Separator();
        ImGui::Text("Decide which input device associates to which transformation.");
        ImGui::Text("By default the scroll wheel is associated with scaling"); 
        ImGui::Separator();

        ImGui::Text("Use mouse to "); ImGui::SameLine();
        ImGui::RadioButton("translate", &mouse_mode, 0); ImGui::SameLine();
        ImGui::RadioButton("rotate", &mouse_mode, 1);

        ImGui::Text("Using keyboard to"); ImGui::SameLine();
        ImGui::RadioButton("translate ", &keyboard_mode, 0); ImGui::SameLine();
        ImGui::RadioButton("rotate ", &keyboard_mode, 1);

        keyboard_mode = !(mouse_mode);
        
        ////////////////////////////////////////////////////////////////////////////////////////////////

        ImGui::Text(" ");
        reshape(window, window_width, window_height);
        ImGui::Separator(); ImGui::TextColored({0.0f,1.0f,1.0f,1.0f}, "Camera"); ImGui::Separator(); 
        ImGui::SliderFloat("Field of view", &fovy, 0.0f, 180.0f);
        ImGui::SliderFloat("Frustum near plane", &zNear, 0.0f, 15.0f);
        ImGui::SliderFloat("Frustum far plane", &zFar, 0.0f, 150.0f);
        ImGui::Text(" ");
        ImGui::SliderFloat3("Camera position ", &eye[0], -10.0f, 10.0f);

        ////////////////////////////////////////////////////////////////////////////////////////////////
        
        ImGui::Separator(); ImGui::TextColored({0.0f,1.0f,1.0f,1.0f}, "Lighting"); ImGui::Separator();
        ImGui::Text("Most lights are switched off by default, and the below");
        ImGui::Text("sliders can play with the light positions and color intensities"); 

        for (int i = 0; i < numLights; i++){
            std::string light_name = std::string("Light ") + std::to_string(i + 1); 
            ImGui::Separator(); ImGui::Text("Light"); ImGui::Separator(); 
            ImGui::SliderFloat3((light_name + " positions").c_str(), &light_positions[4 * i], -25.0f, 25.0f);
            ImGui::SliderFloat3((light_name + " colors").c_str(), &light_colors[4 * i], 0.0f, 1.0f);

            light_positions[(4 * i) + 3] = 1.0f;
            light_colors[(4 * i) + 3] = 1.0f;
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////
        
        display(*ambient, *diffusion, *specular, shininess, custom_color, *light_positions, *light_colors);
        
        ////////////////////////////////////////////////////////////////////////////////////////////////
        
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ////////////////////////////////////////////////////////////////////////////////////////////////

        glfwSwapBuffers(window);          
        glfwSetScrollCallback(window, scroll_callback); 
        custom_mouse_callback(window, 0.25);
        glfwPollEvents();
    }
    mesh.destroy_buffers();
    glfwTerminate();
    return 0;
}