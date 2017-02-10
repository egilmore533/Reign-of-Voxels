#include <iostream>
#include <fstream>
#include "Layout.h"
#include "textbox.h"
#include "button.h"
#include "label.h"
#include "Json.hpp"
#include "simple_logger.h"


/*
* @brief loads menu layout from a Json file and generates widgets based on layout data
* @param menu specifies which menu layout to load from file
* @return returns a vector of widget points created according to Json file data
*/
std::vector<Widget*> Layout::LoadMenuLayout(MenuLayouts menu)
{
	Json layout;
	std::string path = "Resources\\gui\\menu.Json";
	std::string menu_type = "";

	switch (menu)
	{
	case LoginMenu:
		menu_type = "login";
		break;
	case LobbyMenu:
		break;
	case OptionsMenu:
		break;
	default:
		break;
	}

	std::ifstream in(path.c_str());
	in >> layout;
	layout = layout[menu_type];

	Json widgets = layout["widgets"];
	std::vector<Widget*> widget_list;

	m_font = new sf::Font();
	if (!m_font->loadFromFile(layout["font"]))
		slog("Failed to load font");

	m_padding = sf::Vector2f(layout["padding"][0], layout["padding"][1]);
	m_margin = sf::Vector2f(layout["margin"][0], layout["margin"][1]);

	for (Json::iterator it = widgets.begin(); it != widgets.end(); ++it)
	{
		Widget *widget = NULL;
		Json data = *it;
		std::string type = data["type"];

		float posx, posy;

		if (type == "textbox")
			widget = new TextBox();
		if (type == "label")
			widget = new Label();
		if (type == "button")
			widget = new Button();
		
		if (widget)
		{
			LoadWidgetData(data, widget);
			widget_list.push_back(widget);
		}
	}

	return widget_list;
}

void Layout::LoadWidgetData(Json data, Widget *widget)
{
	int posx, posy;
	int width, height;
	

	widget->setFont(m_font);
	//widget->setMargin(m_margin);
	widget->setPadding(m_padding);

	if (data.find("id") != data.end())
	{
		std::string id = data["id"];
		widget->setID(id);
	}
	//get position
	if (data["position"].is_string())
	{
		if (data["position"] == "center")
		{
			posx = g_window->getSize().x / 2;
			posy = g_window->getSize().y / 2;
		}
	}
	else if (data["position"].is_array())
	{
		posx = data["position"][0];
		posy = data["position"][1];
	}

	if (data.find("offset") != data.end())
	{
		int x = data["offset"][0];
		int y = data["offset"][1];

		posx += x;
		posy += y;
	}
	//set width and height
	if (data.find("width") != data.end() && data.find("height") != data.end())
	{
		width = data["width"];
		height = data["height"];

		widget->setSize(width, height);
	}
	if (data.find("text_size") != data.end())
	{
		widget->setTextSize(data["text_size"]);
	}
	//get text color rgb
	sf::Color text_color;
	Json color = data["text_color"];
	text_color = sf::Color(color[0], color[1], color[2], color[3]);

	//set box color
	sf::Color box_color;
	color = data["box_color"];
	box_color = sf::Color(color[0], color[1], color[2], color[3]);
	
	//set widget text
	if (data.find("string") != data.end())
	{
		std::string str = data["string"];
		widget->setString(sf::String(str));
	}
	//applies to textbox
	if (data.find("max_str_length") != data.end())
	{
		widget->setMaxStrLen(data["max_str_length"]);
	}

	if (data.find("event") != data.end())
	{
		widget->setEvent(data["event"]);
	}

	widget->setPadding(m_padding);
	widget->setPosition(sf::Vector2f(posx, posy));
	widget->setTextColor(text_color);
	widget->setBoxColor(box_color);
}

sf::Font & Layout::getFont()
{
	return *m_font;
}

sf::Vector2f Layout::getBoxSize()
{
	return sf::Vector2f(100.0f, 50.0f);
}

sf::Vector2f Layout::getPadding()
{
	return sf::Vector2f(10.0f, 10.0f);
}

sf::Vector2f Layout::getMargin()
{
	return sf::Vector2f(0, 0);
}