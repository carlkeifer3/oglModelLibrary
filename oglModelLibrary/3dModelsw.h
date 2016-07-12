#ifndef __3DMODELS_H__
#define __3DMODELS_H__

#include <afxtempl.h>
#include <vector>
#include "Category.h"

typedef struct vector_s
{
	float x, y, z;
} vector_t;

typedef struct vertex_s
{
	float x, y, z;
	std::vector<vector_t> fNrmls; // dynamic array for storing face normals
	vector_t vNrml;				  // static type to store this vertex's normal
} vertex_t;

typedef struct polygon_s   // defines a triangle and it's normal
{
	// iterator for locating verts associated with this triangle
	int a;					
	int b;
	int c;
} polygon_t;

typedef struct model_s
{
	std::vector<vertex_t> verts;	// dynamic array for storing our vertices
	std::vector<polygon_t> polys;	// dynamic array for defining polys in our model
} model_t;

typedef struct lineSegment_s
{
	float x1;
	float y1;
	float z1;

	float x2;
	float y2;
	float z2;
}lineSegmen_t;

class meshModels
{
public:

	meshModels();
	~meshModels();

	int numSides;	// determines the number of sides our model has default is 32
	float centerX;
	float centerY;	

	std::vector<int> frontFaces; // list of indices of all front faces

	bool	debug;			// used to switch on visual debugging
	bool	smooth;			// sets smooth or flat shading
	bool	sepSide;		// determines if the sides are normalized separately
	bool	lines;			// determines if the lines are rendered or not
	
	bool	transparent;	// turns transparency on for this object
	int		opacity;		// sets opacity for this object

	float	lineWidth;		// sets the width of the current line

	// functions to help with model caching
	void clearModel();
	void cacheRoundSheet(vector_t vArray[], float depth);
	void cacheBox( float topWidth, float botWidth, float height, float depth, float hoodThickness, float hoodDepth);
	void cachePipe(RECTSHELLPROPERTIES shell);

	void cacheNozzle(RECTNOZZLEDATA nozz);
	void cacheImp(RECTNOZZLEDATA nozz);
	void cacheEndCap(RECTENDCAPPROPERTIES eCap);
	void cacheBolt(RECTENDCAPPROPERTIES eCap);

	// functions for drawing our model
	void DrawPolygon();
	void DrawLine();
	void DrawNormals();

	// returns the nubmer of polygons in our scene
	int getSize();

	// returns the coordinates of our polygon
	vector_t getPoly(int i, int p);

private:

	bool dir;      //indicates which way our normals are calculated
	model_t model; // moved the model data to private to prevent outside interference
	std::vector<lineSegmen_t> lineDwg;  // dynamic array that stores our line drawing for this model

	// protected functions to help with model caching
	void cacheCylinder2R(vector_t ring1[], vector_t ring2[], float z0, float z1);
	void cacheCylinderCapV( vector_t radiusPts[], float depth);
	void cacheCylinderCap(vector_t vArray[], float depth);
	int  cacheSphericalCap(float radius, float zStart, float headRatio);
	void cacheOnExisting(int start, int vCountA, int vCountB);

	// functions to help with polygon normalization:
	void calculateFaceNormal (int poly, bool dir);
	void normalizeVerts();
	void normalizeVector (vector_t *v);
	void storeFaceNrml(vertex_t *v, vector_t n);

	int vertCheck(vertex_t v);
};

#endif // __3DMODELS_H__