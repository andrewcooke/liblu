
#include <check.h>
#include <stdlib.h>
#include <inttypes.h>

#include "../lib/lu.h"
#include "../lib/lurand.h"

void assert_next(lurand *rand, uint64_t target) {
    uint64_t value = rand->next(rand);
    ck_assert_msg(value == target, "%" PRId64 " != %" PRId64,
            value, target);
}

START_TEST(test_xoroshiro128plus) {
    lurand *rand;
    ck_assert(lurand_mkxoroshiro128plus(NULL, &rand, 0, 0));
    ck_assert(!lurand_mkxoroshiro128plus(NULL, &rand, 0, 1));
    assert_next(rand, 1);
    assert_next(rand, 68719493121l);
    assert_next(rand, 38280734540038433l);
    assert_next(rand, 1153018330890649890l);
    assert_next(rand, -7381291882501266943l);
    ck_assert(!rand->free(&rand, 0));
    ck_assert(!rand);
} END_TEST


int main(void) {

    int failed = 0;
    Suite *s;
    TCase *c;
    SRunner *r;

    c = tcase_create("case");
    tcase_add_test(c, test_xoroshiro128plus);
    s = suite_create("suite");
    suite_add_tcase(s, c);
    r = srunner_create(s);
    srunner_run_all(r, CK_VERBOSE);
    failed = srunner_ntests_failed(r);
    srunner_free(r);

    return failed ? 1 : 0;
}
