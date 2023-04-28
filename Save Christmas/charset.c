#include <coleco.h>

#define SCROLLOBJ_ARBRE 0
#define SCROLLOBJ_BONHOMME 1
#define SCROLLOBJ_ETOILE 2
#define SCROLLOBJ_BOULE 3

const byte scrollObj_Arbre[] =
{
	 92, 92, 92,106, 92, 92, 92, 92,
	 92, 92,101,107,110,116, 92, 92,
	 92, 92,102,104,111,117, 92, 92,
	 92, 96,103,104,112,118, 92, 92,
	 92, 97,103,104,113,119, 92, 92,
	 92, 98,104,104,104,120,123, 92,
	 92, 99,104,108,114,121,124, 92,
	 92,100,105,109,115,122,125, 92
};

const byte scrollObj_Bonhomme[] =
{
	 92,127,133, 92, 92, 92,
     92,128,134,139, 92, 92,
     92,129,135,140, 92, 92,
     92,130,136,141,144, 92,
     92,131,137,142, 92, 92,
	 92,132,138,143, 92, 92
};

const byte scrollObj_Etoile[] =
{
	145,149,155, 92,
	146,150,156, 92,
	 92,151,157, 92
};

const byte scrollObj_Boule[] =
{
	  92,152,158, 92,
	 147,153,159, 92,
	 148,154,160, 92
};

const byte scrollObj_Init[] =
{
	SCROLLOBJ_ARBRE,8,8,0,
	SCROLLOBJ_BONHOMME,6,6,0,
	SCROLLOBJ_ETOILE,3,4,0,
	SCROLLOBJ_BOULE,3,4,0,
};

const byte* scrollObj_Pointer[] =
{
	scrollObj_Arbre,
	scrollObj_Bonhomme,
	scrollObj_Etoile,
	scrollObj_Boule
};

const byte NAMERLE[] = {
  0xFE, 0x20, 0xFE, 0x20, 0xFE, 0x20, 0xFE, 0x20, 0xFE, 0x20, 0xFE, 0x20, 0x85, 0x20, 0xFF};
 

