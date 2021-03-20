#include <stdio.h>
#include <sys/statvfs.h>
#pragma once

void getStorageUsage(float* usage);

void getMemUsage(FILE* fp, unsigned long *usage, unsigned long *total, char *buffer, size_t buffer_size);

void getCpuUsage(FILE* fp, float* usage, char *buffer, size_t buffer_size);

void getUptime(FILE* fp, unsigned long *uptime, char *buffer, size_t buffer_size);
