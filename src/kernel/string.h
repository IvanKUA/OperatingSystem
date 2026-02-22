#pragma once

const char* strchr(const char* str, char chr);
char* strcpy(char* dst, const char* src);
unsigned strlen(const char* str);
int strcmp(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, unsigned n);
char* strncpy(char* dst, const char* src, unsigned n);
char* strtok(char* str, const char* delim);