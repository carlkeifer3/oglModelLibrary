// 3dModels.cpp - library for building 3d objects

#include "stdafx.h"
#include <afxtempl.h>
#include <math.h>
#include "3dModels.h"

meshModels::meshModels()
{
	debug = FALSE;			// Turn visual debugging off by default
	smooth = TRUE;			// turn smooth shading on by default
	transparent = FALSE;	// turn transparency off by default
	
	opacity = 255;			// default opacity set to 100%

	dir = TRUE;

	numSides = 32;
	centerX = 0.0f;
	centerY = 0.0f;

	lineWidth = 0.5f;

	sepSide = TRUE;
}

meshModels::~meshModels()
{ 
	// clear our vector arrays
	model.polys.~vector<polygon_t>();
	model.verts.~vector<vertex_t>();
}

// calculates the normal for the given face
void meshModels::calculateFaceNormal(int poly, bool dir)
{
    vector_t a, b, normal;

    // calculate the vectors A and B
    // note that v[3] is defined with counterclockwise winding in mind
    // a
	int vA = model.polys[poly].a;
	int vB = model.polys[poly].b;
	int vC = model.polys[poly].c;

	if( dir == TRUE)
	{
		a.x = model.verts[vC].x - model.verts[vB].x;
		a.y = model.verts[vC].y - model.verts[vB].y;
		a.z = model.verts[vC].z - model.verts[vB].z;

		// b
		b.x = model.verts[vB].x - model.verts[vA].x;
		b.y = model.verts[vB].y - model.verts[vA].y;
		b.z = model.verts[vB].z - model.verts[vA].z;
	}
	else
	{
		a.x = model.verts[vA].x - model.verts[vB].x;
		a.y = model.verts[vA].y - model.verts[vB].y;
		a.z = model.verts[vA].z - model.verts[vB].z;

		// b
		b.x = model.verts[vB].x - model.verts[vC].x;
		b.y = model.verts[vB].y - model.verts[vC].y;
		b.z = model.verts[vB].z - model.verts[vC].z;
	}

    // calculate the cross product and place the resulting vector
    // into the address specified by vector_t *normal
    normal.x = (a.y * b.z) - (a.z * b.y);
    normal.y = (a.z * b.x) - (a.x * b.z);
    normal.z = (a.x * b.y) - (a.y * b.x);

    // normalize
    normalizeVector(&normal);

	//save this normal on all three verts
	storeFaceNrml(&model.verts[vA], normal);
	storeFaceNrml(&model.verts[vB], normal);
	storeFaceNrml(&model.verts[vC], normal);
}// End  meshModels::calculateFaceNormal()

// ensures the unit length of the normal is 1
void meshModels::normalizeVector(vector_t *v)
{
    // calculate the length of the vector
    float len = (float)(sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z)));

    // avoid division by 0
    if (len == 0.0f)
        len = 1.0f;

    // reduce to unit size
    v->x /= len;
    v->y /= len;
    v->z /= len;
}//End  meshModels::normalizeVector()

// stores the face normal at the given vertex
void meshModels::storeFaceNrml(vertex_t *v, vector_t n)
{
	// find out how many normals currently exist at this vert
	int nCount = v->fNrmls.size()+1;

	v->fNrmls.reserve(nCount);
	v->fNrmls.push_back(n);
}//End meshModels::storeFaceNrml()

// calculates the normal for every vertex in the model
void meshModels::normalizeVerts()
{
	vector_t avgSum;
	int i, j;

	// determine the number of verts in our mesh
	int nVerts = model.verts.size();

	for( i = 0; i < nVerts; i++)
	{
		// determine the number of face normals collected here
		int n = model.verts[i].fNrmls.size();		
		
		//initialize our sum
		avgSum.x = 0.0f;
		avgSum.y = 0.0f;
		avgSum.z = 0.0f;

		// iterate through the normals and average them
		for( j = 0; j < n; j++)		
		{
			avgSum.x = avgSum.x + model.verts[i].fNrmls[j].x;
			avgSum.y = avgSum.y + model.verts[i].fNrmls[j].y;
			avgSum.z = avgSum.z + model.verts[i].fNrmls[j].z;
		}

		avgSum.x /= n;
		avgSum.y /= n;
		avgSum.z /= n;
		
		normalizeVector(&avgSum);

		// store our normal at the vertex
		model.verts[i].vNrml.x = avgSum.x;
		model.verts[i].vNrml.y = avgSum.y;
		model.verts[i].vNrml.z = avgSum.z;

		// remove the cache of face normals 
		model.verts[i].fNrmls.clear();
	}
}//End meshModels::normalizeVerts()

int meshModels::vertCheck(vertex_t v)
{
	bool vCheck = FALSE;
	int vertId;
	int i;
	int numChecks = model.verts.size();

	// iterate the list of verts
	// compare the verts with our supplied vertex
	for(i=0; i < numChecks; i++)
	{
		// check x and y z should be zero
		if(v.x == model.verts[i].x && v.y == model.verts[i].y && v.z == model.verts[i].z)
		{
			vCheck = TRUE;
			vertId = i;
			break;
		}
	}
	
	if(vCheck == TRUE)
	{
		return vertId;
	}
	else
	{
		vertId = model.verts.size();
		return vertId;
	}
}

void meshModels::clearModel()
{
	// clear the array of faces and points
	model.polys.clear();
	model.verts.clear();

	// empty out our data containers too
	frontFaces.clear();
	lineDwg.clear();
}

void meshModels::cacheRoundSheet(vector_t vArray[], float depth)
{
	//polygon_t poly;
	
	int i = 0;
	float centerZ = 0.0f;
	dir = FALSE;

	// cache the front sheet
	// this first round should capture the front faces
	cacheCylinderCap( vArray, centerZ);

	int size = (int)model.polys.size();
	for(i=0; i < size; i++)
	{
		// store the index of all the front faces.
		// this only works because I know this is the first thing 
		// that gets made in this model. but this will cause data
		// inconsistency if anything is in the cache already.
		frontFaces.push_back(i);
	}

	centerZ = depth;
	dir = TRUE;
	// then cache the back sheet
	cacheCylinderCap( vArray, centerZ);

	/*
	// now we should have all the verts we need for the model contained in these two faces
	// I will build the sides from the existing verts by adding faces only
	int vA = 1;
	int vB = vA + 1;
	int vC = numSides + 2;
	
	dir = FALSE;

	for( i = 0; i < numSides; i++)
	{
		//record all three points as a triangle in our structure
		poly.a = vA;
		poly.b = vB;
		poly.c = vC;
	
		model.polys.push_back(poly);

		int faceCount = model.polys.size()-1;
		calculateFaceNormal(faceCount, dir);

		vA = vB;
		vB++;
		vC++;
		if(vB >= 33)
			vB = 1;
		if(vC >= 66)
			vC = numSides + 2;
	}
	
	vA = numSides +2;
	vB = vA + 1;
	vC = 2;
	dir = TRUE;
	
	for( i = 0; i <= numSides; i++)
	{
		//record all three points as a triangle in our structure
		poly.a = vA;
		poly.b = vB;
		poly.c = vC;
	
		model.polys.push_back(poly);

		int faceCount = model.polys.size()-1;
		calculateFaceNormal(faceCount, dir);

		vA = vB;
		vB++;
		vC++;
		if(vB >= 66)
			vB = numSides + 2;
		if(vC >= 33)
			vC = 1;
	}*/

	if( sepSide == TRUE)
	{
		dir = FALSE;
		// remove the side faces and generate a cylinder that has it's own normal space
		model.polys.resize(numSides*2);
		cacheCylinder2R( vArray, vArray, depth, 0.0f);
	}

	normalizeVerts();
}

