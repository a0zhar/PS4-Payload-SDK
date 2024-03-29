#pragma once

#ifndef DUMP_H
#define DUMP_H

#include "./types.h"

typedef struct {
  int index;
  uint64_t fileoff;
  size_t bufsz;
  size_t filesz;
  int enc;
} SegmentBufInfo;

int is_self(const char *fn);
void decrypt_and_dump_self(char *selfFile, char *saveFile);
void decrypt_dir(const char *sourcedir, const char *destdir);
int waitForApplication(const char *title_id);
int wait_for_bdcopy(char *title_id);
int wait_for_usb(char *usb_name, char *usb_path);


#endif
