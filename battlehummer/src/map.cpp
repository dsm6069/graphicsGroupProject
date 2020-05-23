#include "map.h"
#include <iostream>
#include <GL/glut.h>
#include <ctime>
#include <map>

enum SHAPE {Sph, Cyl, Pri, SphCyl, SphPri, CylPri, All};

const unsigned long long fifteenDig = 1000000000000000;
const int eightDig = 100000000;
int buildings = 0;
int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0;
GLUquadric* quadric = gluNewQuadric();
GLUquadric* antennaQuadric = gluNewQuadric();
int sphereCol = 0, cylCol = 0, prismCol = 0;
std::map<int, std::map<int, SHAPE> > rows;
bool sphRows = true, cylRows = true, priRows = true;
float cubeRot = 0.0f, cylinderRot = 0.0f;
float cubeRotSpeed = 0.25f, cylinderRotSpeed = 0.5f;

void map::render(unsigned long long key)
{
    int k = 0, l = 0;
    SHAPE oneAway, twoAway;
    for(int i = 0; i < 400; i += 20)
    {
        for(int j = 0; j < 400; j += 20)
        {
            if(k > 0) {
                oneAway = rows[k - 1][l];
                twoAway = rows[k - 2][l];
                sphRows = !(oneAway == Sph || oneAway == SphCyl || oneAway == SphPri || oneAway == All);
                cylRows = !(oneAway == Cyl || oneAway == SphCyl || oneAway == CylPri || oneAway == All);
                priRows = !(oneAway == Pri || oneAway == SphPri || oneAway == CylPri || oneAway == All);
            }
            if(buildings < 400)
                ++buildings;
            glPushMatrix();
            glBegin(GL_QUADS);
            glColor3f(0.486f, 0.988f, 0.000f);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(i, 0.0f, -j); //bottom left corner
            glVertex3f(i + 15.5f, 0.0f, -j); // bottom right corner
            glVertex3f(i + 15.5f, 0.0f, -(j + 15.5f)); // top right corner
            glVertex3f(i, 0.0f, -(j + 15.5f)); // top left corner

            if(i + j < 380)
            {
                //block exists in 'left triangle' so the roads are all on the top and right of each block
                topRoad(i, j);
                rightRoad(i, j);
            }
            else if(i + j > 380)
            {
                leftRoad(i, j);
                bottomRoad(i, j);
                //block exists in 'right triangle' so the roads are all on the bottom and left of each block
            }
            else
            {
                //block is on the middle line between the triangles, and as such there are no roads cause they will be on the adjacent blocks
            }

            glEnd();
            glPopMatrix();
            //
            if((key / fifteenDig * (i + 1) * (j + 1)) % 270 < 90)   //one building in block
            {
                if((key % eightDig / (i + 1) * (j + 1)) % 42 < 14 && sphereCol == 0)
                {
                    if(buildings < 400) {
                        rows[k][l] = Sph;
                        ++c;
                    }
                    sphereBuilding(i, j); //generate a spherical building
                }
                else if((key % eightDig / (i + 1) * (j + 1)) % 42 >= 14 && (key % eightDig / (i + 1) * (j + 1)) % 42 < 28 && cylCol == 0)
                {
                    if(buildings < 400) {
                        rows[k][l] = Cyl;
                        ++b;
                    }
                    cylBuilding(i, j); //generate a cylindrical building

                }
                else if((key % eightDig / (i + 1) * (j + 1)) % 42 >= 28 && (key % eightDig / (i + 1) * (j + 1)) % 42 < 42 && prismCol == 0)
                {
                    if(buildings < 400) {
                        rows[k][l] = Pri;
                        ++a;
                    }
                    prismBuilding(i, j); //generate a prismatic building

                }
            }
            else if((key / fifteenDig * (i + 1) * (j + 1)) % 270 >= 90 && (key / fifteenDig * (i + 1) * (j + 1) % 270 < 180))     //two buildings in block
            {
                if((key % eightDig / (i + 1) * (j + 1)) % 42 < 14 && sphereCol == 0 && cylCol == 0)
                {
                    if(buildings < 400) {
                        rows[k][l] = SphCyl;
                        ++d;
                    }
                    sphereBuilding(i, j);
                    cylBuilding(i, j);

                }
                else if((key % eightDig / (i + 1) * (j + 1)) % 42 >= 14 && (key % eightDig / (i + 1) * (j + 1)) % 42 < 28 && sphereCol == 0 && prismCol == 0)
                {
                    if(buildings < 400) {
                        rows[k][l] = SphPri;
                        ++e;
                    }
                    sphereBuilding(i, j);
                    prismBuilding(i, j);
                }
                else if((key % eightDig / (i + 1) * (j + 1)) % 42 >= 28 && (key % eightDig / (i + 1) * (j + 1)) % 42 < 42 && prismCol == 0 && cylCol == 0)
                {
                    if(buildings < 400) {
                        rows[k][l] = CylPri;
                        ++f;
                    }
                    prismBuilding(i, j);
                    cylBuilding(i, j);
                }
            }
            else if ((key / fifteenDig * (i + 1) * (j + 1)) % 270 >= 180 && (key / fifteenDig * (i + 1) * (j + 1)) % 270 < 270)
            {
                prismBuilding(i, j);
                cylBuilding(i, j);
                sphereBuilding(i, j);
                ++g;
            }
            if(sphereCol != 0)
                --sphereCol;
            if(prismCol != 0)
                --prismCol;
            if(cylCol != 0)
                --cylCol;
            if(buildings < 400)
            {
                std::cout << "S: " << c << std::endl;
                std::cout << "C: " << b << std::endl;
                std::cout << "P: " << a << std::endl;
                std::cout << "SC: " << d << std::endl;
                std::cout << "SP: " << e << std::endl;
                std::cout << "PC: " << f << std::endl;
                std::cout << "All: " << g << std::endl;
            }
            ++l;
        }
        ++k;
    }
    cylinderRot += cylinderRotSpeed;
    cubeRot += cubeRotSpeed;
}