void meshModels::cacheCylinderCap(vector_t vArray[], float depth)
{
	// I'm going to attempt to build the tubesheet / baffle from a circle diagram

	vertex_t vertA, vertB, vertC;
	polygon_t poly;
	lineSegmen_t line;

	int j = 0;
	int vCount = model.verts.size();
	int pCount = model.polys.size();
		
	poly.a = vCount; // define the first point of our triangle
	poly.b = vCount + 1; // store initial values that should be correct
	poly.c = vCount + 2;

	// establish center point
	// this should be calculated from bounds
	vertA.x= centerX;
	vertA.y = centerY;
	vertA.z = depth;
	//model.verts.reserve(vCount);
	model.verts.push_back(vertA);

	vCount = model.verts.size()+1;
	int endGon = vCount;	// saving this vert id for later

	// establish the second triangle point along the perimiter
	vertB.x = vArray[j].x;
	vertB.y = vArray[j].y;
	vertB.z = depth;
	//model.verts.reserve(vCount);
	model.verts.push_back(vertB);
	line.x1 = vertB.x;
	line.y1 = vertB.y;
	line.z1 = vertB.z;

	// creating numSides polygons for the end cap
	for(j = 1; j < numSides; ++j)
	{
		vCount = model.verts.size();
		pCount = model.polys.size();

		//establish the final triangle point along the perimiter
		vertC.x = vArray[j].x;
		vertC.y = vArray[j].y;
		vertC.z = depth;

		// here is where I want to check if my vert exists
		// if it does we should use that vert. 
		// if not then we can create a new vert in the array
		
		int newVert = vertCheck(vertC);

		if(newVert >= vCount)
		{
		//model.verts.reserve(vCount+1);
			model.verts.push_back(vertC);
		
			poly.b = vCount;
			poly.c = vCount - 1;
			
			model.polys.push_back(poly);
			calculateFaceNormal(pCount, dir);

		}	
		else
		{
			// set the polygon
			poly.c = poly.b;
			poly.b = newVert;

			bool skip = FALSE;
			// if the values of vert c xyz and vert b xyz are the
			// same the face is really a line and can be skipped
			if (model.verts[poly.c].x == model.verts[newVert].x &&  
				model.verts[poly.c].y == model.verts[newVert].y &&
				model.verts[poly.c].z == model.verts[newVert].z)
			{
				skip = TRUE;
			}

			if(skip == FALSE)
			{
				model.polys.push_back(poly);
				calculateFaceNormal(pCount, dir);
			}
		}
		
		line.x2 = vertC.x;
		line.y2 = vertC.y;
		line.z2 = vertC.z;

		lineDwg.push_back(line);

		line.x1 = line.x2;
		line.y1 = line.y2;
		line.z1 = line.z2;
	}
		
	vCount = model.verts.size();
	pCount = model.polys.size();
	poly.b = endGon - 1;
	poly.c = vCount - 1;

	model.polys.reserve(pCount + 1);
	model.polys.push_back(poly);

	calculateFaceNormal(pCount, dir);

	line.x2 = vertB.x;
	line.y2 = vertB.y;
	line.z2 = vertB.z;
	lineDwg.push_back(line);
}

void meshModels::cacheEndCap(RECTENDCAPPROPERTIES eCap)
{
	vector_t radiusInner[32];
	vector_t radiusOuter[32];
	vector_t flangeRadius[32];

	const double M_PI = 3.14159265;
	float minorStep = 2.0f * (float)M_PI / numSides; // this is phi
	int j;
	int vCountA, vCountB, start, firstVert, polyCount;

	float bump = 0.1f;	// keeps the model from exploding around the cap

	float sphereWidth  = eCap.radiusInner;
	float sphereHeight = eCap.coverLength + eCap.headRatio + bump;

	float bodyStart = 0.0f;
	// we should build our vertex arrays first
	for(j = 0; j < numSides; j++)
	{
		vector_t vertex;
		vertex.z = 0.0f;

		float a = j * minorStep;
		vertex.x = (float)(eCap.radiusInner * cos(a));
		vertex.y = (float)(eCap.radiusInner * sin(a));
		radiusInner[j] = vertex;

		vertex.x = (float)(eCap.radiusOuter * cos(a));
		vertex.y = (float)(eCap.radiusOuter * sin(a));
		radiusOuter[j] = vertex;

		if(eCap.addFlange == TRUE)
		{
			vertex.x = (float)(eCap.radiusFlange * cos(a));
			vertex.y = (float)(eCap.radiusFlange * sin(a));
			flangeRadius[j] = vertex;
		}
	}
	
	if(eCap.addFlange == TRUE)
	{
		dir = TRUE;
		firstVert = 1;
		// front face of EndCap
		cacheCylinder2R( radiusInner, flangeRadius, bodyStart, bodyStart);
		vCountA = model.verts.size();
		start = vCountA;

		// front flange back face
		cacheCylinder2R( flangeRadius, radiusOuter, bodyStart + eCap.flangeThickness, bodyStart + eCap.flangeThickness);
		polyCount = model.polys.size();
		vCountB = model.verts.size();
		cacheOnExisting( firstVert, vCountA, vCountB);
		model.polys.resize(polyCount);
		firstVert = vCountA;
		vCountA = vCountB;
		
		bodyStart = bodyStart + eCap.flangeThickness;	
	}
	else
	{
		// just draw the inside ring for the cap
		// I will add this after I fix the top.
		// we should save our vert pos so we can tie this to the inside
	}

	firstVert = cacheSphericalCap((float)eCap.radiusOuter, eCap.coverLength, eCap.headRatio);
	vCountA = model.verts.size();	

	cacheCylinder2R(radiusOuter, radiusOuter, eCap.coverLength,  bodyStart);
	vCountB = model.verts.size();
	cacheOnExisting( firstVert, vCountA, vCountB);


	if(eCap.addFlange == TRUE)
	{
		// outside face of the flange
		dir = FALSE;
		cacheCylinder2R(flangeRadius, flangeRadius, bodyStart, 0.0f);

	}

	// finished calculate normals for box
	normalizeVerts();
}

