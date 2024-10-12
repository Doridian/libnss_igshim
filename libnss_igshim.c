#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#define PARENT_MODULE "kanidm"

enum nss_status
{
    NSS_STATUS_TRYAGAIN = -2,
    NSS_STATUS_UNAVAIL = -1,
    NSS_STATUS_NOTFOUND = 0,
    NSS_STATUS_SUCCESS = 1,
    NSS_STATUS_RETURN = 2
};

typedef enum nss_status t_mod_setgrent(void);
typedef enum nss_status t_mod_endgrent(void);
typedef enum nss_status t_mod_getgrent_r(struct group *gbuf, char* buf, size_t buflen, struct group **gbufp);
typedef enum nss_status t_mod_getpwnam_r(const char *name, struct passwd *pwd, char *buf, size_t buflen, struct passwd **result);

t_mod_setgrent *nss_mod_setgrent;
t_mod_endgrent *nss_mod_endgrent;
t_mod_getgrent_r *nss_mod_getgrent_r;
t_mod_getpwnam_r *nss_mod_getpwnam_r;

// get_dll from: https://github.com/pikhq/musl-nscd/blob/master/src/main.c
static void *get_dll(const char *service)
{
    char *path;
    void *dll;
    if(asprintf(&path, "libnss_%s.so.2", service) < 0) exit(1);
    dll = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if(!dll) {
        sprintf(path, "libnss_%s.so", service);
        dll = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    }
    if(!dll) {
        printf("%s: %s", path, dlerror());
        exit(1);
    }
    free(path);
    return dll;
}

// get_fn from: https://github.com/pikhq/musl-nscd/blob/master/src/main.c
static void *get_fn(void *dll, const char *name, const char *service)
{
    char *fnname;
    void *fn;
    if(asprintf(&fnname, "_nss_%s_%s", service, name) < 0) exit(1);
    fn = dlsym(dll, fnname);
    free(fnname);
    return fn;
}

__attribute__((constructor))
void igshim_module_init()
{
    void* dll;
    dll = get_dll(PARENT_MODULE);
    if (dll == NULL) {
        printf("[CRITICAL] dll is NULL\n");
        exit(1);
    }

    nss_mod_setgrent = (t_mod_setgrent*)get_fn(dll, "setgrent", PARENT_MODULE);
    if (nss_mod_setgrent == NULL) {
        printf("[CRITICAL] nss_mod_setgrent is NULL\n");
        exit(1);
    }

    nss_mod_endgrent = (t_mod_endgrent*)get_fn(dll, "endgrent", PARENT_MODULE);
    if (nss_mod_endgrent == NULL) {
        printf("[CRITICAL] nss_mod_endgrent is NULL\n");
        exit(1);
    }

    nss_mod_getgrent_r = (t_mod_getgrent_r*)get_fn(dll, "getgrent_r", PARENT_MODULE);
    if (nss_mod_getgrent_r == NULL) {
        printf("[CRITICAL] nss_mod_getgrent_r is NULL\n");
        exit(1);
    }

    nss_mod_getpwnam_r = (t_mod_getpwnam_r*)get_fn(dll, "getpwnam_r", PARENT_MODULE);
    if (nss_mod_getpwnam_r == NULL) {
        printf("[CRITICAL] nss_mod_getpwnam_r is NULL\n");
        exit(1);
    }
}

enum nss_status _nss_igshim_initgroups_dyn(const char* username, gid_t gid, long* start, long* size, gid_t** groupsp, long limit, int *errorp)
{
    if (gid != -1 || username == NULL) {
        return NSS_STATUS_NOTFOUND;
    }

    int res;
    char buf[4096];

    struct passwd pwd;
    struct passwd *pwdp;
    res = nss_mod_getpwnam_r(username, &pwd, buf, sizeof(buf), &pwdp);
    if (res != NSS_STATUS_SUCCESS) {
        *errorp = errno;
        return res;
    }


    struct group grp;
    struct group *grpp;
    long entries = 0;

    char **members;

    if (nss_mod_setgrent() != NSS_STATUS_SUCCESS) {
        goto initgroups_try_again;
    }

    while(entries < limit) {
        res = nss_mod_getgrent_r(&grp, buf, sizeof(buf), &grpp);
        if (res != NSS_STATUS_SUCCESS) {
            if (res == NSS_STATUS_NOTFOUND) {
                break;
            }
            goto initgroups_try_again;
        }


        if (*size == *start) {
            *size *= 2;
            *groupsp = realloc(*groupsp, *size * sizeof(gid_t));
            if (!*groupsp) {
                errno = ENOMEM;
                goto initgroups_try_again;
            }
        }

        for (members = grp.gr_mem; *members != NULL; members++) {
            if (strcmp(username, *members) == 0) {
                break;
            }
        }

        if (*members == NULL) {
            continue;
        }

        (*groupsp)[*start] = grp.gr_gid;
        (*start)++;
        entries++;
    }

    if (nss_mod_endgrent() != NSS_STATUS_SUCCESS) {
        goto initgroups_try_again;
    }
    return NSS_STATUS_SUCCESS;

initgroups_try_again:
    *errorp = errno;
    nss_mod_endgrent();
    return NSS_STATUS_TRYAGAIN;
}
