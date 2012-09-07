
#ifndef NACL_STORAGE_FILE_DATA_H
#define NACL_STORAGE_FILE_DATA_H
#include <stddef.h>
struct FileEntry{
	const char* filename;
	const size_t size;
	const char* data;
};

extern const struct FileEntry gFileImages[];
extern const size_t gNumberOfFileImages;

#endif
