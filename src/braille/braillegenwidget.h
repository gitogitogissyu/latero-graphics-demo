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

#ifndef BRAILLE_GEN_WIDGET_H
#define BRAILLE_GEN_WIDGET_H

#include "braillegen.h"
#include <gtkmm/checkbutton.h>
#include "braillestringwidget.h"
#include "dotwidget.h"
#include "modulatorwidget.h"
//#include <laterographics/generatorwidget.h>

class BrailleGenWidget : public Gtk::VBox
{
public:
	BrailleGenWidget(BrailleGenPtr peer);
	virtual ~BrailleGenWidget();

protected:
	void OnStandardButton();
	void OnScaledButton();
	void OnHalfScaledButton();
	void OnVBDButton();


	void OnChange();	
	Gtk::Widget *CreateVizWidget();
	Gtk::Widget *CreateModeWidget();
	Gtk::Widget *CreateParamsWidget();
	Gtk::Widget *CreateVibWidget();

 	BrailleGenPtr peer_;

	BrailleStringWidget strWidget_;
	DotWidget dot_;
	DotWidget vibDot_;
	Gtk::Adjustment dotWidthAdj_;
	Gtk::Adjustment intraDotDistAdj_;
	Gtk::Adjustment interDotDistAdj_;
	Gtk::Adjustment offsetAdj_;
	Gtk::Adjustment vibFreqAdj_;
	ModulatorWidget vibModWidget_;

	Gtk::RadioButton mode_[BRAILLE_MODE_SIZE];
	Gtk::CheckButton invertCheck_;
};

#endif
#endif
