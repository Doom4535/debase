#pragma once
#include "lib/libcurl/include/curl/curl.h"
#include "lib/toastbox/Defer.h"
#include "lib/toastbox/RuntimeError.h"
#include "lib/nlohmann/json.h"

namespace Network {

template <typename T_Req, typename T_Resp>
inline void Request(const char* url, const T_Req& req, T_Resp& resp) {
    CURL* curl = curl_easy_init();
    if (!curl) throw Toastbox::RuntimeError("curl_easy_init failed");
    Defer(curl_easy_cleanup(curl));
    
    // Set URL
    curl_easy_setopt(curl, CURLOPT_URL, url);
    
    // Set headers
    struct curl_slist* headers = nullptr;
    Defer(curl_slist_free_all(headers));
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    // Encode request as json and put it in the request
    // Curl doesn't copy the data, so it needs to stay live until curl is complete!
    nlohmann::json j = req;
    std::string jstr = j.dump();
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jstr.c_str());
    
    std::stringstream respStream;
    auto respWriter = +[] (char* data, size_t _, size_t len, std::stringstream& respStream) -> size_t {
        respStream.write(data, len);
        return len;
    };
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respStream);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, respWriter);
    
    CURLcode cc = curl_easy_perform(curl);
    if (cc != CURLE_OK) throw Toastbox::RuntimeError("curl_easy_perform failed: %s", curl_easy_strerror(cc));
    
    // Decode response
    j = nlohmann::json::parse(respStream.str());
    j.get_to(resp);
}

} // namespace Network