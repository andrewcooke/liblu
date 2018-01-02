
#include <check.h>
#include <stdlib.h>
#include <math.h>

#include "../lib/lu/gl.h"


START_TEST(test_vec) {
    luglv a = {1,2,3,1}, b = {5,5,5,1}, c = {}, d = {6,7,8,1}, e = {};
    char buffer[100];
    luglv_add(&a, &b, &c);
    ck_assert_msg(luglv_eq(&c, &d), luglv_str(&c, 100, buffer));
    luglv_copy(&d, &e);
    ck_assert_msg(luglv_eq(&c, &e), luglv_str(&e, 100, buffer));
    luglv x = {1,0,0,1}, y = {0,1,0,1}, z = {0,0,1,1};
    luglv_cross(&x, &y, &a);
    ck_assert(luglv_eq(&a, &z));
    float q = luglv_dot(&b, &y);
    ck_assert(q == 5);
    luglv_zero(&a);
    luglv zero = {};
    ck_assert(luglv_eq(&a, &zero));
} END_TEST


START_TEST(test_mat) {
    luglm r = {};
    luglv x = {1,0,0,1}, v = {}, xy = {sqrt(2),sqrt(2),0,1};
    luglm_rotz(M_PI/4, &r);
    luglv_mult(&r, &x, &v);
    ck_assert(luglv_approx3(&v, &xy, 0.001));
} END_TEST


static void assert_inv(lulog *log, luglm *m, char *name) {
    luglm inv = {}, id1 = {}, id2 = {};
    ck_assert_msg(!lumat_inv(log, m, &inv), name);
    luglm_mult(&inv, m, &id1);
    luglm_id(&id2);
    ck_assert_msg(luglm_approx(&id1, &id2, 0.0001), name);
}

START_TEST(test_inv) {
    lulog *log;
    ck_assert(!lulog_stderr_mk(&log, lulog_level_debug));
    luglm m = {};
    luglm_id(&m);
    assert_inv(log, &m, "id");
    luglm_rotx(0.1, &m);
    assert_inv(log, &m, "rotn");
    lumat_log(log, lulog_level_info, &m);
    luglm_set(1,2,4,8,9,10,11,12,1,1,1,1,2,3,5,7,&m);
    assert_inv(log, &m, "count");
    ck_assert(!log->free(&log, 0));
} END_TEST

START_TEST(test_qua) {
    luglv i = {1,0,0,0}, j = {0,1,0,0}, jk = {sqrtf(2),sqrtf(2),0,0}, x = {};
    luglq q = {};
    luglq_rotx(M_PI/4, &q);
    luglv_rot(&q, &i, &x);
    ck_assert(luglv_approx(&i, &x, 0.001));  // i rotated about i is i
    luglv_rot(&q, &j, &x);
    ck_assert(luglv_approx(&jk, &x, 0.001));  // j rotated about i is jk
} END_TEST


int main(void) {

    int failed = 0;
    Suite *s;
    TCase *c;
    SRunner *r;

    c = tcase_create("case");
    tcase_add_test(c, test_vec);
    tcase_add_test(c, test_mat);
    tcase_add_test(c, test_inv);
    tcase_add_test(c, test_qua);
    s = suite_create("suite");
    suite_add_tcase(s, c);
    r = srunner_create(s);
    srunner_run_all(r, CK_VERBOSE);
    failed = srunner_ntests_failed(r);
    srunner_free(r);

    return failed ? 1 : 0;
}
