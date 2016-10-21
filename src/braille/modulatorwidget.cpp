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

#include "modulatorwidget.h"
#include <gtkmm/frame.h>
#include <gtkmm.h>

ModulatorWidget::ModulatorWidget(Modulator *peer) :
	Gtk::Table(2,2),
	peer_(peer),
	ampAdj_(0,0,1,0.1,0.1),
	rampMinAdj_(0,0,500),
	rampMaxAdj_(0,0,500),
	rampMinAmpAdj_(0,0,1),
	rampMaxAmpAdj_(0,0,1),
	maxRiseRateAdj_(0,0.0,1.0),
	maxFallRateAdj_(0,0.0,1.0),
	fixedMode_("fixed"),
	velMode_("vel")
{
	ampAdj_.set_value(1);
	rampMinAdj_.set_value(50);
	rampMaxAdj_.set_value(100);
	rampMinAmpAdj_.set_value(0);
	rampMaxAmpAdj_.set_value(1);
	maxRiseRateAdj_.set_value(0.5);
	maxFallRateAdj_.set_value(0.5);

	Gtk::RadioButton::Group group = fixedMode_.get_group();
	velMode_.set_group(group);
	fixedMode_.set_active(true);
	ampAdj_.set_value(peer_->GetFixedAmp());
	double min,max;
	peer_->GetRampRange(min,max);
	rampMinAdj_.set_value(min);
	rampMaxAdj_.set_value(max);
	peer_->GetRampAmpRange(min,max);
	rampMinAmpAdj_.set_value(min);
	rampMaxAmpAdj_.set_value(max);
	maxRiseRateAdj_.set_value(peer_->GetMaxRiseRate());
	maxFallRateAdj_.set_value(peer_->GetMaxFallRate());

	Gtk::Frame *fixedFrame = manage(new Gtk::Frame);
	Gtk::Frame *velFrame = manage(new Gtk::Frame);
	Gtk::HBox *velAmpBox = manage(new Gtk::HBox);
	Gtk::HBox *velRangeBox = manage(new Gtk::HBox);
	Gtk::HBox *velRateBox = manage(new Gtk::HBox);

	attach(fixedMode_, 0, 1, 0, 1, Gtk::SHRINK);
	attach(*fixedFrame, 1, 2, 0, 1);
		fixedFrame->add(fixedBox_);
		fixedBox_.pack_start(*manage(new Gtk::Label(" Amplitude (0-1) ")), Gtk::PACK_SHRINK);
		fixedBox_.pack_start(*manage(new Gtk::SpinButton(ampAdj_,0, 3)), Gtk::PACK_SHRINK);
	attach(velMode_, 0, 1, 1, 2, Gtk::SHRINK);
	attach(*velFrame, 1, 2, 1, 2);
		velFrame->add(velBox_);
		velBox_.pack_start(*velAmpBox);
			velAmpBox->pack_start(*manage(new Gtk::Label("Amplitude range (0-1): ")), Gtk::PACK_SHRINK);
			velAmpBox->pack_start(*manage(new Gtk::SpinButton(rampMinAmpAdj_,0, 2)), Gtk::PACK_SHRINK);
			velAmpBox->pack_start(*manage(new Gtk::Label("  to  ")), Gtk::PACK_SHRINK);
			velAmpBox->pack_start(*manage(new Gtk::SpinButton(rampMaxAmpAdj_,0, 2)), Gtk::PACK_SHRINK);
		velBox_.pack_start(*velRangeBox);
			velRangeBox->pack_start(*manage(new Gtk::Label("Ramp range (mm/s): ")), Gtk::PACK_SHRINK);
			velRangeBox->pack_start(*manage(new Gtk::SpinButton(rampMinAdj_,0, 2)), Gtk::PACK_SHRINK);
			velRangeBox->pack_start(*manage(new Gtk::Label("  to  ")), Gtk::PACK_SHRINK);
			velRangeBox->pack_start(*manage(new Gtk::SpinButton(rampMaxAdj_,0, 2)), Gtk::PACK_SHRINK);
		velBox_.pack_start(*velRateBox);
			velRateBox->pack_start(*manage(new Gtk::Label(" Maximum transition rate (sec/full): ")), Gtk::PACK_SHRINK);
			velRateBox->pack_start(*manage(new Gtk::Label("rise ")), Gtk::PACK_SHRINK);
			velRateBox->pack_start(*manage(new Gtk::SpinButton(maxRiseRateAdj_,0, 2)), Gtk::PACK_SHRINK);
			velRateBox->pack_start(*manage(new Gtk::Label("  fall ")), Gtk::PACK_SHRINK);
			velRateBox->pack_start(*manage(new Gtk::SpinButton(maxFallRateAdj_,0, 2)), Gtk::PACK_SHRINK);

	ampAdj_.signal_value_changed().connect(
		sigc::mem_fun(*this, &ModulatorWidget::OnChange));
	maxRiseRateAdj_.signal_value_changed().connect(
		sigc::mem_fun(*this, &ModulatorWidget::OnChange));
	maxFallRateAdj_.signal_value_changed().connect(
		sigc::mem_fun(*this, &ModulatorWidget::OnChange));
  	fixedMode_.signal_clicked().connect(
		sigc::mem_fun(*this, &ModulatorWidget::OnChange));
  	velMode_.signal_clicked().connect(
		sigc::mem_fun(*this, &ModulatorWidget::OnChange));
	rampMinAdj_.signal_value_changed().connect(
		sigc::mem_fun(*this, &ModulatorWidget::OnChange));
	rampMaxAdj_.signal_value_changed().connect(
		sigc::mem_fun(*this, &ModulatorWidget::OnChange));
	rampMinAmpAdj_.signal_value_changed().connect(
		sigc::mem_fun(*this, &ModulatorWidget::OnChange));
	rampMaxAmpAdj_.signal_value_changed().connect(
		sigc::mem_fun(*this, &ModulatorWidget::OnChange));


	OnModeChange();
}

ModulatorWidget::~ModulatorWidget()
{
}

void ModulatorWidget::OnChange()
{
	if (rampMaxAdj_.get_value() <= rampMinAdj_.get_value())
	{
		double min,max;
		peer_->GetRampRange(min,max);
		rampMinAdj_.set_value(min);
		rampMaxAdj_.set_value(max);
	}
	else
	{
		peer_->SetRampRange(rampMinAdj_.get_value(),rampMaxAdj_.get_value());
	}
	if (rampMaxAmpAdj_.get_value() <= rampMinAmpAdj_.get_value())
	{
		double min,max;
		peer_->GetRampAmpRange(min,max);
		rampMinAmpAdj_.set_value(min);
		rampMaxAmpAdj_.set_value(max);
	}
	else
	{
		peer_->SetRampAmpRange(rampMinAmpAdj_.get_value(),rampMaxAmpAdj_.get_value());
	}
	peer_->SetMaxRiseRate(maxRiseRateAdj_.get_value());
	peer_->SetMaxFallRate(maxFallRateAdj_.get_value());
	peer_->SetFixedAmp(ampAdj_.get_value());
	if (fixedMode_.get_active()) peer_->SetModeFixed();
	if (velMode_.get_active()) peer_->SetModeVel();
	OnModeChange();
}

void ModulatorWidget::OnModeChange()
{
	fixedBox_.set_sensitive(fixedMode_.get_active());
	velBox_.set_sensitive(velMode_.get_active());
}



#endif
