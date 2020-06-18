//
//	$Revision: 1.3 $
//	$Date: 2001/09/01 11:45:23 $
//
//
#ifndef _VECTOR_H_
#define _VECTOR_H_

//#include <GL/glut.h>
#include <GLUT/glut.h>

typedef int Boolean;

#define false 0
#define true 1

typedef struct {
	GLfloat x;
	GLfloat y;
} Vector;

typedef struct {
	Vector v1;	// $B;OE@(B
	Vector v2;	// $B=*E@(B
} Line;

typedef struct {
	int n;
	Vector v[20];
	GLfloat r,g,b,a;
	GLfloat top,bottom,right,left; // $B%]%j%4%s$r0O$`6k7A(B
	Boolean snap[20];

	Boolean anchor;		// $B%"%s%+!<%]%j%4%s$+$I$&$+(B
	Boolean anchored;	// $BJL$N%]%j%4%s$K4XO"$E$1$i$l$F$$$k(B
	Boolean anchoredx;	// $BA`:nA0$KJL$N%]%j%4%s$K4XO"$E$1$i$l$F$$$?$+(B
	int anchorvertex;	// $B%"%s%+!<D:E@HV9f(B
	Vector vorig;
} PolyGon;

extern const Vector zerovector;

Vector addvector(Vector v1, Vector v2);
Vector subvector(Vector v1, Vector v2);
GLfloat oproduct(Vector v1, Vector v2); // $B30@Q(B
GLfloat iproduct(Vector v1, Vector v2); // $BFb@Q(B
int sign(GLfloat v);
GLfloat length(Vector v);
GLfloat vcos(Vector v1, Vector v2);
GLfloat vsin(Vector v1, Vector v2);
void calcframe(PolyGon *p);
void drawpolygon(PolyGon *p);
int insidepolygon(PolyGon p, Vector v);
int intersectLL(Line l1, Line l2); // p73
int intersectPP(PolyGon p1, PolyGon p2);
GLfloat distVV(Vector v1, Vector v2); // $BE@(Bv1$B$HE@(Bv2$B$N5wN%(B
GLfloat distVL(Vector v, Line l); // v$B$H(Bl$B$N5wN%(B

#endif
