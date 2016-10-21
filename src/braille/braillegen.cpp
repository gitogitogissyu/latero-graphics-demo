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

#include "braillegen.h"
#include "braillegenwidget.h"

#define NB_BRAILLE_CELLS 5

/** standard Braille dimension in mm */
// center to center distance between dots of the same cell
#define STANDARD_INTRA_DOT_DIST 2.290
// center to center distance between adjacent dots of two cells
#define STANDARD_INTER_DOT_DIST 3.710
// width of a dot
#define STANDARD_DOT_WIDTH 1.500

// TODO
#define NB_ROWS 6 

// NOTES
//
// dot centers:
// ldot_center = offset_ + cell_nb*(intraDotDist_ + interDotDist_) + dotWidth_/2.0
// rdot_center = ldot_center + intraDotDist_

BrailleGen::BrailleGen(const latero::Tactograph *dev) :
	latero::graphics::Generator(dev),
	offset_(20),
	dotWidth_(0.7*3.664),
	intraDotDist_(3.664),
	interDotDist_(7.420),
	invert_(true),
	str_(NB_BRAILLE_CELLS)
{
	prevTime_ = boost::posix_time::seconds(0); // TODO: is this ok?

	Dot::SideParams vibParam;
	vibParam.radius = 1.0;
	vibParam.plateau = 0.5;
	vibParam.shape = SHAPE_SINE;
	vibParam.txAmp = 0;
	vibParam.txNbCycles = 1; 
	vibDot_.Set(vibParam, vibParam, true);

	SetVibFreq(25.0);
	SetMode(BRAILLE_MODE_SINGLE_SPARSE);
	AddModifiableChild(&dot_);
	AddModifiableChild(&vibDot_);
	AddModifiableChild(&vibMod_);
	AddModifiableChild(&str_);

	str_.Set("a");
}
	
void BrailleGen::RenderBraille_(const latero::graphics::State *state, latero::RangeImg &frame)
{
	// 1. Position
	BasicRender_(state, frame);

	// 2. Vibration
	double t = state->GetTimeElapsed().total_microseconds() % vibPeriod_.total_microseconds();
	double ratio = t / (double)vibPeriod_.total_microseconds();

	double vibAmp = vibMod_.GetAmp_(state->velMag, (state->GetTimeElapsed() - prevTime_).total_microseconds()*1000); // todo: ok?
	prevTime_ = state->GetTimeElapsed();
	for (uint i=0; i<Dev()->GetFrameSizeX(); ++i)
	{
		for (uint j=0; j<Dev()->GetFrameSizeY(); ++j)
		{
			double x = Dev()->GetActuatorPositions(state->pos, 0).Get(i,j).x; // TODO: expensive!
			float vib = vibAmp * GetVibration_(ratio, x, j);
			if (((i%2) == (j%2)))
				vib *= -1.0;

			// TODO
			// This doesn't quite work! The normal rendering leaves the pins on the right. The
			// vibration should do the same but it doesn't because it inverts the phase. The solution
			// is to obtain the vibration as a deviation from the right that we scale...

			float v =  vib + (1.0 - vibAmp) * frame.Get(i,j); 
			frame.Set(i,j,v);
		}
	}
}

float BrailleGen::FirstContactPos()
{
	float lastPiezoOffset = (Dev()->GetFrameSizeX()-1) * Dev()->GetPitchX();
	float stimulusStart = GetOffset() + (1-dot_.GetLeft().radius)*GetDotWidth()/2;
	return lastPiezoOffset + stimulusStart;
}


float BrailleGen::GetVibration_(float ratio, float xPos, unsigned int row)
{
	if (row >= NB_ROWS)
		return (invert_?1.0:-1.0);

	xPos -= offset_;

	float charSpacing = intraDotDist_ + interDotDist_;
	float dotRadius = dotWidth_ / 2;

	int c = (int)floor(xPos/charSpacing);
	float p = xPos - c*charSpacing;

	BrailleCell *cell = NULL;
	if ((xPos >= 0) && (c<(int)str_.GetSize()))
		cell = str_.GetCell(c);

	float d0 = (p - dotRadius) / dotRadius;
	float d1 = (p - dotRadius - intraDotDist_)  / dotRadius;

	float v = GetVibration_(ratio, row, cell, d0, d1);
	return v;
}


