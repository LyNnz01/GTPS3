//#include <string>
//#include <fstream>
//#include <iostream>
//#include <vector>
//#include "enet/include/enet.h"
//#include <nlohmann/json.hpp>
//#include <rapidjson/document.h>
//#include <rapidjson/istreamwrapper.h>
//#include "rapidjson/writer.h"
//#include "rapidjson/stringbuffer.h"
//#include <rapidjson/ostreamwrapper.h>
//
//
//#include "Item.h"
//#include "Base.h"
//#include "Player.h"
//#include "Packet.h"
////#include "Guilds.h"
//#include "World.h"
//#pragma comment(lib, "Ws2_32.lib")
//using namespace rapidjson;
//using namespace std;
//
//int auth_(ENetPeer* peer) {
//	Player* p_ = pInfo(peer);
//	if (p_->tankIDName.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890") != std::string::npos) return -1;
//	string path_ = "players/" + p_->tankIDName + "_.json";
//	if (_access_s(path_.c_str(), 0) == 0) {
//		ifstream f_(path_, ifstream::binary);
//		if (f_.fail()) return -1;
//		IStreamWrapper isw{ f_ };
//		Document r_{};
//		r_.ParseStream(isw);
//		StringBuffer buffer{};
//		Writer<StringBuffer> writer{ buffer };
//		r_.Accept(writer);
//		if (r_.HasParseError())
//		{
//			std::cout << "Error  : " << r_.GetParseError() << '\n'
//				<< "Offset : " << r_.GetErrorOffset() << '\n';
//			return EXIT_FAILURE;
//		}
//		try {
//			if (p_->tankIDPass != r_["pass"].GetString()) return -1;
//			bool return_shit = false;
//			for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
//				if (currentPeer->data == NULL) continue;
//				if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(p_->tankIDName)) {
//					if (pInfo(currentPeer)->id != pInfo(peer)->id) {
//						gamepacket_t p;
//						p.Insert("OnConsoleMessage"), p.Insert("`4ALREADY ON?! `o: This account was already online, kicking it off so you can log on. (if you were just playing before, this is nothing to worry about)"), p.CreatePacket(peer), p.CreatePacket(currentPeer);
//						peer->data = currentPeer->data;
//						enet_peer_disconnect_later(currentPeer, 0);
//						enet_peer_disconnect_later(peer, 0);
//						return_shit = true;
//					}
//				}
//			}
//			if (return_shit == true) return -2;
//			p_->growid = true;
//			//json b_ = r_["achievements"].get<json>()
//			for (const auto& point : r_["inv"].GetArray()) {
//				Items item_{};
//				item_.id = point["i"].GetInt();
//				item_.count = point["c"].GetInt();
//				p_->inv.push_back(item_);
//			}
//			/*for (int i_ = 0; i_ < b_.size(); i_++) {
//				p_->achievements.insert({ b_[i_]["title"].GetString(), b_[i_]["progress"].GetInt() });
//			}*/
//			for (const auto& point : r_["friends"].GetArray()) {
//				Friends new_friend_;
//				new_friend_.name = point["name"].GetString();
//				new_friend_.mute = point["mute"].GetBool();
//				new_friend_.block_trade = point["block_trade"].GetBool();
//				new_friend_.last_seen = point["last_seen"].GetBool();
//				p_->friends.push_back(new_friend_);
//			}
//			for (const auto& point : r_["worlds_owned"].GetArray()) {
//				p_->worlds_owned.push_back(point.GetString());
//			}
//			for (const auto& point : r_["la_wo"].GetArray()) {
//				p_->last_visited_worlds.push_back(point.GetString());
//			}
//
//			p_->is_legend = r_["legend"].GetBool();
//			p_->bb = r_["bb"].GetBool();
//			for (const auto& point : r_["t_h"].GetArray()) {
//				p_->trade_history.push_back(point.GetString());
//			}
//
//			p_->total_pineapple = (!r_.HasMember("p1_1")) ? 0 : r_["p1_1"].GetInt();
//			//p_->stage_pineapple = (!(r_.find("p1_2") != r_.end()) ? 0 : r_["p1_2"].get<uint16_t>());
//			p_->personal = !r_.HasMember("poeros4") ? p_->personal : r_["poeros4"].GetString();
//			p_->dd = r_["dd"].GetInt();
//			p_->supp = r_["supp"].GetInt();
//			p_->hs = r_["hs"].GetInt();
//			p_->note = r_["note"].GetString();
//			p_->d_name = r_["d_name"].GetString();
//			p_->gtwl = r_["gtwl"].GetInt();
//			p_->vip = r_["vip"].GetInt();
//			p_->rb = r_["rb"].GetInt();
//			p_->mds = r_["mds"].GetInt();
//			p_->gp = r_["gp"].GetInt();
//			p_->gd = r_["gd"].GetInt();
//			p_->glo = r_["glo"].GetInt();
//			for (const auto& point : r_["glo_p"].GetArray()) {
//				p_->glo_p.push_back(point.GetInt());
//			}
//			for (const auto& point : r_["surg_p"].GetArray()) {
//				p_->surg_p.push_back(point.GetInt());
//			}
//			for (const auto& point : r_["t_p"].GetArray()) {
//				p_->t_p.push_back(point.GetInt());
//			}
//			for (const auto& point : r_["bb_p"].GetArray()) {
//				p_->bb_p.push_back(point.GetInt());
//			}
//			for (const auto& point : r_["ff_p"].GetArray()) {
//				p_->ff_p.push_back(point.GetInt());
//			}
//			for (const auto& point : r_["p_p"].GetArray()) {
//				p_->p_p.push_back(point.GetInt());
//			}
//			for (const auto& point : r_["g_p"].GetArray()) {
//				p_->g_p.push_back(point.GetInt());
//			}
//
//			p_->t_xp = r_["t_xp"].GetInt();
//			p_->bb_xp = r_["bb_xp"].GetInt();
//			p_->ff_xp = r_["ff_xp"].GetInt();
//			p_->s_xp = r_["s_xp"].GetInt();
//			p_->g_xp = r_["g_xp"].GetInt();
//			p_->p_xp = r_["p_xp"].GetInt();
//			p_->t_lvl = r_["t_lvl"].GetInt();
//			p_->bb_lvl = r_["bb_lvl"].GetInt();
//			p_->ff_lvl = r_["ff_lvl"].GetInt();
//			p_->s_lvl = r_["s_lvl"].GetInt();
//			p_->g_lvl = r_["g_lvl"].GetInt();
//			p_->p_lvl = r_["p_lvl"].GetInt();
//			p_->b_l = r_["b_l"].GetInt();
//			for (const auto& point : r_["bp"].GetArray()) {
//				p_->bp.push_back(std::pair(point[0].GetInt(), point[1].GetInt()));
//			}
//
//			p_->flagmay = r_["flagmay"].GetInt();
//			p_->cc = r_["cc"].GetInt();
//			p_->opc = r_["opc"].GetInt();
//			p_->superdev = r_["superdev"].GetInt();
//			p_->b_i = r_["b_i"].GetInt();
//			p_->b_bill = !r_.HasMember("b_bill") ? p_->b_bill : r_["b_bill"].GetString();
//			p_->b_w = r_["b_w"].GetInt();
//			p_->b_p = r_["b_p"].GetInt();
//			p_->surgery_skill = r_["skl"].GetInt();
//			p_->surgery_type = r_["sgt"].GetInt();
//			p_->surgery_done = r_["sd"].GetInt();
//			p_->su_8552_1 = r_["su1"].GetInt();
//			p_->su_8552_2 = r_["su2"].GetInt();
//			p_->b_ra = r_["b_ra"].GetInt();
//			p_->b_lvl = r_["b_lvl"].GetInt();
//			p_->i_11818_1 = r_["i_11818_1"].GetInt();
//			p_->i_11818_2 = r_["i_11818_2"].GetInt();
//			p_->roleSkin = r_["roleSkin"].GetInt();
//			p_->roleIcon = r_["roleIcon"].GetInt();
//			p_->radio = r_["rad"].GetInt();
//			p_->w_w = r_["w_w"].GetInt();
//			p_->w_d = r_["w_d"].GetInt();
//			//p_->egg = (!(r_.find("egg") != r_.end()) ? p_->egg : r_["egg"].GetInt());
//			p_->mercy = r_["mer"].GetInt();
//			p_->drtitle = r_["drtt"].GetInt();
//			p_->drt = r_["_drt"].GetInt();
//			p_->lvl125 = r_["lvl125"].GetInt();
//			p_->donor = r_["dnr"].GetInt();
//			p_->master = r_["mst"].GetInt();
//			p_->all_in = r_["al"].GetInt();
//			p_->upload = !r_.HasMember("upl") ? p_->upload : r_["upl"].GetBool();//2022-05-16
//
//			if (!r_.HasMember("p_7")) {
//				p_->pure_shadow = r_["p_7"].GetInt(); //2022-10/06
//			}
//			/*
//			p_->pinata_day = (!(r_.find("p_d1") != r_.end()) ? p_->pinata_day : r_["p_d1"].GetInt());
//			p_->pinata_prize = (!(r_.find("p_d2") != r_.end()) ? p_->pinata_prize : r_["p_d2"].GetBool());
//			p_->pinata_claimed = (!(r_.find("p_d3") != r_.end()) ? p_->pinata_claimed : r_["p_d3"].GetBool());
//			*/
//
//			p_->summer_surprise = !r_.HasMember("smp") ? 0 : r_["smp"].GetInt();
//			if (r_.HasMember("gr_w"))
//			{
//				for (const auto& point : r_["gr_w"].GetArray()) {
//					p_->growtoken_worlds.push_back(point.GetString());
//				}
//
//			}
//
//			p_->growtoken_day = !r_.HasMember("gr_d") ? p_->growtoken_day : r_["gr_d"].GetInt();
//			p_->grow4good_day = !r_.HasMember("g4g_1") ? p_->grow4good_day : r_["g4g_1"].GetInt();
//			p_->grow4good_gems = !r_.HasMember("g4g_7") ? p_->grow4good_gems : r_["g4g_7"].GetInt();
//			p_->grow4good_surgery = !r_.HasMember("g4g_7") ? p_->grow4good_surgery : r_["g4g_7"].GetInt();
//			p_->grow4good_fish = !r_.HasMember("g4g_10") ? p_->grow4good_fish : r_["g4g_10"].GetInt();
//			p_->grow4good_points = !r_.HasMember("g4g_11") ? p_->grow4good_points : r_["g4g_11"].GetInt();
//			p_->grow4good_30mins = !r_.HasMember("g4g_12") ? p_->grow4good_30mins : r_["g4g_12"].GetInt();
//			p_->grow4good_break = !r_.HasMember("g4g_14") ? p_->grow4good_break : r_["g4g_14"].GetInt();
//			p_->grow4good_place = !r_.HasMember("g4g_15") ? p_->grow4good_place : r_["g4g_15"].GetInt();
//			p_->grow4good_trade = !r_.HasMember("g4g_16") ? p_->grow4good_trade : r_["g4g_16"].GetInt();
//			p_->grow4good_sb = !r_.HasMember("g4g_17") ? p_->grow4good_sb : r_["g4g_17"].GetInt();
//			p_->grow4good_enter = !r_.HasMember("g4g_18") ? p_->grow4good_enter : r_["g4g_18"].GetBool();
//			p_->grow4good_provider = !r_.HasMember("g4g_19") ? p_->grow4good_provider : r_["g4g_19"].GetInt();
//			p_->grow4good_provider2 = !r_.HasMember("g4g_20") ? rand() % 450 + 1 : r_["g4g_20"].GetInt();
//			p_->grow4good_geiger = !r_.HasMember("g4g_21") ? p_->grow4good_geiger : r_["g4g_21"].GetInt();
//			p_->grow4good_geiger2 = !r_.HasMember("g4g_22") ? rand() % 10 + 1 : r_["g4g_22"].GetInt();
//
//			if (!r_.HasMember("7bans"))
//			{
//				for (const auto& point : r_["7bans"].GetArray()) {
//					p_->bans.push_back(point.GetString());
//				}//worlds_owned
//			}
//
//			if (!r_.HasMember("fa_ip")) {
//				p_->fa_ip = r_["fa_ip"].GetString();
//			}
//
//			p_->fa2 = !r_.HasMember("2fa") ? p_->fa2 : r_["2fa"].GetInt();
//			//if (!(r_.find("garuda") == r_.end())) p_->garuda = r_["garuda"].GetInt();
//
//			if (!r_.HasMember("grm")) {
//				p_->growmoji = r_["grm"].GetString();
//			}
//			if (!r_.HasMember("myou")) {
//				p_->mini_you = r_["myou"].GetBool();
//			}
//			if (!r_.HasMember("p_1")) {
//				p_->display_age = r_["p_1"].GetBool();
//				p_->display_home = r_["p_2"].GetBool();
//			}
//			p_->fires = !r_.HasMember("fire") ? p_->fires : r_["fire"].GetInt();
//
//			if (!r_.HasMember("g4g_23")) {
//				struct tm newtime;
//				time_t now = time(0);
//				localtime_s(&newtime, &now);
//				p_->grow4good_week = newtime.tm_yday;
//				p_->grow4good_seed = small_seed_pack[rand() % small_seed_pack.size()];
//			}
//			else {
//				p_->grow4good_week = r_["g4g_23"].GetInt();
//				p_->grow4good_seed = r_["g4g_24"].GetInt();
//				p_->grow4good_seed2 = r_["g4g_25"].GetInt();
//			}
//			if (!r_.HasMember("g4g_26")) {
//				struct tm newtime;
//				time_t now = time(0);
//				localtime_s(&newtime, &now);
//				p_->grow4good_month = newtime.tm_yday;
//			}
//			else {
//				p_->grow4good_combine = r_["g4g_26"].GetInt();
//				p_->grow4good_month = r_["g4g_27"].GetInt();
//				p_->grow4good_crystal = r_["g4g_28"].GetInt();
//				p_->grow4good_honors = r_["g4g_29"].GetInt();
//			}
//
//			if (!r_.HasMember("e_1")) {
//				p_->hair_color = r_["e_1"].GetInt();
//				p_->eye_drop = r_["e_2"].GetInt();
//				p_->eye_lenses = r_["e_3"].GetInt();
//			}
//
//
//			//p_->carnival_credit = (!(r_.find("cnv_1") != r_.end()) ? p_->carnival_credit : r_["cnv_1"].GetInt());
//			p_->pearl = !r_.HasMember("bea_1") ? p_->pearl : r_["bea_1"].GetInt();
//			p_->sprite = !r_.HasMember("spr") ? p_->sprite : r_["spr"].GetInt();
//			p_->wild = !r_.HasMember("spr2") ? p_->wild : r_["spr2"].GetInt();
//
//			p_->tankIDName = r_["name"].GetString();
//			p_->email = r_["email"].GetString();
//			p_->b_r = r_["b_r"].GetString();
//			p_->b_b = r_["b_b"].GetString();
//			p_->b_s = r_["b_s"].GetInt();
//			p_->b_t = r_["b_t"].GetInt();
//			p_->m_r = r_["m_r"].GetString();
//			p_->m_b = r_["m_b"].GetString();
//			p_->punch_count = r_["punch"].GetInt();
//			p_->n = r_["n"].GetInt();
//			p_->ghost = r_["ghost"].GetBool();
//			p_->invis = r_["invis"].GetBool();
//			p_->mod = r_["mod"].GetInt();
//			p_->dev = r_["dev"].GetInt();
//			p_->m_h = r_["m_h"].GetInt();
//			p_->gems = r_["gems"].GetInt();
//			p_->xp = r_["xp"].GetInt();
//			p_->level = r_["level"].GetInt();
//			p_->skin = r_["skin"].GetInt();
//			p_->show_location_ = r_["show_location"].GetBool();
//			p_->show_friend_notifications_ = r_["show_notifications"].GetBool();
//			p_->playtime = time(NULL), p_->seconds = r_["playtime"].GetInt();
//			p_->account_created = r_["date"].GetInt();
//			if (p_->account_created == 0) p_->account_created = time(NULL) / 86400;
//			p_->home_world = r_["home_world"].GetString();
//			p_->hair = r_["hair"].GetInt();
//			p_->shirt = r_["shirt"].GetInt();
//			p_->pants = r_["pants"].GetInt();
//			p_->feet = r_["feet"].GetInt();
//			p_->face = r_["face"].GetInt();
//			p_->ip = r_["ip"].GetString();
//			p_->hand = r_["hand"].GetInt();
//			p_->back = r_["back"].GetInt();
//			p_->mask = r_["mask"].GetInt();
//			p_->necklace = r_["necklace"].GetInt();
//			p_->ances = r_["ances"].GetInt();
//			//	if (r_.find("abcguild") != r_.end()) p_->guild_id = r_["abcguild"].GetInt();
//			if (r_.HasMember("geiger")) {
//				p_->geiger_ = r_["geiger"].GetInt();
//			}
//			if (r_.HasMember("booty_broken")) {
//				p_->booty_broken = r_["booty_broken"].GetInt();
//			}
//			if (r_.HasMember("playmods")) {
//				for (const auto& point : r_["playmods"].GetArray()) {
//					PlayMods give_playmod{};
//					give_playmod.id = point["id"].GetInt();
//					give_playmod.time = point["time"].GetInt();
//					if (r_.HasMember("user")) {
//						give_playmod.user = point["user"].GetString();
//					}
//					p_->playmods.push_back(give_playmod);
//				}
//			}
//			if (r_.HasMember("blarney")) {
//
//				for (const auto& point : r_["blarney"].GetArray()) {
//					int w_ = point["world"].GetInt();
//					long long tim_ = point["time"].GetInt();
//					for (int c_ = 0; c_ < p_->completed_blarneys.size(); c_++) {
//						if ((int)p_->completed_blarneys[c_][0] == w_) {
//							p_->completed_blarneys[c_][1] = tim_;
//							break;
//						}
//					}
//				}
//			}
//		}
//		catch (exception) {
//			return -1;
//		}
//		if (pInfo(peer)->ghost or pInfo(peer)->invis) {
//			if (not pInfo(peer)->mod and not pInfo(peer)->dev) {
//				pInfo(peer)->invis = false;
//			}
//			if (not pInfo(peer)->dev) {
//				pInfo(peer)->ghost = false;
//				if (pInfo(peer)->skin == -2450) pInfo(peer)->skin = 0x8295C3FF;
//			}
//		}
//		if (pInfo(peer)->b_s == 0) {
//			if (pInfo(peer)->supp == 2) {
//				/*gamepacket_t p;
//				p.Insert("OnOverrideGDPRFromServer");
//				p.Insert(39);
//				p.Insert(1);
//				p.Insert(0);
//				p.Insert(1);
//				p.CreatePacket(peer);*/
//				{
//					gamepacket_t p;
//					p.Insert("OnSetRoleSkinsAndTitles");
//					// pirmas - farmer
//					// antras - builder
//					// trecias - surgeon
//					// ketvirtas - fisherman
//					// penktas - chef
//					// sestas - starcaptain
//					string a = (pInfo(peer)->bb_lvl >= 50 ? "1" : "0");
//					string a1 = (pInfo(peer)->bb_lvl >= 50 ? "2" : "0");
//					string farm_set_skins = (pInfo(peer)->t_lvl >= 50 ? "1" : "0") + a + (pInfo(peer)->s_lvl >= 50 ? "1" : "0") + (pInfo(peer)->ff_lvl >= 50 ? "1" : "0");
//					string farm_set_titles = (pInfo(peer)->t_lvl >= 50 ? "2" : "0") + a1 + (pInfo(peer)->s_lvl >= 50 ? "2" : "0") + (pInfo(peer)->ff_lvl >= 50 ? "2" : "0");
//					p.Insert(farm_set_skins + "00");
//					p.Insert(farm_set_titles + "00");
//					p.CreatePacket(peer);
//				}
//			}
//			if (pInfo(peer)->gp == 1) {
//				gamepacket_t p;
//				p.Insert("OnPaw2018SkinColor1Changed");
//				p.Insert(1);
//				p.CreatePacket(peer);
//				{
//					gamepacket_t p;
//					p.Insert("OnPaw2018SkinColor2Changed");
//					p.Insert(1);
//					p.CreatePacket(peer);
//				}
//			}
//			{
//				gamepacket_t p;
//				p.Insert("OnSetBux");
//				p.Insert(pInfo(peer)->gems);
//				p.Insert(1);
//				p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
//				if (pInfo(peer)->supp >= 2) {
//					p.Insert((float)33796, (float)1, (float)0);
//				}
//				p.CreatePacket(peer);
//			}
//			{
//				gamepacket_t p2;
//				p2.Insert("SetHasAccountSecured"), p2.Insert(1), p2.CreatePacket(peer);
//			}
//			gamepacket_t p;
//			p.Insert("SetHasGrowID"), p.Insert(1), p.Insert(p_->tankIDName), p.Insert(p_->tankIDPass), p.CreatePacket(peer);
//			int jau_turi = 0;
//			modify_inventory(peer, 6336, jau_turi);
//			if (jau_turi == 0) modify_inventory(peer, 6336, jau_turi = 1);
//		}
//		return 0;
//	}
//	return -1;
//}



