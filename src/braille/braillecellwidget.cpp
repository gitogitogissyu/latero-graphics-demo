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

#include "braillecellwidget.h"
#include <gtkmm/table.h>

BrailleCellWidget::BrailleCellWidget(BrailleCell *peer):
	peer_(peer),
	disableSignals_(false)
{
	Refresh();

	Gtk::Table *table = new Gtk::Table(2,3);
	add(*manage(table));

	for (uint x=0; x<2; ++x)
	{
		for (uint y=0; y<3; ++y)
		{
			table->attach(dots_[x][y], x, x+1, y, y+1, Gtk::SHRINK, Gtk::SHRINK);
			dots_[x][y].signal_toggled().connect(
				sigc::mem_fun(*this, &BrailleCellWidget::OnChange));
			dots_[x][y].set_active(peer->Get(x,y));
		}
	}
}

void BrailleCellWidget::OnChange()
{
	if (!disableSignals_)
	{
		for (uint x=0; x<2; ++x)
			for (uint y=0; y<3; ++y)
				peer_->Set(x,y,dots_[x][y].get_active());

		signal_value_changed();
	}
}

void BrailleCellWidget::Refresh()
{
	for (uint x=0; x<2; ++x)
		for (uint y=0; y<3; ++y)
			dots_[x][y].set_active(peer_->Get(x,y));
}

void BrailleCellWidget::SetPattern(unsigned char pattern)
{
	disableSignals_ = true;
	dots_[0][0].set_active(pattern&DOT_1);
	dots_[0][1].set_active(pattern&DOT_2);
	dots_[0][2].set_active(pattern&DOT_3);
	dots_[1][0].set_active(pattern&DOT_4);
	dots_[1][1].set_active(pattern&DOT_5);
	dots_[1][2].set_active(pattern&DOT_6);
	disableSignals_ = false;
}

#endif
