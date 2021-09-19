#define SWI_OPENGL
#include <6.0g/swi.h>
#include <gl/gl.h>
#include <gl/glu.h>
float a = 0;
void render(void) {
	a += 0.1f;
	glClearColor(0.4f, 0.8f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(a, 0);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(a + 100, 0);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(a + 100, 100);
	glEnd();
	if (a > 640) a = 0;
}

int main(void) {
	var h = swi_openglwindow();
	setSize(h, 640, 480);
	setPos(h, 200, 200);
	setVisible(h, true);

    glViewport(0, 0, 640, 480);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
    glMatrixMode(GL_MODELVIEW);  
    glLoadIdentity(); 
    
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
			Sleep(10);
		}
	}
}
