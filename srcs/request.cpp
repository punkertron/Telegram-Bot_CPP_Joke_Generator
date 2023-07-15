#include <iostream>
#include <utility>

#include <curl/curl.h>

#include "joke_generator.hpp"

static std::string argument_to_query(const joke& joke)
{
    std::string arg;

    for(auto i : joke.user_category)
    {
        arg += i;
    }
    return std::move(arg);
}

void request_hh(struct response& resp, const joke& joke)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;

    curl = curl_easy_init();
    if (curl)
    {
        fp = fopen("response.json", "wb");
        if (!fp)
        {
            std::cerr << "Can't open response.json" << std::endl;
            resp.is_error = true;
            return ;
        }
		std::string query("https://v2.jokeapi.dev/joke/");
		query += argument_to_query(joke);
        curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            resp.is_error = true;
            return ;
        }

        curl_easy_cleanup(curl);
        fclose(fp);
    }

    return ;
}

