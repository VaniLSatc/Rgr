#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#ifdef CAESARCIPHER_EXPORTS
#define CAESARCIPHER_API __declspec(dllexport)
#else
#define CAESARCIPHER_API __declspec(dllimport)
#endif

extern "C" CAESARCIPHER_API void Caesar_Cipher(int console_answer);
