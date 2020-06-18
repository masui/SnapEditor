//
//	$Revision: 1.4 $
//	$Date: 2001/09/01 11:45:23 $
//

//
//	$BE,1~E*%9%J%C%T%s%0IA2h$N?75!<4(B
//
//	$B!z(B $BF0$+$7$?5wN%$K$h$j%9%J%C%T%s%0$,JQ$o$k(B
//	   $B!&$"$kDxEY0J>e0\F0$9$k$H%9%J%C%T%s%0(B
//	   $B!&$"$kDxEY0J>e2sE>$9$k$H%9%J%C%T%s%0(B
//	   $B!&1s$/$K0\F0$7$?>l9g$O$=$l$J$j$NBg$-$5$N%0%j%C%I$K%9%J%C%T%s%0(B
//	   $B!&2?$b$J$$$H$3$m$G2sE>$9$k$H(B0$BEY!"(B90$BEY$J$I$G%9%J%C%T%s%0(B
//	   $B!&3HBgA`:n$bF1MM$K$9$k(B
//	
//	$B!z(B $B<~0O$H$N4X78$K$h$j%9%J%C%T%s%0$,JQ$o$k(B
//	   $B!&2?$+$K$V$D$+$k$H(BSticky$B$K%9%J%C%T%s%0(B
//	

#include <stdio.h>
#include <math.h>
//#include <GL/glu.h>
//#include <GL/glut.h>
#include <GLUT/glut.h>

#include "vector.h"

void regpolygon(PolyGon *p);

PolyGon *polygons[100];
int npolygons = 0;

Vector origmouse;	// $B%^%&%9$r%/%j%C%/$7$?$H$-$N0LCV(B
Vector curmouse;	// $B8=:_$N%^%&%90LCV(B
Vector prevmouse;	// $B$R$H$DA0$N%^%&%90LCV(B

int selpolygon;		// $BA*Br$5$l$?%]%j%4%s$NHV9f(B
int rotvertex;		// $B2sE>$9$kD:E@$NHV9f(B
int centerindex;	// $B2sE>$NCf?4$H$J$kD:E@$NHV9f(B
Vector center;		// $B2sE>$NCf?4:BI8(B
Vector prevdir;		// $B2sE>$N;2>H$H$J$kJ}8~(B
PolyGon pprev;		// $BD>A0$N(Bvalid$B$J%]%j%4%s%G!<%?(B
PolyGon porig;		// $B%^%&%9$r%/%j%C%/$7$?$H$-$N%]%j%4%s%G!<%?(B

int enlargeedge;	// $B3HBg$9$kJU(B

GLfloat dist;		// $B%^%&%9$r2!$7$F$+$i$N0\F05wN%(B
GLfloat collisiondist;	// $B$V$D$+$C$F$+$i$N0\F05wN%(B
GLfloat gridsizex = 0.0;	// $B%0%j%C%I%5%$%:(B
GLfloat gridsizey = 0.0;	// $B%0%j%C%I%5%$%:(B
GLfloat gridx = 80.0;
GLfloat gridy = 80.0;
int collision=0;	// $B%*%V%8%'%/%H$,$V$D$+$C$F$$$k$+$I$&$+(B
GLfloat gridorigx = 0.0, gridorigy = 0.0;

int pickpolygon;	// $B%\%?%s$r2!$5$:$K%^%&%9$rF0$+$7$F$$$k$H$-$N%]%j%4%s>pJs(B
int pickvertex;
int pickcenterindex;

double totalangle;

Boolean passive = true;	// $B%^%&%9$r2!$5$:$K%I%i%C%0Cf$+$I$&$+(B

Boolean snapped = false;

Boolean snappedtoobj = false;

PolyGon *ap;		// $B%"%s%+!<%]%j%4%s(B
PolyGon *newap;		// $B?7$7$$%"%s%+!<%]%j%4%s8uJd(B

/////////////////////////////////////////////////////////////////////////
Vector polygonpos;
//int anchorvertex;
Boolean dolog = true;

typedef void EditFunc(int,int);

typedef struct {
	EditFunc *editfunc;
	int arg1;
	int arg2;
} Command;

Command history[100];

void addcommand(EditFunc *editfunc, int arg1, int arg2)
{
	int i;
	if(!dolog) return;
	for(i=100-2;i>=0;i--){
		history[i+1] = history[i];
	}
	history[0].editfunc = editfunc;
	history[0].arg1 = arg1;
	history[0].arg2 = arg2;
}

void Select(int n, int dummy)
{
	printf("Select(%d)\n",n);
	selpolygon = n;
	if(selpolygon >= 0){
		polygonpos = polygons[selpolygon]->v[0];
	}
	addcommand(Select,n,dummy);
}

void Copy(int dummy1, int dummy2)
{
	addcommand(Copy,dummy1,dummy2);
}

