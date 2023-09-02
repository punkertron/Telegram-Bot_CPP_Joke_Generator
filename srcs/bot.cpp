#include "bot.hpp"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <thread>

#include "utils.hpp"

// https://apps.timwhitlock.info/emoji/tables/unicode

static const std::vector<std::string> Emodjis = {
    "\xF0\x9F\x98\x81", "\xF0\x9F\x98\x82", "\xF0\x9F\x98\x83",
    "\xF0\x9F\x98\x84", "\xF0\x9F\x98\x85", "\xF0\x9F\x98\x86",
    "\xF0\x9F\x98\x9C"};

static const std::vector<std::string> Stickers = {
    "CAACAgIAAxkBAAEJtLBks8ksyTXO3Ix0TWXqTm7rc5Z5IgAChgMAAkcVaAl9En4PCvCBli8E",
    "CAACAgIAAxkBAAEJtLJks8kwT28BiSPKQ-DW9oH5RBHvowACRgMAAm2wQgMPoPbV7P8zDi8E",
    "CAACAgQAAxkBAAEJtLRks8kyLxeO7SI0wruuxTxT7d8R0AACCQAD133WJREiMki6J6qGLwQ",
    "CAACAgIAAxkBAAEJtLZks8k1cImcOnQssaM4ozk4OWXVCAACXQADKuBSG0Y9HMuI3J2ELwQ",
    "CAACAgQAAxkBAAEJtLhks8k46Qy3wvEGBWuvKngULGbgdwACMgEAAqghIQZ4LmzPtHHGZC8E",
    "CAACAgIAAxkBAAEJtLpks8k7OIPir4W5klqy5jgRUmprpwACRgADUomRI_j-5eQK1QodLwQ",
    "CAACAgIAAxkBAAEJtLxks8k9VBC4vFHIM80nCrOapuHZUAACdAMAAvoLtgjV5oYGGtDaUC8E",
    "CAACAgIAAxkBAAEJtL5ks8lAuP8ySxKIz3KqU5Ha3dVccgACvg8AAvbk0EmejJ6Vjfwi8i8E",
    "CAACAgIAAxkBAAEJtMBks8lDbMxp8k9wTc-n4ieBwyFFzwACzxkAAiUBwEh5PNDrZsRPSS8E"};

// KeyboradOneCol
static const std::string GenJokeKey = "Generate Joke \xF0\x9F\x9A\x80";
static const std::string SetUpKey = "Set up filters \xF0\x9F\x94\xA7";
static const std::string ShowFiltersKey = "Show filters \xF0\x9F\x94\x8E";

// KeyboardSettings
static const std::string SafeModeKey = "Safe-mode \xF0\x9F\x91\xB6";
static const std::string CategKey = "Categories \xF0\x9F\x94\xAE";
static const std::string LangKey = "Language \xF0\x9F\x91\x80";
static const std::string BlackKey = "Blacklist \xF0\x9F\x9A\xAB";
static const std::string TypeKey = "Type \xF0\x9F\x98\xAF";
static const std::string DefaultKey = "Make default \xE2\x99\xBB";
static const std::string BackSetKey = "\xF0\x9F\x94\x99";

// KeyboardLanguage
static const std::string EnKey = "English \xF0\x9F\x87\xAC\xF0\x9F\x87\xA7";
static const std::string DeKey = "German \xF0\x9F\x87\xA9\xF0\x9F\x87\xAA";
static const std::string CsKey = "Czech 🇨🇿";
static const std::string EsKey = "Spanish \xF0\x9F\x87\xAA\xF0\x9F\x87\xB8";
static const std::string FrKey = "French \xF0\x9F\x87\xAB\xF0\x9F\x87\xB7";
static const std::string PtKey = "Portuguese 🇵🇹";
static const std::string BackLanKey = "\xF0\x9F\x94\x9A";

