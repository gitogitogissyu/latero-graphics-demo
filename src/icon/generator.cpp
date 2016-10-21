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

#include "generator.h"
#include <laterographics/graphics/canvas.h>
#include <laterographics/graphics/texture/texture.h>
#include <laterographics/graphics/circle.h>
#include <laterographics/graphics/polygon.h>
#include <laterographics/grating.h>
#include <laterographics/graphics/stroke.h>
#include <laterographics/graphics/texture/stock.h>

namespace IconDemo {

Generator::Generator(const latero::Tactograph *dev) :
	latero::graphics::Canvas(dev),
	areaTexture_(latero::graphics::Texture::Create(dev,latero::graphics::tx_grating_horizontal)),
    sizeId_(SIZE_L),
    contourThickness_(CONTOUR_THICK_S),
	contour_(CONTOUR_VIB),
	shape_(SHAPE_PLUS)
{
	UpdateGen();
}


void Generator::SetContour(int id)
{
	contour_ = id;
	UpdateGen();
}

int Generator::GetContour()
{
	return contour_;
}

void Generator::SetContourThickness(int id)
{
	contourThickness_ = id;
	stroke_->SetWidth(GetStrokeWidth());
}

int Generator::GetContourThickness()
{
	return contourThickness_;
}

void Generator::SetShape(int shape)
{
	shape_ = shape;
	UpdateGen();
}

int Generator::GetShape()
{
	return shape_;
}

void Generator::SetSize(int id)
{
	sizeId_ = id;
	UpdateGen();
}

int Generator::GetSize()
{
	return sizeId_;
}

double Generator::GetSize_()
{
	int id = GetSize();
	if (id == SIZE_S) return 30;
	else if (id == SIZE_M) return 60;
	else if (id == SIZE_L) return 90;
    else return 30; // default
}


void Generator::UpdateGen()
{
	using namespace latero::graphics;
	GetGroup()->ClearPatterns();

	if (shape_ == SHAPE_CIRCLE)
	{
		CirclePtr p = Circle::Create(Dev(), Dev()->GetSurfaceSize()/2, GetSize_()/2);
		stroke_ = p->GetStroke();
		icon_ = p;
	}
	else
	{
		std::vector<latero::graphics::Point> points = GetPolygon(shape_, GetSize_());
		for (unsigned int i=0; i<points.size(); ++i) 
            points[i] += Dev()->GetSurfaceSize()/2;
		PolygonPtr p = Polygon::Create(Dev());
		icon_ = p;
		stroke_ = p->GetStroke();
		p->SetPoints(points);
	}

	// fill texture
	icon_->SetFillTexture(GetAreaTexture());
	icon_->SetFillTextureEnable(true);
	icon_->SetFillTextureGap(1.0);

	// stroke
	if (contour_ == CONTOUR_NONE)
	{
		stroke_->SetWidth(0);
	}
	else if (contour_ == CONTOUR_VIB)
	{
		stroke_->SetWidth(GetStrokeWidth());
		stroke_->GetOscillator()->SetFreq(50);
		stroke_->GetOscillator()->SetEnable(true);
	}
	else if (contour_ == CONTOUR_STROKE)
	{
		stroke_->SetWidth(GetStrokeWidth());
		stroke_->GetTexture()->SetRidgeSize(2);
		stroke_->GetTexture()->Regularize();
		stroke_->GetTexture()->SetEnable(true);
	}
	else if (contour_ == CONTOUR_DOT)
	{
		stroke_->SetWidth(GetStrokeWidth());
		stroke_->SetDotEnable(true);
	}

	GetGroup()->InsertPattern(icon_);
}

latero::graphics::TexturePtr Generator::GetAreaTexture() const
{
	return areaTexture_;
}

void Generator::SetAreaTexture(const latero::graphics::TexturePtr tx)
{
	areaTexture_ = tx;
	if (icon_) icon_->SetFillTexture(tx);
}


double Generator::GetStrokeWidth()
{
	int id = GetContourThickness();
	if (id == CONTOUR_THICK_S)	return 2;
	else if (id == CONTOUR_THICK_M)	return 4;
	else if (id == CONTOUR_THICK_L)	return 6;
    else return 2; // default
}

std::vector<latero::graphics::Point> Generator::GetPolygon(int shape, double size)
{
	std::vector<latero::graphics::Point> rv;
	if (shape == Generator::SHAPE_SQUARE)
	{
		rv.push_back(latero::graphics::Point(-size/2, -size/2));
		rv.push_back(latero::graphics::Point(size/2, -size/2));
		rv.push_back(latero::graphics::Point(size/2, size/2));
		rv.push_back(latero::graphics::Point(-size/2, size/2));
	}
	else if (shape == Generator::SHAPE_TRIANGLE)
	{
		float d = size/2;
		rv.push_back(latero::graphics::Point(-d, -d));
		rv.push_back(latero::graphics::Point(d, -d));
		rv.push_back(latero::graphics::Point(0, d));
	}
	else if  (shape == Generator::SHAPE_RATRIANG)
	{
		float d = size/2;
		rv.push_back(latero::graphics::Point(-d, -d));
		rv.push_back(latero::graphics::Point(d, d));
		rv.push_back(latero::graphics::Point(-d, d));
	}
	else if  (shape == Generator::SHAPE_DIAMOND)
	{
		float d = size/2;
		rv.push_back(latero::graphics::Point(0, -d));
		rv.push_back(latero::graphics::Point(d, 0));
		rv.push_back(latero::graphics::Point(0, d));
		rv.push_back(latero::graphics::Point(-d, 0));
	}
	else if  (shape == Generator::SHAPE_PLUS)
	{
		double a = size/6;
		double b = size/2;

		rv.push_back(latero::graphics::Point(-b, -a));
		rv.push_back(latero::graphics::Point(-a, -a));
		rv.push_back(latero::graphics::Point(-a, -b));
		rv.push_back(latero::graphics::Point(a, -b));
		rv.push_back(latero::graphics::Point(a, -a));
		rv.push_back(latero::graphics::Point(b, -a));
		rv.push_back(latero::graphics::Point(b, a));
		rv.push_back(latero::graphics::Point(a, a));
		rv.push_back(latero::graphics::Point(a, b));
		rv.push_back(latero::graphics::Point(-a, b));
		rv.push_back(latero::graphics::Point(-a, a));
		rv.push_back(latero::graphics::Point(-b, a));
	}

	return rv;
}

} // namespace

#endif // !DISABLED