void Paste(int dummy1, int dummy2)
{
	int i;
	PolyGon *p;
	printf("Paste()\n");
	p = (PolyGon*)malloc(sizeof(PolyGon));
	if(selpolygon >= 0){
		*p = *(polygons[selpolygon]);
		for(i=0;i<p->n;i++){
			p->snap[i] = false;
		}
		p->anchor = false;
		p->anchored = false;
		p->anchoredx = false;
			
		regpolygon(p);
	}

	selpolygon = npolygons-1;
	// $BA*$s$@%]%j%4%s$,@hF,$KMh$k$h$&$KJB$Y$+$($k(B
	p = polygons[selpolygon];
	for(i=selpolygon-1;i>=0;i--){
		polygons[i+1] = polygons[i];
	}
	polygons[0] = p;
	selpolygon = 0;

	addcommand(Paste,dummy1,dummy2);
}

void Delete(int polygon, int dummy)
{
	int i;
	for(i=polygon;i<npolygons;i++){
		polygons[i] = polygons[i+1];
	}
	if(npolygons > 0)npolygons--;
	addcommand(Delete,polygon,dummy);
}

void MoveRelative(int x, int y)
{
	int i;
	Vector newpos;
	Vector relmove;
	PolyGon *p;
	Vector v;

	printf("MoveRelative(%d,%d)\n",x,y);
	v.x = (GLfloat)x;
	v.y = (GLfloat)y;
	if(selpolygon >= 0){
		p = polygons[selpolygon];
		newpos = addvector(polygonpos,v);
		relmove = subvector(newpos,p->v[0]);
		for(i=0;i<p->n;i++){
			p->v[i] = addvector(p->v[i],relmove);
		}

		polygonpos = polygons[selpolygon]->v[0];

		for(i=0;i<p->n;i++){
			p->snap[i] = 0;
		}
		//p->snap[anchorvertex] = 1;
		p->snap[p->anchorvertex] = 1;

		p->anchored = true;
		p->anchoredx = true;

		p->vorig = p->v[p->anchorvertex];
	}
	addcommand(MoveRelative,x,y);
}

void MoveAbsolute(int x, int y)
{
	int i;
	Vector newpos;
	Vector relmove;
	PolyGon *p;
	Vector v;

	printf("MoveAbsolute(%d,%d)\n",x,y);
	v.x = (GLfloat)x;
	v.y = (GLfloat)y;
	if(selpolygon >= 0){
		p = polygons[selpolygon];
		relmove = subvector(v,p->v[0]);
		for(i=0;i<p->n;i++){
			p->v[i] = addvector(p->v[i],relmove);
		}
		p->anchored = false;

		p->vorig = p->v[p->anchorvertex];
	}
	addcommand(MoveAbsolute,x,y);
}

void SetAnchorVertex(int vertex, int dummy)
{
	PolyGon *p;
	printf("SetAnchorVertex(%d,%d)\n",vertex,dummy);
	if(selpolygon >= 0){
		p = polygons[selpolygon];
		//		p->anchorvertex = vertex;
		p->vorig = p->v[p->anchorvertex];
		//		p->anchoredx = p->anchored;
	}
	addcommand(SetAnchorVertex,vertex,dummy);
}


/////////////////////////////////////////////////////////////////////////