float BrailleGen::GetVibration_(float ratio, unsigned int row, BrailleCell *cell, float d0, float d1)
{
	if (disableRow_[row])
		return (invert_?1.0:-1.0);

	float inv = 1.0;
	if (invertRow_[row])
		inv = -1.0;

	int r = row/2;
	if (narrow_)
		if (row > 2)
			return -1.0*inv;
		else
			r = row;

	float rv = (invert_?1.0:-1.0);;
	if (cell)
	{
		if (fabs(d0) <= 1.0)
		{
			if (cell->Get_(0,r) && !(diagRow_ && (row%2)))
				rv = sin(2.0*M_PI*ratio) * vibDot_.GetValue(d0);
		}
		else if (fabs(d1) <= 1.0)
		{
			if (cell->Get_(1,r) && !(diagRow_ && !(row%2)))
				rv = sin(2.0*M_PI*ratio) * vibDot_.GetValue(d1);
		}
	}

	return rv*inv;
}




float BrailleGen::GetDeflection_(float xPos, unsigned int row)
{
	if (row >= NB_ROWS)
		return (invert_?1.0:-1.0);

	xPos -= offset_;

	float charSpacing = intraDotDist_ + interDotDist_;
	float dotRadius = dotWidth_ / 2;

	int c = (int)floor(xPos/charSpacing);
	float p = xPos - c*charSpacing;

	BrailleCell *cell = NULL;
	if ((xPos >= 0) && (c<(int)str_.GetSize()))
		cell = str_.GetCell(c);

	float d0 = (p - dotRadius) / dotRadius;
	float d1 = (p - dotRadius - intraDotDist_)  / dotRadius;

	float v = GetDeflection_(row, cell, d0, d1);
	if (invert_)
		v *= -1.0;
	return v;
}

float BrailleGen::GetDeflection_(unsigned int row, BrailleCell *cell, float d0, float d1)
{
	if (disableRow_[row])
		return -1.0;

	float inv = 1.0;
	if (invertRow_[row])
		inv = -1.0;

	int r = row/2;
	if (narrow_)
		if (row > 2)
			return -1.0*inv;
		else
			r = row;

	float rv = -1.0;
	if (cell)
	{
		if (fabs(d0) <= 1.0)
		{
			if (cell->Get_(0,r) && !(diagRow_ && (row%2)))
				rv = -1.0 + 2.0*dot_.GetValue(d0);
		}
		else if (fabs(d1) <= 1.0)
		{
			if (cell->Get_(1,r) && !(diagRow_ && !(row%2)))
				rv = -1.0 + 2.0*dot_.GetValue(d1);
		}
	}

	return rv*inv;
}


void BrailleGen::SetOffset(float v)
{
	LATERO_GRAPHICS_GUARD
	SetLastModified_();
	offset_ = v;
}

void BrailleGen::SetInvert(bool v)
{
	LATERO_GRAPHICS_GUARD
	SetLastModified_();
	invert_ = v;
}

bool BrailleGen::GetInvert() const
{
	LATERO_GRAPHICS_GUARD;
	return invert_;
}


float BrailleGen::GetOffset() const
{
	LATERO_GRAPHICS_GUARD;
	return offset_;
}

void BrailleGen::SetVibFreq(float hz)
{
	LATERO_GRAPHICS_GUARD
	SetLastModified_();
	vibPeriod_ = boost::posix_time::microseconds(1E6/hz);
}

float BrailleGen::GetVibFreq() const
{
	LATERO_GRAPHICS_GUARD;
	return 1E6/vibPeriod_.total_microseconds();
}


void BrailleGen::SetDotWidth(float v)
{
	LATERO_GRAPHICS_GUARD
	SetLastModified_();
	dotWidth_ = v;
}

float BrailleGen::GetDotWidth() const
{
	LATERO_GRAPHICS_GUARD;
	return dotWidth_;
}

void BrailleGen::SetIntraDotDist(float v)
{
	LATERO_GRAPHICS_GUARD
	SetLastModified_();
	intraDotDist_ = v;
}

float BrailleGen::GetIntraDotDist() const
{
	LATERO_GRAPHICS_GUARD;
	return intraDotDist_;
}

void BrailleGen::SetInterDotDist(float v)
{
	LATERO_GRAPHICS_GUARD
	SetLastModified_();
	interDotDist_ = v;
}

float BrailleGen::GetInterDotDist() const
{
	LATERO_GRAPHICS_GUARD;
	return interDotDist_;
}

void BrailleGen::SetDotWidthRel(float v)
{
	LATERO_GRAPHICS_GUARD
	SetLastModified_();
	dotWidth_ = v * intraDotDist_;

}

float BrailleGen::GetDotWidthRel() const
{
	LATERO_GRAPHICS_GUARD;
	return dotWidth_ / intraDotDist_;
}

void BrailleGen::SetIntraDotDistRel(float v)
{
	LATERO_GRAPHICS_GUARD
	SetLastModified_();
	dotWidth_ = (dotWidth_ / intraDotDist_) * v;
	intraDotDist_ = v * STANDARD_INTRA_DOT_DIST;
}

