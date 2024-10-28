#include "toolkit/time.h"

#ifdef BUILDFLAG_LINUX
    #define _XOPEN_SOURCE 600 /* Needed for `time.h` to expose `clock_gettime`. */
    #include <time.h>
    typedef struct timespec timespec_t;
    typedef struct tm tm_t;
#endif

/*
    Fetch the current time, formatted as a `timestamp_t`.

    @param `monotonic`: Whether to return the monotonic time. Realtime otherwise.

    @returns Formatted `timestamp_t`.

    @note Monotonic time is NOT time since epoch. 
    @note Use monotonic (`monotonic = true`) for timedeltas.
    @note Use realtime (`monotonic = false`) for wall-clock times.
*/
timestamp_t timestamp_get(bool monotonic) {
    #ifdef BUILDFLAG_LINUX
        timespec_t timespec; 
        timestamp_t timestamp;
        
        clock_gettime(monotonic ? CLOCK_MONOTONIC : CLOCK_REALTIME, &timespec);

        timestamp.sec = (int64_t)timespec.tv_sec;
        timestamp.nsec = (int64_t)timespec.tv_nsec;

        return timestamp;
    #endif

    ERROR("Time is not portable. Build with BUIDFLAG_[...], where [...] is WINDOWS, LINUX, or MAC.");
    return (timestamp_t){0};
}

/* 
    Difference between two `timestamp_t` as a `timestamp_t`. 

    @param `timestamp1`: The first timestamp (subtracted).
    @param `timestamp2`: The second timestamp (subtracted from).

    @returns Timdelta as a `timestamp_t` (`timestamp_t::sec` and `timestamp_t::nsec` are signed!).

    @note Argument order mimics variable definition order:
    @note `timestamp_t t1 = timestamp_get(true)` first.
    @note `timestamp_t t2 = timestamp_get(true)` second.
    @note `timestamp_t td = timestamp_dif(t1, t2)` is positive.
*/
inline timestamp_t timestamp_dif(timestamp_t timestamp1, timestamp_t timestamp2) {
    return (timestamp_t){.sec = timestamp2.sec - timestamp1.sec, .nsec = timestamp2.nsec - timestamp1.nsec};
}

/*
    `strftime` but for `timestamp_t`.

    @param `buf`: Buffer to print to. `strftime`'s `__s`.
    @param `max_put`: Maximum characters to print to the buffer. `strftime`'s `__maxsize`.
    @param `format`: How to format the `timestamp_t`. `strftime`'s `__format`.
    @param `timestamp`: The `timestamp_t` to format.
*/
inline void timestamp_strftime(char* buf, size_t max_put, const char* format, timestamp_t timestamp) {
    tm_t tm;
    memcpy(&tm, localtime(&timestamp.sec), sizeof(tm_t));
    strftime(buf, max_put, format, &tm);
}