#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "imgloader.h"

GLuint starTexture;
GLUquadric *sun;
GLuint sunTexture;
GLUquadric *mercur;
GLuint mercurTexture;
GLUquadric *venus;
GLuint venusTexture;
GLUquadric *earth;
GLuint earthTexture;
GLUquadric *moon;
GLuint moonTexture;
GLUquadric *mars;
GLuint marsTexture;
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
float speed = 0.1f;

boolean spin = true, spinR= true, spinG= true, spinB= true, spinM= true, spinJ= true, spinS= true, spinN= true, spinU= true, spinP= true;
float Cx = 0.0f, Cy = 2.5f, Cz = 10.0f;
float Lx = 0.0f, Ly = 2.5f, Lz = -50.0f;
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
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -2.0, 2.0);

    Image* starImage=loadBMP("poze/stars.bmp");
    starTexture = loadTexture(starImage);

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
    earth = gluNewQuadric();
    gluQuadricTexture( earth, GL_TRUE);
    Image* earthImage=loadBMP("poze/earth.bmp");
    earthTexture = loadTexture(earthImage);
    //////////////////////////////////////////////////
    // texture lun
    moon = gluNewQuadric();
    gluQuadricTexture( moon, GL_TRUE);
    Image* moonImage=loadBMP("poze/moon.bmp");
    moonTexture = loadTexture(moonImage);
    //////////////////////////////////////////////////
    // texture planet mars
    mars = gluNewQuadric();
    gluQuadricTexture( mars, GL_TRUE);
    Image* marsImage=loadBMP("poze/mars.bmp");
    marsTexture = loadTexture(marsImage);
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
float Ex = 0 ,Ey = 0,Ez,Cex,Cey,Cez;

static void display(void)
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
glMatrixMode(GL_MODELVIEW);

glLoadIdentity();
gluLookAt(Cx, Cy, Cz,
            Lx, Ly, Lz,
            0, 1, 0);
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

