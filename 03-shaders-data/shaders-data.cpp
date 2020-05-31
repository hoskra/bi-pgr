//----------------------------------------------------------------------------------------
/**
 * \file    shaders-data.cpp
 * \author  Tomas Barak, Jaroslav Sloup
 * \date    2012/09/11
 * \brief   Shader programming seminar II
 */
//----------------------------------------------------------------------------------------

#include <iostream>

#include "pgr.h"    // pgr framework
#include "birds.h"  // contains our vertex data

// main window attributes
const int   WIN_WIDTH  = 512;
const int   WIN_HEIGHT = 512;
const char* WIN_TITLE  = "PGR - Shaders";

// animation parameters
const int refreshTimeMs   = 33;   // window contents refresh time in ms
const int animFrameTimeMs = 150;  // one animation frame duration in ms

// application state variables
struct State {
  glm::mat4 projection;        // projection transform matrix
  glm::mat4 model;             // modeling transform matrix
  int frame, nextFrame;        // frames used in bird animation
  float t;                     // 0.0 -> frame; 1.0 -> nextFrame
  int lastMouseX, lastMouseY;  
  float yaw, pitch;            // variables used for model rotation
  float scale;                 // model scale
} state;

// OpenGL resources 
struct Resources {
  GLuint program;   // shader program id
  GLuint vbo, ebo;  // vertex and element buffer objects
  GLuint vao;       // vertex array object
} resources;

// Shader attribute/parameter locations 
struct Locations {
  GLint position;
// ======== BEGIN OF SOLUTION - TASK 3-1 ======== //
  // declare handle to the aNextPosition shader input 
  GLint aNextPosition;
// ========  END OF SOLUTION - TASK 3-1  ======== //
  GLint PVM;
// ======== BEGIN OF SOLUTION - TASK 2-1 ======== //
  // declare handle to the scale shader uniform 
  GLint scale;
// ========  END OF SOLUTION - TASK 2-1  ======== //
  GLint t;
  GLint color;
} handles;

// vertex shader code
const char * srcVertexShader =
  "#version 140\n"
  "in vec3 aPosition;\n"
// ======== BEGIN OF SOLUTION - TASK 3-2 ======== //
  // declare the aNextPosition input vec3 variable 
  "in vec3 aNextPosition;"
// ========  END OF SOLUTION - TASK 3-2  ======== //
  "uniform mat4 PVM;\n"
// ======== BEGIN OF SOLUTION - TASK 2-2 ======== //
  // declare scale uniform float variable 
   "uniform float scale;\n"
// ========  END OF SOLUTION - TASK 2-2  ======== //
  "uniform float t;\n"
  "\n"
  "void main() {\n"
// ======== BEGIN OF SOLUTION - TASK 3-3 ======== //
  // you can use the mix() glsl function to interpolate between aPosition and aNextPosition 
  // use the uniform variable t as the interpolation parameter 
  "  vec3 pos = mix(aPosition, aNextPosition, t);\n"
// ========  END OF SOLUTION - TASK 3-3  ======== //
// ======== BEGIN OF SOLUTION - TASK 2-3 ======== //
  // enlarge/shrink vertex position by a given scale (uniform parameter)
  "  pos = pos * scale;\n"
// ========  END OF SOLUTION - TASK 2-3  ======== //
  "  gl_Position = PVM * vec4(pos, 1.0f);\n"
  "}\n"
  "\n";

// fragment shader code
const char * srcFragmentShader =
  "#version 140\n"
  "uniform vec3 color;\n"
  "out vec4 fragmentColor;"
  "\n"
  "void main() {\n"
  "  fragmentColor = vec4(color, 1.0f);\n"
  "}\n"
  "\n";