void map::topRoad(int currentX, int currentZ)
{
    glColor3f(0.412, 0.412, 0.412);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(currentX + 15.5f, 0.0f, -(currentZ + 15.5f)); //bottom right corner
    glVertex3f(currentX + 15.5f, 0.0f, -(currentZ + 20.0f));  //top right corner
    glVertex3f(currentX, 0.0f, -(currentZ + 20.0f));  //top left corner
    glVertex3f(currentX, 0.0f, -(currentZ + 15.5f));  //bottom left corner

    //dotted center line
    for(int i = 0; i < 15; ++i) {
        glColor3f(1.000, 0.843, 0.000);
        glVertex3f(currentX + i + 1.0f, 0.02f, -(currentZ + 17.65));
        glVertex3f(currentX + i + 1.0f, 0.02f, -(currentZ + 17.85));
        glVertex3f(currentX + i + 0.5f, 0.02f, -(currentZ + 17.85));
        glVertex3f(currentX + i + 0.5f, 0.02f, -(currentZ + 17.65));
    }

    glColor3f(1.000, 0.843, 0.000);
    glVertex3f(currentX + 15.5f, 0.015f, -(currentZ + 16.0f));
    glVertex3f(currentX + 15.5f, 0.015f, -(currentZ + 16.1f));
    glVertex3f(currentX, 0.015f, -(currentZ + 16.1f));
    glVertex3f(currentX, 0.015f, -(currentZ + 16.0f));

    glColor3f(1.000, 0.843, 0.000);
    glVertex3f(currentX + 15.5f, 0.015f, -(currentZ + 19.4f));
    glVertex3f(currentX + 15.5f, 0.015f, -(currentZ + 19.5f));
    glVertex3f(currentX, 0.015f, -(currentZ + 19.5f));
    glVertex3f(currentX, 0.015f, -(currentZ + 19.4f));

}

void map::rightRoad(int currentX, int currentZ)
{
    glColor3f(0.412, 0.412, 0.412);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(currentX + 15.5f, 0.0f, -currentZ); //bottom left corner
    glVertex3f(currentX + 20.0f, 0.0f, -currentZ); //bottom right corner
    glVertex3f(currentX + 20.0f, 0.0f, -(currentZ + 15.5f)); //top right corner
    glVertex3f(currentX + 15.5f, 0.0f, -(currentZ + 15.5f)); //top left corner

    glColor3f(0.412, 0.412, 0.412);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(currentX + 15.5f, 0.0f, -(currentZ + 15.5f));
    glVertex3f(currentX + 20.0f, 0.0f, -(currentZ + 15.5f));
    glVertex3f(currentX + 20.0f, 0.0f, -(currentZ + 20.0f));
    glVertex3f(currentX + 15.5f, 0.0f, -(currentZ + 20.0f));

    for(int i = 0; i < 15; ++i) {
        glColor3f(1.000, 0.843, 0.000);
        glVertex3f(currentX + 17.65f, 0.02f, -(currentZ + i + 0.5f));
        glVertex3f(currentX + 17.85f, 0.02f, -(currentZ + i + 0.5f));
        glVertex3f(currentX + 17.85f, 0.02f, -(currentZ + i + 1.0f));
        glVertex3f(currentX + 17.65f, 0.02f, -(currentZ + i + 1.0f));
    }

    glColor3f(1.000, 0.843, 0.000);
    glVertex3f(currentX + 16.0f, 0.015f, -currentZ);
    glVertex3f(currentX + 16.1f, 0.015f, -currentZ);
    glVertex3f(currentX + 16.1f, 0.015f, -(currentZ + 15.5f));
    glVertex3f(currentX + 16.0f, 0.015f, -(currentZ + 15.5f));

    glColor3f(1.000, 0.843, 0.000);
    glVertex3f(currentX + 19.4f, 0.015f, -currentZ);
    glVertex3f(currentX + 19.5f, 0.015f, -currentZ);
    glVertex3f(currentX + 19.5f, 0.015f, -(currentZ + 15.5f));
    glVertex3f(currentX + 19.4f, 0.015f, -(currentZ + 15.5f));


}

