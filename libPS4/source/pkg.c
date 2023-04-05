#include "../include/file.h"
#include "../include/libc.h"
#include "../include/strings.h"
#include "../include/pkg.h"
#define EOF '\00'

static uint16_t bswap_16(uint16_t val) {
  uint16_t retValue;
  retValue = (val & (uint16_t)0x00ff) << 8;
  retValue |= (val & (uint16_t)0xff00) >> 8;
  return retValue;
}

static uint32_t bswap_32(uint32_t val) {
  uint32_t retValue;
  retValue = (val & (uint32_t)0x000000ffUL) << 24;
  retValue |= (val & (uint32_t)0x0000ff00UL) << 8;
  retValue |= (val & (uint32_t)0x00ff0000UL) >> 8;
  retValue |= (val & (uint32_t)0xff000000UL) >> 24;
  return retValue;
}
// Just shortcuts 
typedef struct cnt_pkg_content_header CNTPKGCONTENTHDR;
typedef struct cnt_pkg_main_header    CNTPKGMAINHDR;
#define printf(...) 
// Checks whether an application is of type Fake
int isfpkg(char *pkgfn) {
  CNTPKGMAINHDR m_header;
  FILE *in = 0;
  int result = 0;
  char buffer[5];

  // Open File using (Read-Binary) as Mode
  in = fopen(pkgfn, "rb");
  if (in == NULL) {
    printf("can't open file %s", pkgfn);
    result = 1;
    goto skipit;
  }
  // Read the first 5 characters of the file
  if (fread(buffer, 1, sizeof(buffer), in) <= 0) {
    printf("an error occured while reading file %s", pkgfn);
    goto skipit;
  }
  if (strcmp(buffer, "CNT@") != 0) {
    printf("CNT@ wasn't present within the file");
    // go back to the begining of file
    fseek(in, 0, SEEK_SET);
    // read 384 bytes from file into m_header
    if (fread(&m_header, 1, 0x180, in) <= 0) {
      printf("an error occured while reading file %s", pkgfn);
      goto skipit;
    }
    // If the m_header.magic dont match ps4 pkg magic
    if (m_header.magic != PS4_PKG_MAGIC) {
      printf("m_header magic doesn't match PS4_PKG_MAGIC macro");
      result = 2;
    } else if (bswap_32(m_header.type) != 1) {
      printf("bswap_32() m_header type is not 1");
      result = 3;
    }
  }skipit:;
  if (in != NULL) fclose(in);
  return result;
}

static void _mkdir(const char *dir) {
  char tmp[256];
  char *p = NULL;

  snprintf(tmp, sizeof(tmp), "%s", dir);
  for (p = tmp + 1; *p; p++) {
    if (*p == '/') {
      *p = 0;
      mkdir(tmp, 0777);
      *p = '/';
    }
  }
}
static char *get_entry_name_by_type(uint32_t type) {
  static char entry_name[32];
  switch (type) {
    case 0x1201 ... 0x121F: sprintf(entry_name, "icon0_%02u.png", type - 0x1201); break;
    case 0x1241 ... 0x125F: sprintf(entry_name, "pic1_%02u.png", type - 0x1241);  break;
    case 0x1261 ... 0x127F: sprintf(entry_name, "changeinfo/changeinfo_%02u.xml", type - 0x1261); break;
    case 0x1281 ... 0x129F: sprintf(entry_name, "icon0_%02u.dds", type - 0x1281); break;
    case 0x12C1 ... 0x12DF: sprintf(entry_name, "pic1_%02u.dds", type - 0x12C1);  break;
    case 0x1400 ... 0x147F: sprintf(entry_name, "trophy/trophy%02u.trp", type - 0x1400); break;
    case 0x1600 ... 0x1609: sprintf(entry_name, "keymap_rp/%03u.png", type - 0x15FF); break;
    case 0x1610 ... 0x16F5:
    int quotient = (type - 0x1610) / 10;
    int remainder = (type - 0x1610) % 10;
    int second_argument = remainder ? remainder : 10;
    sprintf(entry_name, "keymap_rp/%02u/%03u.png", quotient, second_argument);
    break;
    // Otherwise, if none of the above cases were true.
    default:
    switch (type) {
      case 0x0400: return "license.dat"; break;
      case 0x0401: return "license.info"; break;
      case 0x0402: return "nptitle.dat"; break;
      case 0x0403: return "npbind.dat"; break;
      case 0x0404: return "selfinfo.dat"; break;
      case 0x0406: return "imageinfo.dat"; break;
      case 0x0407: return "target-deltainfo.dat"; break;
      case 0x0408: return "origin-deltainfo.dat"; break;
      case 0x0409: return "psreserved.dat"; break;
      case 0x1000: return "param.sfo"; break;
      case 0x1001: return "playgo-chunk.dat"; break;
      case 0x1002: return "playgo-chunk.sha"; break;
      case 0x1003: return "playgo-manifest.xml"; break;
      case 0x1004: return "pronunciation.xml"; break;
      case 0x1005: return "pronunciation.sig"; break;
      case 0x1006: return "pic1.png"; break;
      case 0x1007: return "pubtoolinfo.dat"; break;
      case 0x1008: return "app/playgo-chunk.dat"; break;
      case 0x1009: return "app/playgo-chunk.sha"; break;
      case 0x100A: return "app/playgo-manifest.xml"; break;
      case 0x100B: return "shareparam.json"; break;
      case 0x100C: return "shareoverlayimage.png"; break;
      case 0x100D: return "save_data.png"; break;
      case 0x100E: return "shareprivacyguardimage.png"; break;
      case 0x1200: return "icon0.png"; break;
      case 0x1220: return "pic0.png"; break;
      case 0x1240: return "snd0.at9"; break;
      case 0x1260: return "changeinfo/changeinfo.xml"; break;
      case 0x1280: return "icon0.dds"; break;
      case 0x12A0: return "pic0.dds"; break;
      case 0x12C0: return "pic1.dds"; break;
    }
    break;
  }
  return entry_name;
}

