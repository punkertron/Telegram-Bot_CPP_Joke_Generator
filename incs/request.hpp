#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <tgbot/tgbot.h>

#include <string>
#include <unordered_set>

class request {
   private:
    // TgBot::User::Ptr user;
    std::unordered_set<std::string> m_joke_category;
    std::string m_language;
    std::unordered_set<std::string> m_blacklist;
    std::string m_type;
    std::string m_safe_mode;

    std::string argument_to_query() const;

   public:
    request();
    ~request() = default;

    void request_joke(struct response& resp);

    const std::string show_filters() const;

    const std::string setSafeMode();
    const std::string setDefault();
    const std::string setLang(const char* s);
    const std::string setBlackList(const std::string s);
    const std::string setCategory(const std::string s);
    const std::string setType(const std::string s);
};

#endif