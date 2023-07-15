#include <iostream>
#include <utility>

#include <curl/curl.h>

#include "json.hpp"

#include "request_response.hpp"

request::request()
{
    joke_category.insert("Any");
}

std::string request::argument_to_query(const request& request)
{
    std::string arg;

    for(auto i : request.joke_category)
    {
        arg += i;
    }
    return std::move(arg);
}

void request::request_joke(struct response& resp)
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
		query += argument_to_query(*this);
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
        resp.update();
    }

    return ;
}

response::response():
    is_error(false)
{
    ;
}

void response::update()
{
    std::ifstream f("response.json");
    if (!f.is_open())
    {
        is_error = true;
        return ;
    }
    nlohmann::json data = nlohmann::json::parse(f);
    if (data["error"])
    {
        is_error = true;
        f.close();
        return ;
    }
    type = data["type"];
    if (type == "single")
        joke = data["joke"];
    else
    {
        setup = data["setup"];
        delivery = data["delivery"];
    }
}
