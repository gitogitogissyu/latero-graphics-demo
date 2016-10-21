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

#ifndef ICON_DEMO_GENERATOR_H
#define ICON_DEMO_GENERATOR_H

#include <laterographics/graphics/texture/texturefwd.h>
#include "../generatorhandle.h"
#include <laterographics/graphics/canvas.h>
#include <laterographics/graphics/closedpatternfwd.h>
#include <laterographics/graphics/strokefwd.h>

namespace IconDemo {

class Generator;
typedef boost::shared_ptr<Generator> GeneratorPtr;

class Generator : public latero::graphics::Canvas
{
public:
	enum {SIZE_S, SIZE_M, SIZE_L};
	enum {CONTOUR_THICK_S, CONTOUR_THICK_M, CONTOUR_THICK_L};
	enum {CONTOUR_VIB, CONTOUR_NONE, CONTOUR_DOT, CONTOUR_STROKE};
	enum {SHAPE_CIRCLE, SHAPE_TRIANGLE, SHAPE_SQUARE, SHAPE_RATRIANG, SHAPE_DIAMOND, SHAPE_PLUS};

public:
	Generator(const latero::Tactograph *dev);
	virtual ~Generator() {};

	virtual void AppendXML(latero::graphics::XMLOutputNode &root) {};

	void SetContour(int id);
	int GetContour();

	void SetContourThickness(int id);
	int GetContourThickness();

	void SetShape(int shape);
	int GetShape();

	void SetSize(int shape);
	int GetSize();

	latero::graphics::TexturePtr GetAreaTexture() const;
	void SetAreaTexture(const latero::graphics::TexturePtr tx);

protected:

	std::vector<latero::graphics::Point> GetPolygon(int shape, double size);
	double GetSize_();
	double GetStrokeWidth();

	void UpdateGen();
	latero::graphics::StrokePtr stroke_;
	latero::graphics::ClosedPatternPtr icon_;
	latero::graphics::TexturePtr areaTexture_;

	int sizeId_;
	int contourThickness_;
	int contour_;
	int shape_;
};

} // namespace

#endif
#endif // !DISABLED
