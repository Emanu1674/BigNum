#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#ifdef _WIN32
#define CLEAR "cls"
#include <windows.h>
#include <conio.h>
#else
#define CLEAR "clear"
#endif

#pragma once

int main() {

    setlocale( LC_ALL, "en_us.UTF-8" );
    return 0;
}