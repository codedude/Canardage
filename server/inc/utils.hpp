/*
    utils.h : set of utilities routines
    Must be included in each file
*/

#ifndef UTILS__H
    #define UTILS__H

    #include <cstdio>
    #include <string>


namespace Utils {
    /* Functions */
    void memset_s(void *ptr, int v, size_t n);
    int getline_s(char* s, std::streamsize n);

    /* Characters tests */
    inline bool isspace(char c) {
        return (c == ' ');
    }
    inline bool istab(char c) {
        return (c == '\t');
    }
    inline bool isvtab(char c) {
        return (c == '\v');
    }
    inline bool isnl(char c) {
        return (c == '\n');
    }
    inline bool iscr(char c) {
        return (c == '\r');
    }
    inline bool isdigit(char c) {
        return (c >= '0' && c <= '9');
    }
    inline bool isalpha(char c) {
        return (isupper(c) || islower(c));
    }
    inline bool isupper(char c) {
        return (c >= 'A' && c <= 'Z');
    }
    inline bool islower(char c) {
        return (c >= 'a' && c <= 'z');
    }
    inline bool isalnum(char c) {
        return (isalpha(c) || isdigit(c));
    }
    inline bool isblank(char c) {
        return (isspace(c) || isnl(c) || istab(c)
            || isvtab(c)|| iscr(c));
    }
}

    /* Return of functions */
    #define SUCCESS (0)
    #define ERROR (-1)
    #define NULL_ERROR (NULL)


    /* Debug macros
        One macro for variadic and one for no args => g++ warnings
    */
    #define RC putchar('\n');

    #ifdef NDEBUG
        #define log_errs(M, ...) fprintf(stderr, \
            "\033[31m[ERROR] " M "\033[0m\n", ##__VA_ARGS__)
        #define log_err(M) fprintf(stderr, "\033[31m[ERROR] " M "\033[0m\n")
    #else
        #define log_errs(M, ...) fprintf(stderr, \
            "\033[31m[ERROR] (%s:%d) : " M "\033[0m\n", \
            __FILE__, __LINE__, ##__VA_ARGS__)
        #define log_err(M) fprintf(stderr, \
            "\033[31m[ERROR] (%s:%d) : " M "\033[0m\n", \
            __FILE__, __LINE__)
    #endif
    #define log_warns(M, ...) fprintf(stderr, \
        "\033[01;33m[WARN] " M "\033[0m\n", ##__VA_ARGS__)
    #define log_warn(M) fprintf(stderr, "\033[01;33m[WARN] " M "\033[0m\n")
    #define log_infos(M, ...) fprintf(stderr, \
        "\033[01;32m[INFO] " M "\033[0m\n", ##__VA_ARGS__)
    #define log_info(M) fprintf(stderr, "\033[01;32m[INFO] " M "\033[0m\n")

#endif

