#ifndef BOT_HPP
#define BOT_HPP

#include <vector>
#include <string>
#include <initializer_list>

#include <tgbot/tgbot.h>

#include <request_response.hpp>

class tgbot
{
private:
	TgBot::Bot m_bot;
	TgBot::TgLongPoll m_longPoll;
	TgBot::ReplyKeyboardMarkup::Ptr keyboardOneCol;
	TgBot::ReplyKeyboardMarkup::Ptr keyboardSettings;
	TgBot::ReplyKeyboardMarkup::Ptr keyboardLanguage;

	class request req;
	class response res;

	const std::vector<std::string> bot_cmds = {"start", "help"};
	const std::vector<std::string> bot_cmds_descriptions = {"The beginning", "Show reference information"};

public:
	tgbot(const std::string& api_key);
	~tgbot() = default;

	void run();

	inline void start();
	inline void help();
	inline void any_message();

	inline void generate_joke(TgBot::Message::Ptr message);
};

#endif // BOT_HPP