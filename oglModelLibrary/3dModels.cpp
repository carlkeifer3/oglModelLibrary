// 3dModels.cpp - library for building 3d objects

#include <iostream>
#include <math.h>

#define GLEW_STATIC
#include <GL\glew.h>
#include "3dModels.h"
#include "Macros.h"
#include "GLShader.hpp"

#define GLM_FORCE_RADIANS
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

#ifndef bool
    #define bool int
    #define FALSE ((bool)0)
    #define TRUE  ((bool)1)
#endif

meshModels::meshModels()
{
	debug = FALSE;			// Turn visual debugging off by default
	smooth = TRUE;			// turn smooth shading on by default
	transparent = FALSE;	// turn transparency off by default
	
	opacity = 255;			// default opacity set to 100%

	dir = FALSE;

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
void meshModels::calculateFaceNormal (int poly)
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
void meshModels::normalizeVector (vector_t *v)
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
}//End  meshModels::vertCheck(vertex_t v)

void meshModels::clearModel()
{
	// clear the array of faces and points
	model.polys.clear();
	model.verts.clear();

	// empty out our data containers too
	frontFaces.clear();
	lineDwg.clear();
}//End meshModels::clearModel()

void meshModels::cacheCylinder(float vertsX[], float vertsY[], float depth)
{
	dir = TRUE;
	cacheEndCap( vertsX, vertsY, 0.0f);
	dir = FALSE;
	cacheEndCap( vertsX, vertsY, depth);
	dir = FALSE;

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
	vertA.x = vertsX[i];
	vertA.y = vertsY[i];
	vertA.z = 0.0f;
	// push it into our array
	model.verts.reserve(poly.a);
	model.verts.push_back(vertA);

	//build our third vertex
	vertB.x = vertsX[i];
	vertB.y = vertsY[i];
	vertB.z = depth;
	// push it into our array
	model.verts.reserve(poly.b);
	model.verts.push_back(vertB);
	
	i++;

	// build a cylinder 2 triangles at a time
	for( j = 0; j < 62; ++j)
	{
		// ensure our array iterator is not too large
		if(i>=32)
			i = 0;

		//record all three points as a triangle in our structure
		poly.a = vcount;
		poly.b = vcount +1;
		poly.c = vcount +2;

		//build our second vertex
		vertC.x = vertsX[i];
		vertC.y = vertsY[i];
		vertC.z = 0.0f;
		// push it into our array
		model.verts.reserve(poly.c);
		model.verts.push_back(vertC);


		// record the triangle
		model.polys.reserve(j);
		model.polys.push_back(poly);

		faceCount = model.polys.size()-1;

		calculateFaceNormal(faceCount);

		j++; // advance to the next polygon
	
		// vertex A is really the old vertex b so lets record the change here
		poly.a = poly.b;

		vertB.x = vertsX[i];
		vertB.y = vertsY[i];
		vertB.z = depth;
		poly.b = model.verts.size(); // set the iterator for the new vertex
		model.verts.reserve(poly.b);
		model.verts.push_back(vertB);

		// VertC is the same and remains unchanged
		// lets store our triangle
		model.polys.reserve(j);
		model.polys.push_back(poly);

		faceCount = model.polys.size()-1;

		calculateFaceNormal(faceCount);

		vcount = vcount +2;
		i++;
	}

	// add our last two triangles using existing verts
	poly.a = model.verts.size() - 2;
	poly.b = model.verts.size() - 1;
	poly.c = endGon;

	model.polys.reserve(vcount);
	model.polys.push_back(poly);

	faceCount = model.polys.size()-1;
	calculateFaceNormal(faceCount);

	// add our last two triangles using existing verts
	poly.a = model.verts.size() - 1;
	poly.b = endGon + 1;
	poly.c = endGon;

	model.polys.reserve(vcount + 1);
	model.polys.push_back(poly);

	faceCount = model.polys.size()-1;
	calculateFaceNormal(faceCount);

	normalizeVerts();

	makeShader();
	makeBuffer();
}

void meshModels::cacheEndCap(float vertsX[], float vertsY[], float depth)
{
		vertex_t vertA, vertB, vertC;
		polygon_t poly;

		int j = 0;
		int vCount = model.verts.size();
		int pCount = model.polys.size();

		poly.a = vCount; // define the first point of our triangle
		poly.b = vCount + 1; // store initial values that should be correct
		poly.c = vCount + 2;

		// establish center point
		// this should be calculated from bounds
		vertA.x= 0.0f;
		vertA.y = 0.0f;
		vertA.z = depth;
		//model.verts.reserve(0);
		model.verts.push_back(vertA);

		vCount = model.verts.size()+1;
		int endGon = vCount;	// saving this vert id for later

		// establish the second triangle point along the perimiter
		vertB.x = vertsX[j];
		vertB.y = vertsY[j];
		vertB.z = depth;
		//model.verts.reserve(vCount);
		model.verts.push_back(vertB);

		// creating 32 polygons for the end cap
		for(j = 1; j < 32; ++j)
		{
			vCount = model.verts.size();
			pCount = model.polys.size();

			//establish the final triangle point along the perimiter
			vertC.x = vertsX[j];
			vertC.y = vertsY[j];
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
				calculateFaceNormal(pCount);

			}else{
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
					calculateFaceNormal(pCount);
				}
			}
		}
		
		vCount = model.verts.size();
		pCount = model.polys.size();
		poly.b = endGon - 1;
		poly.c = vCount - 1;

		model.polys.reserve(pCount + 1);
		model.polys.push_back(poly);

		calculateFaceNormal(pCount);

}

