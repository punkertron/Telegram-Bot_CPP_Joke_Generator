#include <chrono>
#include <thread>

#include <ctime>
#include <cstdlib>

#include "bot.hpp"
#include "utils.hpp"
#include "request_response.hpp"


// https://apps.timwhitlock.info/emoji/tables/unicode

static const std::vector<std::string> Emodjis = {
	"\xF0\x9F\x98\x81",
	"\xF0\x9F\x98\x82",
	"\xF0\x9F\x98\x83",
	"\xF0\x9F\x98\x84",
	"\xF0\x9F\x98\x85",
	"\xF0\x9F\x98\x86",
	"\xF0\x9F\x98\x9C"
};

static const std::vector<std::string> Stickers = {
	"CAACAgIAAxkBAAEJtLBks8ksyTXO3Ix0TWXqTm7rc5Z5IgAChgMAAkcVaAl9En4PCvCBli8E",
	"CAACAgIAAxkBAAEJtLJks8kwT28BiSPKQ-DW9oH5RBHvowACRgMAAm2wQgMPoPbV7P8zDi8E",
	"CAACAgQAAxkBAAEJtLRks8kyLxeO7SI0wruuxTxT7d8R0AACCQAD133WJREiMki6J6qGLwQ",
	"CAACAgIAAxkBAAEJtLZks8k1cImcOnQssaM4ozk4OWXVCAACXQADKuBSG0Y9HMuI3J2ELwQ",
	"CAACAgQAAxkBAAEJtLhks8k46Qy3wvEGBWuvKngULGbgdwACMgEAAqghIQZ4LmzPtHHGZC8E",
	"CAACAgIAAxkBAAEJtLpks8k7OIPir4W5klqy5jgRUmprpwACRgADUomRI_j-5eQK1QodLwQ",
	"CAACAgIAAxkBAAEJtLxks8k9VBC4vFHIM80nCrOapuHZUAACdAMAAvoLtgjV5oYGGtDaUC8E",
	"CAACAgIAAxkBAAEJtL5ks8lAuP8ySxKIz3KqU5Ha3dVccgACvg8AAvbk0EmejJ6Vjfwi8i8E",
	"CAACAgIAAxkBAAEJtMBks8lDbMxp8k9wTc-n4ieBwyFFzwACzxkAAiUBwEh5PNDrZsRPSS8E"
};



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

static void random_smile(const TgBot::Api& api, const int64_t chat_id)
{
	srand(time(NULL));

	int r = rand();
	if (r % 3 == 1)
		api.sendMessage(chat_id, Emodjis[r % Emodjis.size()]);
	else if (r % 3 == 2)
		api.sendSticker(chat_id, Stickers[r % Stickers.size()]);
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
					std::this_thread::sleep_for(std::chrono::seconds(4));
					random_smile(m_bot.getApi(), message->chat->id);
				}
				else
				{
					m_bot.getApi().sendMessage(message->chat->id, res.getSetup());
					std::this_thread::sleep_for(std::chrono::seconds(4));
					m_bot.getApi().sendMessage(message->chat->id, res.getDelivery());
					std::this_thread::sleep_for(std::chrono::seconds(2));
					random_smile(m_bot.getApi(), message->chat->id);
				}
			}
		}
	});

	while (true) {
        m_longPoll.start();
    }
}

