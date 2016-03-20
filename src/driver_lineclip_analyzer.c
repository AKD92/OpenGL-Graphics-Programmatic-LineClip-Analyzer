

/************************************************************************************
	Midpoint Line Drawing Algorithm driver C file (MAIN)
	Author:             Ashis Kumar Das
	ID:                 11301002
	Email:              akd.bracu@gmail.com
*************************************************************************************/



#include <time.h>
#include <list.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iup.h>                /* For GUI toolkit */
#include <iupgl.h>


#define BTN_CLOSE "btnClose"
#define GL_CANVAS "GLCanvas"
#define MAIN_WINDOW "423Homework05MainWindow"



/************************************************************************************
*
*   Function Prototypes and Variable Definitions
*
*************************************************************************************/

List pointList;

char *labTitle = "Graphics Homework 05 (11301002)\n"
					"Programmatic LineClipping Analyzer";





int main(void);

void clipal_drawPixel(float x, float y);

int util_readFromFile(const char *fileName);

void util_generateRandomFloat(float lb, float ub, unsigned int len, List *outList);

/*	GUI Toolkit related functions	*/

Ihandle * createMainWindow(void);
int repaint_cb(Ihandle *canvas);
int btnRandLine_cb(Ihandle *btn);
int btnClearLines_cb(Ihandle *btn);
int exit_cb(Ihandle *btn);



/*	OpenGL related functions	*/

void clipal_drawClipRectangle(void);
void mygl_drawGenericLine(float x0, float y0, float x1, float y1);






/************************************************************************************
*
*   Implementation of midpoint driver routines
*
*************************************************************************************/


Ihandle * createMainWindow(void) {

	Ihandle *bottomHbox, *mainVbox;
	Ihandle *lblTitle, *glCanvas;
	Ihandle *btnRandomLine, *btnClearLines, *btnClose;
	Ihandle *dialog;

	lblTitle = IupLabel(labTitle);
	IupSetAttribute(lblTitle, "EXPAND", "HORIZONTAL");
	IupSetAttribute(lblTitle, "ALIGNMENT", "ALEFT:ACENTER");
	IupSetAttribute(lblTitle, "FONTSIZE", "10");

	glCanvas = IupGLCanvas(0);
	IupSetAttribute(glCanvas, "EXPAND", "YES");
	IupSetAttribute(glCanvas, "BORDER", "NO");
	IupSetAttribute(glCanvas, "CANFOCUS", "NO");
	IupSetCallback(glCanvas, "ACTION", (Icallback) repaint_cb);

	btnRandomLine = IupButton("Generate Random Lines", 0);
	IupSetAttribute(btnRandomLine, "RASTERSIZE", "150x33");
	IupSetCallback(btnRandomLine, "ACTION", (Icallback) btnRandLine_cb);
	
	btnClearLines = IupButton("Clear All Lines", 0);
	IupSetAttribute(btnClearLines, "RASTERSIZE", "100x33");
	IupSetCallback(btnClearLines, "ACTION", (Icallback) btnClearLines_cb);
	
	btnClose = IupButton("Close", 0);
	IupSetAttribute(btnClose, "RASTERSIZE", "80x33");
	IupSetCallback(btnClose, "ACTION", (Icallback) exit_cb);

	bottomHbox = IupHbox(btnRandomLine, btnClearLines, IupFill(), btnClose, 0);
	IupSetAttribute(bottomHbox, "EXPAND", "HORIZONTAL");
	IupSetAttribute(bottomHbox, "NGAP", "5");
	IupSetAttribute(bottomHbox, "NMARGIN", "0x5");

	mainVbox = IupVbox(lblTitle, glCanvas, bottomHbox, 0);
	IupSetAttribute(mainVbox, "NMARGIN", "10x10");
	IupSetAttribute(mainVbox, "NGAP", "10");
	
	IupSetHandle(BTN_CLOSE, btnClose);
	IupSetHandle(GL_CANVAS, glCanvas);

	dialog = IupDialog(mainVbox);
	IupSetAttribute(dialog, "TITLE", "GL Labwork");
	IupSetAttribute(dialog, "RASTERSIZE", "425x550");
	IupSetAttribute(dialog, "SHRINK", "YES");
	IupSetAttribute(dialog, "DEFAULTESC", BTN_CLOSE);
	
	IupSetHandle(MAIN_WINDOW, dialog);

	return dialog;
}


//int resize_cb(Ihandle *canvas, int newWidth, int newHeight) {
//
//	IupGLMakeCurrent(canvas);
//
//	glViewport(0, 0, (GLsizei) newWidth, (GLsizei) newHeight);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluOrtho2D(0.0, (GLdouble) newWidth/5, 0.0, (GLdouble) newHeight/5);
//
//	return IUP_DEFAULT;
//}


int repaint_cb(Ihandle *canvas) {

	register ListElem *elem;
	register float *xy, *xy_2;

//	if (IupGLIsCurrent(canvas) == 0) {
		IupGLMakeCurrent(canvas);                   /* Declare our canvas for OpenGL */
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glPointSize(1.0);
//	}
	glClear(GL_COLOR_BUFFER_BIT);

	elem = list_head(&pointList);
	glBegin(GL_POINTS);

		while (elem != 0) {                         /* Draw all the lines from linked list */

			xy = (float*) list_data(elem);          /* One endpoint of the line being drawn */
			elem = list_next(elem);
			xy_2 = (float*) list_data(elem);        /* Another endpoint of the same line */
			
			mygl_drawGenericLine(*xy, *(xy + 1), *xy_2, *(xy_2 + 1));
			elem = list_next(elem);
		}
		
		clipal_drawClipRectangle();                 /* Draw clip rectangle on the canvas */

	glEnd();

	glFlush();

	return IUP_DEFAULT;
}


int btnRandLine_cb(Ihandle *btn) {
	
	Ihandle *canvas;
	void (*destroy_routine) (void *data);
	
	canvas = IupGetHandle(GL_CANVAS);
	destroy_routine = pointList.destroy;
	
	list_destroy(&pointList);
	list_init(&pointList, destroy_routine);

	util_generateRandomFloat(-1.0, 1.0, 30, &pointList);
	IupRedraw(canvas, 0);
//	IupUpdate(canvas);
	
	printf("%d random floats generated\n", list_size(&pointList));
	
	return IUP_DEFAULT;
}


int btnClearLines_cb(Ihandle *btn) {
	
	Ihandle *canvas;
	void (*destroy_routine) (void *data);
	
	canvas = IupGetHandle(GL_CANVAS);
	destroy_routine = pointList.destroy;
	
	list_destroy(&pointList);
	list_init(&pointList, destroy_routine);
	
	IupRedraw(canvas, 0);
	
	return IUP_DEFAULT;
}


int exit_cb(Ihandle *btn) {

	return IUP_CLOSE;
}


int main(void) {

	Ihandle *dlg;
	int res;
	extern void (*routine_drawPixel) (float x, float y);

	srand((unsigned int) time(0));                  /* Seed to current time value */
	
	routine_drawPixel = clipal_drawPixel;

	list_init(&pointList, free);
	res = util_readFromFile("clip_rectangle.txt");
	printf("Clip Rectangle loaded, return value: %d\n", res);
	
	if (res == -1) {
		printf("File read error\n");\
		getche();
		goto TERMINATE;
	}

	IupOpen(0, 0);
		IupGLCanvasOpen();

		dlg = createMainWindow();
		IupShowXY(dlg, IUP_CENTER, IUP_CENTER);

		IupMainLoop();
	IupClose();
	
	TERMINATE:
	list_destroy(&pointList);
	return 0;
}
