#include <stdio.h>
#include <string.h>
#include <emscripten.h>

#define MAX_FRIEND_OUTPUT 4096
static char output[MAX_FRIEND_OUTPUT];

EMSCRIPTEN_KEEPALIVE
const char* friend_map(const char* platform, const char* username) {
    memset(output, 0, MAX_FRIEND_OUTPUT);

    if (!platform || !username || strlen(platform) == 0 || strlen(username) == 0) {
        snprintf(output, MAX_FRIEND_OUTPUT, "❌ ERROR: Platform and username are required.\n");
        return output;
    }

    snprintf(output, MAX_FRIEND_OUTPUT, "=== Friend Graph Lookup (%s) ===\n\n", platform);

    if (strcmp(platform, "steam") == 0) {
        snprintf(output + strlen(output), MAX_FRIEND_OUTPUT - strlen(output),
            "🔗 Steam Profile: https://steamcommunity.com/id/%s\n"
            "👥 Friends List:  https://steamcommunity.com/id/%s/friends/\n",
            username, username);
    } else if (strcmp(platform, "instagram") == 0) {
        snprintf(output + strlen(output), MAX_FRIEND_OUTPUT - strlen(output),
            "🔗 Instagram Profile: https://www.instagram.com/%s\n"
            "👥 Followers:         https://www.instagram.com/%s/followers\n"
            "👤 Following:         https://www.instagram.com/%s/following\n",
            username, username, username);
    } else if (strcmp(platform, "twitter") == 0 || strcmp(platform, "x") == 0) {
        snprintf(output + strlen(output), MAX_FRIEND_OUTPUT - strlen(output),
            "🔗 Twitter Profile: https://twitter.com/%s\n"
            "👥 Followers:       https://twitter.com/%s/followers\n"
            "👤 Following:       https://twitter.com/%s/following\n",
            username, username, username);
    } else if (strcmp(platform, "github") == 0) {
        snprintf(output + strlen(output), MAX_FRIEND_OUTPUT - strlen(output),
            "🔗 GitHub Profile: https://github.com/%s\n"
            "👥 Followers:      https://github.com/%s?tab=followers\n"
            "👤 Following:      https://github.com/%s?tab=following\n",
            username, username, username);
    } else if (strcmp(platform, "reddit") == 0) {
        snprintf(output + strlen(output), MAX_FRIEND_OUTPUT - strlen(output),
            "🔗 Reddit Profile:  https://www.reddit.com/user/%s\n"
            "🔍 Mention Search: https://www.reddit.com/search/?q=u/%s\n",
            username, username);
    } else {
        snprintf(output + strlen(output), MAX_FRIEND_OUTPUT - strlen(output),
            "⚠️ Unsupported platform: %s\n", platform);
    }

    return output;
}
