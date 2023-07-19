#include <curl/curl.h>
#include <tgbot/tgbot.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "bot.hpp"

int main(void) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    tgbot bot(std::getenv("API_KEY"));

    try {
        bot.run();
    } catch (TgBot::TgException& e) {
        std::cerr << "Error TgBot: " << e.what() << '\n';
    } catch (...) {
        std::cerr << "Some critical error\n";
    }

    curl_global_cleanup();

    return 0;
}
