# Object / Mesh Viewer

The following repository contains the implementation of a basic mesh/object viewer for `.obj` files. The project has been implemented in C++ using OpenGL and GLSL. This project was largely pursued to be able to get familiar with OpenGL and GLSL. The project uses the typical OpenGL rendering pipeline and uses the phong lighting model for lighting calculations.   

## Usage

A sample binary has been uploaded with the repo (compiled on x86, as a 64 bit application), but its unlikely that it would generally work even on a system with the same configuration (try anyway, it just might). 

To compile, create a directory called `build` and create Makefile using the cmake build system

    mkdir build && cd build
    cmake ..
    make

Once the project is compiled, the binary is directed to a `bin` directory, and takes the `.obj` mesh as a command line argument 

    ./bin/viewer objects/bunny.obj 
    
The text newlines and messages have been hardcoded for full HD (1920x1080) resolution, but lower resolutions should not really be a problem.

## Tweakable: Render Mode 
One of Smooth, Lines, Point Cloud, Mesh
![](https://github.com/amanshenoy/object-viewer/blob/main/demonstrations/new_render_mode.gif)

## Tweakable: Reflective Properties
Sliders for Ambient, Diffuse, Specular, Shininess (in accordance to Phong Lighting model), and a checkbox for activating RBG inputs for the above sliders

![](https://github.com/amanshenoy/object-viewer/blob/main/demonstrations/reflective_properties.gif)

## Tweakable: Input devices
Keyboard or Mouse, for Translation and Rotation (Alternate)
![](https://github.com/amanshenoy/object-viewer/blob/main/demonstrations/input_devices.gif)

## Tweakable: Camera
Same thing as using mouse to move object around, but finer control
![](https://github.com/amanshenoy/object-viewer/blob/main/demonstrations/camera.gif)

## Tweakable: Lighting 

RGB and XYZ Sliders for 5 light sources, 4 of which are off by default 
![](https://github.com/amanshenoy/object-viewer/blob/main/demonstrations/lighting.gif)


## Dependencies and Other Stuff

The project uses [glm](https://github.com/g-truc/glm) for maths, [ImGui](https://github.com/ocornut/imgui) for the GUI and [GLFW](glfw.org) as the windowing library. GLEW was used to access OpenGL. The folder `objects` has a fair few objects to be able to play around with. Most of these were downloaded from [here](https://people.sc.fsu.edu/~jburkardt/data/obj/obj.html) and were corrected using [Meshlab](https://www.meshlab.net/).  
