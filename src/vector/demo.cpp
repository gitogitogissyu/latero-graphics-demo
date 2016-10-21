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
#ifndef DISABLE_VECTOR_DEMO

#include "demo.h"
#include <laterographics/graphics/polygon.h>
#include <laterographics/graphics/line.h>
#include <laterographics/graphics/circle.h>

namespace VectorDemo {

Demo::Demo(const latero::Tactograph *dev) :
	preview_(dev, latero::graphics::GeneratorPtr(), true)
{
	using namespace latero::graphics;

	peer_ = Canvas::Create(dev);

	double w = dev->GetSurfaceWidth();
	double h = dev->GetSurfaceHeight();
	peer_->GetGroup()->InsertPattern(Circle::Create(dev, dev->GetSurfaceSize()/2.0, 50));
	peer_->GetGroup()->InsertPattern(Line::Create(dev, latero::graphics::Point(0.25*w,0.5*h), latero::graphics::Point(0.75*w,0.5*h)));
	//peer_->InsertObject(Line::Create(dev, latero::graphics::Point(50,60), latero::graphics::Point(175,60)));

	/*
	PolygonPtr poly = Polygon::Create(dev);
	poly->InsertPoint(Point(50,50));
	poly->InsertPoint(Point(50,150));
	poly->InsertPoint(Point(150,150));
	peer_->InsertObject(poly);	
	*/

	/*
	PolygonPtr poly = Polygon::Create(dev);
	poly->InsertPoint(Point(0.25*w, 0.25*h));
	poly->InsertPoint(Point(0.25*w, 0.75*h));
	poly->InsertPoint(Point(0.75*w, 0.75*h));
	poly->InsertPoint(Point(0.75*w, 0.25*h));
	poly->InsertPoint(Point(0.50*w, 0.50*h));
	peer_->InsertPattern(poly);	
	*/

	//BlendPtr blend = Blend::Create(dev);
	//peer_->InsertPattern(blend);

	//peer_->InsertObject(Dot::Create(dev, Point(0.75*w,0.75*h)));

	//DotsPtr dots = Dots::Create(dev);
	//dots->InsertPoint(Point(0.25*w,0.25*h));
	//peer_->InsertObject(dots);

	preview_.ShowCursor();
	preview_.AnimateCursor(true);
	preview_.SetGenerator(peer_);

	Gtk::VBox *vbox = manage(new Gtk::VBox);

	vectorWidget_ = (CanvasWidget*)manage(peer_->CreateWidget(peer_));

	add(*vbox);
	vbox->pack_start(preview_);
	vbox->pack_start(*vectorWidget_, Gtk::PACK_SHRINK);

	show_all_children();
};

}; // namespace

#endif
