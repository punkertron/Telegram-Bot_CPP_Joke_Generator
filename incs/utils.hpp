#ifndef UTILS_HPP
#define UTILS_HPP

#include "bot.hpp"

void createOneColumnKeyboard(const std::vector<std::string>& buttonStrings, TgBot::ReplyKeyboardMarkup::Ptr& kb);

#endif // UTILS_HPP