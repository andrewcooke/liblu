
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "../lib/lu.h"
#include "../lib/lustr.h"
#include "../lib/lulog.h"
#include "../lib/lugrey.h"
#include "../lib/lusimplex.h"


static void assert_constant(const char *name, const char *expr, double value, double target) {
    printf("%s = %s = %.*f\n", name, expr, DECIMAL_DIG, target);
    ck_assert(value == target);
}

START_TEST(test_constants) {
    int n_p, i, count[256] = {0};
    const uint8_t *p = NULL;
    double f2, g2, f4, g4;
    lusimplex_constants(&n_p, &p, &f2, &g2, &f4, &g4);
    ck_assert(n_p == 512);
    for (i = 0; i < n_p; ++i) count[p[i]] += 1;
    for (i = 0; i < 256; ++i) ck_assert_msg(count[i] == 2, "count at %d is %d", i, count[i]);
    assert_constant("F2", "0.5*(sqrt(3.0)-1.0)", f2, 0.5*(sqrt(3.0)-1.0));
    assert_constant("G2", "(3.0-sqrt(3.0))/6.0", g2, (3.0-sqrt(3.0))/6.0);
    assert_constant("F4", "(sqrt(5.0)-1.0)/4.0", f4, (sqrt(5.0)-1.0)/4.0);
    assert_constant("G4", "(5.0-sqrt(5.0))/20.0", g4, (5.0-sqrt(5.0))/20.0);
} END_TEST


static void assert_value(double x, double y, double target) {
    double value = lusimplex_noise2(x, y);
    ck_assert_msg(value == target, "%.*f != %.*f at (%0.1g, %0.1g)",
            DECIMAL_DIG, value, DECIMAL_DIG, target, x, y);
}

START_TEST(test_values) {
    assert_value(0, 0, 0.0);  // suspicious but stepping through seems ok
    assert_value(0.5, 0, -0.833975998411992436310);
    assert_value(0.5, 0.5, -0.307156513627216187778);
    assert_value(1, 1, -0.440267719075499441850);
} END_TEST


START_TEST(print_octave) {
    lustr s;
    lulog *log;
    int i, j, k, nx = 40, ny = 20, *grey = NULL;
    ck_assert(!lustr_initn(NULL, &s, (nx+1)*ny+1));
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    double x, y, data[nx*ny];
    memset(data, 0, sizeof(data));
    for (i = 0; i < nx; ++i) {
        for (j = 0; j < ny; ++j) {
            for (k = 0; k < 5; ++k) {
                x = (i + (100*k)) / pow(2, k);
                y = (j + (100*k)) / pow(2, k);
                data[i+j*nx] += lusimplex_noise2(x, y) * pow(2, k);
            }
        }
    }
    ck_assert(!lugrey_scale(log, data, nx*ny, 9, &grey));
    ck_assert(!lugrey_str(log, grey, nx, ny, " .:+*oO#@", &s));
    ck_assert_msg(strlen(s.c) == (nx+1) * ny, "length %zu != %d", strlen(s.c), (nx+1) * ny);
    printf(s.c);
    ck_assert(!lustr_free(log, &s, 0));
    ck_assert(!log->free(&log, 0));
    free(grey);
} END_TEST


int main(void) {

    int failed = 0;
    Suite *s;
    TCase *c;
    SRunner *r;

    c = tcase_create("case");
    tcase_add_test(c, test_constants);
    tcase_add_test(c, test_values);
    tcase_add_test(c, print_octave);
    s = suite_create("suite");
    suite_add_tcase(s, c);
    r = srunner_create(s);
    srunner_run_all(r, CK_VERBOSE);
    failed = srunner_ntests_failed(r);
    srunner_free(r);

    return failed ? 1 : 0;
}
