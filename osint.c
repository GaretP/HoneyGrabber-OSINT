#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <emscripten.h>

#define MAX_OUTPUT 32768
#define MAX_VARIATIONS 32
static char output[MAX_OUTPUT];

// Utility: lower-case string in-place
void to_lower(char* str) {
    for (; *str; ++str) *str = tolower(*str);
}

// Generate simple username variations
int generate_variations(char variations[][64], const char* first, const char* middle, const char* last) {
    char f[64], m[64], l[64];
    strncpy(f, first, 63); to_lower(f);
    strncpy(m, middle ? middle : "", 63); to_lower(m);
    strncpy(l, last, 63); to_lower(l);

    int i = 0;
    snprintf(variations[i++], 64, "%s%s", f, l);               // alexsanches
    snprintf(variations[i++], 64, "%s.%s", f, l);              // alex.sanches
    snprintf(variations[i++], 64, "%s_%s", f, l);              // alex_sanches
    snprintf(variations[i++], 64, "%s%s", l, f);               // sanchesalex
    snprintf(variations[i++], 64, "%s.%s", l, f);              // sanches.alex
    snprintf(variations[i++], 64, "%c.%s", f[0], l);           // a.sanches
    snprintf(variations[i++], 64, "%s%s", f, m);               // alexj
    snprintf(variations[i++], 64, "%s.%c.%s", f, m[0], l);     // alex.j.sanches
    snprintf(variations[i++], 64, "%s-%s", f, l);              // alex-sanches

    return i; // count
}

void append_platform_block(char* buf, size_t bufsize, const char* label, const char* username) {
    char line[512];

    // Label (username variant being tested)
    snprintf(line, sizeof(line), "\n=== Results for: %s ===\n", label);
    strncat(buf, line, bufsize - strlen(buf) - 1);

    // Social Media
    snprintf(line, sizeof(line), "@instagram: https://www.instagram.com/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@tiktok: https://www.tiktok.com/@%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@snapchat: https://www.snapchat.com/add/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@telegram: https://t.me/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@reddit: https://www.reddit.com/user/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@linkedin: https://www.linkedin.com/in/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@pinterest: https://www.pinterest.com/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@tumblr: https://%s.tumblr.com\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@mastodon: https://mastodon.social/@%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@threads: https://www.threads.net/@%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);

    // Messaging
    snprintf(line, sizeof(line), "@discord: (username may require full tag like %s#1234)\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@matrix: https://matrix.to/#/@%s:matrix.org\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@skype: skype:%s?chat\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@kik: (no public profile, try searching for: %s)\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);

    // Developer / Tech
    snprintf(line, sizeof(line), "@github: https://github.com/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@gitlab: https://gitlab.com/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@stackoverflow: https://stackoverflow.com/users/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@devto: https://dev.to/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@hackernews: https://news.ycombinator.com/user?id=%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);

    // Streaming / Content
    snprintf(line, sizeof(line), "@youtube: https://www.youtube.com/@%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@twitch: https://www.twitch.tv/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@vimeo: https://vimeo.com/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@dailymotion: https://www.dailymotion.com/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@soundcloud: https://soundcloud.com/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@bandcamp: https://%s.bandcamp.com\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);

    // Art / Portfolio
    snprintf(line, sizeof(line), "@deviantart: https://www.deviantart.com/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@behance: https://www.behance.net/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@dribbble: https://dribbble.com/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@artstation: https://www.artstation.com/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@kofi: https://ko-fi.com/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@carrd: https://%s.carrd.co\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);

    // Blogs / Forums
    snprintf(line, sizeof(line), "@medium: https://medium.com/@%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@substack: https://%s.substack.com\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@wordpress: https://%s.wordpress.com\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@blogger: https://%s.blogspot.com\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@disqus: https://disqus.com/by/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);

    // Forum Sites
    snprintf(line, sizeof(line), "@somethingawful: https://forums.somethingawful.com/search.php?query=%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@kiwifarms: https://kiwifarms.net/members/?q=%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@forumotion: https://%s.forumotion.com\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);

    // Crypto / Web3
    snprintf(line, sizeof(line), "@ens: https://app.ens.domains/name/%s.eth\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@lens: https://lenster.xyz/u/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@farcaster: https://warpcast.com/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
    snprintf(line, sizeof(line), "@opensea: https://opensea.io/%s\n", username);
    strncat(buf, line, bufsize - strlen(buf) - 1);
}

EMSCRIPTEN_KEEPALIVE
const char* sweep_osint(const char* first, const char* middle, const char* last, const char* alias, const char* email, const char* location) {
    memset(output, 0, MAX_OUTPUT);

    if (!first || !last || strlen(first) == 0 || strlen(last) == 0) {
        snprintf(output, MAX_OUTPUT, "❌ ERROR: First and last name are required.\n");
        return output;
    }

    strncat(output, "=== OSINT Username Sweep ===\n", MAX_OUTPUT - strlen(output) - 1);

    char variations[MAX_VARIATIONS][64];
    int count = generate_variations(variations, first, middle, last);

    // Output all variations
    for (int i = 0; i < count; i++) {
        append_platform_block(output, MAX_OUTPUT, variations[i], variations[i]);
    }

    // Alias (if any)
    if (alias && strlen(alias) > 0) {
        append_platform_block(output, MAX_OUTPUT, alias, alias);
    }

    // Optional info
    if (email && strlen(email) > 0) {
        strncat(output, "\n=== Email Provided ===\n", MAX_OUTPUT - strlen(output) - 1);
        strncat(output, email, MAX_OUTPUT - strlen(output) - 1);
        strncat(output, "\n", MAX_OUTPUT - strlen(output) - 1);
    }

    if (location && strlen(location) > 0) {
        strncat(output, "\n=== Location Provided ===\n", MAX_OUTPUT - strlen(output) - 1);
        strncat(output, location, MAX_OUTPUT - strlen(output) - 1);
        strncat(output, "\n", MAX_OUTPUT - strlen(output) - 1);
    }

    return output;
}

