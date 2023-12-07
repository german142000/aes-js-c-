#include <iostream>
#include "aes.h"
#include <windows.h>
#pragma comment(lib, "aes.lib")

using namespace std;

int main()
{
    encrypt((char*)"Hello world!", 12, (char*)"dgfyrhebsdhehdcf");
    int ns = 0;
    char* res = getResult(&ns);
    std::string enc(res, ns);
    std::cout << enc << std::endl;
    decrypt(res, ns - 1, (char*)"dgfyrhebsdhehdcf");
    char* res2 = getResult(&ns);
    std::string dec(res2, ns);
    std::cout << dec << std::endl;
}
