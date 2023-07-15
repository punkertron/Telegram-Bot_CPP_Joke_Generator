#ifndef JOKE_GENERATOR_HPP
#define JOKE_GENERATOR_HPP

#include <unordered_set>
#include <string>


struct joke
{
	std::unordered_set<std::string> user_category;
};

enum class type {single, twopart};

struct response
{
	bool is_error;
	type t;
    std::string setup;
	std::string delivery;
};

void request_hh(struct response& resp, const joke& joke);

#endif // JOKE_GENERATOR_HPP