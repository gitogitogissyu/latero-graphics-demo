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

#include "cardcollection.h"
#include <iostream>

CardCollection::CardCollection()
{
}

CardCollection::~CardCollection()
{
	for (uint i=0; i<size(); ++i)
		delete (*this)[i];
	clear();
}

bool CardCollection::Load(std::string file, const latero::Tactograph *dev, uint scale)
{
	std::string path = Glib::path_get_dirname(file) + "/";

	try
	{
		xmlpp::DomParser parser;
		//parser.set_validate();
		parser.set_substitute_entities();
		parser.parse_file(file);
		if (parser)
		{
			const xmlpp::Node* pNode = parser.get_document()->get_root_node();
			if (pNode)
			{
				const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(pNode);
				if (nodeElement)
					name_ = nodeElement->get_attribute("name")->get_value();

				xmlpp::Node::NodeList list = pNode->get_children();
				for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
				{
					const xmlpp::Node* n = *iter;
					if (n->get_name() == "CardSet")
					{
						const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(n);
						assert(nodeElement);
					
						std::string file = nodeElement->get_attribute("file")->get_value();
						CardSet *cs = new CardSet();
						cs->Load(path+file, dev, scale);
						push_back(cs);
					}
				}
			}
   		}
	}
	catch(const std::exception& ex)
	{
		std::cout << "Exception caught: " << ex.what() << std::endl;
		return false;
	}
	return true;
}

#endif
