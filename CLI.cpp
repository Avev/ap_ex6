#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
}

void CLI::start() {
    argus arg;
    arg.hybrid = new HybridAnomalyDetector;
    struc = &arg;
    auto **commands = new Command *[6];
    commands[0] = new UploadCommand(dio, &arg);
    commands[1] = new SettingsCommand(dio, &arg);
    commands[2] = new DetectCommand(dio, &arg);
    commands[3] = new DisplayCommand(dio, &arg);
    commands[4] = new UploadAndAnalyzeCommand(dio, &arg);
    while (true) {
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for (int i = 0; i < 5; i++) {
            dio->write(i + 1);
            dio->write(".");
            dio->write(commands[i]->getDescription() + "\n");
        }
        dio->write("6." );
        dio->write("exit\n");
        int opt = std::stoi(dio->read());
        if (opt == 6) {
            for (int i = 0; i < 5; i++) {
                delete commands[i];
            }
            delete[] commands;
            delete arg.hybrid;
            return;
        }
        commands[opt-1]->execute();
    }
}

CLI::~CLI() {
}
