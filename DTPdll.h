#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

#ifdef DTPCIPHER_EXPORTS
#define DTPCIPHER_API __declspec(dllexport)
#else
#define DTPCIPHER_API __declspec(dllimport)
#endif

extern "C" DTPCIPHER_API void doubleTablePermutationCipher(int console_answer);
