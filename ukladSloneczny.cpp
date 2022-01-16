#define _USE_MATH_DEFINES
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <thread>
using namespace std;

typedef float point3[3];
typedef float point9[9];
static GLfloat viewer[] = { 100.0, 500.0, 100.0 , 0.0, 200.0 , 0.0 };
static GLfloat azymuth = 0;
static GLfloat elevation = 0;
static GLfloat azymuth2 = 0;
static GLfloat elevation2 = 0;
static int x_pos_old = 0;
static int delta_x = 0;
static int y_pos_old = 0;
static int delta_y = 0;
static GLfloat pix2angle;
static GLint statusMiddle = 0;
static GLint statusLeft = 0;
static GLint statusRight = 0;
int r = 10;
int days[] = { 88,224,365,686,4333,10756,30707, 60223 };
int speed = 1;
long long day = 0;
time_t start;
int radius[] = { 57,108,149,227,778,1433,2872,4498 };
float planetSize[] = { 4.87, 12.1, 12.76, 6.79, 71, 60, 25, 24, 30 };
bool simulation = false;

int segments = 1000;

void tekstura(const char plik[]);

void orbit(int planet) {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++) {
		float angle = 1.0 * float(i) / float(360);
		float x = planet * cos(2 * M_PI * angle);
		float y = planet * sin(2 * M_PI * angle);

		glVertex3f(x, 0, y);
	}
	glEnd();
}

void orbits() {
	tekstura("orbits.tga");
	for (int planet : radius) {
		orbit(planet);
	}
}

void sun() {
	tekstura("sun.tga");
	glutSolidSphere(planetSize[8], segments, segments);
}

void mercury() {
	float angle = 1.0 * day / days[0];
	float x = cos(2 * angle * M_PI) * radius[0];
	float y = -1 * sin(2 * angle * M_PI) * radius[0];

	glTranslatef(x, 0, y);
	tekstura("mercury.tga");
	glutSolidSphere(planetSize[0], segments, segments);
	glTranslatef(-x, 0, -y);
}

void venus() {
	float angle = 1.0 * day / days[1];
	float x = cos(2 * angle * M_PI) * radius[1];
	float y = -1 * sin(2 * angle * M_PI) * radius[1];

	glTranslatef(x, 0, y);
	tekstura("venus.tga");
	glutSolidSphere(planetSize[1], segments, segments);
	glTranslatef(-x, 0, -y);
}

void earth() {
	float angle = 1.0 * day / days[2];
	float x = cos(2 * angle * M_PI) * radius[2];
	float y = -1 * sin(2 * angle * M_PI) * radius[2];

	glTranslatef(x, 0, y);
	tekstura("earth.tga");
	glutSolidSphere(planetSize[2], segments, segments);
	glTranslatef(-x, 0, -y);
}

void mars() {
	float angle = 1.0 * day / days[3];
	float x = cos(2 * angle * M_PI) * radius[3];
	float y = -1 * sin(2 * angle * M_PI) * radius[3];

	glTranslatef(x, 0, y);
	tekstura("mars.tga");
	glutSolidSphere(planetSize[3], segments, segments);
	glTranslatef(-x, 0, -y);
}

void jupiter() {
	float angle = 1.0 * day / days[4];
	float x = cos(2 * angle * M_PI) * radius[4];
	float y = -1 * sin(2 * angle * M_PI) * radius[4];

	glTranslatef(x, 0, y);
	tekstura("jupiter.tga");
	glutSolidSphere(planetSize[4], segments, segments);
	glTranslatef(-x, 0, -y);
}

void saturn() {
	float angle = 1.0 * day / days[5];
	float x = cos(2 * angle * M_PI) * radius[5];
	float y = -1 * sin(2 * angle * M_PI) * radius[5];

	glTranslatef(x, 0, y);
	tekstura("saturn.tga");
	glutSolidSphere(planetSize[5], segments, segments);
	glTranslatef(-x, 0, -y);
}

void uranus() {
	float angle = 1.0 * day / days[6];
	float x = cos(2 * angle * M_PI) * radius[6];
	float y = -1 * sin(2 * angle * M_PI) * radius[6];

	glTranslatef(x, 0, y);
	tekstura("uranus.tga");
	glutSolidSphere(planetSize[6], segments, segments);
	glTranslatef(-x, 0, -y);
}

