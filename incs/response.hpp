#ifndef REQUEST_RESPONSE_HPP
#define REQUEST_RESPONSE_HPP

#include <tgbot/tgbot.h>

#include <string>

class response {
   private:
    bool is_error = false;
    std::string type;

    std::string joke;  // type == single

    std::string setup;     // type == twopart
    std::string delivery;  // type == twopart

   public:
    response() = default;
    ~response() = default;

    void update();

    const std::string &getType() const;
    const std::string &getJoke() const;
    const std::string &getSetup() const;
    const std::string &getDelivery() const;
    const bool &getError() const;

    void setError(bool t);
};

#endif  // REQUEST_RESPONSE_HPP