void meshModels::cacheBolt(RECTENDCAPPROPERTIES eCap)
{

	// first I think we need to cache a cylinder that is the
	// overall length so we have something to display
	vector_t boltBody[32];
	vector_t boltHead[6];

	const double M_PI = 3.14159265;
	float minorStep = 2.0f * (float)M_PI / numSides; // this is phi
	int j;
	float radius = eCap.boltRadius;

	// we should build our vertex arrays first
	for(j = 0; j < numSides; j++)
	{
		vector_t vertex;
		vertex.z = 0.0f;

		float a = j * minorStep;
		vertex.x = (float)(radius * cos(a));
		vertex.y = (float)(radius * sin(a));
		boltBody[j] = vertex;
	
	}

	radius = radius + (eCap.boltRadius * 0.7f);
	minorStep = 2.0f * (float)M_PI / 6; // this is phi

	// we should build our vertex arrays first
	for(j = 0; j < 6; j++)
	{
		vector_t vertex;
		vertex.z = 0.0f;

		float a = j * minorStep;
		vertex.x = (float)(radius * cos(a));
		vertex.y = (float)(radius * sin(a));
		boltHead[j] = vertex;
	
	}

	// now we can draw the caps and bolt body
	

	
	cacheCylinder2R( boltBody, boltBody, eCap.boltOutLength * -1, -0.01f);
	cacheCylinderCapV( boltBody, eCap.boltOutLength * -1);


	// construct the bolt Head
	numSides = 6;
	cacheCylinder2R( boltHead, boltHead, 0.0f, eCap.boltOutLength * 0.1f);
	cacheCylinderCapV( boltHead, 0.0f);
	cacheCylinderCapV( boltHead, eCap.boltOutLength * 0.1f);

	// then construct the nut
	cacheCylinder2R( boltHead, boltHead, (eCap.boltInLength + eCap.boltOutLength * 0.1f) * -1,  eCap.boltInLength * -1);
	cacheCylinderCapV( boltHead, (eCap.boltInLength + eCap.boltOutLength * 0.1f) * -1);
	cacheCylinderCapV( boltHead,  eCap.boltInLength * -1);

	// set our sides back so this works if we cache again
	numSides = 32;
	normalizeVerts();
	/*
	radius = eCap.boltRadius;
	minorStep = 2.0f * (float)M_PI / 32; // this is phi

	// reAssign array for thread pattern
	for(j = 0; j < numSides; j++)
	{
		vector_t vertex;
		vertex.z = 0.0f;

		float a = j * minorStep;
		vertex.x = (float)((radius + 0.01f) * cos(a));
		vertex.y = (float)((radius + 0.01f) * sin(a));
		boltBody[j] = vertex;
	
	}

	// draw the thread pattern
	float zOffA = 0.0f;
	float zOffB = -0.003f;
	int i = 1;
	int numThreads = (int)eCap.boltOutLength * 10;
	for(int k=0; k <= numThreads; k++)
	{

		for(j = 0; j < numSides; j++)
		{
			lineSegmen_t line;
	
			line.x1 = boltBody[j].x;
			line.y1 = boltBody[j].y;
			line.z1 = zOffA;

			zOffA = zOffB;
			i = j + 1;
			if(i >= numSides)
			{
				i = 0;
			}

			line.x2 = boltBody[i].x;
			line.y2 = boltBody[i].y;
			line.z2 = zOffB;
	
			zOffB = zOffB - 0.003f;

			lineDwg.push_back(line);
		}
	}*/
}

