#define SWI_STATEFUL_MODEL
#include <6.0g/swi.h>

int x, y;

void render() {
   to_msg_target();
   to_paint();

   setFill(255, 255, 255);
   setStroke(255, 255, 255);
   Rectangle(swi_hdc, 0, 0, 640, 480);
   
   setStroke(255, 0, 0);
   setFill(0, 255, 0);   
   Ellipse(swi_hdc, x - 60, y - 60, x + 60, y + 60);
   
   end_paint();
}


swi_result mymsg(void) {
	to_msg_target();
	if (event(WM_MOUSEMOVE)) {
	    getMouseXY(x, y);
	    InvalidateRect(swi_current, 0, 0);
	    return 1;
	}
	if (event(WM_PAINT)) {
	    render();
	    return 1;
	}
	return swi_call_default();
}


int main(void) {
    window();
    modifyStyle(0, WS_THICKFRAME);
    setUserHandler(mymsg);
    setText("Hello world");
    setVisible(true);
 	eventloop();
}

