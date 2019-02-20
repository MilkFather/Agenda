#include "AgendaUI.hpp"
#include <csignal>
#include <iostream>

#ifdef _WIN32
#include <windows.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#endif

using std::cout;
using std::endl;

AgendaUI *ui = nullptr;

void sig_handler(int sig) {
    if (sig == SIGINT) {
        cout << endl << "\033[1;31m" << "[Agenda] User interrupt, saving" << "\033[0m" <<  endl;
        if (ui != nullptr) {
            delete ui;
        }
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    signal(SIGINT, sig_handler);
#ifdef _WIN32
    // enable ANSI escape sequence on Windows
    // require Windows 10 or newer
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return GetLastError();
    }
    
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        return GetLastError();
    }
    
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) {
        return GetLastError();
    }
#endif
    ui = new AgendaUI;
    ui->OperationLoop();
    return 0;
}
