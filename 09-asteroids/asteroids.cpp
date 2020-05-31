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


extern SCommonShaderProgram shaderProgram;
extern bool useLighting;

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

  GameObjectsList asteroids;
  GameObjectsList missiles;
  GameObjectsList ufos;

  GameObjectsList explosions;
  BannerObject* bannerObject; // NULL;
} gameObjects;

//**************************************************************************************************
/// Checks whether a given point is inside a sphere or not.
/**
 \param[in]  point      Point to be tested.
 \param[in]  center     Center of the sphere.
 \param[in]  radius     Radius of the sphere.
 \return                True if the point lies inside the sphere, otherwise false.
*/
bool pointInSphere(const glm::vec3 &point, const glm::vec3 &center, float radius) {
// ======== BEGIN OF SOLUTION - TASK 6_2-1 ======== //
  // check whether the given point lies within a sphere of given radius or not
    
    // varianta 1
    //float dx = point.x - center.x;
    //float dy = point.y - center.y;
    //float dz = point.z - center.z;
    //float dst = dx * dx + dy * dy + dz * dz;

    //if (dst <= radius * radius)
    //    return true;

    
    // varianta 2
    if (glm::dot(point - center, point - center) <= radius * radius)
        return true;


    // varianta 3
    //if (glm::distance(center, point) <= radius)
    //    return true;


// ========  END OF SOLUTION - TASK 6_2-1  ======== //

  return false;
}

//**************************************************************************************************
/// Checks if there is intersection between two given spheres or not.
/**
 \param[in]  center1    First sphere center.
 \param[in]  radius1    First sphere radius.
 \param[in]  center2    Second sphere center.
 \param[in]  radius2    Second sphere radius.
 \return                True if the spheres overlap, otherwise false.
*/
bool spheresIntersection(const glm::vec3 &center1, float radius1, const glm::vec3 &center2, float radius2) {
// ======== BEGIN OF SOLUTION - TASK 6_2-2 ======== //
  // check whether given spheres are intersecting each other or not
/*    glm::vec3 delta = center1 - center2;

    if (glm::dot(delta, delta) < (radius1 + radius2) * (radius1 + radius2))
        return true;*/

    float dx = center1.x - center2.x;
    float dy = center1.y - center2.y;
    float dz = center1.z - center2.z;
    float dst = dx * dx + dy * dy + dz * dz;

    if (glm::distance(center1, center2) <= (radius1 + radius2))
        return true;


// ========  END OF SOLUTION - TASK 6_2-2  ======== //

  return false;
}

void insertExplosion(const glm::vec3 &position) {

  ExplosionObject* newExplosion = new ExplosionObject;

  newExplosion->speed = 0.0f;
  newExplosion->destroyed = false;

  newExplosion->startTime = gameState.elapsedTime;
  newExplosion->currentTime = newExplosion->startTime;

  newExplosion->size = BILLBOARD_SIZE;
  newExplosion->direction = glm::vec3(0.0f, 0.0f, 1.0f);

  newExplosion->frameDuration = 0.1f;
  newExplosion->textureFrames = 16;

  newExplosion->position = position;

  gameObjects.explosions.push_back(newExplosion);
}

void increaseSpaceShipSpeed(float deltaSpeed = SPACESHIP_SPEED_INCREMENT) {

  gameObjects.spaceShip->speed =
    std::min(gameObjects.spaceShip->speed + deltaSpeed, SPACESHIP_SPEED_MAX);
}

void decreaseSpaceShipSpeed(float deltaSpeed = SPACESHIP_SPEED_INCREMENT) {

  gameObjects.spaceShip->speed =
    std::max(gameObjects.spaceShip->speed - deltaSpeed, 0.0f);
}

void turnSpaceShipLeft(float deltaAngle) {

  gameObjects.spaceShip->viewAngle += deltaAngle;

  if(gameObjects.spaceShip->viewAngle > 360.0f)
    gameObjects.spaceShip->viewAngle -= 360.0f;

  float angle = glm::radians(gameObjects.spaceShip->viewAngle);

  gameObjects.spaceShip->direction.x = cos(angle);
  gameObjects.spaceShip->direction.y = sin(angle);
}

