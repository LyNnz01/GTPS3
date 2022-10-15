#include "world.h"
#include "HTTPRequest.hpp"
#include "proton/rtparam.hpp"
#include "HTTPRequest.hpp"
#include "globals.h"
#include <iostream>
#include <fstream>
#include <map>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "json.hpp"
using nlohmann::json;

inline bool File_Exists(const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}
int main()
{
        std::string gameversion = "", deposit = "",growid="",password="";
        if (!File_Exists("config.json"))
        {
            std::ofstream f("config.json", std::ios_base::trunc | std::ios_base::out);  f << "{\n	\"growid\":\"\",\n	\"password\":\"\",\n	\"deposit\":\"\",\n	\"game_version\":\"\"\n}";
            std::cout << "Deposit Bot doesnt configured, please configure it!\npress any key to exit..." << endl;
            cin.get();
            exit(-1);
        }
        else
        {
            std::ifstream ifs("config.json");
            json j = json::parse(ifs);
            gameversion = j.at("game_version").get<string>();
            deposit = j.at("deposit").get<string>();
            growid = j.at("growid").get<string>();
            password = j.at("password").get<string>();

            std::cout << "Game Version: " << j.at("game_version").get<string>() << " Deposit World: " << j.at("deposit").get<string>() <<" GrowID: " << growid << " Password: " <<password << endl;
        }
        std::cout << "Decoding items.dat file..." << endl;
        Mercury::bots.back()->world.itemDataContainer.LoadItemData("C:\\Users\\SrMotion\\AppData\\Local\\Growtopia\\cache\\items.dat");
        std::cout << "Decoded.." << endl;

        Mercury::addBot(growid, password, gameversion, deposit);// set growid and pass null to login non growid
    std::cin.get();
}
