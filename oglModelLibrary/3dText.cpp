//
#include "stdafx.h"
#include <afxtempl.h>
#include "3dText.h"

C3dText::C3dText()
{
	m_logFont.lfHeight = 24;								// Height of font
	m_logFont.lfWidth  =  0;								// Width of font
	m_logFont.lfEscapement = 0;								// Angle of escapement
	m_logFont.lfOrientation = 0;							// Orientation Angle
	m_logFont.lfWeight = FW_BOLD;							// Font Weight
	m_logFont.lfItalic = FALSE;								// Italic
	m_logFont.lfUnderline = FALSE;							// Underline
	m_logFont.lfStrikeOut = FALSE;							// Strikeout
	m_logFont.lfCharSet = ANSI_CHARSET;						// Character Set Identifier
	m_logFont.lfOutPrecision = OUT_TT_PRECIS;				// Output precision
	m_logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;		// Clipping precision
	m_logFont.lfQuality = ANTIALIASED_QUALITY;				// Output quality
	m_logFont.lfPitchAndFamily = FF_DONTCARE|DEFAULT_PITCH;	// Family and Pitch 
	lstrcpy(m_logFont.lfFaceName, "Courier New");			// Font Name
}

C3dText::~C3dText()
{
	KillFont();
}

void C3dText::DrawTxt(fData fontData)
{
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glPolygonMode(GL_FRONT,GL_FILL);
	glTranslatef(0.0f, 0.0f, -1.0f);
	glRasterPos3f (fontData.pos.x, fontData.pos.y, fontData.pos.z);
	glColor4ub (fontData.color.r, fontData.color.g, fontData.color.b, 255);	
	glPrint(fontData.m_cDisplayText);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void C3dText::NewFont()
{
	// grant public access to create a new font
	KillFont();
	BuildFont();
}

GLvoid C3dText::BuildFont()
{
	// building the bitmap font from provided font info
	HWND m_hWnd = GetActiveWindow();
	HDC m_hDC = GetDC(m_hWnd);

	CDC *pDC = CDC::FromHandle(m_hDC);

	CFont	font;							// Windows Font ID
	CFont	*oldFont;						// Used for good housekeeping

	m_lBase = glGenLists(256);					// storage for 96 characters
	
	BOOL success = font.CreateFontIndirect(&m_logFont);

	// I need to get HDC in the constructor?
	// Stores the currently selected font in oldFont,
	// and then selects the font we want to build in the
	// display list
	oldFont = pDC->SelectObject(&font);

	// this should give us some metrics on our currently selected font
	TEXTMETRIC text;
	pDC->GetTextMetrics(&text);

	wglUseFontBitmaps(m_hDC, 0, 256, m_lBase);	// Builds 96 characters starting with char 32
	SelectObject(m_hDC, &oldFont);				// restores selected font 

	DeleteObject(font);								// remove our font info
	::ReleaseDC(m_hWnd,m_hDC);
}

GLvoid C3dText::KillFont(GLvoid)
{
	glDeleteLists(m_lBase, 256);	// Delete all 96 characters
}

GLvoid C3dText::glPrint(const char *fmt, ...)
{	
	if(fmt == NULL){			// If there's no text
		return;					// Do nothing
	}

	// Pushes the Display list Bits
	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);	

	glListBase(m_lBase);								// Sets the Base character to 32
	glCallLists(strlen(fmt), GL_UNSIGNED_BYTE, fmt);	// Draws the display list text
	glPopAttrib();										// pops the display list bits
}
