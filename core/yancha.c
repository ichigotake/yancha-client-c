#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>
#include <math.h>
#include "yancha.h"

// memorry function pointer
// ref: https://github.com/akheron/jansson/blob/c8d017bd8842bbaa1b15d1f49f57577089666424/src/memory.c
static yancha_malloc_t do_malloc = malloc;
static yancha_free_t do_free = free;

// MEMFILE: http://mattn.kaoriya.net/software/lang/c/20130710214647.htm
typedef struct {
    char *data;  // response data from server
    size_t size; // response size of data
} MEMFILE;

MEMFILE *memfopen() {
    MEMFILE *mf = (MEMFILE *)malloc(sizeof(MEMFILE));
    if (mf) {
        mf->data = NULL;
        mf->size = 0;
    }
    return mf;
}

void memfclose(MEMFILE *mf) {
    if (mf->data)
        free(mf->data);
    free(mf);
}

size_t memfwrite(char *ptr, size_t size, size_t nmemb, void *stream) {
    MEMFILE *mf = (MEMFILE *)stream;
    size_t block = size * nmemb;
    if (!mf)
        return block; // through
    if (!mf->data)
        mf->data = (char *)malloc(block);
    else
        mf->data = (char *)realloc(mf->data, mf->size + block);
    if (mf->data) {
        memcpy(mf->data + mf->size, ptr, block);
        mf->size += block;
    }
    return block;
}

void *yancha_malloc(size_t size) {
    if (!size) {
        return NULL;
    }
    return (*do_malloc)(size);
}

void yancha_free(void *ptr) {
    if (!ptr)
        return;

    (*do_free)(ptr);
}

yancha_t *yancha_init(char *serverUrl, const int max_mgs_arr_size) {
    yancha_connection_t connection = { serverUrl, NULL, NULL };
    yancha_t *yancha = yancha_malloc(sizeof(yancha_t));
    yancha->connection = connection;
    yancha->max_message_size = max_mgs_arr_size;
    return yancha;
}

void yancha_cleanup(yancha_t *yancha) { yancha_free(yancha); }

yancha_search_condition_t yancha_search_condition_init() {
    yancha_search_condition_t condition = {};
    return condition;
}

void yancha_login(yancha_t *yancha, char *nickname) {
    yancha->connection.nickname = nickname;

    MEMFILE *mf = NULL;
    mf = memfopen();
    CURL *curl = curl_easy_init();
    char *url = NULL;
    char *path = "/login?token_only=1&nick=taro";
    size_t url_size = strlen(yancha->connection.serverUrl) + strlen(path);

    url = (char *)malloc(url_size);
    sprintf(url, "%s%s", yancha->connection.serverUrl, path);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, mf);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, memfwrite);
    curl_easy_perform(curl);
    yancha->connection.token = mf->data;

    memfclose(mf);
    free(url);
    curl_easy_cleanup(curl);
}

int yancha_search(yancha_t *yancha, yancha_message_t *response,
                  yancha_search_condition_t condition) {
    CURL *curl = curl_easy_init();
    CURLcode res;
    MEMFILE *mf = NULL;
    char *url, *data = NULL;
    {
        char *url_fmt = "%s/api/search?page=%d";
        size_t url_size = strlen(yancha->connection.serverUrl) - 2 + strlen(url_fmt) - 4;
        url = (char *)malloc(url_size);
        float page = fmaxf(1, condition.page);
        sprintf(url, url_fmt, yancha->connection.serverUrl, page);
    }
    mf = memfopen();

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, mf);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, memfwrite);

    res = curl_easy_perform(curl);

    free(url);
    free(data);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        return YANCHA_REST_API_RESPONSE_FAIL;
    }

    json_error_t err;
    json_t *json = json_loads(mf->data, 0, &err);
    size_t size = json_array_size(json);
    for (size_t i = 0; i < size; i++) {
        json_t *row = json_array_get(json, i);
        yancha_message_t msg = {};

        // text
        const char *text = json_string_value(json_object_get(row, "text"));
        msg.text = malloc(sizeof(char) * (strlen(text) + 1));
        strcpy(msg.text, text);

        yancha_user_t author = {};
        {
            // key
            const char *key = json_string_value(json_object_get(row, "user_key"));
            author.key = malloc(sizeof(char) * (strlen(key) + 1));
            strcpy(author.key, key);

            // nickname
            const char *nickname = json_string_value(json_object_get(row, "nickname"));
            author.nickname = malloc(sizeof(char) * (strlen(nickname) + 1));
            strcpy(author.nickname, nickname);

            // profile_image_url
            const char *profile_image_url =
                json_string_value(json_object_get(row, "profile_image_url"));
            author.profile_image_url = malloc(sizeof(char) * (strlen(profile_image_url) + 1));
            strcpy(author.profile_image_url, profile_image_url);

            // profile_url
            const char *profile_url = json_string_value(json_object_get(row, "profile_url"));
            author.profile_url = malloc(sizeof(char) * (strlen(profile_url) + 1));
            strcpy(author.profile_url, profile_url);
        }
        msg.author = author;

        response[i] = msg;
    }

    return YANCHA_REST_API_RESPONSE_OK;
}

int yancha_post(yancha_t *yancha, char *message) {
    CURL *curl = curl_easy_init();
    CURLcode res;
    char *url, *data = NULL;
    {
        char *path = "/api/post";
        size_t url_size = strlen(yancha->connection.serverUrl) + strlen(path);
        url = (char *)malloc(url_size);
        sprintf(url, "%s%s", yancha->connection.serverUrl, path);
    }
    {
        char *data_format = "token=%s&text=%s";
        size_t data_size =
            strlen(data_format) - 4 + strlen(yancha->connection.token) + strlen(message);
        data = (char *)malloc(data_size);
        sprintf(data, data_format, yancha->connection.token, message);
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    res = curl_easy_perform(curl);

    free(url);
    free(data);
    curl_easy_cleanup(curl);

    if (res == CURLE_OK) {
        return YANCHA_REST_API_RESPONSE_OK;
    }
    return YANCHA_REST_API_RESPONSE_FAIL;
}
