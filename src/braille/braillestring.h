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

#ifndef BRAILLE_STRING_H
#define BRAILLE_STRING_H

#include "braillecell.h"
#include <string>
#include <vector>
#include <laterographics/modifiable.h>

class BrailleString : public latero::graphics::Modifiable
{
public:
	BrailleString(unsigned int size);
	BrailleString(const BrailleString& p);
	BrailleString operator= (const BrailleString& s);
	virtual ~BrailleString();

	void Set(std::string str);
	std::string Get();
	std::string GetPattern();
	
	unsigned int GetSize() const { return cells_.size(); }
	BrailleCell* GetCell(unsigned int i);
	std::string GetDesc(std::string prefix);

public:
	std::vector<BrailleCell> cells_;
};

#endif
#endif
