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

#ifndef MODULATOR_H
#define MODULATOR_H

#include <laterographics/modifiable.h>
#include <string>

class Modulator : public latero::graphics::Modifiable
{
public:
	Modulator();
	virtual ~Modulator();
	double GetFixedAmp();
	void SetFixedAmp(float v);
	inline double GetFixedAmp_() { return fixedAmp_; }

	double GetAmp(double vel, double period);
	double GetAmp_(double vel, double period);

	void SetModeFixed();
	void SetModeVel();

	void GetRampRange(double &vmin, double &vmax);
	void SetRampRange(double vmin, double vmax);

	void GetRampAmpRange(double &vmin, double &vmax);
	void SetRampAmpRange(double vmin, double vmax);

	double GetMaxRiseRate();
	void SetMaxRiseRate(double v);
	double GetMaxFallRate();
	void SetMaxFallRate(double v);

	std::string GetDesc(std::string prefix);

protected:
	double fixedAmp_;
	int mode_;
	double vmin_;
	double vmax_;
	double amin_;
	double amax_;
	double maxRiseRate_; // in second per full transition (0.0 to 1.0)
	double maxFallRate_;

	double amp_;
};

#endif
#endif
