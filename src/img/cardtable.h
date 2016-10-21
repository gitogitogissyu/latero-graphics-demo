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

#ifndef CARD_TABLE_H
#define CARD_TABLE_H

#include "../config.h"
#ifndef DISABLE_GRAPHICS_DEMO

#include <gtkmm.h>
#include "card.h"

class CardTable : public Gtk::EventBox
{
public:
	CardTable(uint sx, uint sy);
	~CardTable();
	void SetCards(std::vector<Card*> &cards);
	void SetCard(uint x, uint y, Card* card);
	void RemoveCards();
	void GetLocation(Card* card, int &x, int &y);
	Card* GetCard(uint x, uint y);

protected:
	
	uint GetNbRows() { return table_.property_n_rows();	}
	uint GetNbCols() { return table_.property_n_columns();	}

	Gtk::Table table_;
};

#endif
#endif

