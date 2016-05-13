
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
    ck_assert(!lutriplex_mktriangle(log, &triangle, 4, 9, 1.0));
    luarray_ijz *ijz = NULL;
    ck_assert(!triangle->enumerate(triangle, log, config, -1, &ijz));
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


START_TEST(test_small_hexagon) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    lutriplex_config *config;
    ck_assert(!lutriplex_defaultconfig(log, &config));
    lutriplex_tile *hexagon;
    ck_assert(!lutriplex_mkhexagon(log, &hexagon, 1, 1, 1.0));
    luarray_ijz *ijz = NULL;
    ck_assert(!hexagon->enumerate(hexagon, log, config, -1, &ijz));
    for (size_t i = 0; i < ijz->mem.used; ++i) {
//        ludebug(log, "%zu: (%d, %d)", i, ijz->ijz[i].i, ijz->ijz[i].j);
    }
    ck_assert_msg(ijz->mem.used == 7, "Expected 7 points, found %zu", ijz->mem.used);
    luarray_xyz *strips = NULL;
    luarray_int *offsets = NULL;
    ck_assert(!lutriplex_strips(log, ijz, &strips, &offsets));
    ck_assert(offsets->mem.used == 3);
    ck_assert(offsets->i[0] == 0);
    ck_assert(offsets->i[1] == 5);
    ck_assert(offsets->i[2] == 10);
    ck_assert(!luarray_freeint(&offsets, 0));
    ck_assert(!luarray_freexyz(&strips, 0));
    ck_assert(!luarray_freeijz(&ijz, 0));
    ck_assert(!hexagon->free(&hexagon, 0));
    ck_assert(!lutriplex_freeconfig(&config, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


START_TEST(test_medium_hexagon) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    lutriplex_config *config;
    ck_assert(!lutriplex_defaultconfig(log, &config));
    lutriplex_tile *hexagon;
    ck_assert(!lutriplex_mkhexagon(log, &hexagon, 2, 1, 1.0));
    luarray_ijz *ijz = NULL;
    ck_assert(!hexagon->enumerate(hexagon, log, config, -1, &ijz));
    for (size_t i = 0; i < ijz->mem.used; ++i) {
//        ludebug(log, "%zu: (%d, %d)", i, ijz->ijz[i].i, ijz->ijz[i].j);
    }
    ck_assert_msg(ijz->mem.used == 19, "Expected 19 points, found %zu", ijz->mem.used);
    luarray_xyz *strips = NULL;
    luarray_int *offsets = NULL;
    ck_assert(!lutriplex_strips(log, ijz, &strips, &offsets));
    ck_assert(offsets->mem.used == 5);
    ck_assert(offsets->i[0] == 0);
    ck_assert(offsets->i[1] == 7);
    ck_assert(offsets->i[2] == 16);
    ck_assert(offsets->i[3] == 25);
    ck_assert(offsets->i[4] == 32);
    ck_assert(!luarray_freeint(&offsets, 0));
    ck_assert(!luarray_freexyz(&strips, 0));
    ck_assert(!luarray_freeijz(&ijz, 0));
    ck_assert(!hexagon->free(&hexagon, 0));
    ck_assert(!lutriplex_freeconfig(&config, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


START_TEST(test_large_hexagon) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    lutriplex_config *config;
    ck_assert(!lutriplex_defaultconfig(log, &config));
    lutriplex_tile *hexagon;
    ck_assert(!lutriplex_mkhexagon(log, &hexagon, 4, 13, 1));
    luarray_ijz *ijz = NULL;
    ck_assert(!hexagon->enumerate(hexagon, log, config, -1, &ijz));
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
    ck_assert(!hexagon->free(&hexagon, 0));
    ck_assert(!lutriplex_freeconfig(&config, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


void append_hexagon(lulog *log, luarray_ijz *tiled, luarray_ijz *hexagon, int di, int dj) {
    for (size_t i = 0; i < hexagon->mem.used; ++i) {
        ludata_ijz ijz = hexagon->ijz[i];
        luarray_pushijz(log, tiled, ijz.i + di, ijz.j + dj, ijz.z);
    }
}

START_TEST(test_tiled_hexagon) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    lutriplex_config *config;
    ck_assert(!lutriplex_defaultconfig(log, &config));
    lutriplex_tile *hexagon;
    size_t n = 1;
    ck_assert(!lutriplex_mkhexagon(log, &hexagon, n, n, 1.0));
    luarray_ijz *ijz = NULL;
    ck_assert(!hexagon->enumerate(hexagon, log, config, 7, &ijz));
    luarray_ijz *tiled = NULL;
    ck_assert(!luarray_mkijzn(log, &tiled, 0));
    append_hexagon(log, tiled, ijz, 0, 0);
    append_hexagon(log, tiled, ijz, 4*n, 4*n);
    append_hexagon(log, tiled, ijz, 8*n, -4*n);
    append_hexagon(log, tiled, ijz, 12*n, 0);
    size_t nx, ny; int *grey; double *data;
    ck_assert(!lutriplex_rasterize(log, tiled, &nx, &ny, &data));
    ck_assert(!lugrey_quantize(log, data, nx*ny, 9, &grey));
    lustr s;
    ck_assert(!lustr_mkn(NULL, &s, (nx+1)*ny+1));
    ck_assert(!lugrey_str(log, grey, nx, ny, " .:+*oO#@", &s));
    printf(s.c);
    ck_assert(!lustr_free(&s, 0));
    free(grey); free(data);
    ck_assert(!luarray_freeijz(&tiled, 0));
    ck_assert(!luarray_freeijz(&ijz, 0));
    ck_assert(!hexagon->free(&hexagon, 0));
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
    tcase_add_test(c, test_small_hexagon);
    tcase_add_test(c, test_medium_hexagon);
    tcase_add_test(c, test_large_hexagon);
    tcase_add_test(c, test_tiled_hexagon);
    s = suite_create("suite");
    suite_add_tcase(s, c);
    r = srunner_create(s);
    srunner_run_all(r, CK_VERBOSE);
    failed = srunner_ntests_failed(r);
    srunner_free(r);

    return failed ? 1 : 0;
}
