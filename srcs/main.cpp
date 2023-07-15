#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <curl/curl.h>

#include <tgbot/tgbot.h>

#include "joke_generator.hpp"


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

void createKeyboard(const std::vector<std::vector<std::string>>& buttonLayout, TgBot::ReplyKeyboardMarkup::Ptr& kb)
{
  for (size_t i = 0; i < buttonLayout.size(); ++i) {
    std::vector<TgBot::KeyboardButton::Ptr> row;
    for (size_t j = 0; j < buttonLayout[i].size(); ++j) {
      TgBot::KeyboardButton::Ptr button(new TgBot::KeyboardButton);
      button->text = buttonLayout[i][j];
      row.push_back(button);
    }
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

	curl_global_init(CURL_GLOBAL_DEFAULT);

    struct response resp;
    struct joke joke;
    
    // bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
    //     bot.getApi().sendMessage(message->chat->id, "Hi!");
    // });

	TgBot::ReplyKeyboardMarkup::Ptr keyboardOneCol(new TgBot::ReplyKeyboardMarkup);
    createOneColumnKeyboard({"New Joke", "Set up filters"}, keyboardOneCol);

	bot.getEvents().onCommand("start", [&bot, &keyboardOneCol](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hello, " + message->chat->firstName + "!\n" +
            "This bot will make you laugh!"
            , false, 0, keyboardOneCol);
        bot.getApi().sendSticker(message->chat->id, "CAACAgIAAxkBAAEJs39ksteuz27hq4bNyGwfrJ3ppVKHpAAC8gADVp29ChCdi3ZTetJkLwQ");
    });

    TgBot::ReplyKeyboardMarkup::Ptr keyboardWithLayout(new TgBot::ReplyKeyboardMarkup);
    createKeyboard({
      {"Dog", "Cat", "Mouse"},
      {"Green", "White", "Red"},
      {"On", "Off"},
      {"Back"},
      {"Info", "About", "Map", "Etc"}
    }, keyboardWithLayout);
	
    //bot.getEvents().onAnyMessage();
    bot.getEvents().onCommand("New Joke", [&bot, &keyboardOneCol](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hello, " + message->chat->firstName + "!\n" +
            "This bot will make you laugh!"
            , false, 0, keyboardOneCol);
    });

    bot.getEvents().onCommand("layout", [&bot, &keyboardWithLayout](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "/start for one column keyboard\n/layout for a more complex keyboard", false, 0, keyboardWithLayout);
    });

    bot.getEvents().onCallbackQuery([&](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "New Joke"))
            std::cout << "Hey" << std::endl;
    }
    );

    std::initializer_list<std::string> bot_commands = {"start", "help"};

    std::vector<TgBot::BotCommand::Ptr> commands;
    for (auto i : bot_commands)
    {
        TgBot::BotCommand::Ptr cmd(new TgBot::BotCommand);
        cmd->command = i;
        cmd->description = "bla";
        commands.push_back(cmd);
    }

    bot.getApi().setMyCommands(commands);

	bot.getEvents().onCommand("Find by keyword", [&bot](TgBot::Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
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

    curl_global_cleanup();

	return 0;
}