// KeyboardType
static const std::string tAnyKey = "Any \xE2\x9C\xA8 	";
static const std::string tSingleKey = "single \x31\xE2\x83\xA3";
static const std::string tTwoKey = "twopart \x32\xE2\x83\xA3";
static const std::string tBackKey = "Back \xE2\x86\xA9";

// KeyboardBlacklist
static const std::string bNoneKey = "None \xF0\x9F\x94\x9E";
static const std::string bNSFWKey = "nsfw";
static const std::string bReligKey = "religious";
static const std::string bPolitKey = "political";
static const std::string bRacistKey = "racist";
static const std::string bSexistKey = "sexist";
static const std::string bExplicitKey = "explicit";
static const std::string bBackKey = "Back \xF0\x9F\x91\x88";

// KeyboardCategory
static const std::string cAnyKey = "Any";
static const std::string cProgKey = "Programming \xF0\x9F\x92\xBB";
static const std::string cMiscKey = "Misc \xF0\x9F\x98\x9D";
static const std::string cDarkKey = "Dark \xF0\x9F\x98\x88";
static const std::string cPunKey = "Pun \xF0\x9F\x98\xB2";
static const std::string cSpookyKey = "Spooky \xF0\x9F\x91\xBB";
static const std::string cChtmsKey = "Christmas \xF0\x9F\x8C\xB2";
static const std::string cBackKey = "Back \xF0\x9F\x86\x99";

tgbot::tgbot(const std::string &api_key)
    : m_bot(api_key),
      m_longPoll(m_bot),
      keyboardOneCol(new TgBot::ReplyKeyboardMarkup),
      keyboardSettings(new TgBot::ReplyKeyboardMarkup),
      keyboardLanguage(new TgBot::ReplyKeyboardMarkup),
      keyboardType(new TgBot::ReplyKeyboardMarkup),
      keyboardBlacklist(new TgBot::ReplyKeyboardMarkup),
      keyboardCategory(new TgBot::ReplyKeyboardMarkup) {
    std::vector<TgBot::BotCommand::Ptr> commands;

    for (typename std::vector<std::string>::size_type i = 0;
         i < bot_cmds.size(); ++i) {
        TgBot::BotCommand::Ptr cmd(new TgBot::BotCommand);
        cmd->command = bot_cmds[i];
        cmd->description = bot_cmds_descriptions[i];
        commands.push_back(cmd);
    }

    m_bot.getApi().setMyCommands(commands);

    createOneColumnKeyboard({GenJokeKey, SetUpKey, ShowFiltersKey},
                            keyboardOneCol);

    createKeyboard({{SafeModeKey},
                    {CategKey, LangKey},
                    {BlackKey, TypeKey},
                    {DefaultKey},
                    {BackSetKey}},
                   keyboardSettings);

    createKeyboard(
        {{EnKey, DeKey}, {CsKey, EsKey}, {FrKey, PtKey}, {BackLanKey}},
        keyboardLanguage);

    createKeyboard({{tAnyKey}, {tSingleKey, tTwoKey}, {tBackKey}},
                   keyboardType);

    createKeyboard({{bNoneKey},
                    {bNSFWKey, bReligKey},
                    {bPolitKey, bRacistKey},
                    {bSexistKey, bExplicitKey},
                    {bBackKey}},
                   keyboardBlacklist);

    createKeyboard({{cAnyKey},
                    {cProgKey, cMiscKey},
                    {cDarkKey, cPunKey},
                    {cSpookyKey, cChtmsKey},
                    {cBackKey}},
                   keyboardCategory);
}

static void random_smile(const TgBot::Api &api, const int64_t chat_id) {
    std::srand(std::time(NULL));

    int r1 = std::rand(), r2 = std::rand();
    if (r1 % 3 == 1)
        api.sendMessage(chat_id, Emodjis[r2 % Emodjis.size()]);
    else if (r1 % 3 == 2)
        api.sendSticker(chat_id, Stickers[r2 % Stickers.size()]);
}

