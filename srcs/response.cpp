#include "response.hpp"

#include "json.hpp"

void response::update() {
    std::ifstream f("response.json");
    if (!f.is_open()) {
        is_error = true;
        return;
    }
    nlohmann::json data = nlohmann::json::parse(f);
    if (data["error"]) {
        is_error = true;
        f.close();
        return;
    }
    type = data["type"];
    if (type == "single")
        joke = data["joke"];
    else {
        setup = data["setup"];
        delivery = data["delivery"];
    }
}

const std::string &response::getType() const { return type; }

const std::string &response::getJoke() const { return joke; }

const std::string &response::getSetup() const { return setup; }

const std::string &response::getDelivery() const { return delivery; }

const bool &response::getError() const { return is_error; }

void response::setError(bool t) {
    is_error = t;
    return;
}
