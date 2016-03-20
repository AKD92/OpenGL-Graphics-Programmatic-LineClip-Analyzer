


#include <GL\gl.h>
#include <GL\glu.h>





float xmin, xmax, ymin, ymax;
float insideR, insideG, insideB;
float outsideR, outsideG, outsideB;




void clipal_drawClipRectangle(void);

void clipal_drawPixel(float x, float y);

void mygl_drawGenericLine(float x0, float y0, float x1, float y1);




void clipal_drawClipRectangle(void) {
	
	// draw bottom line
	mygl_drawGenericLine(xmin, ymin, xmax, ymin);
	
	// draw top line
	mygl_drawGenericLine(xmin, ymax, xmax, ymax);
	
	// draw left line
	mygl_drawGenericLine(xmin, ymin, xmin, ymax);
	
	// draw rght line
	mygl_drawGenericLine(xmax, ymin, xmax, ymax);
	
	return;
}


void clipal_drawPixel(float x, float y) {
	
	unsigned int inside_x, inside_y, insideFull;
	
	inside_x = x >= xmin && x <= xmax;      /* check if x falls between xmin and xmax */
	inside_y = y >= ymin && y <= ymax;      /* check if y falls between ymin and ymax */
	insideFull = inside_x & inside_y;       /* check if both (x, y) falls inside clip */
	
	if (insideFull != 0) {
		glColor3f(insideR, insideG, insideB);   	/* Color GREEN */
	}
	else {
		glColor3f(outsideR, outsideG, outsideB);    /* Color RED */
	}
	
	glVertex2f(x, y);
	
	return;
}
