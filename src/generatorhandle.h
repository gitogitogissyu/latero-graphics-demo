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

#ifndef GENERATOR_HANDLE_H
#define GENERATOR_HANDLE_H

#include <laterographics/generator.h>

class GeneratorHandle;
typedef boost::shared_ptr<GeneratorHandle> GeneratorHandlePtr;

class GeneratorHandle : public latero::graphics::Generator
{
public:
	GeneratorHandle(const latero::Tactograph *dev);
	virtual ~GeneratorHandle();
	virtual void AppendXML(latero::graphics::XMLOutputNode &root) const {};
	virtual latero::graphics::gtk::Animation GetIllustration(uint w, boost::posix_time::time_duration t) const;
	virtual void PlayAudio(latero::graphics::AudioDevicePtr dev);
	void SetGenerator(latero::graphics::GeneratorPtr gen);
	latero::graphics::GeneratorPtr GetGenerator();
	virtual bool OnKeyPress(GdkEventKey* event);

protected:
	virtual void DoRender_(const latero::graphics::State *state, latero::BiasedImg &frame);
	latero::graphics::GeneratorPtr gen_;
};

#endif
