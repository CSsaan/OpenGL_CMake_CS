
#ifndef __CLIP_H__
#define __CLIP_H__

// Math macros
#undef SQR
#define SQR(x) ((x) * (x))
#undef CLIP
#define CLIP(x, y, z) ((x) < (y) ? (y) : ((x) > (z) ? (z) : (x)))
#undef RECLIP
#define RECLIP(x, y, z) ((x) = ((x) < (y) ? (y) : ((x) > (z) ? (z) : (x))))
#undef CLAMP
#define CLAMP(x, y, z) ((x) = ((x) < (y) ? (y) : ((x) > (z) ? (z) : (x))))
#undef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#undef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#undef EQUIV
#define EQUIV(x, y) (fabs((x) - (y)) < 0.001)
#undef DISTANCE
#define DISTANCE(x1, y1, x2, y2) \
(sqrt(((x2) - (x1)) * ((x2) - (x1)) + ((y2) - (y1)) * ((y2) - (y1))))
#define TO_RAD(x) ((x) * 2 * M_PI / 360)
#define TO_DEG(x) ((x) * 360 / 2 / M_PI)

#define	bc_always_inline __attribute__ ((__always_inline__)) inline

static bc_always_inline int bmin(int a, int b) { return a < b ? a : b; }
static bc_always_inline float bmin(float a, float b) { return a < b ? a : b; }
static bc_always_inline double bmin(double a, double b) { return a < b ? a : b; }
static bc_always_inline int bmax(int a, int b) { return a > b ? a : b; }
static bc_always_inline float bmax(float a, float b) { return a > b ? a : b; }
static bc_always_inline double bmax(double a, double b) { return a > b ? a : b; }

static bc_always_inline int iclip(int iv, int imn, int imx) {
	return iv < imn ? imn : iv > imx ? imx : iv;
}
static bc_always_inline int bclip(int &iv, int imn, int imx) {
	return iv < imn ? imn : iv > imx ? imx : iv;
}
static bc_always_inline float bclip(float &fv, float fmn, float fmx) {
	return fv < fmn ? fmn : fv > fmx ? fmx : fv;
}
static bc_always_inline double bclip(double &dv, double dmn, double dmx) {
	return dv < dmn ? dmn : dv > dmx ? dmx : dv;
}

static bc_always_inline void bclamp(int &iv, int imn, int imx) {
	if( iv < imn ) iv = imn; else if( iv > imx ) iv = imx;
}
static bc_always_inline void bclamp(float &fv, float fmn, float fmx) {
	if( fv < fmn ) fv = fmn; else if( fv > fmx ) fv = fmx;
}
static bc_always_inline void bclamp(double &dv, double dmn, double dmx) {
	if( dv < dmn ) dv = dmn; else if( dv > dmx ) dv = dmx;
}

#endif