//World get_world(const string& name_) {
//	//int start = clock();
//	/*galima buvo daryti pagal worldid o ne name bet jeigu worldus trinti is memory tai nk gero nebus*/
//	vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
//	if (p != worlds.end()) {
//		return worlds[p - worlds.begin()]; // return worldo struktura   
//	}
//	// Worldo Nera bet jis gali buti issaugotas  
//	string path_ = "worlds/" + name_ + "_.json";
//	if (_access_s(path_.c_str(), 0) == 0) {
//		World world_;
//		try {
//			// Pakrauti worlda is json      
//
//			/*
//			jeigu nera naudoti sita
//						if (world_read.find("rainbows") != world_read.end()) world_.rainbows = world_read["rainbows"].GetBool();
//						*/
//			Document world_read;
//			ifstream world_file(path_, ifstream::binary);
//			IStreamWrapper isw(world_file);
//			world_read.ParseStream(isw);
//			StringBuffer buffer{};
//			Writer<StringBuffer> writer{ buffer };
//			world_read.Accept(writer);
//			const std::string jsonStr{ buffer.GetString() };
//
//			world_.name = name_;
//			world_.entry_level = world_read["entry_level"].GetInt();
//			world_.open_to_public = world_read["public"].GetBool();
//			world_.nuked = world_read["nuked"].GetBool();
//			world_.n_t = world_read["n_t"].GetString();
//			world_.n_b = world_read["n_b"].GetString();
//			world_.music_bpm = world_read["music_bpm"].GetInt();
//			world_.weather = world_read["weather"].GetInt();
//			world_.make_music_blocks_invisible = world_read["invisible_music_blocks"].GetBool();
//			world_.disable_music_blocks = world_read["disable_music_blocks"].GetBool();
//			world_.owner_name = world_read["owner"].GetString();
//			world_.rainbows = world_read["rainbows"].GetBool();
//			world_.silence = world_read["silence"].GetBool();
//			//world_.guild_id = (!(world_read.find("20_guild_id") != world_read.end()) ? world_.guild_id : world_read["20_guild_id"].GetInt());
//			world_.d_weather = world_read["d_weather"].GetInt();
//			world_.owner_named = world_read["o2"].GetString();
//			world_.v_p = world_read["v_p"].GetBool();
//
//			if (jsonStr.find("xeno") != -1) {
//				world_.xenonite = world_read["xeno"].GetInt();
//				world_.xenonite_double_jump = world_read["xeno1"].GetInt();
//				world_.xenonite_high_jump = world_read["xeno2"].GetInt();
//				world_.xenonite_heat_resist = world_read["xeno3"].GetInt();
//				world_.xenonite_strong_punch = world_read["xeno4"].GetInt();
//				world_.xenonite_long_punch = world_read["xeno5"].GetInt();
//				world_.xenonite_speedy = world_read["xeno6"].GetInt();
//				world_.xenonite_powerups = world_read["xeno7"].GetInt();
//				world_.xenonite_long_build = world_read["xeno8"].GetInt();
//			}
//			/*if cant find
//						json gscan_ = json::array();
//			if (world_read.find("gscan") != world_read.end()) gscan_ = world_read["gscan"].Get<json>();
//			*/
//			Document sbx1_; sbx1_.Parse(world_read["sbox1"].GetString());
//			Document gscan_; gscan_.Parse(world_read["gscan"].GetString());
//			Document bulletins_; bulletins_.Parse(world_read["sbull"].GetString());
//			Document cctv_get_settings_; cctv_get_settings_.Parse(world_read["sstv"].GetString());
//			Document cctv_; cctv_.Parse(world_read["cctv"].GetString());
//			Document bullet_; bullet_.Parse(world_read["bulletin"].GetString());
//			Document a_;a_.Parse(world_read["drop"].GetString());
//			Document machine_vector_; machine_vector_.Parse( world_read["machine"].GetString());
//			Document p_;p_.Parse(world_read["blocks"].GetString());
//			Document adm_;
//			//npc_vector_ = world_read["npc"].Get<json>();machine_vector_ = world_read["machine"].Get<json>();
//			for (SizeType i_ = 0; i_ < world_read["admins"].Size(); i_++) {
//				string name_ = adm_[i_]["name"].GetString();
//				world_.admins.push_back(name_);
//			}
//			for (SizeType i_ = 0; i_ < world_read["blocks"].Size(); i_++) {
//				WorldBlock b_{};
//				if (p_[i_].FindMember("f") != p_.MemberEnd()) b_.fg = p_[i_]["f"].GetInt();
//				if (p_[i_].FindMember("b") != p_.MemberEnd()) b_.bg = p_[i_]["b"].GetInt();
//				if (p_[i_].FindMember("p") != p_.MemberEnd()) b_.planted = p_[i_]["p"].Get<long long>();
//				if (p_[i_].FindMember("fc") != p_.MemberEnd()) b_.fruit = p_[i_]["fc"].GetInt();
//				if (p_[i_].FindMember("sp") != p_.MemberEnd()) b_.sp = p_[i_]["sp"].GetInt();
//				if (p_[i_].FindMember("t") != p_.MemberEnd()) b_.txt = p_[i_]["t"].GetString();
//				if (p_[i_].FindMember("n") != p_.MemberEnd()) b_.roll = p_[i_]["n"].GetInt();
//				if (p_[i_].FindMember("id") != p_.MemberEnd()) b_.id = p_[i_]["id"].GetInt();
//				if (p_[i_].FindMember("c") != p_.MemberEnd()) b_.c_ = p_[i_]["c"].GetInt();
//				if (p_[i_].FindMember("wl") != p_.MemberEnd()) b_.wl = p_[i_]["wl"].GetInt();
//				if (p_[i_].FindMember("pr") != p_.MemberEnd()) b_.pr = p_[i_]["pr"].GetInt();
//				if (p_[i_].FindMember("o") != p_.MemberEnd()) b_.open = p_[i_]["o"].GetInt();
//				if (p_[i_].FindMember("dd") != p_.MemberEnd()) b_.door_destination = p_[i_]["dd"].GetString();
//				if (p_[i_].FindMember("di") != p_.MemberEnd()) b_.door_id = p_[i_]["di"].GetString();
//				if (p_[i_].FindMember("fl") != p_.MemberEnd()) {
//					b_.flags = p_[i_]["fl"].GetInt();
//					if (b_.flags & 0x00400000 && (b_.fg == 4992 || b_.fg == 226 or b_.fg == 3616 or b_.fg == 1276 or b_.fg == 1278 or b_.fg == 4758 or b_.fg == 3750 or b_.fg == 4992 or b_.fg == 3072 or b_.fg == 4884)) world_.active_jammers.push_back(b_.fg);
//				}
//				//if (p_[i_].find("ps") != p_[i_].end()) b_.projectile_speed = p_[i_]["ps"].GetInt();
//				//if (p_[i_].find("rf") != p_[i_].end()) b_.rate_of_fire = p_[i_]["rf"].GetInt();
//				if (p_[i_].FindMember("en") != p_.MemberEnd()) b_.enabled = p_[i_]["en"].GetBool();
//				if (p_[i_].FindMember("in") != p_.MemberEnd()) b_.invert = p_[i_]["in"].GetBool();
//				if (p_[i_].FindMember("sn") != p_.MemberEnd()) b_.spin = p_[i_]["sn"].GetBool();
//				if (p_[i_].FindMember("g") != p_.MemberEnd()) b_.gravity = p_[i_]["g"].GetInt();
//				if (p_[i_].FindMember("m") != p_.MemberEnd()) b_.magnetron = p_[i_]["m"].GetBool();
//				if (p_[i_].FindMember("l") != p_.MemberEnd()) b_.locked = p_[i_]["l"].GetBool();
//				if (p_[i_].FindMember("lq") != p_.MemberEnd()) b_.lock_origin = p_[i_]["lq"].GetInt();
//				if (p_[i_].FindMember("lo") != p_.MemberEnd()) b_.owner_name = p_[i_]["lo"].GetString();
//				if (p_[i_].FindMember("ea") != p_.MemberEnd()) b_.empty_air = p_[i_]["ea"].GetBool();
//				if (p_[i_].FindMember("otp") != p_.MemberEnd()) b_.open_to_public = p_[i_]["otp"].GetBool();
//				if (p_[i_].FindMember("ad") != p_.MemberEnd()) {
//					Document read_block_admins_;
//					read_block_admins_.Parse(p_[i_]["ad"].GetString());
//					for (SizeType i_2 = 0; i_2 < read_block_admins_.Size(); i_2++) {
//						string name_ = read_block_admins_[i_2]["name"].GetString();
//						b_.admins.push_back(name_);
//					}
//				}
//				if (p_[i_].FindMember("bo") != p_.MemberEnd()) b_.build_only = p_[i_]["bo"].GetBool();
//				if (p_[i_].FindMember("la") != p_.MemberEnd()) b_.limit_admins = p_[i_]["la"].GetBool();
//					if (p_[i_].FindMember("cr") != p_.MemberEnd()) b_.crystals = p_[i_]["cr"].Get<vector<vector<int>>>();
//					if (p_[i_].FindMember("mq") != p_.MemberEnd()) b_.mannequin = p_[i_]["mq"].Get<Mannequin>();
//					if (p_[i_].FindMember("dnt") != p_.MemberEnd()) {
//						Document read_block_donation_;
//						read_block_donation_.Parse(p_[i_]["dnt"].GetString());
//							for (SizeType i_2 = 0; i_2 < read_block_donation_.Size(); i_2++) {
//								Donate new_donate;
//									new_donate.item = read_block_donation_[i_2]["item"].GetInt();
//									new_donate.count = read_block_donation_[i_2]["count"].GetInt();
//									new_donate.text = read_block_donation_[i_2]["text"].GetString();
//									new_donate.name = read_block_donation_[i_2]["name"].GetString();
//									b_.donates.push_back(new_donate);
//							}
//					}
//				if (p_[i_].FindMember("r1") != p_.MemberEnd()) b_.r = p_[i_]["r1"].GetInt();
//				if (p_[i_].FindMember("g1") != p_.MemberEnd()) b_.g = p_[i_]["g1"].GetInt();
//				if (p_[i_].FindMember("b1") != p_.MemberEnd()) b_.b = p_[i_]["b1"].GetInt();
//				if (p_[i_].FindMember("prs") != p_.MemberEnd()) b_.portrait = p_[i_]["prs"].Get<Portrait>();
//				if (p_[i_].FindMember("bep") != p_.MemberEnd()) b_.bunny_egg_progress = p_[i_]["bep"].GetInt();
//				if (p_[i_].FindMember("hm") != p_.MemberEnd()) b_.heart_monitor = p_[i_]["hm"].GetString();
//				if (p_[i_].FindMember("sh1") != p_.MemberEnd()) b_.shelf_1 = p_[i_]["sh1"].GetInt();
//				if (p_[i_].FindMember("sh2") != p_.MemberEnd()) b_.shelf_2 = p_[i_]["sh2"].GetInt();
//				if (p_[i_].FindMember("sh3") != p_.MemberEnd()) b_.shelf_3 = p_[i_]["sh3"].GetInt();
//				if (p_[i_].FindMember("sh4") != p_.MemberEnd()) b_.shelf_4 = p_[i_]["sh4"].GetInt();
//				world_.blocks.push_back(b_);
//			}
//			for (SizeType i_ = 0; i_ < world_read["drop"].Size(); i_++) {
//				WorldDrop b_{};
//				b_.id = a_[i_]["i"].GetInt();
//				b_.count = a_[i_]["c"].GetInt();
//				b_.uid = a_[i_]["u"].GetInt();
//				b_.x = a_[i_]["x"].GetInt();
//				b_.y = a_[i_]["y"].GetInt();
//				world_.drop.push_back(b_);
//			}
//			for (SizeType i_ = 0; i_ < world_read["sbox1"].Size(); i_++) {
//				WorldSBOX1 db_{};
//				db_.id = sbx1_[i_]["i"].GetInt();
//				db_.count = sbx1_[i_]["c"].GetInt();
//				db_.x = sbx1_[i_]["x"].GetInt();
//				db_.y = sbx1_[i_]["y"].GetInt();
//				world_.sbox1.push_back(db_);
//			}
//			for (SizeType i_ = 0; i_ < gscan_.Size(); i_++) {
//				WorldGrowscan db_{};
//				db_.x = gscan_[i_]["x"].GetInt();
//				db_.y = gscan_[i_]["y"].GetInt();
//				db_.world_public = gscan_[i_]["world_public"].GetBool();
//				db_.floating_public = gscan_[i_]["floating_public"].GetBool();
//				world_.gscan.push_back(db_);
//			}
//			for (SizeType i_ = 0; i_ < cctv_get_settings_.Size(); i_++) {
//				WorldCCTVSettings db_{};
//				db_.x = cctv_get_settings_[i_]["x"].GetInt();
//				db_.y = cctv_get_settings_[i_]["y"].GetInt();
//				db_.show_item_taking = cctv_get_settings_[i_]["s1"].GetBool();
//				db_.show_item_dropping = cctv_get_settings_[i_]["s2"].GetBool();
//				db_.show_people_entering = cctv_get_settings_[i_]["s3"].GetBool();
//				db_.show_people_exiting = cctv_get_settings_[i_]["s4"].GetBool();
//				db_.dont_show_owner = cctv_get_settings_[i_]["s5"].GetBool();
//				db_.dont_show_admins = cctv_get_settings_[i_]["s6"].GetBool();
//				db_.dont_show_noaccess = cctv_get_settings_[i_]["s7"].GetBool();
//				
//				db_.show_vend_logs = (!(cctv_get_settings_[i_].FindMember("sh2") != cctv_get_settings_.MemberEnd()) ? false : cctv_get_settings_[i_]["s8"].GetBool());
//				world_.cctv_settings.push_back(db_);
//			}
//			for (SizeType i_ = 0; i_ < cctv_.Size(); i_++) {
//				WorldCCTV db_{};
//				db_.x = cctv_[i_]["x"].GetInt();
//				db_.y = cctv_[i_]["y"].GetInt();
//				db_.logs = cctv_[i_]["l"].GetString();
//				world_.cctv.push_back(db_);
//			}
//			for (SizeType i_ = 0; i_ < bullet_.Size(); i_++) {
//				WorldBulletin db_{};
//				db_.x = bullet_[i_]["x"].GetInt();
//				db_.y = bullet_[i_]["y"].GetInt();
//				db_.name = bullet_[i_]["n"].GetString();
//				db_.text = bullet_[i_]["t"].GetString();
//				world_.bulletin.push_back(db_);
//			}
//			for (SizeType i_ = 0; i_ < bulletins_.Size(); i_++) {
//				WorldBulletinSettings db_{};
//				db_.x = bulletins_[i_]["x"].GetInt();
//				db_.y = bulletins_[i_]["y"].GetInt();
//				db_.public_can_add = bulletins_[i_]["s1"].GetBool();
//				db_.hide_names = bulletins_[i_]["s2"].GetBool();
//				world_.bulletins.push_back(db_);
//			}
//			/*
//			for (int i_ = 0; i_ < npc_vector_.size(); i_++) {
//				WorldNPC npc_{};
//				npc_.id = npc_vector_[i_]["i"].GetInt();
//				npc_.x = npc_vector_[i_]["x"].GetInt();
//				npc_.y = npc_vector_[i_]["y"].GetInt();
//				npc_.projectile_speed = npc_vector_[i_]["ps"].GetInt();
//				npc_.rate_of_fire = npc_vector_[i_]["rf"].GetInt();
//				npc_.kryptis = npc_vector_[i_]["kr"].Get<float>();
//				npc_.enabled = npc_vector_[i_]["en"].GetBool();
//				npc_.last_ = npc_vector_[i_]["l"].Get<long long>();
//				npc_.started_moving = npc_vector_[i_]["sm"].Get<long long>();
//				world_.npc.push_back(npc_);
//			}*/
//			for (SizeType i_ = 0; i_ < machine_vector_.Size(); i_++) {
//				WorldMachines machine_{};
//				machine_.id = machine_vector_[i_]["i"].GetInt();
//				machine_.x = machine_vector_[i_]["x"].GetInt();
//				machine_.y = machine_vector_[i_]["y"].GetInt();
//				machine_.target_item = machine_vector_[i_]["ti"].GetInt();
//				machine_.enabled = machine_vector_[i_]["en"].GetBool();
//				machine_.last_ = machine_vector_[i_]["l"].Get<long long>();
//				world_.machines.push_back(machine_);
//			}
//			//int end = clock();//Now check what amount of ticks we have now. 
//			//std::cout << name_ << " : it took " << end - start << "ticks, or " << ((float)end - start) / CLOCKS_PER_SEC << "seconds." << std::endl;
//		}
//		catch (exception err) {
//			cout << "world load error name: " << name_ << endl;
//			remove(path_.c_str());
//			world_ = create_world(name_, 6000);
//		}
//		worlds.push_back(world_);
//		if (/*world_.npc.size() != 0 or */world_.machines.size() != 0) {
//			if (find(t_worlds.begin(), t_worlds.end(), world_.name) == t_worlds.end()) {
//				t_worlds.push_back(world_.name);
//			}
//		}
//		return world_;
//	}
//	return create_world(name_, 6000);
//}