void neptune() {
	float angle = 1.0 * day / days[7];
	float x = cos(2 * angle * M_PI) * radius[7];
	float y = -1 * sin(2 * angle * M_PI) * radius[7];

	glTranslatef(x, 0, y);
	tekstura("neptune.tga");
	glutSolidSphere(planetSize[7], segments, segments);
	glTranslatef(-x, 0, -y);
}

void zoom(bool zoom) {
	float x = (viewer[3] - viewer[0]);
	float y = (viewer[4] - viewer[1]);
	float z = (viewer[5] - viewer[2]);
	if (zoom) {
		x *= -1;
		y *= -1;
		z *= -1;
	}
	viewer[0] += x;
	viewer[1] += y;
	viewer[2] += z;
	viewer[3] += x;
	viewer[4] += y;
	viewer[5] += z;
}


void ukladSloneczny() {
	if (statusRight == 1) {
		zoom(delta_y > 0);
	}

	if (statusLeft == 1) {
		azymuth += delta_x * pix2angle * 0.01;
		elevation += delta_y * pix2angle * 0.01;
	}
	viewer[3] = 10 * cos(azymuth) * cos(elevation) + viewer[0];
	viewer[4] = 10 * sin(elevation) + viewer[1];
	viewer[5] = 10 * sin(azymuth) * cos(elevation) + viewer[2];

	orbits();
	sun();
	mercury();
	venus();
	earth();
	mars();
	jupiter();
	saturn();
	uranus();
	neptune();
}

void printDay() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	tekstura("orbits.tga");
	glRasterPos2i(10, 970);
	char printDay[20];
	sprintf_s(printDay, " day: %d", day);
	for (int i = 0; i < 20; ++i) {
		glColor3f(1, 1, 1);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], viewer[3], viewer[4], viewer[5], 0.0, 1.0, 0.0);

	ukladSloneczny();
	printDay();

	glutSwapBuffers();
}


void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {//sprawdzenie czy przycisniety zostal prawy klawisz
		x_pos_old = x;
		y_pos_old = y;
		statusRight = 1;        //ustawienie flagi przycisku   
	}
	else {
		statusRight = 0;		//ustawienie flagi przycisku  
	}

	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {//sprawdzenie czy przycisniety zostal lewy klawisz
		x_pos_old = x;
		y_pos_old = y;
		statusLeft = 1;    //ustawienie flagi przycisku        
	}
	else {
		statusLeft = 0;		//ustawienie flagi przycisku  
	}

	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {//sprawdzenie czy przycisniety zostal lewy klawisz
		x_pos_old = x;
		y_pos_old = y;
		statusMiddle = 1;    //ustawienie flagi przycisku    
	}
	else {
		statusMiddle = 0;		//ustawienie flagi przycisku  
	}

	RenderScene();
}

void keys(unsigned char key, int x, int y)
{
	if (key == 'r') {
		viewer[0] = 0;
		viewer[1] = 100;
		viewer[2] = 0;
		viewer[3] = 0;
		viewer[4] = 0;
		viewer[5] = 0;
	}
	if (key == 'u') {
		speed++;
		if (speed > 10) {
			speed = 10;
		}
	}
	if (key == 'j') {
		speed--;
		if (speed < -10) {
			speed = -10;
		}
	}

	RenderScene();
}

void Motion(GLsizei x, GLsizei y)
{
	delta_x = x - x_pos_old;//zmiana pozycji x
	x_pos_old = x;

	delta_y = y - y_pos_old;//zmiana pozycji y
	y_pos_old = y;

	glutPostRedisplay();
}