void tgbot::log_info(const int64_t userId, const std::string &fisrt_name,
                     const std::string &last_name, const std::string &username,
                     const std::string &text) const {
    // get current time
    auto now = std::chrono::system_clock::now();

    // convert to std::time_t
    auto now_c = std::chrono::system_clock::to_time_t(now);

    // convert to local time
    std::tm *local = std::localtime(&now_c);

    std::cerr << std::ctime(&now_c) << "Username = " << username
              << ". First_name = " << fisrt_name
              << ". Last_name = " << last_name << ". User_Id = " << userId
              << ". Text = " << text << std::endl;
}

void tgbot::start() {
    m_bot.getEvents().onCommand("start", [&](TgBot::Message::Ptr message) {
        // log_info(message->from->id, message->chat->firstName,
        //          message->chat->lastName, message->chat->username,
        //          message->text);
        m_bot.getApi().sendMessage(
            message->chat->id, "Hello, " + message->chat->firstName + "!\n" +
                                   "This bot will make you laugh!");
        m_bot.getApi().sendSticker(message->chat->id,
                                   "CAACAgIAAxkBAAEJs39ksteuz27hq4bNyGwfrJ3ppVK"
                                   "HpAAC8gADVp29ChCdi3ZTetJkLwQ",
                                   0, keyboardOneCol);
    });
}

void tgbot::help() {
    m_bot.getEvents().onCommand("help", [&](TgBot::Message::Ptr message) {
        // log_info(message->from->id, message->chat->firstName,
        //          message->chat->lastName, message->chat->username,
        //          message->text);
        m_bot.getApi().sendMessage(
            message->chat->id,
            "You don't need any help, " + message->chat->firstName + "!");
    });
}

void tgbot::generate_joke(int64_t userId, TgBot::Message::Ptr message) {
    req[userId].request_joke(res);
    if (res.getError())
        m_bot.getApi().sendMessage(
            message->chat->id,
            "The Bot is now in sadness and unable to produce a joke");
    else {
        if (res.getType() == "single") {
            m_bot.getApi().sendMessage(message->chat->id, res.getJoke(), false,
                                       0, keyboardOneCol);
            std::this_thread::sleep_for(std::chrono::seconds(4));
            random_smile(m_bot.getApi(), message->chat->id);
        } else {
            m_bot.getApi().sendMessage(message->chat->id, res.getSetup(), false,
                                       0, keyboardOneCol);
            std::this_thread::sleep_for(std::chrono::seconds(4));
            m_bot.getApi().sendMessage(message->chat->id, res.getDelivery(),
                                       false, 0, keyboardOneCol);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            random_smile(m_bot.getApi(), message->chat->id);
        }
    };
}

