#ifndef _COMMON_MACRO_H_
#define _COMMON_MACRO_H_

#define _CONCAT(a, b) a##b
#define CONCAT(a, b) _CONCAT(a,b)
#define FUNC_NAME(name) CONCAT(los_, CONCAT(name, _entry))
#define FUNC_DECL(name) do { extern void FUNC_NAME(name)(void); FUNC_NAME(name)();} while(0)

#define STRINGIFY_INNER(x) #x
#define STRINGIFY(x) STRINGIFY_INNER(x)
#define FUNC_DECL2(name) do { app_manager_start(STRINGIFY(name), NULL); } while(0)

#endif //_COMMON_MACRO_H_
