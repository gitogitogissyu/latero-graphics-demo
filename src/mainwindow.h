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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "config.h"
#include <gtkmm.h>
#include <laterographics/tactileengine.h>
#include <laterographics/audioengine.h>
#include "img/demo.h"
#include "icon/demo.h"
#include "texture/demo.h"
#include "schoolbook/demo.h"
#include "braille/demo.h"
#include "vector/demo.h"

class MainWindow : public Gtk::Window
{
public:
	MainWindow(latero::graphics::TactileEngine *tEngine, latero::graphics::AudioEngine *aEngine, bool disableAudio);
	virtual ~MainWindow();

protected:
	void OnQuit();
	void OnFullscreen();

#ifndef DISABLE_TEXTURE_DEMO
	TextureDemo::Demo textureDemo_;
	void OnDemoTexture();
	Glib::RefPtr<Gtk::RadioAction> actionDemoTexture_;
#endif

#ifndef DISABLE_ICON_DEMO
	IconDemo::Demo iconDemo_;
	void OnDemoIcon();
	Glib::RefPtr<Gtk::RadioAction> actionDemoIcon_;
#endif

#ifndef DISABLE_VECTOR_DEMO
	VectorDemo::Demo vectorDemo_;
	void OnDemoVector();
	Glib::RefPtr<Gtk::RadioAction> actionDemoVector_;
#endif
    
#ifndef DISABLE_SCHOOLBOOK_DEMO
	SchoolbookDemo::Demo schoolbookDemo_;
	void OnDemoSchoolbook();
	Glib::RefPtr<Gtk::RadioAction> actionDemoSchoolbook_;
#endif
    
#ifndef DISABLE_GRAPHICS_DEMO
	ImgDemo::Demo graphicsDemo_;
	void OnDemoGraphics();
	Glib::RefPtr<Gtk::RadioAction> actionDemoGraphics_;
#endif

#ifndef DISABLE_BRAILLE_DEMO
	BrailleDemo::Demo brailleDemo_;
	void OnDemoBraille();
	Glib::RefPtr<Gtk::RadioAction> actionDemoBraille_;
#endif

	void ReplaceDemo(Demo *demo);

	Gtk::Widget *CreateMenu();

	Glib::RefPtr<Gtk::UIManager> uiManager_;
	Glib::RefPtr<Gtk::ToggleAction> actionSetFullscreen_;

	Gtk::VBox mainbox_;
	Gtk::VBox demobox_;
	latero::graphics::TactileEngine *tEngine_;
	latero::graphics::AudioEngine *aEngine_;
	Demo *demo_;
};



#endif


