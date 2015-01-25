#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "yancha.h"

// MEMFILE: http://mattn.kaoriya.net/software/lang/c/20130710214647.htm
typedef struct {
    char* data;   // response data from server
    size_t size;  // response size of data
} MEMFILE;

MEMFILE*
memfopen() {
    MEMFILE* mf = (MEMFILE*) malloc(sizeof(MEMFILE));
    if (mf) {
        mf->data = NULL;
        mf->size = 0;
    }
    return mf;
}

void
memfclose(MEMFILE* mf) {
    if (mf->data) free(mf->data);
    free(mf);
}

size_t
memfwrite(char* ptr, size_t size, size_t nmemb, void* stream) {
    MEMFILE* mf = (MEMFILE*) stream;
    size_t block = size * nmemb;
    if (!mf) return block; // through
    if (!mf->data)
        mf->data = (char*) malloc(block);
    else
        mf->data = (char*) realloc(mf->data, mf->size + block);
    if (mf->data) {
        memcpy(mf->data + mf->size, ptr, block);
        mf->size += block;
    }
    return block;
}

yancha_connection_t
yancha_init(char* serverUrl)
{
    yancha_connection_t connection = {};
    realloc(connection.serverUrl, sizeof(serverUrl));
    connection.serverUrl = serverUrl;
    return connection;
}

yancha_search_condition_t
yancha_search_condition_init()
{
    yancha_search_condition_t condition = {};
    return condition;
}

void
yancha_login(yancha_connection_t* connection, char* nickname)
{
    realloc(connection->nickname, sizeof(nickname));
    connection->nickname = nickname;

    MEMFILE* mf = NULL; mf = memfopen();
    CURL *curl = curl_easy_init();
    char* url = NULL;
    char* path = "/login?token_only=1&nick=taro";
    size_t url_size = strlen(connection->serverUrl) + strlen(path);

    url = (char*)malloc(url_size);
    sprintf(url, "%s%s", connection->serverUrl, path);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, mf);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, memfwrite);
    curl_easy_perform(curl);
    connection->token = mf->data;

    memfclose(mf);
    free(url);
    curl_easy_cleanup(curl);
}

void
yancha_search(yancha_message_t* response, yancha_search_condition_t condition)
{
    fprintf(stderr, "no implements: yancha_search");
}

int
yancha_post(yancha_connection_t* connection, char* message)
{
    CURL *curl = curl_easy_init();
    CURLcode res;
    char *path = "/api/post";
    char *url, *data = NULL;
    char *data_format = "token=%s&text=%s";
    {
        size_t url_size = strlen(connection->serverUrl) + strlen(path);
        url = (char *)malloc(url_size);
        sprintf(url, "%s%s", connection->serverUrl, path);
    }
    {
        size_t data_size = strlen(data_format)-4 + strlen(connection->token) + strlen(message);
        data = (char *)malloc(data_size);
        sprintf(data, data_format, connection->token, message);
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    res = curl_easy_perform(curl);

    free(url);
    free(data);

    if (res == CURLE_OK) {
        return YANCHA_REST_API_RESPONSE_OK;
    }
    return YANCHA_REST_API_RESPONSE_FAIL;
}

void
yancha_login_users(yancha_user_t* response, yancha_connection_t connection)
{
    fprintf(stderr, "no implements: yancha_login_users");
}
