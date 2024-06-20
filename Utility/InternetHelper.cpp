//
// Created by wuru8 on 2024/6/13.
//
#include "Engine/LOG.hpp"
#include "InternetHelper.hpp"
#include <curl/curl.h>

size_t InternetHelper::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

bool InternetHelper::downloadFile(const std::string &url, const std::string &localPath) {
    Engine::LOG(Engine::INFO) << "Try to download file from " << url << " to " << localPath;
    CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
        fp = fopen(localPath.c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
        return res == CURLE_OK;
    }
    return false;
}

bool InternetHelper::uploadFile(const std::string &url_to_upload_php, const std::string &localPath) {
    Engine::LOG(Engine::INFO) << "Try to Upload File from " << localPath << " to " << url_to_upload_php;

    CURL *curl;
    CURLcode res;

    curl_mime *form = NULL;
    curl_mimepart *field = NULL;

    curl = curl_easy_init();
    if (curl) {
        // Create the form
        form = curl_mime_init(curl);

        // Add the file field
        field = curl_mime_addpart(form);
        curl_mime_name(field, "file");
        curl_mime_filedata(field, localPath.c_str());

        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, url_to_upload_php.c_str());

        // Set the form info
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            Engine::LOG(Engine::WARN) << "curl_easy_perform() failed: " << curl_easy_strerror(res);
        }

        // Clean up
        curl_mime_free(form);
        curl_easy_cleanup(curl);

        return (res == CURLE_OK);
    }
    return false;
}