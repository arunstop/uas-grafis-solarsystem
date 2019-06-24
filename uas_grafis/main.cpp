#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

GLUquadric *sun;
GLuint sunTexture;
GLUquadric *mercur;
GLuint mercurTexture;
GLUquadric *venus;
GLuint venusTexture;
GLUquadric *pamant;
GLuint pamantTexture;
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

int LoadBitmap(char *filename)
{
    FILE * file;
    char temp;
    long i;

    BITMAPINFOHEADER infoheader;
    unsigned char *infoheader_data;

    GLuint num_texture;

    if( (file = fopen(filename, "rb"))==NULL) return (-1); // Open the file for reading

    fseek(file, 18, SEEK_CUR);  /* start reading width & height */
    fread(&infoheader.biWidth, sizeof(int), 1, file);

    fread(&infoheader.biHeight, sizeof(int), 1, file);

    fread(&infoheader.biPlanes, sizeof(short int), 1, file);
    if (infoheader.biPlanes != 1) {
      printf("Planes from %s is not 1: %u", filename, infoheader.biPlanes);
      return 0;
    }

    // read the bpp
    fread(&infoheader.biBitCount, sizeof(unsigned short int), 1, file);
    if (infoheader.biBitCount != 24) {
      printf("Bpp from %s is not 24: %d", filename, infoheader.biBitCount);
      return 0;
    }

    fseek(file, 24, SEEK_CUR);

    // read the data
    if(infoheader.biWidth<0){
  infoheader.biWidth = -infoheader.biWidth;
    }
    if(infoheader.biHeight<0){
  infoheader.biHeight = -infoheader.biHeight;
    }
    infoheader_data = (unsigned char *) malloc(infoheader.biWidth * infoheader.biHeight * 3);
    if (infoheader_data == NULL) {
      printf("Error allocating memory for color-corrected image data");
      return 0;
    }

    if ((i = fread(infoheader_data, infoheader.biWidth * infoheader.biHeight * 3, 1, file)) != 1) {
      printf("Error reading image data from %s.", filename);
      return 0;
    }

    for (i=0; i<(infoheader.biWidth * infoheader.biHeight * 3); i+=3) { // reverse all of the colors. (bgr -> rgb)
      temp = infoheader_data[i];
      infoheader_data[i] = infoheader_data[i+2];
      infoheader_data[i+2] = temp;
    }


    fclose(file); // Closes the file stream

    glGenTextures(1, &num_texture);
    glBindTexture(GL_TEXTURE_2D, num_texture); // Bind the ID texture specified by the 2nd parameter

    // The next commands sets the texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Finally we define the 2d texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, infoheader.biWidth, infoheader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, infoheader_data);

    // And create 2d mipmaps for the minifying function
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, infoheader.biWidth, infoheader.biHeight, GL_RGB, GL_UNSIGNED_BYTE, infoheader_data);

    free(infoheader_data); // Free the memory we used to load the texture

    return (num_texture); // Returns the current texture OpenGL ID
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
    // textura soare //////////////////////////////////
    sun = gluNewQuadric();
    gluQuadricTexture( sun, GL_TRUE);
    //  sunTexture = LoadBitmap("poze/sun.bmp");
    ///////////////////////////////////////////////////
    // textura planeta mercur
    mercur = gluNewQuadric();
    gluQuadricTexture( mercur, GL_TRUE);
    //mercurTexture = LoadBitmap("poze/mercury.bmp");
    //////////////////////////////////////////////////
    // textura planeta venus
    venus = gluNewQuadric();
    gluQuadricTexture( venus, GL_TRUE);
    //venusTexture = LoadBitmap("poze/venus.bmp");
    //////////////////////////////////////////////////
    // textura planeta pamant
    pamant = gluNewQuadric();
    gluQuadricTexture( pamant, GL_TRUE);
    //pamantTexture = LoadBitmap("poze/earth.bmp");
    //////////////////////////////////////////////////
    // textura luna
    luna = gluNewQuadric();
    gluQuadricTexture( luna, GL_TRUE);
    //lunaTexture = LoadBitmap("poze/moon.bmp");
    //////////////////////////////////////////////////
    // textura planeta marte
    marte = gluNewQuadric();
    gluQuadricTexture( marte, GL_TRUE);
    //marteTexture = LoadBitmap("poze/mars.bmp");
    //////////////////////////////////////////////////
    // textura planeta jupiter
    jupiter = gluNewQuadric();
    gluQuadricTexture( jupiter, GL_TRUE);
    //jupiterTexture = LoadBitmap("poze/jupiter.bmp");
    //////////////////////////////////////////////////
    // textura planeta saturn
    saturn = gluNewQuadric();
    gluQuadricTexture( saturn, GL_TRUE);
    //saturnTexture = LoadBitmap("poze/saturn.bmp");
    //////////////////////////////////////////////////
    // textura planeta uranus
    uranus = gluNewQuadric();
    gluQuadricTexture( uranus, GL_TRUE);
    //uranusTexture = LoadBitmap("poze/uranus.bmp");
    //////////////////////////////////////////////////
    // textura planeta neptun
    neptun = gluNewQuadric();
    gluQuadricTexture( neptun, GL_TRUE);
    //neptunTexture = LoadBitmap("poze/neptune.bmp");
    //////////////////////////////////////////////////
    // textura planeta pluto
    pluto = gluNewQuadric();
    gluQuadricTexture( pluto, GL_TRUE);
    //plutoTexture = LoadBitmap("poze/pluto.bmp");
    //////////////////////////////////////////////////
    gluPerspective(80.0f, (GLfloat)width/(GLfloat)height, 2.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);

}



