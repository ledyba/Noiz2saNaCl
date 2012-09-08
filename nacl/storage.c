/* Copyright (c) 2011 The Native Client Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.

 * This file provides wrappers for most of the FILE* related functions
 * such a fopen/fclose/fread/fwrite.
 * Use this with the nacl_file_hooks.h header to "hijack" file operations.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <dirent.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NACL_FILE_MAGIC 0xdeadbeef
#define FILE_TO_NACLFILE(f) file_to_naclfile((f), __FUNCTION__)

#define OUT stdout

static void debug(int level, const char* fmt, ...) {
	fputs("// DEBUG: ", OUT);
	va_list argp;
	va_start(argp, fmt);
	vfprintf(OUT, fmt, argp);
	va_end(argp);
	fflush(OUT);
}


static void warn(const char* fmt, ...) {
	fputs("WARN: ", OUT);
	va_list argp;
	va_start(argp, fmt);
	vfprintf(OUT, fmt, argp);
	va_end(argp);
	fflush(OUT);
}

#include "file_data.h"
#include "storage.h"
#include "storage_sdl.h"

typedef struct {
	int magic;
	const struct FileEntry* entry;
	size_t pos;
	int error;
	int eof;
	int opened;
} NACL_FILE;

static NACL_FILE* file_to_naclfile(FILE* stream, const char* function) {
	NACL_FILE* nf = (NACL_FILE*) stream;
	if( nf->magic != NACL_FILE_MAGIC) {
		warn("invalid stream pointer in %s\n", function);
	}
	return nf;
}

const struct FileEntry* searchFile(const char* filename)
{
	for(int i=0;i<gNumberOfFileImages; ++i){
		const struct FileEntry* entry = &gFileImages[i];
		if(strcmp(filename, entry->filename) == 0){
			return entry;
		}
	}
	return 0;
}

extern SDL_RWops* fopenSDL_RWops(const char* filename)
{
	debug(1, "fopenSDL_RWops(%s)\n", filename);
	const struct FileEntry* entry = searchFile(filename);
	if(entry == NULL){
		warn("Oops! Failed to find: %s\n", filename);
		return 0;
	}
	return SDL_RWFromConstMem(entry->data, entry->size);
}
FILE* nacl_fopen(const char* filename, const char* mode) {
	debug(1, "nacl_fopen(%s,%s)\n", filename, mode);

	if (mode[0] == 'r') {
		const struct FileEntry* entry = searchFile(filename);
		if(entry == NULL){
			warn("Oops! Failed to find: %s\n", filename);
			return 0;
		}
		NACL_FILE* nf = malloc(sizeof(NACL_FILE));
		nf->magic = NACL_FILE_MAGIC;
		nf->entry = entry;
		nf->opened=1;
		nf->error=0;
		nf->eof=0;
		nf->pos=0;
		return (FILE*)nf;
	} else if (mode[0] == 'w') {
		warn("Oops. Writing is not supported. %s\n", filename);
		return 0;
	} else {
		warn("unexpected open mode %s", mode);
		return 0;
	}
}


int nacl_fclose(FILE *stream) {
	NACL_FILE* nf = FILE_TO_NACLFILE(stream);
	debug(1, "fclose(%s) -> %d\n", nf->entry->filename, nf->entry->size);
	free(nf);
	return 0;
}


int nacl_ferror(FILE *stream) {
	const NACL_FILE* nf = FILE_TO_NACLFILE(stream);
	debug(1, "ferror(%s)\n", nf->entry->filename);
	return nf->error;
}


void nacl_clearerr(FILE *stream) {
	NACL_FILE* nf = FILE_TO_NACLFILE(stream);
	debug(1, "clearerr(%s)\n", nf->entry->filename);
	nf->error = 0;
	nf->eof = 0;
}


int nacl_feof(FILE *stream) {
	const NACL_FILE* nf = FILE_TO_NACLFILE(stream);
	debug(1, "feof(%s)\n", nf->entry->filename);
	return nf->eof;
}


void nacl_rewind(FILE *stream) {
	NACL_FILE* nf = FILE_TO_NACLFILE(stream);
	debug(1, "rewind(%s)\n", nf->entry->filename);

	nf->pos = 0;
	nf->eof = 0;
	nf->error = 0;
}


size_t nacl_fread(void *ptr, size_t size, size_t nmemb, FILE* stream) {
	if ( stream == NULL) {
		warn("fread with NULL FILE*\n");
		return -1;
	}
	NACL_FILE* nf = FILE_TO_NACLFILE(stream);
	debug(1, "fread(%s, %u, %u)\n", nf->entry->filename, size, nmemb);
	size_t total_size = size * nmemb;
	if (nf->pos + total_size > nf->entry->size) {
		nf->eof = 1;
		total_size = nf->entry->size - nf->pos;
	}

	memmove(ptr, nf->entry->data + nf->pos, total_size);
	nf->pos += total_size;
	debug(1, "fread -> %d\n", total_size / size);
	return total_size / size;
}

size_t nacl_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	if ( stream == NULL) {
		warn("fwrite with NULL FILE*\n");
		return -1;
	}
	NACL_FILE* nf = FILE_TO_NACLFILE(stream);
	warn("fwrite not supported!!!: %s\n", nf->entry->filename);
}


size_t nacl_ftell (FILE *stream) {
	if (stream == NULL) {
		warn("ftell with NULL FILE*\n");
		return -1;
	}
	NACL_FILE* nf = FILE_TO_NACLFILE(stream);
	debug(1, "ftell(%s)\n", nf->entry->filename);
	return nf->pos;
}

int nacl_fseek (FILE *file, size_t offset, int whence)
{
	if (file == NULL) {
		warn("fseek with NULL FILE*\n");
		return -1;
	}
	NACL_FILE* nf = FILE_TO_NACLFILE(file);
	debug(1, "fseek(%s, %ld, %d)\n", nf->entry->filename, offset, whence);
	switch (whence) {
		case SEEK_SET:
			break;
		case SEEK_CUR:
			offset = nf->pos + offset;
			break;
		case SEEK_END:
			offset = nf->entry->size + offset;
			break;
	}

	if (offset < 0) {
		return -1;
	}

	/* NOTE: the emulation of fseek is not 100% compatible */
	nf->eof = 0;
	nf->pos = offset;
	return 0;
}


