#version 140

uniform mat4 PVMmatrix;     // Projection * View * Model --> model to clip coordinates
uniform float time;         // used for simulation of moving lights (such as sun)

in vec3 position;           // vertex position in world space
in vec2 texCoord;           // incoming texture coordinates

smooth out vec2 texCoord_v; // outgoing texture coordinates

float decay = 0.05;

void main() {

  // vertex position after the projection (gl_Position is predefined output variable)
  gl_Position = PVMmatrix * vec4(position, 1.0);   // outgoing vertex in clip coordinates

  float localTime = time * decay;
  // localTime = 0;

// ======== BEGIN OF SOLUTION - TASK 4_3-4 ======== //

  // play with the x (u) coordinate of offset variable to simulate the movement,
  // you should use initially the "time" uniform (in final version you have to replace
  // time uniform by "localTime" just to slow down the movement)
  // keep in mind that "game over" text has to cross the window during one second

  vec2 offset = vec2( 1.0 - localTime, 0.0f);

// ========  END OF SOLUTION - TASK 4_3-4  ======== //

  // outputs entering the fragment shader
  texCoord_v = texCoord + offset;
}
