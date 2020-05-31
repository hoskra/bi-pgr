//----------------------------------------------------------------------------------------
/**
 * \file    data.h
 * \author  Jaroslav Sloup, Tomas Barak, Petr Felkel
 * \date    2013
 * \brief   Basic defines and data structures.
 */
//----------------------------------------------------------------------------------------

#ifndef __DATA_H
#define __DATA_H

#define WINDOW_WIDTH   750
#define WINDOW_HEIGHT  750
#define WINDOW_TITLE   "Asteroids Game"

// keys used in the key map
enum { KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_SPACE, KEYS_COUNT };

#define ASTEROIDS_COUNT_MIN  5
#define ASTEROIDS_COUNT_MAX 10
#define ASTEROID_PARTS       3

#define ASTEROID_SIZE        0.05f
#define ASTEROID_SIZE_FACTOR 0.75f
#define ASTEROID_SIZE_MIN    (ASTEROID_SIZE*ASTEROID_SIZE_FACTOR*ASTEROID_SIZE_FACTOR)

#define UFOS_COUNT_MIN 1
#define UFOS_COUNT_MAX 3

#define SPACESHIP_VIEW_ANGLE_DELTA 2.0f // in degrees

#define MISSILE_MAX_DISTANCE       1.5f
#define MISSILE_LAUNCH_TIME_DELAY  0.25f // seconds

#define SPACESHIP_SIZE   0.05f
#define UFO_SIZE         0.05f
#define MISSILE_SIZE     0.0085f
#define BILLBOARD_SIZE   0.1f
#define BANNER_SIZE      1.0f

#define MISSILE_SPEED              1.5f
#define ASTEROID_SPEED_MAX         0.5f
#define SPACESHIP_SPEED_INCREMENT  0.025f
#define SPACESHIP_SPEED_MAX        1.0f

#define ASTEROID_ROTATION_SPEED_MAX 1.0f
#define UFO_ROTATION_SPEED_MAX      1.0f

#define SCENE_WIDTH  1.0f
#define SCENE_HEIGHT 1.0f
#define SCENE_DEPTH  1.0f

#define CAMERA_ELEVATION_MAX 45.0f

// default shaders - color per vertex and matrix multiplication
const std::string colorVertexShaderSrc(
    "#version 140\n"
    "uniform mat4 PVMmatrix;\n"
    "in vec3 position;\n"
    "in vec3 color;\n"
    "smooth out vec4 theColor;\n"
    "void main() {\n"
	"  gl_Position = PVMmatrix * vec4(position, 1.0);\n"
	"  theColor = vec4(color, 1.0);\n"
    "}\n"
);

const std::string colorFragmentShaderSrc(
    "#version 140\n"
    "smooth in vec4 theColor;\n"
    "out vec4 outputColor;\n"
    "void main() {\n"
    "  outputColor = theColor;\n"
    "}\n"
);

//
// missile geometry definition 
//

const int missileTrianglesCount = 4;
// temp constants used for missileVertices array contents definition
const float invSqrt2 = (float)(1.0 / sqrt(2.0));

// missile is represented as a tetrahedron (polyhedron with 4 faces and 4 vertices)
const float missileVertices[] = {
  // non-interleaved array
  // vertices of tetrahedron, each face is an equilateral triangle, edge length 2.0

  // vertices
  //  1.0f,  0.0f, -invSqrt2,   -> vertex 0
  //  0.0f,  1.0f,  invSqrt2,   -> vertex 1
  // -1.0f,  0.0f, -invSqrt2,   -> vertex 2
  //  0.0f, -1.0f,  invSqrt2    -> vertex 3

  // three vertices per each triangle
   0.0f, -1.0f,  invSqrt2, // 3
   1.0f,  0.0f, -invSqrt2, // 0
   0.0f,  1.0f,  invSqrt2, // 1

  -1.0f,  0.0f, -invSqrt2, // 2
   0.0f, -1.0f,  invSqrt2, // 3
   0.0f,  1.0f,  invSqrt2, // 1

   1.0f,  0.0f, -invSqrt2, // 0
  -1.0f,  0.0f, -invSqrt2, // 2
   0.0f,  1.0f,  invSqrt2, // 1

  -1.0f,  0.0f, -invSqrt2, // 2
   1.0f,  0.0f, -invSqrt2, // 0
   0.0f, -1.0f,  invSqrt2, // 3

  // colors for vertices
  1.0f, 0.0f, 1.0f, // 3
  1.0f, 0.0f, 1.0f, // 0
  0.0f, 1.0f, 0.0f, // 1

  0.0f, 0.0f, 1.0f, // 2
  1.0f, 0.0f, 1.0f, // 3
  0.0f, 1.0f, 0.0f, // 1

  1.0f, 0.0f, 1.0f, // 0
  0.0f, 0.0f, 1.0f, // 2
  0.0f, 1.0f, 0.0f, // 1

  0.0f, 0.0f, 1.0f, // 2
  1.0f, 0.0f, 1.0f, // 0
  1.0f, 0.0f, 1.0f, // 3

  // normals
};

