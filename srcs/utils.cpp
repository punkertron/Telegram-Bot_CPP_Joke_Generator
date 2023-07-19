#include "utils.hpp"

#include <utility>

void createOneColumnKeyboard(const std::vector<std::string>& buttonStrings,
                             TgBot::ReplyKeyboardMarkup::Ptr& kb) {
    for (typename std::vector<std::string>::size_type i = 0;
         i < buttonStrings.size(); ++i) {
        TgBot::KeyboardButton::Ptr button(new TgBot::KeyboardButton);
        button->text = buttonStrings[i];
        button->requestContact = false;
        button->requestLocation = false;
        kb->keyboard.push_back(
            std::move(std::vector<TgBot::KeyboardButton::Ptr>{button}));
    }
}

void createKeyboard(const std::vector<std::vector<std::string> >& buttonLayout,
                    TgBot::ReplyKeyboardMarkup::Ptr& kb) {
    for (typename std::vector<std::string>::size_type i = 0;
         i < buttonLayout.size(); ++i) {
        std::vector<TgBot::KeyboardButton::Ptr> row;
        for (typename std::vector<std::string>::size_type j = 0;
             j < buttonLayout[i].size(); ++j) {
            TgBot::KeyboardButton::Ptr button(new TgBot::KeyboardButton);
            button->text = buttonLayout[i][j];
            button->requestContact = false;
            button->requestLocation = false;
            row.push_back(button);
        }
        kb->keyboard.push_back(row);
    }
    kb->resizeKeyboard = true;
}
