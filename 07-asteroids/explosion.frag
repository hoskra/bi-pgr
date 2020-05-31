#version 140

uniform float time;           // used for simulation of moving lights (such as sun) and to select proper animation frame
uniform mat4 Vmatrix;         // view (camera) transform --> world to eye coordinates
uniform sampler2D texSampler; // sampler for texture access

smooth in vec3 position_v;    // camera space fragment position
smooth in vec2 texCoord_v;    // fragment texture coordinates

out vec4 color_f;             // outgoing fragment color

// there are 8 frames in the row, two rows total
uniform ivec2 pattern = ivec2(8, 2);
// one frame lasts 0.1s
uniform float frameDuration = 0.1f;


vec4 sampleTexture(int frame) {

// ======== BEGIN OF SOLUTION - TASK 4_2-3 ======== //
  // use the frame parameter to calculate offset of the uv coordinates
  // use the texture function, texSampler as a first parameter and your calculated uv coordinates
  // the pattern uniform stores number of frames stored in the texture (in rows/cols)
  
  //vec2 texCoordBase = texCoord_v / vec2(pattern);
  //vec2 texCoord = texCoordBase + vec2(frame, (frame / pattern.x)) * offset;
  //return texture(texSampler, texCoord);

  vec2 uv = texCoord_v / pattern;
  vec2 size = vec2(1.0) / pattern;

  int colx = frame % pattern.x;     // 0, 1, 2, 3, 4, 5, 6, 7
  int rowy = (frame/ pattern.x) % pattern.y;

  vec2 offset = vec2(float(colx), float(rowy)) * size;
  
  return texture(texSampler, uv + offset);
// ========  END OF SOLUTION - TASK 4_2-3  ======== //
}

void main() {
  // frame of the texture to be used for explosion drawing 
  int frame = int(time / frameDuration);

  // sample proper frame of the texture to get a fragment color  
  color_f = sampleTexture(frame);
}
