//----------------------------------------------------------------------------------------
/**
 * \file    asteroids.cpp
 * \author  Jaroslav Sloup, Tomas Barak, Petr Felkel
 * \date    2011-2012
 * \brief   Simple implementaion of Asteroids game.
 */
//----------------------------------------------------------------------------------------

#include <time.h>
#include <list>
#include "pgr.h"
#include "render_stuff.h"
#include "spline.h"



typedef std::list<void *> GameObjectsList; 

struct GameState {

  int windowWidth;    // set by reshape callback
  int windowHeight;   // set by reshape callback

  bool gameOver;              // false;
  bool keyMap[KEYS_COUNT];    // false

  float elapsedTime;
  float missileLaunchTime;
  float ufoMissileLaunchTime;

} gameState;

struct GameObjects {

  SpaceShipObject *spaceShip; // NULL

  GameObjectsList asteroids;
  GameObjectsList missiles;
  GameObjectsList ufos;

} gameObjects;


void teleport(void) {

  // generate new space ship position randomly
  gameObjects.spaceShip->position = glm::vec3(
    (float)(2.0 * (rand() / (double)RAND_MAX) - 1.0),
    (float)(2.0 * (rand() / (double)RAND_MAX) - 1.0),
    0.0f
  );
}

void cleanUpObjects(void) {

  // delete asteroids
  while(!gameObjects.asteroids.empty()) {
    delete gameObjects.asteroids.back();
    gameObjects.asteroids.pop_back();
  } 

  // delete ufos
  while(!gameObjects.ufos.empty()) {
    delete gameObjects.ufos.back();
    gameObjects.ufos.pop_back();
  } 
}

// generates random position that does not collide with the spaceship
glm::vec3 generateRandomPosition(void) {
 glm::vec3 newPosition;

    // position is generated randomly
    // coordinates are in range -1.0f ... 1.0f
    newPosition = glm::vec3(
      (float)(2.0 * (rand() / (double)RAND_MAX) - 1.0),
      (float)(2.0 * (rand() / (double)RAND_MAX) - 1.0),
      0.0f
    );

  return newPosition;
}

AsteroidObject* createAsteroid(void) {
 AsteroidObject* newAsteroid = new AsteroidObject;

  newAsteroid->destroyed = false;

  newAsteroid->startTime = gameState.elapsedTime;
  newAsteroid->currentTime = newAsteroid->startTime;

  newAsteroid->size = ASTEROID_SIZE;

  // generate motion direction randomly in range -1.0f ... 1.0f
  newAsteroid->direction = glm::vec3(
    (float)(2.0 * (rand() / (double)RAND_MAX) - 1.0),
    (float)(2.0 * (rand() / (double)RAND_MAX) - 1.0),
    0.0f
  );
  newAsteroid->direction = glm::normalize(newAsteroid->direction);

  // position is generated randomly as well
  newAsteroid->position = generateRandomPosition();

  // motion speed 0.0f ... 1.0f
  newAsteroid->speed = ASTEROID_SPEED_MAX * (float)(rand() / (double)RAND_MAX);
  // rotation speed 0.0f ... 1.0f
  newAsteroid->rotationSpeed = ASTEROID_ROTATION_SPEED_MAX * (float)(rand() / (double)RAND_MAX);

  return newAsteroid;
}

UfoObject* createUfo(void) {
 UfoObject* newUfo = new UfoObject;

  newUfo->destroyed = false;

  newUfo->startTime = gameState.elapsedTime;
  newUfo->currentTime = newUfo->startTime;

  newUfo->size = UFO_SIZE;

  // generate initial position randomly
  newUfo->initPosition = generateRandomPosition();
  newUfo->position = newUfo->initPosition;
  // random speed in range 0.0f ... 1.0f
  newUfo->speed = (float)(rand() / (double)RAND_MAX);
  // random rotation speed in range 0.0f ... 1.0f
  newUfo->speed = UFO_ROTATION_SPEED_MAX * (float)(rand() / (double)RAND_MAX);

  // generate randomly in range -1.0f ... 1.0f
  newUfo->direction = glm::vec3(
    (float)(2.0 * (rand() / (double)RAND_MAX) - 1.0),
    (float)(2.0 * (rand() / (double)RAND_MAX) - 1.0),
    0.0f
  );
  newUfo->direction = glm::normalize(newUfo->direction);

  return newUfo;
}

