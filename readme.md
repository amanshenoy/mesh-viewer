# Object / Mesh Viewer

The following repository contains the implementation of a basic mesh/object viewer for `.obj` files. The project has been implemented in C++ using OpenGL and GLSL.

This application can be used to understand some basic concepts of the OpenGL pipeline, and to understand the relevance of certain variables in an interactive manner. The project uses the typical OpenGL rendering pipeline and uses the phong lighting model for lighting calculations.   

## Usage

A sample binary has been uploaded with the repo (compiled on x86 as a 64 bit application), but its unlikely that it would generally work. To compile in a device specific manner, you can create a Makefile using `cmake` (`CMakeLists.txt` given). After creating the Makefile using cmake and compiling the project, the binary is to be used with a single command line argument - the path to the `.obj` file to display. 

    >>> object-viewer objects/car.obj 
    
The text newlines and messages have been hardcoded for full HD resolution, but lower resolutions should not really be a problem.

## Tweakable settings
### Render Mode 

Once the application has been launched, the first changeable setting is of render mode (triangles, lines, points). This chooses the primitive given to the fragment shader by the rasterisation process. 

![](https://github.com/amanshenoy/object-viewer/blob/main/demonstrations/render_mode.gif)

### Reflective Properties

These sliders allow you to change the object reflective properties associated with the phong lighting model. It is initially assumed that all objects are white, but clicking on the 'Custom colors' checkbox, enables RGB inputs for each property.

![](https://github.com/amanshenoy/object-viewer/blob/main/demonstrations/reflective_properties.gif)

### Input devices

By default the keyboard arrow keys are used for translation, and the mouse is used for rotation. This can be changed in the input devices tab. Switching one input device to translation automatically switches the other to rotation. The scroll wheel is fixed to scaling.  

![](https://github.com/amanshenoy/object-viewer/blob/main/demonstrations/input_devices.gif)

### Camera

For simplicity, the rotation transformation of an object has been implemented as camera movement around the object. To give the illusion of object floating, the lighting has to stay still and for this the light positions are transformed by the inverse of the modelview transformation. Apart from the mouse click and drag callbacks, this can be changed using this tab for more control. 
![](https://github.com/amanshenoy/object-viewer/blob/main/demonstrations/camera.gif)

### Lighting 

Up to any number of light sources can be used, the default being 5. Only the first light is switched on initially and the others can be easily added by changing RGB and XYZ info appropriately in the sliders. 
![](https://github.com/amanshenoy/object-viewer/blob/main/demonstrations/lighting.gif)


## Dependancies and Other Stuff

The project uses [glm](https://github.com/g-truc/glm) for maths, [ImGui](https://github.com/ocornut/imgui) for the GUI and [GLFW](glfw.org) as the windowing library. The folder `objects` has a fair few objects to be able to play around with. Most of these were downloaded from [here](https://people.sc.fsu.edu/~jburkardt/data/obj/obj.html) and were corrected using [Meshlab](https://www.meshlab.net/).  