const byte PATTERNRLE[] = {
  0xFE, 0x00, 0xFE, 0x00, 0x8B, 0x00, 0x04, 0x0C, 0x18, 0x30, 0x00, 0x60, 0x82, 0x00, 0x81,
  0x66, 0x85, 0x00, 0x21, 0x66, 0xFF, 0x66, 0xFF, 0x66, 0x00, 0x18, 0x3E, 0x60, 0x3C, 0x06, 0x7C,
  0x18, 0x00, 0xFF, 0x81, 0xB9, 0xA5, 0xB9, 0xA5, 0x81, 0xFF, 0x3C, 0x66, 0x3C, 0x38, 0x67, 0x66,
  0x3F, 0x80, 0x00, 0x00, 0x18, 0x30, 0x85, 0x00, 0x00, 0x18, 0x82, 0x30, 0x00, 0x18, 0x82, 0x00,
  0x00, 0x18, 0x82, 0x0C, 0x00, 0x18, 0x82, 0x00, 0x04, 0x5A, 0x3C, 0x7E, 0x3C, 0x5A, 0x82, 0x00,
  0x81, 0x18, 0x02, 0x7E, 0x18, 0x18, 0x85, 0x00, 0x01, 0x18, 0x30, 0x84, 0x00, 0x00, 0x7E, 0x88,
  0x00, 0x00, 0x18, 0x82, 0x00, 0x04, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x82, 0x00, 0x04, 0x7C, 0xCE,
  0xD6, 0xE6, 0x7C, 0x82, 0x00, 0x04, 0x18, 0x38, 0x18, 0x18, 0x7E, 0x82, 0x00, 0x04, 0x7C, 0x06,
  0x7E, 0x60, 0x7E, 0x82, 0x00, 0x04, 0x7C, 0x06, 0x7C, 0x06, 0x7C, 0x82, 0x00, 0x81, 0xCC, 0x02,
  0xFE, 0x0C, 0x0C, 0x82, 0x00, 0x04, 0x7E, 0x60, 0x7C, 0x06, 0x7C, 0x82, 0x00, 0x04, 0x3E, 0x60,
  0x7C, 0x66, 0x3C, 0x82, 0x00, 0x04, 0xFE, 0x06, 0x0C, 0x18, 0x30, 0x82, 0x00, 0x04, 0x3C, 0x66,
  0x3C, 0x66, 0x3C, 0x82, 0x00, 0x04, 0x3C, 0x66, 0x3C, 0x06, 0x7C, 0x83, 0x00, 0x02, 0x18, 0x00,
  0x18, 0x84, 0x00, 0x0A, 0x18, 0x00, 0x18, 0x30, 0x0E, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0E, 0x84,
  0x00, 0x0A, 0x7E, 0x00, 0x7E, 0x00, 0x00, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x70, 0x83, 0x00, 0x04,
  0x38, 0x6C, 0x08, 0x00, 0x18, 0x8A, 0x00, 0x04, 0x7C, 0xC6, 0xFE, 0xC6, 0xC6, 0x82, 0x00, 0x04,
  0xFC, 0xC6, 0xFC, 0xC6, 0xFC, 0x82, 0x00, 0x04, 0x7C, 0xC6, 0xC0, 0xC6, 0x7C, 0x82, 0x00, 0x00,
  0xFC, 0x82, 0xC6, 0x00, 0xFC, 0x82, 0x00, 0x04, 0xFE, 0xC0, 0xFE, 0xC0, 0xFE, 0x82, 0x00, 0x04,
  0xFE, 0xC0, 0xFE, 0xC0, 0xC0, 0x82, 0x00, 0x04, 0x7E, 0xC0, 0xDC, 0xC6, 0x7C, 0x82, 0x00, 0x81,
  0xC6, 0x02, 0xFE, 0xC6, 0xC6, 0x82, 0x00, 0x00, 0xFC, 0x82, 0x30, 0x00, 0xFC, 0x82, 0x00, 0x04,
  0xFE, 0x06, 0x06, 0xC6, 0x7C, 0x82, 0x00, 0x04, 0xC6, 0xCC, 0xF8, 0xCC, 0xC6, 0x82, 0x00, 0x83,
  0xC0, 0x00, 0xFE, 0x82, 0x00, 0x04, 0xC6, 0xEE, 0xFE, 0xD6, 0xC6, 0x82, 0x00, 0x04, 0xC6, 0xE6,
  0xFE, 0xCE, 0xC6, 0x82, 0x00, 0x00, 0x7C, 0x82, 0xC6, 0x00, 0x7C, 0x82, 0x00, 0x04, 0xFC, 0xC6,
  0xFC, 0xC0, 0xC0, 0x82, 0x00, 0x04, 0x7C, 0xC6, 0xC6, 0xCC, 0x76, 0x82, 0x00, 0x04, 0xFC, 0xC6,
  0xFC, 0xCC, 0xC6, 0x82, 0x00, 0x04, 0x7E, 0xC0, 0x7C, 0x06, 0xFC, 0x82, 0x00, 0x00, 0xFC, 0x83,
  0x30, 0x82, 0x00, 0x83, 0xC6, 0x00, 0x7C, 0x82, 0x00, 0x82, 0xC6, 0x01, 0x6C, 0x38, 0x82, 0x00,
  0x04, 0xC6, 0xD6, 0xFE, 0xEE, 0xC6, 0x82, 0x00, 0x04, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0x82, 0x00,
  0x81, 0xC6, 0x02, 0x7E, 0x06, 0xFC, 0x82, 0x00, 0x04, 0xFC, 0x18, 0x30, 0x60, 0xFC, 0x88, 0x00,
  0x8A, 0xFF, 0x83, 0xFC, 0x86, 0xFF, 0x04, 0xE1, 0xC3, 0xC1, 0xC1, 0xE1, 0x89, 0xFF, 0x17, 0xF2,
  0xE1, 0xC1, 0xC1, 0xE1, 0xF3, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0x80, 0xC0, 0xC0,
  0x9C, 0xF8, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x82, 0xFF, 0x00, 0xF8, 0x89, 0xFF, 0x02, 0xFE, 0xFC,
  0x8F, 0x82, 0xF0, 0x0A, 0x8E, 0xFE, 0xFC, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xC0, 0x80, 0x80, 0x8E,
  0xFF, 0x84, 0xFE, 0x83, 0xFF, 0x04, 0xF7, 0xE3, 0xC1, 0xC1, 0x80, 0x97, 0xFF, 0x81, 0x80, 0x02,
  0xC0, 0xE0, 0xF0, 0x82, 0xF8, 0x08, 0xC0, 0xE0, 0xE0, 0xF0, 0xF0, 0xF8, 0xF8, 0xFC, 0xFE, 0x85,
  0xFF, 0x81, 0xFC, 0x00, 0xFE, 0x8B, 0xFF, 0x83, 0x80, 0x84, 0xC0, 0x84, 0xFF, 0x03, 0x80, 0xC0,
  0xC0, 0x80, 0x8A, 0xFF, 0x17, 0x80, 0xC0, 0xF0, 0xE0, 0xE0, 0xE1, 0xF3, 0xFF, 0xFF, 0x80, 0xC0,
  0xE0, 0xF0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFE, 0xE2, 0xF0, 0xF8, 0xFC, 0xFE, 0xFE, 0x83, 0xFF, 0x00,
  0xF8, 0x88, 0xFF, 0x00, 0xC0, 0x83, 0xE0, 0x00, 0xC0, 0x84, 0xFF, 0x06, 0x80, 0xC8, 0xF0, 0xE1,
  0xE1, 0xF1, 0xF7, 0x84, 0xFF, 0x05, 0xF0, 0xE7, 0xCF, 0xDF, 0xCF, 0xE0, 0x84, 0xFF, 0x01, 0xC7,
  0x83, 0x84, 0xFF, 0x13, 0xFE, 0xF8, 0xE3, 0xCF, 0x9F, 0x9F, 0xDF, 0xCF, 0xEF, 0xE7, 0xF3, 0xE8,
  0xE0, 0x9F, 0x80, 0xC0, 0x80, 0x81, 0x80, 0x80, 0x82, 0x81, 0x82, 0x80, 0x06, 0xC0, 0xBF, 0x9F,
  0xCF, 0xE3, 0xF8, 0xFE, 0x89, 0xFF, 0x02, 0xC0, 0xF0, 0xF8, 0x82, 0xFC, 0x1F, 0xF1, 0xC5, 0xE8,
  0xF0, 0xF8, 0xEF, 0xE7, 0xF0, 0xFF, 0xF8, 0xFC, 0xFF, 0xFF, 0xDF, 0x80, 0x80, 0xFF, 0xE7, 0xC7,
  0xE7, 0xFF, 0xF7, 0xE3, 0xF7, 0xFF, 0xFF, 0xE7, 0xC3, 0xE7, 0xFF, 0xF8, 0xFC, 0x89, 0xFF, 0x11,
  0xF8, 0xF0, 0x80, 0xE0, 0xA0, 0xA0, 0x9F, 0xC0, 0xE0, 0xE1, 0xFC, 0xF0, 0xF3, 0xFB, 0xFC, 0xFC,
  0xFD, 0xFC, 0x83, 0xFE, 0x06, 0xFC, 0xF9, 0xFB, 0xFB, 0xC7, 0xE0, 0x80, 0x84, 0xFF, 0x01, 0xC0,
  0x9F, 0x82, 0xDF, 0x00, 0xC0, 0x87, 0xFF, 0x04, 0xFE, 0xFC, 0xFC, 0xFE, 0xFE, 0x86, 0xFF, 0x81,
  0xFE, 0x01, 0xFD, 0xFC, 0x83, 0xFD, 0x01, 0xFC, 0xFE, 0x85, 0xFF, 0x04, 0xF7, 0xE9, 0xED, 0xEC,
  0xF1, 0x82, 0xFF, 0x07, 0x80, 0xC0, 0xC0, 0x80, 0x82, 0x8F, 0x98, 0xF0, 0x8C, 0xFF, 0x0F, 0xF0,
  0xCF, 0xE0, 0xF8, 0xB3, 0xBD, 0xCF, 0xFA, 0xCD, 0xE5, 0xF1, 0xF8, 0x83, 0xC1, 0x8E, 0xE0, 0x86,
  0xFF, 0x0A, 0xFC, 0xFB, 0xF3, 0xE7, 0xCF, 0xDF, 0xCF, 0xEF, 0xEF, 0x90, 0xF0, 0x8C, 0xFF, 0x11,
  0xF3, 0xE3, 0xFC, 0xC7, 0xE7, 0xF7, 0xF7, 0xF3, 0xCC, 0x93, 0xD3, 0xC7, 0xF8, 0xEF, 0xCF, 0xC0,
  0x80, 0xFF, 0xFE, 0x00, 0xFE, 0x00, 0xFE, 0x00, 0xFE, 0x00, 0xFE, 0x00, 0xFC, 0x00, 0xFF};

