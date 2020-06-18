//
//	$Revision: 1.1.1.1 $
//	$Date: 2001/09/01 02:53:28 $
//
//	ベクトルの計算とか線分の交差判定とかのライブラリ
//
#include <stdio.h>
#include <math.h>
//#include <GL/glut.h>
#include <GLUT/glut.h>

#include "vector.h"

const Vector zerovector  = {0.0, 0.0};

Vector
addvector(Vector v1, Vector v2)
{
	Vector v;
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	return v;
}

Vector
subvector(Vector v1, Vector v2)
{
	Vector v;
	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	return v;
}

GLfloat
oproduct(Vector v1, Vector v2) // 外積
{
	return v1.x * v2.y - v1.y * v2.x;
}

GLfloat
iproduct(Vector v1, Vector v2) // 内積
{
	return v1.x * v2.x + v1.y * v2.y;
}

int
sign(GLfloat v)
{
	return v > 0.0 ? 1 : v < 0.0 ? -1 : 0;
}

GLfloat
length(Vector v)
{
//#ifndef FHYPOT
//	return sqrt(v.x*v.x+v.y*v.y);
//#else
//	return fhypot(v.x,v.y);
	return (GLfloat)hypot(v.x,v.y);
//#endif
}

GLfloat
vcos(Vector v1, Vector v2)
{
	return iproduct(v1,v2) / length(v1) / length(v2);
}

GLfloat
vsin(Vector v1, Vector v2)
{
	return oproduct(v1,v2) / length(v1) / length(v2);
}

void
calcframe(PolyGon *p)
{
	int i;
	p->left = p->right = p->v[0].x;
	p->top = p->bottom = p->v[0].y;
	for(i=1;i<p->n;i++){
		if(p->v[i].x > p->right) p->right = p->v[i].x;
		if(p->v[i].x < p->left) p->left = p->v[i].x;
		if(p->v[i].y > p->top) p->top = p->v[i].y;
		if(p->v[i].y < p->bottom) p->bottom = p->v[i].y;
	}
}

void
drawpolygon(PolyGon *p)
{
	int i;
	GLfloat v[2];
//	glColor3f(p.r,p.g,p.b);
	glColor4f(p->r,p->g,p->b,p->a);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
	glBegin(GL_POLYGON);
	for(i=0;i< p->n;i++){
		v[0] = p->v[i].x;
		v[1] = p->v[i].y;
		glVertex2fv(v);
	}
	glEnd();

	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINE_LOOP);
	for(i=0;i< p->n;i++){
		v[0] = p->v[i].x;
		v[1] = p->v[i].y;
		glVertex2fv(v);
	}
	glEnd();
}

int
insidepolygon(PolyGon p, Vector v)
//
//	点vが凸ポリゴンpに含まれるかどうか
//
{
	int i;
	int j;
	Vector v1,v2;
	int s = 0;
	for(i=0;i<p.n;i++){
		v1 = subvector(v,p.v[i]);
		v2 = subvector(p.v[(i+1)%p.n],p.v[i]);
		j = sign(oproduct(v1,v2));
		s += j;
	}
	return s == p.n || s == -p.n;
}

static GLfloat
D(GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
	return a * d - b * c;
}

#define X1 l1.v1.x
#define X2 l1.v2.x
#define X3 l2.v1.x
#define X4 l2.v2.x
#define Y1 l1.v1.y
#define Y2 l1.v2.y
#define Y3 l2.v1.y
#define Y4 l2.v2.y

int
intersectLL(Line l1, Line l2) // p73
{
	GLfloat d,dl,dm;
	GLfloat l,m;
	d = D(X2-X1,Y2-Y1,X3-X4,Y3-Y4);
	if(d == 0.0) return 0; // ????
	dl = D(X3-X1,Y3-Y1,X3-X4,Y3-Y4);
	dm = D(X2-X1,Y2-Y1,X3-X1,Y3-Y1);
	l = dl/d;
	m = dm / d;
	return l > 0.0 && l < 1.0 && m > 0.0 && m < 1.0;
}

int
intersectPP(PolyGon p1, PolyGon p2)
//
//	p1とp2が重なっているかどうか
//
{
/*	
//
//	各点が他ポリゴンの内部にあるかの判定
//	これだと判定できない場合がある！
//
	int i;
	for(i=0;i<p1.n;i++){
		if(insidepolygon(p2,p1.v[i])) return 1;
	}
	for(i=0;i<p2.n;i++){
		if(insidepolygon(p1,p2.v[i])) return 1;
	}
	return 0;
*/
//
//	線分が重なるかどうかの判定
//
	int i1,i2;
	int n1,n2;
	Line l1,l2;
	for(i1=0;i1<p1.n;i1++){
		n1 = (i1+1) % p1.n;
		l1.v1 = p1.v[i1];
		l1.v2 = p1.v[n1];
		for(i2=0;i2<p2.n;i2++){
			n2 = (i2+1) % p2.n;
			l2.v1 = p2.v[i2];
			l2.v2 = p2.v[n2];
			if(intersectLL(l1,l2)) return 1;
		}
	}
	return 0;
}

GLfloat
distVV(Vector v1, Vector v2) // 点v1と点v2の距離
{
	return length(subvector(v1,v2));
}

GLfloat
distVL(Vector v, Line l) // vとlの距離
{
	GLfloat s; // 面積
	if(l.v1.x == l.v2.x && l.v1.y == l.v2.y){
		return distVV(v,l.v1);
	}
	s = oproduct(subvector(l.v1,v),subvector(l.v2,v));
	s *= sign(s); // 正の数にする
	return s / length(subvector(l.v1,l.v2));
}