void restartGame(void) {

  cleanUpObjects();

  gameState.elapsedTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME); // milliseconds => seconds

  // initialize space ship
  if(gameObjects.spaceShip == NULL)
    gameObjects.spaceShip = new SpaceShipObject;

  gameObjects.spaceShip->position = glm::vec3(0.0f, 0.0f, 0.0f);
  gameObjects.spaceShip->viewAngle = 90.0f; // degrees
  gameObjects.spaceShip->direction = glm::vec3(cos(glm::radians(gameObjects.spaceShip->viewAngle)), sin(glm::radians(gameObjects.spaceShip->viewAngle)), 0.0f);
  gameObjects.spaceShip->speed = 0.0f;
  gameObjects.spaceShip->size = SPACESHIP_SIZE;
  gameObjects.spaceShip->destroyed = false;
  gameObjects.spaceShip->startTime = gameState.elapsedTime;
  gameObjects.spaceShip->currentTime = gameObjects.spaceShip->startTime;

  // initialize asteroids
  for(int i=0; i<ASTEROIDS_COUNT_MIN; i++) {
    AsteroidObject* newAsteroid = createAsteroid();
    
    gameObjects.asteroids.push_back(newAsteroid); 
  }

  // reset key map
  for(int i=0; i<KEYS_COUNT; i++)
    gameState.keyMap[i] = false;

  gameState.gameOver = false;
  gameState.missileLaunchTime = -MISSILE_LAUNCH_TIME_DELAY;
  gameState.ufoMissileLaunchTime = -MISSILE_LAUNCH_TIME_DELAY;
}

void drawWindowContents() {

  // setup parallel projection
  glm::mat4 orthoProjectionMatrix = glm::ortho(
    -SCENE_WIDTH, SCENE_WIDTH,
    -SCENE_HEIGHT, SCENE_HEIGHT,
    -10.0f*SCENE_DEPTH, 10.0f*SCENE_DEPTH
  );
  // static viewpoint - top view
  glm::mat4 orthoViewMatrix = glm::lookAt(
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f)
  );

  // setup camera & projection transform
  glm::mat4 viewMatrix  = orthoViewMatrix;
  glm::mat4 projectionMatrix = orthoProjectionMatrix;

  // draw space ship
  drawSpaceShip(gameObjects.spaceShip, viewMatrix, projectionMatrix);

  CHECK_GL_ERROR(); 
  // draw asteroids
  for(GameObjectsList::iterator it = gameObjects.asteroids.begin(); it != gameObjects.asteroids.end(); ++it) {
    CHECK_GL_ERROR(); 

    AsteroidObject* asteroid = (AsteroidObject*)(*it);
    drawAsteroid(asteroid, viewMatrix, projectionMatrix);
  }

  // draw ufos
  for(GameObjectsList::iterator it = gameObjects.ufos.begin(); it != gameObjects.ufos.end(); ++it) {
    UfoObject* ufo = (UfoObject *)(*it);
    drawUfo(ufo, viewMatrix, projectionMatrix); 
  }

  if(gameState.gameOver == true) {
  }
}

// Called to update the display. You should call glutSwapBuffers after all of your
// rendering to display what you rendered.
void displayCallback() {
  GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

  glClear(mask);

  drawWindowContents();

  glutSwapBuffers();
}

// Called whenever the window is resized. The new window size is given, in pixels.
// This is an opportunity to call glViewport or glScissor to keep up with the change in size.
void reshapeCallback(int newWidth, int newHeight) {

  gameState.windowWidth = newWidth;
  gameState.windowHeight = newHeight;

  glViewport(0, 0, (GLsizei) newWidth, (GLsizei) newHeight);
}

void updateObjects(float elapsedTime) {

  // update space ship 
  float timeDelta = elapsedTime - gameObjects.spaceShip->currentTime;
  gameObjects.spaceShip->currentTime = elapsedTime;

  // update asteroids
  GameObjectsList::iterator it = gameObjects.asteroids.begin();
  while(it != gameObjects.asteroids.end()) {
    AsteroidObject* asteroid = (AsteroidObject*)(*it);

    if(asteroid->destroyed == true) {
      it = gameObjects.asteroids.erase(it);
    }
    else {
      // update asteroid
      float timeDelta = elapsedTime - asteroid->currentTime;

      asteroid->currentTime = elapsedTime;
      asteroid->position += timeDelta * asteroid->speed * asteroid->direction;

      ++it;
    }
  }

  // update missiles
  it = gameObjects.missiles.begin();
  while(it != gameObjects.missiles.end()) {
    MissileObject* missile = (MissileObject*)(*it);

    // update missile
    float timeDelta = elapsedTime - missile->currentTime;

    missile->currentTime = elapsedTime;
    missile->position += timeDelta * missile->speed * missile->direction;

    if((missile->currentTime-missile->startTime)*missile->speed > MISSILE_MAX_DISTANCE) 
      missile->destroyed = true;

    if(missile->destroyed == true) {
      it = gameObjects.missiles.erase(it);
    }
    else {
      ++it;
    }
  }

  // update ufos
  it = gameObjects.ufos.begin();
  while(it != gameObjects.ufos.end()) {
    UfoObject* ufo = (UfoObject*)(*it);

    if(ufo->destroyed == true) {
      it = gameObjects.ufos.erase(it);
    }
    else {
      // update ufo
      ufo->currentTime = elapsedTime;

      ++it;
    }
  }
}

