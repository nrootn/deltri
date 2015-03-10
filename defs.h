
#define DIM 3
#define _X_ 0
#define _Y_ 1
#define _Z_ 2
#define ELE 0
#define POINT 1

typedef double coords_t[3];  // coords_t: array of 3 float
typedef uint   tet_t[4];     // tet_t:   array of 4 uint

typedef struct point {
	uint indx;
	coords_t coords;
	uint rid;
} point_t;

typedef struct ele {
	uint indx;
	tet_t tet;
	uint rid;
} ele_t;
