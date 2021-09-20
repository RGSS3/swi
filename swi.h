#ifndef SWI_H_
#define SWI_H_
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#ifdef SWI_OPENGL
#include <gl/gl.h>
#include <gl/glu.h>
#endif

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)

#define WINASSERT(x) do{DWORD err;\
	if(!(x) || ((err=GetLastError())!=0)){\
		LPTSTR str=0;\
		if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,0,err,0,(LPTSTR)&str,1,NULL)){\
			fwprintf(stderr, L"SWI_ASSERT FAILED: " WIDEN(#x) " @ " WIDEN(__FILE__) " GetLastError()=%d (%s)\n",err, str);\
		}else{fwprintf(stderr, L"SWI_ASSERT FAILED: " WIDEN(#x) " @ " WIDEN(__FILE__) " GetLastError()=%d (SPC Unknown Error)\n",err);}abort();\
	}\
}while(0);

static LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

__attribute__((constructor)) void register_class(void) {
	WNDCLASSEX wc;
	
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc;
	wc.style         = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszClassName = "SWICLASS";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		exit(1);
	}
}


HWND swi_window(void) {
    return CreateWindowEx(0, "SWICLASS", "", 0x80cf0000, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, 0, 0, 0, 0);
}

#ifdef SWI_OPENGL
HWND swi_openglwindow(void) {
	HWND hwnd = swi_window();
	PIXELFORMATDESCRIPTOR pfd= {
    	sizeof(pfd),
    	1,
    	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER ,
    	PFD_TYPE_RGBA,
    	32,
    	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
    };
    HDC hdc=GetDC(hwnd);
    assert(hdc);
	WINASSERT(hdc!=NULL);
    SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);
    HGLRC hrc=wglCreateContext(hdc);
    WINASSERT(hrc!=NULL);
    WINASSERT(wglMakeCurrent(hdc, hrc));
    ReleaseDC(hwnd, hdc);
    return hwnd;
}

void swi_glFlat(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);  
    glLoadIdentity(); 	
}


#endif

HWND swi_button(HWND par) {
    return CreateWindowEx(0, "Button", "", WS_CHILD|WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 200, 200, par, 0, 0, 0);
}

HWND swi_textbox(HWND par) {
    return CreateWindowEx(0, "Edit", "", WS_CHILD|WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 200, 200, par, 0, 0, 0);
}

HWND swi_listbox(HWND par) {
    return CreateWindowEx(0, "ListBox", "", WS_CHILD|WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 200, 200, par, 0, 0, 0);
}

HWND swi_static(HWND par) {
    return CreateWindowEx(0, "Static", "", WS_CHILD|WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 200, 200, par, 0, 0, 0);
}

LONG swi_getStyle(HWND h) {
	return GetWindowLong(h, GWL_STYLE);
}

void swi_setStyle(HWND h, LONG new) {
	SetWindowLong(h, GWL_STYLE, new);
}

void swi_modifyStyle(HWND h, LONG add, LONG remove) {
	LONG old = GetWindowLong(h, GWL_STYLE);
	old |= add;
	old &= ~remove;
	SetWindowLong(h, GWL_STYLE, old);
}


LONG swi_getExStyle(HWND h) {
	return GetWindowLong(h, GWL_EXSTYLE);
}

void swi_setExStyle(HWND h, LONG new) {
	SetWindowLong(h, GWL_EXSTYLE, new);
}

void swi_modifyExStyle(HWND h, LONG add, LONG remove) {
	LONG old = GetWindowLong(h, GWL_EXSTYLE);
	old |= add;
	old &= ~remove;
	SetWindowLong(h, GWL_EXSTYLE, old);
}

void swi_setText(HWND h, const char *text) {
    SetWindowText(h, text);	
}

void swi_setVisible(HWND h, bool visible) {
    if (visible) {
	    ShowWindow(h, SW_SHOW);
	} else {
		ShowWindow(h, SW_HIDE);
	}
}

char* swi_aGetText(HWND h) {
	SetLastError(0);
    int len = GetWindowTextLength(h);
    if (len == 0 && GetLastError() != 0) {
    	return NULL;	
    }
    char *ret = malloc((size_t)(len + 1));
    GetWindowText(h, ret, len + 1);
    return ret;
}

void swi_setPos(HWND h, int x, int y) {
    SetWindowPos(h, 0, x, y, 0, 0,  SWP_NOZORDER | SWP_NOSIZE);
}

