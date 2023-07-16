#include "utils.hpp"

#include <utility>

void createOneColumnKeyboard(const std::vector<std::string>& buttonStrings, TgBot::ReplyKeyboardMarkup::Ptr& kb)
{
	for (typename std::vector<std::string>::size_type i = 0; i < buttonStrings.size(); ++i)
	{
		TgBot::KeyboardButton::Ptr button(new TgBot::KeyboardButton);
		button->text = buttonStrings[i];
		button->requestContact = false;
		button->requestLocation = false;
		kb->keyboard.push_back(std::move(std::vector<TgBot::KeyboardButton::Ptr> {button}));
	}
}
