#ifndef COMMONINCLUDE_H
#define COMMONINCLUDE_H
#ifdef WIN32
#include<WinSock.h>
#else
#include<sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h> 
#include   <arpa/inet.h>
typedef unsigned long DWORD;
#endif
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;
#endif
