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
#ifndef DISABLE_ICON_DEMO

#ifndef ICON_DEMO_H
#define ICON_DEMO_H

#include "../demo.h"
#include <laterographics/virtualsurfacewidget.h>
#include "generator.h"
#include "toolbar.h"

namespace IconDemo {

class Demo : public ::Demo
{
public:
	Demo(const latero::Tactograph *dev, Gtk::Window *window);
	virtual ~Demo() {};
	virtual latero::graphics::GeneratorPtr Gen() { return peer_; };
protected:

	GeneratorPtr peer_;
	latero::graphics::VirtualSurfaceWidget preview_;
	Toolbar genToolbar_;

	Gtk::VBox mainBox_;
};

}; // namespace

#endif
#endif // !DISABLED
