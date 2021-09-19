# swi
Simple Windows/Window Interface, no not that SWI-   
GCC only   
One needs no VS and One who needs VS needs no swi   


Please add '-lgdi32' in your linker options

## Sample
```c
#include <6.0g/swi.h>
int main(void) {
	var h = window();
	setSize(h, 640, 480);
	setPos(h, 200, 200);

	var font = aGetFont(h); // prefix 'a' means the object is (m)alloc'ed, you should free after use, like in asprintf
	strcpy(font->lfFaceName, "Cambria");
	font->lfHeight = 14;
	var t1 = textbox(h);
	setSize(t1, 100, 20);
	setPos(t1, 10, 10);
	setFont(t1, font);

	var t2 = textbox(h);
	setSize(t2, 100, 20);
	setPos(t2, 120, 10);
	setFont(t2, font);


	var b = button(h);
	setText(b, "Add");
	setSize(b, 100, 20);
	setPos(b, 10, 60);
	setFont(b, font);
	modifyStyle(b, BS_FLAT, 0);

	var t3 = textbox(h);
	setSize(t3, 100, 20);
	setPos(t3, 10, 110);
	setFont(t3, font);

	free(font);


	setVisible(h, true);
	while (1) {
		MSG msg;
		if (poll(&msg)) {
			if (isevent(&msg, b, WM_LBUTTONUP)) {
				char *a1 = aGetText(t1);
				char *a2 = aGetText(t2);
				int v1 = (int)strtol(a1, NULL, 10);
				int v2 = (int)strtol(a2, NULL, 10);
				int v3 = v1 + v2;
				char *a3 = swi_asprintf("%d", v3);
				setText(t3, a3);
				free(a3);
				free(a2);
				free(a1);
			}

			if (isevent(&msg, 0, WM_QUIT)) {
				break;
			}
			process(&msg);
		}
	}
}
```
