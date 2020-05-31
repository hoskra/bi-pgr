//----------------------------------------------------------------------------------------
/**
 * \file    render_stuff.cpp
 * \author  Jaroslav Sloup, Tomas Barak, Petr Felkel
 * \date    2013
 * \brief   Rendering stuff - drawing functions for models, etc..
 */
//----------------------------------------------------------------------------------------

#include <iostream>
#include "pgr.h"
#include "render_stuff.h"
#include "data.h"
#include "spline.h"

MeshGeometry* asteroidGeometry = NULL;
MeshGeometry* spaceShipGeometry = NULL;
MeshGeometry* ufoGeometry = NULL;
MeshGeometry* missileGeometry = NULL;

const char* ASTEROID_MODEL_NAME = "data/asteroid.obj";
const char* SPACESHIP_MODEL_NAME = "data/ghoul.obj";

SCommonShaderProgram shaderProgram;


void setTransformUniforms(const glm::mat4 &modelMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {

  glm::mat4 PVM = projectionMatrix * viewMatrix * modelMatrix;
  glUniformMatrix4fv(shaderProgram.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));
}

void drawSpaceShip(SpaceShipObject *spaceShip, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

  glUseProgram(shaderProgram.program);

  // prepare modeling transform matrix
  glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), spaceShip->position);
  modelMatrix = glm::rotate(modelMatrix, glm::radians(spaceShip->viewAngle), glm::vec3(0, 0, 1));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(spaceShip->size, spaceShip->size, spaceShip->size));

  // send matrices to the vertex & fragment shader
  setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);

  // draw geometry
  glBindVertexArray(spaceShipGeometry->vertexArrayObject);
  glDrawElements(GL_TRIANGLES, spaceShipGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
  glUseProgram(0);

  return;
}

void drawAsteroid(AsteroidObject* asteroid, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

  glUseProgram(shaderProgram.program);

  glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), asteroid->position);
  modelMatrix = glm::scale(modelMatrix, glm::vec3(asteroid->size));

  // send matrices to the vertex & fragment shader
  setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);

  // draw geometry
  glBindVertexArray(asteroidGeometry->vertexArrayObject);
  glDrawElements(GL_TRIANGLES, asteroidGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
  glUseProgram(0);

  return;
}

void drawMissile(MissileObject* missile, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {
  
  glUseProgram(shaderProgram.program);

  // align missile coordinate system to match its position and direction - see alignObject() function
  glm::mat4 modelMatrix = alignObject(missile->position, missile->direction, glm::vec3(0.0f, 0.0f, 1.0f));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(missile->size));

  // angular speed = 2*pi*frequency => path = angular speed * time
  const float frequency = 2.0f; // per second
  const float angle = 2.0f*M_PI * frequency * (missile->currentTime-missile->startTime); // angle in radians
  modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));

  // send matrices to the vertex & fragment shader
  setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);
  // draw the missile using glDrawArrays 
  glBindVertexArray(missileGeometry->vertexArrayObject);
  glDrawArrays(GL_TRIANGLES, 0, missileGeometry->numTriangles*3);

  glBindVertexArray(0);
  glUseProgram(0);

  return;
}

void drawUfo(UfoObject* ufo, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

  glUseProgram(shaderProgram.program);

  // align ufo coordinate system to match its position and direction - see alignObject() function
  glm::mat4 modelMatrix = alignObject(ufo->position, ufo->direction, glm::vec3(0.0f, 0.0f, 1.0f));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(ufo->size));

  // send matrices to the vertex & fragment shader
  setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);
// ======== BEGIN OF SOLUTION - TASK 1_2-2 ======== //
  // draw the first three (yellow) triangles of ufo top using glDrawArrays 
  glBindVertexArray(ufoGeometry->vertexArrayObject);
  glDrawArrays(GL_TRIANGLES, 0, ufoGeometry->numTriangles *3); // <------- comment this line to view just the bottom

// ========  END OF SOLUTION - TASK 1_2-2  ======== //
  CHECK_GL_ERROR();
// ======== BEGIN OF SOLUTION - TASK 1_2-3 ======== //
  // draw the second three (magenta) triangles of ufo top using glDrawArrays 
 glDrawArrays(GL_TRIANGLES, ufoGeometry->numTriangles * 3/2, ufoGeometry->numTriangles * 3/2); // <------- comment this line to view just the bottom
// ========  END OF SOLUTION - TASK 1_2-3  ======== //
  CHECK_GL_ERROR();

// ======== BEGIN OF SOLUTION - TASK 1_3-3 ======== //
  // draw the six triangles of ufo bottom using glDrawElements
  glDrawElements(GL_TRIANGLES, ufoGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

// ========  END OF SOLUTION - TASK 1_3-3  ======== //
  CHECK_GL_ERROR();

  glBindVertexArray(0);
  glUseProgram(0);

  return;
}

