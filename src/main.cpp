#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <gumbo.h>
#include "Article.h"

// Requirement: Modular Design (CURL Helper)
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Requirement: Data Extraction (Title Finder)
std::string find_title(GumboNode* node) {
    if (node->type != GUMBO_NODE_ELEMENT) return "";
    if (node->v.element.tag == GUMBO_TAG_TITLE && node->v.element.children.length != 0) {
        return static_cast<GumboNode*>(node->v.element.children.data[0])->v.text.text;
    }
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        std::string title = find_title(static_cast<GumboNode*>(children->data[i]));
        if (!title.empty()) return title;
    }
    return "";
}

// Requirement: Data Extraction (Paragraph Extractor)
void extract_paragraphs(GumboNode* node, std::string& out_text) {
    if (node->type == GUMBO_NODE_TEXT) {
        out_text += node->v.text.text;
        out_text += " ";
        return;
    }
    if (node->type == GUMBO_NODE_ELEMENT && 
        node->v.element.tag != GUMBO_TAG_SCRIPT && 
        node->v.element.tag != GUMBO_TAG_STYLE) {
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            extract_paragraphs(static_cast<GumboNode*>(children->data[i]), out_text);
        }
    }
}

int main() {
    // 1. Requirement: File I/O (Read URLs from input file)
    std::vector<std::string> urls;
    std::ifstream inputFile("targets.txt");

    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            if (!line.empty()) urls.push_back(line);
        }
        inputFile.close();
    } else {
        std::cerr << "Note: targets.txt not found, using default." << std::endl;
        urls.push_back("https://www.google.com");
    }

    std::vector<Article> library;

    // 2. Requirement: Control Flow (Processing loop)
    for (const std::string& current_url : urls) {
        std::cout << "Scraping: " << current_url << "..." << std::endl;
        
        // --- SCOPE START ---
        std::string htmlBuffer; // Variable is born here

        CURL* curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, current_url.c_str());
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlBuffer); // Used here
            
            CURLcode res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                std::cerr << "CURL Error: " << curl_easy_strerror(res) << std::endl;
            }
            
            curl_easy_cleanup(curl);
        }

        // Parse the buffer while it's still in scope
        GumboOutput* output = gumbo_parse(htmlBuffer.c_str()); // Used here
        
        Article art;
        art.url = current_url;
        art.title = find_title(output->root);
        extract_paragraphs(output->root, art.body);
        
        library.push_back(art);
        gumbo_destroy_output(&kGumboDefaultOptions, output);
        // --- SCOPE END: htmlBuffer is destroyed here and recreated for next loop ---
    }

    // 3. Requirement: File I/O (JSON Export)
    nlohmann::json final_output = nlohmann::json::array();
    for (const auto& a : library) {
        final_output.push_back(a.to_json());
    }

    std::ofstream file("articles.json");
    if (file.is_open()) {
        file << final_output.dump(4);
        file.close();
        std::cout << "\nSuccess! Check articles.json for results." << std::endl;
    }

    return 0;
}