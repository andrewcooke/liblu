
#include <check.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#include "../lib/lu/error_codes.h"
#include "../lib/lu/arrays.h"
#include "../lib/lu/greyscale.h"
#include "../lib/lu/tiles.h"
#include "../lib/lu/minmax.h"

//static char *scale1 = " .:+*oO#@";
static char *scale2 = " '.\",-=+*:;ijcoebmIJCOEBM#@";


START_TEST(test_config) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    lutle_config *config;
    ck_assert(!lutle_defaultconfig(log, &config, 0));
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
    ck_assert(!lutle_freeconfig(&config, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


START_TEST(test_triangle) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    lutle_config *config;
    ck_assert(!lutle_defaultconfig(log, &config, 0));
    lutle_tile *triangle;
    ck_assert(!lutle_mktriangle(log, &triangle, 4, 9, 1.0));
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
    ck_assert(!luary_freeijz(&ijz, 0));
    ck_assert(!triangle->free(&triangle, 0));
    ck_assert(!lutle_freeconfig(&config, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


START_TEST(test_small_hexagon) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    lutle_config *config;
    ck_assert(!lutle_defaultconfig(log, &config, 0));
    lutle_tile *hexagon;
    ck_assert(!lutle_mkhexagon(log, &hexagon, 1, 1, 1.0));
    luary_ijz *ijz = NULL;
    ck_assert(!hexagon->enumerate(hexagon, log, config, -1, &ijz));
    for (size_t i = 0; i < ijz->mem.used; ++i) {
//        ludebug(log, "%zu: (%d, %d)", i, ijz->ijz[i].i, ijz->ijz[i].j);
    }
    ck_assert_msg(ijz->mem.used == 7, "Expected 7 points, found %zu", ijz->mem.used);
//    luary_uint32 *indices = NULL;
//    luary_uint32 *offsets = NULL;
//    ck_assert(!lutle_strips(log, ijz, &indices, &offsets));
//    ck_assert(offsets->mem.used == 3);
//    ck_assert(offsets->i[0] == 0);
//    ck_assert(offsets->i[1] == 5);
//    ck_assert(offsets->i[2] == 10);
//    ck_assert_msg(indices->i[0] == 2, "index 0 == %d", indices->i[0]);
//    ck_assert_msg(indices->i[1] == 0, "index 1 == %d", indices->i[1]);
//    ck_assert_msg(indices->i[2] == 3, "index 2 == %d", indices->i[2]);
//    ck_assert_msg(indices->i[3] == 1, "index 3 == %d", indices->i[2]);
//    ck_assert_msg(indices->i[4] == 4, "index 4 == %d", indices->i[2]);
//    ck_assert_msg(indices->i[5] == 2, "index 5 == %d", indices->i[2]);
//    ck_assert_msg(indices->i[6] == 5, "index 6 == %d", indices->i[2]);
//    ck_assert_msg(indices->i[7] == 3, "index 7 == %d", indices->i[2]);
//    ck_assert_msg(indices->i[8] == 6, "index 8 == %d", indices->i[2]);
//    ck_assert_msg(indices->i[9] == 4, "index 9 == %d", indices->i[2]);
//    ck_assert(!luary_freeuint32(&offsets, 0));
//    ck_assert(!luary_freeuint32(&indices, 0));
    ck_assert(!luary_freeijz(&ijz, 0));
    ck_assert(!hexagon->free(&hexagon, 0));
    ck_assert(!lutle_freeconfig(&config, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


START_TEST(test_medium_hexagon) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    lutle_config *config;
    ck_assert(!lutle_defaultconfig(log, &config, 0));
    lutle_tile *hexagon;
    ck_assert(!lutle_mkhexagon(log, &hexagon, 2, 1, 1.0));
    luary_ijz *ijz = NULL;
    ck_assert(!hexagon->enumerate(hexagon, log, config, -1, &ijz));
    for (size_t i = 0; i < ijz->mem.used; ++i) {
//        ludebug(log, "%zu: (%d, %d)", i, ijz->ijz[i].i, ijz->ijz[i].j);
    }
    ck_assert_msg(ijz->mem.used == 19, "Expected 19 points, found %zu", ijz->mem.used);
//    luary_uint32 *offsets = NULL;
//    luary_uint32 *indices = NULL;
//    ck_assert(!lutle_strips(log, ijz, &indices, &offsets));
//    ck_assert(offsets->mem.used == 5);
//    ck_assert(offsets->i[0] == 0);
//    ck_assert(offsets->i[1] == 7);
//    ck_assert(offsets->i[2] == 16);
//    ck_assert(offsets->i[3] == 25);
//    ck_assert(offsets->i[4] == 32);
//    ck_assert(!luary_freeuint32(&offsets, 0));
//    ck_assert(!luary_freeuint32(&indices, 0));
    ck_assert(!luary_freeijz(&ijz, 0));
    ck_assert(!hexagon->free(&hexagon, 0));
    ck_assert(!lutle_freeconfig(&config, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


START_TEST(test_large_hexagon) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    lutle_config *config;
    ck_assert(!lutle_defaultconfig(log, &config, 0));
    lutle_tile *hexagon;
    ck_assert(!lutle_mkhexagon(log, &hexagon, 4, 13, 1));
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
    ck_assert(!luary_freeijz(&ijz, 0));
    ck_assert(!hexagon->free(&hexagon, 0));
    ck_assert(!lutle_freeconfig(&config, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


void append_hexagon(lulog *log, luary_ijz *tiled, luary_ijz *hexagon, int di, int dj) {
    for (size_t i = 0; i < hexagon->mem.used; ++i) {
        ludta_ijz ijz = hexagon->ijz[i];
        luary_pushijz(log, tiled, ijz.i + di, ijz.j + dj, ijz.z);
    }
}

START_TEST(test_tiled_hexagon) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    lutle_config *config;
    ck_assert(!lutle_defaultconfig(log, &config, 0));
    lutle_tile *hexagon;
    size_t n = 3, m = 2;
    ck_assert(!lutle_mkhexagon(log, &hexagon, n, m, 1.0));
    luary_ijz *ijz = NULL;
    ck_assert(!hexagon->enumerate(hexagon, log, config, 7, &ijz));
    luary_ijz *tiled = NULL;
    ck_assert(!luary_mkijzn(log, &tiled, 0));
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
    ck_assert(!luary_freeijz(&tiled, 0));
    ck_assert(!luary_freeijz(&ijz, 0));
    ck_assert(!hexagon->free(&hexagon, 0));
    ck_assert(!lutle_freeconfig(&config, 0));
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
