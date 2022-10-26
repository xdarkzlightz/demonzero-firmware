#pragma once
#include <flipper_format/flipper_format.h>
#include "spi_mem_app.h"

void spi_mem_file_create_folder(SPIMemApp* app);
bool spi_mem_file_select(SPIMemApp* app);
bool spi_mem_file_create(SPIMemApp* app, const char* file_name);
bool spi_mem_file_delete(SPIMemApp* app);
FlipperFormat* spi_mem_file_open(SPIMemApp* app);
bool spi_mem_file_write_block(FlipperFormat* file, uint8_t* data, size_t size);
void spi_mem_file_close(FlipperFormat* file);