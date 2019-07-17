#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "imgloader.h"

GLUquadric *sun;
GLuint sunTexture;
GLUquadric *mercur;
GLuint mercurTexture;
GLUquadric *venus;
GLuint venusTexture;
GLUquadric *pamant;
GLuint earthTexture;
GLUquadric *luna;
GLuint lunaTexture;
GLUquadric *marte;
GLuint marteTexture;
GLUquadric *jupiter;
GLuint jupiterTexture;
GLUquadric *saturn;
GLuint saturnTexture;
GLUquadric *uranus;
GLuint uranusTexture;
GLUquadric *neptun;
GLuint neptunTexture;
GLUquadric *pluto;
GLuint plutoTexture;
int zoom = 0;
float speed = 1.0f;

boolean twist = true, twistR= true, twistG= true, twistB= true, twistM= true, twistJ= true, twistS= true, twistN= true, twistU= true, twistP= true;
float Cx = 0.0f, Cy = 2.5f, Cz = 0.0f;
float Lx = 0.0f, Ly = 2.5f, Lz = -20.0f;
float sudut_x = 0.0f;
float sudut_x2 = 0.0f;
float sudut_z = 0.0f;
float sudut_z2 = 0.0f;
float sudut_y = 0.0f;
float sudut_y2 = 0.0f;


float toRadians(float angle){
    return angle * M_PI / 180;
}

class Vector{
public :
    float x, y, z;
    void set_values (float startX, float startY, float startZ){
         x = startX;
         y = startY;
         z = startZ;
    }

    void vectorRotation(Vector refs, float angle){
        Vector temp = refs;
        float magnitude = sqrt(pow(temp.x, 2) + pow(temp.y, 2) + pow(temp.z, 2));
        temp.x = temp.x / magnitude;
        temp.y = temp.y / magnitude;
        temp.z = temp.z / magnitude;
        float dot_product = (x * temp.x)+(y * temp.y)+(z * temp.z);
        float cross_product_x = (y * temp.z) - (temp.z * z);
        float cross_product_y = -((x * temp.z) - (z * temp.x));
        float cross_product_z = (x * temp.y) - (y * temp.x);
        float last_factor_rodrigues = 1.0f - cos(toRadians(fmod(angle, 360.0f)));
        x = (x * cos(toRadians(fmod(angle, 360.0f)))) + (cross_product_x * sin(toRadians(fmod(angle, 360.0f)))) + (dot_product * last_factor_rodrigues * x);
        y = (y * cos(toRadians(fmod(angle, 360.0f)))) + (cross_product_y * sin(toRadians(fmod(angle, 360.0f)))) + (dot_product * last_factor_rodrigues * y);
        z = (z * cos(toRadians(fmod(angle, 360.0f)))) + (cross_product_z * sin(toRadians(fmod(angle, 360.0f)))) + (dot_product * last_factor_rodrigues * z);
    }
};

void cameraRotation(Vector refer, double angle){
    float M = sqrt(pow(refer.x, 2) + pow(refer.y, 2) + pow(refer.z, 2));
    float Up_x1 = refer.x / M;
    float Up_y1 = refer.y / M;
    float Up_z1 = refer.z / M;
    float VLx = Lx - Cx;
    float VLy = Ly - Cy;
    float VLz = Lz - Cz;
    float dot_product = (VLx * Up_x1) + (VLy * Up_y1) + (VLz * Up_z1);
    float cross_product_x = (Up_y1 * VLz) - (VLy * Up_z1);
    float cross_product_y = -((Up_x1 * VLz) - (Up_z1 * VLx));
    float cross_product_z = (Up_x1 * VLy) - (Up_y1 * VLx);
    float last_factor_rodrigues = 1.0f - cos(toRadians(angle));
    float Lx1 = (VLx * cos(toRadians(angle))) + (cross_product_x * sin(toRadians(angle))) + (dot_product * last_factor_rodrigues * VLx);
    float Ly1 = (VLy * cos(toRadians(angle))) + (cross_product_y * sin(toRadians(angle))) + (dot_product * last_factor_rodrigues * VLy);
    float Lz1 = (VLz * cos(toRadians(angle))) + (cross_product_z * sin(toRadians(angle))) + (dot_product * last_factor_rodrigues * VLz);
    Lx = Lx1+Cx;
    Ly = Ly1+Cy;
    Lz = Lz1+Cz;
}


