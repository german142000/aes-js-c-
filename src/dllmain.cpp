#include "httplib.h"
#include "base64.h"
#include "aes.h"

#include <windows.h>
#include <iostream>
#include <iphlpapi.h>
#include <random>
#include <fstream>
#include <sstream>
#include <thread>

#pragma comment(lib, "iphlpapi.lib")
#pragma warning(disable : 4996)

using namespace std;

string res = "no res";
string error = "no errors";
string path = "no path";
int port = 0;

int getRandomNumber(int min, int max){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

bool isPortFree(int port){
    MIB_TCPTABLE2* tcpTable;
    DWORD tcpTableSize = 0;
    if (GetTcpTable2(nullptr, &tcpTableSize, FALSE) != ERROR_INSUFFICIENT_BUFFER){
        std::cout << "Failed to get TCP table size." << std::endl;
        return false;
    }
    tcpTable = reinterpret_cast<MIB_TCPTABLE2*>(new char[tcpTableSize]);
    if (GetTcpTable2(tcpTable, &tcpTableSize, FALSE) != NO_ERROR){
        delete[] tcpTable;
        std::cout << "Failed to get TCP table." << std::endl;
        return false;
    }
    for (DWORD i = 0; i < tcpTable->dwNumEntries; ++i){
        MIB_TCPROW2& tcpRow = tcpTable->table[i];
        if (tcpRow.dwLocalPort == port){
            delete[] tcpTable;
            return false;
        }
    }
    delete[] tcpTable;
    return true;
}

string GetCurrentDirectory() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    string path(buffer);
    size_t pos = path.find_last_of("\\/");
    return path.substr(0, pos);
}

bool initAes() {
    port = getRandomNumber(8000, 65000);
    while (!isPortFree(port)) {
        port = getRandomNumber(8000, 65000);
    }
    thread t1([&]() {
        string command = "cd " + GetCurrentDirectory() + " & node\\node.exe aes\\main.js " + to_string(port);
        system(command.c_str());
    });
    t1.detach();
    return true;
}

bool exitAes() {
    string host = "http://127.0.0.1:" + to_string(port);
    string path = "/?func=exit";
    httplib::Client nhtpp(host);
    nhtpp.Get(path);
    return true;
}

bool encrypt(char* plainText, int size, char* key) {
    try {
        string text(plainText, size);
        Base64 b64(text, Base64::TextEncode);
        text = b64.encode();
        string skey = "xxxxxxxxxxxxxxxx";
        for (int i = 0; i < 16; i++) {
            skey[i] = key[i];
        }
        string host = "http://127.0.0.1:" + to_string(port);
        string path = "/?func=enc&text=" + text + "&key=" + skey;

        httplib::Client nhtpp(host);
        auto hres = nhtpp.Get(path);
        res = hres->body;
        res.erase(res.find_last_not_of(" \n\r\t") + 1);
        return true;
    }
    catch (exception e) {
        error = e.what();
        return false;
    }
}

bool decrypt(char* chiperText, int size, char* key) {
    try {
        string text(chiperText, size);
        string skey = "xxxxxxxxxxxxxxxx";
        for (int i = 0; i < 16; i++) {
            skey[i] = key[i];
        }
        string host = "http://127.0.0.1:" + to_string(port);
        string path = "/?func=dec&text=" + text + "&key=" + skey;

        httplib::Client nhtpp(host);
        auto hres = nhtpp.Get(path);
        res = hres->body;
        res.erase(res.find_last_not_of(" \n\r\t") + 1);
        Base64 b64(res, Base64::Decode);
        string ib64(reinterpret_cast<char const*>(b64.decode()));
        res = ib64;
        return true;
    }
    catch (exception e) {
        error = e.what();
        return false;
    }
}

char* getResult(int* size) {
    *size = res.length();
    char* cstr = (char*)malloc(sizeof(char) * (res.length() + 1));
    for (int i = 0; i < res.length() + 1; i++) {
        cstr[i] = res[i];
    }
    return cstr;
}

char* getError(int* size) {
    *size = error.length();
    char* cstr = (char*)malloc(sizeof(char) * (error.length() + 1));
    for (int i = 0; i < error.length() + 1; i++) {
        cstr[i] = error[i];
    }
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
