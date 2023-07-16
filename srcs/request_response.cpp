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

std::string request::argument_to_query() const
{
    std::string arg;

    for(auto s : m_joke_category)
        arg = arg + s + ',';
    arg.erase(--arg.end()); // erase last ','

    arg += "?format=json";

    if (m_safe_mode == "Yes")
        arg += "&safe-mode";
    if (m_language != "en - English")
        arg = arg + "&lang=" + m_language.substr(0, 2);
    if (m_type != "Any")
        arg = arg + "&type=" + m_type;

    if (m_blacklist.find("None") == m_blacklist.end())
    {
        arg += "&blacklistFlags=";
        for (auto s : m_blacklist)
            arg = arg + s + ',';
        arg.erase(--arg.end()); // erase last ','
    }
    std::cerr << arg << std::endl;
    return std::move(arg);
}

const std::string request::show_filters() const
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

const std::string request::setSafeMode()
{
    std::string res("Safe-mode is set to ");
    if (m_safe_mode == "No")
    {
        m_safe_mode = "Yes";
        res += "Yes";
    }
    else
    {
        m_safe_mode = "No";
        res += "No";
    }
    return std::move(res);
}

const std::string request::setDefault()
{
    m_language = "en - English";
    m_type = "Any";
    m_safe_mode = "No";
    m_joke_category.clear();
    m_joke_category.insert("Any");
    m_blacklist.clear();
    m_blacklist.insert("None");
    return std::move(std::string("All settings are set to default!"));
}

const std::string request::setLang(const char *s)
{
    m_language = s;
    std::string res("Language is set to ");
    res += s;
    return std::move(res);
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
		query += argument_to_query();
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
