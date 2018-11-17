#include "AgendaUI.hpp"
#include <csignal>
#include <iostream>

AgendaUI *ui = nullptr;

void sig_handler(int sig) {
    if (sig == SIGINT) {
        std::cout << std::endl << "\033[1;31m" << "[Agenda] User interrupt, saving" << "\033[0m" <<  std::endl;
        if (ui != nullptr) {
            delete ui;
        }
        exit(sig);
    }
}

int main(int argc, char *argv[]) {
    /* insert your code here */
    signal(SIGINT, sig_handler);
    ui = new AgendaUI;
    ui->OperationLoop();
    return 0;
}
