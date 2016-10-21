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


#include "braillestringwidget.h"
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

BrailleStringWidget::BrailleStringWidget(BrailleString *peer):
	Frame("Braille String"),
	randomButton_("random"),
	wordButton_("word"),
	peer_(peer)
{
	srand(time(NULL));

	Gtk::RadioButton::Group group = cellMode_.get_group();
	textMode_.set_group(group);
	cellMode_.set_active(true);

	Gtk::HBox *box = manage(new Gtk::HBox);

	add(*box);
	box->pack_start(cellMode_, Gtk::PACK_SHRINK);
	box->pack_start(cellBox_, Gtk::PACK_SHRINK);
	for (uint i=0; i<peer_->GetSize(); ++i)
	{
		BrailleCellWidget *cellWidget = manage(new BrailleCellWidget(peer_->GetCell(i)));
		cell_.push_back(cellWidget);
		cellBox_.pack_start(*cellWidget, Gtk::PACK_SHRINK);
	}
	box->pack_start(textMode_, Gtk::PACK_SHRINK);
	box->pack_start(textBox_, Gtk::PACK_SHRINK);
	textBox_.pack_start(entry_, Gtk::PACK_SHRINK);
	textBox_.pack_start(randomButton_, Gtk::PACK_SHRINK);
	textBox_.pack_start(wordButton_, Gtk::PACK_SHRINK);

  	textMode_.signal_clicked().connect(
		sigc::mem_fun(*this, &BrailleStringWidget::OnModeChange));
  	cellMode_.signal_clicked().connect(
		sigc::mem_fun(*this, &BrailleStringWidget::OnModeChange));
	randomButton_.signal_clicked().connect(
		sigc::mem_fun(*this, &BrailleStringWidget::OnRandomClicked));
	wordButton_.signal_clicked().connect(
		sigc::mem_fun(*this, &BrailleStringWidget::OnWordClicked));

	for (uint i=0; i<cell_.size(); ++i)
		cell_[i]->signal_value_changed.connect(
			sigc::mem_fun(*this, &BrailleStringWidget::OnCellChange));
	entry_.signal_insert_text().connect(
		sigc::mem_fun(*this, &BrailleStringWidget::OnInsertText));
	entry_.signal_changed().connect(
		sigc::mem_fun(*this, &BrailleStringWidget::OnTextChange));


	entry_.set_max_length(cell_.size());
	entry_.set_width_chars(cell_.size());
	entry_.set_text(peer_->Get());
	Glib::RefPtr<Gtk::RcStyle> style = entry_.get_modifier_style();
	Pango::FontDescription font;
	font.set_size(40000);
	style->set_font(font);
	entry_.modify_style(style);

	CreateWords();
	
	OnModeChange();
}

BrailleStringWidget::~BrailleStringWidget()
{
}

void BrailleStringWidget::OnModeChange()
{
	cellBox_.set_sensitive(cellMode_.get_active());
	textBox_.set_sensitive(textMode_.get_active());
}

void BrailleStringWidget::OnTextChange()
{
	if (textMode_.get_active())
	{
		std::stringstream text;
		text << entry_.get_text();
		while (text.str().size() < cell_.size())
			text << " ";
		peer_->Set(text.str());
		std::string pattern = peer_->GetPattern();
		for (uint i=0; (i<pattern.size()) && (i<cell_.size()); ++i)
			cell_[i]->SetPattern(pattern.at(i));
	}
	signal_value_changed();
}

void BrailleStringWidget::OnCellChange()
{
	if (cellMode_.get_active())
		entry_.set_text(peer_->Get());
	signal_value_changed();
}


void BrailleStringWidget::OnInsertText(const Glib::ustring& str, int *pos)
{
	if (str.find_first_not_of("abcdefghijklmnopqrstuvwxyz ") != std::string::npos)
	{
		// TODO: figure out how to handle this better...
		*pos = 0;
		entry_.set_text("");
	}
}

void BrailleStringWidget::OnRandomClicked()
{
	if (textMode_.get_active())
	{
		std::stringstream text;
		for (uint i=0; i<cell_.size(); ++i)
			text << (char)(97 + (rand()%26));
		entry_.set_text(text.str());
	}
}

void BrailleStringWidget::OnWordClicked()
{
	if (textMode_.get_active())
		entry_.set_text(words_[rand()%words_.size()]);
}

void BrailleStringWidget::CreateWords()
{
	std::vector<std::string> words;
	words.push_back("rat");
	words.push_back("chat");
	words.push_back("loup");
	words.push_back("chien");
	words.push_back("cheval");
	words.push_back("souris");
	words.push_back("elephant");
	words.push_back("renard");
	words.push_back("oiseau");
	words.push_back("lion");
	words.push_back("tigre");
	words.push_back("aigle");
	words.push_back("canard");
	words.push_back("coq");
	words.push_back("girafe");
	words.push_back("lapin");
	words.push_back("ours");
   	words.push_back("abeille");
    	words.push_back("agneau");
    	words.push_back("anaconda");
    	words.push_back("antilope");
    	words.push_back("biche");
    	words.push_back("bison");
    	words.push_back("blaireau");
    	words.push_back("boeuf");
    	words.push_back("bouc");
    	words.push_back("brebis");
    	words.push_back("canari");
    	words.push_back("carcajou");
    	words.push_back("caribou");
    	words.push_back("castor");
    	words.push_back("cerf");
    	words.push_back("chacal");
    	words.push_back("chouette");
    	words.push_back("cigogne");
    	words.push_back("cochon");
    	words.push_back("corbeau");
    	words.push_back("crabe");
    	words.push_back("crapaud");
    	words.push_back("crevette");
    	words.push_back("dauphin");
    	words.push_back("faisan");
    	words.push_back("fourmi");
    	words.push_back("hibou");
    	words.push_back("koala");
    	words.push_back("loutre");
    	words.push_back("louve");
    	words.push_back("lynx");
    	words.push_back("manchot");
    	words.push_back("mouche");
    	words.push_back("oie");
    	words.push_back("phoque");
    	words.push_back("pieuvre");
    	words.push_back("pingouin");
    	words.push_back("poisson");
    	words.push_back("poule");
    	words.push_back("puce");
    	words.push_back("python");
    	words.push_back("requin");
    	words.push_back("sanglier");
    	words.push_back("scorpion");
    	words.push_back("serpent");
    	words.push_back("taupe");
    	words.push_back("taureau");
    	words.push_back("tortue");
    	words.push_back("vache");

	// limit of 5 letters...
	for (uint i=0; i<words.size(); i++)
		if (words[i].length() <= 5)
			words_.push_back(words[i]);

	/*
	words_.push_back("amande");
	words_.push_back("ananas");
	words_.push_back("banane");
	words_.push_back("cerise");
	words_.push_back("citron");
	words_.push_back("figue");
	words_.push_back("fraise");
	words_.push_back("kiwi");
	words_.push_back("mangue");
	words_.push_back("melon");
	words_.push_back("mure");
	words_.push_back("noix");
	words_.push_back("olive");
	words_.push_back("orange");
	words_.push_back("papaye");
	words_.push_back("peche");
	words_.push_back("piment");
	words_.push_back("poire");
	words_.push_back("pomme");
	words_.push_back("prune");
	words_.push_back("pruneau");
	words_.push_back("raisin");
	words_.push_back("tomate");
	*/
}


#endif
