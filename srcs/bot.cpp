#include <chrono>
#include <thread>

#include <ctime>
#include <cstdlib>

#include "bot.hpp"
#include "utils.hpp"
#include "request_response.hpp"

tgbot::tgbot(const std::string &api_key):
	m_bot(api_key), m_longPoll(m_bot)
{
	std::vector<TgBot::BotCommand::Ptr> commands;

	for (typename std::vector<std::string>::size_type i = 0; i < bot_cmds.size(); ++i)
	{
		TgBot::BotCommand::Ptr cmd(new TgBot::BotCommand);
		cmd->command = bot_cmds[i];
		cmd->description = bot_cmds_descriptions[i];
		commands.push_back(cmd);
	}

	m_bot.getApi().setMyCommands(commands);
}

static const std::string random_smile()
{
	srand(time(NULL));

	std::vector<std::string> v = {"\xF0\x9F\x98\x81"
								, "\xF0\x9F\x98\x82"
								, "\xF0\x9F\x98\x83"
								, "\xF0\x9F\x98\x84"
								, "\xF0\x9F\x98\x85"
								, "\xF0\x9F\x98\x86"
								, "\xF0\x9F\x98\x9C"};

	return std::move(v[rand() % v.size()]);
}


void tgbot::run()
{
	TgBot::ReplyKeyboardMarkup::Ptr keyboardOneCol(new TgBot::ReplyKeyboardMarkup);
	createOneColumnKeyboard({"Generate Joke", "Set up filter"}, keyboardOneCol);

	m_bot.getEvents().onCommand("start", [&](TgBot::Message::Ptr message) {
        m_bot.getApi().sendMessage(message->chat->id, "Hello, " + message->chat->firstName + "!\n" +
            "This bot will make you laugh!");
        m_bot.getApi().sendSticker(message->chat->id, "CAACAgIAAxkBAAEJs39ksteuz27hq4bNyGwfrJ3ppVKHpAAC8gADVp29ChCdi3ZTetJkLwQ",
			0, keyboardOneCol);
    });

	m_bot.getEvents().onCommand("help", [&](TgBot::Message::Ptr message) {
        m_bot.getApi().sendMessage(message->chat->id, "You don't need any help, " + message->chat->firstName + "!");
    });

	struct request req;
	struct response res;


	m_bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
		if (message->text == "Generate Joke")
		{
			req.request_joke(res);
			if (res.getError())
				m_bot.getApi().sendMessage(message->chat->id, "The Bot is now in sadness and unable to produce a joke");
			else
			{
				if (res.getType() == "single")
				{
					m_bot.getApi().sendMessage(message->chat->id, res.getJoke());
					std::this_thread::sleep_for(std::chrono::seconds(2));
					m_bot.getApi().sendMessage(message->chat->id, random_smile());
				}
				else
				{
					m_bot.getApi().sendMessage(message->chat->id, res.getSetup());
					std::this_thread::sleep_for(std::chrono::seconds(4));
					m_bot.getApi().sendMessage(message->chat->id, res.getDelivery());
					std::this_thread::sleep_for(std::chrono::seconds(2));
					m_bot.getApi().sendMessage(message->chat->id, random_smile());
				}
			}
		}
	});

	while (true) {
        m_longPoll.start();
    }
}

