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

#ifndef ICON_GENERATOR_TOOLBAR
#define ICON_GENERATOR_TOOLBAR

#include "generator.h"
#include <laterographics/graphics/texture/texturefwd.h>
#include <gtkmm.h>

namespace IconDemo {

/** small toolbar with minimal controls for the generator parameters */
class Toolbar : public Gtk::Frame
{
public:
	Toolbar(GeneratorPtr peer, Gtk::Window *window);
	virtual ~Toolbar() {};
protected:
	void Rebuild();
	void OnAdvanced();
	void OnTextureChanged(latero::graphics::TexturePtr tx);

	Gtk::Window *window_;
	GeneratorPtr peer_;
};

}; // namespace

#endif
#endif // !DISABLED
