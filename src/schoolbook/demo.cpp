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

#include "demo.h"
#ifndef DISABLE_SCHOOLBOOK_DEMO

#include "../generatorhandle.h"

namespace SchoolbookDemo {

#define SEL_OEIL	0
#define SEL_CATH	1
#define SEL_BAR		2
#define SEL_MAP		3

const std::string GEN_OEIL = media_dir+"/schoolbook/oeil3/oeil3.gen";
const std::string GEN_CATH = media_dir+"/schoolbook/A1B1_Cathedrale_Gothique.gen";
const std::string GEN_BAR = media_dir+"/schoolbook/A2_Population_Attique/A2_Population_Attique.gen";
const std::string GEN_MAP = media_dir+"/schoolbook/A3A4_Civilisations.gen";

Demo::Demo(const latero::Tactograph *dev) :
	preview_(dev, latero::graphics::GeneratorPtr(), true),
	sel_(0),
	dev_(dev)
{
	peer_ = GeneratorHandlePtr(new GeneratorHandle(dev));
	preview_.SetGenerator(peer_);
	preview_.RefreshBackground();


	CreateGenerators();
	Select(SEL_CATH);

	preview_.ShowCursor();
	Gtk::RadioButton::Group g = selButton_[0].get_group();
	for (int i=1; i<NB_BUTTONS; ++i)
		selButton_[i].set_group(g);

	Gtk::VBox *box = manage(new Gtk::VBox);
	Gtk::HBox *buttonBox = manage(new Gtk::HBox);

	add(*box);
	box->pack_start(preview_);
	box->pack_start(*buttonBox, Gtk::PACK_SHRINK);
	for (int i=0; i<NB_BUTTONS; ++i)
	{
		buttonBox->pack_start(selButton_[i]);
		selButton_[i].signal_clicked().connect(
			sigc::mem_fun(*this, &Demo::OnSelChange));
	}

	selButton_[0].set_label("oeil");
	selButton_[1].set_label("cathedrale gothique");
	selButton_[2].set_label("population attique");
	selButton_[3].set_label("civilisations");

	show_all_children();

	signal_key_press_event().connect(
		sigc::mem_fun(*this, &Demo::OnKeyPress));
}

Demo::~Demo() 
{
}

void Demo::CreateGenerators()
{
	list_.push_back(latero::graphics::Generator::Create(GEN_OEIL, dev_));
	list_.push_back(latero::graphics::Generator::Create(GEN_CATH, dev_));
	list_.push_back(latero::graphics::Generator::Create(GEN_BAR, dev_));
	list_.push_back(latero::graphics::Generator::Create(GEN_MAP, dev_));

    /*
	for (unsgigned int i=0; i<list_.size(); ++i)
	{
		float w = 1000;
		float h = w * dev_->GetSurfaceHeight() / dev_->GetSurfaceWidth();
		list_[i]->StoreIllustration(w,h);
	}
    */
}


void Demo::OnSelChange()
{
	if (selButton_[0].get_active())		Select(SEL_OEIL);
	else if (selButton_[1].get_active())	Select(SEL_CATH);
	else if (selButton_[2].get_active())	Select(SEL_BAR);
	else if (selButton_[3].get_active())	Select(SEL_MAP);
}


bool Demo::OnKeyPress(GdkEventKey* event)
{
	list_[sel_]->OnKeyPress(event);
	Select(sel_); // TODO: this is wasteful (only necessary if there's a toggle
	return true;
}

void Demo::Select(int i)
{
	sel_ = i;
	peer_->SetGenerator(list_[sel_]);
	assert(list_[sel_]);
	//preview_.SetGenerator(list_[sel_]);
	//preview_.RefreshBackground();
}

}; // namespace

#endif
