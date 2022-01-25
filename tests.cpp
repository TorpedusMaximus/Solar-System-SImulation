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

int func(void) {
	
	double e = 0.2;
	double baRatio = sqrt(1 - pow(e, 2));
	double a = 0.57;
	double b = a * baRatio;

	GLdouble p = pow(b, 2) / a;
	GLdouble M = 2 * p * (double)(1.0/6);
	GLdouble E[2];

	E[1] = M;

	do {
		E[0] = E[1];
		E[1] = M + e * sin(E[0]);
		cout << E[0] << " " << E[1] << endl;
	} while (abs(E[1] - E[0]) > 0.01);






	return 0;
}