#define SWI_STATEFUL_MODEL
#include <6.0g/swi.h>

int main(void) {
    window();
    var b = button();
    setText("Hello");
    setPos(10, 10);
    setSize(100, 30);
    modifyStyle(BS_FLAT, 0);
    
    ATWINDOW();
    setVisible(true);
    
    
    while (1) {
		if (poll()) {
			AT(b);
			if (event(WM_LBUTTONUP)) {
				MessageBox(swi_current_window, "Hello", 0, 16);
				continue;
			}
			
			AT(NULL);
			if (event(WM_QUIT)) {
				break;
			}
			process();
		}
	}	
}