void meshModels::cacheBox( float topWidth, float botWidth, float height, float depth, float hoodThickness, float hoodDepth)
{
	vertex_t vert;
	polygon_t poly;
	lineSegmen_t line;

	// clear our front faces cache
	frontFaces.clear();

	topWidth = topWidth * 0.5f;
	botWidth = botWidth * 0.5f;
	height   = height * 0.5f;

	// polygon 0
	poly.a = 1;
	poly.b = 0;
	poly.c = 2;

	// vertex 0
	vert.x = topWidth;
	vert.y = height;
	vert.z = hoodDepth * -1;
	line.x1 = vert.x;
	line.y1 = vert.y;
	line.z1 = vert.z;

	model.verts.push_back(vert);

	// vertex 1
	vert.x = topWidth;
	vert.y = height;
	vert.z = depth;

	// line segment (0,1)
	line.x2 = vert.x;
	line.y2 = vert.y;
	line.z2 = vert.z;
	lineDwg.push_back(line);

	model.verts.push_back(vert);

	//vertex 2
	vert.x = topWidth * -1;
	vert.y = height;
	vert.z = hoodDepth * -1;
	
	// line segment (0,2)
	line.x2 = vert.x;
	line.y2 = vert.y;
	line.z2 = vert.z;
	lineDwg.push_back(line);

	model.verts.push_back(vert);
	model.polys.push_back(poly);
	calculateFaceNormal(0, FALSE);
	
	// top side faces
	// polygon 1
	poly.a = 2;
	poly.b = 3;
	poly.c = 1;

	//vertex 3
	vert.x = topWidth * -1;
	vert.y = height;
	vert.z = depth;

	model.verts.push_back(vert);
	model.polys.push_back(poly);
	calculateFaceNormal(1, FALSE);

	// line segment (2,3)
	line.x1 = vert.x;
	line.y1 = vert.y;
	line.z1 = vert.z;
	lineDwg.push_back(line);

	// line segment (1,3)
	line.x2 = model.verts[1].x;
	line.y2 = model.verts[1].y;
	line.z2 = model.verts[1].z;
	lineDwg.push_back(line);

	// right Side faces
	// polygon 2
	poly.a = 2;
	poly.b = 4;
	poly.c = 3;

	//vertex 4
	vert.x = botWidth * -1;
	vert.y = height * -1;
	vert.z = hoodDepth * -1;

	model.verts.push_back(vert);
	model.polys.push_back(poly);
	calculateFaceNormal(2, FALSE);

	// line segment (2,4)
	line.x1 = model.verts[2].x;
	line.y1 = model.verts[2].y;
	line.z1 = model.verts[2].z;

	line.x2 = vert.x;
	line.y2 = vert.y;
	line.z2 = vert.z;
	lineDwg.push_back(line);

	// polygon 3
	poly.a = 4;
	poly.b = 5;
	poly.c = 3;

	//vertex 5
	vert.x = botWidth * -1;
	vert.y = height * -1;
	vert.z = depth;

	model.verts.push_back(vert);
	model.polys.push_back(poly);
	calculateFaceNormal(3, FALSE);

	// line segment (4,5)
	line.x1 = model.verts[4].x;
	line.y1 = model.verts[4].y;
	line.z1 = model.verts[4].z;

	line.x2 = vert.x;
	line.y2 = vert.y;
	line.z2 = vert.z;
	lineDwg.push_back(line);

	// line segment (3,5)
	line.x1 = model.verts[3].x;
	line.y1 = model.verts[3].y;
	line.z1 = model.verts[3].z;

	lineDwg.push_back(line);

	// bottom faces

	// polygon 4
	poly.a = 4;
	poly.b = 6;
	poly.c = 5;

	//vertex 6
	vert.x = botWidth;
	vert.y = height * -1;
	vert.z = hoodDepth * -1;

	model.verts.push_back(vert);
	model.polys.push_back(poly);
	calculateFaceNormal(4, FALSE);

	// line segment (4,6)
	line.x1 = model.verts[4].x;
	line.y1 = model.verts[4].y;
	line.z1 = model.verts[4].z;

	line.x2 = vert.x;
	line.y2 = vert.y;
	line.z2 = vert.z;
	lineDwg.push_back(line);

	// polygon 5
	poly.a = 6;
	poly.b = 7;
	poly.c = 5;

	//vertex 7
	vert.x = botWidth;
	vert.y = height * -1;
	vert.z = depth;

	model.verts.push_back(vert);
	model.polys.push_back(poly);
	calculateFaceNormal(5, FALSE);

	// line segment (5,7)
	line.x1 = model.verts[5].x;
	line.y1 = model.verts[5].y;
	line.z1 = model.verts[5].z;

	line.x2 = vert.x;
	line.y2 = vert.y;
	line.z2 = vert.z;
	lineDwg.push_back(line);

	// line segment (5,7)
	line.x1 = model.verts[6].x;
	line.y1 = model.verts[6].y;
	line.z1 = model.verts[6].z;

	lineDwg.push_back(line);

	// left side faces

	// polygon 6
	poly.a = 6;
	poly.b = 0;
	poly.c = 7;

	model.polys.push_back(poly);
	calculateFaceNormal(6, FALSE);

	// polygon 7
	poly.a = 0;
	poly.b = 1;
	poly.c = 7;

	model.polys.push_back(poly);
	calculateFaceNormal(7, FALSE);

	// line segment (1,7)
	line.x1 = model.verts[1].x;
	line.y1 = model.verts[1].y;
	line.z1 = model.verts[1].z;

	lineDwg.push_back(line);

	// line segment (0,6)
	line.x1 = model.verts[0].x;
	line.y1 = model.verts[0].y;
	line.z1 = model.verts[0].z;

	line.x2 = model.verts[6].x;
	line.y2 = model.verts[6].y;
	line.z2 = model.verts[6].z;

	lineDwg.push_back(line);

	// back faces
	// polygon 8
	poly.a = 3;
	poly.b = 5;
	poly.c = 1;

	model.polys.push_back(poly);
	calculateFaceNormal(8, FALSE);

	// polygon 9
	poly.a = 1;
	poly.b = 5;
	poly.c = 7;

	model.polys.push_back(poly);
	calculateFaceNormal(9, FALSE);

	// check for hood thickness
	// if it exists create a hood
	if(hoodThickness > 0.0f)
	{
		// front faces - box bottom
		// vertex 8
		vert.x = topWidth - hoodThickness;
		vert.y = height - hoodThickness;
		vert.z = 0.0f;
		model.verts.push_back(vert);

		//vertex 9
		vert.x = (topWidth - hoodThickness)* -1;
		vert.y = height - hoodThickness;
		vert.z = 0.0f;
		model.verts.push_back(vert);

		// line segment (8,9)
		line.x1 = model.verts[8].x-0.01f;
		line.y1 = model.verts[8].y-0.01f;
		line.z1 = model.verts[8].z-0.01f;

		line.x2 = model.verts[9].x+0.01f;
		line.y2 = model.verts[9].y-0.01f;
		line.z2 = model.verts[9].z-0.01f;

		lineDwg.push_back(line);

		//vertex 10
		vert.x = (botWidth - hoodThickness) * -1;
		vert.y = (height - hoodThickness) * -1;
		vert.z = 0.0f;
		model.verts.push_back(vert);

		// line segment (9,10)
		line.x1 = model.verts[9].x+0.01f;
		line.y1 = model.verts[9].y-0.01f;
		line.z1 = model.verts[9].z-0.01f;

		line.x2 = model.verts[10].x+0.01f;
		line.y2 = model.verts[10].y+0.01f;
		line.z2 = model.verts[10].z-0.01f;

		lineDwg.push_back(line);

		//tubesheet front face(box bottom)
		// polygon 10
		poly.a = 10;
		poly.b = 9;
		poly.c = 8;

		model.polys.push_back(poly);
		frontFaces.push_back(10);
		calculateFaceNormal(10, FALSE);
	
		//vertex 11
		vert.x = (botWidth - hoodThickness);
		vert.y = (height - hoodThickness) * -1;
		vert.z = 0.0f;
		model.verts.push_back(vert);

		// polygon 11
		poly.a = 8;
		poly.b = 11;
		poly.c = 10;

		model.polys.push_back(poly);
		frontFaces.push_back(11);
		calculateFaceNormal(11, FALSE);


		// line segment (8,11)
		line.x1 = model.verts[8].x-0.01f;
		line.y1 = model.verts[8].y-0.01f;
		line.z1 = model.verts[8].z-0.01f;

		line.x2 = model.verts[11].x-0.01f;
		line.y2 = model.verts[11].y+0.01f;
		line.z2 = model.verts[11].z-0.01f;

		lineDwg.push_back(line);

		// line segment (10,11)
		line.x1 = model.verts[10].x+0.01f;
		line.y1 = model.verts[10].y+0.01f;
		line.z1 = model.verts[10].z-0.01f;

		line.x2 = model.verts[11].x-0.01f;
		line.y2 = model.verts[11].y+0.01f;
		line.z2 = model.verts[11].z-0.01f;

		lineDwg.push_back(line);

		// top ring of the box
		//vertex 12
		vert.x = (topWidth - hoodThickness);
		vert.y = (height - hoodThickness);
		vert.z = hoodDepth * -1;

		model.verts.push_back(vert);

		// polygon 12
		poly.a = 0;
		poly.b = 12;
		poly.c = 2;

		model.polys.push_back(poly);
		calculateFaceNormal(12, FALSE);

		// line segment (12,8)
		line.x1 = model.verts[12].x-0.01f;
		line.y1 = model.verts[12].y-0.01f;
		line.z1 = model.verts[12].z;

		line.x2 = model.verts[8].x-0.01f;
		line.y2 = model.verts[8].y-0.01f;
		line.z2 = model.verts[8].z-0.01f;

		lineDwg.push_back(line);

		//vertex 13
		vert.x = (topWidth - hoodThickness) * -1;
		vert.y = (height - hoodThickness);
		vert.z = hoodDepth * -1;

		model.verts.push_back(vert);

		// polygon 13
		poly.a = 12;
		poly.b = 13;
		poly.c = 2;

		model.polys.push_back(poly);
		calculateFaceNormal(13, FALSE);

		// line segment (13,9)
		line.x1 = model.verts[13].x+0.01f;
		line.y1 = model.verts[13].y-0.01f;
		line.z1 = model.verts[13].z;

		line.x2 = model.verts[9].x+0.01f;
		line.y2 = model.verts[9].y-0.01f;
		line.z2 = model.verts[9].z-0.01f;

		lineDwg.push_back(line);

		// line segment (12,13)
		line.x1 = model.verts[12].x-0.01f;
		line.y1 = model.verts[12].y-0.01f;
		line.z1 = model.verts[12].z;

		line.x2 = model.verts[13].x+0.01f;
		line.y2 = model.verts[13].y-0.01f;
		line.z2 = model.verts[13].z;

		lineDwg.push_back(line);

		// box ring right side
		//vertex 14
		vert.x = (botWidth - hoodThickness) * -1;
		vert.y = (height - hoodThickness) * -1;
		vert.z = hoodDepth * -1;

		model.verts.push_back(vert);

		// polygon 14
		poly.a = 13;
		poly.b = 14;
		poly.c = 2;

		model.polys.push_back(poly);
		calculateFaceNormal(14, FALSE);

		// polygon 15
		poly.a = 2;
		poly.b = 14;
		poly.c = 4;

		model.polys.push_back(poly);
		calculateFaceNormal(15, FALSE);

		// line segment (14,10)
		line.x1 = model.verts[14].x+0.01f;
		line.y1 = model.verts[14].y+0.01f;
		line.z1 = model.verts[14].z;

		line.x2 = model.verts[10].x+0.01f;
		line.y2 = model.verts[10].y+0.01f;
		line.z2 = model.verts[10].z-0.01f;
		lineDwg.push_back(line);

		// line segment (13,14)
		line.x1 = model.verts[13].x+0.01f;
		line.y1 = model.verts[13].y-0.01f;
		line.z1 = model.verts[13].z;

		line.x2 = model.verts[14].x+0.01f;
		line.y2 = model.verts[14].y+0.01f;
		line.z2 = model.verts[14].z;
		lineDwg.push_back(line);

		// box ring bottom
		//vertex 15
		vert.x = (botWidth - hoodThickness);
		vert.y = (height - hoodThickness) * -1;
		vert.z = hoodDepth * -1;

		model.verts.push_back(vert);

		// polygon 16
		poly.a = 14;
		poly.b = 15;
		poly.c = 4;

		model.polys.push_back(poly);
		calculateFaceNormal(16, FALSE);

		// polygon 17
		poly.a = 15;
		poly.b = 6;
		poly.c = 4;

		model.polys.push_back(poly);
		calculateFaceNormal(17, FALSE);

		// line segment (15,11)
		line.x1 = model.verts[15].x-0.01f;
		line.y1 = model.verts[15].y+0.01f;
		line.z1 = model.verts[15].z;

		line.x2 = model.verts[11].x-0.01f;
		line.y2 = model.verts[11].y+0.01f;
		line.z2 = model.verts[11].z-0.01f;
		lineDwg.push_back(line);

		// line segment (14,15)
		line.x1 = model.verts[14].x+0.01f;
		line.y1 = model.verts[14].y+0.01f;
		line.z1 = model.verts[14].z;

		line.x2 = model.verts[15].x-0.01f;
		line.y2 = model.verts[15].y+0.01f;
		line.z2 = model.verts[15].z;
		lineDwg.push_back(line);


		// line segment (12,15)
		line.x1 = model.verts[12].x-0.01f;
		line.y1 = model.verts[12].y-0.01f;
		line.z1 = model.verts[12].z;

		line.x2 = model.verts[15].x-0.01f;
		line.y2 = model.verts[15].y+0.01f;
		line.z2 = model.verts[15].z;
		lineDwg.push_back(line);

		// box ring left side
		// polygon 18
		poly.a = 0;
		poly.b = 6;
		poly.c = 15;

		model.polys.push_back(poly);
		calculateFaceNormal(18, FALSE);

		// polygon 19
		poly.a = 15;
		poly.b = 12;
		poly.c = 0;

		model.polys.push_back(poly);
		calculateFaceNormal(19, FALSE);

		// box inside top
		// polygon 20
		poly.a = 12;
		poly.b = 8;
		poly.c = 9;

		model.polys.push_back(poly);
		calculateFaceNormal(20, FALSE);

		// polygon 21
		poly.a = 9;
		poly.b = 13;
		poly.c = 12;

		model.polys.push_back(poly);
		calculateFaceNormal(21, FALSE);

		// box inside right side
		// polygon 22
		poly.a = 9;
		poly.b = 14;
		poly.c = 13;

		model.polys.push_back(poly);
		calculateFaceNormal(22, FALSE);
		
		// polygon 23
		poly.a = 10;
		poly.b = 14;
		poly.c = 9;

		model.polys.push_back(poly);
		calculateFaceNormal(23, FALSE);

		// box inside bottom
		// polygon 24
		poly.a = 10;
		poly.b = 15;
		poly.c = 14;

		model.polys.push_back(poly);
		calculateFaceNormal(24, FALSE);
		
		// polygon 25
		poly.a = 11;
		poly.b = 15;
		poly.c = 10;

		model.polys.push_back(poly);
		calculateFaceNormal(25, FALSE);

		// box inside left side
		// polygon 26
		poly.a = 8;
		poly.b = 12;
		poly.c = 15;

		model.polys.push_back(poly);
		calculateFaceNormal(26, FALSE);
		
		// polygon 27
		poly.a = 15;
		poly.b = 11;
		poly.c = 8;

		model.polys.push_back(poly);
		calculateFaceNormal(27, FALSE);

	}
	else
	{
		// Front faces
		// polygon 10
		poly.a = 4;
		poly.b = 2;
		poly.c = 0;

		model.polys.push_back(poly);
		calculateFaceNormal(10, FALSE);
		frontFaces.push_back(10);
	
		// polygon 11
		poly.a = 0;
		poly.b = 6;
		poly.c = 4;

		model.polys.push_back(poly);
		calculateFaceNormal(11, FALSE);
		frontFaces.push_back(11);
	}

	// finished calculate normals for box
	normalizeVerts();
}

