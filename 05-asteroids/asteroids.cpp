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

  bool freeCameraMode;        // false;
  float cameraElevationAngle; // in degrees = initially 0.0f

  bool gameOver;              // false;
  bool keyMap[KEYS_COUNT];    // false

  float elapsedTime;
  float missileLaunchTime;
  float ufoMissileLaunchTime;

} gameState;

struct GameObjects {

  SpaceShipObject *spaceShip; // NULL

  GameObjectsList asteroids;     // BOD
  GameObjectsList missiles;      // vector
  GameObjectsList ufos;

} gameObjects;

// typedef struct Spaceshi

void increaseSpaceShipSpeed(float deltaSpeed = SPACESHIP_SPEED_INCREMENT) {

// ======== BEGIN OF SOLUTION - TASK 2_2-1 ======== //
  // increase the space ship speed by deltaSpeed
  // speed should not exceed the SPACESHIP_SPEED_MAX

    gameObjects.spaceShip->speed += deltaSpeed;
    if (gameObjects.spaceShip->speed > SPACESHIP_SPEED_MAX)
        gameObjects.spaceShip->speed = SPACESHIP_SPEED_MAX;


// ========  END OF SOLUTION - TASK 2_2-1  ======== //
}

void decreaseSpaceShipSpeed(float deltaSpeed = SPACESHIP_SPEED_INCREMENT) {

// ======== BEGIN OF SOLUTION - TASK 2_2-2 ======== //
  // decrease the space ship speed by deltaSpeed
  // speed have to be always greater or equal to zero

    gameObjects.spaceShip->speed -= deltaSpeed;
    if (gameObjects.spaceShip->speed < 0.0f)
        gameObjects.spaceShip->speed = 0.0f;

// ========  END OF SOLUTION - TASK 2_2-2  ======== //
}

void turnSpaceShipLeft(float deltaAngle) {

// ======== BEGIN OF SOLUTION - TASK 2_2-3 ======== //
  // update space ship view angle (azimuth) by deltaAngle and recalculate its direction
  // keep always view angle in range <0,360> degrees

    gameObjects.spaceShip->viewAngle += deltaAngle;

    if (gameObjects.spaceShip->viewAngle > 360.0f)
    {
        gameObjects.spaceShip->viewAngle -= 360.0f;
    }

    float angleInRadians = glm::radians(gameObjects.spaceShip->viewAngle);

    gameObjects.spaceShip->direction.x = cos(angleInRadians);
    gameObjects.spaceShip->direction.y = sin(angleInRadians);
        


// ========  END OF SOLUTION - TASK 2_2-3  ======== //
}

void turnSpaceShipRight(float deltaAngle) {

// ======== BEGIN OF SOLUTION - TASK 2_2-4 ======== //
  // update space ship view angle (azimuth) by deltaAngle and recalculate its direction
  // keep always view angle in range <0,360> degrees

    gameObjects.spaceShip->viewAngle -= deltaAngle;

    if (gameObjects.spaceShip->viewAngle < 0.0f)
    {
        gameObjects.spaceShip->viewAngle += 360.0f;
    }

    float angleInRadians = glm::radians(gameObjects.spaceShip->viewAngle);

    gameObjects.spaceShip->direction.x = cos(angleInRadians);
    gameObjects.spaceShip->direction.y = sin(angleInRadians);



// ========  END OF SOLUTION - TASK 2_2-4  ======== //
}

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

  // delete missiles
  while(!gameObjects.missiles.empty()) {
    delete gameObjects.missiles.back();
    gameObjects.missiles.pop_back();
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

  if(gameState.freeCameraMode == true) {
    gameState.freeCameraMode = false;
    glutPassiveMotionFunc(NULL);
  }
  gameState.cameraElevationAngle = 0.0f;

  // reset key map
  for(int i=0; i<KEYS_COUNT; i++)
    gameState.keyMap[i] = false;

  gameState.gameOver = false;
  gameState.missileLaunchTime = -MISSILE_LAUNCH_TIME_DELAY;
  gameState.ufoMissileLaunchTime = -MISSILE_LAUNCH_TIME_DELAY;
}

