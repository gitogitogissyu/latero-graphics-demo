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

#ifndef BRAILLE_GEN_H
#define BRAILLE_GEN_H

#include "braillegenfwd.h"
#include <laterographics/generator.h>
#include "braillecell.h"
#include "dot.h"
#include "modulator.h"
#include "braillestring.h"

typedef enum {
	BRAILLE_MODE_SINGLE_COMPACT=0,
	BRAILLE_MODE_SINGLE_SPARSE=1,
	BRAILLE_MODE_SINGLE_DIAG=2,
	BRAILLE_MODE_DOUBLE=3,
	BRAILLE_MODE_DOUBLE_DOT_SHEAR=4,
	BRAILLE_MODE_DOUBLE_ROW_SHEAR=5
} braille_mode_t;

#define BRAILLE_MODE_SIZE (BRAILLE_MODE_DOUBLE_ROW_SHEAR+1)

class BrailleGen : public latero::graphics::Generator
{
public:
	static BrailleGenPtr Create(const latero::Tactograph *dev)
	{
		return BrailleGenPtr(new BrailleGen(dev));
	}

	virtual void RenderBraille_(const latero::graphics::State *state, latero::RangeImg &frame);

	virtual ~BrailleGen() {};

	virtual const char *GetName() { return "Braille"; }
	void AppendXML(latero::graphics::XMLOutputNode &root) const {};

	void SetOffset(float v);
	float GetOffset() const;

	void SetDotWidthRel(float v); // % of intraDotDist
	float GetDotWidthRel() const;
	void SetIntraDotDistRel(float v); // rel to standard Braille
	float GetIntraDotDistRel() const;
	void SetInterDotDistRel(float v); // rel to standard Braille
	float GetInterDotDistRel() const;

	void SetDotWidth(float v); // % of intraDotDist
	float GetDotWidth() const;
	void SetIntraDotDist(float v); // rel to standard Braille
	float GetIntraDotDist() const;
	void SetInterDotDist(float v); // rel to standard Braille
	float GetInterDotDist() const;

	braille_mode_t GetMode();
	void SetMode(braille_mode_t mode);

	void SetInvert(bool v);
	bool GetInvert() const;

	void SetVibFreq(float hz);
	float GetVibFreq() const;

	float FirstContactPos();

	virtual float GetDeflection_(float pos, uint row);

	virtual latero::graphics::gtk::Animation GetIllustration(uint w, uint h) const;

	virtual Gtk::Widget *CreateWidget(latero::graphics::GeneratorPtr ptr);

	float GetLastPos()
	{
		LATERO_GRAPHICS_GUARD;
		return lastPos_;
	}


protected:
	BrailleGen(const latero::Tactograph *dev);
	void BasicRender_(const latero::graphics::State *state, latero::RangeImg &frame);
	virtual void DoRender_(const latero::graphics::State *state, latero::BiasedImg &frame);


private:
	float GetVibration_(float ratio, float xPos, unsigned int row);
	float GetVibration_(float ratio, unsigned int row, BrailleCell *cell, float d0, float d1);
	float GetDeflection_(unsigned int row, BrailleCell *cell, float d0, float d1);

	float offset_;		/* offset at the begining of the string */

	float dotWidth_; 	/* width of dot */
	float intraDotDist_; 	/* distance between dots inside a cell */
	float interDotDist_; 	/* distance between adjacent dots between cells  */

	bool invert_;
	bool disableRow_[6];
	bool invertRow_[6];
	bool diagRow_;
	bool narrow_;
	braille_mode_t mode_; // todo: redundant?

	// vibration
	boost::posix_time::time_duration vibPeriod_;

	float lastPos_;

public: // submodules
	Dot dot_;
	Dot vibDot_;
	Modulator vibMod_;
	BrailleString str_;
	boost::posix_time::time_duration prevTime_;

};

#endif
#endif

