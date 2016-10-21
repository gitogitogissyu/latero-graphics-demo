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

#include "braillegenwidget.h"
#include "braillegen.h"
#include "braillecellwidget.h"
#include <gtkmm/alignment.h>
#include <gtkmm/notebook.h>
#include "dot.h"
#include <laterographics/gtk/numwidget.h>
#include "virtualsurfacewidget.h"
#include "positiongraph.h"


BrailleGenWidget::BrailleGenWidget(BrailleGenPtr peer) :
	//latero::graphics::GeneratorWidget(peer),
	peer_(peer),
	strWidget_(&peer->str_),
	dot_(&peer->dot_),
	vibDot_(&peer->vibDot_),
	dotWidthAdj_(0,0.1,1),
	intraDotDistAdj_(0,0.1,3),
	interDotDistAdj_(0,0.1,4),
	offsetAdj_(1,0.0001,50),
	vibFreqAdj_(0,1,25),
	vibModWidget_(&peer->vibMod_),
	invertCheck_("invert")
{
	mode_[BRAILLE_MODE_SINGLE_COMPACT].set_label("single (compact)"),
	mode_[BRAILLE_MODE_SINGLE_DIAG].set_label("single (diagonal)"),
	mode_[BRAILLE_MODE_SINGLE_SPARSE].set_label("single (sparse)"),
	mode_[BRAILLE_MODE_DOUBLE].set_label("double"),
	mode_[BRAILLE_MODE_DOUBLE_DOT_SHEAR].set_label("double (dot shear)"),
	mode_[BRAILLE_MODE_DOUBLE_ROW_SHEAR].set_label("double (row shear)"),

	dotWidthAdj_.set_value(peer_->GetDotWidthRel());
	intraDotDistAdj_.set_value(peer_->GetIntraDotDistRel());
	interDotDistAdj_.set_value(peer_->GetInterDotDistRel());
	offsetAdj_.set_value(peer_->GetOffset());
	vibFreqAdj_.set_value(peer_->GetVibFreq());

	Gtk::Notebook *notebook = manage(new Gtk::Notebook);
	Gtk::Alignment *align = manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,Gtk::ALIGN_CENTER,0,0));

	pack_start(*CreateVizWidget());
	pack_start(*align, Gtk::PACK_SHRINK);
		align->add(strWidget_);
	pack_start(*notebook, Gtk::PACK_SHRINK);
		notebook->append_page(*CreateParamsWidget(), "General Settings");
		notebook->append_page(dot_, "Deflection Profile");
		notebook->append_page(*CreateVibWidget(), "Vibration Settings");
		notebook->append_page(vibDot_, "Vibration Profile");

	dot_.signal_value_changed.connect(
		sigc::mem_fun(*this, &BrailleGenWidget::OnChange));
	vibDot_.signal_value_changed.connect(
		sigc::mem_fun(*this, &BrailleGenWidget::OnChange));
	strWidget_.signal_value_changed.connect(
		sigc::mem_fun(*this, &BrailleGenWidget::OnChange));
}

Gtk::Widget *BrailleGenWidget::CreateVibWidget()
{
	Gtk::Frame *frame = manage(new Gtk::Frame("Vibration Parameters"));
	Gtk::VBox *box = manage(new Gtk::VBox);

	frame->add(*box);
	box->pack_start(*manage(new latero::graphics::gtk::HNumWidget("frequency", vibFreqAdj_,0)), Gtk::PACK_SHRINK);
	box->pack_start(vibModWidget_);
	vibFreqAdj_.signal_value_changed().connect(
		sigc::mem_fun(*this, &BrailleGenWidget::OnChange));
	return frame;
}


Gtk::Widget *BrailleGenWidget::CreateParamsWidget()
{
	Gtk::HBox *box = manage(new Gtk::HBox);
	Gtk::VBox *paramBox = manage(new Gtk::VBox);
	Gtk::VBox *presetBox = manage(new Gtk::VBox);
	Gtk::Frame *presetFrame = manage(new Gtk::Frame("presets"));

	Gtk::Button *stdButton = manage(new Gtk::Button("standard braille"));
	Gtk::Button *scaledButton = manage(new Gtk::Button("scaled braille (full height)"));
	Gtk::Button *halfScaledButton = manage(new Gtk::Button("scaled braille (half height)"));
	Gtk::Button *vbdButton = manage(new Gtk::Button("vbd braille"));

	box->pack_start(*CreateModeWidget(), Gtk::PACK_SHRINK);
	box->pack_start(*paramBox);
	box->pack_start(*presetFrame, Gtk::PACK_SHRINK);

	paramBox->pack_start(
		*manage(new latero::graphics::gtk::HNumWidget("dot width (% of space)", dotWidthAdj_,3)));
	paramBox->pack_start(
		*manage(new latero::graphics::gtk::HNumWidget("dot-to-dot spacing (in cell, % of standard)", intraDotDistAdj_,3)));
	paramBox->pack_start(
		*manage(new latero::graphics::gtk::HNumWidget("dot-to-dot spacing (between cells, % of standard)", interDotDistAdj_,3)));
	paramBox->pack_start(
		*manage(new latero::graphics::gtk::HNumWidget("offset", offsetAdj_,3)));

	presetFrame->add(*presetBox);
		presetBox->pack_start(*stdButton, Gtk::PACK_SHRINK);
		presetBox->pack_start(*scaledButton, Gtk::PACK_SHRINK);
		presetBox->pack_start(*halfScaledButton, Gtk::PACK_SHRINK);
		presetBox->pack_start(*vbdButton, Gtk::PACK_SHRINK);

	dotWidthAdj_.signal_value_changed().connect(
		sigc::mem_fun(*this, &BrailleGenWidget::OnChange));
	intraDotDistAdj_.signal_value_changed().connect(
		sigc::mem_fun(*this, &BrailleGenWidget::OnChange));
	interDotDistAdj_.signal_value_changed().connect(
		sigc::mem_fun(*this, &BrailleGenWidget::OnChange));
	offsetAdj_.signal_value_changed().connect(
		sigc::mem_fun(*this, &BrailleGenWidget::OnChange));

	stdButton->signal_clicked().connect(
		sigc::mem_fun(*this, &BrailleGenWidget::OnStandardButton));
	scaledButton->signal_clicked().connect(
		sigc::mem_fun(*this, &BrailleGenWidget::OnScaledButton));
	halfScaledButton->signal_clicked().connect(
		sigc::mem_fun(*this, &BrailleGenWidget::OnHalfScaledButton));
	vbdButton->signal_clicked().connect(
		sigc::mem_fun(*this, &BrailleGenWidget::OnVBDButton));
	
	return box;
}


