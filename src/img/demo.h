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

#ifndef IMG_DEMO_H
#define IMG_DEMO_H

#include "../config.h"
#ifndef DISABLE_GRAPHICS_DEMO

#include "cardset.h"
#include "cardtable.h"
#include "cardcollection.h"
#include "../generatorhandle.h"
#include "../demo.h"

namespace ImgDemo {

class Demo : public ::Demo
{
public:
	Demo(const latero::Tactograph *dev);
	virtual ~Demo();

	virtual latero::graphics::GeneratorPtr Gen();

protected:
	void OnPrevSet();
	void OnNextSet();

	void UpdateSet();
	void ClearSet();
	void LoadSet(const CardSet &set);
	void Reset();

	bool OnBoardClick(GdkEventButton*);
	void OnDemoClick(Card* card);
	void OnRightClick(Card* card);
	void OnSetChanged();
	bool OnIdle();
	void OnShowCursor();
	bool OnKeyPress(GdkEventKey* event);
	void UpdateZoom(Card* card);


private:
	void SetCurrentCard(Card *card);
	Card *GetCard(int x, int y);
	CardCollection cardSets_;
	void UpdateMode();

	CardSet demoCards_;
	const CardSet* currentSet_;

	latero::graphics::BaseVirtualSurfaceWidget zoomImg_;

	CardTable demoTable_;
	Card* curCard_; // currently activated card	
	GeneratorHandlePtr gen_;

	std::vector<Glib::RefPtr<Gtk::RadioAction> > setActions_;

	latero::graphics::INTPoint keyLocation_;
	int currentSetIdx_;
};

} // namespace

#endif
#endif
