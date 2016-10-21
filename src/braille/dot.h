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


#ifndef DOT_H
#define DOT_H

#include <string>
#include <vector>
#include <laterographics/modifiable.h>

#define SHAPE_SINE "sinusoid"
#define SHAPE_TRIANG "triangular"
#define SHAPE_GAUSS "gaussian"
#define SHAPE_SIGMOID "sigmoid"

class Dot : public latero::graphics::Modifiable
{
public:

	class SideParams
	{
	public:
		SideParams(){};
		~SideParams(){};
		
		float radius; // between 0 and 1
		float plateau; // between 0 and 1 (% of radius)
		std::string shape;
		float txAmp; // amplitude between 0.0 and 0.5
		unsigned int txNbCycles; // nb of full cycles (a half cycle is added)
	};

	Dot();
	Dot(const SideParams &left, const SideParams &right, bool locked);

	Dot operator=(const Dot& s);

	virtual ~Dot() {};


	/** @return a value between 0 and 1, based on distance from 0 to 1 from center */
	float GetValue(float distance, std::string shape, float plateau);

	/**
	 * @param d distance from center (negative if on the left)
	 * @return a value between 0 and 1
	 */
	float GetValue(float d);
	float GetValue_(float d);

	static std::vector<std::string> GetShapes();

	void Set(const SideParams &left, const SideParams &right, bool locked);

	bool GetLockedSides();
	SideParams GetLeft() const;
	SideParams GetRight() const;
	
	std::string GetDesc(std::string prefix);

	void SetTextureAmp(float l, float r);

protected:
	
	SideParams l_, r_;
	bool lockedSides_;
};

#endif
#endif
