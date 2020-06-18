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
	Vector v1;	// 始点
	Vector v2;	// 終点
} Line;

typedef struct {
	int n;
	Vector v[20];
	GLfloat r,g,b,a;
	GLfloat top,bottom,right,left; // ポリゴンを囲む矩形
	Boolean snap[20];

	Boolean anchor;		// アンカーポリゴンかどうか
	Boolean anchored;	// 別のポリゴンに関連づけられている
	Boolean anchoredx;	// 操作前に別のポリゴンに関連づけられていたか
	int anchorvertex;	// アンカー頂点番号
	Vector vorig;
} PolyGon;

extern const Vector zerovector;

Vector addvector(Vector v1, Vector v2);
Vector subvector(Vector v1, Vector v2);
GLfloat oproduct(Vector v1, Vector v2); // 外積
GLfloat iproduct(Vector v1, Vector v2); // 内積
int sign(GLfloat v);
GLfloat length(Vector v);
GLfloat vcos(Vector v1, Vector v2);
GLfloat vsin(Vector v1, Vector v2);
void calcframe(PolyGon *p);
void drawpolygon(PolyGon *p);
int insidepolygon(PolyGon p, Vector v);
int intersectLL(Line l1, Line l2); // p73
int intersectPP(PolyGon p1, PolyGon p2);
GLfloat distVV(Vector v1, Vector v2); // 点v1と点v2の距離
GLfloat distVL(Vector v, Line l); // vとlの距離

#endif
