#ifndef BOT_HPP
#define BOT_HPP

#include <tgbot/tgbot.h>

#include <initializer_list>
#include <string>
#include <vector>

#include "request.hpp"
#include "response.hpp"

class tgbot {
   private:
    TgBot::Bot m_bot;
    TgBot::TgLongPoll m_longPoll;
    TgBot::ReplyKeyboardMarkup::Ptr keyboardOneCol;
    TgBot::ReplyKeyboardMarkup::Ptr keyboardSettings;
    TgBot::ReplyKeyboardMarkup::Ptr keyboardLanguage;
    TgBot::ReplyKeyboardMarkup::Ptr keyboardType;
    TgBot::ReplyKeyboardMarkup::Ptr keyboardBlacklist;
    TgBot::ReplyKeyboardMarkup::Ptr keyboardCategory;

    class request req;
    class response res;

    const std::vector<std::string> bot_cmds = {"start", "help"};
    const std::vector<std::string> bot_cmds_descriptions = {
        "The beginning", "Show reference information"};

   public:
    tgbot(const std::string &api_key);
    ~tgbot() = default;

    void run();

    inline void start();
    inline void help();
    inline void any_message();

    void log_info(const std::string &fisrt_name, const std::string &last_name,
                  const std::string &username, const std::string &text) const;

    inline void generate_joke(TgBot::Message::Ptr message);
};

#endif  // BOT_HPP