void onTimer(int) {
  int timeMs = glutGet(GLUT_ELAPSED_TIME);     // time now [0.. infinity]
  int e = timeMs % animFrameTimeMs;            // relative time in the frame [0..150ms]  <- animFrameTimeMs
  state.frame = timeMs / animFrameTimeMs;      // current frame index [0..10] ...        <- birds_data.nAnimFrames
  state.nextFrame = state.frame + 1;           // current frame + 1 modulo 11 [0..10]
  state.frame %= birds_data.nAnimFrames;
  state.nextFrame %= birds_data.nAnimFrames;
  state.t = float(e) / float(animFrameTimeMs); // relative time in the frame [0..1]      <- e % 150ms

  // modeling transformation matrix
  state.model =
      glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -7.0f)) *
      glm::rotate(glm::mat4(1.0f), glm::radians(state.pitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
      glm::rotate(glm::mat4(1.0f), glm::radians(state.yaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
      glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));

  glutPostRedisplay();
  glutTimerFunc(refreshTimeMs, onTimer, 0);
}

void onReshape(int width, int height) {
  glViewport(0, 0, width, height);
  state.projection = glm::perspective(glm::radians(60.0f), float(width) / float(height), 1.0f, 20.0f);
}

void onDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(resources.program);

  glUniformMatrix4fv(handles.PVM, 1, GL_FALSE, glm::value_ptr(state.projection * state.model));
  if(handles.t > -1)
    glUniform1f(handles.t, state.t);
  glUniform3fv(handles.color, 1, birds_data.color);
  // ======== BEGIN OF SOLUTION - TASK 2-4 ======== //
    // set scale uniform value 
    glUniform1f(handles.scale, state.scale);
  // ========  END OF SOLUTION - TASK 2-4  ======== //

// ======== BEGIN OF SOLUTION - TASK 1-1 ======== //
  // bind vertex array object 
    glBindVertexArray(resources.vao);
    glVertexAttribPointer(handles.position,
        3, GL_FLOAT,
        GL_FALSE,
        0,
        (void*)(state.frame * birds_data.nVertices * 3 * sizeof(float)));

  // interconnect position attribute with the data in buffers 
  // use the frame counter to index array of vertices 
// ========  END OF SOLUTION - TASK 1-1  ======== //

// ======== BEGIN OF SOLUTION - TASK 3-4 ======== //
  // use the nextFrame counter to index array of vertices and set is as the nextPosition input 
    glVertexAttribPointer(handles.aNextPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)(state.nextFrame * birds_data.nVertices * 3 * sizeof(float)));
// ========  END OF SOLUTION - TASK 3-4  ======== //

// ======== BEGIN OF SOLUTION - TASK 1-2 ======== //
  // draw the bird model using glDrawElements command 
    glDrawElements(GL_TRIANGLES,
        birds_data.nFaces * 3,
        GL_UNSIGNED_SHORT,
        (void*)0);
  // see birds_data_t data structure declared in birds.h header file to get necessary parameters 
// ========  END OF SOLUTION - TASK 1-2  ======== //

  CHECK_GL_ERROR();
  glutSwapBuffers();
}

void onMouseMotion(int x, int y) {
  int dx = x - state.lastMouseX;
  int dy = y - state.lastMouseY;
  state.lastMouseX = x;
  state.lastMouseY = y;
  state.yaw += 0.5f * dx;
  state.pitch += 0.5f * dy;
}

void onMouse(int, int, int x, int y) {
  state.lastMouseX = x;
  state.lastMouseY = y;
}

void onSpecialKey(int key, int mouseX, int mouseY) {

  switch(key) {
// ======== BEGIN OF SOLUTION - TASK 2-5 ======== //
    // handle left and right arrow keys -> increase/decrese model scale -> variable state.scale 
    // scale have to be always in range 0.1 - 2.0 
      case GLUT_KEY_LEFT:
          state.scale /= 1.1f;
          if (state.scale < 0.1f)
              state.scale = 0.1f;
          break;
      case GLUT_KEY_RIGHT:
          state.scale *= 1.1f;
          if (state.scale > 2.0f)
              state.scale = 2.0f;
          break;
// ========  END OF SOLUTION - TASK 2-5  ======== //
    default:
      ;
  }
  glutPostRedisplay();
}

