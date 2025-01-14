#include <stdio.h>
#include <curl/curl.h>

#include "includes/config.h"

void* keepalive_request() {

    CURL *curl;
    CURLcode res;

    const char *url = config->host;

    // Initialize a CURL session
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = NULL;

        // Add API key header
        char api_key_header[256];
        snprintf(api_key_header, sizeof(api_key_header), "Authorization: Bearer %s", config->api_key);
        headers = curl_slist_append(headers, api_key_header);
        headers = curl_slist_append(headers, "Content-Type: application/json");

        // Set CURL options
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POST, 1L); // Enable POST method
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{}"); // Send an empty JSON body

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "Failed to initialize CURL\n");
    }

    return NULL;
}