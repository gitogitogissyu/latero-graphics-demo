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

#include "virtualsurfacewidget.h"
#include <laterographics/gtk/pixbufops.h>
#include <gtkmm.h>
#include <math.h>

#define UPDATE_RATE_MS 300

VirtualSurfaceArea::VirtualSurfaceArea(const latero::Tactograph *dev) :
	dev_(dev),
	tdState_(dev->GetFrameSizeX(), dev->GetFrameSizeY())
{
}



bool VirtualSurfaceArea::on_expose_event(GdkEventExpose* event)
{
	if (!bg_)
	{
		get_window()->clear();
		return true;
	}

	Cairo::RefPtr<Cairo::Context> cr = get_window()->create_cairo_context();
	if (event)
	{
		cr->rectangle(event->area.x, event->area.y, event->area.width, event->area.height);
    		cr->clip();
	}

	Glib::RefPtr<Gdk::Pixbuf> buf = bg_;
	if (buf)
	{
		buf = buf->scale_simple(GetWidth(),GetHeight(),Gdk::INTERP_NEAREST);
		buf->render_to_drawable(get_window(), get_style()->get_black_gc(),
   			0, 0, 0, 0, GetWidth(), GetHeight(), Gdk::RGB_DITHER_NONE, 0, 0);
	}

	// draw cursor
	double dpmm_x = GetWidth() / dev_->GetSurfaceWidth();
	double dpmm_y = GetHeight() / dev_->GetHeight();
	cr->scale(dpmm_x, dpmm_y); // scale to mm
	cr->translate(pos_, dev_->GetHeight()/2);	// shift origin to center of TD

	float motionRange = 0.6 * dev_->GetPitchX();
	int hPiezo = dev_->GetContactorSizeY();

	cr->set_line_width(0.5);
	cr->set_source_rgb(1.0, 0.0, 0.0);
	for (uint j=0; j< dev_->GetFrameSizeY(); ++j)
	{
		for (uint i=0; i< dev_->GetFrameSizeX(); ++i)
		{
			latero::graphics::Point p = dev_->GetActuatorOffset(i,j);
			float x = p.x + motionRange*(0.5-tdState_.Get(i,j));
			cr->move_to(x, p.y - 0.5*hPiezo);
	        	cr->line_to(x, p.y + 0.5*hPiezo);
			cr->stroke();
		}
	}
	return true;
}


void VirtualSurfaceArea::SetDisplayState(double pos, const latero::BiasedImg &f)
{
	assert(f.Size() ==  tdState_.Size());
	pos_ = pos;
	tdState_ = f;
	Invalidate();
}



void VirtualSurfaceArea::Invalidate()
{
    Glib::RefPtr<Gdk::Window> win = get_window();
    if (win)
    {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
}


void VirtualSurfaceArea::Set(Glib::RefPtr<Gdk::Pixbuf> bg)
{
	bg_ = bg;
	Invalidate();
}









bool VirtualSurfaceWidget::RefreshCursor()
{
	latero::BiasedImg frame = peer_->GetLatestFrame();
	surface_.SetDisplayState(peer_->GetLastPos(), frame);
	return true;
}


bool VirtualSurfaceWidget::OnCheckPeer()
{
	if (peer_->GetLastModified() > bgUpdateTime_)
		RefreshBackground();
	return true;
}

void VirtualSurfaceWidget::RefreshBackground()
{
	bgUpdateTime_ = boost::posix_time::microsec_clock::universal_time();
	latero::graphics::gtk::Animation anim = peer_->GetIllustration(surface_.GetWidth(),surface_.GetHeight());
	surface_.Set(anim);
}


void VirtualSurfaceWidget::on_size_allocate(Gtk::Allocation& allocation)
{
	EventBox::on_size_allocate(allocation);
	RefreshBackground();
}
#endif
