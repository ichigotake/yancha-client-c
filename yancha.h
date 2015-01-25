#ifndef yancha_h
#define yancha_h

#include "curl/curl.h"

#define YANCHA_REST_API_RESPONSE_OK 0;
#define YANCHA_REST_API_RESPONSE_FAIL 1;

typedef struct {
    char* token;
    yancha_user_t user;
} yancha_connection_t;

typedef struct {
    char* key;
    char* nickname;
    char* profile_url;
    char* profile_image_url;
} yancha_user_t;

typedef struct {
    int page;
} yancha_search_condition_t;

typedef struct {
    int id;
    yancha_user_t author;
    char* text;
    long timestamp_ms;
} yancha_message_t;

yancha_connection_t yancha_login();

yancha_message_t* yancha_search(yancha_search_condition_t condition);

// Return the YANCHA_REST_API_RESPONSE_*
int yancha_post(yancha_connection_t connection, char* message);

yancha_user_t* yancha_login_users(yancha_connection_t connection);

#endif
