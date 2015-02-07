#ifndef yancha_h
#define yancha_h

#define YANCHA_REST_API_RESPONSE_OK 0;
#define YANCHA_REST_API_RESPONSE_FAIL 1;

typedef struct {
    char *key;
    char *nickname;
    char *profile_url;
    char *profile_image_url;
} yancha_user_t;

typedef struct {
    char *serverUrl;
    char *token;
    char *nickname;
} yancha_connection_t;

typedef struct {
    int page;
} yancha_search_condition_t;

// TODO: yancha_message_t as linked-list
typedef struct {
    long id;
    yancha_user_t author;
    char *text;
    long created_at_ms;
} yancha_message_t;

typedef struct {
    yancha_connection_t connection;
    int max_message_size;
} yancha_t;

yancha_t *yancha_init(char *serverUrl, const int maxMsgArrSize);

void yancha_cleanup(yancha_t *yancha);

yancha_search_condition_t yancha_search_condition_init();

void yancha_login(yancha_t *yancha, char *nickname);

int yancha_search(yancha_t *yancha, yancha_message_t *response,
                  yancha_search_condition_t condition);

void yancha_message_free(yancha_message_t *msg);

// Return the YANCHA_REST_API_RESPONSE_*
int yancha_post(yancha_t *yancha, char *message);

/**
 * custom memory allocation
 * ref:
 * https://github.com/akheron/jansson/blob/c8d017bd8842bbaa1b15d1f49f57577089666424/src/jansson.h
 */
typedef void *(*yancha_malloc_t)(size_t);
typedef void (*yancha_free_t)(void *);

#endif
