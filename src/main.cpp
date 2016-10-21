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

#include <iostream>
#include <gtkmm/main.h>
#include "mainwindow.h"
#include <laterographics/tactileengine.h>
#include <laterographics/audioengine.h>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
const float DefaultUpdateRateHz = 750.0;

int main(int argc, char *argv[])
{
	// parse command-line parameters

	// description
	po::options_description desc("Options");
	desc.add_options()
    	("haptic-rate", po::value<int>(), "set haptic update rate")
	("input-file", po::value< std::vector<std::string> >(), "load input file")
   	("disable-audio",po::value<bool>(), "disable audio thread")
	;

	// unnamed argument is interpreted as input-file
	po::positional_options_description p;
	p.add("input-file", -1);

	// parse
	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	po::notify(vm);

	// convert
	if (vm.count("help")) { std::cout << desc << "\n"; return 1; }
	float UpdateRateHz = DefaultUpdateRateHz; // default
	if (vm.count("haptic-rate")) UpdateRateHz = vm["haptic-rate"].as<int>();
	std::string filename;
	if (vm.count("input-file"))
		filename = vm["input-file"].as< std::vector<std::string> >().at(0); // ignore multiples for now
	bool disableAudio = false;
	if (vm.count("disable-audio")) disableAudio = true;


	Gtk::Main kit(argc, argv);
	
	latero::Tactograph dev;

	boost::posix_time::time_duration UpdatePeriod = boost::posix_time::microseconds(1E6/UpdateRateHz);
	latero::graphics::TactileEngine tEngine(&dev,UpdatePeriod);
	latero::graphics::AudioEngine aEngine(&dev, boost::posix_time::milliseconds(30));
	MainWindow wnd(&tEngine, &aEngine, disableAudio);

	std::cout << "Starting graphical user interface...\n";
	Gtk::Main::run(wnd);

	std::cout << "Exiting...\n";
	return 0;
}
