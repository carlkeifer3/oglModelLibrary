
#include "stdafx.h"
#include "3dMaterials.h"


meshMaterials::meshMaterials()
{
	prop.ambient.r = 0.0f;
	prop.ambient.g = 0.0f;
	prop.ambient.b = 0.0f;
	prop.ambient.a = 1.0f;

	prop.diffuse.r = 1.0f;
	prop.diffuse.g = 1.0f;
	prop.diffuse.b = 1.0f;
	prop.diffuse.a = 1.0f;

	prop.specular.a = 0.5f;
	prop.specular.g = 0.5f;
	prop.specular.b = 0.5f;
	prop.specular.a = 1.0f;

	prop.shine = 5.0f;

	prop.diffColor.r = 127;
	prop.diffColor.g = 127;
	prop.diffColor.b = 127;
	prop.diffColor.a = 127;
}

meshMaterials::~meshMaterials()
{
}

void meshMaterials::setMaterial(int type)
{
	
	if(type == 1) // Brass
	{
		prop.ambient.r = 0.029412f;
		prop.ambient.g = 0.023529f;
		prop.ambient.b = 0.027451f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.780392f;
		prop.diffuse.g = 0.568627f;
		prop.diffuse.b = 0.113725f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.992157f;
		prop.specular.g = 0.941176f;
		prop.specular.b = 0.807843f;

		prop.shine = 27.89743616f;

		prop.diffColor.r = 181;
		prop.diffColor.g = 166;
		prop.diffColor.b = 66;
		prop.diffColor.a = 127;
	}
	else if(type == 2) // Copper
	{
		prop.ambient.r = 0.019125f;
		prop.ambient.g = 0.00735f;
		prop.ambient.b = 0.00225f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.7038f;
		prop.diffuse.g = 0.27048f;
		prop.diffuse.b = 0.0828f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.256777f;
		prop.specular.g = 0.137622f;
		prop.specular.b = 0.086014f;

		prop.shine = 12.80f;

		prop.diffColor.r = 184;
		prop.diffColor.g = 115;
		prop.diffColor.b = 51;
		prop.diffColor.a = 127;
	}
	else if(type == 3) // Steel or chrome
	{
		// I don't like this material
		prop.ambient.r = 0.025f;
		prop.ambient.g = 0.025f;
		prop.ambient.b = 0.025f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.4f;
		prop.diffuse.g = 0.4f;
		prop.diffuse.b = 0.4f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.774597f;
		prop.specular.g = 0.774597f;
		prop.specular.b = 0.774597f;

		prop.shine = 76.8f;

		prop.diffColor.r = 102;
		prop.diffColor.g = 102;
		prop.diffColor.b = 102;
		prop.diffColor.a = 127;
	}
	else if(type == 4) // Silver
	{
		prop.ambient.r = 0.019225f;
		prop.ambient.g = 0.019225f;
		prop.ambient.b = 0.019225f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.50754f;
		prop.diffuse.g = 0.50754f;
		prop.diffuse.b = 0.51f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.508273f;
		prop.specular.g = 0.508273f;
		prop.specular.b = 0.508273f;

		prop.shine = 51.2f;

		prop.diffColor.r = 129;
		prop.diffColor.g = 129;
		prop.diffColor.b = 131;
		prop.diffColor.a = 127;
	}
	else if(type == 5) // Gold
	{
		prop.ambient.r = 0.245f;
		prop.ambient.g = 0.1995f;
		prop.ambient.b = 0.0745f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.754164f;
		prop.diffuse.g = 0.60648f;
		prop.diffuse.b = 0.22648f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.628281f;
		prop.specular.g = 0.555802f;
		prop.specular.b = 0.366065f;

		prop.shine = 51.2f;

		prop.diffColor.r = 191;
		prop.diffColor.g = 154;
		prop.diffColor.b = 57;
		prop.diffColor.a = 127;
	}
	else if(type == 6) // Emerald
	{
		prop.ambient.r = 0.00215f;
		prop.ambient.g = 0.01745f;
		prop.ambient.b = 0.00215f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.07568f;
		prop.diffuse.g = 0.61424f;
		prop.diffuse.b = 0.07568f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.633f;
		prop.specular.g = 0.727811f;
		prop.specular.b = 0.633f;

		prop.shine = 7.68f;

		prop.diffColor.r = 19;
		prop.diffColor.g = 156;
		prop.diffColor.b = 19;
		prop.diffColor.a = 127;
	}
	else if(type == 7) // Obsidian
	{
		prop.ambient.r = 0.005375f;
		prop.ambient.g = 0.005f;
		prop.ambient.b = 0.006625f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.18275f;
		prop.diffuse.g = 0.17f;
		prop.diffuse.b = 0.22525f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.332741f;
		prop.specular.g = 0.328634f;
		prop.specular.b = 0.346435f;

		prop.shine = 3.84f;

		prop.diffColor.r = 46;
		prop.diffColor.g = 43;
		prop.diffColor.b = 57;
		prop.diffColor.a = 75;
	}
	else if(type == 8) // Ruby
	{
		prop.ambient.r = 0.01745f;
		prop.ambient.g = 0.001175f;
		prop.ambient.b = 0.001175f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.61424f;
		prop.diffuse.g = 0.04136f;
		prop.diffuse.b = 0.04136f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.727811f;
		prop.specular.g = 0.626959f;
		prop.specular.b = 0.626959f;

		prop.shine = 7.68f;

		prop.diffColor.r = 156;
		prop.diffColor.g = 11;
		prop.diffColor.b = 11;
		prop.diffColor.a = 75;
	}
	else if(type == 9) // Rubber Black
	{
		prop.ambient.r = 0.02f;
		prop.ambient.g = 0.02f;
		prop.ambient.b = 0.02f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.01f;
		prop.diffuse.g = 0.01f;
		prop.diffuse.b = 0.01f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.4f;
		prop.specular.g = 0.4f;
		prop.specular.b = 0.4f;

		prop.shine = 10.0f;

		prop.diffColor.r = 2;
		prop.diffColor.g = 2;
		prop.diffColor.b = 2;
		prop.diffColor.a = 175;
	}
	else if(type == 10) // Rubber White
	{
		prop.ambient.r = 0.05f;
		prop.ambient.g = 0.05f;
		prop.ambient.b = 0.05f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.5f;
		prop.diffuse.g = 0.5f;
		prop.diffuse.b = 0.5f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.7f;
		prop.specular.g = 0.7f;
		prop.specular.b = 0.7f;

		prop.shine = 10.0f;

		prop.diffColor.r = 255;
		prop.diffColor.g = 255;
		prop.diffColor.b = 255;
		prop.diffColor.a = 175;
	}
	else if(type == 11) // Rubber Red
	{
		prop.ambient.r = 0.05f;
		prop.ambient.g = 0.0f;
		prop.ambient.b = 0.0f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.5f;
		prop.diffuse.g = 0.4f;
		prop.diffuse.b = 0.4f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.7f;
		prop.specular.g = 0.04f;
		prop.specular.b = 0.04f;

		prop.shine = 10.0f;

		prop.diffColor.r = 127;
		prop.diffColor.g = 102;
		prop.diffColor.b = 102;
		prop.diffColor.a = 75;
	}
	else if(type == 12) // Rubber Cyan
	{
		prop.ambient.r = 0.0f;
		prop.ambient.g = 0.05f;
		prop.ambient.b = 0.05f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.4f;
		prop.diffuse.g = 0.5f;
		prop.diffuse.b = 0.5f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.04f;
		prop.specular.g = 0.7f;
		prop.specular.b = 0.7f;

		prop.shine = 10.0f;

		prop.diffColor.r = 102;
		prop.diffColor.g = 127;
		prop.diffColor.b = 127;
		prop.diffColor.a = 75;
	}
	else if(type == 13) // Rubber Green
	{
		prop.ambient.r = 0.0f;
		prop.ambient.g = 0.05f;
		prop.ambient.b = 0.0f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.4f;
		prop.diffuse.g = 0.5f;
		prop.diffuse.b = 0.4f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.04f;
		prop.specular.g = 0.7f;
		prop.specular.b = 0.04f;

		prop.shine = 10.0f;

		prop.diffColor.r = 102;
		prop.diffColor.g = 127;
		prop.diffColor.b = 102;
		prop.diffColor.a = 75;
	}
	else if(type == 14) // Rubber Yellow
	{
		prop.ambient.r = 0.05f;
		prop.ambient.g = 0.05f;
		prop.ambient.b = 0.0f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.5f;
		prop.diffuse.g = 0.5f;
		prop.diffuse.b = 0.4f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.7f;
		prop.specular.g = 0.7f;
		prop.specular.b = 0.04f;

		prop.shine = 10.0f;

		prop.diffColor.r = 127;
		prop.diffColor.g = 127;
		prop.diffColor.b = 102;
		prop.diffColor.a = 75;
	}
	else if(type == 15) // Plastic Black
	{
		prop.ambient.r = 0.0f;
		prop.ambient.g = 0.0f;
		prop.ambient.b = 0.0f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.01f;
		prop.diffuse.g = 0.01f;
		prop.diffuse.b = 0.01f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.5f;
		prop.specular.g = 0.5f;
		prop.specular.b = 0.5f;

		prop.shine = 32.0f;

		prop.diffColor.r = 2;
		prop.diffColor.g = 2;
		prop.diffColor.b = 2;
		prop.diffColor.a = 75;
	}
	else if(type == 16) // Plastic Cyan
	{
		prop.ambient.r = 0.0f;
		prop.ambient.g = 0.1f;
		prop.ambient.b = 0.06f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.094117f;
		prop.diffuse.g = 0.827450f;
		prop.diffuse.b = 0.839215f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.50196078f;
		prop.specular.g = 0.50196078f;
		prop.specular.b = 0.5019678f;

		prop.shine = 32.0f;

		prop.diffColor.r = 24;
		prop.diffColor.g = 211;
		prop.diffColor.b = 214;
		prop.diffColor.a = 75;
	}
	else if(type == 17) // Plastic Blue
	{
		prop.ambient.r = 0.0f;
		prop.ambient.g = 0.0f;
		prop.ambient.b = 0.06f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.09411f;
		prop.diffuse.g = 0.403921f;
		prop.diffuse.b = 0.839215f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.50196078f;
		prop.specular.g = 0.50196078f;
		prop.specular.b = 0.50196078f;

		prop.shine = 32.0f;

		prop.diffColor.r = 24;
		prop.diffColor.g = 103;
		prop.diffColor.b = 214;
		prop.diffColor.a = 75;
	}
	else if(type == 18) // Plastic Green
	{
		prop.ambient.r = 0.0f;
		prop.ambient.g = 0.0f;
		prop.ambient.b = 0.0f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.062745f;
		prop.diffuse.g = 0.701960f;
		prop.diffuse.b = 0.211764f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.45f;
		prop.specular.g = 0.55f;
		prop.specular.b = 0.45f;

		prop.shine = 32.0f;

		prop.diffColor.r = 16;
		prop.diffColor.g = 179;
		prop.diffColor.b = 54;
		prop.diffColor.a = 175;
	}
	else if(type == 19) // Plastic Red
	{
		prop.ambient.r = 0.0f;
		prop.ambient.g = 0.0f;
		prop.ambient.b = 0.0f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.749019f;
		prop.diffuse.g = 0.082352f;
		prop.diffuse.b = 0.082352f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.7f;
		prop.specular.g = 0.7f;
		prop.specular.b = 0.7f;

		prop.shine = 32.0f;

		prop.diffColor.r = 191;
		prop.diffColor.g = 21;
		prop.diffColor.b = 21;
		prop.diffColor.a = 175;
	}
	else if(type == 20) // Plastic Yellow
	{
		prop.ambient.r = 0.0f;
		prop.ambient.g = 0.0f;
		prop.ambient.b = 0.0f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.894117f;
		prop.diffuse.g = 0.941176f;
		prop.diffuse.b = 0.054901f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.6f;
		prop.specular.g = 0.6f;
		prop.specular.b = 0.5f;

		prop.shine = 32.0f;

		prop.diffColor.r = 228;
		prop.diffColor.g = 240;
		prop.diffColor.b = 14;
		prop.diffColor.a = 175;
	}
	else if(type == 21) // Plastic Magenta
	{
		prop.ambient.r = 0.0f;
		prop.ambient.g = 0.0f;
		prop.ambient.b = 0.0f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.749019f;
		prop.diffuse.g = 0.066666f;
		prop.diffuse.b = 0.705882f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.5f;
		prop.specular.g = 0.0f;
		prop.specular.b = 0.4f;

		prop.shine = 10.0f;

		prop.diffColor.r = 191;
		prop.diffColor.g = 17;
		prop.diffColor.b = 180;
		prop.diffColor.a = 175;
	}
	else if(type == 22) // Plastic Orange
	{
		prop.ambient.r = 0.0f;
		prop.ambient.g = 0.0f;
		prop.ambient.b = 0.0f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.9294117f;
		prop.diffuse.g = 0.5882352f;
		prop.diffuse.b = 0.0352941f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.4f;
		prop.specular.g = 0.4f;
		prop.specular.b = 0.4f;

		prop.shine = 10.0f;

		prop.diffColor.r = 237;
		prop.diffColor.g = 150;
		prop.diffColor.b = 9;
		prop.diffColor.a = 175;
	}
	else if(type == 23) // Plastic Brown
	{
		prop.ambient.r = 0.0f;
		prop.ambient.g = 0.0f;
		prop.ambient.b = 0.0f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.501960f;
		prop.diffuse.g = 0.333333f;
		prop.diffuse.b = 0.023529f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.4f;
		prop.specular.g = 0.4f;
		prop.specular.b = 0.4f;

		prop.shine = 10.0f;

		prop.diffColor.r = 128;
		prop.diffColor.g = 85;
		prop.diffColor.b = 6;
		prop.diffColor.a = 175;
	}
	else if(type == 24) // End Cap
	{
		prop.ambient.r = 0.0f;
		prop.ambient.g = 0.0f;
		prop.ambient.b = 0.0f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.1f;
		prop.diffuse.g = 0.5f;
		prop.diffuse.b = 0.8f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 1.0f;
		prop.specular.g = 1.0f;
		prop.specular.b = 1.0f;

		prop.shine = 5.0f;

		prop.diffColor.r = 75;
		prop.diffColor.g = 75;
		prop.diffColor.b = 75;
		prop.diffColor.a = 75;
	}
	else if(type == 25) // Shell
	{
		prop.ambient.r = 0.0f;
		prop.ambient.g = 0.0f;
		prop.ambient.b = 0.0f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.1f;
		prop.diffuse.g = 0.5f;
		prop.diffuse.b = 0.8f;
		prop.diffuse.a = 1.0f;

		prop.shine = 5.0f;

		prop.diffColor.r = 12;
		prop.diffColor.g = 225;
		prop.diffColor.b = 45;
		prop.diffColor.a = 75;
	}
	else // default
	{
		prop.ambient.r = 0.0f;
		prop.ambient.g = 0.0f;
		prop.ambient.b = 0.0f;
		prop.ambient.a = 1.0f;

		prop.diffuse.r = 0.5f;
		prop.diffuse.g = 0.5f;
		prop.diffuse.b = 0.5f;
		prop.diffuse.a = 1.0f;

		prop.specular.r = 0.5f;
		prop.specular.g = 0.5f;
		prop.specular.b = 0.5f;

		prop.shine = 5.0f;

		prop.diffColor.r = 127;
		prop.diffColor.g = 127;
		prop.diffColor.b = 127;
		prop.diffColor.a = 127;
	}
}