void map::leftRoad(int currentX, int currentZ)
{
    glColor3f(0.412, 0.412, 0.412);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(currentX, 0.0f, -currentZ); //bottom right
    glVertex3f(currentX, 0.0f, -(currentZ + 15.5f)); //top right
    glVertex3f(currentX - 4.5f, 0.0f, -(currentZ + 15.5f)); //top left
    glVertex3f(currentX - 4.5f, 0.0f, -currentZ); //bottom left

    glColor3f(0.412, 0.412, 0.412);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(currentX, 0.0f, -currentZ); //top right
    glVertex3f(currentX - 4.5f, 0.0f, -currentZ); //top left
    glVertex3f(currentX - 4.5f, 0.0f, -(currentZ - 4.5f));
    glVertex3f(currentX, 0.0f, -(currentZ - 4.5f));

    for(int i = 0; i < 15; ++i) {
        glColor3f(1.000, 0.843, 0.000);
        glVertex3f(currentX - 2.25f, 0.02f, -(currentZ + i + 0.5f));
        glVertex3f(currentX - 2.25f, 0.02f, -(currentZ + i + 1.0f));
        glVertex3f(currentX - 2.35f, 0.02f, -(currentZ + i + 1.0f));
        glVertex3f(currentX - 2.35f, 0.02f, -(currentZ + i + 0.5f));
    }

    glColor3f(1.000, 0.843, 0.000);
    glVertex3f(currentX - 0.5f, 0.015f, -currentZ);
    glVertex3f(currentX - 0.5f, 0.015f, -(currentZ + 15.5f));
    glVertex3f(currentX - 0.6f, 0.015f, -(currentZ + 15.5f));
    glVertex3f(currentX - 0.6f, 0.015f, -currentZ);

    glColor3f(1.000, 0.843, 0.000);
    glVertex3f(currentX - 3.9f, 0.015f, -currentZ);
    glVertex3f(currentX - 3.9f, 0.015f, -(currentZ + 15.5f));
    glVertex3f(currentX - 4.0f, 0.015f, -(currentZ + 15.5f));
    glVertex3f(currentX - 4.0f, 0.015f, -currentZ);
}

void map::bottomRoad(int currentX, int currentZ)
{
    glColor3f(0.412, 0.412, 0.412);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(currentX + 15.5f, 0.0f, -currentZ);
    glVertex3f(currentX, 0.0f, -currentZ);
    glVertex3f(currentX, 0.0f, -(currentZ - 4.5f));
    glVertex3f(currentX + 15.5f, 0.0f, -(currentZ - 4.5f));

    for(int i = 0; i < 15; ++i) {
        glColor3f(1.000, 0.843, 0.000);
        glVertex3f(currentX + i + 1.0f, 0.02f, -(currentZ - 2.25));
        glVertex3f(currentX + i + 0.5f, 0.02f, -(currentZ - 2.25));
        glVertex3f(currentX + i + 0.5f, 0.02f, -(currentZ - 2.35));
        glVertex3f(currentX + i + 1.0f, 0.02f, -(currentZ - 2.35));
    }

    glColor3f(1.000, 0.843, 0.000);
    glVertex3f(currentX + 15.5f, 0.015f, -(currentZ - 0.5f));
    glVertex3f(currentX, 0.015f, -(currentZ - 0.5f));
    glVertex3f(currentX, 0.015f, -(currentZ - 0.6f));
    glVertex3f(currentX + 15.5f, 0.015f, -(currentZ - 0.6f));

    glColor3f(1.000, 0.843, 0.000);
    glVertex3f(currentX + 15.5f, 0.015f, -(currentZ - 3.9f));
    glVertex3f(currentX, 0.015f, -(currentZ - 3.9f));
    glVertex3f(currentX, 0.015f, -(currentZ - 4.0f));
    glVertex3f(currentX + 15.5f, 0.015f, -(currentZ - 4.0f));

}

