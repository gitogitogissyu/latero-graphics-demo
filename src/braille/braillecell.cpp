// -----------------------------------------------------------
//
// Copyright (c) 2015 by Vincent Levesque. All Rights Reserved.
//
// This file is part of latero-demo.
//
//    latero-demo is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    latero-demo is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with latero-demo.  If not, see <http://www.gnu.org/licenses/>.
//
// -----------------------------------------------------------

#include "../config.h"
#ifndef DISABLE_BRAILLE_DEMO

#include "braillecell.h"
#include <math.h>
#include <laterographics/mutex.h>

#define BRAILLE_A DOT_1
#define BRAILLE_B DOT_1 | DOT_2
#define BRAILLE_C DOT_1 | DOT_4
#define BRAILLE_D DOT_1 | DOT_4 | DOT_5
#define BRAILLE_E DOT_1 | DOT_5
#define BRAILLE_F DOT_1 | DOT_2 | DOT_4
#define BRAILLE_G DOT_1 | DOT_2 | DOT_4 | DOT_5
#define BRAILLE_H DOT_1 | DOT_2 | DOT_5
#define BRAILLE_I DOT_2 | DOT_4
#define BRAILLE_J DOT_2 | DOT_4 | DOT_5

#define BRAILLE_K DOT_3 | BRAILLE_A
#define BRAILLE_L DOT_3 | BRAILLE_B
#define BRAILLE_M DOT_3 | BRAILLE_C
#define BRAILLE_N DOT_3 | BRAILLE_D
#define BRAILLE_O DOT_3 | BRAILLE_E
#define BRAILLE_P DOT_3 | BRAILLE_F
#define BRAILLE_Q DOT_3 | BRAILLE_G
#define BRAILLE_R DOT_3 | BRAILLE_H
#define BRAILLE_S DOT_3 | BRAILLE_I
#define BRAILLE_T DOT_3 | BRAILLE_J

#define BRAILLE_U DOT_6 | BRAILLE_K
#define BRAILLE_V DOT_6 | BRAILLE_L
#define BRAILLE_X DOT_6 | BRAILLE_M
#define BRAILLE_Y DOT_6 | BRAILLE_N
#define BRAILLE_Z DOT_6 | BRAILLE_O

#define BRAILLE_W DOT_2 | DOT_4 | DOT_5 | DOT_6


BrailleCell::BrailleCell()
{
	dot_ = 0x00;
}

void BrailleCell::Set(unsigned int x, unsigned int y, bool v)
{
	LATERO_GRAPHICS_GUARD;
	return Set_(x,y,v);
}

void BrailleCell::Set_(unsigned int x, unsigned int y, bool v)
{
	unsigned int d = y+3*x;
	if (v)
		dot_ |= (1<<d);
	else
		dot_ &= ~(1<<d);
}

bool BrailleCell::Get(unsigned int x, unsigned int y)
{
	LATERO_GRAPHICS_GUARD;
	return Get_(x,y);
}

bool BrailleCell::Get_(unsigned int x, unsigned int y)
{
	unsigned int d = y+3*x;
	return dot_ & (1<<d);
}

unsigned char BrailleCell::Encode(char c)
{
	switch(c)
	{
		case 'a' : return BRAILLE_A;
		case 'b' : return BRAILLE_B;
		case 'c' : return BRAILLE_C;
		case 'd' : return BRAILLE_D;
		case 'e' : return BRAILLE_E;
		case 'f' : return BRAILLE_F;
		case 'g' : return BRAILLE_G;
		case 'h' : return BRAILLE_H;
		case 'i' : return BRAILLE_I;
		case 'j' : return BRAILLE_J;
		case 'k' : return BRAILLE_K;
		case 'l' : return BRAILLE_L;
		case 'm' : return BRAILLE_M;
		case 'n' : return BRAILLE_N;
		case 'o' : return BRAILLE_O;
		case 'p' : return BRAILLE_P;
		case 'q' : return BRAILLE_Q;
		case 'r' : return BRAILLE_R;
		case 's' : return BRAILLE_S;
		case 't' : return BRAILLE_T;
		case 'u' : return BRAILLE_U;
		case 'v' : return BRAILLE_V;
		case 'w' : return BRAILLE_W;
		case 'x' : return BRAILLE_X;
		case 'y' : return BRAILLE_Y;
		case 'z' : return BRAILLE_Z;
		default : return 0x00;
	}
}

char BrailleCell::Decode(unsigned char b)
{
	switch(b)
	{
		case BRAILLE_A : return 'a';
		case BRAILLE_B : return 'b';
		case BRAILLE_C : return 'c';
		case BRAILLE_D : return 'd';
		case BRAILLE_E : return 'e';
		case BRAILLE_F : return 'f';
		case BRAILLE_G : return 'g';
		case BRAILLE_H : return 'h';
		case BRAILLE_I : return 'i';
		case BRAILLE_J : return 'j';
		case BRAILLE_K : return 'k';
		case BRAILLE_L : return 'l';
		case BRAILLE_M : return 'm';
		case BRAILLE_N : return 'n';
		case BRAILLE_O : return 'o';
		case BRAILLE_P : return 'p';
		case BRAILLE_Q : return 'q';
		case BRAILLE_R : return 'r';
		case BRAILLE_S : return 's';
		case BRAILLE_T : return 't';
		case BRAILLE_U : return 'u';
		case BRAILLE_V : return 'v';
		case BRAILLE_W : return 'w';
		case BRAILLE_X : return 'x';
		case BRAILLE_Y : return 'y';
		case BRAILLE_Z : return 'z';
		default : return ' ';
	};
}


void BrailleCell::SetLetter(char letter)
{
	LATERO_GRAPHICS_GUARD
	dot_ = Encode(letter);
}

char BrailleCell::GetLetter()
{
	LATERO_GRAPHICS_GUARD;
	return Decode(dot_);
}

unsigned char BrailleCell::GetPattern()
{
	LATERO_GRAPHICS_GUARD;
	return dot_;
}

BrailleCell::BrailleCell(const BrailleCell& p)
{
	LATERO_GRAPHICS_GUARD;
	dot_ = p.dot_;
}

BrailleCell BrailleCell::operator= (const BrailleCell& s)
{ 
	if (&s == this) return *this;
	LATERO_GRAPHICS_GUARD;
	dot_ = s.dot_;
	return *this; 
};


#endif
