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
#ifndef DISABLE_GRAPHICS_DEMO

#include "card.h"

Card::Card(latero::graphics::GeneratorPtr gen, uint width, uint height, uint scale) :
	blindMode_(false),
	gen_(gen),
	width_(width), 
	height_(height),
	img_(gen->Dev())
{  
	img_.SetRounded();
	assert(gen);

	// too slow to be done at runtime
	largeFaceUpAnim_ = gen_->GetIllustration(width*scale,boost::posix_time::seconds(0));
	//faceUpAnim_ = gen->GetIllustration(width, height);
	faceUpAnim_ = largeFaceUpAnim_; // TODO: this makes the image look nicer... try to do use exactly the right size instead? 

	Initialize();
}



Card::Card(const Card& p) :
	img_(p.gen_->Dev())
{
	img_.SetRounded();
	faceUpAnim_ = p.faceUpAnim_;
	width_ = p.width_;
	height_ = p.height_;
	blindMode_ = p.blindMode_;
	gen_ = p.gen_;
	largeFaceUpAnim_ = p.largeFaceUpAnim_;

	Initialize();
};

Card::~Card()
{
}


void Card::Initialize()
{
	add(img_);
	set_events(Gdk::BUTTON_PRESS_MASK);
	signal_button_press_event().connect(
		sigc::mem_fun(*this, &Card::OnClicked));
	show_all_children();
}

Card Card::operator= (const Card& p)
{ 
	if (&p == this) return *this;
	largeFaceUpAnim_ = p.largeFaceUpAnim_;
	faceUpAnim_ = p.faceUpAnim_;
	width_ = p.width_;
	height_ = p.height_;
	blindMode_ = p.blindMode_;
	gen_ = p.gen_;
	UpdateImg();
	return *this;
};

void Card::ClearImg()
{
	img_.Clear(0xffffffff);
}


void Card::UpdateImg()
{
	if (blindMode_)
		img_.Clear(0xffffffff);
	else
		img_.Set(faceUpAnim_);

	//img_.ShowCursor(state_  == state_sel);
	show_all_children();
}

bool Card::OnClicked(GdkEventButton* b)
{
	if (b->button == 1)
		signal_clicked1(this);
	else if (b->button == 3)
		signal_clicked3(this);
	return true;
}

void Card::SetBlind(bool v)
{
	blindMode_ = v;
	UpdateImg();
}

void Card::ModifyBackground(const Gdk::Color &bg)
{
	modify_bg(Gtk::STATE_NORMAL, bg);
}

void Card::SetDisplayState(const latero::graphics::Point &center, double angle, const latero::BiasedImg &frame)
{
	img_.SetDisplayState(center, angle, frame);
}

void Card::HideTD()
{
	img_.ShowCursor(false);
}


latero::graphics::gtk::Animation Card::GetLargeFaceUpAnim()
{
	return largeFaceUpAnim_;
}

#endif