void map::prismBuilding(int currentX, int currentZ)
{
    glPushMatrix();
    glBegin(GL_QUADS);
    //building faces

    //left
    glColor3f(1.0f, 0.549f, 0.0f);
    glVertex3f(currentX + 10.75f, 0.0f, -(currentZ + 10.0f));
    glVertex3f(currentX + 10.75f, 0.0f, -(currentZ + 7.0f));
    glVertex3f(currentX + 10.75f, 6.0f, -(currentZ + 7.0f));
    glVertex3f(currentX + 10.75f, 6.0f, -(currentZ + 10.0f));

    //front
    glColor3f(1.0f, 0.549f, 0.0f);
    glVertex3f(currentX + 10.75f, 0.0f, -(currentZ + 7.0f));
    glVertex3f(currentX + 13.75f, 0.0f, -(currentZ + 7.0f));
    glVertex3f(currentX + 13.75f, 6.0f, -(currentZ + 7.0f));
    glVertex3f(currentX + 10.75f, 6.0f, -(currentZ + 7.0f));

    //right
    glColor3f(1.0f, 0.549f, 0.0f);
    glVertex3f(currentX + 13.75f, 0.0f, -(currentZ + 7.0f));
    glVertex3f(currentX + 13.75f, 0.0f, -(currentZ + 10.0f));
    glVertex3f(currentX + 13.75f, 6.0f, -(currentZ + 10.0f));
    glVertex3f(currentX + 13.75f, 6.0f, -(currentZ + 7.0f));

    //back
    glColor3f(1.0f, 0.549f, 0.0f);
    glVertex3f(currentX + 13.75f, 0.0f, -(currentZ + 10.0f));
    glVertex3f(currentX + 10.75f, 0.0f, -(currentZ + 10.0f));
    glVertex3f(currentX + 10.75f, 6.0f, -(currentZ + 10.0f));
    glVertex3f(currentX + 13.75f, 6.0f, -(currentZ + 10.0f));

    //top
    glColor3f(1.0f, 0.549f, 0.0f);
    glVertex3f(currentX + 13.75f, 6.0f, -(currentZ + 10.0f));
    glVertex3f(currentX + 10.75f, 6.0f, -(currentZ + 10.0f));
    glVertex3f(currentX + 10.75f, 6.0f, -(currentZ + 7.0f));
    glVertex3f(currentX + 13.75f, 6.0f, -(currentZ + 7.0f));

    //windows
    //left
    glColor3f(0.686, 0.933, 0.933);
    glVertex3f(currentX + 10.73f, 3.0f, -(currentZ + 9.0f));
    glVertex3f(currentX + 10.73f, 3.0f, -(currentZ + 8.0f));
    glVertex3f(currentX + 10.73f, 4.5f, -(currentZ + 8.0f));
    glVertex3f(currentX + 10.73f, 4.5f, -(currentZ + 9.0f));

    //front
    glColor3f(0.686, 0.933, 0.933);
    glVertex3f(currentX + 11.75f, 2.0f, -(currentZ + 6.98f));
    glVertex3f(currentX + 12.75f, 2.0f, -(currentZ + 6.98f));
    glVertex3f(currentX + 12.75f, 5.0f, -(currentZ + 6.98f));
    glVertex3f(currentX + 11.75f, 5.0f, -(currentZ + 6.98f));

    //right
    glColor3f(0.686, 0.933, 0.933);
    glVertex3f(currentX + 13.77f, 0.5f, -(currentZ + 8.5f));
    glVertex3f(currentX + 13.77f, 0.5f, -(currentZ + 9.0f));
    glVertex3f(currentX + 13.77f, 2.0f, -(currentZ + 9.0f));
    glVertex3f(currentX + 13.77f, 2.0f, -(currentZ + 8.5f));

    //back
    glColor3f(0.686, 0.933, 0.933);
    glVertex3f(currentX + 13.5f, 3.0f, -(currentZ + 10.02f));
    glVertex3f(currentX + 11.0f, 3.0f, -(currentZ + 10.02f));
    glVertex3f(currentX + 11.0f, 4.5f, -(currentZ + 10.02f));
    glVertex3f(currentX + 13.5f, 4.5f, -(currentZ + 10.02f));

    glEnd();

    prismCol = 2;
}

