#define SWI_OPENGL
#include <6.0g/swi.h>
#include <math.h>
int x = 0;
void render(void) {
	glClearColor(0.4f, 0.8f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_LINES);
	double p = 2 * M_PI / 3600;
	x+=10;
	for (int i = x; i < 1200 + x; ++i) {
		double x1 = 320 + cos(i * p) * 40;
		double y1 = 240 + sin(i * p) * 40;
		double x2 = 320 + cos((i + 1) * p) * 40;
		double y2 = 240 + sin((i + 1) * p) * 40;
		glColor3f(1, 0, 0);
		glVertex2f((float)x1, (float)y1);
		glColor3f(1, 0, 0);
		glVertex2f((float)x2, (float)y2);
	}
	glEnd();
}

int main(void) {
	var h = swi_openglwindow();
	setSize(h, 640, 480);
	setPos(h, 200, 200);
	setVisible(h, true);
	glFlat(640, 480);
	while (1) {
		MSG msg;
		if (poll(&msg)) {
			if (isevent(&msg, 0, WM_QUIT)) {
				break;
			}
			process(&msg);
		} else {
			render();
			swapBuffers(h);
		}
	}
}