GLbyte* LoadTGAImage(const char* FileName, GLint* ImWidth, GLint* ImHeight, GLint* ImComponents, GLenum* ImFormat)
{
#pragma pack(1)           
	typedef struct
	{
		GLbyte    idlength;
		GLbyte    colormaptype;
		GLbyte    datatypecode;
		unsigned short    colormapstart;
		unsigned short    colormaplength;
		unsigned char     colormapdepth;
		unsigned short    x_orgin;
		unsigned short    y_orgin;
		unsigned short    width;
		unsigned short    height;
		GLbyte    bitsperpixel;
		GLbyte    descriptor;
	}TGAHEADER;
#pragma pack(8)

	FILE* pFile;
	TGAHEADER tgaHeader;
	unsigned long lImageSize;
	short sDepth;
	GLbyte* pbitsperpixel = NULL;


	/*************************************************************************************/

	// Wartoi domyne zwracane w przypadku b喚du

	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;

#pragma warning(suppress : 4996)
	pFile = fopen(FileName, "rb");
	if (pFile == NULL)
		return NULL;

	/*************************************************************************************/
	// Przeczytanie nag堯wka pliku 


	fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);


	/*************************************************************************************/

	// Odczytanie szerokoi, wysokoi i g喚bi obrazu

	*ImWidth = tgaHeader.width;
	*ImHeight = tgaHeader.height;
	sDepth = tgaHeader.bitsperpixel / 8;


	/*************************************************************************************/
	// Sprawdzenie, czy g喚bia spe軟ia za這穎ne warunki (8, 24, lub 32 bity)

	if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
		return NULL;

	/*************************************************************************************/

	// Obliczenie rozmiaru bufora w pami璚i


	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;


	/*************************************************************************************/

	// Alokacja pami璚i dla danych obrazu


	pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

	if (pbitsperpixel == NULL)
		return NULL;

	if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
	{
		free(pbitsperpixel);
		return NULL;
	}


	/*************************************************************************************/

	// Ustawienie formatu OpenGL


	switch (sDepth)

	{

	case 3:

		*ImFormat = GL_BGR_EXT;

		*ImComponents = GL_RGB8;

		break;

	case 4:

		*ImFormat = GL_BGRA_EXT;

		*ImComponents = GL_RGBA8;

		break;

	case 1:

		*ImFormat = GL_LUMINANCE;

		*ImComponents = GL_LUMINANCE8;

		break;

	};



	fclose(pFile);



	return pbitsperpixel;

}

void tekstura(const char plik[]) {//plik to nazwa wczytywanego pliku
	GLbyte* pBytes;
	GLint ImWidth, ImHeight, ImComponents;
	GLenum ImFormat;

	pBytes = LoadTGAImage(plik, &ImWidth, &ImHeight, &ImComponents, &ImFormat);//wczytanie tekstury
	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);//zdefiniowanie tekstury
	free(pBytes);

	glEnable(GL_CULL_FACE);//uruchomienie teksturowania jednostronnego
	glCullFace(GL_FRONT);//uruchomienie teksturowania frontu
	glEnable(GL_TEXTURE_2D);//uruchomienie tekstur
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//tryb teksturowania
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//sposob nakladania tekstur
}


void material() {
	float MatAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float MatDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float MatSpecular[] = { 0.1f, 0.1f, 0.0f, 0.1f };
	float MatShininess = 20;
	float black[] = { 0.4f,0.4f,0.4f,1.0f };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
}

void light() {
	GLfloat att_constant = { 1.0 };
	GLfloat att_linear = { 0.05 };
	GLfloat att_quadratic = { 0.001 };
	float LightAmbient[] = { 0.1f, 0.1f, 0.05f, 1.0f };
	float LightEmission[] = { 1.0f, 1.0f, 0.8f, 1.0f };
	float LightDiffuse[] = { 1.0f, 1.0f, 0.8f, 1.0f };
	float LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float LightPosition[] = { 0.0f, 0.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void MyInit()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	material();
	light();
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;//usatwienie wielkosci do obracania obiektu
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 1.6, 1.0, 100000.0);

	if (horizontal <= 1.6*vertical)
		glViewport(0,(vertical - horizontal/1.6) / 2, 1.6*vertical, vertical);

	else
		glViewport((horizontal - 1.6 * vertical) / 2, 0, 1.6 * vertical, vertical);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void dayByDay() {
	day += speed;
	Sleep(5);
	RenderScene();

	if (day >= LLONG_MAX - 2) {
		day = LLONG_MIN + 2;
	}
	if (day <= LLONG_MIN + 2) {
		day = LLONG_MAX - 2;
	}
}


void main(void)
{
	srand(time(NULL));
	start = clock();

	cout << "" << endl;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1600, 1000);
	glutCreateWindow("Układ Słoneczny");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	MyInit();
	glutMouseFunc(Mouse);//"lapanie" akcji na przyciskach myszy
	glutMotionFunc(Motion);//"lapanie" ruchu myszki
	glutKeyboardFunc(keys);//"lapanie" akcji na klawiaturze
	glEnable(GL_DEPTH_TEST);
	glutIdleFunc(dayByDay);
	glutMainLoop();
}