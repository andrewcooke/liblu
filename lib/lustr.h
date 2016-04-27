
#ifndef LU_STRING_H
#define LU_STRING_H

typedef struct lustr {
	char *c;
	size_t nalloc;
} lustr;

int lustr_mknew(lulog *log, lustr *str);
int lustr_mkstr(lulog *log, lustr *str, const char *c);
int lustr_free(struct lustr str, int status);

int lustr_append(struct lustr *str, const char *c);

#endif
