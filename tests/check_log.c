
#include <check.h>
#include <stdlib.h>

#include "../lib/lu/error_codes.h"
#include "../lib/lu/log.h"


START_TEST(test_stderr) {

    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_info));
    ck_assert(!ludebug(log, "this should not be displayed"));
    ck_assert(!luinfo(log, "this should be displayed"));
    ck_assert(!log->free(&log, LU_OK));
    ck_assert(!log);

} END_TEST

START_TEST(test_output) {

    lulog *log;
    FILE *stream = tmpfile();
    ck_assert(!lulog_mkstream(&log, stream, lulog_level_info, 0));
    ck_assert(!ludebug(log, "this should not be displayed"));
    ck_assert(!luinfo(log, "this should be displayed"));
    ck_assert(!log->free(&log, LU_OK));
    ck_assert(!log);
    rewind(stream);
    char *line = NULL;
    size_t n = 0;
    getline(&line, &n, stream);
    ck_assert_msg(!strcmp(line, "info: this should be displayed\n"),
            "bad output: '%s'", line);
    fclose(stream);
    free(line);

} END_TEST

START_TEST(test_string) {
    lulog *log;
    lustr *str;
    ck_assert(!lulog_mkstring(&log, &str, lulog_level_debug));
    ck_assert(!ludebug(log, "hello world"));
    ck_assert(!strcmp(str->c, "debug: hello world\n"));
    ck_assert(!log->free(&log, 0));
} END_TEST

START_TEST(test_syslog) {
    lulog *log;
    ck_assert(!lulog_mksyslog(&log, "lulib", lulog_level_debug));
    ck_assert(!ludebug(log, "test message"));
    // check that message appears in system log
    ck_assert(!log->free(&log, 0));
} END_TEST


int main(void) {

    int failed = 0;
    Suite *s;
    TCase *c;
    SRunner *r;

    c = tcase_create("case");
    tcase_add_test(c, test_stderr);
    tcase_add_test(c, test_output);
    tcase_add_test(c, test_string);
    tcase_add_test(c, test_syslog);
    s = suite_create("suite");
    suite_add_tcase(s, c);
    r = srunner_create(s);
    srunner_run_all(r, CK_VERBOSE);
    failed = srunner_ntests_failed(r);
    srunner_free(r);

    return failed ? 1 : 0;
}
