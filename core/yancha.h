#ifndef yancha_h
#define yancha_h

#define YANCHA_REST_API_RESPONSE_OK 0;
#define YANCHA_REST_API_RESPONSE_FAIL 1;

typedef struct {
    char* key;
    char* nickname;
    char* profile_url;
    char* profile_image_url;
} yancha_user_t;

typedef struct {
    char* serverUrl;
    char* token;
    char* nickname;
} yancha_connection_t;

typedef struct {
    int page;
} yancha_search_condition_t;

typedef struct {
    int id;
    yancha_user_t author;
    char* text;
    long timestamp_ms;
} yancha_message_t;

yancha_connection_t yancha_init(char* serverUrl);

yancha_search_condition_t yancha_search_condition_init();

void yancha_login(yancha_connection_t* connection, char* nickname);

void yancha_search(yancha_message_t* response, yancha_search_condition_t condition);

// Return the YANCHA_REST_API_RESPONSE_*
int yancha_post(yancha_connection_t* connection, char* message);

void yancha_login_users(yancha_user_t* response, yancha_connection_t connection);

#endif
