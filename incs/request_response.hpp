#ifndef REQUEST_RESPONSE_HPP
#define REQUEST_RESPONSE_HPP

#include <unordered_set>
#include <string>
#include <tgbot/tgbot.h>


class request
{
private:
	//TgBot::User::Ptr user;
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

};

class response
{
private:
	bool is_error = false;
	std::string type;

	std::string joke; // type == single

    std::string setup; // type == twopart
	std::string delivery; // type == twopart

public:
	response() = default;
	~response() = default;

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

	void setError(bool t)
	{
		is_error = t;
		return ;
	}
	
};

#endif // REQUEST_RESPONSE_HPP