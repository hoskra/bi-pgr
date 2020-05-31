//----------------------------------------------------------------------------------------
/**
 * \file       spline.cpp
 * \author     Miroslav Miksik & Jaroslav Sloup & Petr Felkel
 * \date       2013
 * \brief      Utility functions and stuff tasks concerning animation curves.
*/
//----------------------------------------------------------------------------------------

#include "spline.h"

//**************************************************************************************************
/// Checks whether vector is zero-length or not.
bool isVectorNull(const glm::vec3 &vect) {

  return !vect.x && !vect.y && !vect.z;
}

//**************************************************************************************************
/// Align (rotate and move) current coordinate system to given parameters.
/**
 This function works similarly to \ref gluLookAt, however it is used for object transform
 rather than view transform. The current coordinate system is moved so that origin is moved
 to the \a position. Object's local front (-Z) direction is rotated to the \a front and
 object's local up (+Y) direction is rotated so that angle between its local up direction and
 \a up vector is minimum.

 \param[in]  position           Position of the origin.
 \param[in]  front              Front direction.
 \param[in]  up                 Up vector.
 */
glm::mat4 alignObject(const glm::vec3 &position, const glm::vec3 &front, const glm::vec3 &up) {

  glm::vec3 z = -glm::normalize(front);

  if (isVectorNull(z))
    z = glm::vec3(0.0, 0.0, 1.0);

  glm::vec3 x = glm::normalize(glm::cross(up, z));

  if (isVectorNull(x))
    x =  glm::vec3(1.0, 0.0, 0.0);

  glm::vec3 y = glm::cross(z, x);
  //mat4 matrix = mat4(1.0f);
  glm::mat4 matrix = glm::mat4(
      x.x,        x.y,        x.z,        0.0,
      y.x,        y.y,        y.z,        0.0,
      z.x,        z.y,        z.z,        0.0,
      position.x, position.y, position.z, 1.0
  );

  return matrix;
}



