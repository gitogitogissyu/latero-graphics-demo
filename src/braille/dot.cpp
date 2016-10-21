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

#include "dot.h"
#include "math.h"
#include <assert.h>
#include <iostream>
#include <sstream>

Dot::Dot()
{
	SideParams side;
	side.radius = 1.0;
	side.plateau = 0;
	side.shape = SHAPE_SINE;
	side.txAmp = 0;
	side.txNbCycles = 3;
	Set(side, side, true);
}

Dot::Dot(const SideParams &left, const SideParams &right, bool locked) :
	l_(left),
	r_(right),
	lockedSides_(locked)
{
}

Dot Dot::operator=(const Dot& s)
{ 
	if (&s == this) return *this;

	LATERO_GRAPHICS_GUARD
	SetLastModified_();
	l_ = s.l_;
	r_ = s.r_;
	lockedSides_ = s.lockedSides_;

	return *this; 
};


void Dot::Set(const SideParams &left, const SideParams &right, bool locked)
{
	LATERO_GRAPHICS_GUARD
	SetLastModified_();
	lockedSides_ = locked;
	l_ = left;
	r_ = right;
}



float Dot::GetValue(float d)
{
	LATERO_GRAPHICS_GUARD
	return GetValue_(d);
}

float Dot::GetValue_(float d)
{
	SideParams *side = ((d<0) || lockedSides_)?&l_:&r_;

	assert(side->radius > 0);
	assert(side->radius <= 1.0);

	d = fabs(d/side->radius);
	float tx = side->txAmp * cos(d * 2 * M_PI * (side->txNbCycles + 0.5));
	float value = GetValue(d, side->shape, side->plateau);
	return tx + (1.0-(2.0*side->txAmp))*value + side->txAmp;
}


float Dot::GetValue(float d, std::string shape, float plateau)
{
	if (d < plateau)
		return 1.0;
	else if (d >= 1.0)
		return 0.0;
	else
		d = (d-plateau)/(1.0-plateau);


	if (shape == SHAPE_SINE)
	{
		return 0.5 + 0.5*cos(d * M_PI);
	}
	else if (shape == SHAPE_TRIANG)
	{
		return 1.0 - 1.0*d;
	}
	else if (shape == SHAPE_GAUSS)
	{
		float sigma = 0.5;
		return exp((-1.0 * d * d )/(2*sigma*sigma));
	}
	else if (shape == SHAPE_SIGMOID)
	{
		return 1.0 / (1.0 + exp(d - 7.0));
	}
	else
		return 0.0;
}

std::vector<std::string> Dot::GetShapes()
{
	std::vector<std::string> rv;
	rv.push_back(SHAPE_SINE);
	rv.push_back(SHAPE_TRIANG);
	rv.push_back(SHAPE_GAUSS);
	rv.push_back(SHAPE_SIGMOID);
	return rv;
}

bool Dot::GetLockedSides()
{
	LATERO_GRAPHICS_GUARD
	return lockedSides_;
}

Dot::SideParams Dot::GetLeft() const
{
	LATERO_GRAPHICS_GUARD
	return l_;
}

Dot::SideParams Dot::GetRight() const
{
	LATERO_GRAPHICS_GUARD
	return r_;
}

std::string Dot::GetDesc(std::string prefix)
{
	std::stringstream stream;
	SideParams l = GetLeft();
	SideParams r = GetRight();

	stream << prefix << "Dot\n";
	if (GetLockedSides())
	{
		stream << prefix << "radius: " << l.radius << "\n";
		stream << prefix << "plateau: " << l.plateau << "\n";
		stream << prefix << "shape: " << l.shape << "\n";
		stream << prefix << "texture amplitude: " << l.txAmp << "\n";
		stream << prefix << "texture nb cycles: " << l.txNbCycles<< "\n";
	}
	else
	{
		stream << prefix << "LEFT: \n";
		stream << prefix << "radius: " << l.radius << "\n";
		stream << prefix << "plateau: " << l.plateau << "\n";
		stream << prefix << "shape: " << l.shape << "\n";
		stream << prefix << "texture amplitude: " << l.txAmp << "\n";
		stream << prefix << "texture nb cycles: " << l.txNbCycles<< "\n";
		stream << prefix << "RIGHT: \n";
		stream << prefix << "radius: " << r.radius << "\n";
		stream << prefix << "plateau: " << r.plateau << "\n";
		stream << prefix << "shape: " << r.shape << "\n";
		stream << prefix << "texture amplitude: " << r.txAmp << "\n";
		stream << prefix << "texture nb cycles: " << r.txNbCycles<< "\n";
	}

	return stream.str();
}

void Dot::SetTextureAmp(float l, float r)
{
	LATERO_GRAPHICS_GUARD
	l_.txAmp = l;
	r_.txAmp = r;
}


#endif
