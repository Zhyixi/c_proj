#include <iostream>
#include <curl/curl.h>
#define CURL_STATICLIB
// 寫入回呼函式，用於處理接收到的數據
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    std::string* buffer = static_cast<std::string*>(userp);
    buffer->append(static_cast<char*>(contents), total_size);
    return total_size;
}

int main() {
    CURL* curl;
    CURLcode res;
    
    // 初始化 libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // 創建一個 curl handle
    curl = curl_easy_init();
    if (curl) {
        // 設置請求的 URL
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.example.com");
        
        // 設置回呼函式，用於處理接收到的數據
        std::string response_buffer;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_buffer);
        
        // 執行請求
        res = curl_easy_perform(curl);
        
        // 檢查是否發生錯誤
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // 請求成功，輸出接收到的數據
            std::cout << "Response:\n" << response_buffer << std::endl;
        }
        
        // 釋放 curl handle
        curl_easy_cleanup(curl);
    }
    
    // 釋放 libcurl
    curl_global_cleanup();
    
    return 0;
}