//
// ufo geometry definition 
//

const int ufoTrianglesCount = 6;
// temp constants used for ufoVertices array contents definition
const float ufoH = 0.25f;
const float cos30d = (float)cos(M_PI/6.0);
const float sin30d = (float)sin(M_PI/6.0);

const float ufoVertices[] = {
  // ufo is formed by two parts (top and bottom) joined together
  // each part is drawn as six separate triangles connected together

  // drawArrays() part of data (top part), interleaved array
  // vertices 0..5 are on the border, vertex 6 is in the center
  // colors of the triangles alternate between yellow (1,1,0) and magenta (1,0,1)

// ======== BEGIN OF SOLUTION - TASK 1_2-1 ======== //
  // fill in correct vertex attributes (position/color)
  //  x      y        z      r     g     b
    cos30d,   0.0f,   -sin30d,  1.0f,  1.0f,  0.0f,      // yellow
    0.0f,     ufoH,   0.0f,       1.0f,  1.0f,  0.0f,
    cos30d,   0.0f,   sin30d,   1.0f,  1.0f,  0.0f,

    0.0f,   0.0f,   1.0f,  1.0f,  1.0f,  0.0f,      // yellow
    0.0f,   ufoH,   0.0f,       1.0f,  1.0f,  0.0f,
    -cos30d,   0.0f,   sin30d,   1.0f,  1.0f,  0.0f,

    -cos30d,   0.0f,   -sin30d,  1.0f,  1.0f,  0.0f,      // yellow
    0.0f,   ufoH,   0.0f,       1.0f,  1.0f,  0.0f,
     0.0f,   0.0f,   -1.0f,   1.0f,  1.0f,  0.0f,

     cos30d,   0.0f,   sin30d,  1.0f,  0.0f,  1.0f,      // purple
    0.0f,   ufoH,   0.0f,       1.0f,  0.0f,  1.0f,
    0.0f,   0.0f,    1.0f,   1.0f,  0.0f,  1.0f,

    -cos30d,   0.0f,  sin30d,  1.0f,  0.0f,  1.0f,      // purple
    0.0f,   ufoH,   0.0f,       1.0f,  0.0f,  1.0f,
    -cos30d,   0.0f,   -sin30d,   1.0f,  0.0f,  1.0f,

    0.0f,   0.0f,   -1.0f,  1.0f,  0.0f,  1.0f,      // purple
    0.0f,   ufoH,   0.0f,       1.0f,  0.0f,  1.0f,
     cos30d,   0.0f,   -sin30d,   1.0f,  0.0f,  1.0f,

// ========  END OF SOLUTION - TASK 1_2-1  ======== //

  // draw elements data part (bottom part), interleaved array
  // vertices 0..5 are on the border, vertex 6 is in the center
  // vertices on the border have the same color while vertex in the middle differs

// ======== BEGIN OF SOLUTION - TASK 1_3-1 ======== //
  // fill in correct vertex attributes (position/color)
  //  x      y        z      r     g     b
  
    cos30d,   0.0f,   sin30d,   1.0f,  0.0f,  1.0f,      // 0
    0.0f,     0.0f,   1.0f,     1.0f,  0.0f,  1.0f,      // 1

    -cos30d,   0.0f,  sin30d,   1.0f,  0.0f,  1.0f,      // 2
    -cos30d,   0.0f,  -sin30d,  1.0f,  0.0f,  1.0f,      // 3

    0.0f,      0.0f,   -1.0f,   1.0f,  0.0f,  1.0f,      // 4
    cos30d,    0.0f,   -sin30d, 1.0f,  0.0f,  1.0f,      // 5
    0.0f,     -ufoH,   0.0f,    0.3f,  0.3f,  0.3f,      // 6

// ========  END OF SOLUTION - TASK 1_3-1  ======== //

};

// indices of the 6 faces used to draw the bottom part of the ufo
const unsigned int ufoIndices[] = {

// ======== BEGIN OF SOLUTION - TASK 1_3-2 ======== //
  // fill in the correct vertex indices
  18 + 5, 18 + 0, 18 + 6,


// ========  END OF SOLUTION - TASK 1_3-2  ======== //

};


#endif // __DATA_H