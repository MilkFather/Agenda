#include "AgendaUI.hpp"
#include <csignal>

AgendaUI *ui = nullptr;

void sig_handler(int sig) {
    if (sig == SIGINT) {
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
