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

#include "demo.h"
#include "settings.h"
#include "../generatorhandle.h"

#define REFRESH_INTERVAL_MS 30
#define SCALE_UP_FACTOR 5

namespace ImgDemo {

Demo::Demo(const latero::Tactograph *dev) :
	currentSet_(NULL),
	zoomImg_(dev),
	demoTable_(NbCardsX,NbCardsY),
	curCard_(NULL),
	keyLocation_(0,0),
	currentSetIdx_(0)
{ 
	gen_ = GeneratorHandlePtr(new GeneratorHandle(dev));

	//zoomImg_.SetRounded(); // seems to cause problems with xeno 2.4.2
	cardSets_.Load(media_dir+"/img/main.col", dev, SCALE_UP_FACTOR);

	Gtk::RadioAction::Group setGroup;
	for (uint i=0; i<cardSets_.size(); ++i)
	{
		std::stringstream id;
		id << "Set" << i;
		std::string name = cardSets_[i]->GetName();
		Glib::RefPtr<Gtk::RadioAction> action = Gtk::RadioAction::create(setGroup, id.str(), name);
		setActions_.push_back(action);
	}


	Gtk::VBox *box = manage(new Gtk::VBox);
	Gtk::HBox *hbox = manage(new Gtk::HBox);
	Gtk::Button *prevButton = manage(new Gtk::Button("<"));
	Gtk::Button *nextButton = manage(new Gtk::Button(">"));

	add(*box);
	box->pack_start(zoomImg_);
	box->pack_start(*hbox);
		hbox->pack_start(*prevButton, Gtk::PACK_SHRINK);
		hbox->pack_start(demoTable_);
		hbox->pack_start(*nextButton, Gtk::PACK_SHRINK);

	set_events(Gdk::BUTTON_PRESS_MASK);
	signal_button_press_event().connect(sigc::mem_fun(*this, &Demo::OnBoardClick));

	prevButton->signal_clicked().connect(sigc::mem_fun(*this, &Demo::OnPrevSet));
	nextButton->signal_clicked().connect(sigc::mem_fun(*this, &Demo::OnNextSet));

	show_all_children();

	UpdateSet();

	 Glib::signal_timeout().connect(
		sigc::mem_fun(*this, &Demo::OnIdle),
		REFRESH_INTERVAL_MS,
		Glib::PRIORITY_DEFAULT_IDLE); 

	zoomImg_.ShowCursor();

	signal_key_press_event().connect(
		sigc::mem_fun(*this, &Demo::OnKeyPress));
}

Demo::~Demo()
{
	ClearSet();
}


void Demo::UpdateSet()
{
	for (uint i=0; i<setActions_.size(); ++i)
	{
		if (setActions_[i]->get_active())
		{
			currentSetIdx_ = i;
			std::stringstream id;
			id << "Set" << i;
			CardSet *set = cardSets_.GetSet(setActions_[i]->property_label().get_value());
			LoadSet(*set);
		}
	}
	keyLocation_.x =  keyLocation_.y = 0;
	OnDemoClick(GetCard(keyLocation_.x, keyLocation_.y));
}

void Demo::UpdateMode()
{
}

void Demo::OnSetChanged()
{
	UpdateSet();
}

void Demo::ClearSet()
{
	demoTable_.RemoveCards();
	zoomImg_.Clear(0xffffffff);
	demoCards_.DeleteCards();
	currentSet_ = NULL;
	SetCurrentCard(NULL);
}


void Demo::LoadSet(const CardSet &set)
{
	if (&set == currentSet_)
		return;
	ClearSet();

	currentSet_ = &set;
	for (uint i=0; i<set.size(); ++i)
		demoCards_.push_back(new Card(*set[i]));

	for (uint i=0; i<demoCards_.size(); ++i)
	{
 		demoCards_[i]->signal_clicked1.connect(sigc::mem_fun(*this, &Demo::OnDemoClick));
		demoCards_[i]->signal_clicked3.connect(sigc::mem_fun(*this, &Demo::OnRightClick));
	}

	demoTable_.SetCards(demoCards_);
	show_all_children();
}

void Demo::Reset()
{
	GetCard(keyLocation_.x, keyLocation_.y)->ShowCursor(false);
	zoomImg_.Clear(0xffffffff);
	SetCurrentCard(NULL);
}

void Demo::OnRightClick(Card* card)
{
	Reset();
}

void Demo::UpdateZoom(Card* card)
{
	latero::graphics::gtk::Animation  anim = card->GetLargeFaceUpAnim();
	zoomImg_.Set(anim);
	show_all_children();
}

void Demo::OnDemoClick(Card* card)
{
	Reset();
	SetCurrentCard(card);
	UpdateZoom(card);

	demoTable_.GetLocation(card, keyLocation_.x, keyLocation_.y);
}


bool Demo::OnBoardClick(GdkEventButton* b)
{
	if (b->button == 3)
		Reset();
	return true;
}


bool Demo::OnIdle()
{
	if (curCard_)
	{
		latero::BiasedImg frame = gen_->GetLatestFrame();
		latero::graphics::Point center = gen_->GetDisplayCenter();
		double angle = gen_->GetDisplayOrientation();
		curCard_->SetDisplayState(center, angle, frame);
		zoomImg_.SetDisplayState(center, angle, frame);
	}


	 Glib::signal_timeout().connect(
		sigc::mem_fun(*this, &Demo::OnIdle),	
		REFRESH_INTERVAL_MS,
		Glib::PRIORITY_DEFAULT_IDLE); 

	return false;
}

void Demo::OnShowCursor()
{
}

bool Demo::OnKeyPress(GdkEventKey* event)
{
	GetCard(keyLocation_.x, keyLocation_.y)->ShowCursor(false);

	// select
	if (event->keyval == GDK_KP_5)
		OnDemoClick(GetCard(keyLocation_.x, keyLocation_.y));

	// move up
	if ((event->keyval == GDK_KP_7) || (event->keyval == GDK_KP_8) || (event->keyval == GDK_KP_9))
		keyLocation_.y--;

	if ((event->keyval == GDK_KP_1) || (event->keyval == GDK_KP_2) || (event->keyval == GDK_KP_3))
		keyLocation_.y++;
	
	if ((event->keyval == GDK_KP_7) || (event->keyval == GDK_KP_4) || (event->keyval == GDK_KP_1))
		keyLocation_.x--;

	if ((event->keyval == GDK_KP_9) || (event->keyval == GDK_KP_6) || (event->keyval == GDK_KP_3))
		keyLocation_.x++;

	if (keyLocation_.x > 5)
	{
		keyLocation_.x = 0;
		currentSetIdx_ = (currentSetIdx_+1)%setActions_.size();
		setActions_[currentSetIdx_]->set_active();
		OnDemoClick(GetCard(keyLocation_.x, keyLocation_.y));
	}
	else if (keyLocation_.x < 0)
	{
		currentSetIdx_--;
		if (currentSetIdx_<0) currentSetIdx_  = setActions_.size()-1;
		setActions_[currentSetIdx_]->set_active();
		keyLocation_.x = 5;
		OnDemoClick(GetCard(keyLocation_.x, keyLocation_.y));
	}

	keyLocation_.y = keyLocation_.y%2;
	if (keyLocation_.y<0) keyLocation_.y = 1;

	GetCard(keyLocation_.x, keyLocation_.y)->ShowCursor(true);

	return true;
}

void Demo::OnPrevSet()
{
	currentSetIdx_--;
	if (currentSetIdx_<0)
		currentSetIdx_ = (currentSetIdx_+1)%setActions_.size();
	setActions_[currentSetIdx_]->set_active();
	UpdateSet();
}

void Demo::OnNextSet()
{
	currentSetIdx_ = (currentSetIdx_+1)%setActions_.size();
	setActions_[currentSetIdx_]->set_active();
	UpdateSet();
}

Card *Demo::GetCard(int x, int y)
{
	assert((x>=0)&&(x<6));
	assert((y>=0)&&(y<2));
	return demoTable_.GetCard(x,y);
}

void Demo::SetCurrentCard(Card *card)
{
	curCard_ = card;
	if (card)
		gen_->SetGenerator(card->GetGenerator());
	else
		gen_->SetGenerator(latero::graphics::GeneratorPtr());
}

latero::graphics::GeneratorPtr Demo::Gen() { return gen_; };

} // namespace

#endif
