#ifndef IOS_LOGGER_H_INCLUDED
#define IOS_LOGGER_H_INCLUDED
//---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void Logger_debug(const char*);
void Logger_info(const char*);
void Logger_warn(const char*);
void Logger_error(const char*);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
//---------------------------------------------------------------

#endif // IOS_LOGGER_H_INCLUDED