void display(void)
{
	int i,j,k;
	GLfloat width,height;
	GLfloat x,y;
	static int count = 0;
	PolyGon *p;
	int nanchored;

	height = (GLfloat)glutGet(GLUT_WINDOW_HEIGHT);
	width = (GLfloat)glutGet(GLUT_WINDOW_WIDTH);

	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(gridsizex != 0.0 && !collision){
		glColor3f(0.7,0.7,1.0);
		glBegin(GL_LINES);
		for(x = gridorigx;x<=width;x+=gridsizex){
			glVertex2f(x,0.0);
			glVertex2f(x,height);
		}
		for(y = gridorigy;y<=height;y+=gridsizey){
			glVertex2f(0.0,y);
			glVertex2f(width,y);
		}
		glEnd();
	}

	for(i=npolygons-1;i>=0;i--){
		p = polygons[i];
		if(i == pickpolygon && pickvertex >= 0 && passive){
			GLfloat c,s;
			PolyGon q;

			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

			q = *p;
			c = 1.0;
//			printf("count=%d\n",count);
			s = (count > 0 ? 0.08 : -0.08);
			s = 0.08;
			for(j=0;j<q.n;j++){
				GLfloat dx,dy;
/*
				if(j == pickcenterindex) continue;
				dx = q.v[j].x - q.v[pickcenterindex].x;
				dy = q.v[j].y - q.v[pickcenterindex].y;
				q.v[j].x = q.v[pickcenterindex].x + c * dx - s * dy;
				q.v[j].y = q.v[pickcenterindex].y + s * dx + c * dy;
*/
				dx = q.v[j].x - center.x;
				dy = q.v[j].y - center.y;
				q.v[j].x = center.x + c * dx - s * dy;
				q.v[j].y = center.y + s * dx + c * dy;
			}
			q.r = 0.8;
			q.g = q.b = 0.5;
			q.a = 0.5;
			drawpolygon(&q);

			q = *p;
			c = 1.0;
//			printf("count=%d\n",count);
			s = (count > 0 ? -0.08 : 0.08);
			s = -0.08;
			for(j=0;j<q.n;j++){
				GLfloat dx,dy;
/*
				if(j == pickcenterindex) continue;
				dx = q.v[j].x - q.v[pickcenterindex].x;
				dy = q.v[j].y - q.v[pickcenterindex].y;
				q.v[j].x = q.v[pickcenterindex].x + c * dx - s * dy;
				q.v[j].y = q.v[pickcenterindex].y + s * dx + c * dy;
*/
				dx = q.v[j].x - center.x;
				dy = q.v[j].y - center.y;
				q.v[j].x = center.x + c * dx - s * dy;
				q.v[j].y = center.y + s * dx + c * dy;
			}
			q.r = q.g = 0.5;
			q.b = 0.8;
			q.a = 0.5;
			drawpolygon(&q);

			count++;
			if(count >= 30) count = -30;

/*
			x = polygons[pickpolygon].v[pickvertex].x;
			y = polygons[pickpolygon].v[pickvertex].y;
			glColor3f(0.3,0.3,0.3);
			glRectf(x-10.0,y-10.0,x+10.0,y+10.0);
*/
			glBlendFunc(GL_ONE,GL_ZERO);
		}
		drawpolygon(polygons[i]);

		j = p->anchorvertex;
		if(p->anchor){
			x = p->v[j].x;
			y = p->v[j].y;
			glColor3f(0.3,0.3,1.0);
			glRectf(x-10.0,y-10.0,x+10.0,y+10.0);

			glColor3f(0.0,0.0,0.0);
			glBegin(GL_LINE_LOOP);
			glVertex2f(x-10.0, y-10.0);
			glVertex2f(x+10.0, y-10.0);
			glVertex2f(x+10.0, y+10.0);
			glVertex2f(x-10.0, y+10.0);
			glEnd();
		}
		if(p->anchored){
			x = p->v[j].x;
			y = p->v[j].y;
			glColor3f(1.0,1.0,1.0);
			glRectf(x-10.0,y-10.0,x+10.0,y+10.0);

			glLineWidth(0.1);
			glColor3f(0.0,0.0,0.0);
			glBegin(GL_LINE_LOOP);
			glVertex2f(x-10.0, y-10.0);
			glVertex2f(x+10.0, y-10.0);
			glVertex2f(x+10.0, y+10.0);
			glVertex2f(x-10.0, y+10.0);
			glEnd();
		}
	}

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLfloat)w, (GLfloat)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, (GLfloat)w, 0, (GLfloat)h);
	glMatrixMode(GL_MODELVIEW);
}

void idlefunc()
{
/*
	static int i = 0;
	if(shouldsnap){
		i++;
		if(i > 10000){
			shouldsnap = 0;	
			i = 0;
		}
	}
*/
	glutPostRedisplay();
}

//
//	v$B$,$I$N%]%j%4%s$N>e$K$"$k$+(B
//
int onpolygon(Vector v)
{
	int i;
	for(i=0;i<npolygons;i++){
		if(insidepolygon(*polygons[i],v))
			return i;
	}
	return -1;
}

//
//	v$B$,%]%j%4%s(Bp$B$N$I$ND:E@$K6a$$$+(B
//
int nearvertex(Vector v, int p)
{
	int i;
	if(p < 0) return -1;
	for(i=0;i<polygons[p]->n;i++){
		if(distVV(polygons[p]->v[i],v) < 20.0)
			return i;
	}
	return -1;
}

//
//	v$B$,%]%j%4%s(Bp$B$N$I$l$+$NJU$K6a$$$+(B
//
int nearedge(Vector v, int p)
{
	int i;
	Line l;
	if(p < 0) return -1;
	for(i=0;i<polygons[p]->n;i++){
		l.v1 = polygons[p]->v[i];
		l.v2 = polygons[p]->v[(i+1)%(polygons[p]->n)];
//		printf("distVL = %f\n",distVL(v,l));
		if(distVL(v,l) < 10.0)
			return i;
	}
	return -1;
}

//
//	$B%]%j%4%s(Bp$B$ND:E@(Br$B$r2sE>$9$k$H$-$N2sE>$NCf?4D:E@$r5a$a$k(B
//
int rotcenter(int p, int r)
{
	int i;
	int n;

	if(p < 0 || r < 0) return -1;

	return polygons[p]->anchorvertex;

	// $B%9%J%C%W$7$F$$$kD:E@$,$"$l$P$=$l$rA*Br(B
	n = polygons[p]->n;
	for(i=0;i<n;i++){
		if(polygons[p]->snap[i]) return i;
	}
	// $B$J$1$l$PH?BPB&$ND:E@$rA*Br(B
	return (r + n/2) % n;
}

