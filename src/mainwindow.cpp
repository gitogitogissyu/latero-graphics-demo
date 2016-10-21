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

#include "mainwindow.h"
#include <sstream>
#include <iostream>

MainWindow::MainWindow(latero::graphics::TactileEngine *tEngine, latero::graphics::AudioEngine *aEngine, bool disableAudio) :
#ifndef DISABLE_TEXTURE_DEMO
	textureDemo_(tEngine->Dev()),
#endif
#ifndef DISABLE_ICON_DEMO
	iconDemo_(tEngine->Dev(),this),
#endif
#ifndef DISABLE_VECTOR_DEMO
	vectorDemo_(tEngine->Dev()),
#endif
#ifndef DISABLE_SCHOOLBOOK_DEMO
	schoolbookDemo_(tEngine->Dev()),
#endif
#ifndef DISABLE_GRAPHICS_DEMO
	graphicsDemo_(tEngine->Dev()),
#endif
#ifndef DISABLE_BRAILLE_DEMO
	brailleDemo_(tEngine->Dev()),
#endif
	tEngine_(tEngine),
	aEngine_(aEngine),
	demo_(NULL)
{
	set_title("LATERO Tactile Graphics Demonstration");
	maximize();

	add(mainbox_);
	mainbox_.pack_start(*CreateMenu(), Gtk::PACK_SHRINK);
	mainbox_.pack_start(demobox_);

	std::cout << "Starting tactile & audio engines...\n";
	tEngine_->Start();
	if (!disableAudio) aEngine_->Start();

#ifndef DISABLE_VECTOR_DEMO
	OnDemoVector();
#else
#ifndef DISABLE_TEXTURE_DEMO
	OnDemoTexture();
#else
#ifndef DISABLE_GRAPHICS_DEMO
	OnDemoGraphics();
#else
#ifndef DISABLE_SCHOOLBOOK_DEMO
	OnDemoSchoolbook();
#else
#ifndef DISABLE_ICON_DEMO
	OnDemoIcon();
#endif
#ifndef DISABLE_BRAILLE_DEMO
	OnDemoBraille();
#endif

#endif
#endif
#endif
#endif

	show_all_children();
}

MainWindow::~MainWindow() 
{
	std::cout << "~MainWindow()\n";

	std::cout << "Stopping tactile & audio engines...\n";
	tEngine_->Stop();
	aEngine_->Stop();
	std::cout << "Stopped tactile & audio engines...\n";
}


Gtk::Widget *MainWindow::CreateMenu()
{
	Glib::RefPtr<Gtk::ActionGroup> group = Gtk::ActionGroup::create();

	//
	// FILE
	//

	group->add(Gtk::Action::create("FileMenu", "File"));
	group->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT), 
		sigc::mem_fun(*this, &MainWindow::OnQuit));

	std::stringstream fileMenuUI;
	fileMenuUI << "<menu action='FileMenu'>";
	fileMenuUI << 	"<menuitem action='FileQuit'/>";
	fileMenuUI << "</menu>";

	//
	// VIEW
	//

	group->add(Gtk::Action::create("ViewMenu", "View"));
	actionSetFullscreen_ = Gtk::ToggleAction::create("Fullscreen", "fullscreen");
	group->add(actionSetFullscreen_, Gtk::AccelKey("F4"), sigc::mem_fun(*this, &MainWindow::OnFullscreen));
	//actionSetFullscreen_->set_active();

	std::stringstream viewMenuUI;
	viewMenuUI << "<menu action='ViewMenu'>";
	viewMenuUI << 	"<menuitem action='Fullscreen'/>";
	viewMenuUI << "</menu>";

	//
	// DEMO
	//

	group->add(Gtk::Action::create("DemoMenu", "Demo"));
	Gtk::RadioAction::Group demoGroup;
#ifndef DISABLE_VECTOR_DEMO
	actionDemoVector_ = Gtk::RadioAction::create(demoGroup, "DemoVector", "Vector");
	group->add(actionDemoVector_,sigc::mem_fun(*this, &MainWindow::OnDemoVector));
#endif
#ifndef DISABLE_ICON_DEMO
	actionDemoIcon_ = Gtk::RadioAction::create(demoGroup, "DemoIcon", "Icon");
	group->add(actionDemoIcon_,sigc::mem_fun(*this, &MainWindow::OnDemoIcon));
#endif
#ifndef DISABLE_TEXTURE_DEMO
	actionDemoTexture_ = Gtk::RadioAction::create(demoGroup, "DemoTexture", "Texture");
	group->add(actionDemoTexture_ ,sigc::mem_fun(*this, &MainWindow::OnDemoTexture));
