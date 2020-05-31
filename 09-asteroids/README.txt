Task 1 [1 point]    >>> TASK 6_1 <<<
----------------
Wrapping positions of objects lying outside the visible part of the 
scene.

* Valid object's position lies within the following range:
    x: -(SCENE_WIDTH  + objectSize) ... +(SCENE_WIDTH  + objectSize)
    y: -(SCENE_HEIGHT + objectSize) ... +(SCENE_HEIGHT + objectSize)
    z: -(SCENE_DEPTH  + objectSize) ... +(SCENE_DEPTH  + objectSize)

* Coordinates lying outside the valid range have to be changed to fall
  into the valid range again => the scene is considered to be cyclic 
  in all directions, i.e., if the object gets out of the visible part 
  of the scene on the left then it has to appear again on the right
  side. The same holds for remaining directions. 

* Check and correction of object's coordinates has to be done inside
  the function named checkBounds() located in the file render_stuff.cpp. 
  You have to append lines of code to implement this functionality. 
  Note that checkBounds() function is already being called during the 
  update of objects' positions. 

Task 2 [1 point]    >>> TASK 6_2 <<<
----------------
Test collisions among missiles and other objects in the scene (ufos, 
the spaceship, and asteroids) as well as collisions of the spaceship 
with asteroids and ufos.

* Complete the function pointInSphere() in the file asteroids.cpp 
  that checks for a given point whether it is lying inside a sphere 
  of the given radius or not.      >>> TASK 6_2-1 <<<
  => This function is used to test if missile collides with other 
  objects in the scene (ufos, asteroids, and spaceship).

* Complete the function spheresIntersection() in the file asteroids.cpp
  that checks whether two spheres are overlapping (they have non-zero
  intersection).      >>> TASK 6_2-2 <<<
  => This function is used to find out if the spaceship collides with
  asteroids and ufos.


Task 3 - bonus [1 point]    >>> TASK 6_3 <<<
------------------------
Clicking an asteroid makes it explode. The selection has to be done
using a stencil buffer.

The selection process has to be implemented in three steps:

* set up a proper function to be used to update stencil buffer contents
  => see function drawWindowContents() in file asteroids.cpp
  >>> TASK 6_3-1 <<<
* set up proper comparison function to be used to compare contents of
  the stencil buffer with the reference value of drawn fragment
  => see function drawWindowContents() in file asteroids.cpp 
  >>> TASK 6_3-2 <<<
* read back contents of the stencil buffer (stencil buffer value for 
  the pixel under the mouse pointer)
  => identifier of asteroid that has been clicked by mouse
  => see function mouseCallback() in file asteroids.cpp  
  >>> TASK 6_3-3 <<<

Notes:
------
* parts of the source code that should be modified to fulfill the
  tasks are marked by the following sequence of comments where X 
  indicates the task number and Y the subtask number:


    // ======== BEGIN OF SOLUTION - TASK 6_X-Y ======== //
    ...
    // ========  END OF SOLUTION - TASK 6_X-Y  ======== //
What files do you have to edit:
TASK 6_1:
 -> render_stuff.cpp: 85
TASK 6_2:
 -> asteroids.cpp: 60, 78
TASK 6_3:
 -> asteroids.cpp: 400, 411, 909
