#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>
#include <nlohmann/json.hpp> // Requirement: Technical Implementation (JSON)

struct Article {
    std::string title;
    std::string url;
    std::string body;

    // Requirement: Data Storage (Organize extracted information)
    // This helper function makes exporting to JSON a one-liner later.
    nlohmann::json to_json() const {
        return nlohmann::json{
            {"title", title},
            {"url", url},
            {"body", body}
        };
    }
};

#endif