void passivemotionfunc(int x, int y)
{
	int i;
	int nanchored;

	curmouse.x = (GLfloat)x;
	curmouse.y = (GLfloat)(glutGet(GLUT_WINDOW_HEIGHT) - y);

	pickpolygon = onpolygon(curmouse);
	pickvertex = nearvertex(curmouse,pickpolygon);
	pickcenterindex = rotcenter(pickpolygon,pickvertex);

	pickcenterindex = 0;
	if(pickpolygon >= 0){
		pickcenterindex = polygons[pickpolygon]->anchorvertex;
	}

	if(pickpolygon >= 0){
		center = polygons[pickpolygon]->v[pickcenterindex];
	}
	nanchored=0;
	for(i=0;i<npolygons;i++){
		if(polygons[i]->anchoredx && ! polygons[i]->anchor) nanchored++;
	}
	if(nanchored && ap) center = ap->v[ap->anchorvertex];

	passive = true;

	glutPostRedisplay();
}

void motionfunc(int x, int y)
{
	int i,j,k;
	Vector vmov;
	Vector v1,v2,v3; 
	PolyGon p = pprev;
	PolyGon bestp = pprev;
	GLfloat radius,newradius;
	PolyGon *pp;
	Boolean anchoreddrag;	// $BJ#?t%*%V%8%'%/%H4V$N@)Ls$,4X78$9$k%I%i%C%0$+$I$&$+(B
	int nanchored;

//	int newanchor, newanchored;

	snappedtoobj = false;

	newap = NULL;

	if(selpolygon < 0) return;
	pp = polygons[selpolygon];

	nanchored=0;
	for(i=0;i<npolygons;i++){
		if(polygons[i]->anchoredx && ! polygons[i]->anchor) nanchored++;
	}
	anchoreddrag = (ap && pp != ap && pp->anchoredx) || (ap && (ap == pp) && nanchored > 0);

	passive = false;

	curmouse.x = (GLfloat)x;
	curmouse.y = (GLfloat)(glutGet(GLUT_WINDOW_HEIGHT) - y);

	collisiondist += distVV(curmouse,prevmouse);
	dist += distVV(curmouse,prevmouse);

	if(rotvertex >= 0){ // $B2sE>$7$F$_$k(B
		Vector vr,vp;
		GLfloat c,s,cc,ss;
		GLfloat dx,dy;
		double angle;
		double minangle;
		double minc,mins;

//printf("nanchored = %d\n",nanchored);
		if(nanchored && ap) center = ap->v[ap->anchorvertex];
		else center = pprev.v[centerindex];

//		radius = distVV(prevmouse,center);
		vr = subvector(curmouse,center);
//		newradius = length(vr);
		c = vcos(prevdir,vr);
		s = vsin(prevdir,vr);

		angle = atan2(s,c);
		if(angle < 0.0) angle = -angle;
		totalangle += angle;
//		printf("totalangle = %f\n",totalangle);

/*
		for(i=0;i<pprev.n;i++){
			GLfloat dx,dy;
			if(i == centerindex) continue;
			dx = pprev.v[i].x - pprev.v[centerindex].x;
			dy = pprev.v[i].y - pprev.v[centerindex].y;
			p.v[i].x = pprev.v[centerindex].x + c * dx - s * dy;
			p.v[i].y = pprev.v[centerindex].y + s * dx + c * dy;
		}
*/
		for(i=0;i<pprev.n;i++){
			GLfloat dx,dy;
			dx = pprev.v[i].x - center.x;
			dy = pprev.v[i].y - center.y;
			p.v[i].x = center.x + c * dx - s * dy;
			p.v[i].y = center.y + s * dx + c * dy;
		}
		cc = c;
		ss = s;

		bestp = p;
#ifdef USEANGLE
//  $B2sE>$9$k%]%j%4%s$NJU$N3QEY$rA4$F7W;;$7!"<~0O$NJU$H$N3QEY$,>.$5$1$l$P%9%J%C%W$9$k(B?
		if(totalangle > 0.5){
			minangle = 1000.0;
			for(i=0;i<p.n;i++){
				Vector p1,p2,p3,p4;
				p1 = p.v[i];
				p2 = (i < p.n-1 ? p.v[i+1] : p.v[0]);
				vr = subvector(p1,p2);
				for(j=0;j<npolygons;j++){
					if(j == selpolygon) continue;
					for(k=0;k<polygons[j]->n;k++){
						p3 = polygons[j]->v[k];
						p4 = (k < polygons[j]->n-1 ? polygons[j]->v[k+1] : polygons[j]->v[0]);
						if(distVV(p1,p3) < 50.0 ||
						   distVV(p1,p4) < 50.0 ||
						   distVV(p2,p3) < 50.0 ||
						   distVV(p2,p4) < 50.0){
							vp = subvector(p3,p4);
							s = vsin(vr,vp);
							c = vcos(vr,vp);
							angle = atan2(s,c);
							if(angle < 0) angle = -angle;
							if(angle >= M_PI/2.0){
								angle = M_PI-angle;
								c = -c;
								s = -s;
							}
							if(angle < minangle){
								minangle = angle;
								minc = c;
								mins = s;
							}
//						printf("minangle = %f\n",minangle);
						}
					}
				}
			}
			if(minangle < 0.03){
				double a1,a2;

				c = minc;
				s = mins;

				a1 = atan2(ss,cc);
				a2 = atan2(s,c);
				cc = cos(a1+a2);
				ss = sin(a1+a2);
/*
				for(i=0;i<bestp.n;i++){
					GLfloat dx,dy;
					if(i == centerindex) continue;
					dx = bestp.v[i].x - bestp.v[centerindex].x;
					dy = bestp.v[i].y - bestp.v[centerindex].y;
					bestp.v[i].x = bestp.v[centerindex].x + c * dx - s * dy;
					bestp.v[i].y = bestp.v[centerindex].y + s * dx + c * dy;
				}
*/
				for(i=0;i<bestp.n;i++){
					GLfloat dx,dy;
					dx = bestp.v[i].x - center.x;
					dy = bestp.v[i].y - center.y;
					bestp.v[i].x = center.x + c * dx - s * dy;
					bestp.v[i].y = center.y + s * dx + c * dy;
				}
			}
		}
#endif
#ifdef USEINTERSECTION
		for(i=0;i<npolygons;i++){
			if(i == selpolygon) continue;
			if(intersectPP(p,*polygons[i])) break;
		}
		if(i < npolygons){
			bestp = pprev;
			v1 = prevdir;
			v2 = vr;
			for(j=0;j<10;j++){
				vr.x = (v1.x+v2.x)/2.0;
				vr.y = (v1.y+v2.y)/2.0;
				c = vcos(prevdir,vr);
				s = vsin(prevdir,vr);
				for(i=0;i<pprev.n;i++){
					float dx,dy;
					if(i == centerindex) continue;
					dx = pprev.v[i].x - pprev.v[centerindex].x;
					dy = pprev.v[i].y - pprev.v[centerindex].y;
					p.v[i].x = pprev.v[centerindex].x + c * dx - s * dy;
					p.v[i].y = pprev.v[centerindex].y + s * dx + c * dy;
				}
				for(i=0;i<npolygons;i++){
					if(i == selpolygon) continue;
					if(intersectPP(p,*polygons[i])) break;
				}
				if(i >= npolygons){
					bestp = *p; 
					v1 = vr;
				}
				else {
					v2 = vr;
				}
			}
		}
#endif
		prevdir = subvector(curmouse,center);
		pprev = 
		*polygons[selpolygon] = bestp;
		calcframe(polygons[selpolygon]);

		for(i=0;i<npolygons;i++){
			PolyGon *q,pp;
			if(selpolygon == i) continue;
			q = polygons[i];
			pp = *q;
			if((q->anchor || q->anchored) && nanchored){
				for(j=0;j<q->n;j++){
					GLfloat dx,dy;
					dx = q->v[j].x - center.x;
					dy = q->v[j].y - center.y;
					pp.v[j].x = center.x + cc * dx - ss * dy;
					pp.v[j].y = center.y + ss * dx + cc * dy;
				}
			}
			*q = pp;
		}
	}
	else {
		int bestp, bestv, bestv2;
		float bestd;
		float bestd2;
		Vector vbest = zerovector;
		Vector vbest2 = zerovector;
		Vector vs;
		PolyGon *snappedpolygon = NULL;

		if(enlargeedge >= 0){ // $B3HBg(B
			radius = distVV(prevmouse,center);
			newradius = distVV(curmouse,center);
//			printf("radius=%f, newradius=%f\n",radius,newradius);

			for(i=0;i<pprev.n;i++){
				GLfloat dx,dy;
				if(i == centerindex) continue;
				dx = pprev.v[i].x - pprev.v[centerindex].x;
				dy = pprev.v[i].y - pprev.v[centerindex].y;
				dx *= newradius/radius;
				dy *= newradius/radius;
				p.v[i].x = pprev.v[centerindex].x + dx;
				p.v[i].y = pprev.v[centerindex].y + dy;
			}

		}
		else { // $B0\F0(B
			vmov = subvector(curmouse,origmouse);
			// $B%9%J%C%W$J$7$N>l9g$r$H$j$"$($:7W;;(B
			for(i=0;i<porig.n;i++){
				p.v[i].x = porig.v[i].x + vmov.x;
				p.v[i].y = porig.v[i].y + vmov.y;
			}
		}

		//
		// $BJL%*%V%8%'%/%H$H$N%9%J%C%W7W;;(B
		//
		bestp = 0;
		bestd = 1000.0;
		bestd2 = 1000.0;
		bestv = -1;
		bestv2 = -1;

		for(i=0;i<p.n;i++){
			p.snap[i] = 0;
		}
		for(k=0;k<porig.n;k++){
			for(i=0;i<npolygons;i++){
				int j1,j2;
				if(i == selpolygon) continue;
				for(j1=0;j1<polygons[i]->n;j1++){
					float len;
					Vector v1,v2,v12;
					j2 = (j1+1) % polygons[i]->n;
					// $B%]%j%4%s(Bi$B$NJU(Bj1-j2$B$N$&$A:G$b(Bp$B$K6a$$$H$3$m$r5a$a$k(B
					v1 = subvector(polygons[i]->v[j1],p.v[k]);
					v2 = subvector(polygons[i]->v[j2],p.v[k]);
					v12 = subvector(polygons[i]->v[j2],polygons[i]->v[j1]);
					if(iproduct(v1,v12) >= 0){ // v1$B$,:G$b6a$$(B
						vs = v1;
						len = length(vs);
						if(len < bestd){
							bestd = len;
							bestp = i;
							vbest = vs;
							bestv = k;
							snappedpolygon = polygons[i];
						}
					}
					else if(iproduct(v2,v12) <= 0){ // v2$B$,:G$b6a$$(B
						vs = v2;
						len = length(vs);
						if(len < bestd){
							bestd = len;
							bestp = i;
							vbest = vs;
							bestv = k;
							snappedpolygon = polygons[i];
						}
					}
					else { // j1-j2$B$X$N?b@~$,:G$b6a$$(B

						float d;
						d = oproduct(v1,v2) / iproduct(v12,v12);
						vs.x = d * v12.y;
						vs.y = -d * v12.x;
						len = length(vs);
						if(len < bestd2){
							bestd2 = len;
							bestp = i;
							vbest2 = vs;
							bestv2 = k;
							snappedpolygon = polygons[i];
						}
					}
				}
			}
		}
		//
		// $B$I$N%9%J%C%T%s%0$K$9$k$+7h$a$k(B
		//

		//
		// $B?7$7$$0LCV$,4{B8%*%V%8%'%/%H$H$V$D$+$C$F$k$+H=Dj(B
		//
/*
		for(i=0;i<npolygons;i++){
			if(i == selpolygon) continue;
			if(intersectPP(p,*polygons[i])) break;
		}
		if(i >= npolygons){ // $B>WFM$J$7(B
			if(collision > 0){
				collision--;
				if(collision == 0){
					collisiondist = 0.0;
				}
			}
		}
		else {
			collision = 50;
		}
*/

		p.anchor = false;
		p.anchored = false;

//		printf("bestd=%f\n",bestd);
		if(bestd < 15.0){
//			if(distVV(curmouse,origmouse) > 20.0){
			if(dist > 50.0){
				for(i=0;i<porig.n;i++){
					p.v[i].x += vbest.x;
					p.v[i].y += vbest.y;
				}
				p.snap[bestv] = 1;
				//anchorvertex = bestv;

				if(! anchoreddrag){
					p.anchorvertex = bestv;

					newap = pp;
					if(snappedpolygon->anchor || (snappedpolygon->anchored && pp != ap)){
						newap = NULL;
						p.anchored = true;
					}
				}
			}
			snappedtoobj = true;
		}
		else if(bestd2 < 10.0){
//			if(distVV(curmouse,origmouse) > 20.0){
			if(dist > 50.0){
				for(i=0;i<porig.n;i++){
					p.v[i].x += vbest2.x;
					p.v[i].y += vbest2.y;
				}
				p.snap[bestv2] = 1;
				//anchorvertex = bestv;

				if(! anchoreddrag){
					p.anchorvertex = bestv2;

					newap = pp;
					if(snappedpolygon->anchor || (snappedpolygon->anchored && pp != ap)){
						newap = NULL;
						p.anchored = true;
					}
				}
			}
			snappedtoobj = true;
		}
		else if(! collision){ // $BJL%*%V%8%'%/%H$K$O6a$/$J$$(B ... $B%0%j%C%I%9%J%C%W(B
			Vector vnew,vd;
#ifdef OBJGRID
#else
			gridx = gridy = 80.0;
#endif

			gridsizex = gridx;
			gridsizey = gridy;
			if(collisiondist < 250.0){
				gridsizex = 0.0;
				gridsizey = 0.0;
			}
			else if(collisiondist < 300.0){
				gridsizex = gridx / 8.0;
				gridsizey = gridy / 8.0;
			}
			else if(collisiondist < 400.0){
				gridsizex = gridx / 4.0;
				gridsizey = gridy / 4.0;
			}
			else if(collisiondist < 600.0){
				gridsizex = gridx / 2.0;
				gridsizey = gridy / 2.0;
			}
#ifdef NOSNAP
// $B%0%j%C%I$X$N%9%J%C%T%s%0$rL5$7$K$9$k(B
			gridsizex = gridsizey = 0.0;
#endif
			if(gridsizex > 0.0 && gridsizey > 0.0){
				Vector vmin;
				int bestv;
				vmin.x = vmin.y = 1000.0;
				for(i=0;i<p.n;i++){
					vnew.x = (int)((p.v[i].x-gridorigx) / gridsizex) * gridsizex + gridorigx; // $B%9%J%C%W$"$k>l9g$N0\F00LCV(B
					vnew.y = (int)((p.v[i].y-gridorigy) / gridsizey) * gridsizey + gridorigy;
					vd.x = p.v[i].x - vnew.x;
					vd.y = p.v[i].y - vnew.y;
					if(length(vd) < length(vmin)){
						bestv = i;
						vmin = vd;
					}
				}
				p.snap[bestv] = 1;
				if(! anchoreddrag){
					p.anchorvertex = bestv;
				}
				for(i=0;i<porig.n;i++){
					p.v[i].x -= vmin.x;
					p.v[i].y -= vmin.y;
				}
				calcframe(&p);
				snapped = true;

				if(! anchoreddrag){
					newap = pp;
				}
			}
			else {
				newap = pp;
			}
		}

		if(ap){
			if(pp == ap){ // $B%"%s%+!<$r%I%i%C%0$7$F$$$k>l9g(B
				// $B4XO"$9$kA4%]%j%4%s$r0\F0(B
				GLfloat x,y;
				x = p.v[0].x - polygons[selpolygon]->v[0].x;
				y = p.v[0].y - polygons[selpolygon]->v[0].y;
				for(i=0;i<npolygons;i++){
					if(polygons[i]->anchor || polygons[i]->anchored){
						PolyGon *p;
						p = polygons[i];
						for(j=0;j<p->n;j++){
							p->v[j].x += x;
							p->v[j].y += y;
						}
					}
				}
				pprev = 
				*polygons[selpolygon] = p;
			}
			else if(pp->anchoredx){ // $B%"%s%+!<0J30$r%I%i%C%0$7$F$$$k>l9g(B
				Vector anchor;
				Vector danchor;	// $B%I%i%C%0Cf%]%j%4%s$N%"%s%+!<E@:BI8(B
				Vector dorig; 	// $B%I%i%C%0Cf%]%j%4%s$N:G=i$N%"%s%+!<E@:BI8(B
				Vector vd;

				anchor = ap->v[ap->anchorvertex];
				danchor = p.v[p.anchorvertex];
				dorig = p.vorig;

				for(i=0;i<npolygons;i++){
					PolyGon *q;
					GLfloat dx,dy;
					q = polygons[i];
					if(q->anchored){
						PolyGon qq;
						GLfloat ratio;
/*
						ratio = 1.0;
						if(dorig.x - anchor.x != 0){
							ratio = (q->vorig.x - anchor.x) / (dorig.x - anchor.x);
						}
						else if(dorig.y - anchor.y != 0){
							ratio = (q->vorig.y - anchor.y) / (dorig.y - anchor.y);
						}
*/

						if(dorig.x != anchor.x || dorig.y != anchor.y){
							ratio = length(subvector(q->vorig,anchor))/
								length(subvector(dorig,anchor));
						}

						dx = (danchor.x - dorig.x) * ratio;
						dy = (danchor.y - dorig.y) * ratio;
/*
						dx = (danchor.x - dorig.x);
						if(dorig.x - anchor.x != 0){
							dx = (danchor.x - dorig.x) * (q->vorig.x - anchor.x) /
								(dorig.x - anchor.x);
						}
						dy = (danchor.y - dorig.y);
						if(dorig.y - anchor.y != 0){
							dy = (danchor.y - dorig.y) * (q->vorig.y - anchor.y) /
								(dorig.y - anchor.y);
						}
*/
						qq = *q;
						for(j=0;j<q->n;j++){
							qq.v[j].x = q->vorig.x + (q->v[j].x - q->v[q->anchorvertex].x) + dx;
							qq.v[j].y = q->vorig.y + (q->v[j].y - q->v[q->anchorvertex].y) + dy;
						}
						*q = qq;
					}
				}

				p.anchored = true;
				pprev = 
				*polygons[selpolygon] = p;
				calcframe(polygons[selpolygon]);
			}
			else {
				pprev = 
				*polygons[selpolygon] = p;
				calcframe(polygons[selpolygon]);
			}
			ap->anchor = true;
		}
		else {
//			p.anchor = newanchor;
//			p.anchored = newanchored;
			p.anchor = true;
			p.anchored = false;

			pprev = 
			*polygons[selpolygon] = p;
			calcframe(polygons[selpolygon]);
		}
	}

	prevmouse = curmouse;

	glutPostRedisplay();
}

