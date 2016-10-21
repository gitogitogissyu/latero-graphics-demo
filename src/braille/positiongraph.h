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

#ifndef POSITION_GRAPH_H
#define POSITION_GRAPH_H

#include "braillegen.h"
#include <laterographics/gtk/plot.h>
#include <gtkmm/table.h>
#include <vector>

class PositionGraph : public Gtk::Table
{
public:
	PositionGraph(BrailleGenPtr peer);
	virtual ~PositionGraph() {};

protected:
	bool Monitor();
	bool OnRefreshRow();

	std::vector<latero::graphics::gtk::Plot*> plots_;
	BrailleGenPtr peer_;
	uint y_;
	boost::posix_time::ptime lastUpdated_;
};

#endif
#endif
