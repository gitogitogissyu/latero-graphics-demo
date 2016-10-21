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

#include "positiongraph.h"

#define NB_POINTS 1000

PositionGraph::PositionGraph(BrailleGenPtr peer) :
	Gtk::Table(1,peer->Dev()->GetFrameSizeY()),
	peer_(peer), y_(0)
{
	set_row_spacings(5);
	set_size_request(-1, 100);

	for (uint i=0; i<peer->Dev()->GetFrameSizeY(); ++i)
	{
		latero::graphics::gtk::Plot* plot = manage(new latero::graphics::gtk::Plot);
		plots_.push_back(plot);
		plot->SetRangeX(0, peer_->Dev()->GetSurfaceWidth());
		plot->SetRangeY(-1.0, 1.0);
		attach(*plot,0,1,i,i+1);
	}

	Glib::signal_timeout().connect(
		sigc::mem_fun(*this, &PositionGraph::Monitor),
		500, Glib::PRIORITY_DEFAULT_IDLE);
}


bool PositionGraph::Monitor()
{
	if ((lastUpdated_ < peer_->GetLastModified()))
	{	
		lastUpdated_ = boost::posix_time::microsec_clock::universal_time();
		y_ = 0;
		Glib::signal_idle().connect(sigc::mem_fun(*this, &PositionGraph::OnRefreshRow));
	}
	return true;
}


bool PositionGraph::OnRefreshRow()
{
	if (y_<plots_.size())
	{
		plots_[y_]->Clear();
		float incr = peer_->Dev()->GetSurfaceWidth() / (float)NB_POINTS;
		for (float x=0; x<peer_->Dev()->GetSurfaceWidth(); x+=incr)
		{
			float v = peer_->GetDeflection_(x, y_);
			plots_[y_]->InsertPoint(x, v);
		}
		plots_[y_]->Draw();
		y_++;
		return true;
	}
	return false;
}


#endif
