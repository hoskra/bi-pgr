//----------------------------------------------------------------------------------------
/**
 * \file    render_stuff.h
 * \author  Jaroslav Sloup, Tomas Barak, Petr Felkel
 * \date    2013
 * \brief   Rendering stuff - drawing functions for models, etc.
 */
//----------------------------------------------------------------------------------------

#ifndef __RENDER_STUFF_H
#define __RENDER_STUFF_H

#include "data.h"

// defines geometry of object in the scene (space ship, ufo, asteroid, etc.)
// geometry is shared among all instances of the same object type
typedef struct _MeshGeometry {
  GLuint        vertexBufferObject;   // identifier for the vertex buffer object
  GLuint        elementBufferObject;  // identifier for the element buffer object
  GLuint        vertexArrayObject;    // identifier for the vertex array object
  unsigned int  numTriangles;         // number of triangles in the mesh
  // material
  glm::vec3     ambient;
  glm::vec3     diffuse;
  glm::vec3     specular;
  float         shininess;
  GLuint        texture;

} MeshGeometry;

// parameters of individual objects in the scene (e.g. position, size, speed, etc.)
typedef struct _Object {
  glm::vec3 position;
  glm::vec3 direction;
  float     speed;
  float     size;

  bool destroyed;

  float startTime;
  float currentTime;

} Object;

typedef struct _SpaceShipObject : public Object {

  float viewAngle; // in degrees

} SpaceShipObject;

typedef struct _AsteroidObject : public Object {

  float rotationSpeed;

} AsteroidObject;

typedef struct _MissileObject : public Object {

} MissileObject;

typedef struct _UfoObject : public Object {

  float     rotationSpeed;
  glm::vec3 initPosition;

} UfoObject;

typedef struct _ExplosionObject : public Object {

  int    textureFrames;
  float  frameDuration;

} ExplosionObject;

typedef struct _BannerObject : public Object {

} BannerObject;

typedef struct _commonShaderProgram {
  // identifier for the shader program
  GLuint program;          // = 0;
  // vertex attributes locations
  GLint posLocation;       // = -1;
  GLint colorLocation;     // = -1;
  GLint normalLocation;    // = -1;
  GLint texCoordLocation;  // = -1;
  // uniforms locations
  GLint PVMmatrixLocation;    // = -1;
  GLint VmatrixLocation;      // = -1;  view/camera matrix
  GLint MmatrixLocation;      // = -1;  modeling matrix
  GLint normalMatrixLocation; // = -1;  inverse transposed Mmatrix

  GLint timeLocation;         // = -1; elapsed time in seconds

  // material 
  GLint diffuseLocation;    // = -1;
  GLint ambientLocation;    // = -1;
  GLint specularLocation;   // = -1;
  GLint shininessLocation;  // = -1;
  // texture
  GLint useTextureLocation; // = -1; 
  GLint texSamplerLocation; // = -1;
  // reflector related uniforms
  GLint reflectorPositionLocation;  // = -1; 
  GLint reflectorDirectionLocation; // = -1;
} SCommonShaderProgram;


void drawSpaceShip(SpaceShipObject* spaceShip, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawAsteroid(AsteroidObject* asteroid, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawMissile(MissileObject* missile, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawUfo(UfoObject* ufo, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawExplosion(ExplosionObject* explosion, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawBanner(BannerObject* banner, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawSkybox(const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);

void initializeShaderPrograms();
void cleanupShaderPrograms();

void initializeModels();
void cleanupModels();

#endif // __RENDER_STUFF_H