void meshModels::makeShader()
{
	sID = LoadShader("shader.vert", "shader.frag");
	glUseProgram(sID);

	positionID = glGetAttribLocation(sID, "vVertex");
    normalID   = glGetAttribLocation(sID, "vNormal");
	
	diffColorID = glGetUniformLocation(sID, "diffuseColor");
	locLight = glGetUniformLocation(sID, "vLightPosition");

    // Get uniform locations
    mvpID = glGetUniformLocation(sID, "mvpMatrix");
    mvID = glGetUniformLocation(sID, "mvMatrix");
    nmID = glGetUniformLocation(sID, "normalMatrix");

	glUseProgram(0);
}

void meshModels::makeBuffer()
{
	GENVERTEXARRAYS(1, &arrayID);
	BINDVERTEXARRAY(arrayID);

	glGenBuffers(1, &bufferID);
	glBindBuffer( GL_ARRAY_BUFFER, bufferID);
	glBufferData( GL_ARRAY_BUFFER, model.verts.size() * sizeof(vertex_t), model.verts.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &elementID);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, elementID);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, model.polys.size() * sizeof(polygon_t), model.polys.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(positionID);
	glVertexAttribPointer( positionID, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), 0);
	
	glEnableVertexAttribArray(normalID);
	glVertexAttribPointer( normalID, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(float)*3));

	BINDVERTEXARRAY(0);
	glBindBuffer( GL_ARRAY_BUFFER, 0);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);
}

void meshModels::DrawPolygon(glm::mat4 view, glm::mat4 proj)
{
	static float time = 0.0;
	time += 0.01f;

	GLfloat vEyeLight[] = { 100.0f, 100.0f, 100.0f};
	GLfloat vDiffuseColor[] = {0.5f, 0.5f, 1.0f, 1.0f};

    glm::mat4 modelT = glm::rotate( glm::mat4(), time, glm::vec3(0,1,0) );
	glm::mat4 mvpMat = proj * view;// * modelT;
	glm::mat4 mvMat  = view;// * modelT;

	glm::mat3 nmMat  = glm::transpose(glm::inverse(glm::mat3(mvMat)));

	glUseProgram(sID);
	BINDVERTEXARRAY(arrayID);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, elementID);

	glUniform4fv(diffColorID, 1, vDiffuseColor);
	glUniform3fv(locLight, 1, vEyeLight);
	glUniformMatrix4fv( mvpID, 1, GL_FALSE, glm::value_ptr(mvpMat) );
	glUniformMatrix4fv( mvID, 1, GL_FALSE, glm::value_ptr(mvMat) );
	glUniformMatrix3fv( nmID, 1, GL_FALSE, glm::value_ptr(nmMat) ); 

	glDrawElements(GL_TRIANGLES, model.polys.size() * sizeof(polygon_t), GL_UNSIGNED_INT, 0);

	BINDVERTEXARRAY(0);
	glUseProgram(0);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);
}//End meshModels::DrawPolygon()

/*void meshModels::DrawNormals()
{
	// this function is for the display of generated surface normals.
	// mostly it will help me debug the order in which I am putting together my model.

	int j;
	int polys = model.polys.size();

	glPushMatrix();

	for (j = 0; j < polys; j++) 
	{
		int a = model.polys[j].a;
		int b = model.polys[j].b;
		int c = model.polys[j].c;

		glBegin(GL_LINE_LOOP);
		glColor4ub( 0, 255, 0, 255);

		glVertex3f( model.verts[a].x, model.verts[a].y, model.verts[a].z);
		glVertex3f( model.verts[a].vNrml.x + model.verts[a].x,
					model.verts[a].vNrml.y + model.verts[a].y,
					model.verts[a].vNrml.z + model.verts[a].z);

		glEnd();

		glBegin(GL_LINE_LOOP);

		glVertex3f( model.verts[b].x, model.verts[b].y, model.verts[b].z);
		glVertex3f( model.verts[b].vNrml.x + model.verts[b].x,
					model.verts[b].vNrml.y + model.verts[b].y,
					model.verts[b].vNrml.z + model.verts[b].z);

		glEnd();
		
		glBegin(GL_LINE_LOOP);

		glVertex3f( model.verts[c].x, model.verts[c].y, model.verts[c].z);
		glVertex3f( model.verts[c].vNrml.x + model.verts[c].x,
					model.verts[c].vNrml.y + model.verts[c].y,
					model.verts[c].vNrml.z + model.verts[c].z);
		glEnd();

	}
	glEnd();

	glPopMatrix();
}//End meshModels::DrawNormal()*/