
#ifndef NACL_STORAGE_FILE_SDL_H
#define NACL_STORAGE_FILE_SDL_H
#ifdef __cplusplus
extern "C" {
#endif
#include <SDL_rwops.h>
extern SDL_RWops* fopenSDL_RWops(const char* filename);

#ifdef __cplusplus
}
#endif

#endif