GLuint loadTexture(Image* image) {

    GLuint textureId;

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    return textureId;

}

void initGL(int width, int height)
{
    const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -2.0, 2.0);
    // texture sun //////////////////////////////////
    sun = gluNewQuadric();
    gluQuadricTexture( sun, GL_TRUE);
    gluQuadricNormals(sun, GL_SMOOTH);
    Image* sunImage=loadBMP("poze/sun.bmp");
    sunTexture = loadTexture(sunImage);
    ///////////////////////////////////////////////////
    // texture planet mercur
    mercur = gluNewQuadric();
    gluQuadricTexture( mercur, GL_TRUE);
    Image* mercurImage=loadBMP("poze/mercury.bmp");
    mercurTexture = loadTexture(mercurImage);
    //////////////////////////////////////////////////
    // texture planet venus
    venus = gluNewQuadric();
    gluQuadricTexture( venus, GL_TRUE);
    Image* venusImage=loadBMP("poze/venus.bmp");
    venusTexture = loadTexture(venusImage);
    //////////////////////////////////////////////////
    // texture planet earth
    pamant = gluNewQuadric();
    gluQuadricTexture( pamant, GL_TRUE);
    Image* earthImage=loadBMP("poze/earth.bmp");
    earthTexture = loadTexture(earthImage);
    //////////////////////////////////////////////////
    // texture lun
    luna = gluNewQuadric();
    gluQuadricTexture( luna, GL_TRUE);
    Image* moonImage=loadBMP("poze/moon.bmp");
    lunaTexture = loadTexture(moonImage);
    //////////////////////////////////////////////////
    // texture planet mars
    marte = gluNewQuadric();
    gluQuadricTexture( marte, GL_TRUE);
    Image* marsImage=loadBMP("poze/mars.bmp");
    marteTexture = loadTexture(marsImage);
    //////////////////////////////////////////////////
    // texture planet jupiter
    jupiter = gluNewQuadric();
    gluQuadricTexture( jupiter, GL_TRUE);
    Image* jupiterImage=loadBMP("poze/jupiter.bmp");
    jupiterTexture = loadTexture(jupiterImage);
    //////////////////////////////////////////////////
    // texture planet saturn
    saturn = gluNewQuadric();
    gluQuadricTexture( saturn, GL_TRUE);
    Image* saturnImage=loadBMP("poze/saturn.bmp");
    saturnTexture = loadTexture(saturnImage);
    //////////////////////////////////////////////////
    // texture planet uranus
    uranus = gluNewQuadric();
    gluQuadricTexture( uranus, GL_TRUE);
    Image* uranusImage=loadBMP("poze/uranus.bmp");
    uranusTexture = loadTexture(uranusImage);
    //////////////////////////////////////////////////
    // texture planet neptun
    neptun = gluNewQuadric();
    gluQuadricTexture( neptun, GL_TRUE);
    Image* neptunImage=loadBMP("poze/neptune.bmp");
    neptunTexture = loadTexture(neptunImage);
    //////////////////////////////////////////////////
    // texture planet pluto
    glColor3f(0.658824f, 0.658824f, 0.658824f);
    pluto = gluNewQuadric();
    gluQuadricTexture( pluto, GL_TRUE);
    //plutoTexture = LoadBitmap("poze/pluto.bmp");
    //////////////////////////////////////////////////
    gluPerspective(80.0f, (GLfloat)width/(GLfloat)height, 2.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);

}


Vector sumbu_z, sumbu_x, sumbu_y;


static void display(void)
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
glMatrixMode(GL_MODELVIEW);
gluLookAt(Cx, Cy, Cz,
            Lx, Ly, Lz,
            sumbu_y.x, sumbu_y.y, sumbu_y.z);
glLoadIdentity();
static float axisRot = 0.0f;
static float globRotR = 0.0f;
static float globRotG = 50.0f;
static float globRotB = 75.0f;
static float globRotM = 100.0f;
static float globRotJ = 125.0f;
static float globRotS = 150.0f;
static float globRotU = 175.0f;
static float globRotN = 200.0f;
static float globRotP = 225.0f;

/* Sun */
    //glColor3f(1.0f, 0.5f, 0.0f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, sunTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTranslatef(0.0f,0.0f,-20); //xyz
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(sun, 2, 20, 20); //sphere
    glPopMatrix();

