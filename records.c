#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <emscripten.h>

#define MAX_REC_OUTPUT 8192
static char output[MAX_REC_OUTPUT];

EMSCRIPTEN_KEEPALIVE
const char* search_records(const char* first, const char* middle, const char* last, const char* alias, const char* email, const char* location) {
    if (!first || !last || strlen(first) == 0 || strlen(last) == 0) {
        snprintf(output, MAX_REC_OUTPUT, "❌ ERROR: First and last name are required for record search.\n");
        return output;
    }

    char fullname[256] = {0};
    if (middle && strlen(middle) > 0) {
        snprintf(fullname, sizeof(fullname), "%s %s %s", first, middle, last);
    } else {
        snprintf(fullname, sizeof(fullname), "%s %s", first, last);
    }

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
        fullname,
        fullname, fullname, fullname, fullname,
        fullname, fullname, fullname, fullname
    );

    // Append optional info
    if (alias && strlen(alias) > 0) {
        strncat(output, "\nAlias Provided: ", MAX_REC_OUTPUT - strlen(output) - 1);
        strncat(output, alias, MAX_REC_OUTPUT - strlen(output) - 1);
        strncat(output, "\n", MAX_REC_OUTPUT - strlen(output) - 1);
    }

    if (email && strlen(email) > 0) {
        strncat(output, "\nEmail Provided: ", MAX_REC_OUTPUT - strlen(output) - 1);
        strncat(output, email, MAX_REC_OUTPUT - strlen(output) - 1);
        strncat(output, "\n", MAX_REC_OUTPUT - strlen(output) - 1);
    }

    if (location && strlen(location) > 0) {
        strncat(output, "\nLocation Provided: ", MAX_REC_OUTPUT - strlen(output) - 1);
        strncat(output, location, MAX_REC_OUTPUT - strlen(output) - 1);
        strncat(output, "\n", MAX_REC_OUTPUT - strlen(output) - 1);
    }

    return output;
}

