
#include <check.h>
#include <stdlib.h>

#include "../lib/lu.h"
#include "../lib/lutriplex.h"


int main(void) {

    int failed = 0;
    Suite *s;
    TCase *c;
    SRunner *r;

    c = tcase_create("case");
//    tcase_add_test(c, test_xoroshiro128plus);
    s = suite_create("suite");
    suite_add_tcase(s, c);
    r = srunner_create(s);
    srunner_run_all(r, CK_VERBOSE);
    failed = srunner_ntests_failed(r);
    srunner_free(r);

    return failed ? 1 : 0;
}
