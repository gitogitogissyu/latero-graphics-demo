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
#ifndef DISABLE_GRAPHICS_DEMO

#ifndef CARDCOLLECTION_H
#define CARDCOLLECTION_H

#include "cardset.h"

class CardCollection : public std::vector<CardSet*>
{
public:
	CardCollection();
	virtual ~CardCollection();

	bool Load(std::string file, const latero::Tactograph *dev, uint scale);

	CardSet *GetSet(std::string name)
	{
		for (uint i=0; i<size(); ++i)
			if ((*this)[i]->GetName() == name)
				return (*this)[i];
		return NULL;
	}


	std::string GetName() { return name_; }
private:
	std::string name_;

};

#endif
#endif
