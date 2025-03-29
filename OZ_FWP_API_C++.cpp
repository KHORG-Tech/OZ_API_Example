#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include "json.hpp"  // nlohmann/json

using json = nlohmann::json;
using namespace std;

// Write callback for curl
size_t writeCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Function to send POST requests
string postRequest(const string& url, const string& body, const string& authToken = "") {
    CURL* curl = curl_easy_init();
    string response;
    struct curl_slist* headers = nullptr;

    if (curl) {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        if (!authToken.empty()) {
            headers = curl_slist_append(headers, ("Authorization: Bearer " + authToken).c_str());
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return response;
}

int main() {
    string loginUrl = "https://foodwaste-api-iz3pu.ondigitalocean.app/login";
    string dataUrl = "https://foodwaste-api-iz3pu.ondigitalocean.app/getCurrentFoodwasteData_by_machine_name";

    // Step 1: Login
    json loginPayload = {
        {"email", "your_email"},
        {"password", "your_password"}
    };

    string loginResponseStr = postRequest(loginUrl, loginPayload.dump());
    json loginResponse = json::parse(loginResponseStr);

    if (!loginResponse.contains("token")) {
        cerr << "Login failed or token not returned." << endl;
        return 1;
    }

    string token = loginResponse["token"];
    cout << "Logged in. Token: " << token << endl;

    // Step 2: Get foodwaste data
    json dataPayload = {
        {"machine_name", "your_machine_name"}
    };

    string dataResponseStr = postRequest(dataUrl, dataPayload.dump(), token);
    json dataResponse = json::parse(dataResponseStr);

    cout << "Data received:\n" << dataResponse.dump(2) << endl;

    // Step 3: Save to file
    ofstream file("response.json");
    file << dataResponse.dump(4);
    file.close();

    cout << "Saved to response.json" << endl;

    return 0;
}
