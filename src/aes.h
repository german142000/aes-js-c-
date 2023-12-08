#pragma once
#define EX extern "C" __declspec(dllexport)
#define CDLL __cdecl

EX bool CDLL initAes();
EX bool CDLL exitAes();
EX bool CDLL encrypt(char* plainText, int size, char* key);
EX bool CDLL decrypt(char* chiperText, int size, char* key);
EX char* CDLL getResult(int* size);
EX char* CDLL getError(int* size);