void meshModels::cachePipe(RECTSHELLPROPERTIES shell)
{
	vector_t radiusInner[32];
	vector_t radiusOuter[32];
	vector_t radiusFrontFlange[32];
	vector_t radiusRearFlange[32];

	const double M_PI = 3.14159265;
	double minorStep = 2.0f * M_PI / 32;
	int j;
	int vCountA, vCountB, start, firstVert, polyCount;
	
	float bodyStart = shell.z0;
	float bodyEnd = shell.z0 + shell.length;

	// we should build our vertex arrays first
	for(j = 0; j < numSides; j++)
	{
		vector_t vertex;
		vertex.z = shell.z0;

		double a = j * minorStep;
		vertex.x = (float)(shell.radiusInner * cos(a));
		vertex.y = (float)(shell.radiusInner * sin(a));
		radiusInner[j] = vertex;

		vertex.x = (float)(shell.radiusOuter * cos(a));
		vertex.y = (float)(shell.radiusOuter * sin(a));
		radiusOuter[j] = vertex;

		if(shell.frontFlange == TRUE)
		{
			vertex.x = (float)(shell.radiusfFlange * cos(a));
			vertex.y = (float)(shell.radiusfFlange * sin(a));
			radiusFrontFlange[j] = vertex;
		}
		if(shell.rearFlange == TRUE)
		{
			vertex.x = (float)(shell.radiusrFlange * cos(a));
			vertex.y = (float)(shell.radiusrFlange * sin(a));
			radiusRearFlange[j] = vertex;
		}
	}

	//now we are ready to start caching the geometry
	dir = TRUE;
	firstVert = 1;
	// front face of pipe
	if(shell.frontFlange == TRUE)
	{
		cacheCylinder2R( radiusInner, radiusFrontFlange, shell.z0, shell.z0);
	}
	else
	{

		cacheCylinder2R( radiusInner, radiusOuter, shell.z0, shell.z0);
	}
	vCountA = model.verts.size();
	start = vCountA;

	if(shell.frontFlange == TRUE)
	{
		// front flange back face
		cacheCylinder2R( radiusFrontFlange, radiusOuter, shell.z0 + shell.fFlangeThickness, shell.z0 + shell.fFlangeThickness);
		polyCount = model.polys.size();
		vCountB = model.verts.size();
		cacheOnExisting( firstVert, vCountA, vCountB);
		model.polys.resize(polyCount);
		firstVert = vCountA;
		vCountA = vCountB;
	}

	if(shell.rearFlange == TRUE)
	{
		bodyEnd =  shell.z0 + (shell.length - shell.rFlangeThickness);
		// rear flange back face
		cacheCylinder2R(radiusOuter, radiusRearFlange, bodyEnd, bodyEnd);
		polyCount = model.polys.size();
		vCountB = model.verts.size();
		cacheOnExisting( firstVert+1, vCountA, vCountB);
		model.polys.resize(polyCount);
		firstVert = vCountA;
		vCountA = vCountB;

		// rear face of pipe
		cacheCylinder2R(radiusRearFlange, radiusInner, shell.z0 + shell.length, shell.z0 + shell.length);
	}
	else
	{
		// rear face of pipe
		cacheCylinder2R(radiusOuter, radiusInner, shell.z0 + shell.length, shell.z0 + shell.length);
	}
	polyCount = model.polys.size();
	vCountB = model.verts.size();
	cacheOnExisting( firstVert+1, vCountA, vCountB);
	model.polys.resize(polyCount);
	firstVert = vCountA;
	vCountA = vCountB;

	// now generate normals for the inside pipe

	// cylindrical geometries, all face outward
	dir = FALSE;
	if(shell.frontFlange == TRUE)
	{
		// front flange 
		cacheCylinder2R(radiusFrontFlange, radiusFrontFlange, shell.z0 + shell.fFlangeThickness,  shell.z0);
		bodyStart =  shell.z0 + shell.fFlangeThickness;
	}
	if(shell.rearFlange == TRUE)
	{
		// rear flange
		cacheCylinder2R(radiusRearFlange, radiusRearFlange, shell.z0 + shell.length, bodyEnd);
	}
	
	// pipe body
	cacheCylinder2R(radiusOuter, radiusOuter, bodyEnd,  bodyStart);	
	// inside of pipe
	//cacheCylinder2R(radiusInner, radiusInner, shell.z0, shell.z0 + shell.length);

	// finished calculate normals for box
	normalizeVerts();
}

