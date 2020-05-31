Task 1 (done with the teacher)
------

* walk through the code of the skybox drawing


Task 2 [1 point]      >>> TASK 4_2-Y <<<
----------------
Draw the explosion animation represented as a sequence of 16 images repeatedly
changing over time. The whole sequence of images is stored in one big image in
8x2 layout (8 columns and 2 rows) -> see image data/explode.png. Explosion 
animation is mapped onto a billboard (simple rectangle) that is automatically 
rotated always to face the camera.

* Initial texture mapping has to be done in file data.h through the 
  explosionVertexData[] array. You have to append initial texture coordinates 
  to stretch the texture over the whole billboard (later inside the fragment 
  shader, the initial texture window 1.0x1.0 will be remapped into the proper
  subimage, i.e. frame of the animation through sampleTexture function).    
  >>> TASK 4_2-1 <<<

* Texture animation is accomplished by changing texture coordinates over 
  time according to the passed animation frame number (ranges for 0 to the number
  of frames minus one), i.e., the input texture coordinates from the initial 
  mapping (ranging from 0.0 to 1.0) has to be remapped to select proper
  subimage (animation frame) from the big texture. To do that, you have to 
  complete function sampleTexture() in fragment shader explosion.frag. 
  Texture layout is stored in uniform named "pattern".   >>> TASK 4_2-3 <<<

* Explosion texture has to be blended with the background. You have to enable 
  blending and set proper blending factors. In this case the texture has no
  alpha channel and therefore, it is simply added to the background.
  -> drawExplosion() in file render_stuff.cpp.           >>> TASK 4_2-2 <<<

* note: Through the EXPLOSION_TEXTURE_NAME constant in file render_stuff.cpp,
  you can change the name of the texture to be used for explosion animation 
  -> for debugging purposes you can use a texture with digits (file 
  digits.png) to see if the animation frames are switching in proper order. 


Task 3 [1 point]      >>> TASK 4_3-Y <<<
----------------
Mapping of texture with "game over" text to the triangle strip. Texture 
movement has to be done through the transformation of texture coordinates in
the vertex shader.

* Animate "game over" text movement by shifting the texture coordinates inside 
  the vertex shader stored in file banner.vert. The offset should be derived from 
  the "time" uniform, and its calculation has to ensure that text will scroll 
  through the whole window in one second. In final version, time has to be 
  replaced by localTime just to slow down the movement of the text. 
  >>> TASK 4_3-4 <<<

* At the beginning, the "game over" text has to appear on the left and it 
  should move to the right. When it completely rolls out the window, it has to
  emerge from the window left side again.                >>> TASK 4_3-1 <<<

* Triangle strip has to be subdivided in the direction of the u texture
  coordinate into three parts, but the texture has to be visible just on one 
  part each time. You have to assign texture coordinates correctly in array
  bannerVertexData[] in file data.h.                     >>> TASK 4_3-Y <<<

* Texture (i.e. "game over" text) has to be displayed on the triangle strip
  only once. Therefore, you have to set the proper texture wrap mode (wrap 
  or clamp) in file render_stuff.cpp within initBannerGeometry() function. 
                                                         >>> TASK 4_3-3 <<<

* Texture has to be blended with the background. You have to enable blending
  and set the proper blending factors. The texture contains the alpha channel 
  (texels of the text have the alpha value set to 1.0, and the others are set to 
  0.0) and therefore, it has to be mixed with the background with respect to 
  the alpha component of the incoming fragment color (i.e., the text is opaque, 
  so we see either the text or the background) -> drawBanner() in file 
  render_stuff.cpp.                                      >>> TASK 4_3-2 <<<


Notes:
------
* "e" key press generates an explosion randomly placed in the scene 
* "g" key press terminates the game -> generates rolling text "game over"

* parts of the source code that should be modified to fulfill the tasks are 
  marked by the following sequence of comments where X indicates the task 
  number and Y the subtask number:

    // ======== BEGIN OF SOLUTION - TASK 4_X-Y ======== //
    ...
    // ========  END OF SOLUTION - TASK 4_X-Y  ======== //



What files do you have to edit:
TASK 4_2:
 -> data.h: 260
 -> render_stuff.cpp: 277
 -> explosion.frag: 20
TASK 4_3:
 -> data.h: 279
 -> render_stuff.cpp: 319, 752
 -> banner.vert: 21
