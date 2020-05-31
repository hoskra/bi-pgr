
task 0
------
Examine glut based application structure.

Look at the following pieces of the source code:
* main function
  - initialize glut library
  - context
  - callbacks
  - mainloop
  - pgr framework init + include "pgr.h"
* callbacks to display window contents, reshape window, handle keys
* application init, cleanup

task 1
------
Draw rook model - define necessary objects and use proper drawing function.

Append lines to init() function to:
* generate vertex array object
* generate and fill in vertex buffer object
* generate and fill in element buffer object
* interconnect shader attributes with data

Append lines to onDisplay() function to:
* draw bird model using glDrawElements() command
* values of required parameters can be found in structure birds_data_t declared in header file birds.h

task 2
------
Scale the model using left and right arrows.
* current scale -> variable state.scale (have to be in range 0.1 - 2.0)

Append lines to onSpecialKey() function to:
 * handle left arrow key to enlarge the model -> multiply scale by 1.1f
 * handle right arrow key to shrink the model -> divide scale by 1.1f
Append lines to vertex shader to:
 * declare new uniform for scale, name "scale"
 * multiply the position of vertex by scale (all components)
Append lines to init() function to:
 * get location of shader input parameter "scale"
Append lines to onDisplay() function to:
 * set value of the vertex shader input parameter "scale" according to state.scale

task 3
------
Take a quick look on a “key frame” animation
* Time is continuous domain – we cannot store “all” possible positions
* We can store several “key” positions and interpolate
 - http://en.wikipedia.org/wiki/Key_frame
 - http://en.wikipedia.org/wiki/Interpolate

Check out the output of the our application
* There is no interpolation, we use only one of the key frames
* The animation looks really “choppy” and unnatural

To improve it:
a) Declare new vertex shader input variable aNextPosition
b) Use the aNextPosition and aPosition variables and interpolate
   between them (use the uniform variable t; mix() can do the job)
c) Retrieve the aNextPosition location and enable the attribute array.
   You can use the existing code dealing with the aPosition variable as
   an example – the procedure is analogous
d) Use the state.nextFrame variable to index vertices for the
   aNextFrame shader attribute

What files do you have to edit:
TASK 1:
 -> shaders-data.cpp: 132, 142, 205
TASK 2:
 -> shaders-data.cpp: 49, 64, 75, 128, 168, 198, 262
TASK 3:
 -> shaders-data.cpp: 45, 60, 70, 138, 192, 212
