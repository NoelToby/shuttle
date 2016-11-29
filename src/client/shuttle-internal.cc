#include <iostream>
#include <fstream>
#include <string>
#include <boost/lexical_cast.hpp>

#include "sdk/shuttle.h"
#include "client/config.h"
#include "client/connector.h"

int main(int argc, char** argv) {
    baidu::shuttle::Configuration config;
    int ret = config.ParseCommandLine(argc, argv);
    if (ret != 0) {
        return ret;
    }
    const std::string command = config.GetConf("command");
    if (command == "help" || config.GetConf("help") == "true") {
        std::cerr << config.Help();
        return 1;
    }
    if (command.empty()) {
        std::cerr << "ERROR: please offer a command" << std::endl;
        return -1;
    }
    if (command == "json") {
        std::vector<std::string> subcommand;
        config.GetConf("subcommand", subcommand);
        if (subcommand.empty()) {
            std::cerr << "ERROR: please specify template name" << std::endl;
            return -1;
        }
        if (subcommand.size() < 2) {
            ret = config.BuildJson(std::cout);
        } else {
            std::ofstream ofs(subcommand[1].c_str());
            ret = config.BuildJson(ofs);
            ofs.close();
        }
    } else if (command == "legacy") {
        baidu::shuttle::ShuttleConnector connector(&config);
        ret = connector.Submit();
    } else if (command == "dag") {
        // TODO parse json
        baidu::shuttle::ShuttleConnector connector(&config);
        ret = connector.Submit();
    } else if (command == "set") {
        baidu::shuttle::ShuttleConnector connector(&config);
        ret = connector.Update();
    } else if (command == "kill") {
        baidu::shuttle::ShuttleConnector connector(&config);
        ret = connector.Kill();
    } else if (command == "list") {
        baidu::shuttle::ShuttleConnector connector(&config);
        ret = connector.List();
    } else if (command == "status") {
        baidu::shuttle::ShuttleConnector connector(&config);
        ret = connector.Status();
    } else if (command == "monitor") {
        baidu::shuttle::ShuttleConnector connector(&config);
        ret = connector.Monitor();
    } else {
        std::cerr << "ERROR: " << command << " command is not recognized. "
                  << "Please refer to help command" << std::endl;
        ret = -1;
    }
    return ret;
}
