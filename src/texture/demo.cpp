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
#ifndef DISABLE_TEXTURE_DEMO

#include "demo.h"
#include <laterographics/graphics/texture/stock.h>
#include <laterographics/graphics/texture/texture.h>
#include <laterographics/graphics/group.h>


namespace TextureDemo {

Demo::Demo(const latero::Tactograph *dev) :
	preview_(dev, latero::graphics::GeneratorPtr(), true)
{
	using namespace latero::graphics;
	gen_ = Canvas::Create(dev);
	peer_ = Image::Create(dev,GetMask(dev),Texture::Create(dev,latero::graphics::tx_grating_horizontal));
	gen_->GetGroup()->InsertPattern(peer_);
	preview_.ShowCursor();
	preview_.AnimateCursor(true);
	preview_.SetGenerator(gen_);

	Gtk::VBox *vbox = manage(new Gtk::VBox);

	waveWidget_ = manage(peer_->CreateWidget(peer_));

	add(*vbox);
	vbox->pack_start(preview_);
	vbox->pack_start(*waveWidget_, Gtk::PACK_SHRINK);

	show_all_children();
};

Glib::RefPtr<Gdk::Pixbuf> Demo::GetMask(const latero::Tactograph *dev)
{
	int h = 1000;
	int w = 1000*dev->GetSurfaceWidth()/dev->GetSurfaceHeight();

	Glib::RefPtr<Gdk::Pixbuf> buf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, w, h);
	Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(
		(unsigned char*)buf->get_pixels(),
		Cairo::FORMAT_ARGB32,
		buf->get_width(),
		buf->get_height(),
		buf->get_rowstride()
	);
	Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);

	// clear to black
	cr->set_source_rgb(0.0, 0.0, 0.0); // bgr
	cr->paint();

	cr->set_source_rgb(1.0, 1.0, 1.0); // bgr
	//cr->rectangle(0.1*w,0.1*h,0.8*w,0.8*h);
	cr->rectangle(0,0,w,h);
	cr->fill();
 	return buf;
}


}; // namespace

#endif
