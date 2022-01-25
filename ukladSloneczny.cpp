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

//todo:
//orbital velocity
//angles of orbits
//light

typedef float point3[3];
typedef float point9[9];

static GLfloat viewer[] = { 0.0, 500.0, 0.0 , 0.0, 0.0 , 0.0 };
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
int day = 0;
time_t start;
int radius[] = { 57,108,149,227,778,1433,2872,4498 };
float planetSize[] = { 4.87, 12.1, 12.76, 6.79, 71, 60, 25, 24, 30 };
bool simulation = false;
float ring[3600];
int segments = 40;
double rings[] = { 102,102.3,102.6,103,103.8,104,110,115,119,125,132,136,141,146,149,155,156,158,160,165 };
GLfloat planetTilt[] = { 0, 177.3, 23.4, 25.2, 3.1, 26.7, 97.8, 28.3, 0 };
GLfloat rotation[] = { 1407, 5832, 24, 24, 10, 11, 17, 16, 625 };
GLfloat orbitsEccentrity[] = { 0.206, 0.007, 0.017, 0.093, 0.048, 0.056, 0.046, 0.010 };
GLbyte* textures[10];
GLint ImWidth[10], ImHeight[10], ImComponents[10];
GLenum ImFormat[10];
GLUquadric* sphere;
int hour = 0;

bool show = true;
bool click = false;


void texture(int textureID);

void planetRotation(int planetID) {
	glRotated(-90.0, 1.0, 1.0, 0.0);

	glRotated(-planetTilt[planetID], 1.0, 0.0, 0.0);

	glRotated(360 * ((((double)day * 24) + hour) / rotation[planetID]), 0.0, 0.0, 1.0);

	gluSphere(sphere, planetSize[planetID], segments, segments);

	glRotated(-360 * (((double)day * 24 + hour) / rotation[planetID]), 0.0, 0.0, 1.0);

	glRotated(planetTilt[planetID], 1.0, 0.0, 0.0);

	glRotated(90.0, 1.0, 1.0, 0.0);
}

void translateAngle(int r, GLdouble elipseAngle, GLdouble& x, GLdouble& y) {
	x = cos(elipseAngle) * r;
	y = -1 * sin(elipseAngle) * r;
}

void radiusAngle(int planetID, GLdouble& r, GLdouble& angle) {
	GLdouble time = (double)(day + ((1.0 * hour) / 24));
	GLdouble timeMax = (days[planetID]);
	GLdouble ratio = time / timeMax;
	angle = 2 * M_PI * ratio;

	double e = orbitsEccentrity[planetID];
	double baRatio = sqrt(1 - pow(e, 2));
	double a = radius[planetID];
	double b = a * baRatio;

	/*GLdouble p = pow(b, 2) / a;             //proba dostosowania predkosci planet (II prawo Keplera)
	GLdouble M = 2 * p * ratio;

	GLdouble E[2];

	E[1] = M;

	do {
		E[0] = E[1];
		E[1] = M + e * sin(E[0]);
	} while (abs(E[1] - E[0]) > 0.001);

	GLdouble cosn = (cos(E[1]) - e) / (1 - e * cos(E[1]));
	angle = cosh(cosn);
	if (planetID == 0)
		cout << angle << endl;*/

	double top = a * (1 - pow(e, 2));
	double bottom = 1 + e * cos(angle);

	r = top / bottom;
}

GLdouble orbitPoint(int planetID, GLdouble& r, GLdouble ratio) {
	GLdouble angle = 2 * ratio * M_PI;

	double e = orbitsEccentrity[planetID];
	double baRatio = sqrt(1 - pow(e, 2));
	double a = radius[planetID];
	double b = a * baRatio;

	double top = a * (1 - pow(e, 2));
	double bottom = 1 + e * cos(angle);

	r = top / bottom;

	return angle;
}

void orbit(int planet) {
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 3600; i++) {
		GLdouble r, x, y;
		GLdouble ratio = 1.0 * GLdouble(i) / GLdouble(360);
		GLdouble angle = orbitPoint(planet, r, ratio);
		translateAngle(r, angle, x, y);
		glVertex3d(x, 0, y);
	}
	glEnd();
}

void orbits() {
	for (int i = 0; i < 9; i++) {
		orbit(i);
	}
}