//Background Stars
     glPushMatrix();
      glTranslatef(0.0,0.0,2.0);
      glScalef(0.5,0.5,0.5);
      glEnable ( GL_TEXTURE_2D );
        glBindTexture ( GL_TEXTURE_2D, starTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3f( -10000.0,-10000.0, 0.0);
        glTexCoord2f(10000.0, 0.0);
        glVertex3f(  10000.0,-10000.0, 0.0);
        glTexCoord2f(10000.0, 10000.0);
        glVertex3f(  10000.0, 10000.0, 0.0);
        glTexCoord2f(0.0, 10000.0);
        glVertex3f( -10000.0, 10000.0, 0.0);
        glTexCoord2f(0.0, 0.0);
        glVertex3f( -10000.0,-10000.0, 0.0);
      glEnd();
   glPopMatrix();

/* Sun */
    //glColor3f(1.0f, 0.5f, 0.0f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, sunTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTranslatef(0.0f,0.0f,-20); //xyz
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
    //glTranslatef(0.0f,0.0f,-37); //xyz
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
    //glTranslatef(0.0f,0.0f,-34); //xyz
    glRotatef(globRotG, 0,0,1);
    glTranslatef(7.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(venus, 0.6, 20, 20); //sphere
    glPopMatrix();

/* Planet earth (Terra) */
    //glColor3f(0.196078f, 0.6f, 0.8f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, earthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTranslatef(0.0f,0.0f,-30); //xyz
    glRotatef(globRotB, 0,0,1);
    glTranslatef(9.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(earth, 0.8, 20, 20); //sphere
    glPopMatrix();

/* moon (Terra) */
    //glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, moonTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTranslatef(0.0f,0.0f,-25.5); //xyz
    glRotatef(globRotB, 0,0,1);
    glTranslatef(10.5f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(moon, 0.2, 20, 20); //sphere
    glPopMatrix();

/* Planet mars */
    //glColor3f(1.0f, 0.3f, 0.0f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, marsTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTranslatef(0.0f,0.0f,-23); //xyz
    glRotatef(globRotM, 0,0,1);
    glTranslatef(12.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(mars, 0.5, 20, 20); //sphere
    glPopMatrix();

/* Phobos */
    //glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, moonTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTranslatef(0.0f,0.0f,-21); //xyz
    glRotatef(globRotM, 0,0,1);
    glTranslatef(13.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(moon, 0.1, 20, 20); //sphere
    glPopMatrix();


/* Deimos */
    //glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, moonTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTranslatef(0.0f,0.0f,-21); //xyz
    glRotatef(globRotM, 0,0,1);
    glTranslatef(13.3f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    gluSphere(moon, 0.05, 20, 20); //sphere
    glPopMatrix();

/* Planet Jupiter */
    //glColor3f(0.647059f, 0.164706f, 0.164706f);
    glPushMatrix();
    glEnable ( GL_TEXTURE_2D );
    glBindTexture ( GL_TEXTURE_2D, jupiterTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //lTranslatef(0.0f,0.0f,-20); //xyz
    glRotatef(globRotJ, 0,0,1);
    glTranslatef(16.0f,0.0f,0.0f);
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
    //glTranslatef(0.0f,0.0f,-16); //xyz
    glRotatef(globRotS, 0,0,1);
    glTranslatef(18.0f,0.0f,0.0f);
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
    //glTranslatef(0.0f,0.0f,-14); //xyz
    glRotatef(globRotU, 0,0,1);
    glTranslatef(20.0f,0.0f,0.0f);
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
    //glTranslatef(0.0f,0.0f,-10); //xyz
    glRotatef(globRotN, 0,0,1);
    glTranslatef(22.0f,0.0f,0.0f);
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
    //glTranslatef(0.0f,0.0f,-6); //xyz
    glRotatef(globRotP, 0,0,1);
    glTranslatef(24.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotasi sumbu Y
    //glutSolidSphere(0.2,20,20); //sphere
    gluSphere(pluto, 0.2, 20, 20); //sphere
    glPopMatrix();

if (spin == true){
    axisRot += 0.1f*speed; axisRot=fmod(axisRot, 360.0f);
    if (spinR == true){
        globRotR += 2.0f*speed; globRotR=fmod(globRotR, 360.0f);
    }if (spinG == true){
        globRotG += 1.7f*speed; globRotG=fmod(globRotG, 360.0f);
    }if (spinB == true){
        globRotB += 1.3f*speed; globRotB=fmod(globRotB, 360.0f);
    }if (spinM == true){
        globRotM += 1.0f*speed; globRotM=fmod(globRotM, 360.0f);
    }if (spinJ == true){
        globRotJ += 0.7f*speed; globRotJ=fmod(globRotJ, 360.0f);
    }if (spinS == true){
        globRotS += 0.5f*speed; globRotS=fmod(globRotS, 360.0f);
    }if (spinU == true){
        globRotU += 0.3f*speed; globRotU=fmod(globRotU, 360.0f);
    }if (spinN == true){
        globRotN += 0.2f*speed; globRotN=fmod(globRotN, 360.0f);
    }if (spinP == true){
        globRotP += 0.1f*speed; globRotP=fmod(globRotP, 360.0f);
    }
}

 //draw textured rectangle



glDisable ( GL_TEXTURE_2D );

glutSwapBuffers();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void keyboard(unsigned char key,int x,int y)
{

	switch(key)
	{
    case 'q':
        speed = 0.1;
        Cx = 0.0f; Cy = 2.5f; Cz = 10.0f;
        Lx = 0.0f; Ly = 2.5f; Lz = -50.0f;
        spin = true;
    break;
    case 'a':
        Cx -= 1;
        Lx -= 1;
        break;
    case 's':
        Cz = Cz +1;
        break;
    case 'd':
        Cx += 1;
        Lx += 1;
        break;
    case 'w':
        Cz = Cz -1;
        break;
    case 'i':
        Ly = Ly -1;
        break;
     case 'k':
        Ly = Ly +1;
        break;
    case 'o':
        Cy = Cy -1;
        break;
    case 'l':
        Cy = Cy +1;
        break;
    case 'z':
        speed +=0.05f;
        break;
    case 'x':
        speed -=0.05f;
        break;
    case 'c':
	     if (spin==false){
            spin = true;
            spinR = true;
            spinG = true;
            spinB = true;
            spinM = true;
            spinJ = true;
            spinS = true;
            spinU = true;
            spinN = true;
            spinP = true;
	     }else if(spin == true){
            spin = false;
	     }
        break;
	case '1': /* exit */
	     if (spinR==false){
            spinR = true;
	     }else if(spinR == true){
            spinR = false;
	     }
        break;
	case '2': /* exit */
	     if (spinG==false){
            spinG = true;
	     }else if(spinG == true){
            spinG = false;
	     }
        break;
    case '3': /* exit */
	     if (spinB==false){
            spinB = true;
	     }else if(spinB == true){
            spinB = false;
	     }
        break;
    case '4': /* exit */
	     if (spinM==false){
            spinM = true;
	     }else if(spinM == true){
            spinM = false;
	     }
        break;
    case '5': /* exit */
	     if (spinJ==false){
            spinJ = true;
	     }else if(spinJ == true){
            spinJ = false;
	     }
        break;
    case '6': /* exit */
	     if (spinS==false){
            spinS = true;
	     }else if(spinS == true){
            spinS = false;
	     }
        break;
    case '7': /* exit */
	     if (spinU==false){
            spinU = true;
	     }else if(spinU == true){
            spinU = false;
	     }
        break;
    case '8': /* exit */
	     if (spinN==false){
            spinN = true;
	     }else if(spinN == true){
            spinN = false;
	     }
        break;
    case '9': /* exit */
	     if (spinP==false){
            spinP = true;
	     }else if(spinP == true){
            spinP = false;
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
    glutCreateWindow("Solar System!!!");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    initGL(width, height);
    glutMainLoop();
    return 0;
}
