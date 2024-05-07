#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void URLParser(char url[]) {
    // find fragment
    char* fragment = strchr(url, '#');
    if (fragment == NULL) {
        printf("No fragment found\n");
    }
    else {
        printf("Fragment: %s\n", fragment + 1);
    }

    // find query
    char* query = strchr(url, '?');
    if (query == NULL) {
        printf("No query found\n");
    }
    else {
        // Optionally split the query from the fragment
        char* end = strchr(query, '#');
        if (end) *end = '\0';  // Terminate the query string before the fragment
        printf("Query: %s\n", query + 1);
        if (end) *end = '#';  // Restore the '#' if it was modified
    }

    // find path
    char* path = strchr(url, '/');
    if (path == NULL) {
        printf("No path found\n");
    }
    else {
        char* end = strpbrk(path, "?#");  // End at query or fragment
        if (end) *end = '\0';  // Temporarily terminate
        printf("Path: %s\n", path);
        if (end) *end = end[0];  // Restore character
    }

    // find port
    char* port = strchr(url, ':');
    if (port == NULL) {
        printf("No port found\n");
    }
    else {
        // Ensure it's a port number by checking what follows the colon
        if (port[1] >= '0' && port[1] <= '9') {
            char* end = strpbrk(port, "/?#");
            if (end) *end = '\0';  // Temporarily terminate
            printf("Port: %s\n", port + 1);
            if (end) *end = end[0];  // Restore character
        }
        else {
            printf("No port found\n");
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("No URL provided, terminating.\n");
        exit(-1);
    }
    //  https://agrumer.livejournal.com/493956.html?title=Ello&hashtags=&text=For%20those%20trying%20out%20Ello,%20%20I%E2%80%99m%20there%20as%20avram.
    printf("URL: %s\nParsing...\n", argv[1]);
    URLParser(argv[1]);
    return 0;
}
