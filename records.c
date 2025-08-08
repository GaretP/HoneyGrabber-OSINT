#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <emscripten.h>

#define MAX_REC_OUTPUT 8192
static char output[MAX_REC_OUTPUT];

/* Tiny URL encoder for query components */
static void url_encode(const char* src, char* dst, size_t dst_size) {
    static const char hex[] = "0123456789ABCDEF";
    size_t di = 0;
    for (size_t i = 0; src[i] && di + 4 < dst_size; ++i) {
        unsigned char c = (unsigned char)src[i];
        int safe = (c=='-'||c=='_'||c=='.'||c=='~' ||
                    (c>='A'&&c<='Z') || (c>='a'&&c<='z') || (c>='0'&&c<='9'));
        if (safe) {
            dst[di++] = (char)c;
        } else {
            if (di + 3 >= dst_size) break;
            dst[di++] = '%';
            dst[di++] = hex[(c >> 4) & 0xF];
            dst[di++] = hex[c & 0xF];
        }
    }
    dst[di] = '\0';
}

EMSCRIPTEN_KEEPALIVE
const char* search_records(const char* first, const char* middle, const char* last,
                           const char* alias, const char* email, const char* location) {
    // Require first & last only
    if (!first || !last || strlen(first) == 0 || strlen(last) == 0) {
        snprintf(output, MAX_REC_OUTPUT,
                 "❌ ERROR: First and last name are required for record search.\n");
        return output;
    }

    // Null-safety for optional fields
    if (!middle)   middle   = "";
    if (!alias)    alias    = "";
    if (!email)    email    = "";
    if (!location) location = "";

    // Display name (shows middle and " - location" if provided)
    char display_name[256] = {0};
    if (strlen(middle) > 0) {
        snprintf(display_name, sizeof(display_name), "%s %s %s", first, middle, last);
    } else {
        snprintf(display_name, sizeof(display_name), "%s %s", first, last);
    }
    if (strlen(location) > 0) {
        strncat(display_name, " - ", sizeof(display_name) - strlen(display_name) - 1);
        strncat(display_name, location, sizeof(display_name) - strlen(display_name) - 1);
    }

    // Query string for URLs (name + optional location separated by a space), then URL-encoded
    char query_raw[256] = {0};
    if (strlen(middle) > 0) {
        snprintf(query_raw, sizeof(query_raw), "%s %s %s", first, middle, last);
    } else {
        snprintf(query_raw, sizeof(query_raw), "%s %s", first, last);
    }
    if (strlen(location) > 0) {
        strncat(query_raw, " ", sizeof(query_raw) - strlen(query_raw) - 1);
        strncat(query_raw, location, sizeof(query_raw) - strlen(query_raw) - 1);
    }
    char qenc[768]; // plenty of room for %XX expansion
    url_encode(query_raw, qenc, sizeof(qenc));

    // Build output
    snprintf(output, MAX_REC_OUTPUT,
        "=== Public Records / Court Lookup ===\n"
        "Search Target: %s\n\n"
        "[+] CourtListener:       https://www.courtlistener.com/?q=%s\n"
        "[+] PACER (US Courts):   https://pacer.uscourts.gov/\n"
        "[+] Justia:              https://law.justia.com/search?query=%s\n"
        "[+] StateRecords.org:    https://www.staterecords.org/search?q=%s\n"
        "[+] County Jail Inmate:  https://www.inmatesearcher.com/?q=%s\n"
        "[+] BustedMugshots:      https://www.bustedmugshots.com/search/?q=%s\n"
        "[+] Mugshots.com:        https://mugshots.com/search.html?q=%s\n"
        "[+] ArrestFacts.com:     https://www.arrestfacts.com/search?name=%s\n"
        "[+] OpenSanctions:       https://www.opensanctions.org/search/?q=%s\n",
        display_name,
        qenc, qenc, qenc, qenc, qenc, qenc, qenc, qenc
    );

    // Optional info block (only prints if provided)
    if (strlen(alias) > 0) {
        strncat(output, "\nAlias Provided: ", MAX_REC_OUTPUT - strlen(output) - 1);
        strncat(output, alias,           MAX_REC_OUTPUT - strlen(output) - 1);
        strncat(output, "\n",            MAX_REC_OUTPUT - strlen(output) - 1);
    }
    if (strlen(email) > 0) {
        strncat(output, "\nEmail Provided: ", MAX_REC_OUTPUT - strlen(output) - 1);
        strncat(output, email,           MAX_REC_OUTPUT - strlen(output) - 1);
        strncat(output, "\n",            MAX_REC_OUTPUT - strlen(output) - 1);
    }
    if (strlen(location) > 0) {
        strncat(output, "\nLocation Provided: ", MAX_REC_OUTPUT - strlen(output) - 1);
        strncat(output, location,             MAX_REC_OUTPUT - strlen(output) - 1);
        strncat(output, "\n",                  MAX_REC_OUTPUT - strlen(output) - 1);
    }

    return output;
}
