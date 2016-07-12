#ifndef __3DTEXT_H__
#define __3DTEXT_H__

#include "stdafx.h"
#include <afxtempl.h>
#include <vector>

#include "3dMaterials.h"
#include "3dModels.h"

typedef struct fontData
{
	iColorRGBA	color;
	vector_t	pos;

	char m_cDisplayText[256];
}fData;

class C3dText
{
public:
	C3dText();
	~C3dText();

	void DrawTxt(fData fontData);
	void NewFont();

	LOGFONT m_logFont;			// stored information about the currently selected font
private:
	GLuint m_lBase;		// display list for the Font Set

	GLvoid BuildFont();
	GLvoid KillFont(GLvoid);
	GLvoid glPrint(const char *fmt, ...);
};
#endif