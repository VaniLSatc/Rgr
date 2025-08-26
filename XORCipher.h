#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <random>

#ifdef XORCIPHER_EXPORTS
#define XORCIPHER_API __declspec(dllexport)
#else
#define XORCIPHER_API __declspec(dllimport)
#endif

extern "C" XORCIPHER_API void XOR_Cipher(int console_answer);