float BrailleGen::GetIntraDotDistRel() const
{
	LATERO_GRAPHICS_GUARD;
	return intraDotDist_ / STANDARD_INTRA_DOT_DIST;
}

void BrailleGen::SetInterDotDistRel(float v)
{
	LATERO_GRAPHICS_GUARD
	SetLastModified_();
	interDotDist_ = v  * STANDARD_INTER_DOT_DIST;
}

float BrailleGen::GetInterDotDistRel() const
{
	LATERO_GRAPHICS_GUARD;
	return interDotDist_ /  STANDARD_INTER_DOT_DIST;
}

braille_mode_t BrailleGen::GetMode()
{
	LATERO_GRAPHICS_GUARD;
	return mode_;
}

void BrailleGen::SetMode(braille_mode_t mode)
{
	LATERO_GRAPHICS_GUARD
	mode_ = mode;
	SetLastModified_();
	
	if (mode_ == BRAILLE_MODE_DOUBLE)
	{
		for (int i=0; i<6; ++i)
		{
			invertRow_[i] = false;
			disableRow_[i] = false;
		}
		diagRow_ = false;
		narrow_ = false;
	}
	else if (mode_ == BRAILLE_MODE_SINGLE_SPARSE)
	{
		for (int i=0; i<6; ++i)
			invertRow_[i] = false;
		disableRow_[0] = disableRow_[2] = disableRow_[4] = false;
		disableRow_[1] = disableRow_[3] = disableRow_[5] = true;
		diagRow_ = false;
		narrow_ = false;
	}
	else if (mode_ == BRAILLE_MODE_DOUBLE_DOT_SHEAR)
	{
		for (int i=0; i<6; ++i)
			disableRow_[i] = false;
		for (int i=0; i<6; ++i)
			invertRow_[i] = (i%2)?false:true;
		diagRow_ = false;
		narrow_ = false;
	}
	else if (mode_ == BRAILLE_MODE_DOUBLE_ROW_SHEAR)
	{
		for (int i=0; i<6; ++i)
			disableRow_[i] = false;
		invertRow_[0] = invertRow_[1] = false;
		invertRow_[2] = invertRow_[3] = true;
		invertRow_[4] = invertRow_[5] = false;
		diagRow_ = false;
		narrow_ = false;
	}
	else if (mode_ == BRAILLE_MODE_SINGLE_DIAG)
	{
		for (int i=0; i<6; ++i)
			disableRow_[i] = false;
		for (int i=0; i<6; ++i)
			invertRow_[i] = false;
		diagRow_ = true;
		narrow_ = false;
	}
	else if (mode_ == BRAILLE_MODE_SINGLE_COMPACT)
	{
		for (int i=0; i<6; ++i)
		{
			invertRow_[i] = false;
			disableRow_[i] = false;
		}
		diagRow_ = false;
		narrow_ = true;
	}
}

#define GRAY1 0.5
#define GRAY2 0.75

