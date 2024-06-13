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
