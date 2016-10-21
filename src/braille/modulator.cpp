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


#include "modulator.h"
#include <sstream>
#include <assert.h>
#include <math.h>

#define MODE_FIXED 0
#define MODE_VEL 1

Modulator::Modulator() :
	fixedAmp_(0),
	mode_(MODE_FIXED),
	vmin_(5),
	vmax_(10),
	amin_(0),
	amax_(1),
	maxRiseRate_(0.05),
	maxFallRate_(0.05)
{
}

Modulator::~Modulator()
{
}

double Modulator::GetFixedAmp()
{
	LATERO_GRAPHICS_GUARD
	return GetFixedAmp_();
}

void Modulator::SetFixedAmp(float v)
{
	LATERO_GRAPHICS_GUARD
	SetLastModified_();
	fixedAmp_ = v;
}

double Modulator::GetAmp(double vel, double period)
{
	LATERO_GRAPHICS_GUARD
	return GetAmp_(vel, period);
}

double Modulator::GetAmp_(double vel, double period)
{
	if (mode_ == MODE_FIXED)
	{
		amp_ = fixedAmp_;
	}
	else if (mode_ == MODE_VEL)
	{
		vel = fabs(vel);

		double targetAmp;
		if (vel < vmin_)
			targetAmp = amax_;
		else if (vel < vmax_)
			targetAmp = amax_ - ((vel-vmin_)/(vmax_-vmin_))*(amax_ - amin_);
		else
			targetAmp = amin_;

		// TODO
		double maxRise = 1.0 / (maxRiseRate_ / period);
		double maxFall = 1.0 / (maxFallRate_ /period);
		double deltaAmp = targetAmp - amp_;
		if (deltaAmp > maxRise)
			amp_ += maxRise;
		else if (-deltaAmp > maxFall)
			amp_ -= maxFall;
		else
			amp_ = targetAmp;

		amp_ = targetAmp;
	}
	else
	{
		amp_ = 0.0;
	}

	return amp_;
}

void Modulator::SetModeFixed()
{
	LATERO_GRAPHICS_GUARD
	mode_ = MODE_FIXED;
}

void Modulator::SetModeVel()
{
	LATERO_GRAPHICS_GUARD
	mode_ = MODE_VEL;
}

void Modulator::SetRampRange(double vmin, double vmax)
{
	assert(vmax > vmin);
	LATERO_GRAPHICS_GUARD
	vmin_ = vmin;
	vmax_ = vmax;
}

void Modulator::GetRampRange(double &vmin, double &vmax)
{
	LATERO_GRAPHICS_GUARD
	vmin = vmin_;
	vmax = vmax_;
}

void Modulator::SetRampAmpRange(double amin, double amax)
{
	assert(amax > amin);
	LATERO_GRAPHICS_GUARD
	amin_ = amin;
	amax_ = amax;
}

void Modulator::GetRampAmpRange(double &amin, double &amax)
{
	LATERO_GRAPHICS_GUARD
	amin = amin_;
	amax = amax_;
}


double Modulator::GetMaxRiseRate()
{
	LATERO_GRAPHICS_GUARD
	return maxRiseRate_;
}

void Modulator::SetMaxRiseRate(double v)
{
	LATERO_GRAPHICS_GUARD
	maxRiseRate_ = v;
}

double Modulator::GetMaxFallRate()
{
	LATERO_GRAPHICS_GUARD
	return maxFallRate_;
}

void Modulator::SetMaxFallRate(double v)
{
	LATERO_GRAPHICS_GUARD
	maxFallRate_ = v;
}

std::string Modulator::GetDesc(std::string prefix)
{
	std::stringstream stream;

	LATERO_GRAPHICS_GUARD
	if (mode_ == MODE_FIXED)
	{
		stream << prefix << "Fixed Amplitude: " << fixedAmp_ << "\n";
	}
	else if (mode_ == MODE_VEL)
	{
		stream << prefix << "Velocity Range: " << vmin_ << "mm/s to " << vmax_ << " mm/s\n";
		stream << prefix << "Amplitude Range: " << amin_ << " to " << amax_ << "\n";
		stream << prefix << "Max rate: " << maxRiseRate_ << " s (rising) and " << maxFallRate_ << " s (falling)\n";
	}
	return stream.str();
}

#endif
