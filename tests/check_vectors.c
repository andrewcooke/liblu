
#include <check.h>
#include <stdlib.h>

#include "../lib/lu/structs.h"
#include "../lib/lu/vectors.h"


START_TEST(test_basics) {
    char buf[100];
    ludata_fxyzw i = {1, 0, 0, 1};
    ludata_fxyzw j = {0, 1, 0, 1};
    ludata_fxyzw k = {0, 0, 1, 1};
    ck_assert(lueq4(j, luadd3(i, lusub3(j, i))));
    ck_assert(!lueq4(i, luadd3(i, i)));
    ck_assert(lueq4(i, lunorm3(luadd3(i, i))));
    ck_assert_msg(lueq4(lucross3(i, j), k), "%s", ludata_fxyzw2str(lucross3(i, j), 100, buf));
    ck_assert_msg(lueq4(lucross3(j, k), i), "%s", ludata_fxyzw2str(lucross3(j, k), 100, buf));
    ck_assert_msg(lueq4(lucross3(k, i), j), "%s", ludata_fxyzw2str(lucross3(k, i), 100, buf));
} END_TEST


START_TEST(test_vec) {
    luvec_f4 a = {1,2,3,1}, b = {5,5,5,1}, c = {}, d = {6,7,8,1}, *e = NULL;
    char buffer[100];
    luvec_addf4_3(&a, &b, &c);
    ck_assert_msg(luvec_eqf4(&c, &d), luvec_strf4(&c, 100, buffer));
    ck_assert(!luvec_copyf4(NULL, &d, &e));
    ck_assert_msg(luvec_eqf4(&c, e), luvec_strf4(e, 100, buffer));
    free(e);
    luvec_f4 x = {1,0,0,0}, y = {0,1,0,0}, z = {0,0,1,0};
    luvec_crsf4_3(&x, &y, &a);
    ck_assert(luvec_eqf4(&a, &z));
    float q = luvec_dotf4_3(&b, &y);
    ck_assert(q == 5);
} END_TEST


int main(void) {

    int failed = 0;
    Suite *s;
    TCase *c;
    SRunner *r;

    c = tcase_create("case");
    tcase_add_test(c, test_basics);
    tcase_add_test(c, test_vec);
    s = suite_create("suite");
    suite_add_tcase(s, c);
    r = srunner_create(s);
    srunner_run_all(r, CK_VERBOSE);
    failed = srunner_ntests_failed(r);
    srunner_free(r);

    return failed ? 1 : 0;
}
