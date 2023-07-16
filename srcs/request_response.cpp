#include <iostream>
#include <utility>

#include <curl/curl.h>

#include "json.hpp"

#include "request_response.hpp"

request::request():
    m_language("en - English"), m_type("Any"), m_safe_mode("No")
{
    m_joke_category.insert("Any");
    m_blacklist.insert("None");
}

std::string request::argument_to_query(const request& request)
{
    std::string arg;

    for(auto i : request.m_joke_category)
    {
        arg += i;
    }
    return std::move(arg);
}

std::string request::show_filters() const
{
    std::string res;

    res = "Categories: ";
    for (auto s : m_joke_category)
        res += s;
    res += "\nLanguage: ";
    res += m_language;
    res += "\nBlacklist: ";
    for (auto s : m_blacklist)
        res += s;
    res += "\nType (single or twopart): ";
    res += m_type;
    res += "\nSafe-mode: ";
    res += m_safe_mode;
    return (std::move(res));
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
            resp.setError(true);
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
            resp.setError(true);
            return ;
        }
        curl_easy_cleanup(curl);
        fclose(fp);
        resp.update();
    }

    return ;
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
