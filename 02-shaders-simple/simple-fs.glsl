#version 140
out vec4 fragmentColor;

uniform int iTask;     // task number
// ======== BEGIN OF SOLUTION - TASK 3-4 ======== //
// declare winWidth and alpha uniforms 
// winWidth will be int and it will store window width in pixels 
// alpha will be float and it will store an animation parameter in the interval [0, 1] 
// ========  END OF SOLUTION - TASK 3-4  ======== //
uniform int winWidth;
uniform float alpha;
// ======== BEGIN OF SOLUTION - TASK 4-1 ======== //
// define the input variable color 
in vec4 color;
// ========  END OF SOLUTION - TASK 4-1  ======== //

uniform mat4  mPVM;   // transformation matrix

void task1()
{
  // ======== BEGIN OF SOLUTION - TASK 1-1 ======== //
  // set a constant red color of the fragment 
  fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
  // ========  END OF SOLUTION - TASK 1-1  ======== //
}

void task2()
{
  // ======== BEGIN OF SOLUTION - TASK 2-1 ======== //
  // set red color to the fragments on the window diagonal and the rest green  
  if(abs(gl_FragCoord.x - gl_FragCoord.y) > 40){
    fragmentColor = vec4(0.0, 1.0, 0.0, 1.0);
  } else {
    fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
  }
  
  // ========  END OF SOLUTION - TASK 2-1  ======== //
}

void task3()
{
  // ======== BEGIN OF SOLUTION - TASK 3-5 ======== //
  // use winWidth * alpha as a separator and 
  // color left part of the drawn fragments to red and right part of the fragments to green 
  // HINT: which input built-in variable stores the fragment window-relative coordinates? 
  if(gl_FragCoord.x < winWidth * alpha) {
    fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
  }else {
    fragmentColor = vec4(0.0, 1.0, 0.0, 1.0);
  }


  // ========  END OF SOLUTION - TASK 3-5  ======== //
}

void task4()
{
  // ======== BEGIN OF SOLUTION - TASK 4-2 ======== //
  // set a the color to the color of the vertex [defined in VS] 
  fragmentColor = color;

  // ========  END OF SOLUTION - TASK 4-2  ======== //
}


void main()
{
  switch(iTask)
  {
    case 1:
      task1();
      break;
    case 2:
      task2();
      break;
    case 3:
      task3();
      break;
    case 4:
    case 5:
    case 6:
    case 7:
      task4();  // copy the color
      break;
    default:
      task1();
  }
}
