#define SWI_OPENGL
#include "../swi.h"
#include <math.h>
size_t x = 0;
size_t speed = 10;
HWND h;
void render(void) {
	glClearColor(0.4f, 0.8f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_LINES);
	double p = 2 * M_PI / 3600;
	x += speed;
	for (size_t i = x; i < 1200 + x; ++i) {
		double x1 = 320 + cos((double)i * p) * 80;
		double y1 = 240 + sin((double)i * p) * 80;
		double x2 = 320 + cos((double)(i + 1) * p) * 80;
		double y2 = 240 + sin((double)(i + 1) * p) * 80;
		glColor3f(1, 0, 0);
		glVertex2f((float)x1, (float)y1);
		glColor3f(1, 0, 0);
		glVertex2f((float)x2, (float)y2);
	}
	glEnd();
	
	swapBuffers(h);
	Sleep(10);
}

bool msgfunc(MSG *msg) {
	if (isevent(msg, msg->hwnd, WM_LBUTTONDOWN)) {
		speed = 50;	
		return true;
	}
	
	if (isevent(msg, msg->hwnd, WM_LBUTTONUP)) {
		speed = 10;	
		return true;
	}
	
	return false;
}

int main(void) {
	h = swi_openglwindow();
	setSize(h, 640, 480);
	setPos(h, 200, 200);
	setVisible(h, true);
	setGL2D(640, 480);
	mainWindowLoop(msgfunc, render);
}
