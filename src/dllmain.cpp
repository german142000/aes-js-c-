#include <windows.h>
#include <exception>
#include <string>
#include <fstream>
#include <sstream>
#include "aes.h"

#pragma warning(disable : 4996)

using namespace std;

string res = "no res";
string error = "no errors";
string path = "no path";

string readFile(const std::string& fileName) {
    ifstream f(fileName);
    stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

string GetCurrentDirectory() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    string path(buffer);
    size_t pos = path.find_last_of("\\/");
    return path.substr(0, pos);
}

bool encrypt(char* plainText, int size, char* key) {
    try {
        string path = "cd " + GetCurrentDirectory() + "\\aes & ";
        char* ptx = (char*)malloc(sizeof(char) * size);
        for (int i = 0; i < size; i++) {
            ptx[i] = plainText[i];
        }
        for (int i = 0; i < size; i++) {
            if (ptx[i] == ' ') ptx[i] = '_';
        }
        string ptxt(ptx, size);
        free(ptx);
        string skey(key, 16);
        string encc = path + "node.exe main.js -enc " + ptxt + " " + skey + " >> e.aes";
        system(encc.c_str());
        res = readFile("aes\\e.aes");
        string rfp = GetCurrentDirectory() + "\\aes\\e.aes";
        remove(rfp.c_str());
        return true;
    }
    catch (exception e) {
        error = e.what();
        return false;
    }
}

bool decrypt(char* chiperText, int size, char* key) {
    try {
        string path = "cd " + GetCurrentDirectory() + "\\aes & ";
        string ptxt(chiperText, size);
        string skey(key, 16);
        string encc = path + "node.exe main.js -dec " + ptxt + " " + skey + " >> d.aes";
        system(encc.c_str());
        res = readFile("aes\\d.aes");
        string rfp = GetCurrentDirectory() + "\\aes\\d.aes";
        remove(rfp.c_str());
        return true;
    }
    catch (exception e) {
        error = e.what();
        return false;
    }
}

char* getResult(int* size) {
    *size = res.length();
    char* cstr = (char*)malloc(sizeof(char) * res.length() + 1);
    strcpy(cstr, res.c_str());
    return cstr;
}

char* getError(int* size) {
    *size = error.length();
    char* cstr = (char*)malloc(sizeof(char) * error.length() + 1);
    strcpy(cstr, error.c_str());
    return cstr;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
