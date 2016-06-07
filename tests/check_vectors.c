
#include <check.h>
#include <stdlib.h>
#include <math.h>

#include "../lib/lu/structs.h"
#include "../lib/lu/vectors.h"


START_TEST(test_vec) {
    luvec_f4 a = {1,2,3,1}, b = {5,5,5,1}, c = {}, d = {6,7,8,1}, e = {};
    char buffer[100];
    luvec_addf4_3(&a, &b, &c);
    ck_assert_msg(luvec_eqf4(&c, &d), luvec_strf4(&c, 100, buffer));
    luvec_cpyf4(&d, &e);
    ck_assert_msg(luvec_eqf4(&c, &e), luvec_strf4(&e, 100, buffer));
    luvec_f4 x = {1,0,0,1}, y = {0,1,0,1}, z = {0,0,1,1};
    luvec_crsf4_3(&x, &y, &a);
    ck_assert(luvec_eqf4(&a, &z));
    float q = luvec_dotf4_3(&b, &y);
    ck_assert(q == 5);
    luvec_zrof4(&a);
    luvec_f4 zero = {};
    ck_assert(luvec_eqf4(&a, &zero));
} END_TEST


START_TEST(test_mat) {
    lumat_f4 r = {};
    luvec_f4 x = {1,0,0,1}, v = {}, xy = {sqrt(2),sqrt(2),0,1};
    lumat_rotf4_z(M_PI/4, &r);
    luvec_mulf4(&r, &x, &v);
    ck_assert(luvec_apxf4_3(&v, &xy, 0.001));
} END_TEST


static void assert_inv(lulog *log, lumat_f4 *m, char *name) {
    lumat_f4 inv = {}, id1 = {}, id2 = {};
    ck_assert_msg(!lumat_invf4(log, m, &inv), name);
    lumat_mulf4(&inv, m, &id1);
    lumat_idnf4(&id2);
    ck_assert_msg(lumat_apxf4(&id1, &id2, 0.0001), name);
}

START_TEST(test_inv) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    lumat_f4 m = {};
    lumat_idnf4(&m);
    assert_inv(log, &m, "id");
    lumat_rotf4_x(0.1, &m);
    assert_inv(log, &m, "rotn");
    lumat_setf4(1,2,4,8,9,10,11,12,1,1,1,1,2,3,5,7,&m);
    assert_inv(log, &m, "count");
    ck_assert(!log->free(&log, 0));
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
    s = suite_create("suite");
    suite_add_tcase(s, c);
    r = srunner_create(s);
    srunner_run_all(r, CK_VERBOSE);
    failed = srunner_ntests_failed(r);
    srunner_free(r);

    return failed ? 1 : 0;
}
