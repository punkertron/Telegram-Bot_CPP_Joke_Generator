#ifndef REQUEST_RESPONSE_HPP
#define REQUEST_RESPONSE_HPP

#include <unordered_set>
#include <string>
#include <tgbot/tgbot.h>


class request
{
private:
	//TgBot::User::Ptr user;
	std::unordered_set<std::string> joke_category;
	std::string argument_to_query(const request& request);

public:
	request();
	~request() = default;

	void request_joke(struct response& resp);
};

struct response
{
	bool is_error;
	std::string type;

	std::string joke; // type == single

    std::string setup; // type == twopart
	std::string delivery; // type == twopart

	response();

	void update();

	const std::string &getType() const
	{
		return type;
	}

	const std::string &getJoke() const
	{
		return joke;
	}

	const std::string &getSetup() const
	{
		return setup;
	}

	const std::string &getDelivery() const
	{
		return delivery;
	}

	const bool& getError() const
	{
		return is_error;
	}
	
};

#endif // REQUEST_RESPONSE_HPP