// Callback responsible for the scene update
void timerCallback(int) {

  // update scene time
  gameState.elapsedTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME); // milliseconds => seconds

  // update objects in the scene
  updateObjects(gameState.elapsedTime);

  // generate new ufos randomly
  if(gameObjects.ufos.size() < UFOS_COUNT_MIN) {
    int howManyUfos = rand() % (UFOS_COUNT_MAX - UFOS_COUNT_MIN + 1);

    for(int i=0; i<howManyUfos; i++) {
      UfoObject* newUfo = createUfo();

      gameObjects.ufos.push_back(newUfo); 
    }
  }

  // generate new asteroids randomly
  if(gameObjects.asteroids.size() < ASTEROIDS_COUNT_MIN) {
    int howManyAsteroids = rand() % (ASTEROIDS_COUNT_MAX - ASTEROIDS_COUNT_MIN + 1);

    for(int i=0; i<howManyAsteroids; i++) {
      AsteroidObject* newAsteroid = createAsteroid();

      gameObjects.asteroids.push_back(newAsteroid); 
    }
  }

  // set timeCallback next invocation
  glutTimerFunc(33, timerCallback, 0);

  glutPostRedisplay();
}

// Called whenever a key on the keyboard was pressed. The key is given by the "keyPressed"
// parameter, which is in ASCII. It's often a good idea to have the escape key (ASCII value 27)
// to call glutLeaveMainLoop() to exit the program.
void keyboardCallback(unsigned char keyPressed, int mouseX, int mouseY) {
  
  switch(keyPressed) {
    case 27: // escape
      glutLeaveMainLoop();
      break;
    case 'r': // restart game
      restartGame();
      break;
    case 't': // teleport space ship
      if(gameState.gameOver != true)
        teleport();
      break;
    default:
      ; // printf("Unrecognized key pressed\n");
  }
}

// Called whenever a key on the keyboard was released. The key is given by
// the "keyReleased" parameter, which is in ASCII. 
void keyboardUpCallback(unsigned char keyReleased, int mouseX, int mouseY) {

  switch(keyReleased) {
    default:
      ; // printf("Unrecognized key released\n");
  }
}

// The special keyboard callback is triggered when keyboard function or directional
// keys are pressed.
void specialKeyboardCallback(int specKeyPressed, int mouseX, int mouseY) {

  if(gameState.gameOver == true)
    return;

  switch (specKeyPressed) {
    default:
      ; // printf("Unrecognized special key pressed\n");
  }
}

// The special keyboard callback is triggered when keyboard function or directional
// keys are released.
void specialKeyboardUpCallback(int specKeyReleased, int mouseX, int mouseY) {

  if(gameState.gameOver == true)
    return;

  switch (specKeyReleased) {
		default:
      ; // printf("Unrecognized special key released\n");
  }
}

// Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void initializeApplication() {

  // initialize random seed
  srand ((unsigned int)time(NULL));

  // initialize OpenGL
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glEnable(GL_DEPTH_TEST);

  // initialize shaders
  initializeShaderPrograms();
  // create geometry for all models used
  initializeModels();

  gameObjects.spaceShip = NULL;

  restartGame();
}

void finalizeApplication(void) {

  cleanUpObjects();

  delete gameObjects.spaceShip;
  gameObjects.spaceShip = NULL;

  // delete buffers - space ship, asteroid, missile, ufo, banner, and explosion
  cleanupModels();

  // delete shaders
  cleanupShaderPrograms();
}

int main(int argc, char** argv) {

  // initialize windowing system
  glutInit(&argc, argv);

  glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

  // initial window size
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow(WINDOW_TITLE);

  glutDisplayFunc(displayCallback);
  // register callback for change of window size
  glutReshapeFunc(reshapeCallback);
  // register callbacks for keyboard
  glutKeyboardFunc(keyboardCallback);
  glutKeyboardUpFunc(keyboardUpCallback);
  glutSpecialFunc(specialKeyboardCallback);     // key pressed
  glutSpecialUpFunc(specialKeyboardUpCallback); // key released

  glutTimerFunc(33, timerCallback, 0);

  // initialize PGR framework (GL, DevIl, etc.)
  if(!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
    pgr::dieWithError("pgr init failed, required OpenGL not supported?");

  initializeApplication();

  glutCloseFunc(finalizeApplication);

  glutMainLoop();

  return 0;
}
