Task 1 (done with the teacher)
------
Drawing of the missile and the source code exploration.


Task 2 [1 point] (TASK 1_2-Y)
----------------
Draw the upper part of the ufo by glDrawArrays() command using the interleaved
array (coordinates, colors).
* ufo's upper part is a hexagon whose triangles will be drawn separately 
  (6 triangles)
* hexagon is inscribed in the circle with radius 1.0
* triangles will be drawn in two colors (yellow, magenta) that will be altering

Append lines in data.h to: >>> TASK 1_2-1 <<<
* define contents of the interleaved array (coordinates + colors)
  => array ufoVertices[]
* 3 yellow triangles should be stored first, followed by 3 magenta triangles

Append lines to render_stuff.cpp to:
* initialize ufo geometry in function initUfoGeometry() >>> TASK 1_2-4 <<<
  - initialize vertex array object
  - initialize vertex buffer object
  - interconnect vertex shader attributes (locations are stored in the shader
    structure - input parameter) with buffers 
  - buffers identifiers should be stored in the structure named geometry (input
    parameter) 
* draw ufo geometry in function drawUfo() 
  - the upper part is drawn by two calls glDrawArrays() - magenta and yellow 
    triangles separately >>> TASK 1_2-2 <<< and >>> TASK 1_2-3 <<<

 
Task 3 [1 point] (TASK 1_3-Y)
----------------
Draw the bottom part of the ufo by glDrawElements() command using the
interleaved array (coordinates, colors).
* ufo's bottom part is also a hexagon whose triangles will be drawn 
  separately (6 triangles)
* hexagon is inscribed in the circle with radius 1.0
* vertices on the border have the same color while vertex in the middle differs

Append lines in data.h to:
* add contents of the interleaved array for the bottom part (coordinates + 
  colors) >>> TASK 1_3-1 <<<
  => array ufoVertices[] (the array is shared with the 1st task, append 
     vertices to the end)
* define contents of the element array that includes indices of all vertices 
  used for drawing >>> TASK 1_3-2 <<<
  => array ufoIndices[]

Append lines to render_stuff.cpp to:
* initialize geometry for the bottom part of the ufo in function 
  initUfoGeometry() >>> TASK 1_2-4 <<<
  - initialize vertex array object
  - initialize vertex buffer object
  - initialize element buffer object
  - interconnect vertex shader attributes (locations are stored in shader 
    structure - input parameter) with buffers
  - buffers identifiers should be stored in the structure named geometry (input
    parameter) 
* draw ufo bottom part's geometry in function drawUfo() >>> TASK 1_3-3 <<<
  - the bottom part is drawn by calling glDrawElements() command


Notes:
------
* missile vertices are stored in a noninterlaced array (all vertices followed
  by all colors) 
* you can restart the game by pressing "r" key
* parts of the source code that should be modified to fulfill the tasks are 
  marked by the following sequence of comments:

    // ======== BEGIN OF SOLUTION - TASK 1_X-Y ======== //
    ...
    // ========  END OF SOLUTION - TASK 1_X-Y  ======== //


What files do you have to edit:
TASK 1_2:
 -> data.h: 152
 -> render_stuff.cpp: 110, 115, 303
TASK 1_3:
 -> data.h: 162, 173
 -> render_stuff.cpp: 121
