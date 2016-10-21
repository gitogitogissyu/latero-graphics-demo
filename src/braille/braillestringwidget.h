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


#ifndef BRAILLE_STRING_WIDGET_H
#define BRAILLE_STRING_WIDGET_H

#include <gtkmm/frame.h>
#include <gtkmm/entry.h>
#include "braillestring.h"
#include "braillecellwidget.h"

class BrailleStringWidget : public Gtk::Frame
{
public:
	BrailleStringWidget(BrailleString *peer);
	virtual ~BrailleStringWidget();

	sigc::signal<void> signal_value_changed;
protected:
	void CreateWords();
	void OnModeChange();
	void OnCellChange();
	void OnTextChange();
	void OnRandomClicked();
	void OnWordClicked();
	void OnInsertText(const Glib::ustring& str, int* pos);

	Gtk::RadioButton cellMode_;
	Gtk::RadioButton textMode_;
	Gtk::HBox cellBox_;
	Gtk::HBox textBox_;


	Gtk::Button randomButton_;
	Gtk::Button wordButton_;

	Gtk::Entry entry_;
	std::vector<BrailleCellWidget*> cell_;
	BrailleString *peer_;
	std::vector<std::string> words_;
};

#endif
#endif
