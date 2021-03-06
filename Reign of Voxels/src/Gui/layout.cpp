#include <iostream>
#include <fstream>
#include "game.h"
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
	std::string path = "Resources\\gui\\menu.json";
	std::string menu_type = "";

	switch (menu)
	{
	case LoginMenu:
		menu_type = "login";
		break;
	case LobbyMenu:
		menu_type = "lobby";
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

	//set font
	m_font = new sf::Font();
	std::cout << "font path: " << layout["font"] << std::endl;

	if (!m_font->loadFromFile(layout["font"]))
		slog("Failed to load font");
	//set padding
	m_padding = sf::Vector2f(layout["padding"][0], layout["padding"][1]);

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
/*
* @brief Loads widget position, size, textcolor, boxcolor, textsize, box size, trigger event, and id
into newly allocated widget.
* @param data the json container of the specific widget being loaded
* @param widget the newly allocated widget.
*/
void Layout::LoadWidgetData(Json &data, Widget *widget)
{
	int posx, posy;
	int width, height;
	widget->setFont(m_font);
	widget->setPadding(m_padding);

	if (data.find("id") != data.end())
		widget->setID(data["id"]);

	//set width and height
	if (data.find("width") != data.end() && data.find("height") != data.end())
	{
		width = data["width"];
		height = data["height"];
		widget->setSize(width, height);
	}
	//get position
	if (data["position"].is_string())
	{
		if (data["position"] == "center")
		{
			posx = Game::instance().getWindow()->getSize().x / 2;
			posy = Game::instance().getWindow()->getSize().y / 2;
		}
		else if (data["position"] == "br")//bottom right
		{
			posx = Game::instance().getWindow()->getSize().x - width;
			posy = Game::instance().getWindow()->getSize().y - height;
		}
		else if (data["position"] == "bl")//bottom left
		{
			posx = 0;
			posy = Game::instance().getWindow()->getSize().y - height;
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
/*
* @brief Returns the font being used by this layout
* @return the font this layout is using
*/
sf::Font & Layout::getFont()
{
	return *m_font;
}
/*
* @brief Returns the padding being used by this layout
* @return the padding this layout is using
*/
sf::Vector2f Layout::getPadding()
{
	return sf::Vector2f(10.0f, 10.0f);
}
