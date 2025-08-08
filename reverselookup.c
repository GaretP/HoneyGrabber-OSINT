#include <stdio.h>
#include <string.h>
#include <emscripten/emscripten.h>

#define MAX_RL_OUTPUT 8192
static char output[MAX_RL_OUTPUT];

/* tiny URL-encode for safe queries */
static void url_encode(const char* src, char* dst, size_t dst_size) {
    static const char hex[] = "0123456789ABCDEF";
    size_t di = 0;
    for (size_t i = 0; src[i] && di + 4 < dst_size; ++i) {
        unsigned char c = (unsigned char)src[i];
        int safe = (c=='-'||c=='_'||c=='.'||c=='~' ||
                    (c>='A'&&c<='Z') || (c>='a'&&c<='z') || (c>='0'&&c<='9'));
        if (safe) dst[di++] = (char)c;
        else {
            dst[di++] = '%';
            dst[di++] = hex[(c >> 4) & 0xF];
            dst[di++] = hex[c & 0xF];
        }
    }
    dst[di] = '\0';
}

EMSCRIPTEN_KEEPALIVE
const char* reverse_lookup(const char* email, const char* first, const char* last, const char* location) {
    memset(output, 0, MAX_RL_OUTPUT);

    int hasEmail    = (email    && *email);
    int hasFirst    = (first    && *first);
    int hasLast     = (last     && *last);
    int hasLocation = (location && *location);

    if (!hasEmail && !hasLocation) {
        snprintf(output, MAX_RL_OUTPUT, "ERROR: Provide at least an email or a location (with first & last).\n");
        return output;
    }

    strncat(output, "=== Reverse Lookup Tools ===\n\n", MAX_RL_OUTPUT - strlen(output) - 1);

    /* EMAIL LOOKUPS (unchanged) */
    if (hasEmail) {
        strncat(output, "🔍 Email-Based Lookups:\n", MAX_RL_OUTPUT - strlen(output) - 1);

        char encoded[512];
        url_encode(email, encoded, sizeof(encoded));

        char line[640];
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

    /* LOCATION + NAME LOOKUPS (require first & last if location provided) */
    if (hasLocation) {
        if (!(hasFirst && hasLast)) {
            snprintf(output + strlen(output), MAX_RL_OUTPUT - strlen(output),
                     "❌ ERROR: Location-based search requires First and Last name.\n");
            return output;
        }

        strncat(output, "📍 Location-Based People Searches:\n", MAX_RL_OUTPUT - strlen(output) - 1);

        /* Build encoded components */
        char name_raw[256] = {0};
        snprintf(name_raw, sizeof(name_raw), "%s %s", first, last);

        char nameq[512], locq[512];
        url_encode(name_raw, nameq, sizeof(nameq));
        url_encode(location, locq, sizeof(locq));

        char line[768];

        /* TruePeopleSearch */
        snprintf(line, sizeof(line),
            "[+] TruePeopleSearch: https://www.truepeoplesearch.com/results?name=%s&citystatezip=%s\n",
            nameq, locq);
        strncat(output, line, MAX_RL_OUTPUT - strlen(output) - 1);

        /* Whitepages */
        snprintf(line, sizeof(line),
            "[+] Whitepages: https://www.whitepages.com/name/%s/%s\n",
            nameq, locq);
        strncat(output, line, MAX_RL_OUTPUT - strlen(output) - 1);

        /* FastPeopleSearch */
        snprintf(line, sizeof(line),
            "[+] FastPeopleSearch: https://www.fastpeoplesearch.com/name/%s/%s\n",
            nameq, locq);
        strncat(output, line, MAX_RL_OUTPUT - strlen(output) - 1);

        /* Spokeo */
        snprintf(line, sizeof(line),
            "[+] Spokeo: https://www.spokeo.com/%s?where=%s\n",
            nameq, locq);
        strncat(output, line, MAX_RL_OUTPUT - strlen(output) - 1);
    }

    return output;
}
