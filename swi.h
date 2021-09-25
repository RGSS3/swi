#ifndef SWI_H_
#define SWI_H_
#include <assert.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>


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


typedef LRESULT SWIPROC (void);

SWIPROC *swi_user = NULL;
static __thread HWND swi_current;
static __thread HWND swi_current_window;
static __thread MSG  swi_msg;
static __thread HDC  swi_hdc;
static __thread PAINTSTRUCT swi_ps;

LRESULT swi_call_default(void) {
   	return DefWindowProc(swi_msg.hwnd, swi_msg.message, swi_msg.wParam, swi_msg.lParam);
}

static LRESULT CALLBACK SwiWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		default: {
			if (swi_user) {
				swi_msg.hwnd = hwnd;
				swi_msg.message = Message;
				swi_msg.wParam = wParam;
				swi_msg.lParam = lParam;
				return swi_user();
			} else {
				return DefWindowProc(hwnd, Message, wParam, lParam);
			}
		}
	}
	return 0;
}

__attribute__((constructor)) void register_class(void) {
	WNDCLASSEX wc;
	
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = SwiWndProc;
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

void swi_setGL2D(int w, int h) {
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

HWND swi_combobox(HWND par) {
    return CreateWindowEx(0, "ComboBox", "", WS_CHILD|WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 200, 200, par, 0, 0, 0);
}

HWND swi_listbox(HWND par) {
    return CreateWindowEx(0, "ListBox", "", WS_CHILD|WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 200, 200, par, 0, 0, 0);
}

HWND swi_static(HWND par) {
    return CreateWindowEx(0, "Static", "", WS_CHILD|WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 200, 200, par, 0, 0, 0);
}

HWND swi_createControl(HWND par, const char *class) {
	return CreateWindowEx(0, class, "", WS_CHILD|WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 200, 200, par, 0, 0, 0);
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


static LOGFONT swi_def_font = {
    .lfCharSet = DEFAULT_CHARSET,
    .lfHeight = 14,
    .lfWeight = 500,
    .lfFaceName = "Consolas"
};

static LOGPEN swi_def_pen = {
    .lopnStyle = PS_SOLID,
    .lopnWidth = 1,
    .lopnColor = RGB(0, 0, 0)
};

static LOGBRUSH swi_def_brush = {
	.lbStyle = BS_SOLID,
	.lbColor = RGB(0, 0, 0),	
};
LOGFONT *swi_aGetFont(HWND h) {
	HFONT hfont = (HFONT)SendMessage(h, WM_GETFONT, 0, 0);
	LOGFONT *l = calloc(sizeof(*l), 1);
	if (hfont == NULL || !GetObject(hfont, sizeof(*l), l)) {
		memcpy(l, &swi_def_font, sizeof(l));
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

LOGPEN *swi_aGetPen(HWND h) {
	HDC hdc = GetDC(h);
	LOGPEN *l = malloc(sizeof(*l));
	
	if (!GetObject(hdc, sizeof(*l), l)) {
		memcpy(l, &swi_def_pen, sizeof(*l));
	}
	return l;
}

void swi_setPen(HWND h, LOGPEN *l) {
	HDC hdc = GetDC(h);
	HPEN hh = CreatePenIndirect(l);
	HPEN hold = SelectObject(hdc, hh);
	DeleteObject((HGDIOBJ)hold);
	ReleaseDC(h, hdc);
}

LOGBRUSH *swi_aGetBrush(HWND h) {
	HDC hdc = GetDC(h);
	LOGBRUSH *l = malloc(sizeof(*l));
	
	if (!GetObject(hdc, sizeof(*l), l)) {
		memcpy(l, &swi_def_brush, sizeof(*l));
	}
	return l;
}


void swi_setBrush(HWND h, LOGBRUSH *l) {
	HDC hdc = GetDC(h);
	HBRUSH hh = CreateBrushIndirect(l);
	HBRUSH hold = SelectObject(hdc, hh);
	DeleteObject((HGDIOBJ)hold);
	ReleaseDC(h, hdc);
}

void swi_setStroke(HWND h, int r, int g, int b) {
    LOGPEN *l = swi_aGetPen(h);
	l->lopnColor = RGB(r, g, b);
	swi_setPen(h, l);
	free(l);	
}

void swi_setFill(HWND h, int r, int g, int b) {
    LOGBRUSH *l = swi_aGetBrush(h);
	l->lbColor = RGB(r, g, b);
	swi_setBrush(h, l);
	free(l);
}


int swi_peek(MSG *msg) {
	return PeekMessage(msg,  0, 0, 0, PM_NOREMOVE);
}

int swi_getMessage(MSG *msg) {
	return GetMessage(msg,  0, 0, 0);
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

void swi_eventloop(void) {
	while (1) {
		MSG msg;
		if (swi_peek(&msg) && swi_getMessage(&msg)) {
			if (swi_isevent(&msg, 0, WM_QUIT)) {
				break;
			}
			swi_process(&msg);
		} else {
			Sleep(10);
		}
	}	
}

#define swi_thread_var __thread
typedef LRESULT swi_result;
#if !defined(SWI_NO_SHORTNAMES)
	#if defined(SWI_STATEFUL_MODEL)
		#define STATEFUL(...)  __VA_ARGS__
		#define STATELESS(...)
	#else
		#define STATEFUL(...) 
		#define STATELESS(...) __VA_ARGS__
	#endif
	
	#if defined(SWI_STATEFUL_MODEL)

		
		
		#define AT(a)               do {swi_current = a;} while(0)
	    #define ATWINDOW()          do {swi_current = swi_current_window;} while(0)
	    #define to_parent()         do {swi_current = GetParent(swi_current); } while(0)
	    
	    
	
	
	    #define window()            (swi_current = swi_current_window = swi_window())
	    #define openglwindow()      (swi_current = swi_current_window = swi_openglwindow())
	    #define button()            (swi_current = swi_button(swi_current_window))
	    #define textbox()           (swi_current = swi_textbox(swi_current_window))
	    #define listbox()           (swi_current = swi_listbox(swi_current_window))
	    #define combobox()          (swi_current = swi_combobox(swi_current_window))
	    #define label()             (swi_current = swi_static(swi_current_window))
	    #define createControl(w)    (swi_current = swi_createControl(w, swi_currentwindow))
	    
	    #define setText(t)          (swi_setText(swi_current, t))
	    #define setPos(a, b)        (swi_setPos(swi_current, a, b))
	    #define setSize(a, b)       (swi_setSize(swi_current, a, b))
	    #define setFont(t)          (swi_setFont(swi_current, t))
	    #define setVisible(t)       (swi_setVisible(swi_current, t))
	    #define setStyle(t)         (swi_setStyle(swi_current, t))
	    #define modifyStyle(a, b)   (swi_modifyStyle(swi_current, a, b))
	    #define setExStyle(t)       (swi_setExStyle(swi_current, t))
	    
	    #define peek()              (swi_peek(&swi_msg))
	    #define poll()              (swi_poll(&swi_msg))
	    #define getmsg()            (swi_getMessage(&swi_msg))
	    #define process()           (swi_process(&swi_msg))
	    
	    #define isevent(b, c)       (swi_isevent(&swi_msg, b, c))
	    #define event(c)            (swi_isevent(&swi_msg, swi_current, c))
	    #define event_target()      (swi_msg.hwnd)
	    #define event_type()        (swi_msg.message)
	    
	    #define to_paint()          (swi_hdc = BeginPaint(swi_current, &swi_ps))
	    #define end_paint()         (EndPaint(swi_current, &swi_ps))
	    #define to_dc()             (swi_hdc = GetDC(swi_current))
	    #define release_dc()        (ReleaseDC(swi_current, swi_hdc))
	    
	    #define setPen(b)           (swi_setPen(swi_current, b))
	    #define setBrush(b)         (swi_setBrush(swi_current, b))
	    #define setStroke(r, g, b)  (swi_setStroke(swi_current, r, g, b))
	    #define setFill(r, g, b)    (swi_setFill(swi_current, r, g, b))
	    
	    #define to_msg_target()     (swi_current = swi_msg.hwnd)
	    
	    
	    
	    
    
	#else 	
	    #define window swi_window
	    #define openglwindow swi_openglwindow
	    
	    #define button swi_button
	    #define textbox swi_textbox
	    #define listbox swi_listbox
	    #define combobox swi_combobox
	    #define label swi_static
	    #define createControl swi_createControl
	    
	    
	    #define setText swi_setText
	    #define setPos swi_setPos
	    #define setSize swi_setSize
	    #define setVisible swi_setVisible
	    #define setFont swi_setFont
	    #define setPen swi_setPen
	    #define setBrush swi_setBrush
	    #define setStroke   swi_setStroke
	    #define setFill     swi_setFill
	    
	    #define setStyle swi_setStyle
	    #define modifyStyle swi_modifyStyle
	    #define setExStyle swi_setExStyle
	    
	    #define modifyExStyle swi_modifyExStyle
	    
	    
	    #define isevent swi_isevent
	    #define process swi_process
	    
	    #define peek  swi_peek
	    #define poll  swi_poll
	    #define getmsg  swi_getMessage
	    #define isevent swi_isevent
    
	#endif
	
	
    #define aGetFont swi_aGetFont
    #define aGetText swi_aGetText
    #define aGetPen swi_aGetPen
	#define aGetBrush swi_aGetBrush
    #define getExStyle swi_getExStyle
	#define getStyle swi_getStyle
    #define asprintf swi_asprintf
    #define var __auto_type
    #define swapBuffers swi_swapBuffers
    
    #define setGL2D swi_setGL2D
    #define mainLoop swi_mainLoop
    #define mainWindowLoop swi_mainWindowLoop
    #define eventloop swi_eventloop
    #define setUserHandler(a)  (swi_user = a)
	    
    #define getMouseXY(x, y)          (x = GET_X_LPARAM(swi_msg.lParam), y = GET_Y_LPARAM(swi_msg.lParam))
#endif

#endif