void cleanupShaderPrograms(void) {

  pgr::deleteProgramAndShaders(shaderProgram.program);

}

void initializeShaderPrograms(void) {

  std::vector<GLuint> shaderList;

    // load and compile simple shader (colors only, no lights at all)

    // push vertex shader and fragment shader
    shaderList.push_back(pgr::createShaderFromSource(GL_VERTEX_SHADER, colorVertexShaderSrc));
    shaderList.push_back(pgr::createShaderFromSource(GL_FRAGMENT_SHADER, colorFragmentShaderSrc));

    // create the program with two shaders (fragment and vertex)
    shaderProgram.program = pgr::createProgram(shaderList);
    // get position and color attributes locations
    shaderProgram.posLocation   = glGetAttribLocation(shaderProgram.program, "position");
    shaderProgram.colorLocation = glGetAttribLocation(shaderProgram.program, "color");
    // get uniforms locations
    shaderProgram.PVMmatrixLocation = glGetUniformLocation(shaderProgram.program, "PVMmatrix");

}

/** Load mesh using assimp library
 *  Vertex, normals and texture coordinates data are stored without interleaving |VVVVV...|NNNNN...|tttt
 * \param fileName [in] file to open/load
 * \param shader [in] vao will connect loaded data to shader
 * \param geometry
 */
bool loadSingleMesh(const std::string &fileName, SCommonShaderProgram& shader, MeshGeometry** geometry) {
  Assimp::Importer importer;

  // Unitize object in size (scale the model to fit into (-1..1)^3)
  importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1);

  // Load asset from the file - you can play with various processing steps
  const aiScene * scn = importer.ReadFile(fileName.c_str(), 0
      | aiProcess_Triangulate             // Triangulate polygons (if any).
      | aiProcess_PreTransformVertices    // Transforms scene hierarchy into one root with geometry-leafs only. For more see Doc.
      | aiProcess_GenSmoothNormals        // Calculate normals per vertex.
      | aiProcess_JoinIdenticalVertices);

  // abort if the loader fails
  if(scn == NULL) {
    std::cerr << "assimp error: " << importer.GetErrorString() << std::endl;
    *geometry = NULL;
    return false;
  }

  // some formats store whole scene (multiple meshes and materials, lights, cameras, ...) in one file, we cannot handle that in our simplified example
  if(scn->mNumMeshes != 1) {
    std::cerr << "this simplified loader can only process files with only one mesh" << std::endl;
    *geometry = NULL;
    return false;
  }

  // in this phase we know we have one mesh in our loaded scene, we can directly copy its data to OpenGL ...
  const aiMesh * mesh = scn->mMeshes[0];

  *geometry = new MeshGeometry;

  // vertex buffer object, store all vertex positions and normals
  glGenBuffers(1, &((*geometry)->vertexBufferObject));
  glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, 8*sizeof(float)*mesh->mNumVertices, 0, GL_STATIC_DRAW); // allocate memory for vertices, normals, and texture coordinates
  // first store all vertices
  glBufferSubData(GL_ARRAY_BUFFER, 0, 3*sizeof(float)*mesh->mNumVertices, mesh->mVertices);
  // then store all normals
  glBufferSubData(GL_ARRAY_BUFFER, 3*sizeof(float)*mesh->mNumVertices, 3*sizeof(float)*mesh->mNumVertices, mesh->mNormals);
  
  // just texture 0 for now
  float *textureCoords = new float[2 * mesh->mNumVertices];  // 2 floats per vertex
  float *currentTextureCoord = textureCoords;

  // copy texture coordinates
  aiVector3D vect;
    
  if(mesh->HasTextureCoords(0) ) {
    // we use 2D textures with 2 coordinates and ignore the third coordinate
    for(unsigned int idx=0; idx<mesh->mNumVertices; idx++) {
      vect = (mesh->mTextureCoords[0])[idx];
      *currentTextureCoord++ = vect.x;
      *currentTextureCoord++ = vect.y;
    }
  }
    
  // finally store all texture coordinates
  glBufferSubData(GL_ARRAY_BUFFER, 6*sizeof(float)*mesh->mNumVertices, 2*sizeof(float)*mesh->mNumVertices, textureCoords);

  // copy all mesh faces into one big array (assimp supports faces with ordinary number of vertices, we use only 3 -> triangles)
  unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];
  for(unsigned int f = 0; f < mesh->mNumFaces; ++f) {
    indices[f*3 + 0] = mesh->mFaces[f].mIndices[0];
    indices[f*3 + 1] = mesh->mFaces[f].mIndices[1];
    indices[f*3 + 2] = mesh->mFaces[f].mIndices[2];
  }

  // copy our temporary index array to OpenGL and free the array
  glGenBuffers(1, &((*geometry)->elementBufferObject));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->elementBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned) * mesh->mNumFaces, indices, GL_STATIC_DRAW);

  delete [] indices;

  // copy the material info to MeshGeometry structure
  const aiMaterial *mat  = scn->mMaterials[mesh->mMaterialIndex];
  aiColor4D color;
  aiString name;
  aiReturn retValue = AI_SUCCESS;

  // Get returns: aiReturn_SUCCESS 0 | aiReturn_FAILURE -1 | aiReturn_OUTOFMEMORY -3
  mat->Get(AI_MATKEY_NAME, name); // may be "" after the input mesh processing. Must be aiString type!

  if((retValue = aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &color)) != AI_SUCCESS)
    color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);


  glGenVertexArrays(1, &((*geometry)->vertexArrayObject));
  glBindVertexArray((*geometry)->vertexArrayObject);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->elementBufferObject); // bind our element array buffer (indices) to vao
  glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);

  glEnableVertexAttribArray(shader.posLocation);
  glVertexAttribPointer(shader.posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	  glDisableVertexAttribArray(shader.colorLocation);
	  // following line is problematic on AMD/ATI graphic cards
	  // -> if you see black screen (no objects at all) than try to set color manually in vertex shader to see at least something
    glVertexAttrib3f(shader.colorLocation, color.r, color.g, color.b);
  CHECK_GL_ERROR();

  glBindVertexArray(0);

  (*geometry)->numTriangles = mesh->mNumFaces;

  return true;
}