void turnSpaceShipRight(float deltaAngle) {

  gameObjects.spaceShip->viewAngle -= deltaAngle;

  if(gameObjects.spaceShip->viewAngle < 0.0f)
    gameObjects.spaceShip->viewAngle += 360.0f;

  float angle = glm::radians(gameObjects.spaceShip->viewAngle);

  gameObjects.spaceShip->direction.x = cos(angle);
  gameObjects.spaceShip->direction.y = sin(angle);
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

  // delete explosions
  while(!gameObjects.explosions.empty()) {
    delete gameObjects.explosions.back();
    gameObjects.explosions.pop_back();
  } 

  // remove banner
  if(gameObjects.bannerObject != NULL) {
    delete gameObjects.bannerObject;
    gameObjects.bannerObject = NULL;
  }
}

// generates random position that does not collide with the spaceship
glm::vec3 generateRandomPosition(void) {
 glm::vec3 newPosition;
  bool invalidPosition = false;

  do {

    // position is generated randomly
    // coordinates are in range -1.0f ... 1.0f
    newPosition = glm::vec3(
      (float)(2.0 * (rand() / (double)RAND_MAX) - 1.0),
      (float)(2.0 * (rand() / (double)RAND_MAX) - 1.0),
      0.0f
    );
    invalidPosition = pointInSphere(newPosition, gameObjects.spaceShip->position, 3.0f*SPACESHIP_SIZE);

  } while (invalidPosition == true);

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

BannerObject* createBanner(void) {
 BannerObject* newBanner = new BannerObject;
 
  newBanner->size = BANNER_SIZE;
  newBanner->position = glm::vec3(0.0f, 0.0f, 0.0f);
  newBanner->direction = glm::vec3(0.0f, 1.0f, 0.0f);
  newBanner->speed = 0.0f;
  newBanner->size = 1.0f;

  newBanner->destroyed = false;

  newBanner->startTime = gameState.elapsedTime;
  newBanner->currentTime = newBanner->startTime;

  return newBanner;
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

    glm::vec3 cameraPosition = gameObjects.spaceShip->position;
    glm::vec3 cameraUpVector = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 cameraCenter;

    glm::vec3 cameraViewDirection = gameObjects.spaceShip->direction;

    glm::vec3 rotationAxis = glm::cross(cameraViewDirection, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 cameraTransform = glm::rotate(glm::mat4(1.0f), glm::radians(gameState.cameraElevationAngle), rotationAxis);

    cameraUpVector = glm::vec3(cameraTransform * glm::vec4(cameraUpVector, 0.0f));
    cameraViewDirection = glm::vec3(cameraTransform * glm::vec4(cameraViewDirection, 0.0f));

    cameraCenter = cameraPosition + cameraViewDirection;

    viewMatrix = glm::lookAt(
      cameraPosition,
      cameraCenter,
      cameraUpVector
    );

    projectionMatrix = glm::perspective(glm::radians(60.0f), gameState.windowWidth/(float)gameState.windowHeight, 0.1f, 10.0f);
  }

  glUseProgram(shaderProgram.program);
  glUniform1f(shaderProgram.timeLocation, gameState.elapsedTime);

  glUniform3fv(shaderProgram.reflectorPositionLocation, 1, glm::value_ptr(gameObjects.spaceShip->position));
  glUniform3fv(shaderProgram.reflectorDirectionLocation, 1, glm::value_ptr(gameObjects.spaceShip->direction));
  glUseProgram(0);

  // draw space ship
  drawSpaceShip(gameObjects.spaceShip, viewMatrix, projectionMatrix);

// ======== BEGIN OF SOLUTION - TASK 6_3-1 ======== //
  // enable stencil test
  glEnable(GL_STENCIL_TEST);


  // set the stencil operations - if the stencil test and depth test are passed than
  // value in the stencil buffer is replaced with the object ID (byte 1..255, 0 ... background)
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


// ========  END OF SOLUTION - TASK 6_3-1  ======== //
  CHECK_GL_ERROR(); 
  // draw asteroids
  int id = 0;
  for(GameObjectsList::iterator it = gameObjects.asteroids.begin(); it != gameObjects.asteroids.end(); ++it) {
// ======== BEGIN OF SOLUTION - TASK 6_3-2 ======== //
    // set the stencil test function
    // -> stencil test always passes and reference value for stencil test is set to be object ID (id+1)
      glStencilFunc(GL_ALWAYS, id+1, -1);


// ========  END OF SOLUTION - TASK 6_3-2  ======== //
    CHECK_GL_ERROR(); 

    AsteroidObject* asteroid = (AsteroidObject*)(*it);
    drawAsteroid(asteroid, viewMatrix, projectionMatrix);

    id++;
  }
  // disable stencil test
  glDisable(GL_STENCIL_TEST);

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

  // draw skybox
  drawSkybox(viewMatrix, projectionMatrix); 

  // draw explosions with depth test disabled
  glDisable(GL_DEPTH_TEST);

  for(GameObjectsList::iterator it = gameObjects.explosions.begin(); it != gameObjects.explosions.end(); ++it) {
    ExplosionObject* explosion = (ExplosionObject *)(*it);
    drawExplosion(explosion, viewMatrix, projectionMatrix); 
  }
  glEnable(GL_DEPTH_TEST);

  if(gameState.gameOver == true) {
    // draw game over banner
    if(gameObjects.bannerObject != NULL)
      drawBanner(gameObjects.bannerObject, orthoViewMatrix, orthoProjectionMatrix);
  }
}

// Called to update the display. You should call glutSwapBuffers after all of your
// rendering to display what you rendered.
void displayCallback() {
  GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
  mask |= GL_STENCIL_BUFFER_BIT;

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

// Test collisons between objects in the scene and insert explosion billboards.
void checkCollisions(void) {

  // test collisions between asteroid and spaceship
  GameObjectsList::iterator it;

  for(it = gameObjects.asteroids.begin(); it != gameObjects.asteroids.end(); ++it) {
    AsteroidObject * asteroid = (AsteroidObject *)(*it);

    if(asteroid->destroyed == false) {
      // check whether a given asteroid collides with spaceship or not
      // calls spheresIntersection() function with appropriate parameters
      if(spheresIntersection(gameObjects.spaceShip->position, gameObjects.spaceShip->size, asteroid->position, asteroid->size) == true) {
        asteroid->destroyed = true;	                       // mark asteroid dead
        insertExplosion(gameObjects.spaceShip->position);  // insert explostion billboard
        gameState.gameOver = true;                         // -> game over
      }
    }
  }

  // test collisions beween ufo and spaceship
  for(it = gameObjects.ufos.begin(); it != gameObjects.ufos.end(); ++it) {
    UfoObject * ufo = (UfoObject *)(*it);

    if(ufo->destroyed == false) {
      // check whether a given ufo collides with spaceship or not
      // calls spheresIntersection() function with appropriate parameters
      if(spheresIntersection(gameObjects.spaceShip->position, gameObjects.spaceShip->size, ufo->position, ufo->size) == true) {
        ufo->destroyed = true;	                           // mark ufo dead
        insertExplosion(gameObjects.spaceShip->position);  // insert explosion billboard
        gameState.gameOver = true;                         // -> game over
      }
    }
  }

  // check collisions missile x asteroid and missile x ufo (brute force)  
  for(it = gameObjects.missiles.begin(); it != gameObjects.missiles.end(); ++it) {
    MissileObject * missile = (MissileObject *)(*it);

      // test missile with each asteroid
    for(GameObjectsList::iterator itA = gameObjects.asteroids.begin(); itA != gameObjects.asteroids.end(); ++itA) {
      AsteroidObject * asteroid = (AsteroidObject *)(*itA);

      if(asteroid->destroyed == false) {
        // check whether a given missile hits asteroid or not
        // calls pointInSphere() function with proper parameters
        if(pointInSphere(missile->position, asteroid->position, asteroid->size) == true) {	
          missile->destroyed = true;           // remove missile
          asteroid->destroyed = true;          // remove asteroid
          insertExplosion(missile->position);  // insert explosion billboard

          // asteroid break-up into random number of parts
          if(asteroid->size > ASTEROID_SIZE_MIN) {
            int howManyAsteroids = rand() % ASTEROID_PARTS + 1;

            for(int i=0; i<howManyAsteroids; i++) {
              AsteroidObject* newAsteroid = createAsteroid();

              // same position, smaller size
              newAsteroid->position = asteroid->position;
              newAsteroid->size = asteroid->size * ASTEROID_SIZE_FACTOR;

              gameObjects.asteroids.push_front(newAsteroid); 
            }
          }
        }
      }
    }

    // test missile against each ufo
    for(GameObjectsList::iterator itU = gameObjects.ufos.begin(); itU != gameObjects.ufos.end(); ++itU) {
      UfoObject * ufo = (UfoObject *)(*itU);

      if(ufo->destroyed == false) {
        // check whether a given missile hits ufo or not
        // calls pointInSphere() function with proper parameters
        if(pointInSphere(missile->position, ufo->position, ufo->size) == true) {
          missile->destroyed = true;           // remove missile
          ufo->destroyed = true;               // mark ufo dead
          insertExplosion(missile->position);  // insert explosion billboard
        }
      }
    }

    // check whether a given missile hits spaceship or not
    // calls pointInSphere() function with proper parameters
    if(pointInSphere(missile->position, gameObjects.spaceShip->position, gameObjects.spaceShip->size) == true) {
      missile->destroyed = true;           // remove missile
      insertExplosion(missile->position);  // insert explosion billboard
      gameState.gameOver = true;           // -> game over
    }
  }
}

void updateObjects(float elapsedTime) {

  // update space ship 
  float timeDelta = elapsedTime - gameObjects.spaceShip->currentTime;
  gameObjects.spaceShip->currentTime = elapsedTime;
  gameObjects.spaceShip->position += timeDelta * gameObjects.spaceShip->speed * gameObjects.spaceShip->direction;

  // check the new position and wrap it if it is necessary
  gameObjects.spaceShip->position = checkBounds(gameObjects.spaceShip->position, gameObjects.spaceShip->size);

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

      // check the new position and wrap it if it is necessary
      asteroid->position = checkBounds(asteroid->position, asteroid->size);

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

    // check the new position and wrap it if it is necessary
    missile->position = checkBounds(missile->position, missile->size);

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

      float curveParamT = ufo->speed * (ufo->currentTime - ufo->startTime);

      ufo->position = ufo->initPosition + evaluateClosedCurve( curveData, curveSize, curveParamT );
      ufo->direction = glm::normalize(evaluateClosedCurve_1stDerivative(curveData, curveSize, curveParamT));

      // check the new position and wrap it if it is necessary
      ufo->position = checkBounds(ufo->position, ufo->size);

      ++it;
    }
  }

  // update explosion billboards
  it = gameObjects.explosions.begin();
  while(it != gameObjects.explosions.end()) {
    ExplosionObject* explosion = (ExplosionObject*)(*it);

    // update explosion
    explosion->currentTime = elapsedTime;

    if(explosion->currentTime > explosion->startTime + explosion->textureFrames*explosion->frameDuration)
      explosion->destroyed = true;

    if(explosion->destroyed == true) {
      it = gameObjects.explosions.erase(it);
    }
    else {
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

  if((gameState.gameOver == true) && (gameObjects.bannerObject != NULL)) {
    gameObjects.bannerObject->currentTime = gameState.elapsedTime;
  }

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

  // test collisions among objects in the scene
  checkCollisions();

  // generate new ufos randomly
  if(gameObjects.ufos.size() < UFOS_COUNT_MIN) {
    int howManyUfos = rand() % (UFOS_COUNT_MAX - UFOS_COUNT_MIN + 1);

    for(int i=0; i<howManyUfos; i++) {
      UfoObject* newUfo = createUfo();

      gameObjects.ufos.push_back(newUfo); 
    }
  }

  if(gameObjects.ufos.size() > 0 && (rand() % 100) == 0) { // generate ufo missile or not?
    int ufoIndex = rand() % gameObjects.ufos.size();       // which ufo should shoot?

    GameObjectsList::iterator it = gameObjects.ufos.begin();
    std::advance(it, ufoIndex); // now it points to the nth ufo of the list (counting from zero)
    UfoObject* ufo = (UfoObject*)(*it);

    // missile position and direction
    glm::vec3 missilePosition = ufo->position;
    glm::vec3 missileDirection = ufo->direction;

    missilePosition += missileDirection*1.5f*UFO_SIZE;

    createMissile(missilePosition, missileDirection, gameState.ufoMissileLaunchTime);
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
    if(gameObjects.bannerObject == NULL) {
      // if game over and banner still not created -> create banner
      gameObjects.bannerObject = createBanner();
    }
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
    case'e': { // insert explosion randomly
        glm::vec3 explosionPosition = glm::vec3(
          (float)(2.0 * (rand() / (double)RAND_MAX) - 1.0),
          (float)(2.0 * (rand() / (double)RAND_MAX) - 1.0),
          0.0f
        );
        insertExplosion(explosionPosition);
      }
      break;
    case'g': // game over
      gameState.gameOver = true;
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

// When a user presses and releases mouse buttons in the window, each press
// and each release generates a mouse callback.
void mouseCallback(int buttonPressed, int buttonState, int mouseX, int mouseY ) {

  // do picking only on mouse down
  if( ( buttonPressed == GLUT_LEFT_BUTTON ) && ( buttonState == GLUT_DOWN ) ) {

    // store value from the stencil buffer (byte)
    unsigned char asteroidID = 0; 
// ======== BEGIN OF SOLUTION - TASK 6_3-3 ======== //
    // read value from the stencil buffer for one pixel under the mouse cursor into asteroidID variable
    // recalculate y, as glut has origin in upper left corner

    glReadPixels(mouseX, (gameState.windowHeight - mouseY - 1), 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &asteroidID);

// ========  END OF SOLUTION - TASK 6_3-3  ======== //

    // the buffer was initially cleared to zeros
    if(asteroidID == 0) {
   		// background was clicked
      printf("Clicked on background\n");
    }
    else {
      // object was clicked
      printf("Clicked on object with ID: %d\n", (int)asteroidID);

      GameObjectsList::iterator it = gameObjects.asteroids.begin();
      std::advance(it, asteroidID-1); // now it points to the nth asteroid of the list (counting from zero)
      AsteroidObject* asteroid = (AsteroidObject*)(*it);

      if(asteroid->destroyed == false) {
        asteroid->destroyed = true;	          // remove asteroid
        insertExplosion(asteroid->position);  // insert explosion billboard
      }
    }
  }
}

// Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void initializeApplication() {

  // initialize random seed
  srand ((unsigned int)time(NULL));

  // initialize OpenGL
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClearStencil(0);  // this is the default value
  glEnable(GL_DEPTH_TEST);

  useLighting = true;

  // initialize shaders
  initializeShaderPrograms();
  // create geometry for all models used
  initializeModels();

  gameObjects.spaceShip = NULL;
  gameObjects.bannerObject = NULL;

  // test whether the curve segment is correctly computed (tasks 1 and 2)
  testCurve(evaluateCurveSegment, evaluateCurveSegment_1stDerivative);

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

  glutMouseFunc(mouseCallback);

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
