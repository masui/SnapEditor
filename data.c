#include "vector.h"

extern PolyGon *polygons[];
extern int npolygons;

#ifdef TANGRAM
static PolyGon poly1 = {
	3,
	{{0.0*0.6, 0.0*0.6},
	{200.0*0.6, 0.0*0.6},
	{100.0*0.6, 100.0*0.6}},
	0.9, 0.3, 0.7
};
static PolyGon poly2 = {
	3,
	{{210.0*0.6, 0.0*0.6},
	{410.0*0.6, 0.0*0.6},
	{410.0*0.6, 200.0*0.6}},
	0.8, 0.4, 0.4
};
static PolyGon poly3 = {
	4,
	{{210.0*0.6, 10.0*0.6},
	{310.0*0.6, 110.0*0.6},
	{210.0*0.6, 210.0*0.6},
	{110.0*0.6, 110.0*0.6}},
	0.7, 0.5, 0.7
};
static PolyGon poly4 = {
	3,
	{{320.0*0.6, 120.0*0.6},
	{320.0*0.6, 320.0*0.6},
	{220.0*0.6, 220.0*0.6}},
	0.6, 0.6, 0.4
};
static PolyGon poly5 = {
	4,
	{{330.0*0.6, 130.0*0.6},
	{430.0*0.6, 230.0*0.6},
	{430.0*0.6, 430.0*0.6},
	{330.0*0.6, 330.0*0.6}},
	0.5, 0.7, 0.7
};
static PolyGon poly6 = {
	3,
	{{0.0*0.6, 10.0*0.6},
	{200.0*0.6, 210.0*0.6},
	{0.0*0.6, 410.0*0.6}},
	0.4, 0.8, 0.4
};
static PolyGon poly7 = {
	3,
	{{210.0*0.6, 220.0*0.6},
	{410.0*0.6, 420.0*0.6},
	{10.0*0.6, 420.0*0.6}},
	0.3, 0.9, 0.7
};

#else
static PolyGon poly1 = {
	4,
	{{100.0, 100.0},
	{200.0, 110.0},
	{220.0, 190.0},
	{100.0, 200.0}},
	0.05, 0.75, 0.0
};
static PolyGon poly2 = {
	5,
	{{200.0, 200.0},
	{300.0, 180.0},
	{330.0, 320.0},
	{240.0, 360.0},
	{180.0, 250.0}},
	0.85, 0.35, 0.8
};
static PolyGon poly3 = {
	4,
	{{400.0, 200.0},
	{400.0, 300.0},
	{500.0, 300.0},
	{500.0, 200.0}},
	0.3, 0.9, 0.5
};

static PolyGon rect1 = {
//	1,
	4,
	{{100.0, 100.0},
	{100.0, 220.0},
	{220.0, 220.0},
	{220.0, 100.0}},
	0.3, 0.9, 0.5
};

static PolyGon rect2 = {
//	0,
	4,
	{{320.0, 320.0},
	{320.0, 400.0},
	{400.0, 400.0},
	{400.0, 320.0}},
	0.5, 0.7, 0.7
};
static PolyGon rect3 = {
//	0,
	4,
	{{320.0, 320.0},
	{320.0, 400.0},
	{400.0, 400.0},
	{400.0, 320.0}},
	0.8, 0.2, 0.5
};
static PolyGon rect4 = {
//	0,
	4,
	{{320.0, 320.0},
	{320.0, 400.0},
	{400.0, 400.0},
	{400.0, 320.0}},
	0.6, 0.9, 0.2
};
static PolyGon rect5 = {
//	0,
	4,
	{{320.0, 320.0},
	{320.0, 400.0},
	{400.0, 400.0},
	{400.0, 320.0}},
	0.6, 0.9, 0.2
};
static PolyGon rect6 = {
//	0,
	4,
	{{320.0, 320.0},
	{320.0, 400.0},
	{400.0, 400.0},
	{400.0, 320.0}},
	0.6, 0.9, 0.2
};
static PolyGon rect7 = {
//	0,
	4,
	{{320.0, 320.0},
	{320.0, 400.0},
	{400.0, 400.0},
	{400.0, 320.0}},
	0.6, 0.9, 0.2
};
static PolyGon tri1 = {
//	1,
	3,
	{{200.0, 100.0},
	{500.0, 100.0},
	{500.0, 190.0}},
	0.2, 0.6, 0.8
};

#endif

void
regpolygon(PolyGon *p)
{
	int i;
	polygons[npolygons++] = p;
	calcframe(p);
	for(i=0;i<p->n;i++){
		p->snap[i] = 0;
	}
	p->a = 1.0;

	//	p->anchor = 0;
	//	p->anchored = 0;
	//	p->anchorvertex = 0;
}

void initdata()
{
	npolygons = 0;
#ifdef TANGRAM
	regpolygon(&poly1);
	regpolygon(&poly2);
	regpolygon(&poly3);
	regpolygon(&poly4);
	regpolygon(&poly5);
	regpolygon(&poly6);
	regpolygon(&poly7);
#else
	regpolygon(&poly1);
	regpolygon(&poly2);
	regpolygon(&poly3);
	regpolygon(&rect1);
	regpolygon(&rect2);
	regpolygon(&rect3);
	regpolygon(&rect4);
	regpolygon(&rect5);
	regpolygon(&rect6);
	regpolygon(&rect7);
	regpolygon(&tri1);
#endif
}