//
//	$B%^%&%9%/%j%C%/(B
//
void mousefunc(int button, int state, int x, int y)
{
	int i;
	PolyGon *p;
	Vector anchorv;
	int nanchored;
	static Vector polygonpos;

	collisiondist = 0.0;
	dist = 0.0;

	totalangle = 0.0;

	nanchored=0;
	for(i=0;i<npolygons;i++){
		if(polygons[i]->anchoredx && ! polygons[i]->anchor) nanchored++;
	}

	if(state == GLUT_DOWN){
		origmouse.x = x;
		origmouse.y = glutGet(GLUT_WINDOW_HEIGHT) - y;
		curmouse = prevmouse = origmouse;

		// $BA*Br$5$l$?%]%j%4%sHV9f$r<hF@(B
		selpolygon = onpolygon(origmouse);
		Select(selpolygon,0); //***//
		if(selpolygon < 0){
			for(i=0;i<npolygons;i++){
				polygons[i]->anchor = false;
				polygons[i]->anchored = false;
			}
			ap = newap = NULL;
			return;
		}
		polygonpos = polygons[selpolygon]->v[0];

		porig = 
		pprev = *polygons[selpolygon];

		// $BA*$s$@%]%j%4%sD:E@6a$/$r%/%j%C%/$7$?$+D4$Y$k(B
		rotvertex = nearvertex(origmouse,selpolygon);

		// $B2sE>$NCf?4$H$J$kD:E@$rA*$V(B
		centerindex = rotcenter(selpolygon,rotvertex);
		center = polygons[selpolygon]->v[centerindex];

//		if(ap) center = ap->v[ap->anchorvertex];
		if(nanchored && ap) center = ap->v[ap->anchorvertex];
		else center = pprev.v[centerindex];

		prevdir = subvector(origmouse,center);

		if(rotvertex < 0){
			enlargeedge = nearedge(origmouse,selpolygon);
//			printf("enlargeedge = %d\n",enlargeedge);
			centerindex = rotcenter(selpolygon,enlargeedge);
			center = polygons[selpolygon]->v[centerindex];
		}

		collision = 0;
		snapped = false;
		snappedtoobj = false;
	}
	else if(state == GLUT_UP){
		gridsizex = gridsizey = 0.0;

		if(newap && newap != ap){
			for(i=0;i<npolygons;i++){
				polygons[i]->anchor = false;
				polygons[i]->anchored = false;
			}
			ap = newap;
			ap->anchor = true;
		}
		// $B%"%s%+!<$+$i$N5wN%Hf$r7W;;$7$F$*$/(B
		if(ap){
			anchorv = ap->v[ap->anchorvertex];
			for(i=0;i<npolygons;i++){
				p = polygons[i];
				p->vorig = p->v[p->anchorvertex];
			}
		}
		if(selpolygon >= 0 && (snappedtoobj || polygons[selpolygon]->anchoredx)){
			Vector v;
			v = subvector(polygons[selpolygon]->v[0],polygonpos);
			MoveRelative((int)v.x, (int)v.y);
			SetAnchorVertex(0,0);
		}
		else if(selpolygon >= 0){
			Vector v;
			v = polygons[selpolygon]->v[0];
			MoveAbsolute((int)v.x, (int)v.y);
			//			SetAnchorVertex(anchorvertex,0);
		}
		for(i=0;i<npolygons;i++){
			p = polygons[i];
			p->anchoredx = p->anchored;
		}


		// $BA*$s$@%]%j%4%s$,@hF,$KMh$k$h$&$KJB$Y$+$($k(B
		if(selpolygon >= 0){
			p = polygons[selpolygon];
			for(i=selpolygon-1;i>=0;i--){
				polygons[i+1] = polygons[i];
			}
			polygons[0] = p;
			selpolygon = 0;
		}
		
#ifdef OBJGRID
		if(! snapped){
			calcframe(p);
			gridx = p->right - p->left;
			gridy = p->top - p->bottom;
			if(gridx > 0.0) for(gridorigx=p->left;gridorigx>=0.0;gridorigx-=gridx);
			if(gridy > 0.0) for(gridorigy=p->bottom;gridorigy>=0.0;gridorigy-=gridy);
		}
#endif
	}

	glutPostRedisplay();
}