const byte COLORRLE[] = {
  0xFE, 0x41, 0xFC, 0x41, 0x83, 0x41, 0x87, 0x4F, 0xFE, 0x41, 0xFE, 0x41, 0xFE, 0x41, 0xDA,
  0x41, 0x8A, 0xF0, 0x83, 0xF8, 0x86, 0xF0, 0x84, 0xF8, 0x89, 0xF0, 0x00, 0xF6, 0x84, 0xF8, 0x81,
  0xF0, 0x85, 0xF2, 0x82, 0x82, 0x86, 0xF2, 0x82, 0x20, 0x00, 0x2F, 0x89, 0xF0, 0x81, 0xF2, 0x04,
  0x28, 0x82, 0x82, 0x83, 0xF6, 0x89, 0xF2, 0x8B, 0x20, 0x82, 0xF0, 0x84, 0xF6, 0x83, 0xF0, 0x84,
  0xF2, 0x8D, 0x20, 0x88, 0x60, 0x00, 0xF0, 0x83, 0x2F, 0x03, 0x29, 0x28, 0x28, 0xF8, 0x88, 0x2F,
  0x85, 0x20, 0x82, 0x2F, 0x8B, 0x20, 0x00, 0x62, 0x87, 0x6F, 0x84, 0xF0, 0x83, 0x8F, 0x8A, 0xF0,
  0x81, 0x2F, 0x06, 0x29, 0x28, 0xE8, 0xF8, 0xF8, 0xF0, 0xF0, 0x86, 0x2F, 0x81, 0x2D, 0x85, 0x2F,
  0x83, 0x20, 0x00, 0xF2, 0x88, 0xF0, 0x85, 0xDF, 0x84, 0xF0, 0x03, 0x2F, 0xCF, 0x29, 0x38, 0x82,
  0xF8, 0x84, 0xF0, 0x85, 0xF1, 0x84, 0xF0, 0x81, 0xF2, 0x84, 0x20, 0x81, 0x21, 0x00, 0x31, 0x88,
  0xF1, 0x02, 0xF8, 0x91, 0x19, 0x82, 0xF8, 0x81, 0x19, 0x82, 0xF8, 0x83, 0x1F, 0x85, 0xF1, 0x89,
  0xF0, 0x84, 0x2F, 0x04, 0x1F, 0xF1, 0xF1, 0xF6, 0xF6, 0x83, 0xF1, 0x08, 0xF0, 0xF1, 0x16, 0x80,
  0x80, 0x86, 0x68, 0x8F, 0xF0, 0x82, 0xF6, 0x00, 0xF0, 0x82, 0xF6, 0x81, 0xF0, 0x82, 0xF6, 0x02,
  0xF0, 0xF1, 0x1F, 0x89, 0xF0, 0x81, 0x6F, 0x83, 0x1F, 0x05, 0xF1, 0x1F, 0x6F, 0x91, 0x91, 0x8F,
  0x8E, 0xF1, 0x81, 0x1F, 0x84, 0xF0, 0x00, 0x1F, 0x83, 0xF1, 0x00, 0x1F, 0x87, 0xF0, 0x84, 0xF1,
  0x86, 0xF0, 0x89, 0xF1, 0x85, 0xF0, 0x83, 0xF1, 0x00, 0x1F, 0x82, 0xF0, 0x85, 0x1F, 0x01, 0x1B,
  0x1F, 0x8C, 0xF0, 0x08, 0xF1, 0xB1, 0x1A, 0x1A, 0x31, 0x31, 0x13, 0x13, 0x31, 0x82, 0xA1, 0x81,
  0x1A, 0x01, 0xB1, 0xF1, 0x86, 0xF0, 0x00, 0x1F, 0x87, 0xF1, 0x81, 0x1F, 0x8C, 0xF0, 0x81, 0xF1,
  0x00, 0x1F, 0x84, 0xF1, 0x00, 0x1F, 0x82, 0xF1, 0x05, 0x1F, 0xF1, 0xF1, 0x1F, 0x1F, 0xF0, 0x87,
  0x00, 0xFE, 0x41, 0xFE, 0x41, 0xFE, 0x41, 0xFE, 0x41, 0xFE, 0x41, 0xF4, 0x41, 0xFF};

