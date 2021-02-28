#ifndef YUBI_H
#define YUBI_H

#if defined(_MSC_VER)
#ifdef YUBI_EXPORTS
#define YUBI_API __declspec(dllexport)
#else
#define YUBI_API __declspec(dllimport)
#endif
#elif defined(__GNUC__)
#ifdef YUBI_EXPORTS
#define YUBI_API __attribute__((visibility("default")))
#else
#define YUBI_API
#endif
#else
#define YUBI_API
#pragma warning Unknown dynamic link semantics.
#endif

#endif //YUBI_H
