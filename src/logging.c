/*
 * UglyLogging.  Slow, yet another wheel reinvented, but enough to make the
 * rest of our code pretty enough.
 *
 */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "logging.h"

static int max_level = UDEBUG;

int ugly_init(int maximum_threshold) {
    max_level = maximum_threshold;
    return 0;
}

int ugly_log(int level, const char *tag, int line, const char *format, ...) {
    if (level > max_level) {
        return 0;
    }

    // Flush to maintain order of streams
    fflush(stdout);

    va_list args;
    va_start(args, format);
    time_t mytt = time(NULL);
    struct tm *tt;
    tt = localtime(&mytt);
    fprintf(stderr, "%d-%02d-%02dT%02d:%02d:%02d ", tt->tm_year + 1900, tt->tm_mon + 1, tt->tm_mday, tt->tm_hour, tt->tm_min, tt->tm_sec);
    switch (level) {
    case UDEBUG:
        fprintf(stderr, "DEBUG %s.%d: ", tag, line);
        break;
    case UINFO:
        fprintf(stderr, "INFO %s.%d: ", tag, line);
        break;
    case UWARN:
        fprintf(stderr, "WARN %s.%d: ", tag, line);
        break;
    case UERROR:
        fprintf(stderr, "ERROR %s.%d: ", tag, line);
        break;
    default:
        fprintf(stderr, "%d %s.%d: ", level, tag, line);
        break;
    }
    vfprintf(stderr, format, args);
    fflush(stderr);
    va_end(args);
    return 1;
}


/*
 *  Log message levels.
 *  - LIBUSB_LOG_LEVEL_NONE (0)    : no messages ever printed by the library (default)
 *  - LIBUSB_LOG_LEVEL_ERROR (1)   : error messages are printed to stderr
 *  - LIBUSB_LOG_LEVEL_WARNING (2) : warning and error messages are printed to stderr
 *  - LIBUSB_LOG_LEVEL_INFO (3)    : informational messages are printed to stderr
 *  - LIBUSB_LOG_LEVEL_DEBUG (4)   : debug and informational messages are printed to stderr
*/
int ugly_libusb_log_level(enum ugly_loglevel v)
{
    switch (v) {
    case UDEBUG: return 4;
    case UINFO: return 3;
    case UWARN: return 2;
    case UERROR: return 1;
    };

    return 2;
}
