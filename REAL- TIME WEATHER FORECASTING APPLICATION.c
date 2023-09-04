#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>  // need libcurl for HTTP requests.

// Struct for storing weather data.
struct WeatherData {
    char location[100];
    char temperature[20];
    char conditions[100];
};

// Callback function for libcurl to write response data.
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    strcat((char *)userp, (char *)contents);
    return total_size;
}

// Function to retrieve weather data from an API (OpenWeatherMap in this example).
int GetWeatherData(const char *city, struct WeatherData *weather) {
    CURL *curl;
    CURLcode res;

    // Initialize libcurl.
    curl = curl_easy_init();
    if (curl) {
        char url[200];
        char response_data[1000] = "";

        // Construct the API URL.
        sprintf(url, "http://api.openweathermap.org/data/2.5/weather?q=%s&appid=YOUR_API_KEY", city);

        // Set the URL and callback function.
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_data);

        // Perform the HTTP request.
        res = curl_easy_perform(curl);
        
        if (res != CURLE_OK) {
            fprintf(stderr, "Error: %s\n", curl_easy_strerror(res));
            return 1;  // Request failed.
        }

        // Parse the JSON response (I would need to add JSON parsing logic here).

        // Example parsing:
        // sscanf(response_data, "{\"name\":\"%[^\"]\",\"main\":{\"temp\":%s},\"weather\":[{\"description\":\"%[^\"]\"}]}", weather->location, weather->temperature, weather->conditions);

        // Clean up libcurl.
        curl_easy_cleanup(curl);

        return 0;  // Request succeeded.
    }
    
    return 1;  // Initialization of libcurl failed.
}

int main() {
    struct WeatherData weather;
    char city[100];

    printf("Enter city name: ");
    scanf("%s", city);

    if (GetWeatherData(city, &weather) == 0) {
        printf("\nWeather in %s:\n", weather.location);
        printf("Temperature: %s K\n", weather.temperature);
        printf("Conditions: %s\n", weather.conditions);
    } else {
        printf("Failed to retrieve weather data.\n");
    }

    return 0;
}