void swi_setSize(HWND h, int x, int y) {
    SetWindowPos(h, 0, 0, 0, x, y,  SWP_NOZORDER | SWP_NOMOVE);
}


LOGFONT *swi_aGetFont(HWND h) {
	HFONT hfont = (HFONT)SendMessage(h, WM_GETFONT, 0, 0);
	LOGFONT *l = malloc(sizeof(*l));
	if (hfont == NULL) {
		memset(l, 0, sizeof(*l));
		l->lfCharSet = DEFAULT_CHARSET;
		l->lfHeight = 14;
		l->lfWeight = 500;
		strcpy(l->lfFaceName, "Consolas");
	} else {
		GetObject(hfont, sizeof(*l), l);
	}
	return l;
}

void swi_setFont(HWND h, LOGFONT *l) {
	HDC hdc = GetDC(h);
	HFONT hfont = CreateFontIndirect(l);
	HFONT hold = SelectObject(hdc, hfont);
	SendMessage(h, WM_SETFONT, (WPARAM)hfont, 0);
	DeleteObject((HGDIOBJ)hold);
	ReleaseDC(h, hdc);
}


int swi_peek(MSG *msg) {
	return PeekMessage(msg,  0, 0, 0, PM_NOREMOVE);
}

int swi_poll(MSG *msg) {
	return PeekMessage(msg,  0, 0, 0, PM_REMOVE);
}

void swi_process1(MSG *msg) {
	TranslateMessage(msg);	
}

LRESULT swi_process2(MSG *msg) {
	return DispatchMessage(msg);	
}

LRESULT swi_process(MSG *msg) {
	swi_process1(msg);
	return swi_process2(msg);
}

bool swi_isevent(MSG *msg, HWND h, UINT m) {
	return msg->hwnd == h && msg->message == m;
}

char *swi_asprintf(char const *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	size_t size = (size_t)vsnprintf(NULL, 0, fmt, va);
	char *ret = malloc(size + 1);
	vsnprintf(ret, size + 1, fmt, va);
	va_end(va);
	return ret;
}

void swi_swapBuffers(HWND h) {
	HDC hdc = GetDC(h);
	SwapBuffers(hdc);
	ReleaseDC(h, hdc);	
}

typedef bool SWI_MSGFUNC(MSG *);
typedef void SWI_RENDERFUNC(void);
void swi_mainWindowLoop(SWI_MSGFUNC *msgfunc, SWI_RENDERFUNC *render) {
	while (1) {
		MSG msg;
		if (swi_poll(&msg)) {
			if (swi_isevent(&msg, 0, WM_QUIT)) {
				break;
			}
			if (msgfunc(&msg)) {
				continue;
			}
			swi_process(&msg);
		} else {
			render();
		}
	}	
}

void swi_mainLoop(SWI_MSGFUNC *msgfunc, SWI_RENDERFUNC *render) {
	while (1) {
		MSG msg;
		if (swi_poll(&msg)) {
			if (msgfunc(&msg)) {
				continue;
			}
			swi_process(&msg);
		} else {
			render();
		}
	}	
}

#ifndef SWI_NO_SHORTNAMES
    #define window swi_window
    #define openglwindow swi_openglwindow
    #define button swi_button
    #define textbox swi_textbox
    #define listbox swi_listbox
    #define label swi_static
    #define setText swi_setText
    #define aGetText swi_aGetText
    #define setPos swi_setPos
    #define setSize swi_setSize
    #define setVisible swi_setVisible
    #define peek swi_peek
    #define poll swi_poll
    #define process swi_process
    #define process1 swi_process1
    #define process2 swi_process2
    #define isevent swi_isevent
    
    #define aGetFont swi_aGetFont
    #define setFont swi_setFont
    #define setStyle swi_setStyle
    #define getStyle swi_getStyle
    #define modifyStyle swi_modifyStyle
    #define setExStyle swi_setExStyle
    #define getExStyle swi_getExStyle
    #define modifyExStyle swi_modifyExStyle
    #define asprintf swi_asprintf
    #define var __auto_type
    #define swapBuffers swi_swapBuffers
    
    #define glFlat swi_glFlat
    #define mainLoop swi_mainLoop
    #define mainWindowLoop swi_mainWindowLoop
#endif

#endif
