#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "bot.h"
using namespace std;

namespace Mercury
{
	bool SelectAll = false;
	const int max_client = 20; // for now
	const char* clientNames[Mercury::max_client] = { "NULL" };
	std::vector<BreakerBotV2*>bots;
	int current = -1;
	bool do_once_darildin_cak = false;
	void debug_log(string text) {
		std::cout << text << endl;
	}

	void updateNames() {
		for (int i = 0; i < bots.size(); i++) {
			clientNames[i] = bots[i]->local_player.name.c_str();
		}
	}

	bool botExists(string aga) {
		for (int i = 0; i < bots.size(); i++) {
			if (aga == lowercase(bots[i]->local_player.name)) {
				return true;
			}
		}
		return false;
	}

	void addBot(string gid, string pwd,std::string gameversion,string world) {

		LoginInfo login;
		login.setGrowID(gid);

		if (botExists(login.getGrowID().c_str())) {
			return;
		}

		login.password = pwd;
		login.macAddress = GenerateMacAddress();
		login.version = gameversion;
		login.serverIp = "213.179.209.168";
		login.serverPort = 17199;
		bots.emplace_back(new BreakerBotV2(login));
		bots.back()->startAsync();
		bots.back()->setTargetWorld(world);

	}
}