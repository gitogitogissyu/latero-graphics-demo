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


#include "dotwidget.h"
#include <laterographics/gtk/numwidget.h>

DotSideWidget::DotSideWidget() :
	radiusAdj_(0,0.0,1.0),
	plateauAdj_(0,0.0,1.0),
	txAmpAdj_(0,0.0,0.5),
	txNbCyclesAdj_(0,1.0,20.0)
{
	radiusAdj_.set_value(1.0);
	plateauAdj_.set_value(1.0);
	txAmpAdj_.set_value(0.0);
	txNbCyclesAdj_.set_value(0.0);

	pack_start(shapeCombo_);
	pack_start(*manage(new latero::graphics::gtk::HNumWidget("radius", radiusAdj_, 2)));
	pack_start(*manage(new latero::graphics::gtk::HNumWidget("plateau (% of radius)", plateauAdj_, 2)));
	pack_start(*manage(new latero::graphics::gtk::HNumWidget("texture amplitude", txAmpAdj_, 2)));
	pack_start(*manage(new latero::graphics::gtk::HNumWidget("texture nb cycles", txNbCyclesAdj_, 0)));

	shapeCombo_.signal_changed().connect(
		sigc::mem_fun(*this, &DotSideWidget::OnChange));
	radiusAdj_.signal_value_changed().connect(
		sigc::mem_fun(*this, &DotSideWidget::OnChange));
	plateauAdj_.signal_value_changed().connect(
		sigc::mem_fun(*this, &DotSideWidget::OnChange));
	txAmpAdj_.signal_value_changed().connect(
		sigc::mem_fun(*this, &DotSideWidget::OnChange));
	txNbCyclesAdj_.signal_value_changed().connect(
		sigc::mem_fun(*this, &DotSideWidget::OnChange));
}

Dot::SideParams DotSideWidget::Get()
{
	Dot::SideParams p;
	p.radius = radiusAdj_.get_value();
	p.plateau = plateauAdj_.get_value();
	p.shape = shapeCombo_.get_active_text();
	p.txAmp = txAmpAdj_.get_value();
	p.txNbCycles = (uint)txNbCyclesAdj_.get_value();
	return p;
}

void DotSideWidget::Set(const Dot::SideParams &p)
{
	radiusAdj_.set_value(p.radius);
	plateauAdj_.set_value(p.plateau);
	shapeCombo_.set_active_text(p.shape);
	txAmpAdj_.set_value(p.txAmp);
	txNbCyclesAdj_.set_value(p.txNbCycles);
}

void DotSideWidget::AddShapes(const std::vector<std::string> &shapes)
{
	for (uint i=0; i<shapes.size(); ++i)
		shapeCombo_.append_text(shapes[i]);
}

void DotSideWidget::Disable(bool v)
{
	set_sensitive(!v);
	shapeCombo_.set_sensitive(!v);
}

void DotSideWidget::OnChange()
{
	signal_value_changed();
}


DotWidget::DotWidget(Dot *peer) :
	peer_(peer)
{
	graph_ = manage(new DotGraph(peer));

	std::vector<std::string> shapes = Dot::GetShapes();
	left_.AddShapes(shapes);
	right_.AddShapes(shapes);

	Dot::SideParams l = peer_->GetLeft();
	Dot::SideParams r = peer_->GetRight();
	left_.Set(l);
	right_.Set(r);
	sideLockCheck_.set_active(peer_->GetLockedSides());
	right_.Disable(peer_->GetLockedSides());

	pack_start(left_);
	pack_start(sideLockCheck_, Gtk::PACK_SHRINK);
	pack_start(right_);
	pack_start(*graph_);
	graph_->Refresh();

	left_.signal_value_changed.connect(
		sigc::mem_fun(*this, &DotWidget::OnChange));
	right_.signal_value_changed.connect(
		sigc::mem_fun(*this, &DotWidget::OnChange));
	sideLockCheck_.signal_toggled().connect(
		sigc::mem_fun(*this, &DotWidget::OnChange));
}

void DotWidget::OnChange()
{
	right_.Disable(LockedSides());
	peer_->Set(left_.Get(),right_.Get(),sideLockCheck_.get_active());
	graph_->Refresh();
	signal_value_changed();
}
	
bool DotWidget::LockedSides()
{
	return sideLockCheck_.get_active();
}


#endif
