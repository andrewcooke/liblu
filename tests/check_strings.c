
#include <check.h>
#include <lib/lu/status_codes.h>
#include <stdlib.h>

#include "../lib/lu/strings.h"
#include "../lib/lu/minmax.h"


void assert_str(lustr str, const char *target) {
    ck_assert_msg(!strcmp(str.c, target), "'%s' != '%s'", str.c, target);
    ck_assert(str.mem.used == strlen(target) + 1);
    ck_assert(str.mem.capacity >= str.mem.used);
}

START_TEST(test_add) {

    lustr str = {};

    ck_assert(!lustr_mk(NULL, &str));
    ck_assert(!lustr_add(NULL, &str, 'a'));
    ck_assert(!lustr_add(NULL, &str, 'b'));
    ck_assert(!lustr_add(NULL, &str, 'c'));
    assert_str(str, "abc");
    ck_assert(!lustr_free(&str, 0));

} END_TEST

START_TEST(test_print) {

    lustr str;

    ck_assert(!lustr_mk(NULL, &str));
    assert_str(str, "");
    ck_assert(!lustr_print(NULL, &str, "abc"));
    assert_str(str, "abc");
    ck_assert(!lustr_print(NULL, &str, "def"));
    assert_str(str, "def");
    ck_assert(!lustr_free(&str, 0));

} END_TEST

START_TEST(test_scaling) {

    lustr str;

    ck_assert(!lustr_mk(NULL, &str));
    ck_assert(!lustr_print(NULL, &str, ""));
    ck_assert_msg(str.mem.capacity == 1, "%d != %zu", 1, str.mem.capacity);
    ck_assert(!lustr_print(NULL, &str, "a"));
    ck_assert_msg(str.mem.capacity == 2, "%d != %zu", 2, str.mem.capacity);
    ck_assert(!lustr_print(NULL, &str, "aa"));
    ck_assert_msg(str.mem.capacity == 4, "%d != %zu", 4, str.mem.capacity);
    ck_assert(!lustr_print(NULL, &str, "aaa"));
    ck_assert_msg(str.mem.capacity == 4, "%d != %zu", 4, str.mem.capacity);
    ck_assert(!lustr_print(NULL, &str, "aaaa"));
    ck_assert_msg(str.mem.capacity == 8, "%d != %zu", 8, str.mem.capacity);
    ck_assert(!lustr_free(&str, 0));

} END_TEST

void max_size_test(size_t max_size, size_t chars) {

    lustr str1, str2;
    size_t i;
    int n;

    ck_assert(!lustr_mk(NULL, &str1));
    ck_assert(!lustr_mk(NULL, &str2));
    for (i = 0; i < chars; ++i) {
        ck_assert(!lustr_append(NULL, &str2, "x"));
    }
    ck_assert(strlen(str2.c) == chars);

    ck_assert(!lustr_nprint(NULL, &str1, max_size, &n, str2.c));
    ck_assert_msg(strlen(str1.c) == min(max_size, chars), "'%s' %d -> '%s'",
            str2.c, max_size, str1.c);
    ck_assert(str1.mem.used * 2 >= str1.mem.capacity);
    ck_assert(n == chars);

    ck_assert(!lustr_free(&str1, 0));
    ck_assert(!lustr_free(&str2, 0));
}

START_TEST(test_max_size) {

    int i, j;

    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 5; ++j) {
            max_size_test(i, j);
        }
    }

} END_TEST

START_TEST(test_append) {

    lustr str;

    ck_assert(!lustr_mk(NULL, &str));
    assert_str(str, "");
    ck_assert(!lustr_append(NULL, &str, "abc"));
    assert_str(str, "abc");
    ck_assert(!lustr_append(NULL, &str, "def"));
    assert_str(str, "abcdef");
    ck_assert(!lustr_free(&str, 0));

} END_TEST

START_TEST(test_format) {

    lustr str;

    ck_assert(!lustr_mk(NULL, &str));
    assert_str(str, "");
    ck_assert(!lustr_appendf(NULL, &str, "%s", "abc"));
    assert_str(str, "abc");
    ck_assert(!lustr_appendf(NULL, &str, "%d", 123));
    assert_str(str, "abc123");
    ck_assert(!lustr_free(&str, 0));

} END_TEST

int main(void) {

    int failed = 0;
    Suite *s;
    TCase *c;
    SRunner *r;

    c = tcase_create("case");
    tcase_add_test(c, test_add);
    tcase_add_test(c, test_print);
    tcase_add_test(c, test_scaling);
    tcase_add_test(c, test_max_size);
    tcase_add_test(c, test_append);
    tcase_add_test(c, test_format);
    s = suite_create("suite");
    suite_add_tcase(s, c);
    r = srunner_create(s);
    srunner_run_all(r, CK_VERBOSE);
    failed = srunner_ntests_failed(r);
    srunner_free(r);

    return failed ? 1 : 0;
}