#endif
#ifndef DISABLE_GRAPHICS_DEMO
	actionDemoGraphics_ = Gtk::RadioAction::create(demoGroup, "DemoGraphics", "Graphics");
	group->add(actionDemoGraphics_ ,sigc::mem_fun(*this, &MainWindow::OnDemoGraphics));
#endif
#ifndef DISABLE_SCHOOLBOOK_DEMO
	actionDemoSchoolbook_ = Gtk::RadioAction::create(demoGroup, "DemoSchoolbook", "Schoolbook");
	group->add(actionDemoSchoolbook_ ,sigc::mem_fun(*this, &MainWindow::OnDemoSchoolbook));
#endif
#ifndef DISABLE_BRAILLE_DEMO
	actionDemoBraille_ = Gtk::RadioAction::create(demoGroup, "DemoBraille", "Braille");
	group->add(actionDemoBraille_ ,sigc::mem_fun(*this, &MainWindow::OnDemoBraille));
#endif

	std::stringstream demoMenuUI;
	demoMenuUI << "<menu action='DemoMenu'>";
#ifndef DISABLE_VECTOR_DEMO
	demoMenuUI << 	"<menuitem action='DemoVector'/>";
#endif
#ifndef DISABLE_ICON_DEMO
	demoMenuUI << 	"<menuitem action='DemoIcon'/>";
#endif
#ifndef DISABLE_TEXTURE_DEMO
	demoMenuUI << 	"<menuitem action='DemoTexture'/>";
#endif
#ifndef DISABLE_GRAPHICS_DEMO
	demoMenuUI << 	"<menuitem action='DemoGraphics'/>";
#endif
#ifndef DISABLE_SCHOOLBOOK_DEMO
	demoMenuUI << 	"<menuitem action='DemoSchoolbook'/>";
#endif
#ifndef DISABLE_BRAILLE_DEMO
	demoMenuUI << 	"<menuitem action='DemoBraille'/>";
#endif
	demoMenuUI << "</menu>";

	//
	// MENU
	//

	uiManager_ = Gtk::UIManager::create();
	uiManager_->insert_action_group(group);
 	add_accel_group(uiManager_->get_accel_group());

	std::stringstream buf;
	buf << "<ui>";
	buf << "<menubar name='MenuBar'>";
	buf << fileMenuUI.str();
	buf << viewMenuUI.str();
	buf << demoMenuUI.str();
	buf << "</menubar>";
	buf << "</ui>";

	try
	{	
		uiManager_->add_ui_from_string(buf.str());
	}
	catch(const Glib::Error& ex)
	{
		std::cerr << "building menus failed: " <<  ex.what();
		Gtk::Main::quit();
	}

	return uiManager_->get_widget("/MenuBar");
}

void MainWindow::OnQuit()
{
	Gtk::Main::quit();
}

void MainWindow::OnFullscreen()
{
	if (actionSetFullscreen_->get_active())
		fullscreen();
	else
		unfullscreen();
}

#ifndef DISABLE_VECTOR_DEMO
void MainWindow::OnDemoVector()
{
	if (actionDemoVector_->get_active())
		ReplaceDemo(&vectorDemo_);
}
#endif

#ifndef DISABLE_ICON_DEMO
void MainWindow::OnDemoIcon()
{
	if (actionDemoIcon_->get_active())
		ReplaceDemo(&iconDemo_);

}
#endif

#ifndef DISABLE_TEXTURE_DEMO
void MainWindow::OnDemoTexture()
{
	if (actionDemoTexture_->get_active())
		ReplaceDemo(&textureDemo_);
}
#endif

#ifndef DISABLE_GRAPHICS_DEMO
void MainWindow::OnDemoGraphics()
{
	if (actionDemoGraphics_->get_active())
		ReplaceDemo(&graphicsDemo_);
}
#endif

#ifndef DISABLE_SCHOOLBOOK_DEMO
void MainWindow::OnDemoSchoolbook()
{
	if (actionDemoSchoolbook_->get_active())
		ReplaceDemo(&schoolbookDemo_);
}
#endif

#ifndef DISABLE_BRAILLE_DEMO
void MainWindow::OnDemoBraille()
{
	if (actionDemoBraille_->get_active())
		ReplaceDemo(&brailleDemo_);
}
#endif


void MainWindow::ReplaceDemo(Demo *demo)
{
	if (demo_) demobox_.remove(*demo_);
	demo_ = demo;
	demobox_.add(*demo_);
	show_all_children();
	tEngine_->SetGenerator(demo_->Gen());
	aEngine_->SetGenerator(demo_->Gen());
}
