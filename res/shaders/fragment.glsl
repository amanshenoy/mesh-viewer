# version 330 core

// Inputs passed in from the vertex shader
in vec3 mynormal; 
in vec4 myvertex; 

// Max number of light sources 
const int num_lights = 5;

// Output of the fragment shader
out vec4 fragColor;

// Uniform variable modelview 
uniform mat4 modelview;

// Uniform variables to do with lighting 
uniform vec4 light_posn[num_lights];
uniform vec4 light_col[num_lights];

// Uniform variable for object properties
uniform vec4 ambient; 
uniform vec4 diffuse; 
uniform vec4 specular;  
uniform float shininess; 

vec4 compute_lighting(vec3 direction, vec4 lightcolor, vec3 normal, vec3 halfvec, vec4 mydiffuse, vec4 myspecular, float myshininess){
        float n_dot_l = dot(normal, direction);         
        vec4 lambert = mydiffuse * lightcolor * max (n_dot_l, 0.0);  

        float n_dot_h = dot(normal, halfvec); 
        vec4 phong = myspecular * lightcolor * pow (max(n_dot_h, 0.0), myshininess); 

        vec4 lighting = lambert + phong; 
        return lighting;
}       

void main (void){
    // Start with black, and add ambient        
    vec4 finalcolor = vec4(0.0f); 
    finalcolor += ambient; 

    // Calculate camera direction
    const vec3 eyepos = vec3(0,0,0); 
    vec3 mypos = myvertex.xyz / myvertex.w;  
    vec3 eyedirn = normalize(eyepos - mypos); 

    // Normalise the normal at that point 
    vec3 normal = normalize(mynormal); 

    // Iterating through each light source (on or off)
    for (int i = 0; i < num_lights; i++){ 
        vec4 light_position = inverse(modelview) * light_posn[i];
        vec3 position = light_position.xyz / light_position.w; 
        vec3 direction = normalize(position - mypos); 
        vec3 half_i = normalize(direction + eyedirn);
        vec4 light_color = light_col[i];  
        
        // Compute light based on the normal at the point and the light direction
        vec4 col = compute_lighting(direction, light_color, normal, half_i, diffuse, specular, shininess);
        finalcolor += col;    
    }
    fragColor = finalcolor;         
}