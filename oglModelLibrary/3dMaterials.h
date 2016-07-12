#ifndef __3DMATERIALS_H__
#define __3DMATERIALS_H__

typedef struct _intColorRGBA
{
	int r;
	int g;
	int b;
	int a;
}iColorRGBA;

typedef struct _floatColorRGBA
{
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;
}fColorRGBA;

typedef struct _material
{
	iColorRGBA		diffColor;
	fColorRGBA		ambient;
	fColorRGBA		diffuse;
	fColorRGBA		specular;
	GLfloat			shine;

}material;

class meshMaterials
{
public:

	meshMaterials();
	~meshMaterials();

	void setMaterial(int type);
	void getMaterial();
	int findMaterialId(char string[80]);

private:

	material prop;
};

#endif // __3DMATERIALS_H__