void map::cylBuilding(int currentX, int currentZ)
{
    glPushMatrix();
    glTranslatef(currentX + 4.0f, 0.0f, -(currentZ + 12.5f));
    glRotatef(-90, 1, 0, 0);
    glColor3f(1.0f, 1.0f, 0.0f);
    gluCylinder(quadric, 2, 2, 5, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(currentX + 4.0f, 8.0f, -(currentZ + 12.5f));
    glRotatef(cubeRot, 0.0f, 1.0f, 0.0f);
    glTranslatef(-(currentX + 4.0f), -8.0f, currentZ + 12.5f);
    glBegin(GL_QUADS);
    //building faces

    //left
    glColor3f(1.000, 0.843, 0.000);
    glVertex3f(currentX + 1.0f, 5.0f, -(currentZ + 15.5f));
    glVertex3f(currentX + 1.0f, 5.0f, -(currentZ + 9.5f));
    glVertex3f(currentX + 1.0f, 11.0f, -(currentZ + 9.5f));
    glVertex3f(currentX + 1.0f, 11.0f, -(currentZ + 15.5f));

    //front
    glColor3f(1.000, 0.843, 0.000);
    glVertex3f(currentX + 1.0f, 5.0f, -(currentZ + 9.5f));
    glVertex3f(currentX + 7.0f, 5.0f, -(currentZ + 9.5f));
    glVertex3f(currentX + 7.0f, 11.0f, -(currentZ + 9.5f));
    glVertex3f(currentX + 1.0f, 11.0f, -(currentZ + 9.5f));

    //right
    glColor3f(1.000, 0.843, 0.000);
    glVertex3f(currentX + 7.0f, 5.0f, -(currentZ + 9.5f));
    glVertex3f(currentX + 7.0f, 5.0f, -(currentZ + 15.5f));
    glVertex3f(currentX + 7.0f, 11.0f, -(currentZ + 15.5f));
    glVertex3f(currentX + 7.0f, 11.0f, -(currentZ + 9.5f));

    //back
    glColor3f(1.000, 0.843, 0.000);
    glVertex3f(currentX + 7.0f, 5.0f, -(currentZ + 15.5f));
    glVertex3f(currentX + 1.0f, 5.0f, -(currentZ + 15.5f));
    glVertex3f(currentX + 1.0f, 11.0f, -(currentZ + 15.5f));
    glVertex3f(currentX + 7.0f, 11.0f, -(currentZ + 15.5f));

    //top
    glColor3f(1.000, 0.843, 0.000);
    glVertex3f(currentX + 7.0f, 11.0f, -(currentZ + 15.5f));
    glVertex3f(currentX + 1.0f, 11.0f, -(currentZ + 15.5f));
    glVertex3f(currentX + 1.0f, 11.0f, -(currentZ + 9.5f));
    glVertex3f(currentX + 7.0f, 11.0f, -(currentZ + 9.5f));

    //bottom
    glColor3f(1.000, 0.843, 0.000);
    glVertex3f(currentX + 1.0f, 5.0f, -(currentZ + 15.5f));
    glVertex3f(currentX + 7.0f, 5.0f, -(currentZ + 15.5f));
    glVertex3f(currentX + 7.0f, 5.0f, -(currentZ + 9.5f));
    glVertex3f(currentX + 1.0f, 5.0f, -(currentZ + 9.5f));
    glEnd();

    glPopMatrix();

    cylCol = 2;

}

void map::sphereBuilding(int currentX, int currentZ)
{
    glPushMatrix();
    glTranslatef(currentX + 7.0f, 2.0f, -(currentZ + 2.0f));
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidSphere(2, 40, 40);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(currentX + 7.0f, 3.8f, -(currentZ + 2.0f));
    glRotatef(cylinderRot, 0.0f, 1.0f, 0.0f);
    glRotatef(-70.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(0.12f, 0.1f, 0.1f);
    gluCylinder(antennaQuadric, 0.5, 0.5, 1.5, 20, 20);
    glColor3f(0.12f, 0.1f, 0.1f);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(antennaQuadric, 0, 0.5, 20, 20);
    glTranslatef(0.0f, 0.0f, -1.5f);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(0.12f, 0.1f, 0.1f);
    gluDisk(antennaQuadric, 0, 0.5, 20, 20);
    glPopMatrix();
    sphereCol = 2;
}
