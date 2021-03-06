
#include <check.h>
#include <stdlib.h>
#include <float.h>
#include <lib/lu/status.h>
#include <math.h>

#include "../lib/lu/greyscale.h"
#include "../lib/lu/tiles.h"
#include "../lib/lu/minmax.h"

//static char *scale1 = " .:+*oO#@";
static char *scale2 = " '.\",-=+*:;ijcoebmIJCOEBM#@";


START_TEST(test_config) {
    lulog *log;
    ck_assert(!lulog_stderr_mk(&log, lulog_level_debug));
    lutle_config *config;
    ck_assert(!lutle_config_default(log, &config, 0));
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
    ck_assert(!lutle_config_free(&config, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


START_TEST(test_triangle) {
    lulog *log;
    ck_assert(!lulog_stderr_mk(&log, lulog_level_debug));
    lutle_config *config;
    ck_assert(!lutle_config_default(log, &config, 0));
    lutle_tile *triangle;
    ck_assert(!lutle_triangle_mk(log, &triangle, 4, 9, 1.0));
    luary_ijz *ijz = NULL;
    ck_assert(!triangle->enumerate(triangle, log, config, -1, &ijz));
    size_t nx, ny; int *grey; double *data;
    ck_assert(!lutle_rasterize(log, ijz, &nx, &ny, &data));
    ck_assert(!lugry_quantize(log, data, nx*ny, 9, &grey));
    lustr s;
    ck_assert(!lustr_mkn(NULL, &s, (nx+1)*ny+1));
    ck_assert(!lugry_str(log, grey, nx, ny, " .:+*oO#@", &s));
    printf(s.c);
    ck_assert(!lustr_free(&s, 0));
    free(grey); free(data);
    ck_assert(!luary_ijz_free(&ijz, 0));
    ck_assert(!triangle->free(&triangle, 0));
    ck_assert(!lutle_config_free(&config, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


START_TEST(test_small_hexagon) {
    lulog *log;
    ck_assert(!lulog_stderr_mk(&log, lulog_level_debug));
    lutle_config *config;
    ck_assert(!lutle_config_default(log, &config, 0));
    lutle_tile *hexagon;
    ck_assert(!lutle_hexagon_mk(log, &hexagon, 1, 1, 1.0));
    luary_ijz *ijz = NULL;
    ck_assert(!hexagon->enumerate(hexagon, log, config, -1, &ijz));
    ck_assert_msg(ijz->mem.used == 7, "Expected 7 points, found %zu", ijz->mem.used);
    ck_assert(!luary_ijz_free(&ijz, 0));
    ck_assert(!hexagon->free(&hexagon, 0));
    ck_assert(!lutle_config_free(&config, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


START_TEST(test_medium_hexagon) {
    lulog *log;
    ck_assert(!lulog_stderr_mk(&log, lulog_level_debug));
    lutle_config *config;
    ck_assert(!lutle_config_default(log, &config, 0));
    lutle_tile *hexagon;
    ck_assert(!lutle_hexagon_mk(log, &hexagon, 2, 1, 1.0));
    luary_ijz *ijz = NULL;
    ck_assert(!hexagon->enumerate(hexagon, log, config, -1, &ijz));
    ck_assert_msg(ijz->mem.used == 19, "Expected 19 points, found %zu", ijz->mem.used);
    ck_assert(!luary_ijz_free(&ijz, 0));
    ck_assert(!hexagon->free(&hexagon, 0));
    ck_assert(!lutle_config_free(&config, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


START_TEST(test_large_hexagon) {
    lulog *log;
    ck_assert(!lulog_stderr_mk(&log, lulog_level_debug));
    lutle_config *config;
    ck_assert(!lutle_config_default(log, &config, 0));
    lutle_tile *hexagon;
    ck_assert(!lutle_hexagon_mk(log, &hexagon, 4, 13, 1));
    luary_ijz *ijz = NULL;
    ck_assert(!hexagon->enumerate(hexagon, log, config, -1, &ijz));
    size_t nx, ny; int *grey; double *data;
    ck_assert(!lutle_rasterize(log, ijz, &nx, &ny, &data));
    char *scale = scale2;
    ck_assert(!lugry_quantize(log, data, nx*ny, strlen(scale), &grey));
    lustr s;
    ck_assert(!lustr_mkn(NULL, &s, (nx+1)*ny+1));
    ck_assert(!lugry_str(log, grey, nx, ny, scale, &s));
    printf(s.c);
    ck_assert(!lustr_free(&s, 0));
    free(grey); free(data);
    ck_assert(!luary_ijz_free(&ijz, 0));
    ck_assert(!hexagon->free(&hexagon, 0));
    ck_assert(!lutle_config_free(&config, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


void append_hexagon(lulog *log, luary_ijz *tiled, luary_ijz *hexagon, int di, int dj) {
    for (size_t i = 0; i < hexagon->mem.used; ++i) {
        ludta_ijz ijz = hexagon->ijz[i];
        luary_ijz_push(log, tiled, ijz.i + di, ijz.j + dj, ijz.z);
    }
}

START_TEST(test_tiled_hexagon) {
    lulog *log;
    ck_assert(!lulog_stderr_mk(&log, lulog_level_debug));
    lutle_config *config;
    ck_assert(!lutle_config_default(log, &config, 0));
    lutle_tile *hexagon;
    size_t n = 3, m = 2;
    ck_assert(!lutle_hexagon_mk(log, &hexagon, n, m, 1.0));
    luary_ijz *ijz = NULL;
    ck_assert(!hexagon->enumerate(hexagon, log, config, 7, &ijz));
    luary_ijz *tiled = NULL;
    ck_assert(!luary_ijz_mk(log, &tiled, 0));
    append_hexagon(log, tiled, ijz, 0, 0);
    append_hexagon(log, tiled, ijz, n*m, n*m);
    append_hexagon(log, tiled, ijz, 2*n*m, -n*m);
//    append_hexagon(log, tiled, ijz, 3*n*m, 0);
    size_t nx, ny; int *grey; double *data;
    ck_assert(!lutle_rasterize(log, tiled, &nx, &ny, &data));
    char *scale = scale2;
    ck_assert(!lugry_quantize(log, data, nx*ny, strlen(scale), &grey));
    lustr s;
    ck_assert(!lustr_mkn(NULL, &s, (nx+1)*ny+1));
    ck_assert(!lugry_str(log, grey, nx, ny, scale, &s));
    printf(s.c);
    ck_assert(!lustr_free(&s, 0));
    free(grey); free(data);
    ck_assert(!luary_ijz_free(&tiled, 0));
    ck_assert(!luary_ijz_free(&ijz, 0));
    ck_assert(!hexagon->free(&hexagon, 0));
    ck_assert(!lutle_config_free(&config, 0));
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
    tcase_add_test(c, test_tiled_hexagon);
    tcase_add_test(c, test_large_hexagon);
    s = suite_create("suite");
    suite_add_tcase(s, c);
    r = srunner_create(s);
    srunner_run_all(r, CK_VERBOSE);
    failed = srunner_ntests_failed(r);
    srunner_free(r);

    return failed ? 1 : 0;
}
