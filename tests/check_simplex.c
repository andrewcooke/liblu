
#include <check.h>
#include <stdlib.h>

#include "../lib/lu.h"
#include "../lib/lusimplex.h"


START_TEST(test_constants) {
    int n_p, i, count[256] = {0};
    uint8_t *p = NULL;
    double f2, g2, f4, g4;
    lusimplex_constants(&n_p, &p, &f2, &g2, &f4, &g4);
    ck_assert(n_p == 512);
    for (i = 0; i < n_p; ++i) count[p[i]] += 1;
    for (i = 0; i < 256; ++i) ck_assert_msg(count[i] == 2, "count at %d is %d", i, count[i]);
} END_TEST


int main(void) {

    int failed = 0;
    Suite *s;
    TCase *c;
    SRunner *r;

    c = tcase_create("case");
    tcase_add_test(c, test_constants);
    s = suite_create("suite");
    suite_add_tcase(s, c);
    r = srunner_create(s);
    srunner_run_all(r, CK_VERBOSE);
    failed = srunner_ntests_failed(r);
    srunner_free(r);

    return failed ? 1 : 0;
}
