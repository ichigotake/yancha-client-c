#include "picotest.h"
#include "test.h"

// before run yancha server on http://localhost:3000
int main(int argc, char *argv[]) {
    subtest("yancha/client", yancha_test_client);
    return done_testing();
}
