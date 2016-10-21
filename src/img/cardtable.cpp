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

#include "cardtable.h"

CardTable::CardTable(uint sx, uint sy) :
	table_(sy,sx)
{
	add(table_);
	table_.set_border_width(10);
	table_.set_spacings(10);
}

CardTable::~CardTable()
{
}

void CardTable::SetCards(std::vector<Card*> &cards)
{
	RemoveCards();
	for (uint x=0; x<GetNbCols(); ++x)
		for (uint y=0; y<GetNbRows(); ++y)
			SetCard(x, y, cards[y*GetNbCols()+x]);
}

void CardTable::SetCard(uint x, uint y, Card* card)
{
	/** @todo: doesn't work if a card is already at that location */
	table_.attach(*card, x, x+1, y, y+1);
	card->UpdateImg();
}

void CardTable::RemoveCards()
{
	Gtk::Table::TableList list= table_.children();
	for(Gtk::Table::TableList::iterator iter = list.begin(); iter != list.end(); ++iter)
	{
		Card *card = (Card*)(*iter).get_widget();
		card->ClearImg();
	}

	table_.children().clear();
}

void CardTable::GetLocation(Card* card, int &x, int &y)
{
	for (x=0; x<(int)GetNbCols(); ++x)
		for (y=0; y<(int)GetNbRows(); ++y)
			if (GetCard(x,y) == card)
				return;
	assert(0); 
}

Card* CardTable::GetCard(uint x, uint y)
{
	Gtk::Table::TableList list= table_.children();
	for(Gtk::Table::TableList::iterator iter = list.begin(); iter != list.end(); ++iter)
	{
		if (((*iter).get_left_attach() == x) && ((*iter).get_top_attach() == y))
			return (Card*)(*iter).get_widget();
	}
	return NULL;
}
#endif
