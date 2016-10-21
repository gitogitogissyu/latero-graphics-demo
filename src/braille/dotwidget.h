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

#ifndef DOT_WIDGET_H
#define DOT_WIDGET_H

#include <gtkmm/checkbutton.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/comboboxtext.h>
#include "dot.h"
#include "dotgraph.h"

class DotSideWidget : public Gtk::VBox
{
public:
	DotSideWidget();
	Dot::SideParams Get();
	void Set(const Dot::SideParams &p);
	void AddShapes(const std::vector<std::string> &shapes);
	void Disable(bool v);
	void OnChange();
	sigc::signal<void> signal_value_changed;

protected:
	Gtk::Adjustment radiusAdj_;
	Gtk::Adjustment plateauAdj_;
	Gtk::Adjustment txAmpAdj_;
	Gtk::Adjustment txNbCyclesAdj_;
	Gtk::ComboBoxText shapeCombo_;
};


class DotWidget : public Gtk::HBox
{
public:
	DotWidget(Dot *peer);
	void OnChange();
	bool LockedSides();

	sigc::signal<void> signal_value_changed;

protected:

	DotSideWidget left_;
	DotSideWidget right_;
	Gtk::CheckButton sideLockCheck_;
	Dot *peer_;
	DotGraph *graph_;
};

#endif
#endif
