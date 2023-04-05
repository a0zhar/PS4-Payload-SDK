#pragma once
#ifndef PKG_H
#define PKG_H

#include "./types.h"

#define PS4_PKG_MAGIC 0x544E437F // .CNT

enum PS4_PKG_ENTRY_TYPES {
  PS4_PKG_ENTRY_TYPE_DIGEST_TABLE = 0x0001,
  PS4_PKG_ENTRY_TYPE_0x800 = 0x0010,
  PS4_PKG_ENTRY_TYPE_0x200 = 0x0020,
  PS4_PKG_ENTRY_TYPE_0x180 = 0x0080,
  PS4_PKG_ENTRY_TYPE_META_TABLE = 0x0100,
  PS4_PKG_ENTRY_TYPE_NAME_TABLE = 0x0200,
  PS4_PKG_ENTRY_TYPE_LICENSE = 0x0400,
  PS4_PKG_ENTRY_TYPE_FILE1 = 0x1000,
  PS4_PKG_ENTRY_TYPE_FILE2 = 0x1200
};

// CNT/PKG structures.
// Represents the main header of the CNT/PKG archive.
struct cnt_pkg_main_header {
  uint32_t magic; // Magic value indicating that this is a PKG file.
  uint32_t type; // Type of PKG file (e.g. game data, patch data, add-on content, etc.).
  uint32_t unk_0x08; // Unknown value.
  uint32_t unk_0x0C; // Unknown value.
  uint16_t unk1_entries_num; // Number of entries in the first "unknown" table.
  uint16_t table_entries_num; // Number of entries in the file table.
  uint16_t system_entries_num; // Number of entries in the "system" table.
  uint16_t unk2_entries_num; // Number of entries in the second "unknown" table.
  uint32_t file_table_offset; // Offset of the file table within the PKG file.
  uint32_t main_entries_data_size; // Size of the data block containing the main entries.
  uint32_t unk_0x20; // Unknown value.
  uint32_t body_offset; // Offset of the start of the file data within the PKG file.
  uint32_t unk_0x28; // Unknown value.
  uint32_t body_size; // Total size of the file data within the PKG file.
  uint8_t unk_0x30[0x10]; // Unknown values.
  uint8_t content_id[0x30]; // Content ID string.
  uint32_t unk_0x70; // Unknown value.
  uint32_t unk_0x74; // Unknown value.
  uint32_t unk_0x78; // Unknown value.
  uint32_t unk_0x7C; // Unknown value.
  uint32_t date; // Date of PKG file creation.
  uint32_t time; // Time of PKG file creation.
  uint32_t unk_0x88; // Unknown value.
  uint32_t unk_0x8C; // Unknown value.
  uint8_t unk_0x90[0x70]; // Unknown values.
  uint8_t main_entries1_digest[0x20]; // SHA-256 digest of the first set of main entries.
  uint8_t main_entries2_digest[0x20]; // SHA-256 digest of the second set of main entries.
  uint8_t digest_table_digest[0x20]; // SHA-256 digest of the digest table.
  uint8_t body_digest[0x20]; // SHA-256 digest of the file data.
} __attribute__((packed));

// Represents the content header of a file in the CNT/PKG archive.
struct cnt_pkg_content_header {
  uint32_t unk_0x400; // Unknown field.
  uint32_t unk_0x404; // Unknown field.
  uint32_t unk_0x408; // Unknown field.
  uint32_t unk_0x40C; // Unknown field.
  uint32_t unk_0x410; // Unknown field.
  uint32_t content_offset; // Offset to the content of the file.
  uint32_t unk_0x418; // Unknown field.
  uint32_t content_size; // Size of the content of the file.
  uint32_t unk_0x420; // Unknown field.
  uint32_t unk_0x424; // Unknown field.
  uint32_t unk_0x428; // Unknown field.
  uint32_t unk_0x42C; // Unknown field.
  uint32_t unk_0x430; // Unknown field.
  uint32_t unk_0x434; // Unknown field.
  uint32_t unk_0x438; // Unknown field.
  uint32_t unk_0x43C; // Unknown field.
  uint8_t content_digest[0x20];
  uint8_t content_one_block_digest[0x20];
} __attribute__((packed));

struct cnt_pkg_table_entry {
  uint32_t type;
  uint32_t unk1;
  uint32_t flags1;
  uint32_t flags2;
  uint32_t offset;
  uint32_t size;
  uint32_t unk2;
  uint32_t unk3;
} __attribute__((packed));

// Internal structure.
struct file_entry {
  int offset; 
  int size;
  char *name;
};
static uint32_t bswap_32(uint32_t val);
static uint16_t bswap_16(uint16_t val);

int isfpkg(char *pkgfn);
int unpkg(char *pkgfn, char *tidpath);

#endif
