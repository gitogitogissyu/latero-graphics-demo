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

#ifndef CARD_H
#define CARD_H

#include "gtkmm.h"
#include <laterographics/virtualsurfacewidget.h>
#include <laterographics/generator.h>

class Card : public Gtk::EventBox
{
public:
	Card(latero::graphics::GeneratorPtr gen, uint width, uint height, uint scale);
	~Card();

	Card(const Card& p);
	Card operator= (const Card& p);

	void Initialize();

	void SetBlind(bool v);

	void SetDisplayState(const latero::graphics::Point &center, double angle, const latero::BiasedImg &frame);
	void HideTD();

	inline latero::graphics::GeneratorPtr GetGenerator() {
		return gen_;
	}

	sigc::signal<void,Card*> signal_clicked1;
	sigc::signal<void,Card*> signal_clicked3;

	void ModifyBackground(const Gdk::Color &bg);

	latero::graphics::gtk::Animation GetLargeFaceUpAnim();


	void ShowCursor(bool v) {
		img_.ShowBorder(v);
	}

	void UpdateImg();
	void ClearImg();
protected:

	bool OnClicked(GdkEventButton*);
	bool blindMode_;

	latero::graphics::GeneratorPtr gen_;

	uint width_, height_;

	latero::graphics::gtk::Animation largeFaceUpAnim_;
	latero::graphics::gtk::Animation faceUpAnim_;

	// TODO: make sure img_ is used only for cards we are actually using! (i.e. when inserted in table!)
	latero::graphics::BaseVirtualSurfaceWidget img_;

};

#endif
#endif
