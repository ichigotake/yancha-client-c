#include <stdio.h>
#include <string.h>
#include "picotest.h"
#include "test.h"
#include "yancha.h"

void yancha_test_client() {
    char *name = "taro";

    yancha_t *yancha = yancha_init("http://127.0.0.1:3000", 1000);
    ok(yancha->connection.token == NULL);

    yancha_login(yancha, name);
    ok(strlen(yancha->connection.token) > 0);
    ok(!strcmp(yancha->connection.nickname, name));

    yancha_post(yancha, "Hello!");

    yancha_message_t messages[yancha->max_message_size];
    yancha_search_condition_t condition = yancha_search_condition_init();
    yancha_search(yancha, messages, condition);
    ok(1);

    for (size_t i = 0; i < yancha->max_message_size; i++) {
        yancha_message_t *msg = &messages[i];
        if (msg->text == NULL) {
            break;
        }
        yancha_user_t author = msg->author;
        printf("%ld %s[%s]:(%s|%s)\n%s\n----\n", msg->created_at_ms, author.key, author.nickname,
               author.profile_url, author.profile_image_url, msg->text);
    }

    yancha_cleanup(yancha);
}