void meshModels::cacheNozzle(RECTNOZZLEDATA nozz)
{
	vector_t radiusOutNozz[32];
	vector_t radiusInNozz[32];
	vector_t radiusFlange[32];

	const double M_PI = 3.14159265;
	double minorStep = 2.0f * M_PI / 32;
	int j;

	float inRadius = nozz.radius - (nozz.radius * 0.1f);
	float fRadius  = nozz.radius + (nozz.radius * 0.3f);

	// we should build our vertex arrays first
	for(j = 0; j < numSides; j++)
	{
		vector_t vertex;
		vertex.z = 0.0f;

		double a = j * minorStep;
		vertex.x = (float)(nozz.radius * cos(a));
		vertex.y = (float)(nozz.radius * sin(a));
		radiusOutNozz[j] = vertex;

		vertex.x = (float)(inRadius * cos(a));
		vertex.y = (float)(inRadius * sin(a));
		radiusInNozz[j] = vertex;

		vertex.x = (float)(fRadius * cos(a));
		vertex.y = (float)(fRadius * sin(a));
		radiusFlange[j] = vertex;
	}

	// determine the type of nozzle here and build accordingly
	if(nozz.NozzleType == 0) // no flange straight tube
	{
		dir = FALSE;
		cacheCylinder2R( radiusOutNozz, radiusOutNozz, nozz.length, 0.0f);
		cacheCylinder2R( radiusInNozz,  radiusOutNozz,  nozz.length, nozz.length);
		cacheCylinder2R( radiusInNozz,  radiusInNozz, 0.0f, nozz.length);
		cacheCylinder2R( radiusOutNozz, radiusInNozz, 0.0f, 0.0f);
	}
	if(nozz.NozzleType == 1 || nozz.NozzleType == 2) // flanged pipe
	{
		dir = FALSE;
		float length = nozz.length - (nozz.length * 0.2f);
		cacheCylinder2R( radiusOutNozz, radiusOutNozz, length, 0.0f);
		cacheCylinder2R( radiusFlange,  radiusOutNozz, length, length);
		cacheCylinder2R( radiusFlange,  radiusFlange, nozz.length, length);
		cacheCylinder2R( radiusInNozz,  radiusFlange, nozz.length, nozz.length);
		cacheCylinder2R( radiusInNozz,  radiusInNozz, 0.0f, nozz.length);
		cacheCylinder2R( radiusOutNozz, radiusInNozz, 0.0f, 0.0f);
	
		if(nozz.NozzleType == 2) // flanged pipe with a round circular cover
		{

			length = length + ( nozz.length * 0.4f);
			// now build the cover
			cacheCylinder2R( radiusFlange, radiusFlange, length, nozz.length + 0.03f);
			cacheCylinderCapV(radiusFlange, nozz.length + 0.03f);
			dir = TRUE;
			cacheCylinderCapV(radiusFlange, length);
		}
	}
	if(nozz.NozzleType == 3) // spherical ended pipe
	{

		int firstVert = cacheSphericalCap(nozz.radius, nozz.length, nozz.length * 0.1f);
		int vCountA = model.verts.size();			

		cacheCylinder2R( radiusOutNozz, radiusOutNozz, nozz.length, 0.0f);
		int vCountB = model.verts.size();

		cacheOnExisting( firstVert, vCountA, vCountB);
	}
	if(nozz.NozzleType == 4) // tube with a flat stopper plate inside
	{
		float length = nozz.length - (nozz.length * 0.2f);
		
		cacheCylinder2R( radiusOutNozz, radiusOutNozz, nozz.length,  0.0f);
		cacheCylinder2R( radiusInNozz,  radiusOutNozz, nozz.length, nozz.length);
		cacheCylinder2R( radiusInNozz,  radiusInNozz, length, nozz.length);
		cacheCylinderCapV(radiusInNozz, length);
		cacheCylinderCapV(radiusInNozz, length - 0.1f);
		cacheCylinder2R( radiusInNozz,  radiusInNozz, 0.0f,  length);
		cacheCylinder2R( radiusOutNozz, radiusInNozz, 0.0f, 0.0f);	
	}
	
	// I'm not really sure how to do the clipping. but I would Imagine that it would have to happen here
	// perhaps it could be something that I generate in the circular array I use to build the cylinders
	// which would require a special routine

	normalizeVerts();
}

void meshModels::cacheImp(RECTNOZZLEDATA nozz)
{
	// caches the different types of impingement plates

	if(nozz.impType == 0)// no impingement plate
	{
		// do nothing 
	}
	else if(nozz.impType == 1)// round impingement plate
	{
		vector_t radiusPts[32];

		const double M_PI = 3.14159265;
		double phi = 2.0f * M_PI / 32;
		int j;

		for(j = 0; j < numSides; j++)
		{
			vector_t vertex;
			vertex.z = 0.0f;

			double a = j * phi;

			vertex.x = (float)(nozz.impRadiusLat * cos(a));
			vertex.y = (float)(nozz.impRadiusLat * sin(a));
			radiusPts[j] = vertex;
		}
		
		cacheCylinderCapV(radiusPts, 0.0f);
		cacheCylinderCapV(radiusPts, nozz.impThickness);
		cacheCylinder2R( radiusPts, radiusPts, 0.0f, nozz.impThickness);

		normalizeVerts();
	}
	else if(nozz.impType == 2)// rectangular impingement plate
	{
		cacheBox( nozz.impRadiusLat, nozz.impRadiusLat, nozz.impRadiusLong, nozz.impThickness, 0.0f, 0.0f);
	}
}

