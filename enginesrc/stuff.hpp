#ifndef _STUFF_HPP_
#define _STUFF_HPP_

//#include "glext.h"
#include "global.h"

Texture *findTexture(const char *name);

//interpoloi, palauttaa 0 jos value1 < limit1, palauttaa 0..1 jos välissä, 1 jos menee yli
float calcPosInt(int value1, int limit1, int limit2);

//interpoloi, palauttaa 0 jos value1 < limit1, palauttaa 0..1 jos välissä, 1 jos menee yli
float calcPosFloat(float value1, float limit1, float limit2);

//saa maksimin puolivälissä (limit1+limit2)/2, multiplyllä kerrotaan, paluuarvo klipataan [0..1]
float calcSaturate(float value, float limit1, float limit2, float multiply);

const bool testaaPos(const int aika, const int ajat[]);
const float laskePos(const int aika, const int ajat[]);

void perspective2D(float width, float height);
void perspective3D();

void textures_init();
void textures_free();

#endif