latero::graphics::gtk::Animation BrailleGen::GetIllustration(uint w, uint h) const
{
	Glib::RefPtr<Gdk::Pixbuf> rv = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, w, h);

	// TODO: find a way to draw like that...
	Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(
		(unsigned char*)rv->get_pixels(),
		Cairo::FORMAT_ARGB32,
		rv->get_width(),
		rv->get_height(),
		rv->get_rowstride()
	);

	Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);
	cr->set_line_width(1.0);

	cr->set_source_rgb(1.0, 1.0, 1.0); // bgr
	cr->paint();

	cr->set_source_rgb(0.0, 0.0, 0.0); // bgr

	// grab the values we need

	LATERO_GRAPHICS_LOCK
		float offset = offset_;
		BrailleString str = str_;
		//str.SetMutex(NULL); // otherwise it will lock when we use it... TODO!
		float dotWidth = dotWidth_ * dot_.GetLeft().radius; // TODO: assumes same on both sides, ok for now
		float intraDotDist = intraDotDist_;
		float interDotDist = interDotDist_;
		braille_mode_t mode = mode_;
	LATERO_GRAPHICS_UNLOCK
	float rx = w / Dev()->GetSurfaceWidth();
	float ry = h / Dev()->GetHeight();

	// piezo size
	float contactor_edge_top[6];
	float contactor_edge_bottom[6];
	for (int i=0; i<6; ++i)
	{
		contactor_edge_top[i] = i*Dev()->GetPitchY();
		contactor_edge_bottom[i] = contactor_edge_top[i] + Dev()->GetContactorSizeY();
	}


	for (unsigned int i=0; i<str.GetSize(); ++i)
	{
		BrailleCell *cell = str.GetCell(i);

		float dot_center[2];
		float dot_left[2];
		float dot_right[2];

		dot_center[0] = offset + i*(intraDotDist + interDotDist) + dotWidth/2.0;
		dot_center[1] = dot_center[0] + intraDotDist;
		for (int side=0; side<2; ++side)
		{
			dot_left[side] = dot_center[side] - dotWidth/2.0;
			dot_right[side] = dot_center[side] + dotWidth/2.0;
		}

		for (int r=0; r<3; ++r)
		{
			//float y0 = r*h/4.0;
			for (int side=0; side<2; ++side)
			{
				float ellipse_x = dot_center[side];
				float ellipse_w = dotWidth;

				float top, bottom;
				if (mode == BRAILLE_MODE_SINGLE_SPARSE)
				{
					top = contactor_edge_top[r*2];
					bottom = contactor_edge_bottom[r*2];
				}
				else if (mode == BRAILLE_MODE_SINGLE_COMPACT)
				{
					top = contactor_edge_top[r];
					bottom = contactor_edge_bottom[r];
				}
				else if (mode == BRAILLE_MODE_SINGLE_DIAG)
				{
					top = contactor_edge_top[r*2+side];
					bottom = contactor_edge_bottom[r*2+side];
				}
				else
				{
					top = contactor_edge_top[r*2];
					bottom = contactor_edge_bottom[r*2+1];
				}

				float ellipse_y = (bottom + top)/2.0;
				float ellipse_h = bottom - top;
	
				float top_gray = GRAY1;
				float bottom_gray = GRAY1;
				if (mode == BRAILLE_MODE_DOUBLE_DOT_SHEAR)
				{
					top_gray = GRAY1;
					bottom_gray = GRAY2;
				}
				else if (mode == BRAILLE_MODE_DOUBLE_ROW_SHEAR)
				{
					if (r==1)
					{
						top_gray = GRAY2;
						bottom_gray = GRAY2;
					}
				}

				if (top_gray != bottom_gray)
				{
					// top
					cr->save();
					cr->translate(ellipse_x*rx, ellipse_y*ry);
					cr->scale(ellipse_w*rx/2.0, ellipse_h*ry/2.0);
					cr->arc(0.0, 0.0, 1.0, 1 * M_PI, 2 * M_PI);
					cr->restore();
					cr->set_source_rgb(top_gray, top_gray, top_gray); // bgr
					if (cell->Get(side, r))
						cr->fill();
					else
						cr->stroke();

					// bottom
					cr->save();
					cr->translate(ellipse_x*rx, ellipse_y*ry);
					cr->scale(ellipse_w*rx/2.0, ellipse_h*ry/2.0);
					cr->arc(0.0, 0.0, 1.0, 0.0, 1 * M_PI);
					cr->restore();
					cr->set_source_rgb(bottom_gray, bottom_gray, bottom_gray); // bgr
					if (cell->Get(side, r))
						cr->fill();
					else
						cr->stroke();
				}
				else
				{
					cr->save();
					cr->translate(ellipse_x*rx, ellipse_y*ry);
					cr->scale(ellipse_w*rx/2.0, ellipse_h*ry/2.0);
					cr->arc(0.0, 0.0, 1.0, 0, 2 * M_PI);
					cr->restore();
					cr->set_source_rgb(top_gray, top_gray, top_gray); // bgr
					if (cell->Get(side, r))
						cr->fill();
					else
						cr->stroke();
				}
				
			}
		}
	}
	return rv;
}

Gtk::Widget *BrailleGen::CreateWidget(latero::graphics::GeneratorPtr ptr)
{
	BrailleGenPtr gen = boost::dynamic_pointer_cast<BrailleGen>(ptr);
	if (!gen) return Generator::CreateWidget(ptr); // default
	return new BrailleGenWidget(gen);
}

void BrailleGen::BasicRender_(const latero::graphics::State *state, latero::RangeImg &frame)
{
	lastPos_ = state->pos.x;
	for (uint i=0; i<Dev()->GetFrameSizeX(); ++i)
	{
		for (uint j=0; j<Dev()->GetFrameSizeY(); ++j)
		{
			double x = Dev()->GetActuatorPositions(state->pos, 0).Get(i,j).x; // TODO: expensive!
			frame.Set(i,j,GetDeflection_(x, j));
		}
	}

	// @todo inefficient!
	latero::BiasedImg biasedFrame(frame.SizeX(),frame.SizeY());
	biasedFrame.ConvertFromRange(frame);
	SetLatestState_(*state,biasedFrame);
}

void BrailleGen::DoRender_(const latero::graphics::State *state, latero::BiasedImg &frame)
{
	latero::RangeImg rangeFrame(frame.SizeX(),frame.SizeY());
	RenderBraille_(state, rangeFrame);
	frame.ConvertFromRange(rangeFrame);
}
#endif
