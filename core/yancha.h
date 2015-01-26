#ifndef yancha_h
#define yancha_h

#define YANCHA_REST_API_RESPONSE_OK 0;
#define YANCHA_REST_API_RESPONSE_FAIL 1;

typedef struct {
    const char *key;
    const char *nickname;
    const char *profile_url;
    const char *profile_image_url;
} yancha_user_t;

typedef struct {
    const char *serverUrl;
    const char *token;
    const char *nickname;
} yancha_connection_t;

typedef struct {
    int page;
} yancha_search_condition_t;

typedef struct {
    long id;
    yancha_user_t author;
    const char *text;
    long created_at_ms;
} yancha_message_t;

yancha_connection_t yancha_init(char *serverUrl);

yancha_search_condition_t yancha_search_condition_init();

void yancha_login(yancha_connection_t *connection, char *nickname);

void yancha_search(yancha_connection_t *connection, yancha_message_t *response[],
                   yancha_search_condition_t condition);

// Return the YANCHA_REST_API_RESPONSE_*
int yancha_post(yancha_connection_t *connection, char *message);

void yancha_login_users(yancha_user_t *response, yancha_connection_t connection);

#endif
