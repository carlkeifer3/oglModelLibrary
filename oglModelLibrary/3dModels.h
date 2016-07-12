#ifndef __3DMODELS_H__
#define __3DMODELS_H__

#include <vector>
#define GLM_FORCE_RADIANS
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

typedef struct vector_s
{
	float x, y, z;
} vector_t;

typedef struct vertex_s
{
	float x, y, z;
	vector_t vNrml;				  // static type to store this vertex's normal
	std::vector<vector_t> fNrmls; // dynamic array for storing face normals
} vertex_t;

typedef struct polygon_s   // defines a triangle and it's normal
{
	// iterator for locating verts associated with this triangle
	GLint a, b, c;

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
	void cacheCylinder(float vertsX[], float vertsY[], float depth);
	void cacheEndCap(float vertsX[], float vertsY[], float depth);
	void cacheSPipe();
	void cachefPipe();

	// functions for drawing our model
	void DrawPolygon(glm::mat4 view, glm::mat4 proj);
	void DrawLine();
	void DrawNormals();

	// returns the nubmer of polygons in our scene
	int getSize();

	// returns the coordinates of our polygon
	vector_t getPoly(int i, int p);

private:
	GLuint positionID;
	GLuint normalID;
	GLuint diffColorID;
	GLuint bufferID;
	GLuint elementID;
	GLuint arrayID;

	GLuint locLight;	// the positional vector of our light


	GLuint mvpID;		// model view projection matrix ID
	GLuint mvID;		// model view matrix ID
	GLuint nmID;		// normal matrix ID

	GLuint sID;

	bool dir;      //indicates which way our normals are calculated
	model_t model; // moved the model data to private to prevent outside interference
	std::vector<lineSegmen_t> lineDwg;  // dynamic array that stores our line drawing for this model

	void makeShader();
	void makeBuffer();

	// protected functions to help with model caching
	void cacheCylinder2R(vector_t ring1[], vector_t ring2[], float z0, float z1);
	void cacheCylinderCapV( vector_t radiusPts[], float depth);
	void cacheCylinderCap(vector_t vArray[], float depth);
	int  cacheSphericalCap(float radius, float zStart, float headRatio);
	void cacheOnExisting(int start, int vCountA, int vCountB);

	// functions to help with polygon normalization:
	void calculateFaceNormal (int poly);
	void normalizeVerts();
	void normalizeVector (vector_t *v);
	void storeFaceNrml(vertex_t *v, vector_t n);

	int vertCheck(vertex_t v);
};

#endif // __3DMODELS_H__