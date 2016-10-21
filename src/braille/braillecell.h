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

#ifndef BRAILLE_CELL_H
#define BRAILLE_CELL_H

#define DOT_1 (1<<0)
#define DOT_2 (1<<1)
#define DOT_3 (1<<2)
#define DOT_4 (1<<3)
#define DOT_5 (1<<4)
#define DOT_6 (1<<5)

class BrailleCell
{
public:
	BrailleCell();
	BrailleCell(const BrailleCell& p);
	BrailleCell operator=(const BrailleCell& s);

	void Set(unsigned int x, unsigned int y, bool v);
	void Set_(unsigned int x, unsigned int y, bool v);
	bool Get(unsigned int x, unsigned int y);
	bool Get_(unsigned int x, unsigned int y);

	void SetLetter(char letter);
	char GetLetter();

	unsigned char GetPattern();

protected:
	unsigned char Encode(char c);
	char Decode(unsigned char b);

	unsigned char dot_;
};

#endif
#endif
