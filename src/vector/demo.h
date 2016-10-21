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

#ifndef VECTOR_DEMO_H
#define VECTOR_DEMO_H

#include <laterographics/virtualsurfacewidget.h>
#include <laterographics/graphics/canvas.h>
#include <laterographics/graphics/canvaswidget.h>
#include "../demo.h"


namespace VectorDemo {

class Demo : public ::Demo
{
public:
	Demo(const latero::Tactograph *dev);
	virtual ~Demo() {};
	virtual latero::graphics::GeneratorPtr Gen() { return peer_; };
protected:
	bool RefreshCursor();
	void OnChange();

	latero::graphics::VirtualSurfaceWidget preview_;
	latero::graphics::CanvasPtr peer_;
	latero::graphics::CanvasWidget *vectorWidget_;
};

}; // namespace

#endif
#endif