void initMissileGeometry(SCommonShaderProgram &shader, MeshGeometry **geometry) {

  *geometry = new MeshGeometry;

  glGenVertexArrays(1, &((*geometry)->vertexArrayObject));
  glBindVertexArray((*geometry)->vertexArrayObject);

  glGenBuffers(1, &((*geometry)->vertexBufferObject));
  glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(missileVertices), missileVertices, GL_STATIC_DRAW);
  CHECK_GL_ERROR();

  glEnableVertexAttribArray(shader.posLocation);
  // vertices of triangles - start at the beginning of the array
  glVertexAttribPointer(shader.posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(shader.colorLocation);
    // colors of vertices start after the positions
    glVertexAttribPointer(shader.colorLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)(missileTrianglesCount * 3 * 3 * sizeof(float)));

  glBindVertexArray(0);

  (*geometry)->numTriangles = missileTrianglesCount;
}

void initUfoGeometry(SCommonShaderProgram &shader, MeshGeometry **geometry) {

  *geometry = new MeshGeometry;
// ======== BEGIN OF SOLUTION - TASK 1_2-4 ======== //
  glGenVertexArrays(1, &((*geometry)->vertexArrayObject));
  glBindVertexArray((*geometry)->vertexArrayObject);

  glGenBuffers(1, &((*geometry)->vertexBufferObject));
  glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ufoVertices), ufoVertices, GL_STATIC_DRAW);
  CHECK_GL_ERROR();

  //
  glGenBuffers(1, &((*geometry)->elementBufferObject));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->elementBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ufoIndices), ufoIndices, GL_STATIC_DRAW);
  CHECK_GL_ERROR();




  glEnableVertexAttribArray(shader.posLocation);
  glVertexAttribPointer(shader.posLocation, 3, GL_FLOAT, GL_FALSE, 6 *sizeof(float), (void*)0);

  glEnableVertexAttribArray(shader.colorLocation);
  glVertexAttribPointer(shader.colorLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3*sizeof(float) ));

// ========  END OF SOLUTION - TASK 1_2-4  ======== //

  glBindVertexArray(0);

  (*geometry)->numTriangles = ufoTrianglesCount;
}


/** Initialize vertex buffers and vertex arrays for all objects. 
 */
void initializeModels() {

  // load asteroid model from external file
  if(loadSingleMesh(ASTEROID_MODEL_NAME, shaderProgram, &asteroidGeometry) != true) {
    std::cerr << "initializeModels(): Asteroid model loading failed." << std::endl;
  }
  CHECK_GL_ERROR();

  // load space ship model from external file
  if(loadSingleMesh(SPACESHIP_MODEL_NAME, shaderProgram, &spaceShipGeometry) != true) {
    std::cerr << "initializeModels(): Space ship model loading failed." << std::endl;
  }
  CHECK_GL_ERROR();

  // fill MeshGeometry structure for missile object
  initMissileGeometry(shaderProgram, &missileGeometry);

  // fill MeshGeometry structure for ufo object
  initUfoGeometry(shaderProgram, &ufoGeometry);

}

void cleanupGeometry(MeshGeometry *geometry) {

  glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
  glDeleteBuffers(1, &(geometry->elementBufferObject));
  glDeleteBuffers(1, &(geometry->vertexBufferObject));

}

void cleanupModels() {

  cleanupGeometry(spaceShipGeometry);
  cleanupGeometry(asteroidGeometry);
  cleanupGeometry(missileGeometry);
  cleanupGeometry(ufoGeometry);
}
