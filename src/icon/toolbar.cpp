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
#ifndef DISABLE_ICON_DEMO

#include <laterographics/graphics/texture/texturecombo.h>
#include "toolbar.h"
#include <gtkmm.h>
#include <laterographics/gtk/imagecombo.h>
#include "../config.h"

namespace IconDemo {

const std::string img_path = media_dir + "/icon/img/";

const std::string shape_img_path 	= img_path+"shape/";
const std::string shape_img_circle 	= shape_img_path+"Circle.svg";
const std::string shape_img_square 	= shape_img_path+"Square.svg";
const std::string shape_img_triangle 	= shape_img_path+"Triangle.svg";
const std::string shape_img_ratriang 	= shape_img_path+"Ratriang.svg";
const std::string shape_img_plus 	= shape_img_path+"Plus.svg";
const std::string shape_img_diamond 	= shape_img_path+"Diamond.svg";

const std::string size_img_path 	= img_path+"size/";
const std::string size_img_s 		= size_img_path+"small.png";
const std::string size_img_m 		= size_img_path+"medium.png";
const std::string size_img_l 		= size_img_path+"large.png";

const std::string contour_img_path 	= img_path+"contour/";
const std::string contour_img_vib 	= contour_img_path+"line-vib.png";
const std::string contour_img_none	= contour_img_path+"line-none.png";
const std::string contour_img_dot 	= contour_img_path+"line-dots.png";
const std::string contour_img_stroke 	= contour_img_path+"line-stroke.png";

const std::string thickness_img_path 	= img_path+"thickness/";
const std::string thickness_img_s 	= thickness_img_path+"small.png";
const std::string thickness_img_m 	= thickness_img_path+"medium.png";
const std::string thickness_img_l 	= thickness_img_path+"large.png";

const std::string advanced_img	 	= img_path+"advanced.svg";


class AdvancedButton : public Gtk::Button
{
public:
	AdvancedButton() { set_image(*manage(new Gtk::Image(advanced_img))); }
};


class SizeCombo : public latero::graphics::gtk::ImageCombo
{
public:
	SizeCombo(GeneratorPtr peer) : peer_(peer)
	{
		Append(Generator::SIZE_S, size_img_s);
		Append(Generator::SIZE_M, size_img_m);
		Append(Generator::SIZE_L, size_img_l);
		SetActive(peer_->GetSize());
		SignalChanged().connect(sigc::mem_fun(*this, &SizeCombo::OnChange) );
	}
	virtual ~SizeCombo() {};
protected:
	void OnChange(int id)  { peer_->SetSize(id); }
	GeneratorPtr peer_;
};


class ContourThicknessCombo : public latero::graphics::gtk::ImageCombo
{
public:
	ContourThicknessCombo(GeneratorPtr peer)  : peer_(peer)
	{
		Append(Generator::CONTOUR_THICK_S, thickness_img_s);
		Append(Generator::CONTOUR_THICK_M, thickness_img_m);
		Append(Generator::CONTOUR_THICK_L, thickness_img_l);
		SetActive(peer_->GetContourThickness());
		SignalChanged().connect(sigc::mem_fun(*this, &ContourThicknessCombo::OnChange) );
	}
	virtual ~ContourThicknessCombo() {};
protected:
	void OnChange(int id) { peer_->SetContourThickness(id); };
	GeneratorPtr peer_;
};


class ContourCombo : public latero::graphics::gtk::ImageCombo
{
public:
	ContourCombo(GeneratorPtr peer)  : peer_(peer)
	{
		Append(Generator::CONTOUR_NONE, contour_img_none);
		Append(Generator::CONTOUR_VIB, contour_img_vib);
		Append(Generator::CONTOUR_STROKE, contour_img_stroke);
		Append(Generator::CONTOUR_DOT, contour_img_dot);
		SetActive(peer_->GetContour());
		SignalChanged().connect(sigc::mem_fun(*this, &ContourCombo::OnChange) );
	}
	virtual ~ContourCombo() {};
protected:
	void OnChange(int id) { peer_->SetContour(id); }
	GeneratorPtr peer_;
};


class ShapeCombo : public latero::graphics::gtk::ImageCombo
{
public:
	ShapeCombo(GeneratorPtr peer) : peer_(peer)
	{
		Append(Generator::SHAPE_CIRCLE, shape_img_circle);
		Append(Generator::SHAPE_SQUARE, shape_img_square);
		Append(Generator::SHAPE_TRIANGLE, shape_img_triangle);
		Append(Generator::SHAPE_RATRIANG, shape_img_ratriang);
		Append(Generator::SHAPE_PLUS, shape_img_plus);
		Append(Generator::SHAPE_DIAMOND, shape_img_diamond);
		SetActive(peer_->GetShape());
		SignalChanged().connect(sigc::mem_fun(*this, &ShapeCombo::OnChange) );
	}
	virtual ~ShapeCombo() {};
protected:
	void OnChange(int id)  { peer_->SetShape(id); }
	GeneratorPtr peer_;
};


Toolbar::Toolbar(GeneratorPtr peer, Gtk::Window *window) :
	window_(window), peer_(peer)
{
	Rebuild();
}

void Toolbar::Rebuild()
{
	remove();
	
	Gtk::HBox *box = manage(new Gtk::HBox);
	Gtk::VBox *vbox = manage(new Gtk::VBox);
	latero::graphics::TextureCombo* txCombo = manage(new latero::graphics::TextureCombo(peer_->GetAreaTexture()));

	add(*box);
	box->pack_start(*manage(new ShapeCombo(peer_)), Gtk::PACK_SHRINK);
	box->pack_start(*manage(new SizeCombo(peer_)), Gtk::PACK_SHRINK);
	box->pack_start(*vbox, Gtk::PACK_SHRINK);
	vbox->pack_start(*manage(new ContourCombo(peer_)));
	vbox->pack_start(*manage(new ContourThicknessCombo(peer_)));
	box->pack_start(*txCombo, Gtk::PACK_SHRINK);
	AdvancedButton *advancedButton = manage(new AdvancedButton);
	box->pack_start(*advancedButton, Gtk::PACK_SHRINK);
	show_all_children();

	advancedButton->signal_clicked().connect(sigc::mem_fun(*this, &Toolbar::OnAdvanced));
	txCombo->SignalTextureChanged().connect(sigc::mem_fun(*this, &Toolbar::OnTextureChanged));
}

void Toolbar::OnAdvanced()
{
	Gtk::Dialog genWidget("Icon Properties", *window_, true);
	genWidget.get_vbox()->add(*manage(peer_->CreateWidget(peer_)));
	genWidget.show_all_children();
	genWidget.run();
	Rebuild(); // reconstruct with new settings
}

void Toolbar::OnTextureChanged(latero::graphics::TexturePtr tx)
{
	peer_->SetAreaTexture(tx);
}

} // namespace

#endif // !DISABLED
