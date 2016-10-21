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

#include "cardset.h"
#include <iostream>
#include "settings.h"

CardSet::CardSet()
{
}

CardSet::~CardSet()
{
	DeleteCards();
}

bool CardSet::Load(std::string file, const latero::Tactograph *dev, uint scale)
{
	std::cout << "loading card set: " << file << "\n";
	boost::posix_time::ptime t0 = boost::posix_time::microsec_clock::universal_time();

	// find the path
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
					if (n->get_name() == "card")
					{
						const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(n);
						assert(nodeElement);
					
						//printf("Creating generator...\n"); fflush(stdout);
						std::string file = nodeElement->get_attribute("file")->get_value();
						//std::cout << "loading card: " << file << "\n";

						//std::cout << "creating generator\n";
						latero::graphics::GeneratorPtr gen = latero::graphics::Generator::Create(path + file, dev);
						managedGenerators_.push_back(gen);
						//printf("...done\n"); fflush(stdout);

						//std::cout << "creating card\n";
						Card *c = new Card(
							gen,
							DefaultCardWidth,
							DefaultCardWidth * dev->GetSurfaceHeight() / dev->GetSurfaceWidth(), 
							scale);

						// TODO this is a hack
						if (size() < 12)
						{
							push_back(c);
						}
						else
						{
							// TODO: don't create the card and generator!
							std::cout << "Skipping card from set " << name_ << "(" << file << "):\n" << path + file << "\n";	
							delete c;
						}
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

	printf("\n\ndone in %f seconds\n\n", (boost::posix_time::microsec_clock::universal_time() - t0).total_milliseconds() / 1000.0);

	return true;
}

void CardSet::Shuffle()
{
	std::random_shuffle(begin(), end());
}

#endif