void meshModels::cacheOnExisting(int start, int vCountA, int vCountB)
{
	int j;
	int vA, vB, vC;
	
	polygon_t poly;

	//generate the flange face 
	vA = start;
	vB = vA + 2;
	vC = vCountA + 2;

	for( j = 0; j < numSides; j++)
	{
		//record all three points as a triangle in our structure
		poly.a = vC;
		poly.b = vB;
		poly.c = vA;
	
		model.polys.push_back(poly);

		int faceCount = model.polys.size()-1;
		calculateFaceNormal(faceCount, dir);

		vA = vB;
		vB = vB + 2;
		vC = vC + 2;
		if(vB >= vCountA)
			vB = start;
		if(vC >= vCountB)
			vC = vCountA;
	}

	vA = vCountA + 2;
	vB = vA + 2;
	vC = start + 2;
	
	for( j = 0; j < numSides; j++)
	{
		//record all three points as a triangle in our structure
		poly.a = vA;
		poly.b = vB;
		poly.c = vC;
	
		model.polys.push_back(poly);

		int faceCount = model.polys.size()-1;
		calculateFaceNormal(faceCount, dir);

		vA = vB;
		vB = vB + 2;
		vC = vC + 2;
		if(vB >= vCountB)
			vB = vCountA;
		if(vC >= vCountA)
			vC = start;
	}
}

int meshModels::cacheSphericalCap(float radius, float zStart, float headRatio)
{
		// now we generate the spherical cap
	// using the following formula
	// x = radius * cos(phi)

	const double M_PI = 3.14159265;
	float minorStep = 2.0f * (float)M_PI / numSides; // this is phi
	int j;
	int vCountA, vCountB, firstVert;

	float bump = 0.1f;	// keeps the model from exploding around the cap

	vector_t radiusA[32];
	vector_t radiusB[32];

	float Theta  = (float)M_PI * 0 / numSides;
	float Theta2 = (float)M_PI * 1 / numSides;

	for(j = 0; j < numSides; j++)
	{
		float phi  = 2.0f * (float)M_PI * j / numSides;
		float phi2 = 2.0f * (float)M_PI * (j+1) / numSides;

		vector_t vertex;
		vertex.z = 0.0f;

		float a = j * minorStep;
		vertex.x = (float)radius * sin(Theta)*cos(phi);
		vertex.y = (float)radius * sin(Theta)*sin(phi);
		radiusA[j] = vertex;

		vertex.x = (float)radius * sin(Theta2) * cos(phi2);
		vertex.y = (float)radius * sin(Theta2) * sin(phi2);
		radiusB[j] = vertex;
	}

	dir = FALSE;
	firstVert = model.verts.size()+1;
	cacheCylinder2R(radiusA, radiusB, (zStart + bump) + (headRatio * cos(Theta)), (zStart + bump) + (headRatio * cos(Theta2)));
	vCountA = model.verts.size();
	
	// then the rest is a collection of cacheCylinder2R calls
	// followed by a cache on existing call to marry the cylinder / cap code
	// this should be done on a loop - half of numSides times

	dir = FALSE;

	int i;
	for(i=2; i <= 14; i = i + 2)
	{

		Theta  = (float)M_PI * i / numSides;
		Theta2 = (float)M_PI *(i+1) / numSides;

		for(j = 0; j < numSides; j++)
		{
			float phi  = 2.0f * (float)M_PI * j / numSides;
			float phi2 = 2.0f * (float)M_PI * (j+1) / numSides;

			vector_t vertex;
			vertex.z = 0.0f;

			float a = j * minorStep;
			vertex.x = (float)radius * sin(Theta)*cos(phi);
			vertex.y = (float)radius * sin(Theta)*sin(phi);
			radiusA[j] = vertex;

			vertex.x = (float)radius * sin(Theta2) * cos(phi2);
			vertex.y = (float)radius * sin(Theta2) * sin(phi2);
			radiusB[j] = vertex;
		}
		cacheCylinder2R(radiusA, radiusB, (zStart + bump) + (headRatio * cos(Theta)), (zStart + bump) + (headRatio * cos(Theta2)));
		vCountB = model.verts.size();
		cacheOnExisting( firstVert, vCountA, vCountB);
		firstVert = vCountA + 1;
		vCountA = vCountB;	
	}

	return firstVert;
}

void meshModels::cacheCylinder2R(vector_t ring1[], vector_t ring2[], float z0, float z1)
{
	// objects to build our vertices in
	vertex_t vertA, vertB, vertC;
	polygon_t poly;
	int i = 0;
	int j;
	int faceCount;

	// find out how many verts we have
	int vcount = model.verts.size();
	int endGon = vcount; // and saving the starting vertice index for later

	//record all three points as a triangle in our structure
	poly.a = vcount;
	poly.b = vcount +1;
	poly.c = vcount +2;

	// we need to start with two verts because then 
	//we only need to add 2 verts to get 2 new triangles in the for loop

	// build our first vertex
	vertA.x = ring1[i].x;
	vertA.y = ring1[i].y;
	vertA.z = z0;
	// push it into our array
	model.verts.reserve(poly.a);
	model.verts.push_back(vertA);

	//build our third vertex
	vertB.x = ring2[i].x;
	vertB.y = ring2[i].y;
	vertB.z = z1;
	// push it into our array
	model.verts.reserve(poly.b);
	model.verts.push_back(vertB);
	
	i++;

	int size = (numSides * 2 ) - 2;
	// build a cylinder 2 triangles at a time
	for( j = 0; j < size; ++j)
	{
		// ensure our array iterator is not too large
		if(i>=numSides)
			i = 0;

		//record all three points as a triangle in our structure
		poly.a = vcount;
		poly.b = vcount +1;
		poly.c = vcount +2;

		//build our second vertex
		vertC.x = ring1[i].x;
		vertC.y = ring1[i].y;
		vertC.z = z0;
		// push it into our array
		model.verts.push_back(vertC);

		// record the triangle
		model.polys.push_back(poly);

		faceCount = model.polys.size()-1;

		calculateFaceNormal(faceCount, dir);

		j++; // advance to the next polygon
	
		// vertex A is really the old vertex b so lets record the change here
		poly.a = poly.b;

		vertB.x = ring2[i].x;
		vertB.y = ring2[i].y;
		vertB.z = z1;
		poly.b = model.verts.size(); // set the iterator for the new vertex
		model.verts.reserve(poly.b);
		model.verts.push_back(vertB);

		// VertC is the same and remains unchanged
		// lets store our triangle
		model.polys.push_back(poly);

		faceCount = model.polys.size()-1;

		calculateFaceNormal(faceCount, dir);

		vcount = vcount +2;
		i++;
	}

	// add our last two triangles using existing verts
	poly.a = model.verts.size() - 2;
	poly.b = model.verts.size() - 1;
	poly.c = endGon;

	model.polys.push_back(poly);

	faceCount = model.polys.size()-1;
	calculateFaceNormal(faceCount, dir);

	// add our last two triangles using existing verts
	poly.a = model.verts.size() - 1;
	poly.b = endGon + 1;
	poly.c = endGon;

	model.polys.push_back(poly);

	faceCount = model.polys.size()-1;
	calculateFaceNormal(faceCount, dir);

}

