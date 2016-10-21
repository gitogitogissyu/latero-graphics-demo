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

#include "dotgraph.h"

#define NB_POINTS 1000

DotGraph::DotGraph(Dot *dot) :
	plot_("blue", "white"),
	dot_(dot)
{	
	plot_.set_size_request(200, 200);
	plot_.SetRangeX(-1.0, 1.0);
	plot_.SetRangeY(0.0, 1.0);
	pack_start(plot_);
}

void DotGraph::Refresh()
{
	plot_.Clear();
	float incr = 2.0 / NB_POINTS;
	for (float x=-1.0; x<1.0; x+=incr)
	{
		float y = dot_->GetValue_(x);
		plot_.InsertPoint(x, y);
	}
	plot_.Draw();
}


#endif
