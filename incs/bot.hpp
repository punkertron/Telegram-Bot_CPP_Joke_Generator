#ifndef BOT_HPP
#define BOT_HPP

#include <vector>
#include <string>
#include <initializer_list>

#include <tgbot/tgbot.h>

class tgbot
{
private:
	TgBot::Bot m_bot;
	TgBot::TgLongPoll m_longPoll;
	const std::vector<std::string> bot_cmds = {"start", "help"};
	const std::vector<std::string> bot_cmds_descriptions = {"The beginning", "Show reference information"};

public:
	tgbot(const std::string& api_key);
	~tgbot() = default;

	void run();
};

#endif // BOT_HPP