void meshModels::cacheCylinderCapV( vector_t radiusPts[], float depth)
{
	// I'm going to attempt to build the tubesheet / baffle from a circle diagram

	vertex_t vertA, vertB, vertC;
	polygon_t poly;
	lineSegmen_t line;

	int j = 0;
	int vCount = model.verts.size();
	int pCount = model.polys.size();
		
	poly.a = vCount; // define the first point of our triangle

	// establish center point
	// this should be calculated from bounds
	vertA.x= centerX;
	vertA.y = centerY;
	vertA.z = depth;
	model.verts.reserve(vCount);
	model.verts.push_back(vertA);

	vCount = model.verts.size()+1;
	int endGon = vCount;	// saving this vert id for later

	// establish the second triangle point along the perimiter
	vertB.x = radiusPts[j].x;
	vertB.y = radiusPts[j].y;
	vertB.z = depth;
	model.verts.reserve(vCount);
	model.verts.push_back(vertB);
	line.x1 = vertB.x;
	line.y1 = vertB.y;
	line.z1 = vertB.z;

	// creating numSides polygons for the end cap
	for(j = 1; j < numSides; ++j)
	{
		vCount = model.verts.size();
		pCount = model.polys.size();
		poly.b = vCount;
		poly.c = vCount - 1;

		//establish the final triangle point along the perimiter
		vertC.x = radiusPts[j].x;
		vertC.y = radiusPts[j].y;
		vertC.z = depth;
		model.verts.reserve(vCount+1);
		model.verts.push_back(vertC);
			
		model.polys.reserve(pCount + 1);
		model.polys.push_back(poly);

		calculateFaceNormal(pCount, dir);
		line.x2 = vertC.x;
		line.y2 = vertC.y;
		line.z2 = vertC.z;

		lineDwg.push_back(line);

		line.x1 = line.x2;
		line.y1 = line.y2;
		line.z1 = line.z2;
	}
		
	vCount = model.verts.size();
	pCount = model.polys.size();
	poly.b = endGon - 1;
	poly.c = vCount - 1;

	model.polys.reserve(pCount + 1);
	model.polys.push_back(poly);

	calculateFaceNormal(pCount, dir);

	line.x2 = vertB.x;
	line.y2 = vertB.y;
	line.z2 = vertB.z;
	lineDwg.push_back(line);
}

int meshModels::getSize()
{
	return model.polys.size();
}

vector_t meshModels::getPoly(int i, int p)
{
	// this function returns the xyz coordinates of the 
	// vertex p located at face i

	vector_t vertex;
	int v;

	if(p == 0)
		v = model.polys[i].a;
	else if(p==1)
		v = model.polys[i].b;
	else if(p==2)
		v = model.polys[i].c;

	vertex.x = model.verts[v].x;
	vertex.y = model.verts[v].y;	
	vertex.z = model.verts[v].z;	

	return vertex;
}

void meshModels::DrawPolygon()
{
	if(smooth == TRUE)
	{
		glShadeModel(GL_SMOOTH);
	}
	else
		glShadeModel(GL_FLAT);

	if(debug == TRUE)
	{
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
	}
	if(transparent == TRUE)
	{
		glEnable(GL_BLEND);				// Turn Blending On
		glDisable(GL_DEPTH_TEST);		// Turn Depth Testing Off
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // this function controls how the blend is mixed
	}

	int j;
	int polys = model.polys.size();

	glPushMatrix();
  
	glPolygonMode(GL_FRONT,GL_FILL);

	glBegin(GL_TRIANGLES);
	for (j = 0; j < polys; j++) 
	{
		int a = model.polys[j].a;
		int b = model.polys[j].b;
		int c = model.polys[j].c;

		glNormal3f( model.verts[a].vNrml.x, model.verts[a].vNrml.y, model.verts[a].vNrml.z);
		glVertex3f( model.verts[a].x, model.verts[a].y, model.verts[a].z);

		glNormal3f( model.verts[b].vNrml.x, model.verts[b].vNrml.y, model.verts[b].vNrml.z);
		glVertex3f( model.verts[b].x, model.verts[b].y, model.verts[b].z);

		glNormal3f( model.verts[c].vNrml.x, model.verts[c].vNrml.y, model.verts[c].vNrml.z);
		glVertex3f( model.verts[c].x, model.verts[c].y, model.verts[c].z);

	}
	glEnd();
	glPopMatrix();

	if(transparent == TRUE)
	{
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On
	}
	if(debug == TRUE)
	{
		glDisable(GL_CULL_FACE);
		DrawNormals();
	}

}//End meshModels::DrawPolygon()

void meshModels::DrawLine()
{
	int j;

	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT,GL_LINE);

	glLineWidth(lineWidth);
	// this should come automatically from our material
	// and possibly be configurable in the future
	glColor4ub( 0, 0, 0, 255);
	glPushMatrix();

	int size = (int)lineDwg.size();
	glBegin(GL_LINES);
		for (j = 0; j < size; ++j)  
		{
		
			glVertex3f(lineDwg[j].x1, lineDwg[j].y1, lineDwg[j].z1);
			glVertex3f(lineDwg[j].x2, lineDwg[j].y2, lineDwg[j].z2);

		}
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);
}

void meshModels::DrawNormals()
{
	// this function is for the display of generated surface normals.
	// mostly it will help me debug the order in which I am putting together my model.

	int j;
	int polys = model.polys.size();

	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT,GL_LINE);
	glPushMatrix();

	for (j = 0; j < polys; j++) 
	{
		int a = model.polys[j].a;
		int b = model.polys[j].b;
		int c = model.polys[j].c;

		glBegin(GL_LINES);
		glColor4ub( 0, 155, 30, 255);

		glVertex3f( model.verts[a].x, model.verts[a].y, model.verts[a].z);
		glVertex3f( model.verts[a].vNrml.x + model.verts[a].x,
					model.verts[a].vNrml.y + model.verts[a].y,
					model.verts[a].vNrml.z + model.verts[a].z);

		glEnd();

		glBegin(GL_LINES);

		glVertex3f( model.verts[b].x, model.verts[b].y, model.verts[b].z);
		glVertex3f( model.verts[b].vNrml.x + model.verts[b].x,
					model.verts[b].vNrml.y + model.verts[b].y,
					model.verts[b].vNrml.z + model.verts[b].z);

		glEnd();
		
		glBegin(GL_LINES);

		glVertex3f( model.verts[c].x, model.verts[c].y, model.verts[c].z);
		glVertex3f( model.verts[c].vNrml.x + model.verts[c].x,
					model.verts[c].vNrml.y + model.verts[c].y,
					model.verts[c].vNrml.z + model.verts[c].z);
		glEnd();

	}
	glEnd();

	glPopMatrix();
	glEnable(GL_LIGHTING);

//End meshModels::DrawNormal()
}