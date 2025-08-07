#include <stdio.h>
#include <string.h>
#include <emscripten.h>

#define MAX_RL_OUTPUT 8192
static char output[MAX_RL_OUTPUT];

EMSCRIPTEN_KEEPALIVE
const char* reverse_lookup(const char* email, const char* location) {
    memset(output, 0, MAX_RL_OUTPUT);

    int hasEmail = email && strlen(email) > 0;
    int hasLocation = location && strlen(location) > 0;

    if (!hasEmail && !hasLocation) {
        snprintf(output, MAX_RL_OUTPUT, "ERROR: Provide at least an email or location.\n");
        return output;
    }

    snprintf(output, MAX_RL_OUTPUT,
        "=== Reverse Lookup Tools ===\n\n");

    if (hasEmail) {
        strncat(output, "🔍 Email-Based Lookups:\n", MAX_RL_OUTPUT - strlen(output) - 1);

        char encoded[512];
        snprintf(encoded, sizeof(encoded), "%s", email);  // could urlencode later if needed

        char line[512];
        snprintf(line, sizeof(line), "[+] EmailReputation: https://emailrep.io/%s\n", encoded);
        strncat(output, line, MAX_RL_OUTPUT - strlen(output) - 1);

        snprintf(line, sizeof(line), "[+] Hunter.io: https://hunter.io/email-finder/%s\n", encoded);
        strncat(output, line, MAX_RL_OUTPUT - strlen(output) - 1);

        snprintf(line, sizeof(line), "[+] That’sThem: https://thatsthem.com/email/%s\n", encoded);
        strncat(output, line, MAX_RL_OUTPUT - strlen(output) - 1);

        snprintf(line, sizeof(line), "[+] HaveIBeenPwned: https://haveibeenpwned.com/unifiedsearch/%s\n", encoded);
        strncat(output, line, MAX_RL_OUTPUT - strlen(output) - 1);

        strncat(output, "\n", MAX_RL_OUTPUT - strlen(output) - 1);
    }

    if (hasLocation) {
        strncat(output, "Location-Based People Searches:\n", MAX_RL_OUTPUT - strlen(output) - 1);

        char encodedLoc[512];
        snprintf(encodedLoc, sizeof(encodedLoc), "%s", location);  // could urlencode for JS later

        char line[512];
        snprintf(line, sizeof(line), "[+] TruePeopleSearch: https://www.truepeoplesearch.com/results?searchtype=location&citystatezip=%s\n", encodedLoc);
        strncat(output, line, MAX_RL_OUTPUT - strlen(output) - 1);

        snprintf(line, sizeof(line), "[+] Whitepages: https://www.whitepages.com/name/%s\n", encodedLoc);
        strncat(output, line, MAX_RL_OUTPUT - strlen(output) - 1);

        snprintf(line, sizeof(line), "[+] FastPeopleSearch: https://www.fastpeoplesearch.com/address-search/%s\n", encodedLoc);
        strncat(output, line, MAX_RL_OUTPUT - strlen(output) - 1);

        snprintf(line, sizeof(line), "[+] Spokeo: https://www.spokeo.com/%s\n", encodedLoc);
        strncat(output, line, MAX_RL_OUTPUT - strlen(output) - 1);
    }

    return output;
}
