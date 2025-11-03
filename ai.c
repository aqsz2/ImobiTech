#include "ai.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef USE_CURL
#include <curl/curl.h>

typedef struct {
    char* data;
    size_t size;
} Buffer;

static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    Buffer* mem = (Buffer*)userp;
    char* ptr = (char*)realloc(mem->data, mem->size + realsize + 1);
    if (!ptr) return 0;
    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->data[mem->size] = 0;
    return realsize;
}
#endif

static void localFallback(const char* prompt, char* outBuffer, size_t outSize) {
    const char* samples[] = {
        "Um bug misterioso causa rollback parcial, mas você aprende uma lição valiosa.",
        "Seu pitch em um hackathon rende um micro-investimento surpresa!",
        "Um vazamento de dados exige auditoria rápida e custa caro.",
        "Você otimiza a stack e reduz custos de nuvem neste ciclo.",
        "Uma parceria com uma big tech abre novas oportunidades." 
    };
    size_t idx = (size_t)(rand() % (int)(sizeof(samples)/sizeof(samples[0])));
    snprintf(outBuffer, outSize, "%s\nAI: %s", prompt ? prompt : "", samples[idx]);
}

size_t aiGenerateText(const char* prompt, char* outBuffer, size_t outSize) {
    if (!outBuffer || outSize == 0) return 0;
    outBuffer[0] = '\0';

#ifdef USE_CURL
    const char* apiKey = getenv("GEMINI_API_KEY");
    if (apiKey && apiKey[0]) {
        CURL* curl = curl_easy_init();
        if (curl) {
            char url[512];
            snprintf(url, sizeof(url),
                     "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=%s",
                     apiKey);

            const char* p = prompt ? prompt : "Gere um resumo tecnológico em português.";
            char json[1024];
            snprintf(json, sizeof(json),
                     "{\"contents\":[{\"parts\":[{\"text\":\"%s\"}]}]}", p);

            Buffer buf = {0};
            struct curl_slist* headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/json");

            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&buf);
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK && buf.data) {
                // Simplesmente copia o JSON cru (para simplicidade). Em produção, parseie o JSON.
                snprintf(outBuffer, outSize, "%.*s", (int)(outSize - 1), buf.data);
            } else {
                localFallback(prompt, outBuffer, outSize);
            }
            if (headers) curl_slist_free_all(headers);
            if (buf.data) free(buf.data);
            curl_easy_cleanup(curl);
            return strlen(outBuffer);
        }
    }
#endif
    localFallback(prompt, outBuffer, outSize);
    return strlen(outBuffer);
}


