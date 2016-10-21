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

#include "generatorhandle.h"

GeneratorHandle::GeneratorHandle(const latero::Tactograph *dev) :
	Generator(dev)
{
}

GeneratorHandle::~GeneratorHandle()
{
}

void GeneratorHandle::DoRender_(const latero::graphics::State *state, latero::BiasedImg &frame)
{
	if (gen_)
		gen_->Render_(state, frame);
	else
		frame.Set(0);
}

void GeneratorHandle::SetGenerator(latero::graphics::GeneratorPtr gen)
{
	LATERO_GRAPHICS_GUARD
	SetLastModified_();
	if (gen_) RemoveModifiableChild_(gen_);
	gen_ = gen;
	if (gen_) AddModifiableChild_(gen_);
}

latero::graphics::gtk::Animation GeneratorHandle::GetIllustration(uint w, boost::posix_time::time_duration t) const
{
	LATERO_GRAPHICS_GUARD;
	if (gen_)
		return gen_->GetIllustration(w,t);
	else
		return Generator::GetIllustration(w,t);
}

void GeneratorHandle::PlayAudio(audiere::AudioDevicePtr dev)
{
	latero::graphics::GeneratorPtr gen = GetGenerator();
	if (gen)
	  gen->PlayAudio(dev);
}

latero::graphics::GeneratorPtr GeneratorHandle::GetGenerator()
{
	LATERO_GRAPHICS_GUARD;
	return gen_;
}

bool GeneratorHandle::OnKeyPress(GdkEventKey* event)
{
	return GetGenerator()->OnKeyPress(event);
}