bool init() {
  std::vector<GLuint> shaders;
  shaders.push_back(pgr::createShaderFromSource(GL_VERTEX_SHADER, srcVertexShader));
  shaders.push_back(pgr::createShaderFromSource(GL_FRAGMENT_SHADER, srcFragmentShader));
  if(shaders.size() != 2)
    return false;

  resources.program = pgr::createProgram(shaders);
  if(resources.program == 0)
    return false;

  // be careful, if you don't use a variable in a shader, the shader compiler optimizes it out,
  // so the glGet*Location function will return -1 (invalid location)
  handles.position = glGetAttribLocation(resources.program, "aPosition");
// ======== BEGIN OF SOLUTION - TASK 3-5 ======== //
  // initialize the handles.nextPosition variable, use the line above as inspiration 
  handles.aNextPosition = glGetAttribLocation(resources.program, "aNextPosition");
// ========  END OF SOLUTION - TASK 3-5  ======== //
  handles.PVM = glGetUniformLocation(resources.program, "PVM");
  handles.t = glGetUniformLocation(resources.program, "t");
  handles.color = glGetUniformLocation(resources.program, "color");
// ======== BEGIN OF SOLUTION - TASK 2-6 ======== //
  // initialize the handles.scale variable, use the line above as inspiration 
  handles.scale = glGetUniformLocation(resources.program, "scale");
// ========  END OF SOLUTION - TASK 2-6  ======== //
  // you can uncomment this to check if everything went ok
  //if(handles.position == -1 || handles.nextPosition == -1 || handles.PVM == -1 || handles.t == -1 || handles.color == -1 || handles.scale == -1)
  //  return false;

// ======== BEGIN OF SOLUTION - TASK 1-3 ======== //
  // generate and initialize the vertex array object -> variable resources.vao
  glGenVertexArrays(1, &resources.vao);
  glBindVertexArray(resources.vao); 
  // generate and initialize the vertex buffer object -> variable resources.vbo 
  glGenBuffers(1, &resources.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, resources.vbo);
  glBufferData(GL_ARRAY_BUFFER, birds_data.nAnimFrames * birds_data.nVertices * 3 * sizeof(float), birds_data.vertices, GL_STATIC_DRAW);
  // generate and initialize the element buffer object -> variable resources.ebo 
  glGenBuffers(1, &resources.ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resources.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, birds_data.nFaces * 3 * sizeof(unsigned short), birds_data.faces, GL_STATIC_DRAW);
  // enable and initialize the position attribute array 
  glEnableVertexAttribArray(handles.position);
  glVertexAttribPointer(handles.position,
      3, GL_FLOAT,
      GL_FALSE,
      0,
      (void*)0); 
// ========  END OF SOLUTION - TASK 1-3  ======== //

// ======== BEGIN OF SOLUTION - TASK 3-6 ======== //
  // enable and initialize the nextPosition attribute array 
  glEnableVertexAttribArray(handles.aNextPosition);
  glVertexAttribPointer(handles.aNextPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
// ========  END OF SOLUTION - TASK 3-6  ======== //

  glBindVertexArray(0);

  state.model = glm::mat4(1.0f);
  state.frame = 0;
  state.scale = 1.0f;

  glClearColor(0.5f, 0.4f, 0.8f, 1.0f);
  CHECK_GL_ERROR();
  return true;
}

// release all allocated resources
void cleanup() {
  // delete vertex array object
  glDeleteVertexArrays(1, &resources.vao);
  // delete vertex and element buffer objects
  glDeleteBuffers(1, &resources.vbo);
  glDeleteBuffers(1, &resources.ebo);
  pgr::deleteProgramAndShaders(resources.program);
}

void onKey(unsigned char key, int mouseX, int mouseY) {
  switch(key) {
    case 27:
      // cleanup allocated resources
      cleanup();
      glutLeaveMainLoop();
      break;
  }
} 

int main(int argc, char* argv[]) {
  glutInit(&argc, argv);

  glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
  glutCreateWindow(WIN_TITLE);

  glutReshapeFunc(onReshape);
  glutDisplayFunc(onDisplay);
  glutMotionFunc(onMouseMotion);
  glutMouseFunc(onMouse);
  glutKeyboardFunc(onKey);
// ======== BEGIN OF SOLUTION - TASK 2-7 ======== //
  // add callback to handle left and right arrow keys 
  glutSpecialFunc(onSpecialKey);
  // ========  END OF SOLUTION - TASK 2-7  ======== //
  glutTimerFunc(refreshTimeMs, onTimer, 0);

  // initialize pgr framework
  if(!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
    pgr::dieWithError("pgr init failed, required OpenGL not supported?");

  // initialize application
  if(!init())
    pgr::dieWithError("init failed, cannot continue");

  std::cout << "click and grag the mouse within the window to rotate the model" << std::endl;
  std::cout << "use left/right arrows to decrease/increase scale of the model" << std::endl;

  glutMainLoop();

  return 0;
}