void tgbot::any_message() {
    m_bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
        log_info(message->from->id, message->chat->firstName,
                 message->chat->lastName, message->chat->username,
                 message->text);
        int64_t userId = message->from->id;
        if (req.find(userId) == req.end())
            req.insert(std::pair<int64_t, class request>(userId, {}));
        if (message->text == GenJokeKey)
            generate_joke(userId, message);
        else if (message->text == SetUpKey)
            m_bot.getApi().sendMessage(message->chat->id, "Settings...", false,
                                       0, keyboardSettings);
        else if (message->text == ShowFiltersKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].show_filters(), false, 0,
                                       keyboardOneCol);
        else if (message->text == SafeModeKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setSafeMode(), false, 0,
                                       keyboardSettings);
        else if (message->text == DefaultKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setDefault(), false, 0,
                                       keyboardOneCol);
        else if (message->text == BackSetKey)
            m_bot.getApi().sendMessage(message->chat->id, "Coming back...",
                                       false, 0, keyboardOneCol);
        else if (message->text == LangKey)
            m_bot.getApi().sendMessage(message->chat->id, "Chose your language",
                                       false, 0, keyboardLanguage);
        else if (message->text == BlackKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       "Select flags to blacklist", false, 0,
                                       keyboardBlacklist);
        else if (message->text == CategKey)
            m_bot.getApi().sendMessage(message->chat->id, "Select category",
                                       false, 0, keyboardCategory);
        else if (message->text == TypeKey)
            m_bot.getApi().sendMessage(message->chat->id, "Select type", false,
                                       0, keyboardType);

        else if (message->text == EnKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setLang("en - English"),
                                       false, 0, keyboardSettings);
        else if (message->text == DeKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setLang("de - German"),
                                       false, 0, keyboardSettings);
        else if (message->text == CsKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setLang("cs - Czech"), false,
                                       0, keyboardSettings);
        else if (message->text == EsKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setLang("es - Spanish"),
                                       false, 0, keyboardSettings);
        else if (message->text == FrKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setLang("fr - French"),
                                       false, 0, keyboardSettings);
        else if (message->text == PtKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setLang("pt - Portuguiese"),
                                       false, 0, keyboardSettings);
        else if (message->text == BackLanKey)
            m_bot.getApi().sendMessage(message->chat->id, "Back to Settings",
                                       false, 0, keyboardSettings);

        else if (message->text == bNoneKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setBlackList(bNoneKey),
                                       false, 0, keyboardSettings);
        else if (message->text == bNSFWKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setBlackList(bNSFWKey),
                                       false, 0, keyboardSettings);
        else if (message->text == bReligKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setBlackList(bReligKey),
                                       false, 0, keyboardSettings);
        else if (message->text == bPolitKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setBlackList(bPolitKey),
                                       false, 0, keyboardSettings);
        else if (message->text == bRacistKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setBlackList(bRacistKey),
                                       false, 0, keyboardSettings);
        else if (message->text == bSexistKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setBlackList(bSexistKey),
                                       false, 0, keyboardSettings);
        else if (message->text == bExplicitKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setBlackList(bExplicitKey),
                                       false, 0, keyboardSettings);
        else if (message->text == bBackKey)
            m_bot.getApi().sendMessage(message->chat->id, "Back to Settings",
                                       false, 0, keyboardSettings);

        else if (message->text == cAnyKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setCategory(cAnyKey), false,
                                       0, keyboardSettings);
        else if (message->text == cProgKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setCategory(cProgKey), false,
                                       0, keyboardSettings);
        else if (message->text == cMiscKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setCategory(cMiscKey), false,
                                       0, keyboardSettings);
        else if (message->text == cDarkKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setCategory(cDarkKey), false,
                                       0, keyboardSettings);
        else if (message->text == cPunKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setCategory(cPunKey), false,
                                       0, keyboardSettings);
        else if (message->text == cSpookyKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setCategory(cSpookyKey),
                                       false, 0, keyboardSettings);
        else if (message->text == cChtmsKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setCategory(cChtmsKey),
                                       false, 0, keyboardSettings);
        else if (message->text == cBackKey)
            m_bot.getApi().sendMessage(message->chat->id, "Back to Settings",
                                       false, 0, keyboardSettings);

        else if (message->text == tAnyKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setType(tAnyKey), false, 0,
                                       keyboardSettings);
        else if (message->text == tSingleKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setType(tSingleKey), false,
                                       0, keyboardSettings);
        else if (message->text == tTwoKey)
            m_bot.getApi().sendMessage(message->chat->id,
                                       req[userId].setType(tTwoKey), false, 0,
                                       keyboardSettings);
        else if (message->text == tBackKey)
            m_bot.getApi().sendMessage(message->chat->id, "Back to Settings",
                                       false, 0, keyboardSettings);

        else if (message->text != "/start" && message->text != "/help")
            m_bot.getApi().sendMessage(
                message->chat->id,
                message->chat->firstName +
                    ", you have entered an invalid command.\nPlease, try "
                    "again. I might get angry and stop giving you jokes!",
                false, 0, keyboardOneCol);
    });
}

void tgbot::run() {
    start();
    help();
    any_message();

    while (true) {
        m_longPoll.start();
    }
}
