SEMINAR 2 ASSIGNMENT:

Today, we will concentrate on simple vertex and fragment shaders.

NOTE:
- implicit C++ real constant is of type double   (double a = 0.5; float b = 0.5f)
- implicit GLSL real constant is of type float   (double a = 0.5LF; float b = 0.5)
See https://www.khronos.org/registry/OpenGL/specs/gl/GLSLangSpec.4.30.pdf, Chapter 4.1.4 on page 27

HINT:
- compile and run the program
- order the windows to see both - the graphical window with thwe teapot and the console
- use SPACE bar to select the just solved task (SPACE bar circles between the tasks) 
- edit the shader 
- use r to reload the shader instead of stopping and running the whole program

FRAGMENT SHADER
---------------

task1
Set a color of the fragment to red (a constant value)

task2
Set the fragments on the window diagonal to red color and the remaining fragments to green color
HINT:
  - Which input built-in variable stores the fragment window-relative coordinates? 

task3
Color the fragments on the left part of the window to red and and right part of the fragment to green.
The position of the separator will change based on the alpha uniform. 
HINTS:
  - Declare winWidth and alpha uniforms and use (winWidth * alpha) as a separator x-coordinate
  - This is the only task needing restart of the whole program

task4 - task 7
  - define the input variable color (it will be used by VS/interpolator to pass the fragment color)


VERTEX SHADER
-------------

task4 
- define constant output variable color and set it to blue 

task5
- define the output variable color based on gl_VertexID

task6
- set color.rgb according to the vertex position 

task7
- set the color to white 
- project the position on the teapot surface to the sphere - store this position in a local variable morphPosition
- perform the animated morphing between the original vertex position and the projected position based on alpha
HINT: use the mix function, reference can be found on:
      http://www.khronos.org/files/opengl43-quick-reference-card.pdf 
  and https://www.khronos.org/registry/OpenGL-Refpages/gl4/index.php
- compute gl_Position based on the morphPosition


CPP
---
task3
- declare locations to winWidth and alpha uniforms 
- use the glUniform1i and glUniform1f functions to set the winWidth and alpha uniform values
  (obtain current values from state.alpha and glutGet(GLUT_WINDOW_WIDTH))







What files do you have to edit:
TASK 1:
 -> simple-fs.glsl: 19
TASK 2:
 -> simple-fs.glsl: 27
TASK 3:
 -> simple-fs.glsl: 5, 35
 -> simple-vs.glsl: 12
 -> shaders-simple.cpp: 45, 81, 119
TASK 4:
 -> simple-fs.glsl: 11, 45
 -> simple-vs.glsl: 8, 39
TASK 5:
 -> simple-vs.glsl: 46
TASK 6:
 -> simple-vs.glsl: 53
TASK 7:
 -> simple-vs.glsl: 17, 60
