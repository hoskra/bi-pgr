Task 1 (done with the teacher)
------
Explore the source code related to the keyboard callbacks and object
transformations.


Task 2 [1 point]        >>> TASK 2_2-Y <<<
----------------
Implement free camera (camera freely moving within the scene) that will be:
- moving forward
- able to turn left and right
- increase and decrease movement speed  

The camera view is tied to the spaceship, i.e., camera has the same direction
  and position as spaceship. Camera transformation should be set up by 
  glm::looAt() function whose parameters (center, position, and up vector)
  have to be derived from the spaceship state structure variables (structure
  SpaceShipObject declared in render_stuff.h) 
  -> see GameObjects->spaceShip variable

Camera movement has to be realized through the spaceship movement in the
following functions invoked by arrow keys (all in file asteroids.cpp):

* function increaseSpaceShipSpeed(float deltaSpeed)
* function decreaseSpaceShipSpeed(float deltaSpeed)
* turnSpaceShipLeft(float deltaAngle)
* turnSpaceShipLeft(float deltaAngle)

Append lines to the following functions in file asteroids.cpp to complete the 
camera/spaceship movement:

* function increaseSpaceShipSpeed(float deltaSpeed)       >>> TASK 2_2-1 <<<
  - increase the space ship speed (variable gameObjects.spaceShip->speed) by 
    deltaSpeed
  - speed should not exceed the SPACESHIP_SPEED_MAX

* function decreaseSpaceShipSpeed(float deltaSpeed)       >>> TASK 2_2-2 <<<
  - decrease the space ship speed by deltaSpeed
  - speed has to be always greater or equal to zero

* turnSpaceShipLeft(float deltaAngle)                     >>> TASK 2_2-3 <<<
  - update space ship view angle (variable gameObjects.spaceShip->viewAngle) by
    deltaAngle and recalculate its direction (gameObjects.spaceShip->direction)
  - note that gameObjects.spaceShip->direction represents shaceship orientation
    with respect to its position
  - keep always view angle in range <0,360> degrees

* turnSpaceShipRight(float deltaAngle)                    >>> TASK 2_2-4 <<<
  - update space ship view angle by deltaAngle and recalculate its direction, 
    similarly to turnSpaceShipLeft but in opposite direction
  - keep always view angle in range <0,360> degrees

Append lines to drawWindowContents() function in file asteroids.cpp to:    
										      >>> TASK 2_2-5 a TASK 2_2-6 <<<
* set camera transformation by glm::looAt() function whose parameters (center,
  position, and up vector) have to be derived from the spaceship state
  structure variables (structure SpaceShipObject declared in render_stuff.h) 
  -> see GameObjects.spaceShip variable

Append lines to updateObjects() function in file asteroids.cpp to:    
														    >>> TASK 2_2-7 <<<
* update space ship position taking into account timeDelta, space ship speed, 
  and space ship direction


Task 3 [1 point]        >>> TASK 2_3-Y <<<
----------------

Extend the previous task to move the view direction of the camera also up and
down by mouse (should switch to the free camera mode first).

Append lines to drawWindowContents() function in file asteroids.cpp to:    
															>>> TASK 2_3-1 <<<
* update camera view direction (through variable cameraCenter) and
  cameraUpVector by gameState.cameraElevationAngle variable
  - gameState.cameraElevationAngle variable is already set up in 
    passiveMouseMotionCallback() according to the mouse movement
  - cameraCenter and cameraUpVector are local variables used as parameters for
    the glm::lookAt() function
  - the up vector has to be perpendicular to the view direction - note that 
    this is not required by the lookAt function, but you have to ensure it by
	your calculation


Notes:
------
* you can switch between static viewpoint and free camera view by "c" key
* parts of the source code that should be modified to fulfill the tasks are
  marked by the following sequence of comments where X indicates task number
  and Y subtask:

    // ======== BEGIN OF SOLUTION - TASK 2_X-Y ======== //
    ...
    // ========  END OF SOLUTION - TASK 2_X-Y  ======== //




What files do you have to edit:
TASK 2_2:
 -> asteroids.cpp: 50, 61, 72, 81, 275, 289, 356
TASK 2_3:
 -> asteroids.cpp: 284
