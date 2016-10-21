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

#ifndef TEXTURE_DEMO_H
#define TEXTURE_DEMO_H

#include "../demo.h"
#include <laterographics/virtualsurfacewidget.h>
#include <laterographics/graphics/image.h>
#include <laterographics/graphics/canvas.h>


namespace TextureDemo {

class Demo : public ::Demo
{
public:
	Demo(const latero::Tactograph *dev);
	virtual ~Demo() {};
	virtual latero::graphics::GeneratorPtr Gen() { return gen_; };
protected:
	Glib::RefPtr<Gdk::Pixbuf> GetMask(const latero::Tactograph *dev);
	bool RefreshCursor();
	void OnChange();

	latero::graphics::VirtualSurfaceWidget preview_;
	latero::graphics::CanvasPtr gen_;
	latero::graphics::ImagePtr peer_;
	Gtk::Widget *waveWidget_;
};

}; // namespace

#endif
#endif
