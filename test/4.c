#define SWI_STATEFUL_MODEL
#include <6.0g/swi.h>

int x, y;
void render(void) {
   setFill(255, 255, 255);
   setStroke(255, 255, 255);
   set_point1(0, 0);
   set_point2(640, 480);
   draw_rect();
   
   
   int cx = 2 - (x - 320) + 320;
   int cy = 2 - (y - 240) + 240;
   setStroke(255, 0, 0);
   setFill(255, 0, 255);   
   set_point1(cx - 60, cy - 60);
   set_point2(cx + 60, cy + 60);
   draw_ellipse();
   
   setStroke(255, 0, 0);
   setFill(0, 255, 0);   
   set_point1(x - 60, y - 60);
   set_point2(x + 60, y + 60);
   draw_ellipse();
  
}

void mymsg(void) {
	to_msg_target();
	if (event(WM_MOUSEMOVE)) {
	    getMouseXY(x, y);
	    InvalidateRect(swi_current, 0, 0);
	    set_msg_handled(true);
	    set_msg_result(1);
	}
	if (event(WM_PAINT)) {
	    run_paint(render);
	    set_msg_handled(true);
	    set_msg_result(1);
	}
}

int main(void) {
    window();
    setUserHandler(mymsg);
    setText("Hello world");
    setVisible(true);
 	eventloop();
}

