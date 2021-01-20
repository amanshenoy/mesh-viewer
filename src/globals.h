#ifdef MAINPROGRAM 
    #define EXTERN 
#else 
    #define EXTERN extern 
#endif

EXTERN int amount;
EXTERN vec3 eye;  
EXTERN vec3 up;  

#ifdef MAINPROGRAM 
    vec3 eyeinit(0.0,0.0,5.0);
    vec3 upinit(0.0,1.0,0.0); 
    vec3 center(0.0,0.0,0.0);  
    int mouse_mode = 1; 
    int keyboard_mode = 0; 
    int window_width = 1920; 
    int window_height = 1080; 
    int amountinit = 5;
    float fovy = 90.0f;
    float zFar = 99.0f;
    float zNear = 0.1f;
#else 
    EXTERN vec3 eyeinit; 
    EXTERN vec3 upinit; 
    EXTERN vec3 center; 
    EXTERN int amountinit;
    EXTERN int w, h; 
    EXTERN float fovy; 
#endif 

EXTERN GLuint vertexshader, fragmentshader, shaderprogram; 
EXTERN mat4 projection, modelview; 
EXTERN GLuint projectionPos, modelviewPos;
static enum{view,  translate, scale} transop; 
EXTERN float sx, sy; 
EXTERN float tx, ty; 

// Globals regarding lighting details 
const int numLights = 5; 
EXTERN GLfloat lightposn [4*numLights];
EXTERN GLfloat lightcolor[4*numLights];
EXTERN GLfloat lightransf[4*numLights];

// Variables to set uniform params for lighting fragment shader 
EXTERN GLuint lightcol; 
EXTERN GLuint lightpos; 
EXTERN GLuint numusedcol; 
EXTERN GLuint enablelighting; 
EXTERN GLuint ambientcol; 
EXTERN GLuint diffusecol; 
EXTERN GLuint specularcol; 
EXTERN GLuint emissioncol; 
EXTERN GLuint shininesscol; 

// Callback and reshape globals 
EXTERN int render_mode;
EXTERN int previous_y_position;
EXTERN int previous_x_position;
EXTERN float current_vp_width, current_vp_height;

// Global mesh
EXTERN gl::Mesh mesh;