int unpkg(char *pkgfn, char *tidpath) {
  struct cnt_pkg_main_header m_header;
  struct cnt_pkg_content_header c_header;
  memset(&m_header, 0, sizeof(struct cnt_pkg_main_header));
  memset(&c_header, 0, sizeof(struct cnt_pkg_content_header));

  int fdin = open(pkgfn, O_RDONLY, 0);
  if (fdin == -1) {
    return 1;
  }

  // Read in the main CNT header (size seems to be 0x180 with 4 hashes included).
  lseek(fdin, 0, SEEK_SET);
  read(fdin, &m_header, 0x180);

  if (m_header.magic != PS4_PKG_MAGIC) {
    return 2;
  }

  // Seek to offset 0x400 and read content associated header (size seems to be 0x80 with 2 hashes included).
  lseek(fdin, 0x400, SEEK_SET);
  read(fdin, &c_header, 0x80);

  // Locate the entry table and list each type of section inside the PKG/CNT file.
  lseek(fdin, bswap_32(m_header.file_table_offset), SEEK_SET);

  struct cnt_pkg_table_entry *entries = malloc(sizeof(struct cnt_pkg_table_entry) * bswap_16(m_header.table_entries_num));
  if (entries == NULL) {
    close(fdin);
    return 3;
  }
  memset(entries, 0, sizeof(struct cnt_pkg_table_entry) * bswap_16(m_header.table_entries_num));
  int i;
  for (i = 0; i < bswap_16(m_header.table_entries_num); i++) {
    read(fdin, &entries[i], 0x20);
  }

  // Vars for file name listing.
  struct file_entry *entry_files = malloc(sizeof(struct file_entry) * bswap_16(m_header.table_entries_num));
  if (entry_files == NULL) {
    close(fdin);
    free(entries);
    return 4;
  }
  memset(entry_files, 0, sizeof(struct file_entry) * bswap_16(m_header.table_entries_num));
  char *file_name_list[256];
  int file_name_index = 0;
  int file_count = 0;

  // Search through the data entries and locate the name table entry.
  // This section should keep relevant strings for internal files inside the PKG/CNT file.
  for (i = 0; i < bswap_16(m_header.table_entries_num); i++) {
    if (bswap_32(entries[i].type) == PS4_PKG_ENTRY_TYPE_NAME_TABLE) {
      lseek(fdin, bswap_32(entries[i].offset) + 1, SEEK_SET);
      while ((file_name_list[file_name_index] = read_string(fdin))[0] != '\0') {
        file_name_index++;
      }
    }
  }

  // Search through the data entries and locate file entries.
  // These entries need to be mapped with the names collected from the name table.
  for (i = 0; i < bswap_16(m_header.table_entries_num); i++) {
    // Use a predefined list for most file names.
    entry_files[i].name = get_entry_name_by_type(bswap_32(entries[i].type));
    entry_files[i].offset = bswap_32(entries[i].offset);
    entry_files[i].size = bswap_32(entries[i].size);

    if (((bswap_32(entries[i].type) & PS4_PKG_ENTRY_TYPE_FILE1) == PS4_PKG_ENTRY_TYPE_FILE1) || (((bswap_32(entries[i].type) & PS4_PKG_ENTRY_TYPE_FILE2) == PS4_PKG_ENTRY_TYPE_FILE2))) {
      // If a file was found and it's name is not on the predefined list, try to map it with
      // a name from the name table.
      if (entry_files[i].name == NULL) {
        entry_files[i].name = file_name_list[file_count];
      }
      if (entry_files[i].name != NULL) {
        file_count++;
      }
    }
  }

  // Set up the output directory for file writing.
  char dest_path[256];
  char title_id[256];

  memset(title_id, 0, 256);
  memcpy(title_id, tidpath, 255);
  mkdir(title_id, 0777);

  // Search through the entries for mapped file data and output it.
  for (i = 0; i < bswap_16(m_header.table_entries_num); i++) {
    // Var for file writing.
    unsigned char *entry_file_data = (unsigned char *)realloc(NULL, entry_files[i].size);

    lseek(fdin, entry_files[i].offset, SEEK_SET);
    read(fdin, entry_file_data, entry_files[i].size);

    if (entry_files[i].name == NULL)
      continue;

    sprintf(dest_path, "%s/sce_sys/%s", title_id, entry_files[i].name);

    _mkdir(dest_path);

    int fdout = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fdout != -1) {
      write(fdout, entry_file_data, entry_files[i].size);
      close(fdout);
    } else {
      close(fdin);
      free(entries);
      free(entry_files);
      return 5;
    }
  }

  // Clean up.
  close(fdin);

  free(entries);
  free(entry_files);

  return 0;
}
