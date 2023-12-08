#include <iostream>
#include "aes.h"
#include <windows.h>
#pragma comment(lib, "aes.lib")

using namespace std;

int main()
{
    initAes();
    string key = "dgfyrhebsdhehdcf";
    encrypt((char*)"Hello world!", 21, (char*)"dgfyrhebsdhehdcf");
    int ns = 0;
    char* res = getResult(&ns);
    std::string enc(res, ns);
    std::cout << enc << std::endl;
    decrypt(res, ns, (char*)"dgfyrhebsdhehdcf");
    char* res2 = getResult(&ns);
    std::string dec(res2, ns);
    std::cout << dec << std::endl;
    exitAes();
    return 0;
}
