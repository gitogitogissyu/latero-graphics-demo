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

#ifndef SCHOOLBOOK_DEMO_H
#define SCHOOLBOOK_DEMO_H

#include "../config.h"

#ifndef DISABLE_SCHOOLBOOK_DEMO

#include "../demo.h"
#include <laterographics/virtualsurfacewidget.h>
#include "../generatorhandle.h"

#define NB_BUTTONS 4

namespace SchoolbookDemo {

class Demo : public ::Demo
{
public:
	Demo(const latero::Tactograph *dev);
	virtual ~Demo();

	virtual latero::graphics::GeneratorPtr Gen() { return peer_; };

private:
	void OnSelChange();
	bool OnKeyPress(GdkEventKey* event);
	void Select(int i);
	void CreateGenerators();

	latero::graphics::VirtualSurfaceWidget preview_;
	Gtk::RadioButton selButton_[NB_BUTTONS];

	int sel_;
	std::vector<latero::graphics::GeneratorPtr> list_;
	GeneratorHandlePtr peer_;
	const latero::Tactograph *dev_;
};

}; // namespace

#endif
#endif