const byte SPATTERNRLE[] = {
  0x94, 0x00, 0x2A, 0x06, 0x05, 0x02, 0x01, 0x03, 0x02, 0x03, 0x01, 0x01, 0x00, 0x03, 0x07,
  0x3F, 0x4F, 0x4C, 0x3B, 0x0F, 0x0F, 0xCF, 0xA6, 0x5E, 0xAF, 0x5F, 0xAF, 0xFF, 0x40, 0xFF, 0x80,
  0xE0, 0xA0, 0x70, 0xF0, 0xA0, 0xF0, 0x20, 0xD0, 0x30, 0x78, 0xF0, 0xE0, 0xFC, 0x11, 0xFE, 0x9F,
  0xFF, 0x83, 0x00, 0x07, 0x01, 0x05, 0x15, 0xD5, 0xD5, 0x15, 0x05, 0x01, 0x83, 0x00, 0x02, 0x01,
  0x05, 0x15, 0x89, 0x55, 0x0A, 0x15, 0x05, 0x01, 0x00, 0x00, 0x01, 0x02, 0x02, 0x06, 0x07, 0x00,
  0x86, 0x07, 0x08, 0x03, 0x00, 0x3C, 0xC2, 0x52, 0x5C, 0xFE, 0x9E, 0x00, 0x86, 0x9E, 0x28, 0x9C,
  0x03, 0x04, 0x08, 0x12, 0x12, 0x19, 0x16, 0x1B, 0x17, 0x1A, 0x17, 0x1A, 0x0F, 0x03, 0x00, 0x00,
  0x18, 0xA4, 0x42, 0x52, 0xB2, 0xBE, 0x1A, 0x32, 0xD2, 0x92, 0x12, 0x92, 0x1C, 0xB0, 0xC0, 0x00,
  0x00, 0x01, 0x0E, 0x12, 0x12, 0x37, 0x2B, 0x3F, 0x86, 0x23, 0x08, 0x1F, 0x00, 0xE0, 0x10, 0x90,
  0xE0, 0x90, 0x10, 0xF0, 0x86, 0x10, 0x07, 0xE0, 0x03, 0x04, 0x08, 0x12, 0x12, 0x19, 0x1E, 0x84,
  0x1D, 0x1B, 0x0D, 0x01, 0x00, 0x00, 0x18, 0xA4, 0x42, 0x52, 0xB2, 0xBE, 0x1A, 0x32, 0xF2, 0xF2,
  0x72, 0xF2, 0x7C, 0xF0, 0xC0, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x0F, 0x1F, 0x83, 0x3F,
  0x81, 0x1F, 0x09, 0x0F, 0x00, 0x90, 0x00, 0x74, 0xC0, 0x94, 0x00, 0xC0, 0xE0, 0x83, 0xF0, 0x81,
  0xE0, 0x09, 0xC0, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x01, 0x0F, 0x1F, 0x83, 0x3F, 0x81, 0x1F,
  0x09, 0x0F, 0x00, 0x00, 0x02, 0x70, 0xC0, 0x84, 0x10, 0xC0, 0xE0, 0x83, 0xF0, 0x81, 0xE0, 0x0D,
  0xC0, 0x00, 0x01, 0x03, 0x03, 0x07, 0x1F, 0x3F, 0x1F, 0x07, 0x07, 0x0F, 0x1E, 0x1C, 0x84, 0x00,
  0x81, 0x80, 0x08, 0xC0, 0xF0, 0xF8, 0xF0, 0xC0, 0xC0, 0xE0, 0xF0, 0x70, 0x83, 0x00, 0xFF};
