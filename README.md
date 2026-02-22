# Article Web Scraper CLI

A high-performance C++ tool designed to automate the extraction of titles and body text from web articles. This project demonstrates proficiency in systems integration by combining networking via libcurl, HTML5 parsing with Gumbo, and data serialization with nlohmann-json.

## Instructions for Build and Use

### Software Demo

Steps to build and/or run the software:

1. Install Dependencies: Use vcpkg to install curl, gumbo, and nlohmann-json.

2. Configure CMake: Run the CMake: Configure command in VS Code to link the installed libraries and generate the build system.

3. Build: Click the Build button in the VS Code status bar to compile the WebScraper.exe executable.

Instructions for using the software:

1. Input URLs: Add your target article links to a file named `targets.txt` in the project folder (one URL per line).

2. Run: Execute the program. The terminal will log the scraping progress for each site.

3. Review Output: Open `articles.json` to see the extracted data organized by title, URL, and body content.

## Development Environment

To recreate the development environment, you need the following software and/or libraries with the specified versions:

* Compiler: MSVC (Visual Studio 2022) or GCC supporting C++17.

* Build Tooling: CMake 3.10 or higher.

* Libraries: libcurl, Gumbo-parser, and nlohmann-json (managed via vcpkg).

## Useful Websites to Learn More

I found these websites useful in developing this software:

* libcurl Documentation

* Gumbo Parser GitHub

* C++ Reference (Iterators and Vectors)

## Future Work

The following items I plan to fix, improve, and/or add to this project in the future:

* [ ] Recursive Link Discovery: Add functionality to find and follow links within an article automatically.

* [ ] Error Logging: Implement a dedicated log file to track failed HTTP requests or parsing timeouts.

* [ ] User Interface: Create a simple GUI using Qt or ImGui to manage the URL list and view results.
```