Gtk::Widget *BrailleGenWidget::CreateModeWidget()
{
	Gtk::RadioButton::Group group = mode_[0].get_group();
	for (int i=1; i<BRAILLE_MODE_SIZE; ++i)
		mode_[i].set_group(group);
	mode_[(int)peer_->GetMode()].set_active(true);
	invertCheck_.set_active(peer_->GetInvert());

	Gtk::Frame *modeFrame = manage(new Gtk::Frame("mode"));
	Gtk::VBox *modeBox = manage(new Gtk::VBox);

	modeFrame->add(*modeBox);
	//modeBox->pack_start(*manage(new Gtk::Label("single")));
	modeBox->pack_start(mode_[BRAILLE_MODE_SINGLE_COMPACT]);
	modeBox->pack_start(mode_[BRAILLE_MODE_SINGLE_DIAG]);
	modeBox->pack_start(mode_[BRAILLE_MODE_SINGLE_SPARSE]);
	//modeBox->pack_start(*manage(new Gtk::Label("double")));
	modeBox->pack_start(mode_[BRAILLE_MODE_DOUBLE]);
	modeBox->pack_start(mode_[BRAILLE_MODE_DOUBLE_DOT_SHEAR]);
	modeBox->pack_start(mode_[BRAILLE_MODE_DOUBLE_ROW_SHEAR]);
	modeBox->pack_start(invertCheck_);

	for (int i=0; i<BRAILLE_MODE_SIZE; ++i)
  		mode_[i].signal_clicked().connect(
			sigc::mem_fun(*this, &BrailleGenWidget::OnChange));
	invertCheck_.signal_toggled().connect(
		sigc::mem_fun(*this, &BrailleGenWidget::OnChange));

	return modeFrame;
}


BrailleGenWidget::~BrailleGenWidget()
{
}

void BrailleGenWidget::OnChange()
{
	for (int i=0; i<BRAILLE_MODE_SIZE; ++i)
		if (mode_[i].get_active()) peer_->SetMode((braille_mode_t)i);
	peer_->SetInvert(invertCheck_.get_active());
	peer_->SetIntraDotDistRel(intraDotDistAdj_.get_value());
	peer_->SetInterDotDistRel(interDotDistAdj_.get_value());
	peer_->SetDotWidthRel(dotWidthAdj_.get_value());
	peer_->SetOffset(offsetAdj_.get_value());
	peer_->SetVibFreq(vibFreqAdj_.get_value());
}

void BrailleGenWidget::OnStandardButton()
{
	intraDotDistAdj_.set_value(1.0);
	interDotDistAdj_.set_value(1.0);
	dotWidthAdj_.set_value(0.655);
}

void BrailleGenWidget::OnScaledButton()
{
	// dot-to-dot distance is 2.54 mm vertically
	// assuming the middle of a dot is between rows, that means that 4/6th of the display should cover 5.08 mm
	// => 10.8 * 2/3 = 7.2 mm => ratio is 1.42
	// height of display is 10.8 mm

	intraDotDistAdj_.set_value(1.42);
	interDotDistAdj_.set_value(1.42);
	dotWidthAdj_.set_value(0.655);
}

void BrailleGenWidget::OnHalfScaledButton()
{
	// dot-to-dot distance is 2.54 mm vertically
	// assuming the middle of a dot is between rows, that means that 4/6th of the display should cover 5.08 mm
	// => 5.4 * 2/3 = 3.6 mm => ratio is 0.71
	// height of display is 10.8 mm

	intraDotDistAdj_.set_value(0.71);
	interDotDistAdj_.set_value(0.71);
	dotWidthAdj_.set_value(0.655);
}

void BrailleGenWidget::OnVBDButton()
{
	intraDotDistAdj_.set_value(1.75);
	interDotDistAdj_.set_value(1.75);
	dotWidthAdj_.set_value(0.5);
}

Gtk::Widget *BrailleGenWidget::CreateVizWidget()
{
	Gtk::VBox *box = manage(new Gtk::VBox);

	Gtk::Alignment *align = manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,Gtk::ALIGN_CENTER,0,1));
	align->add(*box);
	VirtualSurfaceWidget *surf = manage(new VirtualSurfaceWidget(peer_));

	surf->set_size_request(1000,1.2*1000*peer_->Dev()->GetHeight()/peer_->Dev()->GetSurfaceWidth());

	PositionGraph *graph = manage(new PositionGraph(peer_));
	box->pack_start(*graph);
	box->pack_start(*surf, Gtk::PACK_SHRINK);

	return align;
}

#endif
