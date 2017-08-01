
#include <check.h>
#include <stdlib.h>
#include <inttypes.h>
#include <float.h>
#include <lib/lu/status_codes.h>

#include "../lib/lu/random.h"


void assert_next(luran *rand, uint64_t target) {
    uint64_t value = rand->next(rand);
    ck_assert_msg(value == target, "%" PRIu64 " != %" PRIu64,
            value, target);
}

void assert_double(luran *rand, double target) {
    double value = luran_double(rand);
    ck_assert_msg(value == target, "%.*f != %.*f",
            DECIMAL_DIG, value, DECIMAL_DIG, target);
}

START_TEST(test_xoroshiro128plus) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    luran *rand;
    ck_assert(!luran_mkxoroshiro128plus(log, &rand, 0));
    assert_next(rand, UINT64_C(5807750865143411619));
    assert_next(rand, UINT64_C(38375600193489914));
    assert_next(rand, UINT64_C(1180499099402622421));
    assert_double(rand, 0.248228328440419643286);
    assert_double(rand, 0.378332628848798702137);
    assert_double(rand, 0.838702161674398150026);
    ck_assert(!rand->free(&rand, 0));
    ck_assert(!rand);
    ck_assert(!log->free(&log, 0));
    ck_assert(!log);
} END_TEST


void assert_rangeu(luran *rand, uint64_t lo, uint64_t hi, int n) {
    int i, hit_lo = 0, hit_hi = 0;
    for (i = 0; i < n; ++i) {
        uint64_t value = luran_uint64_range(rand, lo, hi);
        ck_assert_msg(lo <= value && hi >= value, "%" PRIu64, value);
        hit_lo |= lo == value;
        hit_hi |= hi == value;
    }
    ck_assert(hit_lo);
    ck_assert(hit_hi);
}

START_TEST(test_rangeu) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    luran *rand;
    ck_assert(!luran_mkxoroshiro128plus(log, &rand, 0));
    assert_rangeu(rand, 1234, 5678, 10000);
    ck_assert(!rand->free(&rand, 0));
    ck_assert(!rand);
    ck_assert(!log->free(&log, 0));
    ck_assert(!log);
} END_TEST


void assert_signu(uint64_t lo, uint64_t hi) {
    uint64_t u;
    for (u = lo; u <= hi; ++u) {
        int64_t n = luran_add_sign(u);
        ck_assert_msg(luran_remove_sign(n) == u,
                "Bad conversion from %" PRIu64 " to %" PRId64, u, n);
        if (u == hi) break;  // avoid overflow nasties
    }
}

void assert_signn(int64_t lo, int64_t hi) {
    int64_t n;
    for (n = lo; n <= hi; ++n) {
        int64_t u = luran_remove_sign(n);
        ck_assert_msg(luran_add_sign(u) == n,
                "Bad conversion from %" PRId64 " to %" PRIu64, n, u);
        if (n == hi) break;  // avoid overflow nasties
    }
}

START_TEST(test_sign) {
    assert_signu(UINT64_C(0), UINT64_C(3));
    assert_signu(UINT64_MAX - UINT64_C(3), UINT64_MAX);
    assert_signn(INT64_MIN, INT64_MIN+3);
    assert_signn(-2, 2);
    assert_signn(INT64_MAX-3, INT64_MAX);
} END_TEST


void assert_rangen(luran *rand, int64_t lo, int64_t hi, int n) {
    int i, hit_lo = 0, hit_hi = 0;
    for (i = 0; i < n; ++i) {
        int64_t value = luran_int64_range(rand, lo, hi);
        ck_assert_msg(lo <= value && hi >= value, "%" PRId64, value);
        hit_lo |= lo == value;
        hit_hi |= hi == value;
    }
    ck_assert(hit_lo);
    ck_assert(hit_hi);
}

START_TEST(test_rangen) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    luran *rand;
    ck_assert(!luran_mkxoroshiro128plus(log, &rand, 0));
    assert_rangen(rand, -5678, -1234, 10000);
    ck_assert(!rand->free(&rand, 0));
    ck_assert(!rand);
    ck_assert(!log->free(&log, 0));
    ck_assert(!log);
} END_TEST


START_TEST(test_shuffle) {
    int data1[] = {1,2,3}, target1[] = {2,3,1}, i;
    char data2[] = "hello world", target2[] = "dol roewllh";
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    luran *rand;
    ck_assert(!luran_mkxoroshiro128plus(log, &rand, 0));

    ck_assert(!luran_shuffle(log, rand, data1, sizeof(int), 3));
//    for (i = 0; i < 3; ++i) printf("%d\n", data1[i]);
    ck_assert(!memcmp(data1, target1, sizeof(target1)));

    ck_assert(!luran_shuffle(log, rand, data2, 1, strlen(data2)));
//    for (i = 0; i < strlen(data2); ++i) printf("%c\n", data2[i]);
    ck_assert(!memcmp(data2, target2, strlen(target2)));

    ck_assert(!rand->free(&rand, 0));
    ck_assert(!rand);
    ck_assert(!log->free(&log, 0));
    ck_assert(!log);
} END_TEST


int main(void) {

    int failed = 0;
    Suite *s;
    TCase *c;
    SRunner *r;

    c = tcase_create("case");
    tcase_add_test(c, test_xoroshiro128plus);
    tcase_add_test(c, test_rangeu);
    tcase_add_test(c, test_sign);
    tcase_add_test(c, test_rangen);
    tcase_add_test(c, test_shuffle);
    s = suite_create("suite");
    suite_add_tcase(s, c);
    r = srunner_create(s);
    srunner_run_all(r, CK_VERBOSE);
    failed = srunner_ntests_failed(r);
    srunner_free(r);

    return failed ? 1 : 0;
}
