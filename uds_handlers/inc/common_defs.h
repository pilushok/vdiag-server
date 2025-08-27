#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* so export */
#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define CALLBACK
#define EXPORT __attribute__((visibility("default")))
#endif