/* Planet Mercur */
    //glColor3f(1.0f, 0.8f, 0.0f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, mercurTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTranslatef(0.0f,0.0f,-37); //xyz
    glRotatef(globRotR, 0,0,1);
    glTranslatef(5.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(mercur, 0.3, 20, 20); //sphere
    glPopMatrix();

/* Planet Venus */
    //glColor3f(1.0f, 0.6f, 0.0f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, venusTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTranslatef(0.0f,0.0f,-34); //xyz
    glRotatef(globRotG, 0,0,1);
    glTranslatef(6.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(venus, 0.6, 20, 20); //sphere
    glPopMatrix();

/* Planet Pamant (Terra) */
    //glColor3f(0.196078f, 0.6f, 0.8f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, earthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTranslatef(0.0f,0.0f,-30); //xyz
    glRotatef(globRotB, 0,0,1);
    glTranslatef(7.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(pamant, 0.8, 20, 20); //sphere
    glPopMatrix();

/* Luna (Terra) */
    //glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, lunaTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTranslatef(0.0f,0.0f,-25.5); //xyz
    glRotatef(globRotB, 0,0,1);
    glTranslatef(7.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(luna, 0.2, 20, 20); //sphere
    glPopMatrix();

/* Planet Marte */
    //glColor3f(1.0f, 0.3f, 0.0f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, marteTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTranslatef(0.0f,0.0f,-23); //xyz
    glRotatef(globRotM, 0,0,1);
    glTranslatef(8.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(marte, 0.5, 20, 20); //sphere
    glPopMatrix();

/* Phobos */
    //glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, lunaTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTranslatef(0.0f,0.0f,-21); //xyz
    glRotatef(globRotM, 0,0,1);
    glTranslatef(7.9f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(luna, 0.1, 20, 20); //sphere
    glPopMatrix();


/* Deimos */
    //glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, lunaTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTranslatef(0.0f,0.0f,-21); //xyz
    glRotatef(globRotM, 0,0,1);
    glTranslatef(7.7f,0.0f,1.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(luna, 0.05, 20, 20); //sphere
    glPopMatrix();

/* Planet Jupiter */
    //glColor3f(0.647059f, 0.164706f, 0.164706f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, jupiterTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTranslatef(0.0f,0.0f,-20); //xyz
    glRotatef(globRotJ, 0,0,1);
    glTranslatef(9.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(jupiter, 1, 20, 20); //sphere
    glPopMatrix();

/* Planet Saturn */
    //glColor3f(0.55f, 0.47f, 0.14f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, saturnTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTranslatef(0.0f,0.0f,-16); //xyz
    glRotatef(globRotS, 0,0,1);
    glTranslatef(10.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(saturn, 0.4, 20, 20); //sphere
    glPopMatrix();

/* Planet Uranus */
    //glColor3f(0.0f, 1.0f, 0.0f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, uranusTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTranslatef(0.0f,0.0f,-14); //xyz
    glRotatef(globRotU, 0,0,1);
    glTranslatef(10.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(uranus, 0.4, 20, 20); //sphere
    glPopMatrix();

/* Planet Neptun */
    //glColor3f(0.196078f, 0.5f, 0.7f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, neptunTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTranslatef(0.0f,0.0f,-10); //xyz
    glRotatef(globRotN, 0,0,1);
    glTranslatef(11.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(neptun, 0.3, 20, 20); //sphere
    glPopMatrix();

/* Planet Pluto */
    //glColor3f(0.658824f, 0.658824f, 0.658824f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, plutoTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTranslatef(0.0f,0.0f,-6); //xyz
    glRotatef(globRotP, 0,0,1);
    glTranslatef(12.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    //glutSolidSphere(0.2,20,20); //sphere
    gluSphere(pluto, 0.2, 20, 20); //sphere
    glPopMatrix();

if (twist == true){
    axisRot += 0.1f; axisRot=fmod(axisRot, 360.0f);
    if (twistR == true){
        globRotR += 2.0f*speed; globRotR=fmod(globRotR, 360.0f);
    }if (twistG == true){
        globRotG += 1.7f*speed; globRotG=fmod(globRotG, 360.0f);
    }if (twistB == true){
        globRotB += 1.3f*speed; globRotB=fmod(globRotB, 360.0f);
    }if (twistM == true){
        globRotM += 1.0f*speed; globRotM=fmod(globRotM, 360.0f);
    }if (twistJ == true){
        globRotJ += 0.7f*speed; globRotJ=fmod(globRotJ, 360.0f);
    }if (twistS == true){
        globRotS += 0.5f*speed; globRotS=fmod(globRotS, 360.0f);
    }if (twistU == true){
        globRotU += 0.3f*speed; globRotU=fmod(globRotU, 360.0f);
    }if (twistN == true){
        globRotN += 0.2f*speed; globRotN=fmod(globRotN, 360.0f);
    }if (twistP == true){
        globRotP += 0.1f*speed; globRotP=fmod(globRotP, 360.0f);
    }
}


   //draw textured rectangle
   glPushMatrix();
      glTranslatef(0.0,0.0,1.0);
      glScalef(0.5,0.5,0.5);
      glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3f( -1.0,-1.0, 0.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(  1.0,-1.0, 0.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(  1.0, 1.0, 0.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f( -1.0, 1.0, 0.0);
        glTexCoord2f(0.0, 0.0);
        glVertex3f( -1.0,-1.0, 0.0);
      glEnd();
   glPopMatrix();

glDisable ( GL_TEXTURE_2D );

glutSwapBuffers();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void keyboard(unsigned char key,int x,int y)
{
    if (zoom < -40){
            zoom = -40;
         }
    if (zoom > 60){
            zoom = 60;
         }
	switch(key)
	{
    case 'q':
        speed +=1.0f;
        break;
    case 'w':
        speed -=1.0f;
        break;
    case '74': // J
        sudut_z += 15.0f;
        sumbu_z.vectorRotation(sumbu_y, sudut_z - sudut_z2); //memutar vector sumbu z terhadap x (target, patokan)
        sumbu_x.vectorRotation(sumbu_y, sudut_z - sudut_z2);
        cameraRotation(sumbu_y, sudut_z - sudut_z2); // look at
        sudut_z2 = sudut_z;
        break;
    case 'i': /* zoom in */
         zoom = zoom  - 20.0f;
         glViewport(0, 0, 1024, 768);
         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
         gluPerspective(80.0f + zoom,1024/768,2.0f,100.0f);
         glMatrixMode(GL_MODELVIEW);
         glLoadIdentity();
        break;
    case 'o': /* zoom out */
         zoom = zoom  + 20.0f;
         glViewport(0, 0, 1024, 768);
         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
         gluPerspective(80.0f + zoom,1024/768,2.0f,100.0f);
         glMatrixMode(GL_MODELVIEW);
         glLoadIdentity();
        break;
    case '0': /* exit */
	     if (twist==false){
            twist = true;
            twistR = true;
            twistG = true;
            twistB = true;
            twistM = true;
            twistJ = true;
            twistS = true;
            twistU = true;
            twistN = true;
            twistP = true;
	     }else if(twist == true){
            twist = false;
	     }
        break;
	case '1': /* exit */
	     if (twistR==false){
            twistR = true;
	     }else if(twistR == true){
            twistR = false;
	     }
        break;
	case '2': /* exit */
	     if (twistG==false){
            twistG = true;
	     }else if(twistG == true){
            twistG = false;
	     }
        break;
    case '3': /* exit */
	     if (twistB==false){
            twistB = true;
	     }else if(twistB == true){
            twistB = false;
	     }
        break;
    case '4': /* exit */
	     if (twistM==false){
            twistM = true;
	     }else if(twistM == true){
            twistM = false;
	     }
        break;
    case '5': /* exit */
	     if (twistJ==false){
            twistJ = true;
	     }else if(twistJ == true){
            twistJ = false;
	     }
        break;
    case '6': /* exit */
	     if (twistS==false){
            twistS = true;
	     }else if(twistS == true){
            twistS = false;
	     }
        break;
    case '7': /* exit */
	     if (twistU==false){
            twistU = true;
	     }else if(twistU == true){
            twistU = false;
	     }
        break;
    case '8': /* exit */
	     if (twistN==false){
            twistN = true;
	     }else if(twistN == true){
            twistN = false;
	     }
        break;
    case '9': /* exit */
	     if (twistP==false){
            twistP = true;
	     }else if(twistP == true){
            twistP = false;
	     }
        break;
    case 'e': /* exit */
	     exit(0);
        break;
	default:
		break;
	}
	glutPostRedisplay();
}

/* Masuk Program */
int main(int argc, char *argv[])
{
    int width = 1024;
    int height = 768;
    glutInit(&argc, argv);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Sistemul Solar!!!");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    initGL(width, height);
    glutMainLoop();
    return 0;
}
