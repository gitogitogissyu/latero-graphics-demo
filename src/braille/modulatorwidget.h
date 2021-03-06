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

#ifndef MODULATOR_WIDGET_H
#define MODULATOR_WIDGET_H

#include <gtkmm/table.h>
#include <gtkmm/box.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/radiobutton.h>
#include "modulator.h"

class ModulatorWidget : public Gtk::Table
{
public:
	ModulatorWidget(Modulator *peer);
	virtual ~ModulatorWidget();

protected:
	void OnChange();
	void OnModeChange();

	Modulator *peer_;

	Gtk::Adjustment ampAdj_;
	Gtk::Adjustment rampMinAdj_, rampMaxAdj_;
	Gtk::Adjustment rampMinAmpAdj_, rampMaxAmpAdj_;
	Gtk::Adjustment maxRiseRateAdj_, maxFallRateAdj_;
  	Gtk::RadioButton fixedMode_, velMode_;

	Gtk::HBox fixedBox_;
	Gtk::VBox velBox_;
};

#endif
#endif