void createMissile(const glm::vec3 &missilePosition, const glm::vec3 &missileDirection, float &missileLaunchTime) {

  float currentTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME); // milliseconds => seconds
  if(currentTime-missileLaunchTime < MISSILE_LAUNCH_TIME_DELAY)
    return;

  missileLaunchTime = currentTime;

  MissileObject* newMissile = new MissileObject;

  newMissile->destroyed   = false;
  newMissile->startTime   = gameState.elapsedTime;
  newMissile->currentTime = newMissile->startTime;
  newMissile->size        = MISSILE_SIZE;
  newMissile->speed       = MISSILE_SPEED;
  newMissile->position    = missilePosition;
  newMissile->direction   = glm::normalize(missileDirection);
  
  gameObjects.missiles.push_back(newMissile); 
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

  if(gameState.freeCameraMode == true) {

// ======== BEGIN OF SOLUTION - TASK 2_2-5 ======== //
    // compute correctly camera position, direction, and up vector using space ship parameters


    // glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 cameraCenter   = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraPosition = gameObjects.spaceShip->position;
    glm::vec3 cameraUpVector = glm::vec3(0.0f, 0.0f, 1.0f);

    glm::vec3 cameraViewDirection = gameObjects.spaceShip->direction;
// ========  END OF SOLUTION - TASK 2_2-5  ======== //

// ======== BEGIN OF SOLUTION - TASK 2_3-1 ======== //
    // update camera view direction (through variable cameraCenter) and cameraUpVector by gameState.cameraElevationAngle variable


    glm::vec3 rotationAxis = glm::cross(cameraViewDirection, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 cameraTransform = glm::rotate(
        glm::mat4(1.0f),
        glm::radians(gameState.cameraElevationAngle),
        rotationAxis);


    cameraUpVector = glm::vec3(cameraTransform * glm::vec4(cameraUpVector, 0.0f));
    cameraViewDirection = glm::vec3(cameraTransform * glm::vec4(cameraViewDirection, 0.0f));

    // ========  END OF SOLUTION - TASK 2_3-1  ======== //

// ======== BEGIN OF SOLUTION - TASK 2_2-6 ======== //
    cameraCenter = cameraPosition + cameraViewDirection;
// ========  END OF SOLUTION - TASK 2_2-6  ======== //

    viewMatrix = glm::lookAt(
      cameraPosition,
      cameraCenter,
      cameraUpVector
    );

    projectionMatrix = glm::perspective(glm::radians(60.0f), gameState.windowWidth/(float)gameState.windowHeight, 0.1f, 10.0f);
  }

  // draw space ship
  drawSpaceShip(gameObjects.spaceShip, viewMatrix, projectionMatrix);

  CHECK_GL_ERROR(); 
  // draw asteroids
  for(GameObjectsList::iterator it = gameObjects.asteroids.begin(); it != gameObjects.asteroids.end(); ++it) {
    CHECK_GL_ERROR(); 

    AsteroidObject* asteroid = (AsteroidObject*)(*it);
    drawAsteroid(asteroid, viewMatrix, projectionMatrix);
  }

  // draw missiles
  for(GameObjectsList::iterator it = gameObjects.missiles.begin(); it != gameObjects.missiles.end(); ++it) {
    MissileObject* missile = (MissileObject *)(*it);
    drawMissile(missile, viewMatrix, projectionMatrix); 
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
// ======== BEGIN OF SOLUTION - TASK 2_2-7 ======== //
  // update space ship position taking into account timeDelta, space ship speed, and space ship direction

  gameObjects.spaceShip->position += gameObjects.spaceShip->speed * timeDelta * gameObjects.spaceShip->direction;

// ========  END OF SOLUTION - TASK 2_2-7  ======== //

  //if (gameObjects.spaceShip->position.x < -1.0f)
  //    gameObjects.spaceShip->position.x += 2.0f;
  //if (gameObjects.spaceShip->position.x < 1.0f)
  //    gameObjects.spaceShip->position.x -= 2.0f;
  //if (gameObjects.spaceShip->position.y > -1.0f)
  //    gameObjects.spaceShip->position.y += 2.0f;
 // if (gameObjects.spaceShip->position.y > 1.0f)
 //     gameObjects.spaceShip->position.y -= 2.0f;



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

  // call appropriate actions according to the currently pressed keys in key map
  // (combinations of keys are supported but not used in this implementation)
  if(gameState.keyMap[KEY_RIGHT_ARROW] == true)
    turnSpaceShipRight(SPACESHIP_VIEW_ANGLE_DELTA);

  if(gameState.keyMap[KEY_LEFT_ARROW] == true)
    turnSpaceShipLeft(SPACESHIP_VIEW_ANGLE_DELTA);

  if(gameState.keyMap[KEY_UP_ARROW] == true)
    increaseSpaceShipSpeed();

  if(gameState.keyMap[KEY_DOWN_ARROW] == true)
    decreaseSpaceShipSpeed();

  // update objects in the scene
  updateObjects(gameState.elapsedTime);

  // space pressed -> launch missile
  if(gameState.keyMap[KEY_SPACE] == true) {
    // missile position and direction
    glm::vec3 missilePosition = gameObjects.spaceShip->position;
    glm::vec3 missileDirection = gameObjects.spaceShip->direction;

    missilePosition += missileDirection*1.5f*SPACESHIP_SIZE;

    createMissile(missilePosition, missileDirection, gameState.missileLaunchTime);
  }

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

  // game over? -> create banner with scrolling text "game over"
  if(gameState.gameOver == true) {
    gameState.keyMap[KEY_SPACE] = false;
  }

  // set timeCallback next invocation
  glutTimerFunc(33, timerCallback, 0);

  glutPostRedisplay();
}

// Called when mouse is moving while no mouse buttons are pressed.
void passiveMouseMotionCallback(int mouseX, int mouseY) {

  if(mouseY != gameState.windowHeight/2) {

    float cameraElevationAngleDelta = 0.5f * (mouseY - gameState.windowHeight/2);

    if(fabs(gameState.cameraElevationAngle+cameraElevationAngleDelta) < CAMERA_ELEVATION_MAX)
      gameState.cameraElevationAngle += cameraElevationAngleDelta;

      // set mouse pointer to the window center
      glutWarpPointer(gameState.windowWidth/2, gameState.windowHeight/2);

      glutPostRedisplay();
    }
}

// Called whenever a key on the keyboard was pressed. The key is given by the "keyPressed"
// parameter, which is in ASCII. It's often a good idea to have the escape key (ASCII value 27)
// to call glutLeaveMainLoop() to exit the program.
void keyboardCallback(unsigned char keyPressed, int mouseX, int mouseY) {
  
  switch(keyPressed) {
    case 27: // escape
#ifndef __APPLE__
        glutLeaveMainLoop();
#else
        exit(0);
#endif
      break;
    case 'r': // restart game
      restartGame();
      break;
    case ' ': // launch missile
      if(gameState.gameOver != true)
        gameState.keyMap[KEY_SPACE] = true;
      break;
    case 't': // teleport space ship
      if(gameState.gameOver != true)
        teleport();
      break;
    case 'c': // switch camera
      gameState.freeCameraMode = !gameState.freeCameraMode;
      if(gameState.freeCameraMode == true) {
        glutPassiveMotionFunc(passiveMouseMotionCallback);
        glutWarpPointer(gameState.windowWidth/2, gameState.windowHeight/2);
      }
      else {
        glutPassiveMotionFunc(NULL);
      }
      break;
    default:
      ; // printf("Unrecognized key pressed\n");
  }
}

// Called whenever a key on the keyboard was released. The key is given by
// the "keyReleased" parameter, which is in ASCII. 
void keyboardUpCallback(unsigned char keyReleased, int mouseX, int mouseY) {

  switch(keyReleased) {
    case ' ':
      gameState.keyMap[KEY_SPACE] = false;
      break;
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
    case GLUT_KEY_RIGHT:
      gameState.keyMap[KEY_RIGHT_ARROW] = true;
      break;
    case GLUT_KEY_LEFT:
      gameState.keyMap[KEY_LEFT_ARROW] = true;
      break;
    case GLUT_KEY_UP:
      gameState.keyMap[KEY_UP_ARROW] = true;
      break;
    case GLUT_KEY_DOWN:
      gameState.keyMap[KEY_DOWN_ARROW] = true;
      break;
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
	case GLUT_KEY_RIGHT:
      gameState.keyMap[KEY_RIGHT_ARROW] = false;
      break;
    case GLUT_KEY_LEFT:
      gameState.keyMap[KEY_LEFT_ARROW] = false;
      break;
    case GLUT_KEY_UP:
      gameState.keyMap[KEY_UP_ARROW] = false;
      break;
    case GLUT_KEY_DOWN:
      gameState.keyMap[KEY_DOWN_ARROW] = false;
      break;
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

#ifndef __APPLE__
  glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
#else
  glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
#endif

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

#ifndef __APPLE__
  glutCloseFunc(finalizeApplication);
#else
  glutWMCloseFunc(finalizeApplication);
#endif

  glutMainLoop();

  return 0;
}
