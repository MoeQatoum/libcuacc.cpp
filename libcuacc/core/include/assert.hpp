#ifndef CUDA_COMMON_H
#define CUDA_COMMON_H

#define LOG_COLOR_Y   "\033[93m"
#define LOG_COLOR_R   "\033[91m"
#define LOG_COLOR_W   "\033[0m"
#define LOG_BOLD_FONT "\033[1m"

static_assert(sizeof(int) == 4, "size of int != 4");
static_assert(sizeof(float) == 4, "size of float != 4");
static_assert(sizeof(double) == 8, "size of double != 8");

#define CU_CXX_FORCED_ASSERT(condition, msg, ...)                                                         \
    do {                                                                                                  \
        if (!(condition)) {                                                                               \
            fprintf(stderr,                                                                               \
                    LOG_COLOR_R "ASSERTION %s:%d at %s: " LOG_COLOR_W LOG_BOLD_FONT msg LOG_COLOR_W "\n", \
                    __FILE_NAME__,                                                                        \
                    __LINE__,                                                                             \
                    __PRETTY_FUNCTION__ __VA_OPT__(, ) __VA_ARGS__);                                      \
            abort();                                                                                      \
        }                                                                                                 \
    } while (0)

#define CU_CXX_ABORT(reason, ...)                                                                    \
    do {                                                                                             \
        fprintf(stderr,                                                                              \
                LOG_COLOR_R "ABORT %s:%d at %s: " LOG_COLOR_W LOG_BOLD_FONT reason LOG_COLOR_W "\n", \
                __FILE_NAME__,                                                                       \
                __LINE__,                                                                            \
                __PRETTY_FUNCTION__ __VA_OPT__(, ) __VA_ARGS__);                                     \
        abort();                                                                                     \
    } while (0)

#define CU_CXX_WARN(warn_msg, ...) fprintf(stderr, LOG_COLOR_Y "WARNING: " LOG_BOLD_FONT warn_msg LOG_COLOR_W "\n" __VA_OPT__(, ) __VA_ARGS__)

#ifdef NDEBUG
    #define CU_CXX_DEBUG_LVL 1
#else
    #define CU_CXX_DEBUG_LVL 0
#endif // NDEBUG

#if CU_CXX_DEBUG_LVL > 0
    #define CU_CXX_DEBUG_ASSERT(condition, msg, ...) CUDA_CXX_FORCED_ASSERT(condition, msg __VA_OPT__(, ) __VA_ARGS__)
#else
    #define CU_CXX_DEBUG_ASSERT(condition, msg) \
        do {                                    \
        } while (0);
#endif // CU_CXX_DEBUG_LVL > 0

#endif // CUDA_COMMON_H