void sun() {
	texture(8);
	glRotated(90, 1.0, 0.0, 0.0);
	glRotated(-360 * ((((double)day * 24) + hour) / rotation[8]), 0.0, 0.0, 1.0);
	gluSphere(sphere, planetSize[8], segments, segments);
	glRotated(360 * (((double)day * 24 + hour) / rotation[8]), 0.0, 0.0, 1.0);
	glRotated(-90, 1.0, 0.0, 0.0);
}

void planets() {
	for (int i = 0; i < 8; i++) {
		if (i == 5) {
			continue;
		}

		GLdouble r, elipseAngle;
		radiusAngle(i, r, elipseAngle);

		GLdouble x, y;
		translateAngle(r, elipseAngle, x, y);

		glTranslated(x, 0, y);
		texture(i);
		planetRotation(i);
		glTranslated(-x, 0, -y);
	}
}

void saturn() {
	GLdouble r, elipseAngle;
	radiusAngle(5, r, elipseAngle);

	GLdouble x, y;
	translateAngle(r, elipseAngle, x, y);

	glTranslated(x, 0, y);
	texture(5);
	planetRotation(5);
	glRotated(1.2 * planetTilt[5], -1.0, 0.0, 1.0);
	for (double i : rings) {
		glColor3f(1, 1, 1);

		glBegin(GL_LINE_LOOP);
		for (int ii = 0; ii < 3600; ii++) {
			float angle = 1.0 * float(ii) / float(3600);
			float xr = i * cos(2 * M_PI * angle);
			float yr = i * sin(2 * M_PI * angle);

			glVertex3f(xr, 0, yr);
		}
		glEnd();
	}

	glRotated(-1.2 * planetTilt[5], -1.0, 0.0, 1.0);
	glTranslated(-x, 0, -y);
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


void solarSystem() {
	if (statusRight == 1) {
		zoom(delta_y > 0);
	}

	if (statusLeft == 1) {
		azymuth += ((float)delta_x * pix2angle) * 0.01;
		elevation -= ((float)delta_y * pix2angle) * 0.01;
		if (sin(elevation) >= 0.99) {
			elevation = 1.44;
		}
		if (sin(elevation) <= -0.99) {
			elevation = -1.44;
		}
	}
	viewer[3] = 10 * cos(azymuth) * cos(elevation) + viewer[0];
	viewer[4] = 10 * sin(elevation) + viewer[1];
	viewer[5] = 10 * sin(azymuth) * cos(elevation) + viewer[2];

	orbits();
	sun();
	planets();
	saturn();
}

void printDay() {
	const int characters = 80;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	texture(9);
	glRasterPos2i(10, 970);
	char printDay[characters];
	sprintf_s(printDay, " Day: %d;", day);
	for (int i = 0; i < characters; ++i) {
		if (printDay[i] == ';') {
			break;
		}
		glColor3f(1, 1, 1);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
	}

	glRasterPos2i(10, 945);
	sprintf_s(printDay, " Hour: %d;", hour);
	for (int i = 0; i < characters; ++i) {
		if (printDay[i] == ';') {
			break;
		}
		glColor3f(1, 1, 1);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
	}

	glRasterPos2i(10, 920);
	sprintf_s(printDay, " Speed: %d hours/second;", speed * 10);
	for (int i = 0; i < characters; ++i) {
		if (printDay[i] == ';') {
			break;
		}
		glColor3f(1, 1, 1);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
	}

	glRasterPos2i(10, 895);
	sprintf_s(printDay, " Speed: %d days/second;", (int)(speed * 10) / 24);
	for (int i = 0; i < characters; ++i) {
		if (printDay[i] == ';') {
			break;
		}
		glColor3f(1, 1, 1);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
	}

	if (show) {
		glRasterPos2i(400, 600);
		sprintf_s(printDay, " u - increase speed;");
		for (int i = 0; i < characters; ++i) {
			if (printDay[i] == ';') {
				break;
			}
			glColor3f(1, 1, 1);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
		}
		glRasterPos2i(400, 560);
		sprintf_s(printDay, " j - decrease speed;");
		for (int i = 0; i < characters; ++i) {
			if (printDay[i] == ';') {
				break;
			}
			glColor3f(1, 1, 1);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
		}
		glRasterPos2i(400, 520);
		sprintf_s(printDay, " left mouse - move camera;");
		for (int i = 0; i < characters; ++i) {
			if (printDay[i] == ';') {
				break;
			}
			glColor3f(1, 1, 1);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
		}
		glRasterPos2i(400, 480);
		sprintf_s(printDay, " right mouse - zoom;");
		for (int i = 0; i < characters; ++i) {
			if (printDay[i] == ';') {
				break;
			}
			glColor3f(1, 1, 1);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
		}
	}

	glRasterPos2i(840, 970);
	sprintf_s(printDay, " x: %f;", viewer[0]);
	for (int i = 0; i < characters; ++i) {
		if (printDay[i] == ';') {
			break;
		}
		glColor3f(1, 1, 1);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
	}

	glRasterPos2i(840, 945);
	sprintf_s(printDay, " y: %f;", viewer[1]);
	for (int i = 0; i < characters; ++i) {
		if (printDay[i] == ';') {
			break;
		}
		glColor3f(1, 1, 1);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
	}

	glRasterPos2i(840, 920);
	sprintf_s(printDay, " z: %f;", viewer[2]);
	for (int i = 0; i < characters; ++i) {
		if (printDay[i] == ';') {
			break;
		}
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

	if (click && show) {
		time_t now = clock();
		if ((now - start) / CLOCKS_PER_SEC >= 3) {
			show = false;
		}
	}

	while (hour >= 24) {
		hour -= 24;
		day++;
	}
	while (hour <= -24) {
		hour += 24;
		day--;
	}

	solarSystem();
	printDay();

	int lightRadius = 50;
	int lightAngle = 60;
	for (int i = 0; i <= 5; i++) {

		GLfloat x = cos(i * lightAngle) * lightRadius;
		GLfloat y = -1 * sin(i * lightAngle) * lightRadius;
		GLfloat LightPosition[] = { x, 0.0f, y, 1.0f };
		glLightfv(GL_LIGHT0 + i, GL_POSITION, LightPosition);
	}

	GLfloat topLightPosition[] = { 0, lightRadius, 0, 1.0f };
	glLightfv(GL_LIGHT6, GL_POSITION, topLightPosition);
	GLfloat bottomLightPosition[]= { 0, -lightRadius, 0, 1.0f };
	glLightfv(GL_LIGHT7, GL_POSITION, bottomLightPosition);


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

	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {//sprawdzenie czy przycisniety zostal scroll
		x_pos_old = x;
		y_pos_old = y;
		statusMiddle = 1;    //ustawienie flagi przycisku    
	}
	else {
		statusMiddle = 0;		//ustawienie flagi przycisku  
	}
	if (!click) {    //wylaczenie pomocy
		click = true;
		start = clock();
	}

	RenderScene();
}

void keys(unsigned char key, int x, int y)
{
	int vectorMultiplier = 7;
	if (key == 'r') {
		viewer[0] = 0;
		viewer[1] = 500;
		viewer[2] = 0;
		viewer[3] = 0;
		viewer[4] = 0;
		viewer[5] = 0;

	}

	if (key == 'u') {
		speed++;
	}
	if (key == 'j') {
		speed--;
	}

	if (key == 'w') {
		double x = viewer[3] - viewer[0];
		double y = viewer[5] - viewer[2];
		double vector = sqrt(pow(x, 2) + pow(y, 2));

		viewer[3] += vectorMultiplier * x / vector;
		viewer[0] += vectorMultiplier * x / vector;
		viewer[5] += vectorMultiplier * y / vector;
		viewer[2] += vectorMultiplier * y / vector;

	}
	if (key == 's') {
		double x = viewer[3] - viewer[0];
		double y = viewer[5] - viewer[2];
		double vector = sqrt(pow(x, 2) + pow(y, 2));

		viewer[3] -= vectorMultiplier * x / vector;
		viewer[0] -= vectorMultiplier * x / vector;
		viewer[5] -= vectorMultiplier * y / vector;
		viewer[2] -= vectorMultiplier * y / vector;
	}
	if (key == 'a') {
		double x = viewer[3] - viewer[0];
		double y = viewer[5] - viewer[2];
		double vector = sqrt(pow(x, 2) + pow(y, 2));

		viewer[3] += vectorMultiplier * y / vector;
		viewer[0] += vectorMultiplier * y / vector;
		viewer[5] -= vectorMultiplier * x / vector;
		viewer[2] -= vectorMultiplier * x / vector;
	}
	if (key == 'd') {
		double x = viewer[3] - viewer[0];
		double y = viewer[5] - viewer[2];
		double vector = sqrt(pow(x, 2) + pow(y, 2));

		viewer[3] -= vectorMultiplier * y / vector;
		viewer[0] -= vectorMultiplier * y / vector;
		viewer[5] += vectorMultiplier * x / vector;
		viewer[2] += vectorMultiplier * x / vector;
	}

	if (!click) {
		click = true;
		start = clock();
	}
	if (key == 't') {
		day = 0;
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

void texture(int textureID) {//plik to nazwa wczytywanego pliku

	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents[textureID], ImWidth[textureID], ImHeight[textureID], 0, ImFormat[textureID], GL_UNSIGNED_BYTE, textures[textureID]);//zdefiniowanie tekstury

}

void loadTextures() {
	const char* plik[] = {
		"mercury.tga",
		"venus.tga",
		"earth.tga",
		"mars.tga",
		"jupiter.tga",
		"saturn.tga",
		"uranus.tga",
		"neptune.tga",
		"sun.tga",
		"orbits.tga"
	};
	GLbyte* pBytes;

	int i = 0;
	for (auto sciezka : plik) {
		pBytes = new GLbyte;
		pBytes = LoadTGAImage(sciezka, &ImWidth[i], &ImHeight[i], &ImComponents[i], &ImFormat[i]);//wczytanie tekstury
		textures[i] = pBytes;
		i++;
	}

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

	glEnable(GL_CULL_FACE);//uruchomienie teksturowania jednostronnego
	glCullFace(GL_BACK);//uruchomienie teksturowania frontu
	glEnable(GL_TEXTURE_2D);//uruchomienie tekstur
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//tryb teksturowania
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//sposob nakladania tekstur
}

void light() {
	GLfloat att_constant = { 0.7 };
	GLfloat att_linear = { 0.0000005 };
	GLfloat att_quadratic = { 0.00000000001 };
	float LightAmbient[] = { 0.1f, 0.1f, 0.05f, 1.0f };
	float LightEmission[] = { 1.0f, 1.0f, 0.8f, 1.0f };
	float LightDiffuse[] = { 1.0f, 1.0f, 0.8f, 1.0f };
	float LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	for (int i = 0; i <= 7; i++) {
		glLightfv(GL_LIGHT0 + i, GL_AMBIENT, LightAmbient);
		glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, LightDiffuse);
		glLightfv(GL_LIGHT0 + i, GL_SPECULAR, LightSpecular);
		glLightf(GL_LIGHT0 + i, GL_CONSTANT_ATTENUATION, att_constant);
		glLightf(GL_LIGHT0 + i, GL_LINEAR_ATTENUATION, att_linear);
		glLightf(GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, att_quadratic);
		glEnable(GL_LIGHT0 + i);
	}

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void MyInit()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	material();
	light();
	loadTextures();
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;//usatwienie wielkosci do obracania obiektu
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 1.6, 1.0, 100000.0);

	if (horizontal <= 1.6 * vertical)
		glViewport(0, (vertical - horizontal / 1.6) / 2, 1.6 * vertical, vertical);

	else
		glViewport((horizontal - 1.6 * vertical) / 2, 0, 1.6 * vertical, vertical);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void dayByDay() {
	hour += speed;
	Sleep(10);
	RenderScene();

	if (day >= INT_MAX - 5) {
		day = INT_MIN + 5;
	}
	if (day <= INT_MIN + 5) {
		day = INT_MAX - 5;
	}
}


void main(void)
{
	srand(time(NULL));
	start = clock();

	for (int i = 0; i < 3600; i++) {
		float angle = 1.0 * i / 3600;
		ring[i] = sin(2 * M_PI * angle);
	}

	cout << "" << endl;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1600, 1000);
	glutCreateWindow("Symulacja Układu Słonecznego - index: 252724");
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