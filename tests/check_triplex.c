
#include <check.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#include "../lib/lu.h"
#include "../lib/lutriplex.h"


START_TEST(test_config) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    lutriplex_config *config;
    ck_assert(!lutriplex_default_config(log, &config));
    ck_assert(config->n_perm == 256);
    for (int i = 0; i < 256; ++i) {
        ck_assert(config->perm[i] >= 0);
        ck_assert(config->perm[i] < 256);
        ck_assert(config->perm[i] == config->perm[i+256]);
    }
    ck_assert_msg(config->perm[0] == 255, "%d", config->perm[0]);
    ck_assert_msg(config->perm[1] == 244, "%d", config->perm[1]);
    ck_assert(config->n_grad == 9);
    for (int i = 0; i < 9; ++i) {
        ck_assert(config->grad[i].x == cos(i * 2 * M_PI / 9));
        ck_assert(config->grad[i].y == sin(i * 2 * M_PI / 9));
        ck_assert(abs(1 - (config->grad[i].x * config->grad[i].x + config->grad[i].y * config->grad[i].y)) < 1e-8);
    }
    ck_assert_msg(config->grad[0].x == 1.0, "%.*f", DECIMAL_DIG, config->grad[0].x);
    ck_assert_msg(config->grad[0].y == 0.0, "%.*f", DECIMAL_DIG, config->grad[0].y);
    ck_assert_msg(config->grad[1].x == 0.766044443118978013452, "%.*f", DECIMAL_DIG, config->grad[1].x);
    ck_assert_msg(config->grad[1].y == 0.642787609686539251896, "%.*f", DECIMAL_DIG, config->grad[1].y);
    ck_assert(!lutriplex_free_config(&config, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


int main(void) {

    int failed = 0;
    Suite *s;
    TCase *c;
    SRunner *r;

    c = tcase_create("case");
    tcase_add_test(c, test_config);
    s = suite_create("suite");
    suite_add_tcase(s, c);
    r = srunner_create(s);
    srunner_run_all(r, CK_VERBOSE);
    failed = srunner_ntests_failed(r);
    srunner_free(r);

    return failed ? 1 : 0;
}
