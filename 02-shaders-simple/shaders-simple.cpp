//----------------------------------------------------------------------------------------
/**
 * \file    shaders-simple.cpp
 * \author  Petr Felkel, Tomas Barak
 * \date    2013
 * \brief   Shader programming seminar #1
 * Based on shaders-sin.cpp
 */
//----------------------------------------------------------------------------------------

#include <iostream>

#include "pgr.h"

const int WIN_WIDTH = 512;
const int WIN_HEIGHT = 512;
const char * WIN_TITLE = "PGR - Shaders #1";
const int refreshTimeMs = 33;
const int MAX_TASK_NUMBER = 7;

// you can try another objects (like teapot or monkey)
//const pgr::MeshData & meshData = pgr::cubeData;
const pgr::MeshData & meshData = pgr::teapotData;
//const pgr::MeshData & meshData = pgr::monkeyData;
//const pgr::MeshData & meshData = pgr::icosphere3Data;

struct State {
  float time;
  float alpha;   // varies from 0..1, based on timer
  int task;      // task number
} state;

struct Resources { // program and buffer names
  GLuint program;  // program object
  GLuint vbo_positions, vbo_indices; // vertex buffer objects for coordinates and triangle indices
  GLuint vao;     //vertex array object
} resources;

struct Locations {
  // attributes
  GLint position;
  // uniforms
  GLint PVMMatrix;
  GLint task;  // task number
  // ======== BEGIN OF SOLUTION - TASK 3-1 ======== //
  // declare locations to winWidth and alpha uniforms 
  GLint winWidth;
  GLint alpha;
  // ========  END OF SOLUTION - TASK 3-1  ======== //
} locations;

// projection matrix used by shaders
glm::mat4 projection;

void onTimer(int) {
  glutPostRedisplay();
  glutTimerFunc(refreshTimeMs, onTimer, 0);

  int timeMs = glutGet(GLUT_ELAPSED_TIME);
  state.time = timeMs * 0.001f;

  state.alpha = 0.5f+0.5f*sin(state.time);
}

void onReshape(int width, int height) {
  glViewport(0, 0, width, height);
}

void onDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(resources.program);

  if( true )
    projection = glm::rotate(glm::mat4(1.0f), glm::radians(state.time * 30.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // the animation
  else
    projection = glm::mat4(1.0f);
  glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.6f));

  glUniformMatrix4fv(locations.PVMMatrix, 1, GL_FALSE, glm::value_ptr(projection * model));
  glUniform1i( locations.task, state.task );

  // ======== BEGIN OF SOLUTION - TASK 3-2 ======== //
  // use the glUniform1i and glUniform1f functions to set the winWidth and alpha uniforms 
  // use state.alpha and glutGet(GLUT_WINDOW_WIDTH) to obtain current values 
  glUniform1i(locations.winWidth, glutGet(GLUT_WINDOW_WIDTH));
  glUniform1f(locations.alpha, state.alpha);
  // ========  END OF SOLUTION - TASK 3-2  ======== //

  glBindVertexArray(resources.vao);
  //glDrawArrays(GL_TRIANGLES, 0, meshData.nVertices);
  glDrawElements(GL_TRIANGLES, meshData.nTriangles * 3, GL_UNSIGNED_INT, (void*)0);
  glBindVertexArray(0);

  CHECK_GL_ERROR();
  glutSwapBuffers();
}

bool loadShaders() {
  if(resources.program != 0)
    pgr::deleteProgramAndShaders(resources.program);

  GLuint shaders[] = {
    pgr::createShaderFromFile(GL_VERTEX_SHADER, "simple-vs.glsl"),
    pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "simple-fs.glsl"),
    0,
  };
  if(shaders[0] == 0 || shaders[1] == 0)
    return false;

  resources.program = pgr::createProgram(shaders);
  if(resources.program == 0)
    return false;

  // locations to shader input(s)
  // VS attribute
  locations.position = glGetAttribLocation(resources.program, "position");

  // uniforms
  locations.PVMMatrix = glGetUniformLocation(resources.program, "mPVM");
  locations.task      = glGetUniformLocation(resources.program, "iTask");

  // ======== BEGIN OF SOLUTION - TASK 3-3 ======== //
  // use the glGetUniformLocation fuction to obtain locations to time, omega and ampltitude uniforms 
  locations.alpha = glGetUniformLocation(resources.program, "alpha");
  locations.winWidth = glGetUniformLocation(resources.program, "winWidth");
  // ========  END OF SOLUTION - TASK 3-3  ======== //

  return true;
}

void connectVertexAttributes() {
  glBindVertexArray(resources.vao);

  // vertex positions
  glBindBuffer(GL_ARRAY_BUFFER, resources.vbo_positions);
  glEnableVertexAttribArray(locations.position);
  glVertexAttribPointer(locations.position, 3, GL_FLOAT, GL_FALSE, meshData.nAttribsPerVertex * sizeof(float), (void *)(0));  // [xyz][nx,ny,nz][s,t]

  // triangle indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resources.vbo_indices);

  glBindVertexArray(0);
  CHECK_GL_ERROR();
}

bool init() {
  resources.program = 0;
  if(!loadShaders()) {
    std::cerr << "cannot load shaders" << std::endl;
    return false;
  }

  // buffer for vertices
  glGenBuffers(1, &resources.vbo_positions);
  glBindBuffer(GL_ARRAY_BUFFER, resources.vbo_positions);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshData.nVertices * meshData.nAttribsPerVertex, meshData.verticesInterleaved, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // buffer for triangle indices - ELEMENT_ARRAY
  glGenBuffers(1, &resources.vbo_indices);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resources.vbo_indices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * meshData.nTriangles * 3, meshData.triangles, GL_STATIC_DRAW);  // 3-indices per triangle
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // VAO
  glGenVertexArrays(1, &resources.vao);
  connectVertexAttributes();

  state.task  = 1;
  state.time  = 0.0f;
  state.alpha = 0.0f;

  glClearColor(0.5f, 0.4f, 0.8f, 1.0f);
  CHECK_GL_ERROR();

  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
  glCullFace( GL_BACK);
  glEnable(GL_CULL_FACE);
  return true;
}

void onKey(unsigned char key, int, int) {
  switch(key) {
    case 27:
      glutLeaveMainLoop();
      break;
    case 'r':
      loadShaders();
      connectVertexAttributes();
      break;
    case ' ':
      state.task++;
      if (state.task > MAX_TASK_NUMBER)
        state.task = 1;
      std::cout << "  Task number = " << state.task << std::endl;
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

  glutDisplayFunc(onDisplay);
  glutReshapeFunc(onReshape);
  glutKeyboardFunc(onKey);
  glutTimerFunc(refreshTimeMs, onTimer, 0);

  if(!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
    pgr::dieWithError("pgr init failed, required OpenGL not supported?");

  if(!init())
    pgr::dieWithError("init failed, cannot continue");

  std::cout << "use the spacebar to cycle through tasks, the r key to reload shaders" << std::endl;
  glutMainLoop();
  return 0;
}
