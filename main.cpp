#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <tgbot/tgbot.h>

void request_hh(const std::string &keyword);

void createOneColumnKeyboard(const std::vector<std::string>& buttonStrings, TgBot::ReplyKeyboardMarkup::Ptr& kb)
{
  for (size_t i = 0; i < buttonStrings.size(); ++i) {
    std::vector<TgBot::KeyboardButton::Ptr> row;
    TgBot::KeyboardButton::Ptr button(new TgBot::KeyboardButton);
    button->text = buttonStrings[i];
    row.push_back(button);
    kb->keyboard.push_back(row);
  }
}

int main(void)
{
	std::ifstream api_key_file;
	api_key_file.open("api_key");
	if (!api_key_file.is_open())
		return 1;
	std::string api_key;
	api_key_file >> api_key;
	api_key_file.close();

	TgBot::Bot bot(api_key); // TODO: replace with getenv("TOKEN");

	// bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
    //     bot.getApi().sendMessage(message->chat->id, "Hi!");
    // });

	TgBot::ReplyKeyboardMarkup::Ptr keyboardOneCol(new TgBot::ReplyKeyboardMarkup);
    createOneColumnKeyboard({"Find by keyword"}, keyboardOneCol);

	bot.getEvents().onCommand("start", [&bot, &keyboardOneCol](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hello!", false, 0, keyboardOneCol);
    });
	request_hh("it");

	bot.getEvents().onCommand("Find by keyword", [&bot](TgBot::Message::Ptr message) {
        request_hh("chemist");
		// printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start") || StringTools::startsWith(message->text, "/layout")) {
            return;
        }
        bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
    });

	try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            // printf("Long poll started\n");
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }

	return 0;
}