int nacl_fgetc(FILE* stream) {
	NACL_FILE* nf = FILE_TO_NACLFILE(stream);
	debug(1, "fgetc(%s)\n", nf->entry->filename);
	if (nf->pos >= nf->entry->size) {
		nf->eof = 1;
		return EOF;
	}

	int result =	nf->entry->data[nf->pos];
	nf->pos += 1;
	return result;
}

extern int nacl_fputc(char c, FILE *stream)
{
	warn("Not supported: fputc\n");
}

int nacl_fflush(FILE* stream) {
	NACL_FILE* nf = FILE_TO_NACLFILE(stream);
	debug(1, "fflush(%s)\n", nf->entry->filename);
	return 0;
}

typedef struct NACL_DIR{
	int magick;
	char* dirname;
	int currentIndex;
	struct dirent entry;
} NACL_DIR;

#define DIR_TO_NACLDIR(d) dir_to_nacldir((d), __FUNCTION__)

NACL_DIR* dir_to_nacldir(DIR* dir, const char* function)
{
	NACL_DIR* nd = (NACL_DIR*) dir;
	if( nd->magick != NACL_FILE_MAGIC) {
		warn("invalid stream pointer in %s\n", function);
	}
	return nd;
}

int nextDirEntry(const char* dirname, int offset)
{
	const size_t len = strlen(dirname);
	for(int i=offset;i<gNumberOfFileImages; ++i){
		const struct FileEntry* entry = &gFileImages[i];
		if(strncmp(dirname, entry->filename, len) == 0){
			return i;
		}
	}
	return -1;
}

extern DIR* nacl_opendir(const char* dirname)
{
	debug(1, "opendir(%s)\n", dirname);
	const int offset = nextDirEntry(dirname, 0);
	if(offset >= 0){
		NACL_DIR* dir = malloc(sizeof(NACL_DIR));
		dir->magick = NACL_FILE_MAGIC;
		const int len = strlen(dirname);
		dir->dirname = malloc(len+1);
		memcpy(dir->dirname, dirname, len+1);
		dir->currentIndex = offset;
		return (DIR*)dir;
	}
	warn("Oops. Dir: %s not found.\n", dirname);
	return 0;
}

extern struct dirent* nacl_readdir(DIR* d)
{
	NACL_DIR* nd = DIR_TO_NACLDIR(d);
	debug(1, "readdir(%s)\n", nd->dirname);
	if(nd->currentIndex < 0 || (nd->currentIndex = nextDirEntry(nd->dirname, nd->currentIndex+1)) < 0){
		debug(1, "readdir(%s) stop.\n", nd->dirname);
		return 0;
	}
	struct dirent* entry = &nd->entry;
	entry->d_ino = 0;
	entry->d_off = 0;
	entry->d_reclen = 0;
	const struct FileEntry* fe = &gFileImages[nd->currentIndex];
	memcpy(entry->d_name, &fe->filename[strlen(nd->dirname)+1], strlen(fe->filename));
	debug(1, "readdir(%s) found: %s\n", nd->dirname, entry->d_name);
	return entry;
}

extern DIR* nacl_closedir(DIR* d)
{
	NACL_DIR* nd = DIR_TO_NACLDIR(d);
	debug(1, "closedir(%s)\n", nd->dirname);
	free(nd->dirname);
	free(nd);
}

#ifdef __cplusplus
}
#endif