int meshMaterials::findMaterialId(char string[80])
{
	int matId = 0;

	if(0 == strcmp(string,"Brass"))
	{
		matId  = 1;
	}
	else if(0 == strcmp(string,"Copper"))
	{
		matId  = 2;
	}
	else if(0 == strcmp(string, "Steel"))
	{
		matId  = 3;
	}
	else if(0 == strcmp(string, "Silver"))
	{
		matId  = 4;
	}
	else if(0 == strcmp(string,"Gold"))
	{
		matId  = 5;
	}
	else if(0 == strcmp(string, "Emerald"))
	{
		matId  = 6;
	}
	else if(0 == strcmp(string, "Obsidian"))
	{
		matId  = 7;
	}
	else if(0 == strcmp(string, "Ruby"))
	{
		matId  = 8;
	}
	else if(0 == strcmp(string, "Black Rubber"))
	{
		matId  = 9;
	}
	else if(0 == strcmp(string, "White Rubber"))
	{
		matId  = 10;
	}
	else if(0 == strcmp(string, "Red Rubber"))
	{
		matId  = 11;
	}
	else if(0 == strcmp(string, "Cyan Rubber"))
	{
		matId  = 12;
	}
	else if(0 == strcmp(string, "Green Rubber"))
	{
		matId  = 13;
	}
	else if(0 == strcmp(string, "Yellow Rubber"))
	{
		matId  = 14;
	}
	else if(0 == strcmp(string, "Black Plastic"))
	{
		matId  = 15;
	}
	else if(0 == strcmp(string,"Cyan Plastic"))
	{
		matId  = 16;
	}
	else if(0 == strcmp(string,"Blue Plastic"))
	{
		matId  = 17;
	}
	else if(0 == strcmp(string, "Green Plastic"))
	{
		matId  = 18;
	}
	else if(0 == strcmp(string, "Red Plastic"))
	{
		matId  = 19;
	}
	else if(0 == strcmp(string, "Yellow Plastic"))
	{
		matId  = 20;
	}
	else if(0 == strcmp(string, "Magenta Plastic"))
	{
		matId  = 21;
	}
	else if(0 == strcmp(string,"Orange Plastic"))
	{
		matId  = 22;
	}
	else if(0 == strcmp(string,"Brown Plastic"))
	{
		matId  = 23;
	}
	else
	{
		matId  = 0;
	}

	return matId;
}

void meshMaterials::getMaterial()
{
	float mat[4] = {prop.ambient.r, prop.ambient.g, prop.ambient.g, prop.ambient.a};

	// now we should build the material for this object
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
	mat[0] = prop.diffuse.r;
	mat[1] = prop.diffuse.g;
	mat[2] = prop.diffuse.b;
	mat[3] = prop.diffuse.a;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
	mat[0] = prop.specular.r;
	mat[1] = prop.specular.g;
	mat[2] = prop.specular.b;
	mat[3] = prop.specular.a;
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
	glMaterialf(GL_FRONT, GL_SHININESS, prop.shine);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_SPECULAR);
	glColor4f( prop.specular.r, prop.specular.g, prop.specular.b, prop.specular.a);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	//then we set the material color
	glColor4ub(prop.diffColor.r, prop.diffColor.g, prop.diffColor.b, prop.diffColor.a);
	glDisable(GL_COLOR_MATERIAL);
}