static void display(void)
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
glEnable ( GL_TEXTURE_2D );
// Planete ////////////////////////////////////////
glBindTexture ( GL_TEXTURE_2D, sunTexture);
glBindTexture ( GL_TEXTURE_2D, mercurTexture);
glBindTexture ( GL_TEXTURE_2D, venusTexture);
glBindTexture ( GL_TEXTURE_2D, pamantTexture);
glBindTexture ( GL_TEXTURE_2D, lunaTexture);
//de aici in jos probleme!!!
glBindTexture ( GL_TEXTURE_2D, marteTexture);
glBindTexture ( GL_TEXTURE_2D, jupiterTexture);
glBindTexture ( GL_TEXTURE_2D, saturnTexture);
glBindTexture ( GL_TEXTURE_2D, uranusTexture);
glBindTexture ( GL_TEXTURE_2D, neptunTexture);
glBindTexture ( GL_TEXTURE_2D, plutoTexture);
//////////////////////////////////////////////////
glMatrixMode(GL_MODELVIEW);
glLoadIdentity(); //initializarea sistemului de coordonate
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

/* Soare */
    glColor3f(1.0f, 0.5f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-20); //deplasat pe axele x, y, z
    glRotatef(axisRot,0,1,0); //rotit pe axa Y
    gluSphere(sun, 2, 20, 20); //sfera
    glPopMatrix();

/* Planeta Mercur */
    glColor3f(1.0f, 0.8f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-37); //deplasat pe axele x, y, z
    glRotatef(globRotR, 0,0,1);
    glTranslatef(5.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotit pe axa Y
    gluSphere(mercur, 0.3, 20, 20); //sfera
    glPopMatrix();

/* Planeta Venus */
    glColor3f(1.0f, 0.6f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-34); //deplasat pe axele x, y, z
    glRotatef(globRotG, 0,0,1);
    glTranslatef(5.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotit pe axa Y
    gluSphere(venus, 0.6, 20, 20); //sfera
    glPopMatrix();

/* Planeta Pamant (Terra) */
    glColor3f(0.196078f, 0.6f, 0.8f);
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-30); //deplasat pe axele x, y, z
    glRotatef(globRotB, 0,0,1);
    glTranslatef(5.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotit pe axa Y
    gluSphere(pamant, 0.8, 20, 20); //sfera
    glPopMatrix();

/* Luna (Terra) */
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-25.5); //deplasat pe axele x, y, z
    glRotatef(globRotB, 0,0,1);
    glTranslatef(5.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotit pe axa Y
    gluSphere(luna, 0.2, 20, 20); //sfera
    glPopMatrix();

/* Planeta Marte */
    glColor3f(1.0f, 0.3f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-23); //deplasat pe axele x, y, z
    glRotatef(globRotM, 0,0,1);
    glTranslatef(5.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotit pe axa Y
    gluSphere(marte, 0.5, 20, 20); //sfera
    glPopMatrix();

/* Planeta Jupiter */
    glColor3f(0.647059f, 0.164706f, 0.164706f);
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-20); //deplasat pe axele x, y, z
    glRotatef(globRotJ, 0,0,1);
    glTranslatef(5.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotit pe axa Y
    gluSphere(jupiter, 1, 20, 20); //sfera
    glPopMatrix();

/* Planeta Saturn */
    glColor3f(0.55f, 0.47f, 0.14f);
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-16); //deplasat pe axele x, y, z
    glRotatef(globRotS, 0,0,1);
    glTranslatef(5.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotit pe axa Y
    gluSphere(saturn, 0.4, 20, 20); //sfera
    glPopMatrix();

/* Planeta Uranus */
    glColor3f(0.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-14); //deplasat pe axele x, y, z
    glRotatef(globRotU, 0,0,1);
    glTranslatef(5.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotit pe axa Y
    gluSphere(uranus, 0.4, 20, 20); //sfera
    glPopMatrix();

/* Planeta Neptun */
    glColor3f(0.196078f, 0.5f, 0.7f);
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-10); //deplasat pe axele x, y, z
    glRotatef(globRotN, 0,0,1);
    glTranslatef(5.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotit pe axa Y
    gluSphere(neptun, 0.3, 20, 20); //sfera
    glPopMatrix();

/* Planeta Pluto */
    glColor3f(0.658824f, 0.658824f, 0.658824f);
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-6); //deplasat pe axele x, y, z
    glRotatef(globRotP, 0,0,1);
    glTranslatef(5.0f,0.0f,0.0f);
    glRotatef(axisRot,0,1,0); //rotit pe axa Y
    //glutSolidSphere(0.2,20,20); //sfera
    gluSphere(pluto, 0.2, 20, 20); //sfera
    glPopMatrix();


axisRot += 0.1f; axisRot=fmod(axisRot, 360.0f);
globRotR += 2.0f; globRotR=fmod(globRotR, 360.0f);
globRotG += 1.7f; globRotG=fmod(globRotG, 360.0f);
globRotB += 1.3f; globRotB=fmod(globRotB, 360.0f);
globRotM += 1.0f; globRotM=fmod(globRotM, 360.0f);
globRotJ += 0.7f; globRotJ=fmod(globRotJ, 360.0f);
globRotS += 0.5f; globRotS=fmod(globRotS, 360.0f);
globRotU += 0.3f; globRotU=fmod(globRotU, 360.0f);
globRotN += 0.2f; globRotN=fmod(globRotN, 360.0f);
globRotP += 0.1f; globRotP=fmod(globRotP, 360.0f);


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
	switch(key)
	{
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
	case 'e': /* exit */
	     exit(0);
        break;
	default:
		break;
	}
	glutPostRedisplay();
}

/* Punct de intrare in program */
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
