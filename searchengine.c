#include <stdio.h>
#include <string.h>
#include <emscripten.h>

#define MAX_SE_OUTPUT 16384
static char output[MAX_SE_OUTPUT];

EMSCRIPTEN_KEEPALIVE
const char* search_engines(const char* first, const char* middle, const char* last, const char* alias, const char* email, const char* location) {
    memset(output, 0, MAX_SE_OUTPUT);

    if (!first || !last || strlen(first) == 0 || strlen(last) == 0) {
        snprintf(output, MAX_SE_OUTPUT, "❌ ERROR: First and last name are required for search engine lookup.\n");
        return output;
    }

    char full[256] = {0};
    if (middle && strlen(middle) > 0) {
        snprintf(full, sizeof(full), "%s %s %s", first, middle, last);
    } else {
        snprintf(full, sizeof(full), "%s %s", first, last);
    }

    snprintf(output, MAX_SE_OUTPUT,
        "=== Search Engine Queries ===\n"
        "Search Target: %s\n\n"

        "🟢 General Searches:\n"
        "[+] Google: https://www.google.com/search?q=\"%s\"\n"
        "[+] Reddit: https://www.reddit.com/search/?q=\"%s\"\n"
        "[+] Bing:   https://www.bing.com/search?q=\"%s\"\n"
        "[+] DuckDuckGo: https://duckduckgo.com/?q=\"%s\"\n",
        full, full, full, full, full
    );

    // Alias Dork Set
    if (alias && strlen(alias) > 0) {
        char line[512];
        snprintf(line, sizeof(line),
            "\n🟣 Alias Searches:\n"
            "[+] Alias in Google: https://www.google.com/search?q=\"%s\"\n"
            "[+] Reddit mentions: https://www.google.com/search?q=\"%s\"+site:reddit.com\n"
            "[+] GitHub profiles: https://www.google.com/search?q=\"%s\"+site:github.com\n"
            "[+] Pastebin leaks:  https://www.google.com/search?q=\"%s\"+site:pastebin.com\n",
            alias, alias, alias, alias
        );
        strncat(output, line, MAX_SE_OUTPUT - strlen(output) - 1);
    }

    // Email Dork Set
    if (email && strlen(email) > 0) {
        char line[512];
        snprintf(line, sizeof(line),
            "\n🟠 Email Leak Checks:\n"
            "[+] Search for email: https://www.google.com/search?q=\"%s\"\n"
            "[+] Pastebin mentions: https://www.google.com/search?q=\"%s\"+site:pastebin.com\n"
            "[+] Leaked text files: https://www.google.com/search?q=\"%s\"+filetype:txt\n"
            "[+] Exposed logs: https://www.google.com/search?q=\"%s\"+ext:log\n",
            email, email, email, email
        );
        strncat(output, line, MAX_SE_OUTPUT - strlen(output) - 1);
    }

    // Name-Based Profile Dorks
    {
        char line[1024];
        snprintf(line, sizeof(line),
            "\n🔵 Name-Based Profile Searches:\n"
            "[+] LinkedIn: https://www.google.com/search?q=\"%s\"+site:linkedin.com\n"
            "[+] Twitter:  https://www.google.com/search?q=\"%s\"+site:twitter.com\n"
            "[+] Facebook: https://www.google.com/search?q=\"%s\"+site:facebook.com\n"
            "[+] Medium:   https://www.google.com/search?q=\"%s\"+site:medium.com\n"
            "[+] StackOverflow: https://www.google.com/search?q=\"%s\"+site:stackoverflow.com\n",
            full, full, full, full, full
        );
        strncat(output, line, MAX_SE_OUTPUT - strlen(output) - 1);
    }

    // Optional location-based boost
    if (location && strlen(location) > 0) {
        char line[512];
        snprintf(line, sizeof(line),
            "\n📍 Localized Search:\n"
            "[+] Name + Location: https://www.google.com/search?q=\"%s\"+%s\n",
            full, location
        );
        strncat(output, line, MAX_SE_OUTPUT - strlen(output) - 1);
    }

    return output;
}

