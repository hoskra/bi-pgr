Task 1 (done with teacher)
------

* brief introduction to parametric curves
* alignObject() function


Task 2 [1 point]    >>> TASK_5_2_Y <<<
----------------

* evaluate point position on a Catmull-Rom spline segment 
  -> function evaluateCurveSegment() in file spline.cpp
  >>> TASK_5_2_1 <<<

* evaluate first derivative for a Catmull-Rom spline segment
  -> function evaluateCurveSegment_1stDerivative() in file spline.cpp
  >>> TASK_5_2_2 <<<

* if the implementation of both functions is correct then application will 
  print out into the console:
  - "Spline segment is VALID"
  - "Spline segment 1st derivative is VALID"

* detailed description related to the calculation of position and derivative 
  for the Catmull-Rom spline segment can be found in attached file named 
  tasks_5_animation.pdf

Task 3 [1 point]    >>> TASK_5_3_Y <<<
----------------

* evaluate point position on a closed spline curve composed from Catmull-Rom
  spline segments -> function evaluateClosedCurve() in file spline.cpp
  >>> TASK_5_3_1 <<<

* evaluate first derivative for a closed spline curve
  -> function evaluateClosedCurve_1stDerivative() in file spline.cpp
  >>> TASK_5_3_2 <<<

* detailed description related to the calculation of position and derivative 
  for the Catmull-Rom spline can be found in attached file named 
  tasks_5_animation.pdf


Notes:
------
* for testing purposes we suggest you to set the initial ufo position 
  (newUfo->initPosition) in function createUfo() in file asteroids.cpp 
  to (0,0) -> ufo trajectory will have the shape of infinite sign symbol
  whose center will be in the middle of the window

* parts of the source code that should be modified to fullfill the tasks 
  are marked by the following sequence of comments where X indicates the 
  task number and Y the subtask number:

    // ======== BEGIN OF SOLUTION - TASK 5_X-Y ======== //
    ...
    // ========  END OF SOLUTION - TASK 5_X-Y  ======== //



What files do you have to edit:
TASK 5_2:
 -> spline.cpp: 103, 132
TASK 5_3:
 -> spline.cpp: 159, 188
