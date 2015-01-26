#include <stdio.h>
#include <string.h>
#include "picotest.h"
#include "test.h"
#include "yancha.h"

void yancha_test_client() {
    char *name = "taro";

    yancha_connection_t connection = yancha_init("http://127.0.0.1:3000");
    ok(connection.token == NULL);

    yancha_login(&connection, name);
    ok(strlen(connection.token) > 0);
    ok(!strcmp(connection.nickname, name));

    yancha_post(&connection, "Hello!");

    fprintf(stderr, "TODO:(@ichigotake) No implements: yancha_search\n");
    ok(0);

    fprintf(stderr, "TODO:(@ichigotake) No implements: yancha_login_users\n");
    ok(0);
}
