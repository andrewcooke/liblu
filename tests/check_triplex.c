
#include <check.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#include "../lib/lu.h"
#include "../lib/luarray.h"
#include "../lib/lugrey.h"
#include "../lib/lutriplex.h"


START_TEST(test_config) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    lutriplex_config *config;
    ck_assert(!lutriplex_defaultconfig(log, &config));
    ck_assert(config->n_perm == 256);
    for (int i = 0; i < 256; ++i) {
        ck_assert(config->perm[i] < 256);
        ck_assert(config->perm[i] == config->perm[i+256]);
    }
    ck_assert_msg(config->perm[0] == 255, "%d", config->perm[0]);
    ck_assert_msg(config->perm[1] == 244, "%d", config->perm[1]);
    ck_assert(config->n_grad == 12);
    for (int i = 0; i < 12; ++i) {
        ck_assert(config->grad[i].x == cos(i * 2 * M_PI / 12));
        ck_assert(config->grad[i].y == sin(i * 2 * M_PI / 12));
        ck_assert(abs(1 - (config->grad[i].x * config->grad[i].x + config->grad[i].y * config->grad[i].y)) < 1e-8);
    }
    ck_assert_msg(config->grad[0].x == 1.0, "%.*f", DECIMAL_DIG, config->grad[0].x);
    ck_assert_msg(config->grad[0].y == 0.0, "%.*f", DECIMAL_DIG, config->grad[0].y);
    ck_assert_msg(config->grad[1].x == 0.866025403784438707611, "%.*f", DECIMAL_DIG, config->grad[1].x);
    ck_assert_msg(config->grad[1].y == 0.499999999999999944489, "%.*f", DECIMAL_DIG, config->grad[1].y);
    ck_assert(!lutriplex_freeconfig(&config, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


START_TEST(test_triangle) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    lutriplex_config *config;
    ck_assert(!lutriplex_defaultconfig(log, &config));
    lutriplex_tile *triangle;
    ck_assert(!lutriplex_mktriangle(log, &triangle, 4, 10));
    luarray_ijz *ijz = NULL;
    ck_assert(!triangle->enumerate(triangle, log, config, (ludata_ij){0, 0}, -1, &ijz));
    size_t nx, ny; int *grey; double *data;
    ck_assert(!lutriplex_rasterize(log, ijz, &nx, &ny, &data));
    ck_assert(!lugrey_quantize(log, data, nx*ny, 9, &grey));
    lustr s;
    ck_assert(!lustr_mkn(NULL, &s, (nx+1)*ny+1));
    ck_assert(!lugrey_str(log, grey, nx, ny, " .:+*oO#@", &s));
    printf(s.c);
    ck_assert(!lustr_free(&s, 0));
    free(grey); free(data);
    ck_assert(!luarray_freeijz(&ijz, 0));
    ck_assert(!triangle->free(&triangle, 0));
    ck_assert(!lutriplex_freeconfig(&config, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


int main(void) {

    int failed = 0;
    Suite *s;
    TCase *c;
    SRunner *r;

    c = tcase_create("case");
    tcase_add_test(c, test_config);
    tcase_add_test(c, test_triangle);
    s = suite_create("suite");
    suite_add_tcase(s, c);
    r = srunner_create(s);
    srunner_run_all(r, CK_VERBOSE);
    failed = srunner_ntests_failed(r);
    srunner_free(r);

    return failed ? 1 : 0;
}
