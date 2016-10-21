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

#ifndef CARDSET_H
#define CARDSET_H

#include "../config.h"
#ifndef DISABLE_GRAPHICS_DEMO

#include <vector>
#include <laterographics/generator.h>
#include "card.h"

class CardSet : public std::vector<Card*>
{
public:
	CardSet();
	virtual ~CardSet();

	bool Load(std::string file, const latero::Tactograph *dev, uint scale);

	inline void DeleteCards()
	{
		while (size())
		{
			delete back();
			pop_back();
		}
	}

	void Shuffle();

	std::string GetName() { return name_; }
private:
	std::string name_;
	std::vector<latero::graphics::GeneratorPtr> managedGenerators_;

};

#endif
#endif
