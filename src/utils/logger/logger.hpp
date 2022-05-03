#pragma once

#include <iostream>

#ifdef DEBUG
#define LOG(__FMT__, ...) std::printf("[ INFO ][" __FUNCTION__ "]: " __FMT__ "\n", __VA_ARGS__)
#define LOG_DEBUG(__FMT__, ...) std::printf("[ DEBUG ][" __FUNCTION__ "]: " __FMT__ "\n", __VA_ARGS__)
#define LOG_WARNING(__FMT__, ...) std::printf("[ WARNING ][" __FUNCTION__ "]: " __FMT__ "\n", __VA_ARGS__)
#define LOG_ERROR(__FMT__, ...) std::printf("[ ERROR ][" __FUNCTION__ "]: " __FMT__ "\n", __VA_ARGS__)
#else
#define LOG(__FMT__, ...) std::printf("[ INFO ]: " __FMT__ "\n", __VA_ARGS__)
#define LOG_DEBUG(__FMT__, ...)
#define LOG_WARNING(__FMT__, ...) std::printf("[ WARNING ]: " __FMT__ "\n", __VA_ARGS__)
#define LOG_ERROR(__FMT__, ...) std::printf("[ ERROR ]: " __FMT__ "\n", __VA_ARGS__)
#endif