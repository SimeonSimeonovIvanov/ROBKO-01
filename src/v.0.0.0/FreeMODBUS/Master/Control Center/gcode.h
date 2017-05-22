#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "llist.h"

typedef enum {
	G01MOVE,
	G05MOVE
} MOVE_TYPE;

union union_move;

typedef struct {
	MOVE_TYPE type;
	union union_move *next;
	bool New;
	double x0, y0, z0; // initial position
	double xf, yf, zf; // final position
	double V;          // feedrate
	double t0;
	double tf;

	int n;             // degree
	double sigma[10];  // coefficients [meters]
	double S[10];
	double X[10];
	double Y[10];
	double Z[10];
} g05move;

typedef struct {
	MOVE_TYPE type;
	union union_move *next;
	bool New;
	double x0, y0, z0; // initial position
	double xf, yf, zf; // final position
	double V;          // feedrate
	double t0;
	double tf;
} g01move;

typedef union union_move {
	MOVE_TYPE type;
	g01move g01;
	g05move g05;
} move;

typedef struct gcode_struct {
	char axesFlag;
	int G;				// move type: 1=linear 5=ph

	double X, Y, Z;		// final position
	float V, W;

	//double X0, Y0, Z0;	// initial position
	double A, B, C;		// PH curve U coefficients
	double P, Q, R;		// PH curve V coefficients
	double F, U;		// feedrate
	int H;				// PH curve degree
} gcode;

//void interpolateG05(double t, move *mv, coord *pos);

//void interpolateG01(double t, move *mv, coord *pos);

int gcode_loadstr( char *gcode_str, LPLIST *lpRootGProgramList );

gcode *gcode_load(char *filename);

gcode *gcode_new( void );

//move *convert_g01code(gcode *gc);
//move *convert_g05code(gcode *gc);

move *convert_gcodes(gcode *gc);

//int gcode_inputgc(double t, coord *pos);

void setup();
