#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <curl/curl.h>

#include <tgbot/tgbot.h>

#include "bot.hpp"
#include "request_response.hpp"

int main(void)
{
	// std::ifstream api_key_file;

	// api_key_file.open("api_key");
	// if (!api_key_file.is_open())
	// 	return 1;
	// std::string api_key;
	// api_key_file >> api_key;
	// api_key_file.close();

	curl_global_init(CURL_GLOBAL_DEFAULT);
	tgbot bot(std::getenv("API_KEY")); // TODO: replace with getenv("TOKEN");

	try
	{
		bot.run();
	}
	catch (TgBot::TgException& e)
	{
		std::cerr << "Error TgBot: " << e.what() << '\n';
	}
	catch (...)
	{
		std::cerr << "Some critical error\n";
	}

	curl_global_cleanup();

	return 0;
}