void kbdfunc(unsigned char key,int x, int y)
{
	int rep;
	int i;
	printf("key = %x\n",key);
	if(key == 'd'){
		if(selpolygon >= 0){
			Delete(selpolygon,0);
		}
		else {
			Delete(0,0);
		}
	}
	if(key == 'p'){
		Paste(0,0);
	}
	if(key == 't'){ // Dynamic Macro$BIw$K7+$jJV$7$r8!=P$9$k(B
		for(rep=1;rep<50;rep++){
			for(i=0;i<rep;i++){
				if(history[i].editfunc != history[rep+i].editfunc ||
				   history[i].arg1 != history[rep+i].arg1 ||
				   history[i].arg2 != history[rep+i].arg2)
					break;
			}
			if(i == rep){ // found!
				dolog = false; 
				for(i=0;i<rep;i++){
					(*history[rep-i-1].editfunc)(history[rep-i-1].arg1,history[rep-i-1].arg2);
				}
				dolog = true;
				break;
			}
		}
		//(*history[0].editfunc)(history[0].arg1,history[0].arg2);
	}
	//	sleep(10);
}

#define WINWIDTH 600
#define WINHEIGHT 400
#define WINPOSX 100
#define WINPOSY 100

main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(WINWIDTH,WINHEIGHT);
	glutInitWindowPosition(WINPOSX, WINPOSY);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("HyperSnapping");

	glutDisplayFunc(display);
	glutKeyboardFunc(kbdfunc);
	glutReshapeFunc(reshape);
	glutMotionFunc(motionfunc);
	glutPassiveMotionFunc(passivemotionfunc);
	glutMouseFunc(mousefunc);
	glutIdleFunc(idlefunc);

	glEnable(GL_BLEND); // $BF)L@=hM}(B

	initdata();

	glutMainLoop();
}
