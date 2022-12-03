#pragma once

#include "infrared_settings_filename.h"

#include <furi_hal.h>
#include <stdint.h>
#include <stdbool.h>
#include <toolbox/saved_struct.h>
#include <storage/storage.h>

#define INFRARED_SETTINGS_VER (1)
#define MAX_SAVED_REMOTES (10)

#define INFRARED_SETTINGS_PATH INT_PATH(INFRARED_SETTINGS_FILE_NAME)
#define INFRARED_SETTINGS_MAGIC (0x17)

#define INFRARED_SETTINGS_SAVE(x) \
    saved_struct_save(            \
        INFRARED_SETTINGS_PATH,   \
        (x),                      \
        sizeof(InfraredSettings), \
        INFRARED_SETTINGS_MAGIC,  \
        INFRARED_SETTINGS_VER)

#define INFRARED_SETTINGS_LOAD(x) \
    saved_struct_load(            \
        INFRARED_SETTINGS_PATH,   \
        (x),                      \
        sizeof(InfraredSettings), \
        INFRARED_SETTINGS_MAGIC,  \
        INFRARED_SETTINGS_VER)

#define MAX_LENGTH 128

typedef struct {
    char remote_name[MAX_LENGTH];
    char file_path[MAX_LENGTH];
} SavedRemote;

typedef struct {
    SavedRemote saved_remotes[MAX_SAVED_REMOTES];
} InfraredSettings;
