#define TITLE "C3P0"
#include <GL/glut.h>
#include "map.h"
#include <string>
#include <iostream>
#include <cmath>
#include <sstream>
#include <cstdlib>
#include <ctime>

int Window_Height = 800;
int Window_Width = 800;
int Window_ID;
unsigned long long renderKey;

//lookAt arguments
double eye_X  = 217.7;
double eye_Y  = 0.25;
double eye_Z  = -207.25;
double at_X   = 217.7;
double at_Y   = 0.2;
double at_Z   = -208.25;
double up_X   = 0.0;
double up_Y   = 2.0;
double up_Z   = 0.0;


unsigned long long generateKey() {
    unsigned long long temp = 1;
    srand(time(0));
    for(int i = 0; i < 10; ++i) {
        temp *= rand();
        std::cout << temp << "    " << temp % 1000 << "     " << temp / 1000 << std::endl;
    }
    return temp;
}

void renderScene()
{
    map streets;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    gluLookAt(eye_X, eye_Y, eye_Z,  /* eye coordinates */
              at_X, at_Y, at_Z,  /* At coordinates */
              up_X, up_Y, up_Z); /* Up vector */

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    streets.render(renderKey);

    glutSwapBuffers();


}

void resizeScene(int Width, int Height)
{
    if (Height == 0)
        Height = 1;

    glViewport(0, 0, Width, Height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(left, right, bottom, top, near, far);
    gluPerspective(45.0f, (GLfloat)Width/(GLfloat)Height, 0.1f, 200.0f);

    glMatrixMode(GL_MODELVIEW);
    Window_Width  = Width;
    Window_Height = Height;
}

void init(int Width, int Height)
{
    glClearColor(0.392f, 0.584f, 0.929f, 1.0f);

    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    resizeScene(Width,Height);
    renderKey = generateKey();
}

void keyboardFunction(unsigned char key, int x, int y)
{
    switch (key)
    {
    //wasd controls for moving the camera around
    case 'w':
        eye_Z -= 1;
        at_Z -= 1;
        //up_Z -= 1;
        break;
    case 'a':
        eye_X -= 1;
        at_X -= 1;
        //up_X -= 1;
        break;
    case 's' :
        eye_Z += 1;
        at_Z += 1;
        //up_Z += 1;
        break;
    case 'd' :
        eye_X += 1;
        at_X += 1;
        //up_X += 1;
        break;
    //q to rotate 90 degrees to the left
    case 'q' :
        if(at_X == eye_X && at_Z == (eye_Z - 1)) {
            at_X = eye_X - 1;
            at_Z = eye_Z;
        } else if(at_X == (eye_X - 1) && at_Z == eye_Z) {
            at_X = eye_X;
            at_Z = eye_Z + 1;
        } else if(at_X == eye_X && at_Z == (eye_Z + 1)) {
            at_X = eye_X + 1;
            at_Z = eye_Z;
        } else {
            at_X = eye_X;
            at_Z = eye_Z - 1;
        }
        break;
    //e to rotate 90 degrees to the right
    case 'e' :
        if(at_X == eye_X && at_Z == (eye_Z - 1)) {
            at_X = eye_X + 1;
            at_Z = eye_Z;
        } else if(at_X == (eye_X + 1) && at_Z == eye_Z){
            at_X = eye_X;
            at_Z = eye_Z + 1;
        } else if(at_X == eye_X && at_Z == (eye_Z + 1)) {
            at_X = eye_X - 1;
            at_Z = eye_Z;
        } else if(at_X == (eye_X - 1) && at_Z == eye_Z) {
            at_X = eye_X;
            at_Z = eye_Z - 1;
        }
        break;
    default :
        break;
    }
}


int main(int argc, char** argv)
{


    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(Window_Width, Window_Height);
    Window_ID = glutCreateWindow(TITLE);

    //install functions
    glutDisplayFunc(&renderScene);
    glutIdleFunc(&renderScene);
    glutReshapeFunc(&resizeScene);
    glutKeyboardFunc(&keyboardFunction);

    init(Window_Width, Window_Height);

    glutMainLoop();

    return 1;
}

