#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "enet/include/enet.h"
#include <nlohmann/json.hpp>
#include "Item.h"
#include "Base.h"
#include "Player.h"
#include "Packet.h"
#include <boost/thread/thread.hpp>

//#include "Guilds.h"
#include "World.h"
#pragma comment(lib, "Ws2_32.lib")
BOOL WINAPI ConsoleHandler(DWORD dwType)
{
	switch (dwType) {
	case CTRL_LOGOFF_EVENT: case CTRL_SHUTDOWN_EVENT: case CTRL_CLOSE_EVENT:
	{
		trigger_save_(false);
		return TRUE;
	}
	default:
	{
		break;
	}
	}
	return FALSE;
}
void sendItemDataToPeer(ENetPeer* peer)
{

	ENetPacket* pkt = enet_packet_create(item_data, item_data_size + 60, ENET_PACKET_FLAG_RELIABLE);
	std::cout << "ItemDat Packet Size: " << pkt->dataLength << endl;
	enet_peer_send(peer, 0, pkt);
	 enet_peer_send(peer, 0, pkt); 
}

void loop_worlds() {
	if (f_saving_ == false) {
		long long ms_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count(), time_ = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		/*
		if (last_hm_time - ms_time <= 0) {
			last_hm_time = ms_time + 120000;
			for (int i = 0; i < monitors.size(); i++) {
				string name_ = monitors[i].world_name;
				vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
				if (p != worlds.end()) {
					World* world_ = &worlds[p - worlds.begin()];
					WorldBlock* monitor = &world_->blocks[monitors[i].x + (monitors[i].y * 100)];
					if (!items[monitor->fg].heart_monitor) {
						monitors.erase(monitors.begin() + i);
						i--;
						continue;
					}
					monitors[i].active = 0;
					string find_mon = monitor->heart_monitor;
					if (find_mon.size() >= 2) find_mon.resize(find_mon.size() - 2); // remove `` is galo
					for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
						if (pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName == find_mon) {
							monitors[i].active = 1;
							break;
						}
					}
					PlayerMoving data_{};
					data_.packetType = 5, data_.punchX = monitors[i].x, data_.punchY = monitors[i].y, data_.characterState = 0x8;
					BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, monitor));
					BYTE* blc = raw + 56;
					form_visual(blc, *monitor, *world_, NULL, false, false, monitors[i].x, monitors[i].y);
					for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
						if (pInfo(currentPeer)->world == world_->name) {
							send_raw(currentPeer, 4, raw, 112 + alloc_(world_, monitor), ENET_PACKET_FLAG_RELIABLE);
						}
					}
					delete[] raw, blc;
				}
			}
		}*/

		/*
		if (last_beach_event != 0 and (last_beach_event - ms_time <= 0 || beach_players.size() == 0)) {
			if (beach_players.size() != 0) {
				gamepacket_t p;
				p.Insert("OnEndMission");
				for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
					if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != "BEACHPARTYGAME") continue;
					if (find(beach_players.begin(), beach_players.end(), pInfo(currentPeer)->tankIDName) != beach_players.end()) {
						pInfo(currentPeer)->c_x = 0, pInfo(currentPeer)->c_y = 0;
						SendRespawn(currentPeer, true, 0, 1);
						p.CreatePacket(currentPeer);
					}
				}
				beach_players.clear();
			}
			for (int i_ = 0; i_ < change_id_beach.size(); i_++) update_tile(NULL, change_id_beach[i_] % 100, change_id_beach[i_] / 100, 8676, false, true);
			last_beach_event = 0;
			string name_ = "BEACHPARTYGAME";
			vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
			if (p != worlds.end()) {
				World* world_ = &worlds[p - worlds.begin()];
				for (uint16_t i_ = 4400; i_ < 5300; i_++) if (world_->blocks[i_].fg != 12252) update_tile(NULL, (i_ % 100), (i_ / 100), world_->blocks[i_].fg = 12252, false, true);
			}
		}*/
		if (last_honors_reset - ms_time <= 0) {
			cout << "r h" << endl;
			honors_reset();
			last_honors_reset = ms_time + 3600000;
			cout << "d h" << endl;
		}
		if (last_world_menu - ms_time <= 0) {
			fstream newfile;
			newfile.open("C:\\Users\\admin\\Desktop\\SERVER\\x64\\Release\\db\\deposit.txt", ios::in); //open a file to perform read operation using file object
			if (newfile.is_open()) { //checking whether the file is open
				newfile >> depositworld;
				newfile.close(); //close the file object.
			}
			cout << "r w " << endl;
			active_world_list = "";
			active_world_list.clear();
			sort(top_active_worlds.begin(), top_active_worlds.end());
			reverse(top_active_worlds.begin(), top_active_worlds.end());
			top_active_worlds.resize((top_active_worlds.size() >= 24 ? 24 : top_active_worlds.size()));
			for (uint8_t i = 0; i < top_active_worlds.size(); i++) {
					int w_cz = 0;
					for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != top_active_worlds[i].second) continue;
						w_cz++;
					}
					active_world_list += "\nadd_floater|" + top_active_worlds[i].second + "|" + to_string(w_cz) + "|" + to_string(0.5 - (i * 0.005)) + "|3529161471";
			}
			top_active_worlds.clear();
			if (active_world_list.empty()) active_world_list = "\nadd_floater|START|0|0.5|3529161471";
			last_world_menu = ms_time + 60000;
			cout << "d w" << endl;
		}
		if (last_hm_time - ms_time <= 0) {
			cout << "r g" << endl;
			vector<pair<int, string>> top_tier = top_basher;
			top_basher_list = "", top_basher_list_shop = "";
			sort(top_tier.begin(), top_tier.end());
			reverse(top_tier.begin(), top_tier.end());
			top_tier.resize((top_tier.size() >= 10 ? 10 : top_tier.size()));
			for (uint8_t i = 0; i < top_tier.size(); i++) {
				top_basher_list += "\nadd_smalltext|" + to_string(i + 1) + " : " + top_tier[i].second + " , Score: " + setGems(top_tier[i].first) + "|";
				if (i < 5) top_basher_list_shop += "<CR>" + to_string(i + 1) + " : " + top_tier[i].second + " , Score: " + setGems(top_tier[i].first) + "";
			}
			if (top_basher_list.empty()) top_basher_list = "\nadd_smalltext|The list should update in few minutes..|";
			last_hm_time = ms_time + 600000;
			cout << "d g" << endl;
		}
		if (last_time - ms_time <= 0) {
			last_time = ms_time + 1300;
			for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED || currentPeer->data == NULL or pInfo(currentPeer)->tankIDName.empty()) continue;
				if (pInfo(currentPeer)->fishing_used != 0) {
					if (pInfo(currentPeer)->last_fish_catch + pInfo(currentPeer)->fish_seconds < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count() && rand() % 100 < (pInfo(currentPeer)->hand == 3010 ? 7 : 4)) {
						PlayerMoving data_{};
						data_.packetType = 17, data_.netID = 34, data_.YSpeed = 34, data_.x = pInfo(currentPeer)->f_x * 32 + 16, data_.y = pInfo(currentPeer)->f_y * 32 + 16;
						pInfo(currentPeer)->last_fish_catch = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
						BYTE* raw = packPlayerMoving(&data_);
						gamepacket_t p3(0, pInfo(currentPeer)->netID);
						p3.Insert("OnPlayPositioned"), p3.Insert("audio/splash.wav");
						for (ENetPeer* currentPeer_event = server->peers; currentPeer_event < &server->peers[server->peerCount]; ++currentPeer_event) {
							if (currentPeer_event->state != ENET_PEER_STATE_CONNECTED or currentPeer_event->data == NULL or pInfo(currentPeer_event)->world != pInfo(currentPeer)->world) continue;
							send_raw(currentPeer_event, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE), p3.CreatePacket(currentPeer_event);
						}
						delete[] raw;
					}
				}
				if (not pInfo(currentPeer)->world.empty()) {
					string find = pInfo(currentPeer)->world;
					vector<pair<int, string>>::iterator p = find_if(top_active_worlds.begin(), top_active_worlds.end(), [find](const pair < int, string>& element) { return element.second == find; });
					if (p != top_active_worlds.end()) top_active_worlds[p - top_active_worlds.begin()].first++;
					else top_active_worlds.push_back(make_pair(1, find));
				}
				if (pInfo(currentPeer)->save_time + 600000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
					if (pInfo(currentPeer)->save_time != 0) {
						pInfo(currentPeer)->opc++;
						if (pInfo(currentPeer)->gp) pInfo(currentPeer)->opc++;
						string extra = "";
						if (pInfo(currentPeer)->pants == 9782) pInfo(currentPeer)->opc++, extra = "`9+1 [" + items[pInfo(currentPeer)->pants].ori_name + "]``, ";
						if (pInfo(currentPeer)->hand == 9766 || pInfo(currentPeer)->hand == 9772) pInfo(currentPeer)->opc++, extra = "`9+1 [" + items[pInfo(currentPeer)->hand].ori_name + "]``, ";
						add_honors(pInfo(currentPeer)->world);
						if (pInfo(currentPeer)->grow4good_30mins < 30) daily_quest(currentPeer, false, "30mins", 10);
						gamepacket_t p2;
						p2.Insert("OnConsoleMessage"), p2.Insert(a + "You earned 1" + (pInfo(currentPeer)->gp ? "`2+1``" : "") + extra + " OPC for staying active in the game!"), p2.CreatePacket(currentPeer);
						loop_save(currentPeer);
					}
					pInfo(currentPeer)->save_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
				}
				if (pInfo(currentPeer)->hand == 3578) {
					if (pInfo(currentPeer)->hand_torch + 60000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
						if (pInfo(currentPeer)->hand_torch != 0) {
							int got = 0;
							modify_inventory(currentPeer, 3578, got);
							if (got - 1 >= 1) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(currentPeer)->netID), p.Insert("`4My torch went out, but I have " + to_string(got - 1) + " more!``"), p.CreatePacket(currentPeer);
							}
							modify_inventory(currentPeer, 3578, got = -1);
						}
						pInfo(currentPeer)->hand_torch = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
					}
				}
				if (pInfo(currentPeer)->rb == 1 && pInfo(currentPeer)->name_time + 250 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
					pInfo(currentPeer)->name_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
					string msg2 = pInfo(currentPeer)->name_color;
					if (pInfo(currentPeer)->is_legend) msg2 = "`" + to_string(rand() % 9 + 1) + pInfo(currentPeer)->tankIDName;
					else for (int i = 0; i < pInfo(currentPeer)->tankIDName.length(); i++) msg2 += "`" + to_string(rand() % 9 + 1) + "" + pInfo(currentPeer)->tankIDName[i];
					ChangeName(currentPeer, msg2);
				}
				if (pInfo(currentPeer)->hand == 2204 and pInfo(currentPeer)->x != -1 and pInfo(currentPeer)->y != -1) {
					if (not has_playmod(pInfo(currentPeer), "Irradiated")) {
						if (pInfo(currentPeer)->geiger_x == -1 and pInfo(currentPeer)->geiger_y == -1) {
							pInfo(currentPeer)->geiger_x = (rand() % 100) * 32;
							pInfo(currentPeer)->geiger_y = (rand() % 54) * 32;
						}
						int a_ = pInfo(currentPeer)->geiger_x + ((pInfo(currentPeer)->geiger_y * 100) / 32);
						int b_ = pInfo(currentPeer)->x + ((pInfo(currentPeer)->y * 100) / 32);
						int diff = abs(a_ - b_) / 32;
						if (diff > 30) { // nieko

						}
						else if (diff >= 30) { // raudona
							if (pInfo(currentPeer)->geiger_time + 1500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
								pInfo(currentPeer)->geiger_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
								PlayerMoving data_{};
								data_.packetType = 17, data_.x = pInfo(currentPeer)->x + 10, data_.y = pInfo(currentPeer)->y + 16, data_.characterState = 0x8;
								data_.XSpeed = 0, data_.YSpeed = 114;
								BYTE* raw = packPlayerMoving(&data_, 56);
								raw[3] = 114;
								*(double*)(raw + 40) = -4.13;
								send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
								delete[] raw;
							}
						}
						else if (diff >= 15) { // geltona
							if (pInfo(currentPeer)->geiger_time + 1500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
								pInfo(currentPeer)->geiger_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
								PlayerMoving data_{};
								data_.packetType = 17, data_.x = pInfo(currentPeer)->x + 10, data_.y = pInfo(currentPeer)->y + 16, data_.characterState = 0x8;
								data_.XSpeed = 1, data_.YSpeed = 114;
								BYTE* raw = packPlayerMoving(&data_, 56);
								raw[3] = 114;
								*(double*)(raw + 40) = -4.13;
								send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
								delete[] raw;
							}
						}
						else { // zalia
							if (diff <= 1) { // surado
								{
									if (pInfo(currentPeer)->geiger_time + 2500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
										pInfo(currentPeer)->geiger_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
										pInfo(currentPeer)->geiger_x = -1, pInfo(currentPeer)->geiger_y = -1;
										{
											int c_ = -1;
											modify_inventory(currentPeer, 2204, c_);
											int c_2 = 1;
											if (modify_inventory(currentPeer, 2286, c_2) != 0) {
												string name_ = pInfo(currentPeer)->world;
												vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
												if (p != worlds.end()) {
													World* world_ = &worlds.at(p - worlds.begin());
													WorldDrop drop_block_{};
													drop_block_.id = 2286, drop_block_.count = 1, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = pInfo(currentPeer)->x + rand() % 17, drop_block_.y = pInfo(currentPeer)->y + rand() % 17;
													dropas_(world_, drop_block_);
												}
											}
											PlayMods give_playmod{};
											give_playmod.id = 10;
											give_playmod.time = time(nullptr) + (thedaytoday == 3 ? 600 : 1800);
											pInfo(currentPeer)->playmods.push_back(give_playmod);
											pInfo(currentPeer)->hand = 2286;
											update_clothes(currentPeer);
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("You are aglow with radiation! (`$Irradiated`` mod added, `$" + a + (thedaytoday == 3 ? "1" : "3") + "0 mins`` left)");
											p.CreatePacket(currentPeer);
											packet_(currentPeer, "action|play_sfx\nfile|audio/dialog_confirm.wav\ndelayMS|0");
										}
										if (pInfo(currentPeer)->grow4good_geiger < pInfo(currentPeer)->grow4good_geiger2 && pInfo(currentPeer)->grow4good_geiger != -1) daily_quest(currentPeer, false, "geiger", 1);
										add_event_xp(currentPeer, 1, "geiger");
										vector<int> geiger_items = { 6416,3196,2206,1500,1498,2806,2804,8270,8272,8274,2244,2246,2242,3792,3306,4676,4678,4680,4682,4652,4650,4648,4646,11186,10086 };
										if (rand() % 50 < 10) geiger_items = { 2248,2250,3792,10084, 12502 };
										if (rand() % 50 < 2) geiger_items = { 4654 , 9380 , 11562 };
										int item_ = geiger_items.at(rand() % geiger_items.size());
										int c_ = 1;
										if (modify_inventory(currentPeer, item_, c_) != 0) {
											string name_ = pInfo(currentPeer)->world;
											vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
											if (p != worlds.end()) {
												World* world_ = &worlds.at(p - worlds.begin());
												WorldDrop drop_block_{};
												drop_block_.id = item_, drop_block_.count = 1, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = pInfo(currentPeer)->x + rand() % 17, drop_block_.y = pInfo(currentPeer)->y + rand() % 17;
												dropas_(world_, drop_block_);
											}
										}
										gamepacket_t p;
										p.Insert("OnParticleEffect");
										p.Insert(48);
										p.Insert((float)pInfo(currentPeer)->x + 10, (float)pInfo(currentPeer)->y + 16);
										p.CreatePacket(currentPeer);
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(currentPeer)->netID);
											p.Insert("I found `21 " + items.at(item_).name + "``!");
											p.Insert(0);
											p.CreatePacket(currentPeer);
											gamepacket_t p2;
											p2.Insert("OnConsoleMessage");
											p2.Insert(pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName + "`` found `21 " + items.at(item_).name + "``!");
											PlayerMoving data_{};
											data_.packetType = 19, data_.plantingTree = 0, data_.netID = 0;
											data_.punchX = item_;
											data_.x = pInfo(currentPeer)->x + 10, data_.y = pInfo(currentPeer)->y + 16;
											int32_t to_netid = pInfo(currentPeer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											for (ENetPeer* currentPeer2 = server->peers; currentPeer2 < &server->peers[server->peerCount]; ++currentPeer2) {
												if (currentPeer2->state != ENET_PEER_STATE_CONNECTED or currentPeer2->data == NULL) continue;
												if (pInfo(currentPeer2)->world == pInfo(currentPeer)->world) {
													send_raw(currentPeer2, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
													p2.CreatePacket(currentPeer2);
												}
											}
											delete[]raw;
										}
									}
								}
							}
							else {
								int t_ = 0;
								if (diff >= 6) t_ = 1350;
								else t_ = 1000;
								if (pInfo(currentPeer)->geiger_time + t_ < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
									pInfo(currentPeer)->geiger_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
									PlayerMoving data_{};
									data_.packetType = 17, data_.x = pInfo(currentPeer)->x + 10, data_.y = pInfo(currentPeer)->y + 16, data_.characterState = 0x8;
									data_.XSpeed = 2, data_.YSpeed = 114;
									BYTE* raw = packPlayerMoving(&data_, 56);
									raw[3] = 114;
									*(double*)(raw + 40) = -4.13;
									send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									delete[] raw;
								}
							}
						}
					}
				}
				long long time_ = time(nullptr);
				for (int i_ = 0; i_ < pInfo(currentPeer)->playmods.size(); i_++) {
					if (pInfo(currentPeer)->playmods[i_].id == 12) {
						if (pInfo(currentPeer)->valentine_time + 2500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
							pInfo(currentPeer)->valentine_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
							for (ENetPeer* valentine = server->peers; valentine < &server->peers[server->peerCount]; ++valentine) {
								if (valentine->state != ENET_PEER_STATE_CONNECTED or valentine->data == NULL) continue;
								if (pInfo(valentine)->world == pInfo(currentPeer)->world and pInfo(valentine)->tankIDName == pInfo(currentPeer)->playmods[i_].user) {
									if (not pInfo(valentine)->invis and not pInfo(currentPeer)->invis and pInfo(currentPeer)->x != -1 and pInfo(currentPeer)->y != -1 and pInfo(valentine)->x != -1 and pInfo(valentine)->y != -1) {
										gamepacket_t p;
										p.Insert("OnParticleEffect");
										p.Insert(13);
										p.Insert((float)pInfo(valentine)->x + 10, (float)pInfo(valentine)->y + 16);
										p.Insert((float)0), p.Insert((float)pInfo(currentPeer)->netID);
										bool double_send = false;
										for (int i_2 = 0; i_2 < pInfo(valentine)->playmods.size(); i_2++) {
											if (pInfo(valentine)->playmods[i_2].id == 12 and pInfo(valentine)->playmods[i_2].user == pInfo(currentPeer)->tankIDName) {
												double_send = true;
												break;
											}
										}
										gamepacket_t p2;
										p2.Insert("OnParticleEffect");
										p2.Insert(13);
										p2.Insert((float)pInfo(currentPeer)->x + 10, (float)pInfo(currentPeer)->y + 16);
										p2.Insert((float)0), p2.Insert((float)pInfo(valentine)->netID);
										for (ENetPeer* valentine_bc = server->peers; valentine_bc < &server->peers[server->peerCount]; ++valentine_bc) {
											if (valentine_bc->state != ENET_PEER_STATE_CONNECTED or valentine_bc->data == NULL) continue;
											if (pInfo(valentine_bc)->world == pInfo(currentPeer)->world) {
												p.CreatePacket(valentine_bc);
												if (double_send) p2.CreatePacket(valentine_bc);
											}
										}
									}
									break;
								}
							}
						}
					}
					if (pInfo(currentPeer)->playmods[i_].time - time_ < 0) {
						for (vector<string> get_ : info_about_playmods) {
							uint32_t playmod_id = atoi(get_[0].c_str());
							if (playmod_id == pInfo(currentPeer)->playmods[i_].id) {
								string playmod_name = get_[2];
								string playmod_on_remove = get_[4];
								pInfo(currentPeer)->playmods.erase(pInfo(currentPeer)->playmods.begin() + i_);
								packet_(currentPeer, "action|play_sfx\nfile|audio/dialog_confirm.wav\ndelayMS|0");
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert(playmod_on_remove + " (`$" + playmod_name + "`` mod removed)");
								p.CreatePacket(currentPeer);
								update_clothes(currentPeer);
								break;
							}
						}
					}
				}
			}
		}
		if (last_time_ - ms_time <= 0) {
			last_time_ = ms_time + 500;
			for (int a = 0; a < t_worlds.size(); a++) {
				string name = t_worlds[a];
				vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name](const World& a) { return a.name == name; });
				if (p != worlds.end()) {
					World* world = &worlds[p - worlds.begin()];
					if (world->special_event && world->last_special_event + 30000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
						gamepacket_t p, p2;
						p.Insert("OnAddNotification"), p.Insert("interface/large/special_event.rttex"), p.Insert("`2" + items[world->special_event_item].event_name + ":`` " + (items[world->special_event_item].event_total == 1 ? "`oTime's up! Nobody found it!``" : "`oTime's up! " + to_string(world->special_event_item_taken) + " of " + to_string(items[world->special_event_item].event_total) + " items found.``") + ""), p.Insert("audio/cumbia_horns.wav"), p.Insert(0);
						p2.Insert("OnConsoleMessage"), p2.Insert("`2" + items[world->special_event_item].event_name + ":`` " + (items[world->special_event_item].event_total == 1 ? "`oTime's up! Nobody found it!``" : "`oTime's up! " + to_string(world->special_event_item_taken) + " of " + to_string(items[world->special_event_item].event_total) + " items found.``") + "");
						for (ENetPeer* currentPeer_event = server->peers; currentPeer_event < &server->peers[server->peerCount]; ++currentPeer_event) {
							if (currentPeer_event->state != ENET_PEER_STATE_CONNECTED or currentPeer_event->data == NULL or pInfo(currentPeer_event)->world != world->name) continue;
							p.CreatePacket(currentPeer_event), p2.CreatePacket(currentPeer_event);
							PlayerMoving data_{};
							data_.effect_flags_check = 1, data_.packetType = 14, data_.netID = 0;
							for (int i_ = 0; i_ < world->drop.size(); i_++) {
								if (world->drop[i_].special == true) {
									data_.plantingTree = world->drop[i_].uid;
									BYTE* raw = packPlayerMoving(&data_);
									send_raw(currentPeer_event, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									delete[]raw;
									world->drop[i_].id = 0, world->drop[i_].x = -1, world->drop[i_].y = -1;
								}
							}
						}
						world->last_special_event = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count(), world->special_event_item = 0, world->special_event_item_taken = 0, world->special_event = false;
					}
					if (world->machines.size() == 0 /* and world->npc.size() == 0*/) {
						t_worlds.erase(t_worlds.begin() + a);
						a--;
						continue;
					}
					int ySize = world->blocks.size() / 100, xSize = world->blocks.size() / ySize;
					//vector<int> alr_done;
					for (int i_ = 0; i_ < world->machines.size(); i_++) {
						WorldMachines* machine = &world->machines[i_];
						if (not machine->enabled or machine->target_item == 0) continue;
						WorldBlock* itemas = &world->blocks[machine->x + (machine->y * 100)];
						if (itemas->pr > 0) {
							if (machine->last_ - ms_time > 0) break;
							machine->last_ = ms_time + 1500;
							if (itemas->fg == 6952) {
								vector<WorldBlock>::iterator p = find_if(world->blocks.begin(), world->blocks.end(), [&](const WorldBlock& a) { return a.fg == machine->target_item or a.bg == machine->target_item; });
								if (p != world->blocks.end()) {
									WorldBlock* block_ = &world->blocks[p - world->blocks.begin()];
									int size = p - world->blocks.begin();
									int x_ = size % xSize, y_ = size / xSize;
									if (items[machine->target_item].blockType == BlockTypes::BACKGROUND and block_->fg != 0) continue;
									PlayerMoving data_{};
									data_.packetType = 17, data_.x = x_ * 32 + 16, data_.y = y_ * 32 + 16, data_.characterState = 0x8;
									data_.XSpeed = 2, data_.YSpeed = 1;
									BYTE* raw = packPlayerMoving(&data_);
									float rotation = 2;
									memcpy(raw + 40, &rotation, 4);
									PlayerMoving extended_particle_data_{};
									extended_particle_data_.packetType = 36, extended_particle_data_.x = x_ * 32 + 16, extended_particle_data_.y = y_ * 32 + 16, extended_particle_data_.characterState = 0x8;
									extended_particle_data_.netID = 110;
									BYTE* ex_raw = packPlayerMoving(&extended_particle_data_);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(currentPeer)->world == world->name) {
											send_raw(currentPeer, 4, ex_raw, 56, ENET_PACKET_FLAG_RELIABLE);
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
									}
									delete[] raw, ex_raw;
									itemas->pr--;
									if (itemas->pr <= 0) {
										PlayerMoving data_{};
										data_.packetType = 5, data_.punchX = machine->x, data_.punchY = machine->y, data_.characterState = 0x8;
										BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world, itemas));
										BYTE* blc = raw + 56;
										form_visual(blc, *itemas, *world, NULL, false);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(currentPeer)->world == world->name) {
												send_raw(currentPeer, 4, raw, 112 + alloc_(world, itemas), ENET_PACKET_FLAG_RELIABLE);
											}
										}
										delete[] raw, blc;
									}
									if (block_->hp == -1) {
										block_->hp = items[machine->target_item].breakHits;
										block_->lp = time_;
									}
									block_->hp -= 1;
									if (block_->hp == 0) {
										if (items[machine->target_item].max_gems != 0) {
											int maxgems = items[machine->target_item].max_gems;
											if (machine->target_item == 120) maxgems = 50;
											int c_ = rand() % (maxgems + 1);
											if (c_ != 0) {
												bool no_seed = false, no_gems = false, no_block = false;
												if (machine->target_item == 2242 or machine->target_item == 2244 or machine->target_item == 2246 or machine->target_item == 2248 or machine->target_item == 2250 or machine->target_item == 542) no_seed = true, no_block = true;
												else {
													for (int i_ = 0; i_ < world->drop.size(); i_++) {
														if (abs(world->drop[i_].y - y_ * 32) <= 16 and abs(world->drop[i_].x - x_ * 32) <= 16) {
															if (world->drop[i_].id == 112 and items[machine->target_item].rarity < 8) {
																no_gems = true;
															}
															else {
																no_seed = true, no_block = true;
															}
														}
													}
												}
												int chanced = 0;
												if (thedaytoday == 2) chanced = 5;
												if (rand() % 100 < 7) {
													WorldDrop drop_block_{};
													drop_block_.id = machine->target_item, drop_block_.count = 1, drop_block_.uid = uint16_t(world->drop.size()) + 1, drop_block_.x = (x_ * 32) + rand() % 17, drop_block_.y = (y_ * 32) + rand() % 17;
													if (not use_mag(world, drop_block_, x_, y_) and not no_block) {
														dropas_(world, drop_block_);
													}
												}
												else if (rand() % 100 < (items[machine->target_item].newdropchance + chanced)) {
													WorldDrop drop_seed_{};
													drop_seed_.id = machine->target_item + 1, drop_seed_.count = 1, drop_seed_.uid = uint16_t(world->drop.size()) + 1, drop_seed_.x = (x_ * 32) + rand() % 17, drop_seed_.y = (y_ * 32) + rand() % 17;
													if (not use_mag(world, drop_seed_, x_, y_) and not no_seed) {
														dropas_(world, drop_seed_);
													}
												}
												else if (not no_gems) {
													drop_rare_item(world, NULL, machine->target_item, x_, y_, false);
													 gems_(NULL, world, c_, x_ * 32, y_ * 32, machine->target_item); 

												}
											}
										}
										reset_(block_, x_, y_, world);
										PlayerMoving data_{};
										data_.packetType = 5, data_.punchX = x_, data_.punchY = y_, data_.characterState = 0x8;
										BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world, block_));
										BYTE* blc = raw + 56;
										form_visual(blc, *block_, *world, NULL, false);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(currentPeer)->world == world->name) {
												send_raw(currentPeer, 4, raw, 112 + alloc_(world, block_), ENET_PACKET_FLAG_RELIABLE);
											}
										}
										delete[] raw, blc;
									}
									else {
										PlayerMoving break_data{ -1, 0, 0x8, 0x0, 6, x_, y_, 0, (float)x_, (float)y_, 0, 0 };
										BYTE* p_ = packPlayerMoving(&break_data);
										p_[2] = 0, p_[3] = 0;
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(currentPeer)->world == world->name) {
												send_raw(currentPeer, 4, p_, 56, ENET_PACKET_FLAG_RELIABLE);
											}
										}
										delete[] p_;
									}
								}
							}
							else if (itemas->fg == 6950) {
								for (int a_ = 0; a_ < world->blocks.size(); a_++) {
									if (world->blocks[a_].fg == machine->target_item) {
										long long times_ = time(nullptr);
										uint32_t laikas = uint32_t((times_ - world->blocks[a_].planted <= items[world->blocks[a_].fg].growTime ? times_ - world->blocks[a_].planted : items[world->blocks[a_].fg].growTime));
										if (items[world->blocks[a_].fg].blockType == BlockTypes::SEED and laikas == items[world->blocks[a_].fg].growTime) {
											int x_ = a_ % xSize, y_ = a_ / xSize;
											WorldBlock* block_ = &world->blocks[x_ + (y_ * 100)];
											int drop_count = items[block_->fg - 1].rarity == 1 ? (items[block_->fg - 1].farmable ? (rand() % 9) + 4 : (rand() % block_->fruit) + 1) : items[block_->fg - 1].farmable ? (rand() % 5) + 4 : (rand() % block_->fruit) + 1;
											if (harvest_seed(world, block_, x_, y_, drop_count, -1)) {

											}
											else if (world->weather == 8 and rand() % 300 < 2) {
												WorldDrop drop_block_{};
												drop_block_.id = 3722, drop_block_.count = 1, drop_block_.uid = uint16_t(world->drop.size()) + 1, drop_block_.x = x_ * 32 + rand() % 17, drop_block_.y = y_ * 32 + rand() % 17;
												dropas_(world, drop_block_);
												PlayerMoving data_2{};
												data_2.packetType = 0x11, data_2.x = drop_block_.x, data_2.y = drop_block_.y, data_2.YSpeed = 108;
												BYTE* raw2 = packPlayerMoving(&data_2);
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != world->name) continue;
													send_raw(currentPeer, 4, raw2, 56, ENET_PACKET_FLAG_RELIABLE);
												}
												delete[] raw2;
											}
											if (drop_count != 0) drop_rare_item(world, NULL, machine->target_item - 1, x_, y_, true);
											{
												PlayerMoving data_{};
												data_.packetType = 17, data_.x = x_ * 32 + 16, data_.y = y_ * 32 + 16, data_.characterState = 0x8;
												data_.XSpeed = 2, data_.YSpeed = 1;
												BYTE* raw = packPlayerMoving(&data_, 56);
												float rotation = 2;
												memcpy(raw + 40, &rotation, 4);
												PlayerMoving extended_particle_data_{};
												extended_particle_data_.packetType = 36, extended_particle_data_.x = x_ * 32 + 16, extended_particle_data_.y = y_ * 32 + 16, extended_particle_data_.characterState = 0x8;
												extended_particle_data_.netID = 109;
												BYTE* ex_raw = packPlayerMoving(&extended_particle_data_, 56);
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
													if (pInfo(currentPeer)->world == world->name) {
														send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
														send_raw(currentPeer, 4, ex_raw, 56, ENET_PACKET_FLAG_RELIABLE);
													}
												}
												delete[] raw, ex_raw;
											}
											itemas->pr--;
											if (itemas->pr <= 0) {
												PlayerMoving data_{};
												data_.packetType = 5, data_.punchX = machine->x, data_.punchY = machine->y, data_.characterState = 0x8;
												BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world, itemas));
												BYTE* blc = raw + 56;
												form_visual(blc, *itemas, *world, NULL, false);
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
													if (pInfo(currentPeer)->world == world->name) {
														send_raw(currentPeer, 4, raw, 112 + alloc_(world, itemas), ENET_PACKET_FLAG_RELIABLE);
													}
												}
												delete[] raw, blc;
											}
										}
										break;
									}
								}
							}
						}
					}
					/*
					for (int i_ = 0; i_ < world->npc.size(); i_++) {
						WorldNPC* npc = &world->npc[i_];
						if (not npc->enabled) continue;
						if (npc->last_ - time_ > 0) continue;
						int active = 0;
						map<string, vector<WorldNPC>>::iterator it;
						for (it = active_npc.begin(); it != active_npc.end(); it++) {
							if (it->first == world->name) {
								for (int i_ = 0; i_ < it->second.size(); i_++) {
									if (it->second[i_].uid != -1) active++;
									if (active > 10) break;
								}
								break;
							}
						}
						if (active > 10) continue;
						npc->last_ = time_ + npc->rate_of_fire;
						WorldBlock* itemas = &world->blocks[npc->x + (npc->y * 100)];
						if (not itemas->enabled) continue;
						switch (itemas->fg) {
						case 8020: case 4344:
						{
							uint16_t projectile_speed = npc->projectile_speed;
							PlayerMoving data_{};
							data_.packetType = 34;
							data_.x = npc->x * 32 + 16; //nuo x
							data_.y = npc->y * 32 + (itemas->fg == 8020 ? 6 : 16); //nuo y
							data_.XSpeed = npc->x * 32 + 16; // iki x
							data_.YSpeed = npc->y * 32 + (itemas->fg == 8020 ? 6 : 16); // iki y
							data_.punchY = npc->projectile_speed;
							BYTE* raw = packPlayerMoving(&data_);
							uint16_t uid = (active_npc.find(world->name) != active_npc.end() ? active_npc[world->name].size() : 0);
							raw[1] = (itemas->fg == 8020 ? 15 : 8);
							raw[2] = uid; // npc uid turi buti unique
							raw[3] = 2; // 2 yra spawn o 7 yra despawn
							memcpy(raw + 40, &npc->kryptis, 4);
							npc->uid = uid;
							npc->started_moving = ms_time;
							if (active_npc.find(world->name) != active_npc.end()) {
								active_npc[world->name].push_back(*npc);
							}
							else {
								vector<WorldNPC> list_;
								list_.push_back(*npc);
								active_npc.insert({ world->name, list_ });
							}
							for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
								if (pInfo(currentPeer)->world == world->name and pInfo(currentPeer)->x != -1) {
									send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
								}
							}
							delete[]raw;
							break;
						}
						default:
						{
							bool cant_del = false;
							map<string, vector<WorldNPC>>::iterator it;
							for (it = active_npc.begin(); it != active_npc.end(); it++) {
								if (cant_del) break;
								if (it->first == world->name) {
									for (int i_ = 0; i_ < it->second.size(); i_++) {
										WorldNPC* npc_ = &it->second[i_];
										if (npc->uid == npc_->uid) {
											cant_del = true;
											break;
										}
									}
								}
							}
							if (not cant_del) {
								world->npc.erase(world->npc.begin() + i_);
							}
							break;
						}
						}
					}
					map<string, vector<WorldNPC>>::iterator it;
					for (it = active_npc.begin(); it != active_npc.end(); it++) {
						if (it->first == world->name) {
							for (int i_ = 0; i_ < it->second.size(); i_++) {
								WorldNPC* npc_ = &it->second[i_];
								if (npc_->uid == -1) continue;
								double per_sekunde_praeina_bloku = (double)npc_->projectile_speed / 32;
								double praejo_laiko = (double)(ms_time - npc_->started_moving) / 1000;
								double praejo_distancija = (double)per_sekunde_praeina_bloku * (double)praejo_laiko;
								double current_x = ((int)npc_->kryptis == 180 ? (((double)npc_->x - (double)praejo_distancija) * 32) + 16 : (((double)npc_->x + (double)praejo_distancija) * 32) + 16);
								double current_y = (double)npc_->y * 32;
								if (current_x / 32 < 0 or current_x / 32 >= 100 or current_y / 32 < 0 or current_y / 32 >= 60)
								{
									PlayerMoving data_{};
									data_.packetType = 34;
									data_.x = (current_x); //nuo x
									data_.y = (current_y + (npc_->id == 8020 ? 6 : 16)); //nuo y
									data_.XSpeed = (current_x); // iki x
									data_.YSpeed = (current_y + (npc_->id == 8020 ? 6 : 16)); // iki y
									data_.punchY = npc_->projectile_speed;
									BYTE* raw = packPlayerMoving(&data_);
									raw[1] = 15;
									raw[2] = npc_->uid;
									raw[3] = 7;
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(currentPeer)->world == world->name and pInfo(currentPeer)->x != -1) {
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
									}
									delete[]raw;
									npc_->uid = -1;
									continue;
								}
								try {
									WorldBlock* block_ = &world->blocks[current_x / 32 + (current_y / 32 * 100));
									if (items[block_->fg].collisionType == 1 or (current_x / 32) > 100 or (current_x / 32) < 0) {
										PlayerMoving data_{};
										data_.packetType = 34;
										data_.x = (current_x); //nuo x
										data_.y = (current_y + (npc_->id == 8020 ? 6 : 16)); //nuo y
										data_.XSpeed = (current_x); // iki x
										data_.YSpeed = (current_y + (npc_->id == 8020 ? 6 : 16)); // iki y
										data_.punchY = npc_->projectile_speed;
										BYTE* raw = packPlayerMoving(&data_);
										raw[1] = 15;
										raw[2] = npc_->uid;
										raw[3] = 7;
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(currentPeer)->world == world->name and pInfo(currentPeer)->x != -1) {
												send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											}
										}
										delete[]raw;
										npc_->uid = -1;
									}
								}
								catch (out_of_range) {
									continue;
								}
							}
							break;
						}*/
				}
			}
		}
	}
}
bool isASCII(const std::string& s)
{
	return !std::any_of(s.begin(), s.end(), [](char c) {
		return static_cast<unsigned char>(c) > 127;
	});
}

std::string get_str_between_two_str(const std::string& s,
	const std::string& start_delim,
	const std::string& stop_delim)
{
	unsigned first_delim_pos = s.find(start_delim);
	unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
	unsigned last_delim_pos = s.find(stop_delim);

	return s.substr(end_pos_of_first_delim,
		last_delim_pos - end_pos_of_first_delim);
}

int main() {

	/*
	cout << "Loading saved guild data" << endl;
	for (const auto& entry : fs::directory_iterator("guilds")) {
		if (!fs::is_directory(entry.path())) {
			string guild_id = explode(".", entry.path().filename().string())[0];
			json guild_read;
			ifstream read_guild(entry.path(), ifstream::binary);
			read_guild >> guild_read;
			read_guild.close();
			Guild new_guild{};
			new_guild.guild_id = atoi(guild_id.c_str());
			new_guild.guild_name = guild_read["guild_name"].get<string>();
			new_guild.guild_description = guild_read["guild_description"].get<string>();
			new_guild.guild_mascot = guild_read["guild_mascot"].get<vector<uint16_t>>();
			new_guild.guild_level = guild_read["guild_level"].get<uint16_t>();
			new_guild.guild_xp = guild_read["guild_xp"].get<uint32_t>();
			new_guild.coleader_access = guild_read["coleader_access"].get<bool>();
			new_guild.coleader_elder_access = guild_read["coleader_elder_access"].get<bool>();
			new_guild.all_access = guild_read["all_access"].get<bool>();
			new_guild.guild_world = guild_read["guild_world"].get<string>();
			json a_ = guild_read["guild_members"].get<json>();
			for (int i_ = 0; i_ < a_.size(); i_++) {
				GuildMember new_member{};
				new_member.member_name = a_[i_]["member_name"].get<string>();
				new_member.role_id = a_[i_]["role_id"].get<int>();
				new_member.public_location = a_[i_]["public_location"].get<bool>();
				new_member.show_notifications = a_[i_]["show_notifications"].get<bool>();
				new_member.last_online = a_[i_]["last_online"].get<long long>();
				new_guild.guild_members.push_back(new_member);
			}
			json b_ = guild_read["guild_logs"].get<json>();
			for (int i_ = 0; i_ < b_.size(); i_++) {
				GuildLog new_log{};
				new_log.info = b_[i_]["info"].get<string>();
				new_log.display_id = b_[i_]["display_id"].get<uint16_t>();
				new_log.date = b_[i_]["date"].get<long long>();
				new_guild.guild_logs.push_back(new_log);
			}
			guilds.push_back(new_guild);
			//cout << "Loaded guild -> " << new_guild.guild_id << " >> " << new_guild.guild_name << endl;
		}
	}
	*/

	/*
	string new_items = "";
	int numlines = -7;
	ifstream file("old.txt");
	string str;
	while (getline(file, str))
	{
		numlines++;

		if (numlines == 5136) str = "add_item\\5136\\20\\32\\5\\2\\`4R```8a```6i```2n```pb```#o```5w`` Magic Ore\\tiles_page14.rttex\\-580847623\\25\\-1\\24\\16\\5\\0\\1\\3\\8\\0\\100\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5137) str = "add_item\\5137\\4\\0\\19\\2\\`4R```8a```6i```2n```pb```#o```5w`` Magic Ore Seed\\tiles_page14.rttex\\-580847623\\25\\-1\\24\\16\\5\\0\\0\\20\\2\\0\\100\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,49,56,99\\255,0,255,255\\1003000\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5138) str = "add_item\\5138\\20\\0\\8\\0\\Cashback Coupon 10,000 Gems\\c_items1.rttex\\2062816473\\0\\-1\\2\\3\\1\\0\\0\\5\\8\\0\\999\\200\\game/c_items1_icon.rttex\\-66900836\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5139) str = "add_item\\5139\\4\\0\\19\\0\\Cashback Coupon 10,000 Gems Seed\\c_items1.rttex\\2062816473\\0\\-1\\2\\3\\1\\0\\0\\20\\2\\0\\999\\200\\interface/large/gtps_banner.rttex\\52495505\\400\\\\\\\\\\13\\10\\5\\2\\255,80,98,142\\255,228,236,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5140) str = "add_item\\5140\\20\\0\\8\\0\\Cashback Coupon 50,000 Gems\\c_items1.rttex\\2062816473\\0\\-1\\1\\3\\1\\0\\0\\5\\8\\0\\999\\200\\interface/large/gtps_daily_quest.rttex\\153267683\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5141) str = "add_item\\5141\\4\\0\\19\\0\\Cashback Coupon 50,000 Gems Seed\\c_items1.rttex\\2062816473\\0\\-1\\1\\3\\1\\0\\0\\20\\2\\0\\999\\200\\interface/large/gtps_growpass_banner.rttex\\921893404\\400\\\\\\\\\\13\\10\\5\\2\\255,80,98,142\\255,228,236,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5142) str = "add_item\\5142\\20\\0\\8\\0\\Cashback Coupon 100,000 Gems\\c_items1.rttex\\2062816473\\0\\-1\\3\\3\\1\\0\\0\\5\\8\\0\\999\\200\\interface/large/gtps_vip_banner.rttex\\-844019508\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5143) str = "add_item\\5143\\4\\0\\19\\0\\Cashback Coupon 100,000 Gems Seed\\c_items1.rttex\\2062816473\\0\\-1\\3\\3\\1\\0\\0\\20\\2\\0\\999\\200\\interface/large/gtps_mod_banner.rttex\\-863058540\\400\\\\\\\\\\13\\10\\5\\2\\255,80,98,142\\255,228,236,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";

		if (numlines == 5154) str = "add_item\\5154\\20\\0\\20\\1\\Golden Devil Wings\\c_items1.rttex\\2062816473\\4\\-1\\0\\4\\1\\0\\1\\0\\0\\6\\999\\200\\interface/gtps/discord.rttex\\1976376220\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 5155) str = "add_item\\5155\\4\\0\\19\\1\\Golden Devil Wings Seed\\c_items1.rttex\\2062816473\\0\\-1\\0\\4\\1\\0\\0\\20\\2\\6\\999\\200\\interface/gtps/gui_splicing.rttex\\632269302\\400\\\\\\\\\\11\\6\\3\\6\\255,250,25,26\\255,59,17,17\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5156) str = "add_item\\5156\\20\\128\\20\\1\\Dragon of Phoenix\\c_items1.rttex\\2062816473\\6\\-1\\0\\5\\1\\0\\0\\0\\0\\5\\999\\200\\game/phdrag.rttex\\-2081564225\\1\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5157) str = "add_item\\5157\\4\\0\\19\\1\\Dragon of Phoenix Seed\\c_items1.rttex\\2062816473\\0\\-1\\0\\5\\1\\0\\0\\20\\2\\5\\999\\200\\interface/gtps/gui_combining.rttex\\-488574518\\400\\\\\\\\\\6\\12\\6\\4\\200,174,9,133\\255,70,113,154\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5158) str = "add_item\\5158\\20\\0\\20\\1\\Lapute\\c_items1.rttex\\2062816473\\6\\-1\\2\\0\\1\\0\\1\\0\\0\\5\\999\\200\\game/plape.rttex\\-295327335\\0\\Fox\\foxi\\fox\\Desperate Bite\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5159) str = "add_item\\5159\\4\\0\\19\\1\\Lapute Seed\\c_items1.rttex\\2062816473\\0\\-1\\2\\0\\1\\0\\0\\20\\2\\5\\999\\200\\interface/gtps/gui_combusting.rttex\\1922086556\\400\\\\\\\\\\15\\14\\7\\6\\255,237,97,21\\255,255,211,187\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5160) str = "add_item\\5160\\20\\128\\20\\1\\Nado Hellscythe\\c_items1.rttex\\2062816473\\0\\-1\\0\\6\\1\\0\\1\\0\\0\\5\\999\\200\\interface/gtps/rules.rttex\\-1746871973\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5161) str = "add_item\\5161\\4\\0\\19\\1\\Nado Hellscythe Seed\\c_items1.rttex\\2062816473\\0\\-1\\0\\6\\1\\0\\0\\20\\2\\5\\999\\200\\interface/gtps/gui_crystals.rttex\\-392665913\\400\\\\\\\\\\6\\12\\6\\4\\255,45,45,45\\255,237,20,21\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";

		if (numlines == 5172) str = "add_item\\5172\\21\\0\\17\\2\\GTPS Block\\c_items1.rttex\\2062816473\\0\\-1\\0\\7\\1\\0\\1\\3\\4\\0\\999\\200\\interface/large/store_buttons/store_gtps.rttex\\109493919\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5173) str = "add_item\\5173\\4\\0\\19\\2\\GTPS Block Seed\\c_items1.rttex\\2062816473\\0\\-1\\0\\7\\1\\0\\0\\20\\2\\0\\999\\200\\interface/gtps/gui_news.rttex\\-785122800\\400\\\\\\\\\\3\\6\\3\\6\\255,233,186,23\\255,205,119,19\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5174) str = "add_item\\5174\\20\\128\\20\\1\\Lock Keeper Dragon\\c_items1.rttex\\2062816473\\6\\-1\\0\\8\\1\\0\\0\\0\\0\\5\\999\\200\\game/phdrag2.rttex\\776086155\\1\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5175) str = "add_item\\5175\\4\\0\\19\\1\\Lock Keeper Dragon Seed\\c_items1.rttex\\2062816473\\0\\-1\\0\\8\\1\\0\\0\\20\\2\\5\\999\\200\\interface/gtps/gui_rules.rttex\\1628748440\\400\\\\\\\\\\6\\12\\6\\4\\200,174,9,133\\255,70,113,154\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5176) str = "add_item\\5176\\20\\0\\20\\1\\Among Us\\c_items1.rttex\\2062816473\\6\\-1\\0\\9\\1\\0\\1\\0\\0\\5\\999\\200\\game/pr_among.rttex\\-101538737\\0\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 5177) str = "add_item\\5177\\4\\0\\19\\1\\Among Us Seed\\c_items1.rttex\\2062816473\\0\\-1\\0\\9\\1\\0\\0\\20\\2\\5\\999\\200\\\\0\\400\\\\\\\\\\4\\8\\4\\0\\255,106,196,64\\255,253,30,7\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5178) str = "add_item\\5178\\20\\0\\8\\3\\Gems Potion\\c_items1.rttex\\2062816473\\0\\-1\\0\\10\\1\\0\\1\\6\\8\\0\\999\\200\\interface/gtps/features.rttex\\-1756698631\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5179) str = "add_item\\5179\\4\\0\\19\\3\\Gems Potion Seed\\c_items1.rttex\\2062816473\\0\\-1\\0\\10\\1\\0\\0\\20\\2\\0\\999\\200\\\\0\\400\\\\\\\\\\9\\2\\1\\2\\255,170,252,150\\255,91,238,56\\1300\\0\\0\\\\\\\\0\\0\\\\0\\0";

		if (numlines == 5190) str = "add_item\\5190\\20\\0\\20\\1\\Nyan Hat\\c_items1.rttex\\2062816473\\0\\-1\\0\\14\\1\\0\\1\\0\\18\\0\\999\\200\\game/particles.rttex\\1101283583\\40\\game/c_items1.rttex\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5191) str = "add_item\\5191\\4\\0\\19\\1\\Nyan Hat Seed\\c_items1.rttex\\2062816473\\0\\-1\\0\\14\\1\\0\\0\\20\\2\\0\\999\\200\\\\0\\400\\game/c_items1.rttex\\\\\\\\0\\0\\0\\0\\255,152,210,254\\255,255,177,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5192) str = "add_item\\5192\\20\\0\\20\\1\\Moonglow Everlasting Scythe\\custom_long.rttex\\268109474\\0\\-1\\0\\0\\1\\0\\1\\0\\0\\5\\999\\200\\game/custom_long_icon.rttex\\-1921077504\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\1\\0\\\\0\\0";
		if (numlines == 5193) str = "add_item\\5193\\4\\0\\19\\1\\Moonglow Everlasting Scythe Seed\\custom_long.rttex\\268109474\\0\\-1\\0\\0\\1\\0\\0\\20\\2\\5\\999\\200\\interface/large/store_buttons/token_gtps.rttex\\1147379008\\400\\\\\\\\\\7\\14\\7\\6\\255,82,80,84\\255,220,152,227\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5194) str = "add_item\\5194\\20\\0\\20\\1\\Ultra Zeus' Lightning Bolt\\custom_long.rttex\\268109474\\0\\-1\\0\\1\\1\\0\\1\\0\\0\\5\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\1\\0\\\\0\\0";
		if (numlines == 5195) str = "add_item\\5195\\4\\0\\19\\1\\Ultra Zeus' Lightning Bolt Seed\\custom_long.rttex\\268109474\\0\\-1\\0\\1\\1\\0\\0\\20\\2\\5\\999\\200\\\\0\\400\\\\\\\\\\7\\14\\7\\6\\255,82,80,84\\255,220,152,227\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5196) str = "add_item\\5196\\9\\2\\38\\3\\ATM Machine 3000\\c_items1.rttex\\2062816473\\0\\-1\\0\\11\\1\\0\\0\\8\\5\\0\\999\\200\\\\0\\300\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\79200\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 5197) str = "add_item\\5197\\0\\0\\19\\3\\ATM Machine 3000 Seed\\c_items1.rttex\\2062816473\\0\\-1\\0\\11\\1\\0\\0\\20\\2\\0\\999\\200\\\\0\\400\\\\\\\\\\8\\0\\0\\0\\255,203,203,203\\255,46,141,224\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";

		if (numlines == 6336) str = "add_item\\6336\\0\\128\\8\\1\\Guidebook\\c_items1.rttex\\2062816473\\0\\-1\\1\\15\\1\\0\\0\\10\\1\\0\\999\\200\\\\0\\0\\\\\\\\\\0\\0\\0\\0\\255,255,0,0\\255,255,0,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 6337) str = "add_item\\6337\\0\\0\\19\\0\\Guidebook Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\5\\1\\1\\5\\255,255,0,0\\255,0,255,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";


		if (numlines == 7782) str = "add_item\\7782\\20\\0\\20\\1\\Crystal Angel Wings\\c_items1.rttex\\2062816473\\4\\-1\\0\\0\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 7783) str = "add_item\\7783\\4\\0\\19\\1\\Crystal Angel Wings Seed\\c_items1.rttex\\2062816473\\0\\-1\\0\\0\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,239,239,239\\255,255,255,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 7784) str = "add_item\\7784\\20\\128\\20\\1\\Phoenix Whip\\c_items1.rttex\\2062816473\\30\\-1\\0\\2\\1\\0\\1\\0\\0\\5\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\4096\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 7785) str = "add_item\\7785\\4\\0\\19\\1\\Phoenix Whip Seed\\c_items1.rttex\\2062816473\\0\\-1\\0\\2\\1\\0\\0\\20\\2\\5\\999\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,255,215,0\\255,255,254,190\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";

		if (numlines == 7960) str = "add_item\\7960\\0\\0\\17\\3\\The Dark Stone\\c_items1.rttex\\2062816473\\0\\-1\\0\\1\\1\\0\\1\\20\\8\\0\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 7961) str = "add_item\\7961\\0\\0\\19\\3\\The Dark Stone\\c_items1.rttex\\2062816473\\0\\-1\\0\\1\\1\\0\\0\\20\\2\\0\\999\\200\\\\0\\400\\\\\\\\\\2\\4\\2\\4\\255,255,241,21\\255,255,255,255\\70151\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 7962) str = "add_item\\7962\\0\\0\\8\\0\\Provider Spray Fertilizer\\c_items1.rttex\\2062816473\\0\\-1\\0\\12\\1\\0\\0\\30\\4\\0\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 7963) str = "add_item\\7963\\0\\0\\19\\0\\Provider Spray Fertilizer Seed\\c_items1.rttex\\2062816473\\0\\-1\\0\\12\\1\\0\\0\\20\\2\\0\\999\\200\\\\0\\400\\\\\\\\\\2\\4\\2\\4\\255,158,238,132\\255,215,240,241\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";

		if (numlines == 8428) str = "add_item\\8428\\4\\0\\20\\1\\Venom Mask\\c_items1.rttex\\2062816473\\15\\-1\\0\\13\\1\\0\\1\\0\\0\\7\\44\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 8429) str = "add_item\\8429\\4\\0\\19\\1\\Venom Mask Seed\\c_items1.rttex\\2062816473\\0\\-1\\0\\13\\1\\0\\0\\20\\2\\7\\44\\200\\\\0\\400\\\\\\\\\\1\\2\\1\\2\\255,5,5,5\\255,29,29,29\\86504\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 8430) str = "add_item\\8430\\4\\0\\20\\1\\Extra Backpack\\c_items1.rttex\\2062816473\\0\\-1\\0\\15\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 8431) str = "add_item\\8431\\4\\0\\19\\1\\Extra Backpack Seed\\c_items1.rttex\\2062816473\\0\\-1\\0\\15\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,162,138,72\\255,133,116,71\\5423\\0\\0\\\\\\\\0\\0\\\\0\\0";

		if (numlines == 8440) str = "add_item\\8440\\28\\0\\20\\1\\Legendary Shard Sword\\swords.rttex\\1026736829\\19\\-1\\0\\1\\1\\0\\0\\0\\18\\5\\999\\250\\game/swords_icon.rttex\\-758999821\\2500\\\\0\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\3\\0\\0,0|1,0|2,0|3,0|4,0|5,0|6,0|7,0|8,0|9,0|10,0|11,0|12,0|13,0|0,0|\\\\\\5\\7416\\\\0\\0";
		if (numlines == 8441) str = "add_item\\8441\\0\\0\\19\\1\\Legendary Shard Sword Seed\\swords.rttex\\1026736829\\0\\-1\\0\\6\\1\\0\\0\\20\\2\\5\\999\\250\\\\0\\400\\\\0\\\\\\0\\0\\0\\0\\255,201,159,149\\255,149,251,97\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";

		if (numlines == 8530) str = "add_item\\8530\\20\\0\\20\\1\\Black Devil Wings\\c_items1.rttex\\2062816473\\4\\-1\\2\\4\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 8531) str = "add_item\\8531\\4\\0\\19\\1\\Black Devil Wings Seed\\c_items1.rttex\\2062816473\\0\\-1\\2\\4\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\11\\6\\3\\6\\255,250,25,26\\255,59,17,17\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 8532) str = "add_item\\8532\\0\\0\\17\\0\\Deadly Dirt\\c_items1.rttex\\2062816473\\0\\-1\\0\\17\\5\\0\\1\\3\\8\\0\\8\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 8533) str = "add_item\\8533\\0\\0\\19\\0\\Deadly Dirt Seed\\c_items1.rttex\\2062816473\\0\\-1\\0\\17\\5\\0\\0\\20\\2\\0\\8\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,213,189,27\\255,221,214,177\\752\\0\\0\\\\\\\\0\\0\\\\0\\0";

		if (numlines == 9146) str = "add_item\\9146\\4\\4\\20\\1\\Lithuania Cape\\c_items1.rttex\\2062816473\\0\\-1\\0\\16\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9147) str = "add_item\\9147\\4\\4\\19\\1\\Lithuania Cape Seed\\c_items1.rttex\\2062816473\\0\\-1\\0\\16\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,196,15,12\\255,255,52,47\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9148) str = "add_item\\9148\\4\\4\\20\\1\\Indonesia Cape\\c_items1.rttex\\2062816473\\0\\-1\\1\\16\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9149) str = "add_item\\9149\\4\\4\\19\\1\\Indonesia Cape Seed\\c_items1.rttex\\2062816473\\0\\-1\\1\\16\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,196,15,12\\255,255,52,47\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9150) str = "add_item\\9150\\4\\4\\20\\1\\Turkey Cape\\c_items1.rttex\\2062816473\\0\\-1\\2\\16\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9151) str = "add_item\\9151\\4\\4\\19\\1\\Turkey Cape Seed\\c_items1.rttex\\2062816473\\0\\-1\\2\\16\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,196,15,12\\255,255,52,47\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9152) str = "add_item\\9152\\4\\4\\20\\1\\Russia Cape\\c_items1.rttex\\2062816473\\0\\-1\\3\\16\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9153) str = "add_item\\9153\\4\\4\\19\\1\\Russia Cape Seed\\c_items1.rttex\\2062816473\\0\\-1\\3\\16\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,196,15,12\\255,255,52,47\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9154) str = "add_item\\9154\\4\\4\\20\\1\\America Cape\\c_items1.rttex\\2062816473\\0\\-1\\4\\16\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9155) str = "add_item\\9155\\4\\4\\19\\1\\America Cape Seed\\c_items1.rttex\\2062816473\\0\\-1\\4\\16\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,196,15,12\\255,255,52,47\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9156) str = "add_item\\9156\\4\\4\\20\\1\\Finland Cape\\c_items1.rttex\\2062816473\\0\\-1\\5\\16\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9157) str = "add_item\\9157\\4\\4\\19\\1\\Finland Cape Seed\\c_items1.rttex\\2062816473\\0\\-1\\5\\16\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,196,15,12\\255,255,52,47\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9158) str = "add_item\\9158\\4\\4\\20\\1\\China Cape\\c_items1.rttex\\2062816473\\0\\-1\\6\\16\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9159) str = "add_item\\9159\\4\\4\\19\\1\\China Cape Seed\\c_items1.rttex\\2062816473\\0\\-1\\6\\16\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,196,15,12\\255,255,52,47\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9160) str = "add_item\\9160\\4\\4\\20\\1\\Default Cape\\c_items1.rttex\\2062816473\\0\\-1\\7\\16\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9161) str = "add_item\\9161\\4\\4\\19\\1\\Default Cape Seed\\c_items1.rttex\\2062816473\\0\\-1\\7\\16\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,196,15,12\\255,255,52,47\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9162) str = "add_item\\9162\\0\\0\\16\\0\\Deadly Mushroom\\c_items1.rttex\\2062816473\\0\\-1\\3\\0\\1\\0\\1\\4\\8\\0\\87\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9163) str = "add_item\\9163\\0\\0\\19\\0\\Deadly Mushroom Seed\\c_items1.rttex\\2062816473\\0\\-1\\3\\0\\1\\0\\0\\20\\2\\0\\87\\200\\\\0\\400\\\\\\\\\\7\\12\\5\\7\\255,30,186,0\\255,30,186,0\\31\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9164) str = "add_item\\9164\\20\\0\\8\\0\\Deadly Blast\\c_items1.rttex\\2062816473\\0\\-1\\4\\0\\1\\0\\0\\3\\4\\0\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9165) str = "add_item\\9165\\4\\0\\19\\0\\Deadly Blast Seed\\c_items1.rttex\\2062816473\\0\\-1\\4\\0\\1\\0\\0\\20\\2\\0\\999\\200\\\\0\\400\\\\\\\\\\3\\6\\3\\6\\255,30,88,102\\255,19,105,92\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9166) str = "add_item\\9166\\20\\0\\20\\1\\Eldritch Wings\\c_items1.rttex\\2062816473\\4\\-1\\4\\4\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9167) str = "add_item\\9167\\4\\0\\19\\1\\Eldritch Wings Seed\\c_items1.rttex\\2062816473\\0\\-1\\4\\4\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,252,142,0\\255,255,247,215\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9168) str = "add_item\\9168\\4\\0\\20\\1\\Thug Glasses\\c_items1.rttex\\2062816473\\0\\-1\\5\\0\\1\\0\\1\\0\\0\\4\\16\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 9169) str = "add_item\\9169\\4\\0\\19\\1\\Thug Glasses Seed\\c_items1.rttex\\2062816473\\0\\-1\\5\\0\\1\\0\\0\\20\\2\\4\\16\\200\\\\0\\400\\\\\\\\\\10\\4\\2\\4\\127,184,219,240\\255,62,92,110\\4576\\0\\0\\\\\\\\0\\0\\\\0\\0";

		if (numlines == 9488) str = "add_item\\9488\\20\\128\\20\\1\\Kitsune Mask\\c_items1.rttex\\2062816473\\0\\-1\\6\\0\\1\\0\\1\\0\\0\\0\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9489) str = "add_item\\9489\\4\\0\\19\\1\\Kitsune Mask Seed\\c_items1.rttex\\2062816473\\0\\-1\\6\\0\\1\\0\\0\\20\\2\\0\\999\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,243,152,63\\130,129,73,19\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9490) str = "add_item\\9490\\20\\0\\20\\1\\Frosty Dragon Knight's Helm\\c_items1.rttex\\2062816473\\0\\-1\\2\\15\\1\\0\\1\\0\\0\\0\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9491) str = "add_item\\9491\\4\\0\\19\\1\\Frosty Dragon Knight's Helm Seed\\c_items1.rttex\\2062816473\\0\\-1\\2\\15\\1\\0\\0\\20\\2\\0\\999\\200\\\\0\\400\\\\\\\\\\7\\14\\7\\6\\255,196,36,48\\255,42,47,78\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9492) str = "add_item\\9492\\20\\0\\20\\2\\Frosty Dragon Knight's Chestplate\\c_items1.rttex\\2062816473\\0\\-1\\3\\15\\1\\0\\1\\0\\0\\1\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9493) str = "add_item\\9493\\4\\0\\19\\2\\Frosty Dragon Knight's Chestplate Seed\\c_items1.rttex\\2062816473\\0\\-1\\3\\15\\1\\0\\0\\20\\2\\1\\999\\200\\\\0\\400\\\\\\\\\\8\\0\\0\\0\\255,196,36,48\\255,42,47,78\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9494) str = "add_item\\9494\\20\\0\\20\\1\\Fenix Wings\\c_items1.rttex\\2062816473\\4\\-1\\6\\4\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 9495) str = "add_item\\9495\\4\\0\\19\\1\\Fenix Wings Seed\\c_items1.rttex\\2062816473\\0\\-1\\6\\4\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\11\\6\\3\\6\\255,250,25,26\\255,59,17,17\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9496) str = "add_item\\9496\\28\\0\\20\\1\\Eldritch Sword\\swords9.rttex\\1458222275\\19\\-1\\0\\1\\1\\0\\0\\0\\18\\5\\999\\250\\game/swords9_icon.rttex\\-670815320\\2500\\\\0\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\3\\0\\0,0|1,0|2,0|0,0|\\\\\\5\\7416\\\\0\\0";
		if (numlines == 9497) str = "add_item\\9497\\0\\0\\19\\1\\Eldritch Sword Katana Seed\\swords9.rttex\\1458222275\\0\\-1\\0\\0\\1\\0\\0\\20\\2\\5\\999\\250\\\\0\\400\\\\0\\\\\\0\\0\\0\\0\\255,201,159,149\\255,149,251,97\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9498) str = "add_item\\9498\\4\\0\\20\\1\\White Thug Glasses\\c_items1.rttex\\2062816473\\0\\-1\\7\\0\\1\\0\\1\\0\\0\\4\\16\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 9499) str = "add_item\\9499\\4\\0\\19\\1\\White Thug Glasses Seed\\c_items1.rttex\\2062816473\\0\\-1\\7\\0\\1\\0\\0\\20\\2\\4\\16\\200\\\\0\\400\\\\\\\\\\10\\4\\2\\4\\127,184,219,240\\255,62,92,110\\4576\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9500) str = "add_item\\9500\\52\\0\\20\\1\\Golden AK-47\\c_items1.rttex\\2062816473\\0\\-1\\1\\1\\1\\0\\1\\0\\0\\5\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 9501) str = "add_item\\9501\\4\\0\\19\\1\\Golden AK-47 Seed\\c_items1.rttex\\2062816473\\0\\-1\\1\\1\\1\\0\\0\\20\\2\\5\\999\\200\\\\0\\400\\\\\\\\\\8\\0\\0\\0\\255,41,41,41\\255,255,250,109\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9502) str = "add_item\\9502\\4\\4\\20\\1\\United Arab Emirates Cape\\c_items1.rttex\\2062816473\\0\\-1\\1\\14\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9503) str = "add_item\\9503\\4\\4\\19\\1\\United Arab Emirates Cape Seed\\c_items1.rttex\\2062816473\\0\\-1\\1\\14\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,196,15,12\\255,255,52,47\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9504) str = "add_item\\9504\\4\\4\\20\\1\\Romania Cape\\c_items1.rttex\\2062816473\\0\\-1\\2\\14\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9505) str = "add_item\\9505\\4\\4\\19\\1\\Romania Cape Seed\\c_items1.rttex\\2062816473\\0\\-1\\2\\14\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,196,15,12\\255,255,52,47\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9506) str = "add_item\\9506\\4\\4\\20\\1\\Netherlands Cape\\c_items1.rttex\\2062816473\\0\\-1\\3\\14\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9507) str = "add_item\\9507\\4\\4\\19\\1\\Netherlands Cape Seed\\c_items1.rttex\\2062816473\\0\\-1\\3\\14\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,196,15,12\\255,255,52,47\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9508) str = "add_item\\9508\\4\\4\\20\\1\\Lebanon Cape\\c_items1.rttex\\2062816473\\0\\-1\\4\\14\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9509) str = "add_item\\9509\\4\\4\\19\\1\\Lebanon Cape Seed\\c_items1.rttex\\2062816473\\0\\-1\\4\\14\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,196,15,12\\255,255,52,47\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9510) str = "add_item\\9510\\4\\4\\20\\1\\Hungary Cape\\c_items1.rttex\\2062816473\\0\\-1\\5\\14\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9511) str = "add_item\\9511\\4\\4\\19\\1\\Hungary Cape Seed\\c_items1.rttex\\2062816473\\0\\-1\\5\\14\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,196,15,12\\255,255,52,47\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9512) str = "add_item\\9512\\4\\4\\20\\1\\Ukraine Cape\\c_items1.rttex\\2062816473\\0\\-1\\6\\14\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9513) str = "add_item\\9513\\4\\4\\19\\1\\Ukraine Cape Seed\\c_items1.rttex\\2062816473\\0\\-1\\6\\14\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,196,15,12\\255,255,52,47\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9514) str = "add_item\\9514\\4\\4\\20\\1\\Philippines Cape\\c_items1.rttex\\2062816473\\0\\-1\\7\\14\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9515) str = "add_item\\9515\\4\\4\\19\\1\\Philippines Cape Seed\\c_items1.rttex\\2062816473\\0\\-1\\7\\14\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,196,15,12\\255,255,52,47\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9516) str = "add_item\\9516\\20\\0\\20\\1\\Frosty Dragon Knight's Greaves\\c_items1.rttex\\2062816473\\0\\-1\\4\\15\\1\\0\\1\\0\\0\\2\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9517) str = "add_item\\9517\\4\\0\\19\\1\\Frosty Dragon Knight's Greaves Seed\\c_items1.rttex\\2062816473\\0\\-1\\4\\15\\1\\0\\0\\20\\2\\2\\999\\200\\\\0\\400\\\\\\\\\\9\\2\\1\\2\\255,196,36,48\\255,42,47,78\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9518) str = "add_item\\9518\\4\\0\\20\\1\\DEL's Briefcase\\c_items1.rttex\\2062816473\\0\\-1\\4\\8\\1\\0\\1\\0\\0\\5\\14\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 9519) str = "add_item\\9519\\4\\0\\19\\1\\DEL's Briefcase Seed\\c_items1.rttex\\2062816473\\0\\-1\\4\\8\\1\\0\\0\\20\\2\\5\\14\\200\\\\0\\400\\\\\\\\\\14\\12\\6\\4\\255,56,56,56\\255,93,93,93\\3164\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9520) str = "add_item\\9520\\20\\128\\20\\1\\Golden Digger's Spade\\c_items1.rttex\\2062816473\\0\\-1\\5\\15\\1\\0\\1\\0\\0\\5\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9521) str = "add_item\\9521\\4\\0\\19\\1\\Golden Digger's Spade Seed\\c_items1.rttex\\2062816473\\0\\-1\\5\\15\\1\\0\\0\\20\\2\\5\\999\\200\\\\0\\400\\\\\\\\\\4\\8\\4\\0\\255,0,0,0\\255,0,249,63\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9522) str = "add_item\\9522\\20\\128\\20\\1\\Broken Golden Digger's Spade\\c_items1.rttex\\2062816473\\0\\-1\\6\\15\\1\\0\\1\\0\\0\\5\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9523) str = "add_item\\9523\\4\\0\\19\\1\\Broken Golden Digger's Spade Seed\\c_items1.rttex\\2062816473\\0\\-1\\6\\15\\1\\0\\0\\20\\2\\5\\999\\200\\\\0\\400\\\\\\\\\\4\\8\\4\\0\\255,0,0,0\\255,0,249,63\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9524) str = "add_item\\9524\\20\\0\\20\\1\\Undine's Spine\\custom_long.rttex\\268109474\\19\\-1\\0\\2\\1\\0\\1\\0\\0\\5\\999\\200\\audio/slash.wav\\-613766623\\2000\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\1\\0\\\\0\\0";
		if (numlines == 9525) str = "add_item\\9525\\4\\0\\19\\1\\Undine's Spine Seed\\custom_long.rttex\\268109474\\0\\-1\\0\\2\\1\\0\\0\\20\\2\\5\\999\\200\\\\0\\400\\\\\\\\\\4\\8\\4\\0\\255,85,85,85\\255,210,1,9\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9526) str = "add_item\\9526\\20\\0\\8\\0\\Rare Seed Pack\\c_items1.rttex\\246311281\\0\\-1\\7\\15\\1\\0\\0\\5\\8\\0\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9527) str = "add_item\\9527\\4\\0\\19\\0\\Rare Seed Pack Seed\\c_items1.rttex\\2062816473\\0\\-1\\7\\15\\1\\0\\0\\20\\2\\0\\999\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,184,14,29\\255,213,203,204\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9528) str = "add_item\\9528\\23\\0\\10\\0\\Sales-Man Untradeable\\c_items1.rttex\\2062816473\\0\\-1\\1\\7\\1\\0\\0\\4\\5\\0\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9529) str = "add_item\\9529\\4\\0\\19\\0\\Sales-Man Untradeable Seed\\c_items1.rttex\\2062816473\\0\\-1\\1\\7\\1\\0\\0\\20\\2\\0\\999\\200\\\\0\\400\\\\\\\\\\3\\6\\3\\6\\255,218,198,143\\255,233,26,11\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9530) str = "add_item\\9530\\21\\0\\17\\2\\TikTok Block\\c_items1.rttex\\2062816473\\0\\-1\\2\\7\\1\\0\\1\\3\\4\\0\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9531) str = "add_item\\9531\\4\\0\\19\\2\\TikTok Block Seed\\c_items1.rttex\\2062816473\\0\\-1\\2\\7\\1\\0\\0\\20\\2\\0\\999\\200\\\\0\\400\\\\\\\\\\3\\6\\3\\6\\255,233,186,23\\255,205,119,19\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9532) str = "add_item\\9532\\20\\0\\20\\1\\Black Phoenix Wings\\c_items1.rttex\\2062816473\\4\\-1\\3\\7\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9533) str = "add_item\\9533\\4\\0\\19\\1\\Black Phoenix Wings Seed\\c_items1.rttex\\2062816473\\0\\-1\\3\\7\\1\\0\\0\\20\\2\\6\\999\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,252,142,0\\255,255,247,215\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9534) str = "add_item\\9534\\20\\128\\20\\1\\Pink Focused Eyes\\c_items1.rttex\\2062816473\\4\\-1\\5\\7\\1\\0\\1\\0\\0\\4\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9535) str = "add_item\\9535\\4\\0\\19\\1\\Pink Focused Eyes Seed\\c_items1.rttex\\2062816473\\0\\-1\\5\\7\\1\\0\\0\\20\\2\\4\\999\\200\\\\0\\400\\\\\\\\\\10\\4\\2\\4\\255,255,255,255\\255,68,218,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9536) str = "add_item\\9536\\20\\128\\20\\1\\Pink Pickaxe\\c_items1.rttex\\2062816473\\0\\-1\\5\\8\\1\\0\\1\\0\\0\\5\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 9537) str = "add_item\\9537\\4\\0\\19\\1\\Pink Pickaxe Seed\\c_items1.rttex\\2062816473\\0\\-1\\5\\8\\1\\0\\0\\20\\2\\5\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,255,212,38\\255,225,252,227\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9538) str = "add_item\\9538\\4\\0\\20\\1\\Black Overalls\\c_items1.rttex\\2062816473\\0\\-1\\6\\8\\1\\0\\1\\0\\0\\1\\64\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 9539) str = "add_item\\9539\\4\\0\\19\\1\\Black Overalls Seed\\c_items1.rttex\\2062816473\\0\\-1\\6\\8\\1\\0\\0\\20\\2\\1\\64\\200\\\\0\\400\\\\\\\\\\14\\12\\6\\4\\255,46,111,180\\255,211,129,92\\264064\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9540) str = "add_item\\9540\\20\\0\\20\\1\\Duct Taped\\c_items1.rttex\\2062816473\\0\\-1\\7\\8\\1\\0\\1\\0\\0\\4\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9541) str = "add_item\\9541\\4\\0\\19\\1\\Duct Taped Seed\\c_items1.rttex\\2062816473\\0\\-1\\7\\8\\1\\0\\0\\20\\2\\4\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,63,54,45\\255,38,36,34\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9542) str = "add_item\\9542\\20\\128\\20\\1\\Sanda Mask\\c_items1.rttex\\2062816473\\0\\-1\\1\\10\\1\\0\\1\\0\\0\\0\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9543) str = "add_item\\9543\\4\\0\\19\\1\\Sanda Mask Seed\\c_items1.rttex\\2062816473\\0\\-1\\1\\10\\1\\0\\0\\20\\2\\0\\999\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,243,152,63\\130,129,73,19\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9544) str = "add_item\\9544\\20\\0\\20\\1\\Malaya Scythe\\custom_long.rttex\\268109474\\19\\-1\\0\\3\\1\\0\\1\\0\\0\\5\\999\\200\\audio/slash.wav\\-613766623\\2000\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\1\\0\\\\0\\0";
		if (numlines == 9545) str = "add_item\\9545\\4\\0\\19\\1\\Malaya Scythe Seed\\custom_long.rttex\\268109474\\0\\-1\\0\\3\\1\\0\\0\\20\\2\\5\\999\\200\\\\0\\400\\\\\\\\\\4\\8\\4\\0\\255,85,85,85\\255,210,1,9\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9546) str = "add_item\\9546\\20\\0\\20\\1\\Awpim Katana\\custom_long.rttex\\268109474\\0\\-1\\0\\4\\1\\0\\1\\0\\0\\5\\999\\200\\audio/fireball.wav\\-2138883910\\0\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\5\\0\\\\0\\0";
		if (numlines == 9547) str = "add_item\\9547\\4\\0\\19\\1\\Awpim Buster Katana Seed\\custom_long.rttex\\268109474\\0\\-1\\0\\4\\1\\0\\0\\20\\2\\5\\999\\200\\\\0\\400\\\\\\\\\\7\\14\\7\\6\\255,3,25,63\\255,64,231,213\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9548) str = "add_item\\9548\\20\\0\\20\\1\\Light's Sword\\custom_long.rttex\\268109474\\0\\-1\\0\\5\\1\\0\\1\\0\\0\\5\\999\\200\\audio/fireball.wav\\-2138883910\\0\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\5\\0\\\\0\\0";
		if (numlines == 9549) str = "add_item\\9549\\4\\0\\19\\1\\Light's Sword Seed\\custom_long.rttex\\268109474\\0\\-1\\0\\5\\1\\0\\0\\20\\2\\5\\999\\200\\\\0\\400\\\\\\\\\\7\\14\\7\\6\\255,3,25,63\\255,64,231,213\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9550) str = "add_item\\9550\\28\\0\\20\\1\\Celestial Claymore\\swords2.rttex\\839498822\\19\\-1\\0\\1\\1\\0\\0\\0\\18\\5\\999\\250\\game/swords2_icon.rttex\\1939972110\\2500\\\\0\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\3\\0\\0,0|1,0|2,0|3,0|4,0|5,0|6,0|7,0|8,0|9,0|10,0|11,0|12,0|13,0|14,0|15,0|0,0|\\\\\\5\\7416\\\\0\\0";
		if (numlines == 9551) str = "add_item\\9551\\0\\0\\19\\1\\Celestial Claymore Seed\\swords2.rttex\\839498822\\0\\-1\\0\\0\\1\\0\\0\\20\\2\\5\\999\\250\\\\0\\400\\\\0\\\\\\0\\0\\0\\0\\255,201,159,149\\255,149,251,97\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9552) str = "add_item\\9552\\20\\0\\20\\0\\Vyte Black Hoodie\\c_items1.rttex\\2062816473\\19\\-1\\1\\5\\1\\0\\1\\0\\0\\1\\999\\200\\\\0\\2000\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 9553) str = "add_item\\9553\\4\\0\\19\\0\\Vyte Black Hoodie Seed\\c_items1.rttex\\2062816473\\0\\-1\\1\\5\\1\\0\\0\\20\\2\\1\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,197,206,228\\255,43,45,60\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9554) str = "add_item\\9554\\4\\0\\20\\1\\`cAzraq Hair``\\c_items1.rttex\\2062816473\\0\\-1\\2\\10\\1\\0\\1\\0\\0\\7\\6\\200\\interface/large/gui_shop_grow_pass.rttex\\1285906377\\200\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 9555) str = "add_item\\9555\\4\\0\\19\\1\\`cAzraq Hair`` Seed\\c_items1.rttex\\2062816473\\0\\-1\\2\\10\\1\\0\\0\\20\\2\\7\\6\\200\\\\0\\400\\\\\\\\\\6\\12\\6\\4\\255,255,30,7\\255,252,106,0\\396\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9556) str = "add_item\\9556\\4\\0\\20\\1\\Cross Earrings\\c_items1.rttex\\2062816473\\0\\-1\\3\\10\\1\\0\\1\\0\\0\\4\\57\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9557) str = "add_item\\9557\\4\\0\\19\\1\\Cross Earrings Seed\\c_items1.rttex\\2062816473\\0\\-1\\3\\10\\1\\0\\0\\20\\2\\4\\57\\200\\\\0\\400\\\\\\\\\\14\\12\\6\\4\\255,251,217,25\\255,255,255,150\\186903\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9558) str = "add_item\\9558\\0\\0\\17\\0\\Minecraft Dirt\\c_items1.rttex\\2062816473\\0\\-1\\0\\19\\5\\0\\1\\3\\8\\0\\8\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9559) str = "add_item\\9559\\0\\0\\19\\0\\Minecraft Dirt Seed\\c_items1.rttex\\2062816473\\0\\-1\\0\\19\\5\\0\\0\\20\\2\\0\\8\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,213,189,27\\255,221,214,177\\752\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9560) str = "add_item\\9560\\0\\0\\17\\2\\Minecraft Stone\\c_items1.rttex\\2062816473\\0\\-1\\2\\2\\1\\0\\1\\10\\5\\0\\2\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9561) str = "add_item\\9561\\0\\0\\19\\2\\Minecraft Stone Seed\\c_items1.rttex\\2062816473\\0\\-1\\2\\2\\1\\0\\0\\20\\2\\0\\2\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,128,128,128\\255,200,200,200\\31\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9562) str = "add_item\\9562\\0\\0\\17\\2\\Minecraft Wood\\c_items1.rttex\\2062816473\\0\\-1\\3\\2\\1\\0\\1\\10\\5\\0\\4\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9563) str = "add_item\\9563\\0\\0\\19\\2\\Minecraft Wood Seed\\c_items1.rttex\\2062816473\\0\\-1\\3\\2\\1\\0\\0\\20\\2\\0\\4\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,128,128,128\\255,200,200,200\\31\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9564) str = "add_item\\9564\\0\\0\\17\\2\\Minecraft Leaves\\c_items1.rttex\\2062816473\\0\\-1\\4\\2\\1\\0\\1\\10\\5\\0\\3\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9565) str = "add_item\\9565\\0\\0\\19\\2\\Minecraft Leaves Seed\\c_items1.rttex\\2062816473\\0\\-1\\4\\2\\1\\0\\0\\20\\2\\0\\3\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,128,128,128\\255,200,200,200\\31\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9566) str = "add_item\\9566\\0\\0\\17\\2\\Minecraft Gold Ore\\c_items1.rttex\\2062816473\\0\\-1\\5\\2\\1\\0\\1\\10\\5\\0\\107\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9567) str = "add_item\\9567\\0\\0\\19\\2\\Minecraft Gold Ore Seed\\c_items1.rttex\\2062816473\\0\\-1\\5\\2\\1\\0\\0\\20\\2\\0\\107\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,128,128,128\\255,200,200,200\\31\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9568) str = "add_item\\9568\\0\\0\\17\\2\\Minecraft Diamond Ore\\c_items1.rttex\\2062816473\\0\\-1\\6\\2\\1\\0\\1\\10\\5\\0\\255\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9569) str = "add_item\\9569\\0\\0\\19\\2\\Minecraft Diamond Ore Seed\\c_items1.rttex\\2062816473\\0\\-1\\6\\2\\1\\0\\0\\20\\2\\0\\255\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,128,128,128\\255,200,200,200\\31\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9570) str = "add_item\\9570\\0\\0\\17\\2\\Minecraft Bedrock\\c_items1.rttex\\2062816473\\0\\-1\\5\\1\\1\\0\\1\\10\\5\\0\\255\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9571) str = "add_item\\9571\\0\\0\\19\\2\\Minecraft Bedrock Seed\\c_items1.rttex\\2062816473\\0\\-1\\5\\1\\1\\0\\0\\20\\2\\0\\255\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,128,128,128\\255,200,200,200\\31\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9572) str = "add_item\\9572\\0\\0\\17\\2\\Minecraft TNT\\c_items1.rttex\\2062816473\\0\\-1\\5\\3\\1\\0\\1\\10\\5\\0\\1\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9573) str = "add_item\\9573\\0\\0\\19\\2\\Minecraft TNT Seed\\c_items1.rttex\\2062816473\\0\\-1\\5\\3\\1\\0\\0\\20\\2\\0\\1\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,128,128,128\\255,200,200,200\\31\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9574) str = "add_item\\9574\\28\\0\\20\\1\\Vyte Claymore\\swords3.rttex\\1969349821\\19\\-1\\0\\1\\1\\0\\0\\0\\18\\5\\999\\250\\game/swords3_icon.rttex\\-180570031\\2500\\\\0\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\3\\0\\0,0|1,0|2,0|3,0|4,0|5,0|6,0|7,0|8,0|9,0|10,0|11,0|12,0|13,0|14,0|15,0|0,0|\\\\\\5\\7416\\\\0\\0";
		if (numlines == 9575) str = "add_item\\9575\\0\\0\\19\\1\\Vyte Claymore Seed\\swords3.rttex\\1969349821\\0\\-1\\0\\0\\1\\0\\0\\20\\2\\5\\999\\250\\\\0\\400\\\\0\\\\\\0\\0\\0\\0\\255,201,159,149\\255,149,251,97\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9576) str = "add_item\\9576\\28\\0\\20\\1\\Lightning Nichirin\\swords4.rttex\\1024853216\\19\\-1\\0\\1\\1\\0\\0\\0\\18\\5\\999\\250\\game/swords4_icon.rttex\\-311421054\\2500\\\\0\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\3\\0\\0,0|1,0|2,0|3,0|4,0|5,0|6,0|7,0|8,0|9,0|10,0|11,0|12,0|13,0|14,0|0,0|\\\\\\5\\7416\\\\0\\0";
		if (numlines == 9577) str = "add_item\\9577\\0\\0\\19\\1\\Lightning Nichirin Seed\\swords4.rttex\\1024853216\\0\\-1\\0\\0\\1\\0\\0\\20\\2\\5\\999\\250\\\\0\\400\\\\0\\\\\\0\\0\\0\\0\\255,201,159,149\\255,149,251,97\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9578) str = "add_item\\9578\\66\\0\\26\\1\\Minecraft Portal\\c_items1.rttex\\2062816473\\0\\-1\\6\\3\\1\\0\\0\\5\\8\\0\\84\\200\\audio/teleport.wav\\-2042761159\\200\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9579) str = "add_item\\9579\\0\\0\\19\\1\\Minecraft Portal Seed\\c_items1.rttex\\2062816473\\0\\-1\\6\\3\\1\\0\\0\\20\\2\\0\\84\\200\\\\0\\400\\\\\\\\\\1\\2\\1\\2\\255,39,225,255\\255,11,100,255\\595224\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9580) str = "add_item\\9580\\0\\0\\17\\2\\Minecraft Obsidian\\c_items1.rttex\\2062816473\\0\\-1\\6\\1\\1\\0\\1\\10\\5\\0\\255\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9581) str = "add_item\\9581\\0\\0\\19\\2\\Minecraft Obsidian Seed\\c_items1.rttex\\2062816473\\0\\-1\\6\\1\\1\\0\\0\\20\\2\\0\\255\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,128,128,128\\255,200,200,200\\31\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9582) str = "add_item\\9582\\20\\0\\8\\0\\Minecraft Blast\\c_items1.rttex\\2062816473\\0\\-1\\7\\3\\1\\0\\0\\3\\4\\0\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9583) str = "add_item\\9583\\4\\0\\19\\0\\Minecraft Blast Seed\\c_items1.rttex\\2062816473\\0\\-1\\7\\3\\1\\0\\0\\20\\2\\0\\999\\200\\\\0\\400\\\\\\\\\\3\\6\\3\\6\\255,30,88,102\\255,19,105,92\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9584) str = "add_item\\9584\\2\\0\\10\\0\\Minecraft Sign\\c_items1.rttex\\2062816473\\0\\-1\\7\\2\\1\\0\\0\\5\\8\\0\\2\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9585) str = "add_item\\9585\\0\\0\\19\\0\\Minecraft Sign Seed\\c_items1.rttex\\2062816473\\0\\-1\\7\\2\\1\\0\\0\\20\\2\\0\\2\\200\\\\0\\400\\\\\\\\\\10\\4\\2\\4\\255,157,141,69\\255,204,189,116\\68\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9586) str = "add_item\\9586\\0\\0\\18\\0\\Minecraft Obsidian (walk through)\\c_items1.rttex\\2062816473\\0\\-1\\6\\1\\1\\0\\1\\3\\5\\0\\42\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9587) str = "add_item\\9587\\0\\0\\19\\0\\Minecraft Obsidian (walk through) Seed\\c_items1.rttex\\2062816473\\0\\-1\\6\\1\\1\\0\\0\\20\\2\\0\\42\\200\\\\0\\400\\\\\\\\\\3\\6\\3\\6\\255,34,34,34\\255,0,0,0\\75348\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9588) str = "add_item\\9588\\2\\0\\12\\3\\Minecraft Torch\\c_items1.rttex\\2062816473\\35\\-1\\2\\1\\1\\0\\0\\6\\4\\0\\3\\200\\audio/fire.wav\\776922512\\100\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9589) str = "add_item\\9589\\0\\0\\19\\3\\Minecraft Torch Seed\\c_items1.rttex\\2062816473\\0\\-1\\2\\1\\1\\0\\0\\20\\2\\0\\3\\200\\\\0\\400\\\\\\\\\\12\\8\\4\\0\\255,127,86,53\\255,255,32,21\\117\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9590) str = "add_item\\9590\\4\\0\\20\\1\\Vyte Glasses\\c_items1.rttex\\2062816473\\0\\-1\\2\\5\\1\\0\\1\\0\\0\\4\\16\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 9591) str = "add_item\\9591\\4\\0\\19\\1\\Vyte Glasses Seed\\c_items1.rttex\\2062816473\\0\\-1\\2\\5\\1\\0\\0\\20\\2\\4\\16\\200\\\\0\\400\\\\\\\\\\10\\4\\2\\4\\127,184,219,240\\255,62,92,110\\4576\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9592) str = "add_item\\9592\\20\\0\\20\\1\\Lightning Hair\\c_items1.rttex\\2062816473\\0\\-1\\1\\12\\1\\0\\1\\0\\0\\7\\999\\200\\\\0\\100\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\1\\0\\1,12|2,12|3,12|4,12|5,12|6,12|7,12|\\\\\\0\\0\\\\0\\0";
		if (numlines == 9593) str = "add_item\\9593\\4\\0\\19\\1\\Lightning Hair Seed\\c_items1.rttex\\2062816473\\0\\-1\\1\\12\\1\\0\\0\\20\\2\\7\\999\\200\\\\0\\400\\\\\\\\\\15\\14\\7\\6\\255,2,147,214\\255,163,223,252\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9594) str = "add_item\\9594\\4\\8\\20\\1\\Vyte Hat\\c_items1.rttex\\2062816473\\25\\-1\\3\\11\\1\\0\\1\\0\\0\\0\\65\\200\\\\0\\250\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 9595) str = "add_item\\9595\\4\\8\\19\\1\\Vyte Hat Seed\\c_items1.rttex\\2062816473\\25\\-1\\3\\11\\1\\0\\0\\20\\2\\0\\65\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,85,21,155\\255,28,219,38\\276575\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9596) str = "add_item\\9596\\28\\0\\20\\1\\Fire Nichirin\\swords5.rttex\\-1552503689\\19\\-1\\0\\1\\1\\0\\0\\0\\18\\5\\999\\250\\game/swords5_icon.rttex\\-452172799\\2500\\\\0\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\3\\0\\0,0|1,0|2,0|3,0|4,0|5,0|6,0|7,0|8,0|9,0|10,0|11,0|0,0|\\\\\\5\\7416\\\\0\\0";
		if (numlines == 9597) str = "add_item\\9597\\0\\0\\19\\1\\Fire Nichirin Seed\\swords5.rttex\\-1552503689\\0\\-1\\0\\0\\1\\0\\0\\20\\2\\5\\999\\250\\\\0\\400\\\\0\\\\\\0\\0\\0\\0\\255,201,159,149\\255,149,251,97\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9598) str = "add_item\\9598\\28\\0\\20\\1\\Water Nichirin\\swords6.rttex\\1310758661\\19\\-1\\0\\1\\1\\0\\0\\0\\18\\5\\999\\250\\game/swords6_icon.rttex\\-1257229128\\2500\\\\0\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\3\\0\\0,0|1,0|2,0|3,0|4,0|5,0|6,0|7,0|8,0|0,0|\\\\\\5\\7416\\\\0\\0";
		if (numlines == 9599) str = "add_item\\9599\\0\\0\\19\\1\\Water Nichirin Seed\\swords6.rttex\\1310758661\\0\\-1\\0\\0\\1\\0\\0\\20\\2\\5\\999\\250\\\\0\\400\\\\0\\\\\\0\\0\\0\\0\\255,201,159,149\\255,149,251,97\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9600) str = "add_item\\9600\\0\\0\\17\\2\\Consumable Gacha\\c_items1.rttex\\2062816473\\0\\-1\\7\\1\\1\\0\\1\\10\\5\\0\\255\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9601) str = "add_item\\9601\\0\\0\\19\\2\\Consumable Gacha Seed\\c_items1.rttex\\2062816473\\0\\-1\\7\\1\\1\\0\\0\\20\\2\\0\\255\\200\\\\0\\400\\\\\\\\\\5\\10\\5\\2\\255,128,128,128\\255,200,200,200\\31\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9602) str = "add_item\\9602\\20\\0\\8\\3\\Farm Blast\\c_items1.rttex\\2062816473\\0\\-1\\3\\5\\1\\0\\0\\3\\4\\0\\999\\200\\\\0\\200\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9603) str = "add_item\\9603\\4\\0\\19\\3\\Farm Blast Seed\\c_items1.rttex\\2062816473\\0\\-1\\3\\5\\1\\0\\0\\20\\2\\0\\999\\200\\\\0\\400\\\\\\\\\\1\\2\\1\\2\\255,153,230,95\\255,161,91,64\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9604) str = "add_item\\9604\\20\\0\\20\\1\\Edit's Fan & Scythe\\custom_long.rttex\\268109474\\0\\-1\\0\\6\\1\\0\\1\\0\\0\\5\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\1\\0\\\\0\\0";
		if (numlines == 9605) str = "add_item\\9605\\4\\0\\19\\1\\Edit's Fan & Scythe Seed\\custom_long.rttex\\268109474\\0\\-1\\0\\6\\1\\0\\0\\20\\2\\5\\999\\200\\\\0\\400\\\\\\\\\\7\\14\\7\\6\\255,82,80,84\\255,220,152,227\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";

		if (numlines == 9636) str = "add_item\\9636\\28\\0\\20\\1\\Egyptian Scythe\\swords7.rttex\\553525293\\19\\-1\\0\\1\\1\\0\\0\\0\\18\\5\\999\\250\\game/swords7_icon.rttex\\-684886957\\2500\\\\0\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\3\\0\\0,0|1,0|2,0|3,0|4,0|5,0|6,0|7,0|8,0|9,0|10,0|11,0|12,0|13,0|14,0|15,0|16,0|17,0|18,0|19,0|20,0|21,0|22,0|23,0|24,0|25,0|26,0|27,0|28,0|29,0|30,0|31,0|32,0|33,0|34,0|35,0|36,0|37,0|38,0|39,0|40,0|41,0|42,0|43,0|44,0|45,0|46,0|47,0|48,0|49,0|50,0|51,0|52,0|53,0|54,0|55,0|56,0|57,0|58,0|59,0|60,0|61,0|62,0|63,0|64,0|65,0|66,0|67,0|68,0|69,0|70,0|71,0|72,0|73,0|74,0|75,0|76,0|77,0|78,0|79,0|80,0|81,0|82,0|83,0|84,0|85,0|86,0|87,0|88,0|89,0|90,0|91,0|0,0|\\\\\\5\\7416\\\\0\\0";
		if (numlines == 9637) str = "add_item\\9637\\0\\0\\19\\1\\Egyptian Scythe Seed\\swords7.rttex\\553525293\\0\\-1\\0\\0\\1\\0\\0\\20\\2\\5\\999\\250\\\\0\\400\\\\0\\\\\\0\\0\\0\\0\\255,201,159,149\\255,149,251,97\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9638) str = "add_item\\9638\\4\\0\\20\\1\\Akatsuki's Robe\\c_items1.rttex\\2062816473\\0\\-1\\4\\5\\1\\0\\1\\0\\0\\1\\60\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9639) str = "add_item\\9639\\4\\0\\19\\1\\Akatsuki's Robe Seed\\c_items1.rttex\\2062816473\\0\\-1\\4\\5\\1\\0\\0\\20\\2\\1\\60\\200\\\\0\\400\\\\\\\\\\12\\8\\4\\0\\255,153,64,198\\255,249,237,255\\217800\\0\\0\\\\\\\\0\\0\\\\0\\0";

		if (numlines == 9766) str = "add_item\\9766\\28\\0\\20\\1\\Wound's Daedric Katana\\swords8.rttex\\1006979162\\19\\-1\\0\\1\\1\\0\\0\\0\\18\\5\\999\\250\\game/swords8_icon.rttex\\-364376228\\2500\\\\0\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\3\\0\\0,0|1,0|2,0|3,0|3,0|3,0|0,0|\\\\\\5\\7416\\\\0\\0";
		if (numlines == 9767) str = "add_item\\9767\\0\\0\\19\\1\\Wound's Daedric Katana Seed\\swords8.rttex\\1006979162\\0\\-1\\0\\0\\1\\0\\0\\20\\2\\5\\999\\250\\\\0\\400\\\\0\\\\\\0\\0\\0\\0\\255,201,159,149\\255,149,251,97\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9768) str = "add_item\\9768\\20\\0\\20\\1\\HDC Intergalactic Hair\\c_items1.rttex\\2062816473\\4\\-1\\4\\5\\1\\0\\1\\0\\0\\7\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 9769) str = "add_item\\9769\\4\\0\\19\\1\\HDC Intergalactic Hair Seed\\c_items1.rttex\\2062816473\\0\\-1\\4\\5\\1\\0\\0\\20\\2\\7\\999\\200\\\\0\\400\\\\\\\\\\12\\8\\4\\0\\255,255,240,11\\255,249,180,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9770) str = "add_item\\9770\\20\\0\\20\\1\\Golden Rayman's Fist \\c_items1.rttex\\2062816473\\6\\-1\\6\\5\\1\\0\\1\\0\\0\\5\\999\\200\\game/c_rayman_1.rttex\\123573042\\8\\Globox\\glo\\boxer\\Super Slap\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\-8192\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9771) str = "add_item\\9771\\4\\0\\19\\1\\Golden Rayman's Fist  Seed\\c_items1.rttex\\2062816473\\0\\-1\\6\\5\\1\\0\\0\\20\\2\\5\\999\\200\\\\0\\400\\\\\\\\\\4\\8\\4\\0\\255,255,255,255\\255,156,170,206\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9772) str = "add_item\\9772\\20\\0\\20\\1\\Evil Rayman's Fist\\c_items1.rttex\\2062816473\\6\\-1\\1\\6\\1\\0\\1\\0\\0\\5\\999\\200\\game/c_rayman_2.rttex\\2100676812\\8\\Globox\\glo\\boxer\\Super Slap\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\-8192\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9773) str = "add_item\\9773\\4\\0\\19\\1\\Evil Rayman's Fist Seed\\c_items1.rttex\\2062816473\\0\\-1\\1\\6\\1\\0\\0\\20\\2\\5\\999\\200\\\\0\\400\\\\\\\\\\4\\8\\4\\0\\255,255,255,255\\255,156,170,206\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9774) str = "add_item\\9774\\20\\0\\20\\1\\Leonidas Scythe\\custom_long.rttex\\268109474\\19\\-1\\0\\7\\1\\0\\1\\0\\0\\5\\999\\200\\audio/slash.wav\\-613766623\\2000\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\1\\0\\\\0\\0";
		if (numlines == 9775) str = "add_item\\9775\\4\\0\\19\\1\\Leonidas Scythe Seed\\custom_long.rttex\\268109474\\0\\-1\\0\\7\\1\\0\\0\\20\\2\\5\\999\\200\\\\0\\400\\\\\\\\\\4\\8\\4\\0\\255,85,85,85\\255,210,1,9\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9776) str = "add_item\\9776\\20\\128\\20\\1\\Kucing\\c_items1.rttex\\2062816473\\6\\-1\\3\\6\\1\\0\\1\\0\\0\\5\\999\\200\\game/custom_dog_1.rttex\\470445471\\8\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9777) str = "add_item\\9777\\4\\0\\19\\1\\Kucing Seed\\c_items1.rttex\\2062816473\\0\\-1\\3\\6\\1\\0\\0\\20\\2\\5\\999\\200\\\\0\\400\\\\\\\\\\7\\14\\7\\6\\255,44,34,26\\255,255,3,3\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9778) str = "add_item\\9778\\20\\0\\20\\1\\Anons Bolt\\custom_long.rttex\\268109474\\0\\-1\\0\\8\\1\\0\\1\\0\\0\\5\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\1\\0\\\\0\\0";
		if (numlines == 9779) str = "add_item\\9779\\4\\0\\19\\1\\Anons Bolt Seed\\custom_long.rttex\\268109474\\0\\-1\\0\\8\\1\\0\\0\\20\\2\\5\\999\\200\\\\0\\400\\\\\\\\\\7\\14\\7\\6\\255,82,80,84\\255,220,152,227\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9780) str = "add_item\\9780\\4\\0\\20\\1\\Niuber's Golden Timey Hat\\c_items1.rttex\\2062816473\\0\\-1\\4\\6\\1\\0\\1\\0\\0\\0\\255\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 9781) str = "add_item\\9781\\4\\0\\19\\1\\Niuber's Golden Timey Hat Seed\\c_items1.rttex\\2062816473\\0\\-1\\4\\6\\1\\0\\0\\20\\2\\0\\255\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,92,65,56\\255,67,48,41\\7429\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9782) str = "add_item\\9782\\20\\0\\20\\1\\Egg's Fat Golden Pet Egg\\c_items1.rttex\\2062816473\\6\\-1\\5\\6\\1\\0\\1\\0\\0\\2\\999\\200\\game/custom_pet_1.rttex\\200885135\\0\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9783) str = "add_item\\9783\\4\\0\\19\\1\\Egg's Fat Golden Pet Egg Seed\\c_items1.rttex\\2062816473\\0\\-1\\5\\6\\1\\0\\0\\20\\2\\2\\999\\200\\\\0\\400\\\\\\\\\\12\\8\\4\\0\\255,75,81,94\\255,103,78,56\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9784) str = "add_item\\9784\\28\\0\\20\\1\\Lighting Bolt\\swords10.rttex\\-104881860\\19\\-1\\0\\1\\1\\0\\0\\0\\18\\5\\999\\250\\game/swords10_icon.rttex\\2121290648\\2500\\\\0\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\3\\0\\0,0|1,0|2,0|3,0|0,0|\\\\\\5\\7416\\\\0\\0";
		if (numlines == 9785) str = "add_item\\9785\\0\\0\\19\\1\\Lighting Bolt Seed\\swords10.rttex\\-104881860\\0\\-1\\0\\0\\1\\0\\0\\20\\2\\5\\999\\250\\\\0\\400\\\\0\\\\\\0\\0\\0\\0\\255,201,159,149\\255,149,251,97\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		
		if (numlines == 9786) str = "add_item\\9786\\20\\0\\20\\1\\KingXD Crown\\c_items1.rttex\\2062816473\\0\\-1\\1\\8\\1\\0\\1\\0\\0\\0\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9787) str = "add_item\\9787\\4\\0\\19\\1\\KingXD Crown Seed\\c_items1.rttex\\2062816473\\0\\-1\\1\\8\\1\\0\\0\\20\\2\\0\\999\\200\\\\0\\400\\\\\\\\\\1\\2\\1\\2\\255,255,198,1\\255,25,147,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9788) str = "add_item\\9788\\4\\0\\20\\0\\Tech Fleece Hoodie - Gray\\gtps/clothes2.rttex\\1699773571\\0\\-1\\2\\1\\1\\0\\0\\0\\1\\1\\999\\200\\game/gtps/clothes_icon.rttex\\-676047385\\0\\\\\\\\\\0\\0\\0\\0\\0,0,0,0\\0,0,0,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9789) str = "add_item\\9789\\0\\0\\19\\0\\Tech Fleece Hoodie - Gray Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\10\\4\\1\\1\\255,232,232,232\\255,0,0,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9790) str = "add_item\\9790\\4\\0\\20\\0\\Tech Fleece Joggers - Gray\\gtps/clothes2.rttex\\1699773571\\0\\-1\\2\\2\\1\\0\\0\\0\\1\\2\\999\\200\\\\0\\0\\\\\\\\\\0\\0\\0\\0\\0,0,0,0\\0,0,0,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9791) str = "add_item\\9791\\0\\0\\19\\0\\Tech Fleece Joggers - Gray Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\10\\4\\1\\1\\255,255,0,0\\255,0,0,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9792) str = "add_item\\9792\\4\\0\\20\\1\\Air Forces - White\\gtps/clothes.rttex\\753789050\\0\\-1\\4\\0\\1\\0\\0\\0\\1\\3\\999\\200\\\\0\\0\\\\\\\\\\0\\0\\0\\0\\0,0,0,0\\0,0,0,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9793) str = "add_item\\9793\\0\\0\\19\\0\\Air Forces - White Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\10\\4\\1\\1\\255,255,0,0\\255,0,0,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9794) str = "add_item\\9794\\4\\0\\20\\0\\Tech Fleece Hoodie - Black\\gtps/clothes2.rttex\\1699773571\\0\\-1\\3\\1\\1\\0\\0\\0\\1\\1\\999\\200\\\\0\\0\\\\\\\\\\0\\0\\0\\0\\0,0,0,0\\0,0,0,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9795) str = "add_item\\9795\\0\\0\\19\\0\\Tech Fleece Hoodie - Black Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\10\\4\\1\\1\\255,0,0,0\\255,26,26,26\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9796) str = "add_item\\9796\\4\\0\\20\\0\\Tech Fleece Joggers - Black\\gtps/clothes2.rttex\\1699773571\\0\\-1\\3\\2\\1\\0\\0\\0\\1\\2\\999\\200\\\\0\\0\\\\\\\\\\0\\0\\0\\0\\0,0,0,0\\0,0,0,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9797) str = "add_item\\9797\\0\\0\\19\\0\\Tech Fleece Joggers - Black Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\10\\4\\1\\1\\255,255,0,0\\255,0,0,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9798) str = "add_item\\9798\\4\\0\\20\\1\\Air Forces - Black\\gtps/clothes.rttex\\753789050\\0\\-1\\5\\0\\1\\0\\0\\0\\1\\3\\999\\200\\\\0\\0\\\\\\\\\\0\\0\\0\\0\\0,0,0,0\\0,0,0,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9799) str = "add_item\\9799\\0\\0\\19\\0\\Air Forces - Black Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\10\\4\\1\\1\\255,255,0,0\\255,0,0,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9800) str = "add_item\\9800\\20\\0\\20\\1\\Zarso Sword\\custom_long.rttex\\268109474\\19\\-1\\0\\9\\1\\0\\1\\0\\0\\5\\999\\200\\audio/slash.wav\\-613766623\\2000\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\1\\0\\\\0\\0";
		if (numlines == 9801) str = "add_item\\9801\\4\\0\\19\\1\\Zarso Sword Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\10\\4\\1\\1\\255,255,0,0\\255,0,0,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9802) str = "add_item\\9802\\20\\0\\20\\1\\Hell Demonic Scythe\\custom_long.rttex\\268109474\\19\\-1\\0\\10\\1\\0\\1\\0\\0\\5\\999\\200\\audio/slash.wav\\-613766623\\2000\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\1\\0\\\\0\\0";
		if (numlines == 9803) str = "add_item\\9803\\4\\0\\19\\1\\Hell Demonic Scythe Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\10\\4\\1\\1\\255,255,0,0\\255,0,0,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9804) str = "add_item\\9804\\20\\32\\20\\1\\Nightmare Eyes\\c_items1.rttex\\2062816473\\24\\-1\\1\\13\\1\\0\\1\\0\\0\\4\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9805) str = "add_item\\9805\\4\\0\\19\\1\\Nightmare Eyes Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\10\\4\\1\\1\\255,255,0,0\\255,0,0,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9806) str = "add_item\\9806\\20\\0\\20\\1\\Dark Chimera Wings\\c_items1.rttex\\2062816473\\4\\-1\\4\\13\\1\\0\\1\\0\\0\\6\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9807) str = "add_item\\9807\\4\\0\\19\\1\\Dark Chimera Wings Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\10\\4\\1\\1\\255,255,0,0\\255,0,0,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9808) str = "add_item\\9808\\20\\0\\20\\1\\Jinx Intergalactic Hair\\c_items1.rttex\\2062816473\\4\\-1\\6\\13\\1\\0\\1\\0\\0\\7\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\4\\0\\\\0\\0";
		if (numlines == 9809) str = "add_item\\9809\\4\\0\\19\\1\\Jinx Intergalactic Hair Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\10\\4\\1\\1\\255,255,0,0\\255,0,0,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";

		if (numlines == 9810) str = "add_item\\9810\\20\\0\\20\\1\\Aldo's Soul Eater\\custom_long.rttex\\268109474\\19\\-1\\0\\11\\1\\0\\1\\0\\0\\5\\999\\200\\audio/slash.wav\\-613766623\\2000\\\\\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\0\\0\\\\\\\\1\\0\\\\0\\0";
		if (numlines == 9811) str = "add_item\\9811\\4\\0\\19\\1\\Aldo's Soul Eater Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\10\\4\\1\\1\\255,255,0,0\\255,0,0,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9812) str = "add_item\\9812\\28\\0\\20\\1\\Sword Of Yuna\\swords11.rttex\\1830339645\\19\\-1\\0\\1\\1\\0\\0\\0\\18\\5\\999\\250\\game/swords11_icon.rttex\\1089435820\\2500\\\\0\\\\\\0\\0\\0\\0\\255,255,255,255\\255,255,255,255\\0\\3\\0\\0,0|1,0|2,0|3,0|4,0|5,0|6,0|7,0|8,0|9,0|10,0|11,0|12,0|0,0|\\\\\\5\\7416\\\\0\\0";
		if (numlines == 9813) str = "add_item\\9813\\0\\0\\19\\1\\Sword Of Yuna Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\10\\4\\1\\1\\255,255,0,0\\255,0,0,255\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9814) str = "add_item\\9814\\4\\0\\17\\3\\IOTM Mystery Box\\c_items1.rttex\\2062816473\\0\\-1\\5\\11\\1\\0\\1\\8\\1\\0\\999\\200\\\\0\\0\\\\\\\\\\0\\0\\0\\0\\255,56,49,74\\255,49,47,54\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9815) str = "add_item\\9815\\0\\0\\19\\0\\IOTM Mystery Box Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\5\\1\\1\\5\\255,56,49,74\\255,49,47,54\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";

		if (numlines == 9816) str = "add_item\\9816\\0\\0\\17\\3\\Uranium Ore\\gtps/uranium.rttex\\1801568315\\0\\-1\\0\\0\\2\\0\\1\\5\\1\\0\\220\\200\\\\0\\0\\\\\\\\\\0\\0\\0\\0\\0,0,0,0\\0,0,0,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9817) str = "add_item\\9817\\0\\0\\19\\0\\Uranium Ore Seed\\gtps/uranium.rttex\\1801568315\\0\\-1\\3\\0\\1\\0\\0\\20\\2\\0\\220\\200\\\\0\\400\\\\\\\\\\7\\12\\5\\7\\255,161,45,184\\255,161,45,184\\691200\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9818) str = "add_item\\9818\\4\\0\\20\\1\\Playful Tomato\\gtps/clothes.rttex\\-2134506641\\6\\-1\\3\\0\\1\\0\\0\\10\\1\\5\\999\\200\\game/gtps/pet_tomato.rttex\\1442054562\\0\\\\\\\\\\0\\0\\0\\0\\255,255,0,0\\255,255,0,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9819) str = "add_item\\9819\\0\\0\\19\\0\\Playful Tomato Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\5\\1\\1\\5\\255,255,0,0\\255,0,255,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9820) str = "add_item\\9820\\4\\128\\20\\1\\Latte Cat\\gtps/clothes.rttex\\-2134506641\\6\\-1\\1\\0\\1\\0\\0\\10\\1\\5\\999\\200\\game/gtps/pet_latte.rttex\\-1825212144\\0\\\\\\\\\\0\\0\\0\\0\\255,255,0,0\\255,255,0,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9821) str = "add_item\\9821\\0\\0\\19\\0\\Latte Cat Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\5\\1\\1\\5\\255,255,0,0\\255,0,255,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9822) str = "add_item\\9822\\4\\0\\20\\1\\Playful Blueberry\\gtps/clothes.rttex\\-2134506641\\6\\-1\\0\\0\\1\\0\\0\\10\\1\\5\\999\\200\\game/gtps/pet_berry.rttex\\-1926516459\\0\\\\\\\\\\0\\0\\0\\0\\255,255,0,0\\255,255,0,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9823) str = "add_item\\9823\\0\\0\\19\\0\\Playful Blueberry Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\5\\1\\1\\5\\255,255,0,0\\255,0,255,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9824) str = "add_item\\9824\\4\\0\\20\\1\\Playful Lemon\\gtps/clothes.rttex\\-2134506641\\6\\-1\\2\\0\\1\\0\\0\\10\\1\\5\\999\\200\\game/gtps/pet_mbs.rttex\\-107270147\\0\\\\\\\\\\0\\0\\0\\0\\255,255,0,0\\255,255,0,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9825) str = "add_item\\9825\\0\\0\\19\\0\\Playful Lemon Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\5\\1\\1\\5\\255,255,0,0\\255,0,255,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9826) str = "add_item\\9826\\4\\0\\8\\1\\Provider Wand\\gtps/tiles.rttex\\-496300511\\0\\-1\\3\\0\\1\\0\\0\\10\\1\\0\\999\\200\\\\0\\0\\\\\\\\\\0\\0\\0\\0\\255,255,0,0\\255,255,0,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9827) str = "add_item\\9827\\0\\0\\19\\0\\Provider Wand Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\5\\1\\1\\5\\255,255,0,0\\255,0,255,0\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9828) str = "add_item\\9828\\23\\132\\17\\1\\Salesman\\gtps/tiles.rttex\\-496300511\\4\\-1\\0\\5\\1\\0\\0\\8\\1\\0\\999\\200\\\\0\\400\\\\\\\\\\0\\0\\0\\0\\255,56,49,74\\255,49,47,54\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";
		if (numlines == 9829) str = "add_item\\9829\\0\\0\\19\\0\\Salesman Seed\\\\0\\0\\-1\\0\\0\\0\\0\\0\\0\\0\\0\\999\\200\\\\0\\0\\\\\\\\\\5\\1\\1\\5\\255,56,49,74\\255,49,47,54\\3600\\0\\0\\\\\\\\0\\0\\\\0\\0";

		string second = "";
		bool add_that = false;
		if (str.find(".mp3\\") != string::npos) {
			string first = get_str_between_two_str(str, ".mp3\\", "\\");
			second = get_str_between_two_str(first, "\\", "\\");
			//cout << " first " << first << endl;
			//cout << " second " << second << endl;
			replace_str(str, get_str_between_two_str(str, ".mp3\\", "\\"), "0");
			add_that = true;
		}
		new_items += str + (add_that ? "\\" + second : "") + "\n";
		//if (numlines < 9606) cout << "if (numlines == "+ to_string(numlines) +") str = \"" << str << "\"; " << endl;
	}
	fstream fs;
	fs.open("new.txt", ios::in | ios::app | ios::out | fstream::app), fs << new_items, fs.close();*/
	BOOL ret = SetConsoleCtrlHandler(ConsoleHandler, TRUE);
	srand(unsigned int(time(nullptr)));
	if (items_dat() == -1) cout << "items.dat klaida" << endl;
	else cout << setGems(items.size()) << " items loaded" << endl;
	cout << "server port?:" << endl;
	cin >> server_port;
	if (init_enet(server_port) == -1) cout << "enet klaida" << endl;
	else cout << "[Server]" << "|PORT:" << server_port << "|MAX ITEMS : " << setGems(items.size()) << "| " << endl;
	daily_quest_reset();
	{
		ifstream ifs("db/server_events.json");
		if (ifs.is_open()) {
			json j;
			ifs >> j;
			top_basher = (!(j.find("2") != j.end()) ? top_basher : j["2"].get<vector<pair<int, string>>>());
			top_basher_winners = (!(j.find("3") != j.end()) ? top_basher_winners : j["3"].get<vector<pair<int, string>>>());
			top_old_winners = (!(j.find("4") != j.end()) ? top_old_winners : j["4"].get<string>());
			ridbans = (!(j.find("rid") != j.end()) ? ridbans : j["rid"].get<vector<string>>());
			top_points = (!(j.find("h" + to_string(today_day)) != j.end()) ? top_points : j["h" + to_string(today_day)].get<vector<pair<int, string>>>());
			top_yesterday = (!(j.find("h" + to_string(today_day - 1)) != j.end()) ? top_yesterday : j["h" + to_string(today_day - 1)].get<vector<pair<int, string>>>());
			top_overall = (!(j.find("m" + to_string(today_month)) != j.end()) ? top_overall : j["m" + to_string(today_month)].get<vector<pair<int, string>>>());
		}
	}
	honors_reset();
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	{
		const char* months[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
		string month = months[newtime.tm_mon], translated = "", str = to_string(today_day), locke = "";
		if (str == "01" || str == "21") translated = "st";
		else if (str == "02" || str == "22") translated = "nd";
		else if (str == "03") translated = "rd";
		else translated = "th";
		if (thedaytoday == 5 or thedaytoday == 6 or thedaytoday == 0 or thedaytoday == 1) locke = "\nadd_spacer|small|\nadd_textbox|`oToday Growtopia is being paid a visit by `5Locke`` the traveling salesman! He comes one day a month to hawk his fabulous wares, though this time he'll stick around a day and a half to introduce himself. Checkout the world `5LOCKETOWN``!``|left|";
		news_texture = "set_default_color|`o\nadd_label_with_icon|big|`wThe GTPS 3 Gazette``|left|5016|\nadd_image_button||interface/large/news_banner.rttex|bannerlayout|||\nadd_spacer|small|\nadd_image_button|iotm_layout|interface/gtps/discord.rttex|3imageslayout|https://discord.gg/gtps3|Do you want to join our Discord Server?|\nadd_image_button|rules|interface/gtps/rules.rttex|3imageslayout|||\nadd_image_button|promote|interface/gtps/features.rttex|3imageslayout|||\nadd_spacer|small|\nadd_textbox|`w" + month + " " + to_string(today_day) + "" + translated + ": ```5October Update - Grow Pass``|left|"/*"\nadd_spacer|small|\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_feature_btn04.rttex|3imageslayout|||\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_feature_btn03.rttex|3imageslayout|||\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_feature_btn10.rttex|3imageslayout|||"*/"" + locke + "\nadd_spacer|small|\nadd_textbox|`2GTPS3`` is back with a brand new different server! Now you can finally feel free and own any item you ever wished for!|left|\nadd_spacer|small|\nadd_textbox|What's different? Trees are growing `22x`` faster, providers time is reduced by `250%``, gems are dropping `22x`` and shop is `250%`` cheaper. Also checkout our world `2START`` for more server features.|left|\nadd_spacer|small|\nadd_textbox|We are doing giveaways every few hours! Also we created our own `2GTPS MARKET`` for premium wls, the shop can be found with a command `2/shop``.|left|\nadd_spacer|small|\nadd_textbox|For in-game updates and other information check our `2discord's`` server channel `2#updates in discord.gg/gtps3``.|left|\nadd_spacer|small|\nadd_spacer|small|\nadd_button|deposit|`wContinue``|noflags|0|0|\nend_dialog|gazette|"; // \nadd_quick_exit|
	}
	if (thedaytoday == 1) theitemtoday = 5040;
	else if (thedaytoday == 2) theitemtoday = 5042;
	else if (thedaytoday == 3) theitemtoday = 5044;
	else if (thedaytoday == 4) theitemtoday = 5032;
	else if (thedaytoday == 5)theitemtoday = 5034;
	else if (thedaytoday == 6) theitemtoday = 5036;
	else if (thedaytoday == 0)theitemtoday = 5038;
	ENetEvent event;
	while (true) {
		while (enet_host_service(server, &event, 1100) > 0 && f_saving_ == false) {
			//loop_worlds();
			ENetPeer* peer = event.peer;
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
			{
				bool quit = false;
				char clientConnection[16];
				enet_address_get_host_ip(&peer->address, clientConnection, 16);
				//cout << clientConnection << endl;
				vector<pair<string, long long int>>::iterator p = find_if(banned_ip_temporary.begin(), banned_ip_temporary.end(), [&](const pair < string, long long int>& element) { return element.first == clientConnection; });
				if (p != banned_ip_temporary.end()) if (banned_ip_temporary[p - banned_ip_temporary.begin()].second + CONNECT_TIME > (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) enet_peer_disconnect_later(peer, 0), quit = true;
				/*
				for (pair<string, long long int> p : banned_ip_temporary) {
					if (p.first == clientConnection) {
						if (p.second + CONNECT_TIME > (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) enet_peer_disconnect_later(peer, 0), quit = true;
					}
				}*/
				if (quit) break;
				send_(peer, 1, nullptr, 0);
				peer->data = new Player;
				pInfo(peer)->id = peer->connectID;
				pInfo(peer)->ip = clientConnection;
				//if (pInfo(peer)->ip != "94.244.79.91")  enet_peer_disconnect_later(peer, 0);
				string error = "";
				int logged = 0;
				if (login_time + 6500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
					login_count = 0;
					login_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
				}
				if (login_count < 10) {
					for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->ip != pInfo(currentPeer)->ip) continue;
						logged++;
					}
				}
				if (logged >= 3 || login_count > 10)error = "action|log\nmsg|CT:[S]_ `4OOPS:`` Too many people logging in at once. Please press `5CANCEL`` and try again in a few seconds.";
				//if (pInfo(peer)->ip != pInfo(peer)->meta and pInfo(peer)->meta != "") error = "action|log\nmsg|CT:[S]_ `4Can not make new account!`` Sorry, but IP " + pInfo(peer)->ip + " is not permitted to create NEW Growtopia accounts at this time. (This can be because there is an open proxy/VPN here or abuse has from this IP) Please try again from another IP address.";
				if (error != "") packet_(peer, error, ""), enet_peer_disconnect_later(peer, 0);
				break;
			}
			case ENET_EVENT_TYPE_RECEIVE:
			{
				//loop_save(peer);
				if (pInfo(peer)->bypass == false) {
					pInfo(peer)->new_packets++;
					if (pInfo(peer)->new_packets >= 15) {
						add_modlogs(peer, pInfo(peer)->name_color + pInfo(peer)->tankIDName, "`6 [" + pInfo(peer)->ip + " SHADOW BANNED BY SYSTEM (PACKET BYPASS 1) +" + text_(event.packet), "");
						cout << "banned for bypassing packets >> " << pInfo(peer)->tankIDName << endl;
						banned_ip_temporary.push_back(make_pair(pInfo(peer)->ip, (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()));
						enet_peer_disconnect_later(peer, 0);
					}
				}
				switch (message_(event.packet)) {
				case 2:
				{
					string cch = text_(event.packet);
					if (cch.size() > 1024 || not isASCII(cch)) break;
					if (cch == "action|getDRAnimations\n" || cch == "action|refresh_player_tribute_data\n") break;
					if (pInfo(peer)->bypass == false) {
						if (cch == "action|enter_game\n" || cch.find("requestedName|") != string::npos || cch.find("tankIDName|") != string::npos || cch.find("action|dialog_return\ndialog_name|puzzle_captcha_submit\n") != string::npos || cch == "action|getDRAnimations\n" || cch == "action|refresh_player_tribute_data\n" || cch.find("action|dialog_return\ndialog_name|2fa\nverificationcode|") != string::npos) {

						}
						else {
							 add_modlogs(peer, pInfo(peer)->name_color + pInfo(peer)->tankIDName, "`6 [" + pInfo(peer)->ip + " SHADOW BANNED BY SYSTEM (PACKET BYPASS 2)", "");
							cout << "banned for bypassing packets >> " << pInfo(peer)->tankIDName << " | " <<  cch << endl;
							banned_ip_temporary.push_back(make_pair(pInfo(peer)->ip, (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()));
							enet_peer_disconnect_later(peer, 0);
						}
					}
					//cout << cch << endl;
					if (pInfo(peer)->lpps + 1000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
						pInfo(peer)->pps = 0;
						pInfo(peer)->lpps = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
					}
					else {
						pInfo(peer)->pps++;
						if (pInfo(peer)->pps >= 10) {
							enet_peer_disconnect_later(peer, 0);
							break;
						}
					}
					loop_worlds(); 
					if (pInfo(peer)->requestedName.empty()) {
						if (pInfo(peer)->enter_game != 0 || pInfo(peer)->world != "") enet_peer_disconnect_later(peer, 0);
						else {
							if (!player_login(peer, cch)) {
								break;
							}
						}
					}
					else if (cch.find("action|input") != string::npos) {
						vector<string> t_ = explode("|", cch);
						if (t_.size() < 4) break;
						string msg = explode("\n", t_[3])[0];
						if (pInfo(peer)->tankIDName == "") break;
						if (msg.length() <= 0 || msg.length() > 120 || msg.empty() || std::all_of(msg.begin(), msg.end(), [](char c) {return std::isspace(c); })) continue;
						for (char c : msg) if (c < 0x20 || c>0x7A) continue;
						space_(msg);
						if (msg[0] == '/') SendCmd(peer, msg);
						else {
							if (msg[0] == '`' and msg.size() <= 2) break;
							if (pInfo(peer)->world == "") break;
							if (has_playmod(pInfo(peer), "duct tape") || has_playmod(pInfo(peer), "Iron MMMFF")) {
								string msg2 = "";
								for (int i = 0; i < msg.length(); i++) {
									if (isspace(msg[i])) msg2 += " ";
									else {
										if (isupper(msg[i])) msg2 += i % 2 == 0 ? "M" : "F";
										else msg2 += i % 2 == 0 ? "m" : "f";
									}
								}
								msg = msg2;
							}
							string check_ = msg;
							transform(check_.begin(), check_.end(), check_.begin(), ::tolower);
							{
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									if (world_->silence and pInfo(peer)->superdev != 1 and world_->owner_name != pInfo(peer)->tankIDName and find(world_->admins.begin(), world_->admins.end(), pInfo(peer)->tankIDName) == world_->admins.end()) {
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`1(Peasants must not speak)"), p.Insert(1), p.CreatePacket(peer);
										break;
									}
								}
							}
							bool warned = false;
							pInfo(peer)->not_same++;
							if (pInfo(peer)->last_input_text == msg) pInfo(peer)->same_input++;
							pInfo(peer)->last_input_text = msg;
							if (pInfo(peer)->last_spam_detection + 5000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) pInfo(peer)->last_spam_detection = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count(), pInfo(peer)->same_input = 0, pInfo(peer)->not_same = 0;
							if (pInfo(peer)->same_input >= 3 || pInfo(peer)->not_same >= 5) {
								gamepacket_t p;
								p.Insert("OnConsoleMessage"), p.Insert("`6>>`4Spam detected! ``Please wait a bit before typing anything else.  Please note, any form of bot/macro/auto-paste will get all your accounts banned, so don't do it!"), p.CreatePacket(peer);
							}
							else {
								replaceAll(msg, "`%", "");
								string chat_color = "`$";
								if (pInfo(peer)->d_name.empty()) {
									chat_color = pInfo(peer)->superdev ? "`5" : pInfo(peer)->dev ? "`1" : pInfo(peer)->tmod ? "`^" : "`$";
								}
								if (has_playmod(pInfo(peer), "Infected!")) {
									chat_color = "`2";
									if (rand() % 4 < 1) chat_color += "Brraaiinnss...";
								}
								if (pInfo(peer)->face == 1170)  chat_color = "`4";
							    gamepacket_t p, p2;
								p.Insert("OnConsoleMessage");
								p.Insert("CP:_PL:0_OID:_CT:[W]_ `6<`w" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->name_color + pInfo(peer)->tankIDName) + (pInfo(peer)->is_legend ? " of Legend" : "") + "`6> "+ chat_color + msg);
								p2.Insert("OnTalkBubble");
								p2.Insert(pInfo(peer)->netID);
								if (check_ != ":/" and check_ != ":p" and check_ != ":*" and check_ != ";)" and check_ != ":d" and check_ != ":o" and check_ != ":'(" and check_ != ":(") {
									p2.Insert("CP:_PL:0_OID:_player_chat=" + (chat_color == "`$" ? "" : chat_color) + msg);
								}
								else p2.Insert(msg);
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (pInfo(currentPeer)->world == pInfo(peer)->world) {
										bool muted_ = false;
										for (int c_ = 0; c_ < pInfo(currentPeer)->friends.size(); c_++) {
											if (pInfo(currentPeer)->friends[c_].name == pInfo(peer)->tankIDName) {
												if (pInfo(currentPeer)->friends[c_].mute) {
													muted_ = true;
													break;
												}
											}
										}
										if (not muted_) {
											p.CreatePacket(currentPeer);
											p2.CreatePacket(currentPeer);
										}
									}
								}
							}
						}
						break;
					}
					else if (cch.find("action|mod_trade") != string::npos or cch.find("action|rem_trade") != string::npos) {
						vector<string> t_ = explode("|", cch);
						if (t_.size() < 3) break;
						int item_id = atoi(explode("\n", t_[2])[0].c_str()), c_ = 0;
						modify_inventory(peer, item_id, c_);
						if (c_ == 0) break;
						if (items[item_id].untradeable || items[item_id].blockType == BlockTypes::FISH) {
							gamepacket_t p;
							p.Insert("OnTextOverlay");
							p.Insert("You'd be sorry if you lost that!");
							p.CreatePacket(peer);
							break;
						} if (c_ == 1 or cch.find("action|rem_trade") != string::npos) {
							mod_trade(peer, item_id, c_, (cch.find("action|rem_trade") != string::npos ? true : false));
							break;
						}
						if (cch.find("action|rem_trade") == string::npos) {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`2Trade`` `w" + items[item_id].name + "``|left|" + to_string(item_id) + "|\nadd_textbox|`2Trade how many?``|left|\nadd_text_input|count||" + to_string(c_) + "|5|\nembed_data|itemID|" + to_string(item_id) + "\nend_dialog|trade_item|Cancel|OK|");
							p.CreatePacket(peer);
						}
						break;
					}
					else if (cch.find("action|trade_accept") != string::npos) {
						if (pInfo(peer)->trading_with != -1) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 3) break;
							string status_ = explode("\n", t_[2])[0];
							if (status_ != "1" and status_ != "0") break;
							bool f_ = false;
							for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
								if (pInfo(currentPeer)->world == pInfo(peer)->world) {
									if (pInfo(currentPeer)->netID == pInfo(peer)->trading_with and pInfo(peer)->netID == pInfo(currentPeer)->trading_with) {
										string name_ = pInfo(peer)->world;
										vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
										if (p != worlds.end()) {
											World* world_ = &worlds[p - worlds.begin()];
											if (status_ == "1")
												pInfo(peer)->trade_accept = 1;
											else
												pInfo(peer)->trade_accept = 0;
											if (pInfo(peer)->trade_accept and pInfo(currentPeer)->trade_accept) {
												// check inv space   
												if (not trade_space_check(peer, currentPeer)) {
													pInfo(peer)->trade_accept = 0, pInfo(currentPeer)->trade_accept = 0;
													gamepacket_t p;
													p.Insert("OnTradeStatus");
													p.Insert(pInfo(peer)->netID);
													p.Insert("");
													p.Insert("`o" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->name_color + pInfo(peer)->tankIDName) + "``'s offer.``");
													p.Insert(make_trade_offer(pInfo(peer), true) + "locked|0\naccepted|0");
													p.CreatePacket(peer);
													{
														gamepacket_t p;
														p.Insert("OnTradeStatus");
														p.Insert(pInfo(peer)->netID);
														p.Insert("");
														p.Insert("`o" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->name_color + pInfo(peer)->tankIDName) + "``'s offer.``");
														p.Insert(make_trade_offer(pInfo(peer), true) + "locked|0\nreset_locks|1\naccepted|0");
														p.CreatePacket(currentPeer);
													}
													f_ = true;
													break;
												}
												else if (not trade_space_check(currentPeer, peer)) {
													pInfo(peer)->trade_accept = 0, pInfo(currentPeer)->trade_accept = 0;
													gamepacket_t p;
													p.Insert("OnTradeStatus");
													p.Insert(pInfo(currentPeer)->netID);
													p.Insert("");
													p.Insert("`o" + (not pInfo(currentPeer)->d_name.empty() ? pInfo(currentPeer)->d_name : pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName) + "``'s offer.``");
													p.Insert(make_trade_offer(pInfo(currentPeer), true) + "locked|0\naccepted|0");
													p.CreatePacket(currentPeer);
													{
														gamepacket_t p;
														p.Insert("OnTradeStatus");
														p.Insert(pInfo(currentPeer)->netID);
														p.Insert("");
														p.Insert("`o" + (not pInfo(currentPeer)->d_name.empty() ? pInfo(currentPeer)->d_name : pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName) + "``'s offer.``");
														p.Insert(make_trade_offer(pInfo(currentPeer), true) + "locked|0\nreset_locks|1\naccepted|0");
														p.CreatePacket(peer);
													}
													f_ = true;
													break;
												}
												{
													gamepacket_t p;
													p.Insert("OnForceTradeEnd");
													p.CreatePacket(peer);
												}
												bool blocked = false;
												for (int i_ = 0; i_ < pInfo(currentPeer)->trade_items.size(); i_++) {
													map<string, int>::iterator it;
													for (auto it = pInfo(currentPeer)->trade_items[i_].begin(); it != pInfo(currentPeer)->trade_items[i_].end(); it++) {
														int c_ = 0;
														modify_inventory(currentPeer, it->first, c_);
														if (c_ == 0) if (pInfo(currentPeer)->trading_with != -1) cancel_trade(currentPeer, false, true), blocked = true;
													}
												}
												for (int i_ = 0; i_ < pInfo(peer)->trade_items.size(); i_++) {
													map<string, int>::iterator it;
													for (auto it = pInfo(peer)->trade_items[i_].begin(); it != pInfo(peer)->trade_items[i_].end(); it++) {
														int c_ = 0;
														modify_inventory(peer, it->first, c_);
														if (c_ == 0) if (pInfo(peer)->trading_with != -1) cancel_trade(peer, false, true), blocked = true;
													}
												}
												if (blocked == false) send_trade_confirm_dialog(peer, currentPeer);
												break;
											}
											gamepacket_t p;
											p.Insert("OnTradeStatus");
											p.Insert(pInfo(peer)->netID);
											p.Insert("");
											p.Insert("`o" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->name_color + pInfo(peer)->tankIDName) + "``'s offer.``");
											p.Insert(make_trade_offer(pInfo(peer), true) + "locked|0\naccepted|" + status_);
											p.CreatePacket(peer);
											{
												{
													gamepacket_t p;
													p.Insert("OnTradeStatus");
													p.Insert(pInfo(currentPeer)->netID);
													p.Insert("");
													p.Insert("`o" + (not pInfo(currentPeer)->d_name.empty() ? pInfo(currentPeer)->d_name : pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName) + "``'s offer.``");
													p.Insert("locked|0\nreset_locks|1\naccepted|0");
													p.CreatePacket(currentPeer);
												}
												gamepacket_t p;
												p.Insert("OnTradeStatus");
												p.Insert(pInfo(currentPeer)->netID);
												p.Insert("");
												p.Insert("`o" + (not pInfo(currentPeer)->d_name.empty() ? pInfo(currentPeer)->d_name : pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName) + "``'s offer.``");
												p.Insert("locked|0\naccepted|1");
												p.CreatePacket(currentPeer);
												{
													gamepacket_t p;
													p.Insert("OnTradeStatus");
													p.Insert(pInfo(currentPeer)->netID);
													p.Insert("");
													p.Insert("`o" + (not pInfo(currentPeer)->d_name.empty() ? pInfo(currentPeer)->d_name : pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName) + "``'s offer.``");
													p.Insert(make_trade_offer(pInfo(currentPeer), true) + "locked|0\nreset_locks|1\naccepted|0");
													p.CreatePacket(currentPeer);
												}
												{
													gamepacket_t p;
													p.Insert("OnTradeStatus");
													p.Insert(pInfo(peer)->netID);
													p.Insert("");
													p.Insert("`o" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->name_color + pInfo(peer)->tankIDName) + "``'s offer.``");
													p.Insert(make_trade_offer(pInfo(peer), true) + "locked|0\nreset_locks|1\naccepted|" + status_);
													p.CreatePacket(currentPeer);
												}
											}
										}
										f_ = true;
										break;
									}
								}
							} if (not f_) {
								if (status_ == "1")
									pInfo(peer)->trade_accept = 1;
								else
									pInfo(peer)->trade_accept = 0;
							}
						}
						break;
					}
					else if (cch == "action|trade_cancel\n") cancel_trade(peer);
					if (pInfo(peer)->trading_with == -1) {
						if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|newbie\n\n") {
							{
								gamepacket_t p(7500), p2, p3;
								p.Insert("OnClearNPCMessage"), p.CreatePacket(peer);
								p2.Insert("OnAddNotification"), p2.Insert("interface/tutorial/tut07_create_world.rttex"), p2.Insert("`wLet's create a worlds! You can create a worlds by enter a random worlds and lock it!``"), p2.Insert("audio/tip_start.wav"), p2.Insert(1), p2.CreatePacket(peer);
								p3.Insert("OnPersistentNPCMessage"), p3.Insert(4032), p3.Insert("Welcome to Growtopia Private Server! Type /help to see all commands.\n`2Pickup`` your newbie rewards and start the journey!"), p3.Insert(1), p3.CreatePacket(peer);
							}
							pInfo(peer)->n = 1;
							if (pInfo(peer)->cc == 0) {
								int itemid = 9160, c_ = 1;
								if (pInfo(peer)->country == "lt") itemid = 9146;
								if (pInfo(peer)->country == "id") itemid = 9148;
								if (pInfo(peer)->country == "tr") itemid = 9150;
								if (pInfo(peer)->country == "ru") itemid = 9152;
								if (pInfo(peer)->country == "us") itemid = 9154;
								if (pInfo(peer)->country == "fi") itemid = 9156;
								if (pInfo(peer)->country == "cn") itemid = 9158;
								if (pInfo(peer)->country == "ae") itemid = 9502;
								if (pInfo(peer)->country == "ro") itemid = 9504;
								if (pInfo(peer)->country == "nl") itemid = 9506;
								if (pInfo(peer)->country == "lb") itemid = 9508;
								if (pInfo(peer)->country == "hu") itemid = 9510;
								if (pInfo(peer)->country == "ua") itemid = 9512;
								if (pInfo(peer)->country == "ph") itemid = 9514;
								if (modify_inventory(peer, itemid, c_) == 0) {
									pInfo(peer)->cc = 1;
									gamepacket_t p;
									p.Insert("OnDialogRequest");
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`2" + items[itemid].name + "``|left|" + to_string(itemid) + "|\nadd_smalltext|You received `8" + items[itemid].name + "|left|\nadd_smalltext|`9" + items[itemid].description + "|left|\nend_dialog|||OK|\nadd_quick_exit|\n");
									p.CreatePacket(peer);
								}
							}
							string start = "START";
							join_world(peer, start);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|punish_view\nbuttonClicked|warp_to_") != string::npos) {
							if (pInfo(peer)->mod == 1 || pInfo(peer)->dev == 1) {
								string world_name = cch.substr(67, cch.length() - 67);
								replace_str(world_name, "\n", "");
								replace_str(world_name, "reason|", "");
								join_world(peer, world_name);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|blast\nname|") != string::npos) {
							if (pInfo(peer)->lastchoosenitem == 830 || pInfo(peer)->lastchoosenitem == 9164 || pInfo(peer)->lastchoosenitem == 9602 || pInfo(peer)->lastchoosenitem == 942 || pInfo(peer)->lastchoosenitem == 1060 || pInfo(peer)->lastchoosenitem == 1136 || pInfo(peer)->lastchoosenitem == 1402 || pInfo(peer)->lastchoosenitem == 9582 || pInfo(peer)->lastchoosenitem == 1532 || pInfo(peer)->lastchoosenitem == 3562 || pInfo(peer)->lastchoosenitem == 4774 || pInfo(peer)->lastchoosenitem == 7380 || pInfo(peer)->lastchoosenitem == 7588 || pInfo(peer)->lastchoosenitem == 8556) {
								int blast = pInfo(peer)->lastchoosenitem, got = 0;
								modify_inventory(peer, blast, got);
								if (got == 0) break;
								string world = cch.substr(44, cch.length() - 44).c_str();
								replace_str(world, "\n", "");
								transform(world.begin(), world.end(), world.begin(), ::toupper);
								if (find_if(worlds.begin(), worlds.end(), [world](const World& a) { return a.name == world; }) != worlds.end() || not check_blast(world) || _access_s(("worlds/" + world + "_.json").c_str(), 0) == 0) {
									gamepacket_t p;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("That world name already exists. You'll have to be more original. Maybe add some numbers after it?"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
								}
								else {
									if (modify_inventory(peer, blast, got = -1) == 0) {
										create_world_blast(peer, world, blast);
										if (blast == 830) modify_inventory(peer, 834, got = -100);
										join_world(peer, world);
										pInfo(peer)->worlds_owned.push_back(world);
										gamepacket_t p(750), p2(750);
										p.Insert("OnConsoleMessage"), p.Insert("** `5" + pInfo(peer)->tankIDName + " activates a " + items[blast].name + "! ``**"), p.CreatePacket(peer);
										p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("** `5" + pInfo(peer)->tankIDName + " activates a " + items[blast].name + "! ``**"), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|world_spray\n") {
							int got = 0;
							modify_inventory(peer, 12600, got);
							if (got == 0) break;
							string name = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name](const World& a) { return a.name == name; });
							if (p != worlds.end()) {
								World* world = &worlds[p - worlds.begin()];
								if (world->owner_name == pInfo(peer)->tankIDName || pInfo(peer)->superdev) {
									int remove = -1;
									modify_inventory(peer, 12600, remove);
									for (int i_ = 0; i_ < world->blocks.size(); i_++) if (world->blocks[i_].fg % 2 != 0)  world->blocks[i_].planted = _int64(2.592e+6);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(currentPeer)->world == name) {
											int x = pInfo(currentPeer)->x, y = pInfo(currentPeer)->y;
											exit_(currentPeer, true);
											join_world(currentPeer, name);
										}
									}
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("`wYou must own the world!``"), p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|megaphone\nitemID|2480|\nwords|") != string::npos) {
							string text = cch.substr(62, cch.length() - 63).c_str();
							bool cansb = true;
							int c_ = 0;
							modify_inventory(peer, 2480, c_);
							if (c_ == 0) continue;
							if (has_playmod(pInfo(peer), "duct tape")) {
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert("`6>> That's sort of hard to do while duct-taped.``");
								p.CreatePacket(peer);
								cansb = false;
								continue;
							}
							if (has_playmod(pInfo(peer), "megaphone!")) {
								int time_ = 0;
								for (PlayMods peer_playmod : pInfo(peer)->playmods) {
									if (peer_playmod.id == 13) {
										time_ = peer_playmod.time - time(nullptr);
										break;
									}
								}
								packet_(peer, "action|log\nmsg|>> (" + to_playmod_time(time_) + "before you can broadcast again)", "");
								break;
							}
							if (cansb) {
								pInfo(peer)->usedmegaphone = 1;
								SendCmd(peer, "/sb " + text, false);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|notebook_edit\nbuttonClicked|cancel") != string::npos || cch.find("action|dialog_return\ndialog_name|notebook_edit\nbuttonClicked|clear") != string::npos) {
							if (cch.find("action|dialog_return\ndialog_name|notebook_edit\nbuttonClicked|clear") != string::npos) 	pInfo(peer)->note = "";
							send_wrench_self(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|rules\n\n") {
							SendCmd(peer, "/rules", false);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|promote\n\n") {
							vector<string> updates{ {"Guidebook"},{"Captcha"}, {"Offering Table"}, {"Harvest Festival"}, {"Ultra World Spray"}, {"Tesseract Manipulator & Heart of Gaia"}, {"Zombie Apocalypse & Infection"} , {"Comet Dust Event"}, {"Special Event"}};
							string update_List = "";
							for (int i = 0; i < updates.size(); i++) update_List += "\nadd_spacer|small|\nadd_textbox|`0 >`` " + updates[i] + ".|left|";
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wThe GTPS 3 Gazette``|left|5016|\nadd_image_button||interface/large/news_banner.rttex|bannerlayout|||\nadd_spacer|small|\nadd_image_button|iotm_layout|interface/gtps/discord.rttex|3imageslayout|https://discord.gg/gtps3|Do you want to join our Discord Server?|\nadd_image_button|rules|interface/gtps/rules.rttex|3imageslayout|||\nadd_image_button|promote|interface/gtps/features.rttex|3imageslayout|||\nadd_spacer|small|\nadd_textbox|`5Features`` `0(recently added)```5:``|left|" + update_List + "\nadd_spacer|small|\nadd_button|close|OK|noflags|0|0|\nend_dialog|gazette|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|notebook_edit\nbuttonClicked|save\n\npersonal_note|") != string::npos) {
							string btn = cch.substr(81, cch.length() - 82).c_str();
							if (btn.length() > 128) continue;
							pInfo(peer)->note = btn;
							send_wrench_self(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|3898\nbuttonClicked|") != string::npos || cch == "action|dialog_return\ndialog_name|zurgery_back\nbuttonClicked|53785\n\n" || cch == "action|dialog_return\ndialog_name|zombie_back\nbuttonClicked|53785\n\n") {
							string btn = cch.substr(52, cch.length() - 52).c_str();
							bool fail = false;
							if (cch == "action|dialog_return\ndialog_name|zurgery_back\nbuttonClicked|53785\n\n" || cch == "action|dialog_return\ndialog_name|zombie_back\nbuttonClicked|53785\n\n") btn = "53785";
							replace_str(btn, "\n", "");
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (btn == "12345") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wCrazy Jim's Quest Emporium``|left|3902|\nadd_textbox|HEEEEYYY there Growtopian! I'm Crazy Jim, and my quests are so crazy they're KERRRRAAAAZZY!! And that is clearly very crazy, so please, be cautious around them. What can I do ya for, partner?|left|\nadd_button|chc1_1|Daily Quest|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							else if (btn == "53785") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man``|left|4358|\nadd_textbox|It is I, Sales-Man, savior of the wealthy! Let me rescue you from your riches. What would you like to buy today?|left|\nadd_button|chc4_1|Surgery Items|noflags|0|0|\nadd_button|chc3_1|Zombie Defense Items|noflags|0|0|\nadd_button|chc2_1|Blue Gem Lock|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							else if (btn == "chc1_1") {
								if (today_day != pInfo(peer)->dd) {
									int haveitem1 = 0, haveitem2 = 0, received = 0;
									modify_inventory(peer, item1, haveitem1);
									modify_inventory(peer, item2, haveitem2);
									if (haveitem1 >= item1c && haveitem2 >= item2c) received = 1;
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wCrazy Jim's Daily Quest``|left|3902|\nadd_textbox|I guess some people call me Crazy Jim because I'm a bit of a hoarder. But I'm very particular about what I want! And today, what I want is this:|left|\nadd_label_with_icon|small|`2" + to_string(item1c) + " " + items[item1].name + "|left|" + to_string(item1) + "|\nadd_smalltext|and|left|\nadd_label_with_icon|small|`2" + to_string(item2c) + " " + items[item2].name + "|left|" + to_string(item2) + "|\nadd_spacer|small|\nadd_smalltext|You shove all that through the phone (it works, I've tried it), and I will hand you one of the `2Growtokens`` from my personal collection!  But hurry, this offer is only good until midnight, and only one `2Growtoken`` per person!|left|\nadd_spacer|small|\nadd_smalltext|`6(You have " + to_string(haveitem1) + " " + items[item1].name + " and " + to_string(haveitem2) + " " + items[item2].name + ")``|left|\nadd_spacer|small|" + (received == 1 ? "\nadd_button|turnin|Turn in items|noflags|0|0|" : "") + "\nadd_spacer|small|\nadd_button|12345|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
								}
								else p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wCrazy Jim's Daily Quest``|left|3902|\nadd_textbox|You've already completed my Daily Quest for today! Call me back after midnight to hear about my next cravings.|left|\nadd_button|12345|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							}
							else if (btn == "turnin") {
								if (today_day != pInfo(peer)->dd) {
									int haveitem1 = 0, haveitem2 = 0, received = 0, remove = -1, remove2 = -1, giveitem = 1;
									modify_inventory(peer, item1, haveitem1);
									modify_inventory(peer, item2, haveitem2);
									if (haveitem1 >= item1c && haveitem2 >= item2c) received = 1;
									if (received == 1) {
										if (thedaytoday == 4) giveitem = 3;
										if (has_playmod(pInfo(peer), "Food: Delectable")) {
											if (rand() % 100 < 25) giveitem *= 2;
										}
										int gots = giveitem;
										pInfo(peer)->dd = today_day;
										modify_inventory(peer, item1, remove *= item1c);
										modify_inventory(peer, item2, remove2 *= item2c);
										modify_inventory(peer, 1486, giveitem);
										{
											{
												string name_ = pInfo(peer)->world;
												vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
												if (p != worlds.end()) {
													World* world_ = &worlds[p - worlds.begin()];
													PlayerMoving data_{};
													data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16;
													data_.packetType = 19, data_.plantingTree = 500;
													data_.punchX = 1486, data_.punchY = pInfo(peer)->netID;
													int32_t to_netid = pInfo(peer)->netID;
													BYTE* raw = packPlayerMoving(&data_);
													raw[3] = 5;
													memcpy(raw + 8, &to_netid, 4);
													for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
														if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
														if (pInfo(currentPeer)->world == world_->name) {
															send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
														}
													}
													delete[] raw;
												}
											}
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("[`6You jammed " + to_string(item1c) + " " + items[item1].name + " and " + to_string(item2c) + " " + items[item2].name + " into the phone, and " + to_string(gots) + " `2Growtoken`` popped out!``]");
											p.CreatePacket(peer);
										}
									}
								}
								else {
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wCrazy Jim's Daily Quest``|left|3902|\nadd_textbox|You've already completed my Daily Quest for today! Call me back after midnight to hear about my next cravings.|left|\nadd_button|12345|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
									p.CreatePacket(peer);
								}
							}
							else if (btn == "chc2_1") {
								int c_ = 0;
								modify_inventory(peer, 1796, c_);
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBlue Gem Lock``|left|7188|\nadd_textbox|Excellent! I'm happy to sell you a Blue Gem Lock in exchange for 100 Diamond Lock..|left|\nadd_smalltext|`6You have " + to_string(c_) + " Diamond Lock.``|left|" + (c_ >= 100 ? "\nadd_button|chc2_2_1|Thank you!|noflags|0|0|" : "") + "\nadd_button|53785|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							}
							else if (btn == "chc2_2_1") {
								int c7188 = 0, c1796 = 0, additem = 0;
								modify_inventory(peer, 1796, c1796);
								if (c1796 < 100) continue;
								modify_inventory(peer, 7188, c7188);
								if (c7188 >= 200) {
									gamepacket_t p;
									p.Insert("OnTalkBubble");
									p.Insert(pInfo(peer)->netID);
									p.Insert("You don't have room in your backpack!");
									p.Insert(0), p.Insert(1);
									p.CreatePacket(peer);
									{
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("You don't have room in your backpack!");
										p.CreatePacket(peer);
									}
									fail = true;
									continue;
								}
								if (c1796 >= 100) {
									if (get_free_slots(pInfo(peer)) >= 2) {
										int cz_ = 1;
										if (modify_inventory(peer, 1796, additem = -100) == 0) {
											modify_inventory(peer, 7188, additem = 1);
											{
												PlayerMoving data_{};
												data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16;
												data_.packetType = 19, data_.plantingTree = 500;
												data_.punchX = 7188, data_.punchY = pInfo(peer)->netID;
												int32_t to_netid = pInfo(peer)->netID;
												BYTE* raw = packPlayerMoving(&data_);
												raw[3] = 5;
												memcpy(raw + 8, &to_netid, 4);
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
													if (pInfo(currentPeer)->world == pInfo(peer)->world) {
														send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
													}
												}
												delete[] raw;
												gamepacket_t p;
												p.Insert("OnConsoleMessage");
												p.Insert("[`6You spent 100 Diamond Lock to get 1 Blue Gem Lock``]");
												p.CreatePacket(peer);
											}
										}
										else fail = true;
										int c_ = 0;
										modify_inventory(peer, 1796, c_);
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBlue Gem Lock``|left|7188|\nadd_textbox|Excellent! I'm happy to sell you a Blue Gem Lock in exchange for 100 Diamond Lock..|left|\nadd_smalltext|`6You have " + to_string(c_) + " Diamond Lock.``|left|" + (c_ >= 100 ? "\nadd_button|chc2_2_1|Thank you!|noflags|0|0|" : "") + "\nadd_button|53785|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
									}
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("You don't have enough inventory space!");
									p.CreatePacket(peer);
									fail = true;
								}
							}
							else if (btn == "chc3_1") {
								int zombie_brain = 0, pile = 0, total = 0;
								modify_inventory(peer, 4450, zombie_brain);
								modify_inventory(peer, 4452, pile);
								total += zombie_brain + (pile * 100);
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man: Zombie Defense``|left|4358|\nadd_textbox|Excellent! I'm happy to sell you Zombie Defense supplies in exchange for Zombie Brains.|left|\nadd_smalltext|You currently have " + setGems(total) + " Zombie Brains.|left|\nadd_spacer|small|\ntext_scaling_string|5,000ZB|\n" + zombie_list + "\nadd_button_with_icon||END_LIST|noflags|0||\nadd_button|53785|Back|noflags|0|0|\nend_dialog|zombie_back|Hang Up||\n");
								p.CreatePacket(peer);
							}
							else if (btn == "chc4_1") {
								int zombie_brain = 0, pile = 0, total = 0;
								modify_inventory(peer, 4298, zombie_brain);
								modify_inventory(peer, 4300, pile);
								total += zombie_brain + (pile * 100);
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man: Surgery``|left|4358|\nadd_textbox|Excellent! I'm happy to sell you rare and precious Surgery prizes in exchange for Caduceus medals.|left|\nadd_smalltext|You currently have " + setGems(total) + " Caducei.|left|\nadd_spacer|small|\ntext_scaling_string|50,000ZB|\n" + surgery_list + "\nadd_button_with_icon||END_LIST|noflags|0||\nadd_button|53785|Back|noflags|0|0|\nend_dialog|zurgery_back|Hang Up||\n");
								p.CreatePacket(peer);
							}
							else p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wDisconnected``|left|774|\nadd_textbox|The number you have tried to reach is disconnected. Please check yourself before you wreck yourself.|left|\nend_dialog|3898|Hang Up||\n");
							if (btn != "turnin" && fail == false) p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|punish_view\nbuttonClicked|view_inventory") != string::npos) {
							if (pInfo(peer)->mod == 1 || pInfo(peer)->dev == 1) {
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
										string recently_visited = "";
										for (auto it = pInfo(currentPeer)->last_visited_worlds.rbegin(); it != pInfo(currentPeer)->last_visited_worlds.rend(); ++it) {
											string a_ = *it + (next(it) != pInfo(currentPeer)->last_visited_worlds.rend() ? "``, " : "``");
											recently_visited += "`#" + a_;
										}
										string inventory = "";
										for (int i_ = 0; i_ < pInfo(currentPeer)->inv.size(); i_++) {
											if (pInfo(currentPeer)->inv[i_].id == 0 || pInfo(currentPeer)->inv[i_].id == 18 || pInfo(currentPeer)->inv[i_].id == 32) continue;
											inventory += "\nadd_button_with_icon|" + (pInfo(peer)->dev == 1 ? to_string(pInfo(currentPeer)->inv[i_].id) : "") + "||staticBlueFrame|" + to_string(pInfo(currentPeer)->inv[i_].id) + "|" + to_string(pInfo(currentPeer)->inv[i_].count) + "|";
										}
										string item_log = "\nadd_spacer|small|\ntext_scaling_string|aaaaaaaaaaaaaaa|";
										for (int i_ = 0; i_ < pInfo(currentPeer)->bp.size(); i_++) item_log += "\nadd_button_with_icon||" + items[pInfo(currentPeer)->bp[i_].first].name + "|frame|" + to_string(pInfo(currentPeer)->bp[i_].first) + "|" + to_string(pInfo(currentPeer)->bp[i_].second) + "|";
										gamepacket_t p;
										p.Insert("OnDialogRequest");
										p.Insert("set_default_color|`o\nadd_label_with_icon|small|`0Inventory of " + pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName + "``'s (" + pInfo(currentPeer)->requestedName + ") - #" + to_string(pInfo(currentPeer)->netID) + "|left|3802|\nadd_spacer|small|\nadd_textbox|Last visited: " + recently_visited + "|\nadd_textbox|Gems: `w" + setGems(pInfo(currentPeer)->gems) + "|\nadd_textbox|Backpack items:|" + (pInfo(currentPeer)->bp.size() >= 1 ? item_log + "\nadd_button_with_icon||END_LIST|noflags|0||" : "no items in backpack.") + "\ntext_scaling_string|50,000OPC|\nadd_textbox|Inventory items: `w" + to_string(pInfo(currentPeer)->inv.size() - 1) + "|" + inventory + (inventory.empty() ? "|" : "\nadd_button_with_icon||END_LIST|noflags|0||") + "\nend_dialog|view_inventory|Continue||\nadd_quick_exit|");
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|punish_view\nbuttonClicked|wipe_inventory") != string::npos) {
							if (pInfo(peer)->superdev) {
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
										pInfo(currentPeer)->inv.clear(), pInfo(currentPeer)->bp.clear();
										pInfo(currentPeer)->gems = 0, pInfo(currentPeer)->vip = 0, pInfo(currentPeer)->mod = 0;
										pInfo(currentPeer)->hair = 0;
										pInfo(currentPeer)->shirt = 0;
										pInfo(currentPeer)->pants = 0;
										pInfo(currentPeer)->feet = 0;
										pInfo(currentPeer)->face = 0;
										pInfo(currentPeer)->hand = 0;
										pInfo(currentPeer)->back = 0;
										pInfo(currentPeer)->mask = 0;
										pInfo(currentPeer)->necklace = 0;
										pInfo(currentPeer)->ances = 0;
										update_clothes(currentPeer);
										{
											gamepacket_t p;
											p.Insert("OnSetBux");
											p.Insert(pInfo(currentPeer)->gems);
											p.Insert(1);
											p.Insert((pInfo(currentPeer)->supp >= 1) ? 1 : 0);
											if (pInfo(currentPeer)->supp >= 2) p.Insert((float)33796, (float)1, (float)0);
											p.CreatePacket(currentPeer);
										}
										vector<uint16_t> list{ 454, 682, 3004, 1154,4584, 526, 5666, 340, 3838, 5990 }, list2{ 1008, 866, 872, 928 };
										uint16_t list_s1 = list[rand() % list.size()], list_s2 = list2[rand() % list2.size()];
										pInfo(currentPeer)->inv.push_back({ 18, 1 }), pInfo(currentPeer)->inv.push_back({ 32, 1 }), pInfo(currentPeer)->inv.push_back({ 6336, 1 }), pInfo(currentPeer)->inv.push_back({ 204, 1 }), pInfo(currentPeer)->inv.push_back({ 98, 1 }), pInfo(currentPeer)->inv.push_back({ 954, 20 }), pInfo(currentPeer)->inv.push_back({ list_s1, 50 }), pInfo(currentPeer)->inv.push_back({ 7066, 2 }), pInfo(currentPeer)->inv.push_back({ 3898, 1 }), pInfo(currentPeer)->inv.push_back({ 8430, 1 }), pInfo(currentPeer)->inv.push_back({ list_s2, 1 });
										for (int i_ = 11; i_ <= 16; i_++) pInfo(currentPeer)->inv.push_back({ 0,0 });

										send_inventory(currentPeer);
										pInfo(currentPeer)->gtwl = 0;
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|top\nbuttonClicked|warp_to_") != string::npos) {
							string world_name = cch.substr(59, cch.length() - 61);
							join_world(peer, world_name);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|wotwlistback\n\n") {
							SendCmd(peer, "/top", true);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|shopgemsconfirm\ngemspurchase|") != string::npos) {
							int gems = atoi(cch.substr(62, cch.length() - 62).c_str());
							if (gems <= 0) break;
							pInfo(peer)->offergems = gems;
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase gems``|left|9436|\nadd_spacer|small|\nadd_textbox|`2Purchase`` `9" + setGems(pInfo(peer)->offergems * 2000) + " Gems`` for `9" + to_string(gems) + " World Locks?``|\nadd_button|shopmoneybuy|`0Purchase``|NOFLAGS|0|0|\nadd_button||`0Cancel``|NOFLAGS|0|0|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopmoneybuy\n\n") {
							if (pInfo(peer)->offergems <= 0) break;
							if (pInfo(peer)->gtwl >= pInfo(peer)->offergems) {
								pInfo(peer)->gems += (pInfo(peer)->offergems * 2000);
								{
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("You got `0" + setGems(pInfo(peer)->offergems * 2000) + "`` Gems!");
									p.CreatePacket(peer);
								}
								packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
								gamepacket_t p;
								p.Insert("OnSetBux");
								p.Insert(pInfo(peer)->gems);
								p.Insert(0);
								p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
								if (pInfo(peer)->supp >= 2) {
									p.Insert((float)33796, (float)1, (float)0);
								}
								p.CreatePacket(peer);
								pInfo(peer)->gtwl -= pInfo(peer)->offergems;
							}
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|playerportal\nbuttonClicked|socialportal\n\n" || cch == "action|dialog_return\ndialog_name|\nbuttonClicked|socialportal\n\n" || cch.find("action|friends") != string::npos) {
							send_social(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|socialportal\nbuttonClicked|tradehistory\n\n") {
							string trade_history = "";
							for (int i = 0; i < pInfo(peer)->trade_history.size(); i++) trade_history += "\nadd_spacer|small|\nadd_smalltext|" + pInfo(peer)->trade_history[i] + "|left|";
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|small|" + pInfo(peer)->tankIDName + "'s Trade History|left|242|" + (pInfo(peer)->trade_history.size() == 0 ? "\nadd_spacer|small|\nadd_smalltext|Nothing to show yet.|left|" : trade_history) + "\nadd_spacer|small|\nadd_button|socialportal|Back|noflags|0|0|\nadd_button||Close|noflags|0|0|\nadd_quick_exit|\nend_dialog|playerportal|||");
							p.CreatePacket(peer);
						}
						else if (/*cch == "action|claimprogressbar\n" ||*/ cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopgrowtoken\n\n" || cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopxp\n\n" || cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|toplist\n\n" || cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|yesterdaylist\n\n" || cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|overalllist\n\n" || cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopmoney\n\n" || cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shoprank\n\n" || cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopitems\n\n" || cch == "action|dialog_return\ndialog_name|socialportal\nbuttonClicked|onlinepointhub\n\n" || cch == "action|dialog_return\ndialog_name|gazette\nbuttonClicked|onlinepointhub\n\n") {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shoprank\n\n") p.Insert(a + "set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase roles``|left|278|\n\nadd_spacer|small|\n\nadd_textbox|Choose which role do you want to purchase?|left|\nadd_smalltext|Note: We are only selling 5 roles for real growtopia payment.|left|\ntext_scaling_string|100,000,000,000OPC|\nadd_spacer|small|\nadd_button_with_icon|" + (pInfo(peer)->all_in ? "" : "shopcrank_allin") + "|`pAll-In Pass`` `0(``" + (pInfo(peer)->all_in ? "`2OWNED" : "`94500WL``") + "`0)``|noflags|11816|\nadd_button_with_icon|" + (pInfo(peer)->mod ? "" : "shopcrank_mod") + "|`bModerator`` `0(``" + (pInfo(peer)->mod ? "`2OWNED" : "`94500WL``") + "`0)``|noflags|278|\nadd_button_with_icon|" + (pInfo(peer)->gp ? "" : "shopcrank_growpass") + "|`9Grow Pass`` `0(``" + (pInfo(peer)->gp ? "`2OWNED``" : "`91500WL``") + "`0)``|noflags|11304|\nadd_button_with_icon|" + (pInfo(peer)->vip ? "" : "shopcrank_vip") + "|`eVIP`` `0(``" + (pInfo(peer)->vip ? "`2OWNED``" : "`9600WL``") + "`0)``|noflags|276|\nadd_button_with_icon|" + (pInfo(peer)->glo ? "" : "shopcrank_glory") + "|`9Road to GLory`` `0(``" + (pInfo(peer)->glo ? "`2OWNED``" : "`9250WL``") + "`0)``|noflags|9436|"/*"\nadd_button_with_icon|shoprankgrowpass1|`rGrowPass LVL1`` `0(```9200Wls```0)``|noflags|9222|\nadd_button_with_icon|shoprankgrowpass2|`rGrowPass LVL2`` `0(```9400Wls```0)``|noflags|9222|\nadd_button_with_icon|shoprankgrowpass3|`rGrowPass LVL3`` `0(```9500Wls```0)``|noflags|9222|"*/"||\nadd_button_with_icon||END_LIST|noflags|0||\nadd_button|shop|`0Back``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopitems\n\n") p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase items``|left|9474|\n\nadd_spacer|small|\n\nadd_textbox|Choose which item do you want to purchase?|left|\nadd_smalltext|Note: We are only selling those items for premium wl, if you want any item to be added contact our support.|left|\nadd_spacer|small|\ntext_scaling_string|50,000OPC|" + shop_list + "||\nadd_button_with_icon||END_LIST|noflags|0||\nadd_button|shop|`0Back``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							if (cch == "action|dialog_return\ndialog_name|socialportal\nbuttonClicked|onlinepointhub\n\n" || cch == "action|dialog_return\ndialog_name|gazette\nbuttonClicked|onlinepointhub\n\n") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Online Point Hub``|left|9476|\nadd_spacer|small|\nadd_textbox|Welcome to `pOnline Point Currency HUB``! Do you have any OPC? You can buy items from me with them.|left|\nadd_smalltext|`2You can earn 1 OPC every 10 minutes just by playing the game.``|left|\nadd_spacer|small|\nadd_textbox|You have `p" + setGems(pInfo(peer)->opc) + " Online Point Currency``.|left|\ntext_scaling_string|50,000OPC|" + opc_list + "||\nadd_button_with_icon||END_LIST|noflags|0||\nadd_button|socialportal|Back|noflags|0|0|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopmoney\n\n")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase gems``|left|9436|\n\nadd_spacer|small|\nadd_textbox|You have `2" + setGems(pInfo(peer)->gtwl) + "`` Premium World Locks, how much you want to spend wls for gems? (Enter wl amount):|\nadd_textbox|Rate: `22000``/`2WL``|\nadd_smalltext|Note: You can spend your gems for awesome items or cool packs.|left|\nadd_text_input|gemspurchase|WL||30|\nend_dialog|shopgemsconfirm|Cancel|`0Check price``|\n");
							if (cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|toplist\n\n")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`8Top Worlds Of Today``|left|394|\nadd_spacer|" + top_list + "\nadd_button|wotwlistback|`oBack`|NOFLAGS|0|0|\nend_dialog|top|Close||\n");
							if (cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|yesterdaylist\n\n")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`5Top Worlds Of Yesterday``|left|394|\nadd_spacer|" + top_yesterday_list + "\nadd_button|wotwlistback|`oBack`|NOFLAGS|0|0|\nend_dialog|top|Close||\n");
							if (cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|overalllist\n\n")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Top Worlds Of All Time``|left|394|\nadd_spacer|" + top_overall_list + "\nadd_button|wotwlistback|`oBack`|NOFLAGS|0|0|\nend_dialog|top|Close||\n");
							if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopxp\n\n")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase XP``|left|1488|\nadd_spacer|small|\n\nadd_textbox|Choose which potion you want to purchase:|left|\nadd_smalltext|Note: You can purchase XP here, for example '100' potion will give you 100xp.|left|\ntext_scaling_string|5,000OPC|\nadd_spacer|small|\nadd_button_with_icon|shopxp_100|`01 WL``|noflags|1488|100|\nadd_button_with_icon|shopxp_500|`05 WL``|noflags|1488|500|\nadd_button_with_icon|shopxp_1000|`010 WL``|noflags|1488|1000|\nadd_button_with_icon|shopxp_2500|`025 WL``|noflags|1488|2500|\nadd_button_with_icon|shopxp_5000|`050 WL``|noflags|1488|5000|\nadd_button_with_icon|shopxp_10000|`0100 WL``|noflags|1488|10000|\nadd_button_with_icon||END_LIST|noflags|0||\n\nadd_spacer|\nadd_button|shop|`0Back``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopgrowtoken\n\n")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Growtoken``|left|1486|\nadd_spacer|small|\n\nadd_textbox|Choose how many tokens do you want to purchase:|left|\nadd_smalltext|Note: You can spend tokens for super rare items.|left|\nadd_spacer|small|\ntext_scaling_string|5,000OPC|\nadd_button_with_icon|shopgtoken_1|`0100 WL``|noflags|1486|1|\nadd_button_with_icon|shopgtoken_5|`0500 WL``|noflags|1486|5|\nadd_button_with_icon|shopgtoken_10|`01000 WL``|noflags|1486|10|\nadd_button_with_icon|shopgtoken_25|`02500 WL``|noflags|1486|25|\nadd_button_with_icon|shopgtoken_50|`05000 WL``|noflags|1486|50|\nadd_button_with_icon|shopgtoken_100|`010,000 WL``|noflags|1486|100|\nadd_button_with_icon||END_LIST|noflags|0||\n\nadd_spacer|\nadd_button|shop|`0Back``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							//if (cch == "action|claimprogressbar\n")p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wAbout Valentine's Event``|left|384|\nadd_spacer|small|\nadd_textbox|During Valentine's Week you will gain points for opening Golden Booty Chests. Claim enough points to earn bonus rewards.|left|\nadd_spacer|small|\nadd_textbox|Current Progress: " + to_string(pInfo(peer)->booty_broken) + "/100|left|\nadd_spacer|small|\nadd_textbox|Reward:|left|\nadd_label_with_icon|small|Super Golden Booty Chest|left|9350|\nadd_smalltext|             - 4x chance of getting a Golden Heart Crystal when opening!|left|\nend_dialog|valentines_quest||OK|\n");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|shopcrank_") != string::npos) {
							string role = cch.substr(58, cch.length() - 60).c_str();
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (role == "mod") p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Moderator``|left|18|"/*"\nadd_image_button||interface/large/gtps_mod_banner.rttex|bannerlayout|flag_frames:1,1,0,0|flag_surfsize:985,256|"*/"\nadd_spacer|small|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `34500`` `9World Locks``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `4Get extra items only for Mods from giveaways!``|left|\nadd_textbox|`6Rules:``|left|\nadd_smalltext|`e1.`` `rDo not abuse your role.``|left|\nadd_smalltext|`e2.`` `rIf you are going to ban people, make sure to have screenshots/video for proof.``|left|\nadd_smalltext|`e3.`` `rSharing account will result in account loss.``|left|\nadd_smalltext|`e4.`` `rTrying to sell your account will result in ip-ban!``|left|\nadd_spacer|small|\n\nadd_textbox|`6Commands:``|left|\nadd_smalltext|`eAll commands are displayed in /help ``|left|\nadd_spacer|"+ a + (credit_payment ? "\nadd_url_button|comment|Purchase with Credit Card|noflags|https://privategt.com/purchase/|Open premium wls shop?|0|0|" : "") + "\nadd_button|shopbuyrank_mod|`0Purchase for `94500 WL``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							else if (role == "vip") p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase VIP``|left|18|\nadd_image_button||interface/large/gtps_vip_banner.rttex|bannerlayout|flag_frames:1,1,0,0|flag_surfsize:985,256|\nadd_spacer|small|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `3600`` `9World Locks``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `4Get extra items only for VIPS from giveaways!``|left|\nadd_textbox|`6Rules:``|left|\nadd_smalltext|`e1.`` `rDo not spam with VIP SB.``|left|\nadd_smalltext|`e2.`` `rInpersonating someone with name changing will result in ban!``|left|\nadd_smalltext|`e3.`` `rSharing account will result in account loss.``|left|\nadd_smalltext|`e4.`` `rTrying to sell your account will result in ip-ban!``|left|\nadd_spacer|small|\n\nadd_textbox|`6Commands:``|left|\nadd_smalltext|`eAll commands are displayed in /vhelp (vip help)``|left|\nadd_spacer|" + a + (credit_payment ? "\nadd_url_button|comment|Purchase with Credit Card|noflags|https://privategt.com/purchase/|Open premium wls shop?|0|0|" : "") + "\nadd_button|shopbuyrank_vip|`0Purchase for `9600 WL``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							else if (role == "glory")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Road to Glory``|left|18|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `3250`` `9World Locks``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `7RECEIVE INSTANTLY 100,000 GEMS - LEVEL UP & EARN UP TO 1,600,000 GEMS (save up 600+wls)``|left|\nadd_textbox|`6Rules:``|left|\nadd_smalltext|`e1.`` `rDo not abuse your role.``|left|\nadd_smalltext|`e2.`` `rIf you are going to ban people, make sure to have screenshots/video for proof.``|left|\nadd_smalltext|`e3.`` `rSharing account will result in account loss.``|left|\nadd_smalltext|`e4.`` `rTrying to sell your account will result in ip-ban!``|left|\nadd_spacer|small|\n\nadd_textbox|`6Commands:``|left|\nadd_smalltext|`eAll commands are displayed in /help ``|left|\nadd_spacer|" + a + (credit_payment ? "\nadd_url_button|comment|Purchase with Credit Card|noflags|https://privategt.com/purchase/|Open premium wls shop?|0|0|" : "") + "\nadd_button|shopbuyrank_glory|`0Purchase for `9250 WL``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							else if (role == "growpass")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Grow Pass``|left|18|\nadd_image_button||interface/large/gtps_growpass_banner.rttex|bannerlayout|flag_frames:1,1,0,0|flag_surfsize:985,256|\nadd_spacer|small|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `31500`` `9World Locks``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `7Receive daily items everyday, get 2x opc points, receive newest coolest growotpia items, use /buy <item> command``, `4Get extra items only for Grow-Pass from giveaways!``|left|\n\nadd_textbox|`6Rules:``|left|\nadd_smalltext|`e1.`` `rDo not abuse your role.``|left|\nadd_smalltext|`e2.`` `rIf you are going to ban people, make sure to have screenshots/video for proof.``|left|\nadd_smalltext|`e3.`` `rSharing account will result in account loss.``|left|\nadd_smalltext|`e4.`` `rTrying to sell your account will result in ip-ban!``|left|\nadd_spacer|small|\n\nadd_textbox|`6Commands:``|left|\nadd_smalltext|`eAll commands are displayed in /help ``|left|\nadd_spacer|" + a + (credit_payment ? "\nadd_url_button|comment|Purchase with Credit Card|noflags|https://privategt.com/purchase/|Open premium wls shop?|0|0|" : "") + "\nadd_button|shopbuyrank_growpass|`0Purchase for `91500 WL``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							else if (role == "allin")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase All-In Pass``|left|18|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `34500`` `9World Locks``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `7Permament deposit 2x bonus``, `4Get extra items only for All-In from giveaways!``|left|\n\nadd_textbox|`6Rules:``|left|\nadd_smalltext|`e1.`` `rDo not abuse your role.``|left|\nadd_smalltext|`e2.`` `rIf you are going to ban people, make sure to have screenshots/video for proof.``|left|\nadd_smalltext|`e3.`` `rSharing account will result in account loss.``|left|\nadd_smalltext|`e4.`` `rTrying to sell your account will result in ip-ban!``|left|\nadd_spacer|small|\n\nadd_textbox|`6Commands:``|left|\nadd_smalltext|`eAll commands are displayed in /help ``|left|\nadd_textbox|Purchasing this will give you an `5awesome prizes``:|left|\nadd_label_with_icon|small|100 Cashback Coupon|left|10394|\nadd_label_with_icon|small|5 Experience Potion|left|1488|\nadd_label_with_icon|small|5 Curse Wand|left|278|\nadd_label_with_icon|small|5 Fire Wand|left|276|\nadd_label_with_icon|small|5 Freeze Wand|left|274|\nadd_spacer|" + a + (credit_payment ? "\nadd_url_button|comment|Purchase with Credit Card|noflags|https://privategt.com/purchase/|Open premium wls shop?|0|0|" : "") + "\nadd_button|shopbuyrank_allin|`0Purchase for `94500 WL``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|shopbuyrank_") != string::npos) {
							if (pInfo(peer)->world == "") break;
							string role = cch.substr(60, cch.length() - 62).c_str();
							int price = 0;
							if (role == "growpass") price = 1500;
							else if (role == "vip") price = 600;
							else if (role == "glory") price = 250;
							else if (role == "allin" || role == "mod")  price = 4500;
							if (price != 0 && pInfo(peer)->gtwl >= price) {
								pInfo(peer)->gtwl -= price;
								packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								if (role == "mod") {
									pInfo(peer)->mod = 1, p.Insert("`o>> You purchased moderator! Type /help``");
									pInfo(peer)->name_color = (pInfo(peer)->tankIDName == "chris" ? "`5@Dr. " : (pInfo(peer)->superdev == 1) ? "`6@" : (pInfo(peer)->dev == 1) ? "`9@" : (pInfo(peer)->tmod == 1) ? "`#@" : "`0");
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										gamepacket_t p;
										p.Insert("OnConsoleMessage"), p.Insert("`#**`` `$The Ancient Ones`` have `#made`` `w" + pInfo(peer)->tankIDName + "``->`#@" + pInfo(peer)->tankIDName + "`` a `bmoderator`` `#**`` (`2/deposit`` to purchase role!)"), p.CreatePacket(currentPeer);
									}
								}
								else if (role == "vip") {
									pInfo(peer)->vip = 1;
									p.Insert("`o>> You purchased VIP! Type /vhelp``");
								}
								else if (role == "glory") {
									pInfo(peer)->glo = 1;
									pInfo(peer)->gems += 100000;
									p.Insert("`o>> You purchased Road to Glory! Wrench yourself and press on Road to Glory button!``");
									{
										gamepacket_t p;
										p.Insert("OnSetBux"), p.Insert(pInfo(peer)->gems), p.Insert(0), p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
										if (pInfo(peer)->supp >= 2) p.Insert((float)33796, (float)1, (float)0);
										p.CreatePacket(peer);
									}
								}
								else if (role == "growpass") {
									pInfo(peer)->gp = 1, pInfo(peer)->gd = 0;
									p.Insert("`o>> You purchased Grow Pass! Wrench yourself to check the prizes``");
								}
								else if (role == "allin") {
									pInfo(peer)->all_in = 1;
									p.Insert("`o>> You purchased All-In Pass! Thank you.``");
									vector<int> list{ 10394 , 274 , 276 , 278 , 1488 };
									string name_ = pInfo(peer)->world;
									vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
									if (p != worlds.end()) {
										World* world_ = &worlds[p - worlds.begin()];
										for (int i = 0; i < list.size(); i++) {
											int c_ = (list[i] == 10394 ? 100 : 5);
											if (modify_inventory(peer, list[i], c_) == 0) {

											}
											else {
												WorldDrop drop_block_{};
												drop_block_.id = list[i], drop_block_.count = (list[i] == 10394 ? 100 : 5), drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = pInfo(peer)->x + rand() % 17, drop_block_.y = pInfo(peer)->y + rand() % 17;
												dropas_(world_, drop_block_);
											}
										}
									}
								}
								p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|shop_price_") != string::npos) {
							int item = atoi(cch.substr(59, cch.length() - 59).c_str());
							if (item <= 0 || item >= items.size() || items[item].pwl == 0) continue;
							string special = "`eThis item does not have any special effects.``";
							if (item == 6312 || item == 11118 || item == 9550) special = "`eGet `2Double Gems``";
							if (item == 9496) special = "`eGet `2Triple Gems``";
							if (item == 11118) special += ", `220% higher chance of getting Magic Ores``";
							if (item == 9548) special += ", `240% higher chance of getting Magic Ores``";
							if (item == 9774) special += ", 100% higher chance of getting Magic Ores``";
							if (item == 9546 || item == 9542) special = "`eGet `2x4 Gems``";
							if (item == 9548) special = "`eGet `2x40 Gems``";
							if (item == 9604) special += "`eGet `2x100 Gems``";
							if (item == 9774) special += "`eGet `2x1000 Gems``";
							if (item == 9770) special = "`e1x hit all blocks & 3 hit far``";
							if (item == 9774) special += "`e1x hit all blocks & 3 hit far``";
							if (item == 9772) special = "`e1x hit all blocks & 3 hit far + double opc``";
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase " + items[item].name + "``|left|" + to_string(items[item].id) + "|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `3" + setGems(items[item].pwl) + "`` `9World Locks``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\n\nadd_textbox|`6Special Effects:``|left|\nadd_smalltext|" + special + "|left|\nadd_text_input|howmuch||1|3|\nadd_spacer|\nadd_button|" + to_string(item) + "|`0Purchase for `9" + setGems(items[item].pwl) + " Wls``|noflags|0|0||small|\nend_dialog|shop_item||Cancel|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|lock_price_") != string::npos) {
							int item = atoi(cch.substr(59, cch.length() - 59).c_str());
							if (item <= 0 || item >= items.size() || items[item].gtwl == 0) continue;
							pInfo(peer)->lockeitem = item;
							int wl = 0, dl = 0;
							modify_inventory(peer, 242, wl);
							modify_inventory(peer, 1796, dl);
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`9Buy " + items[item].name + "?``|left|" + to_string(items[item].id) + "|\nadd_smalltext|`4" + items[item].description + "``|left|\nadd_smalltext|`1Price: " + (items[item].gtwl > 200 ? to_string(items[item].gtwl / 100) : to_string(items[item].gtwl)) + " " + (items[item].gtwl > 200 ? "Diamond Lock" : "World Locks") + "``|left|\nadd_spacer|\nadd_textbox|How many " + items[item].name + " do you want to buy, for " + (items[item].gtwl > 200 ? to_string(items[item].gtwl / 100) : to_string(items[item].gtwl)) + " " + (items[item].gtwl > 200 ? "Diamond Lock" : "World Locks") + " each?|left|\nadd_text_input|howmuch||1|5|\nadd_smalltext|" + (wl + dl != 0 ? "You have " + (wl != 0 ? to_string(wl) + " World Locks" : "") + "" + (dl != 0 ? ", " + to_string(dl) + " Diamond Lock." : ".") + "" : "") + "|left|\nadd_button|lock_item_|`9Purchase``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|locke|No thanks|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|locm_price_") != string::npos) {
							int item = atoi(cch.substr(59, cch.length() - 59).c_str());
							if (item <= 0 || item >= items.size() || items[item].u_gtwl == 0) continue;
							pInfo(peer)->lockeitem = item;
							int wl = 0, dl = 0;
							modify_inventory(peer, 242, wl);
							modify_inventory(peer, 1796, dl);
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`9Buy " + items[item].name + "?``|left|" + to_string(items[item].id) + "|\nadd_smalltext|`4" + items[item].description + "``|left|\nadd_smalltext|`1Price: " + setGems(items[item].u_gtwl) + " World Lock``|left|\nadd_spacer|\nadd_textbox|How many " + items[item].name + " do you want to buy, for " + (items[item].u_gtwl > 200 ? to_string(items[item].u_gtwl / 100) : to_string(items[item].u_gtwl)) + " " + (items[item].u_gtwl > 200 ? "Diamond Lock" : "World Locks") + " each?|left|\nadd_text_input|howmuch||1|5|\nadd_smalltext|" + (wl + dl != 0 ? "You have " + (wl != 0 ? to_string(wl) + " World Locks" : "") + "" + (dl != 0 ? ", " + to_string(dl) + " Diamond Lock." : ".") + "" : "") + "|left|\nadd_button|lock_item_|`9Purchase``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|lockm|No thanks|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|locke") != string::npos) {
							if (pInfo(peer)->world == "LOCKETOWN") {
								if (thedaytoday == 0 or thedaytoday == 1 or thedaytoday == 5 or thedaytoday == 6) {
									int wl = 0, dl = 0;
									modify_inventory(peer, 242, wl);
									modify_inventory(peer, 1796, dl);
									gamepacket_t p;
									p.Insert("OnDialogRequest");
									p.Insert(a + "set_default_color|`o\n\nadd_label_with_icon|big|`9GTPS Salesman``|left|9828|\nadd_spacer|small|\nadd_smalltext|Ho there, friend! Locke's my name, and locks are my game. I Love 'em all, Diamond, Huge.. even Small! If you can part with some locks, I'll give you something special in return. Whaddya say?|left|\nadd_spacer|small|\nadd_smalltext|" + (wl + dl != 0 ? "`9(Hmm, smells like you care carrying " + (wl != 0 ? to_string(wl) + " World Locks" : "") + "" + (dl != 0 ? ", and and " + to_string(dl) + " Diamond Lock" : "") + ")``" : "`9(Hmm, smells like you don't care any world locks)``") + "|left|\nadd_spacer|small|" + shop_list2 + "|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
									p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|lockm") != string::npos) {
							int wl = 0, dl = 0, bgl = 0;
							modify_inventory(peer, 242, wl);
							modify_inventory(peer, 1796, dl);
							modify_inventory(peer, 7188, bgl);
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert(a + "set_default_color|`o\n\nadd_label_with_icon|big|`9Sales-Man Untradeable``|left|9528|\nadd_spacer|small|\nadd_smalltext|Damn.. I hate LOCKE!! This guy takes all my diamond locks, i'm hungry so feed me more. The Untradeable Sales-Man|left|\nadd_spacer|small|\nadd_smalltext|" + (wl + dl != 0 ? "`9(Hmm, smells like you care carrying " + (wl != 0 ? to_string(wl) + " World Locks" : "") + "" + (dl != 0 ? ", and and " + to_string(dl) + " Diamond Lock" : "") + ")``" : "`9(Hmm, smells like you don't care any world locks)``") + "|left|" + (bgl == 0 ? "" : "\nadd_smalltext|`9(You are also holding additional " + to_string(bgl) + " Blue Gem Locks)``|left|") + "\nadd_spacer|small|" + shop_list2_2 + "|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|lock_item_\n\nhowmuch|") != string::npos) {
							if (pInfo(peer)->world == "LOCKETOWN") {
								int count = atoi(cch.substr(68, cch.length() - 68).c_str()), count2 = atoi(cch.substr(68, cch.length() - 68).c_str());
								if (count <= 0 || count > 200) continue;
								int item = pInfo(peer)->lockeitem;
								if (item <= 0 || item >= items.size()) continue;
								if (items[item].gtwl == 0 and items[item].u_gtwl == 0) continue;
								int allwl = 0, wl = 0, dl = 0, price = (items[item].gtwl == 0 ? items[item].u_gtwl : items[item].gtwl), priced = 0, bgl = 0;
								price *= count;
								priced = price;
								modify_inventory(peer, 242, wl);
								modify_inventory(peer, 1796, dl);
								modify_inventory(peer, 7188, bgl);
								allwl = wl + (dl * 100);
								int allbgl = bgl * 10000;
								if (allwl >= price || allbgl >= price && price > 20000) {
									int c_ = count;
									if (modify_inventory(peer, item, c_) == 0) {
										if (price <= 20000) {
											if (wl >= price) modify_inventory(peer, 242, price *= -1);
											else {
												modify_inventory(peer, 242, wl *= -1);
												modify_inventory(peer, 1796, dl *= -1);
												int givedl = (allwl - price) / 100;
												int givewl = (allwl - price) - (givedl * 100);
												modify_inventory(peer, 242, givewl);
												modify_inventory(peer, 1796, givedl);
											}
										}
										else {
											int removebgl = (price / 10000) * -1;
											modify_inventory(peer, 7188, removebgl);
										}
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world == pInfo(currentPeer)->world) {
												gamepacket_t p;
												p.Insert("OnConsoleMessage");
												p.Insert("`9[" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->tankIDName) + " bought " + to_string(count2) + " " + items[item].name + " for " + to_string(priced) + " World Lock]");
												p.CreatePacket(currentPeer);
												packet_(currentPeer, "action|play_sfx\nfile|audio/cash_register.wav\ndelayMS|0");
											}
										}
									}
									else {
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("No inventory space.");
										p.CreatePacket(peer);
									}
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("`9You don't have enough wls!``");
									p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|shop_item") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() == 4) {
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase items``|left|9474|\n\nadd_spacer|small|\n\nadd_textbox|Choose which item do you want to purchase?|left|\nadd_smalltext|Note: We are only selling those items for premium wl, if you want any item to be added contact our support.|left|\nadd_spacer|small|\ntext_scaling_string|50,000OPC|" + shop_list + "||\nadd_button_with_icon||END_LIST|noflags|0||\nadd_button|shop|`0Back``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
								p.CreatePacket(peer);
							}
							else if (t_.size() == 5) {
								int item = atoi(explode("\n", t_.at(3)).at(0).c_str()), how_much = atoi(explode("\n", t_.at(4)).at(0).c_str());
								if (item <= 0 || item >= items.size() || items[item].pwl == 0 || how_much <= 0 || how_much > 200) continue;
								if ( pInfo(peer)->gtwl >= items[item].pwl * how_much) {
									int c_ = 1;
									if (how_much > 1) c_ = how_much;
									if (modify_inventory(peer, item, c_) == 0) {
										pInfo(peer)->gtwl -= items[item].pwl * how_much;
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("`o>> You purchased " + to_string(how_much) + " `2" + items[item].name + "``!");
										p.CreatePacket(peer);
										{
											gamepacket_t p;
											p.Insert("OnAddNotification"), p.Insert("interface/cash_icon_overlay.rttex"), p.Insert("You received " + to_string(how_much) + " `5" + items[item].name + "``!"), p.Insert("audio/piano_nice.wav.wav"), p.Insert(0), p.CreatePacket(peer);
										}
									}
									else {
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("No inventory space.");
										p.CreatePacket(peer);
									}
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("You don't have enough world locks.");
									p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|opop_price_") != string::npos) {
							int item = atoi(cch.substr(59, cch.length() - 59).c_str());
							if (item <= 0 || item >= items.size() || items[item].oprc == 0) continue;
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase " + items[item].name + "``|left|" + to_string(items[item].id) + "|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_spacer|small|\nadd_smalltext|Name: `3" + items[item].ori_name + "``|left|\nadd_smalltext|Price: `3" + setGems(items[item].oprc) + "`` `0OPC``|left|\nadd_smalltext|Rarity: `3" + setGems(items[item].rarity) + "``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_spacer|small|\nadd_smalltext|Preview:|left|\nadd_button_with_icon||||" + to_string(item) + "||\nadd_button_with_icon||END_LIST|noflags|0||\nadd_textbox|`6Other information:``|left|\nadd_smalltext|" + items[item].description + "|left|\nadd_spacer|\nadd_button|opop_item_" + to_string(item) + "|`0Purchase `9" + setGems(items[item].oprc) + " OPC``|noflags|0|0|\nadd_spacer|small|\nadd_button|onlinepointhub|`wBack``|noflags|0|0|\nend_dialog|gazette||OK|\nadd_quick_exit|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|gazette\nbuttonClicked|opop_item_") != string::npos) {
							int item = atoi(cch.substr(65, cch.length() - 65).c_str());
							if (item <= 0 || item >= items.size() || items[item].oprc == 0) continue;
							if (pInfo(peer)->opc >= items[item].oprc) {
								int c_ = 1;
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								if (modify_inventory(peer, item, c_) == 0) {
									pInfo(peer)->opc -= items[item].oprc;
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									p.Insert("`o>> You purchased `2" + items[item].name + "``!");
									PlayerMoving data_{};
									data_.packetType = 19, data_.punchX = item, data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16;
									int32_t to_netid = pInfo(peer)->netID;
									BYTE* raw = packPlayerMoving(&data_);
									raw[3] = 5;
									memcpy(raw + 8, &to_netid, 4);
									send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									data_.packetType = 17, data_.netID = 48, data_.YSpeed = 48, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
									raw = packPlayerMoving(&data_);
									send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									delete[]raw;
								}
								else p.Insert("No inventory space.");
								p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|shopxp_") != string::npos) {
							int xp = atoi(cch.substr(55, cch.length() - 55).c_str());
							if (xp > 0 && pInfo(peer)->gtwl >= xp / 100) {
								pInfo(peer)->gtwl -= xp / 100;
								packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
								add_peer_xp(peer, xp);
								gamepacket_t p;
								p.Insert("OnConsoleMessage"), p.Insert("`o>> You purchased " + setGems(xp) + " XP!"), p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|shopgtoken_") != string::npos) {
							int gtokens = atoi(cch.substr(59, cch.length() - 59).c_str());
							if (gtokens > 0 && pInfo(peer)->gtwl >= gtokens * 100) {
								int give = gtokens;
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								if (modify_inventory(peer, 1486, give) == -1) p.Insert("No inventory space.");
								else {
									pInfo(peer)->gtwl -= gtokens * 100;
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									p.Insert("`o>> You purchased " + to_string(gtokens) + " Growtoken!");
								}
								p.CreatePacket(peer);
							}
							break;
						}
						/*
						else if (cch.find("action|showblarneyprogress") != string::npos) {
							gamepacket_t p(550);
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBlarney Bonanza!``|left|528|\nadd_spacer|small|\nadd_textbox|Welcome to the Blarney Bonanza|left|\nadd_spacer|small|\nadd_textbox|As you, as a community, complete Blarneys and kiss the most magical stone, items will unlock for you to pick up in the store.|left|\nadd_spacer|small|\nadd_textbox|There are 4 items to unlock throughout the event.|left|\nadd_spacer|small|\nadd_textbox|Items will only remain unlocked for a short amount of time, so make sure you check back often! These items can be unlocked multiple times throughout the week.|left|\nadd_spacer|small|\nend_dialog|blarney_dialog||OK|\n");
							p.CreatePacket(peer);
							break;
						}*/
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shop\n\n") SendCmd(peer, "/shop", true);
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|deposit\n\n") SendCmd(peer, "/deposit", true); // news
						else if (cch == "action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|claim_reward\n\n") {
							if (pInfo(peer)->gp == 1) {
								if (today_day != pInfo(peer)->gd) {
									vector<int> list2{ 1796, 242, 5196, 10836, 10838, 12642 , 12644 };
									int receive = 1, item = list2[rand() % list2.size()], received = 1;
									if (item == 10386 || item == 722) receive = 20, received = 20;
									if (modify_inventory(peer, item, receive) == 0) {
										pInfo(peer)->gd = today_day;
										{
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("`9 >> You claimed your Grow Pass reward:");
											p.CreatePacket(peer);
										}
										{
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("Given `0" + to_string(received) + " " + items[item].name + "``.");
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17;
										data_.netID = 48;
										data_.YSpeed = 48;
										data_.x = pInfo(peer)->x + 16;
										data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(currentPeer)->world == pInfo(peer)->world) {
												send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											}
										}
										delete[] raw;
									}
									else {
										{
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("Clean your inventory and try again!");
											p.CreatePacket(peer);
										}
									}
								}
								else {
									{
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("You already claimed your reward today!");
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nitemid|") != string::npos) {
							int item = atoi(cch.substr(57, cch.length() - 57).c_str());
							if (item <= 0 || item >= items.size()) break;
							if (pInfo(peer)->lastwrenchb != 4516 and items[item].untradeable == 1 or item == 1424) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You can't store Untradeable items there!"), p.CreatePacket(peer);
							}
							else if (pInfo(peer)->lastwrenchb == 4516 and items[item].untradeable == 0 or item == 18 || item == 32 || item == 6336 || item == 1424) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You can't store Tradeable items there!"), p.CreatePacket(peer);
							}
							else {
								int got = 0, receive = 0;
								modify_inventory(peer, item, got);
								if (got == 0) break;
								pInfo(peer)->lastchoosenitem = item;
								gamepacket_t p;
								p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`w" + items[pInfo(peer)->lastwrenchb].name + "``|left|" + to_string(pInfo(peer)->lastwrenchb) + "|\nadd_textbox|You have " + to_string(got) + " " + items[item].name + ". How many to store?|left|\nadd_text_input|itemcount||" + to_string(got) + "|3|\nadd_spacer|small|\nadd_button|do_add|Store Items|noflags|0|0|\nend_dialog|storageboxxtreme|Cancel||\n"), p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nbuttonClicked|itm") != string::npos) {
							int itemnr = atoi(cch.substr(67, cch.length() - 67).c_str()), itemcount = 0;
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (block_access(peer, world_, block_, false, true)) {
									for (int i_ = 0; i_ < world_->sbox1.size(); i_++) {
										if (world_->sbox1[i_].x == pInfo(peer)->lastwrenchx and world_->sbox1[i_].y == pInfo(peer)->lastwrenchy) {
											itemcount++;
											if (itemnr == itemcount) {
												pInfo(peer)->lastchoosennr = itemnr;
												gamepacket_t p;
												p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`w" + items[pInfo(peer)->lastwrenchb].name + "``|left|" + to_string(pInfo(peer)->lastwrenchb) + "|\nadd_textbox|You have `w" + to_string(world_->sbox1[i_].count) + " " + items[world_->sbox1[i_].id].name + "`` stored.|left|\nadd_textbox|Withdraw how many?|left|\nadd_text_input|itemcount||" + to_string(world_->sbox1[i_].count) + "|3|\nadd_spacer|small|\nadd_button|do_take|Remove Items|noflags|0|0|\nadd_button|cancel|Back|noflags|0|0|\nend_dialog|storageboxxtreme|Exit||\n"), p.CreatePacket(peer);
											}
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|donation_box_edit\nitemid|") != string::npos) {
							int item = atoi(cch.substr(58, cch.length() - 58).c_str()), got = 0;
							modify_inventory(peer, item, got);
							if (got == 0) break;
							if (items[item].untradeable == 1 || item == 1424 || items[item].blockType == BlockTypes::FISH) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`7[```4You can't place that in the box, you need it!`7]``"), p.CreatePacket(peer);
							}
							else if (items[item].rarity == 1) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`7[```4This box only accepts items rarity 2+ or greater`7]``"), p.CreatePacket(peer);
							}
							else {
								pInfo(peer)->lastchoosenitem = item;
								gamepacket_t p;
								p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|" + items[item].name + "``|left|" + to_string(item) + "|\nadd_textbox|How many to put in the box as a gift? (Note: You will `4LOSE`` the items you give!)|left|\nadd_text_input|count|Count:|" + to_string(got) + "|5|\nadd_text_input|sign_text|Optional Note:||128|\nadd_spacer|small|\nadd_button|give|`4Give the item(s)``|noflags|0|0|\nadd_spacer|small|\nadd_button|cancel|`wCancel``|noflags|0|0|\nend_dialog|give_item|||\n");
								p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|donation_box_edit\nbuttonClicked|takeall\n") != string::npos) {
							bool took = false, fullinv = false;
							gamepacket_t p3;
							p3.Insert("OnTalkBubble"), p3.Insert(pInfo(peer)->netID);
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (not block_access(peer, world_, block_)) break;
								if (!items[block_->fg].donation) break;
								for (int i_ = 0; i_ < block_->donates.size(); i_++) {
									int receive = block_->donates[i_].count;
									if (modify_inventory(peer, block_->donates[i_].item, block_->donates[i_].count) == 0) {
										took = true;
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("`7[``" + pInfo(peer)->tankIDName + " receives `5" + to_string(receive) + "`` `w" + items[block_->donates[i_].item].name + "`` from `w" + block_->donates[i_].name + "``, how nice!`7]``");
										block_->donates.erase(block_->donates.begin() + i_);
										i_--;
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											p.CreatePacket(currentPeer);
										}
									}
									else fullinv = true;
								}
								if (block_->donates.size() == 0) {
									if (block_->flags & 0x00400000) block_->flags ^= 0x00400000;
									PlayerMoving data_{};
									data_.packetType = 5, data_.punchX = pInfo(peer)->lastwrenchx, data_.punchY = pInfo(peer)->lastwrenchy, data_.characterState = 0x8;
									BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_));
									BYTE* blc = raw + 56;
									form_visual(blc, *block_, *world_, peer, false);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
										send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_), ENET_PACKET_FLAG_RELIABLE);
									}
									delete[] raw, blc;
									if (block_->locked) upd_lock(*block_, *world_, peer);
								}
							}
							if (fullinv) {
								p3.Insert("I don't have enough room in my backpack to get the item(s) from the box!");
								gamepacket_t p2;
								p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("`2(Couldn't get all of the gifts)``"), p2.CreatePacket(peer);
							}
							else if (took) p3.Insert("`2Box emptied.``");
							p3.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|surge\n") {
							if (pInfo(peer)->lastwrenchb == 4296 || pInfo(peer)->lastwrenchb == 8558) {
								setstats(peer, rand() % 31, "", items[pInfo(peer)->lastwrenchb].name);
								pInfo(peer)->lastwrenchb = 0;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|give_item\nbuttonClicked|give\n\ncount|") != string::npos) {
							int count = atoi(cch.substr(69, cch.length() - 69).c_str()), got = 0;
							string text = cch.substr(80 + to_string(count).length(), cch.length() - 80 + to_string(count).length()).c_str();
							replace_str(text, "\n", "");
							modify_inventory(peer, pInfo(peer)->lastchoosenitem, got);
							if (text.size() > 128 || got <= 0 || count <= 0 || count > items.size()) break;
							if (count > got || items[pInfo(peer)->lastchoosenitem].untradeable == 1 || pInfo(peer)->lastchoosenitem == 1424 || items[pInfo(peer)->lastchoosenitem].blockType == BlockTypes::FISH) {
								gamepacket_t p;
								p.Insert("OnTalkBubble");
								p.Insert(pInfo(peer)->netID);
								if (count > got) p.Insert("You don't have that to give!");
								else p.Insert("`7[```4You can't place that in the box, you need it!`7]``");
								p.CreatePacket(peer);
							}
							else {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
									if (!items[block_->fg].donation) break;
									Donate donate_{};
									donate_.item = pInfo(peer)->lastchoosenitem, donate_.count = count, donate_.name = pInfo(peer)->tankIDName, donate_.text = text;
									block_->donates.push_back(donate_);
									{
										PlayerMoving data_{};
										data_.packetType = 5, data_.punchX = pInfo(peer)->lastwrenchx, data_.punchY = pInfo(peer)->lastwrenchy, data_.characterState = 0x8;
										BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_));
										BYTE* blc = raw + 56;
										block_->flags = (block_->flags & 0x00400000 ? block_->flags : block_->flags | 0x00400000);
										form_visual(blc, *block_, *world_, peer, false, true);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_), ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw, blc;
										if (block_->locked) upd_lock(*block_, *world_, peer);
									}
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
										gamepacket_t p, p2;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`7[```5[```w" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->tankIDName) + "`` places `5" + to_string(count) + "`` `2" + items[pInfo(peer)->lastchoosenitem].name + "`` into the " + items[pInfo(peer)->lastwrenchb].name + "`5]```7]``");
										p.CreatePacket(currentPeer);
										p2.Insert("OnConsoleMessage");
										p2.Insert("`7[```5[```w" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->tankIDName) + "`` places `5" + to_string(count) + "`` `2" + items[pInfo(peer)->lastchoosenitem].name + "`` into the " + items[pInfo(peer)->lastwrenchb].name + "`5]```7]``");
										p2.CreatePacket(currentPeer);
									}
									send_discord_webhook("player: `" + pInfo(peer)->tankIDName + "` level: " + to_string(pInfo(peer)->level) + " places " + to_string(count) + " `" + items[pInfo(peer)->lastchoosenitem].name + "` into the " + items[pInfo(peer)->lastwrenchb].name + " in: [" + pInfo(peer)->world + "]", "992354177206456330/5TBflg9h8NLOkI0AcAihXUegQv3ELFD3TiAgk6G_DQsNi7dcmxmQ-94htOQ-hhchDN5F");
									modify_inventory(peer, pInfo(peer)->lastchoosenitem, count *= -1);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nbuttonClicked|cancel") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (world_->owner_name == pInfo(peer)->tankIDName || pInfo(peer)->dev)load_storagebox(peer, world_, block_);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nbuttonClicked|do_take\n\nitemcount|") != string::npos) {
							int itemnr = pInfo(peer)->lastchoosennr, countofremoval = atoi(cch.substr(83, cch.length() - 83).c_str()), removed = 0, itemcount = 0;
							removed = countofremoval;
							if (countofremoval <= 0) break;
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (block_access(peer, world_, block_, false, true)) {
									for (int i_ = 0; i_ < world_->sbox1.size(); i_++) {
										if (world_->sbox1[i_].x == pInfo(peer)->lastwrenchx and world_->sbox1[i_].y == pInfo(peer)->lastwrenchy) {
											itemcount++;
											if (itemnr == itemcount and countofremoval < world_->sbox1[i_].count) {
												if (world_->sbox1[i_].count <= 0) break;
												world_->sbox1[i_].count -= removed;
												if (modify_inventory(peer, world_->sbox1[i_].id, countofremoval) == 0) {
													gamepacket_t p, p2;
													p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("Removed `w" + to_string(removed) + " " + items[world_->sbox1[i_].id].name + "`` in " + items[pInfo(peer)->lastwrenchb].name + "."), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
													p2.Insert("OnConsoleMessage"), p2.Insert("Removed `w" + to_string(removed) + " " + items[world_->sbox1[i_].id].name + "`` in the " + items[pInfo(peer)->lastwrenchb].name + "."), p2.CreatePacket(peer);
													PlayerMoving data_{};
													data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16, data_.packetType = 19, data_.plantingTree = 500, data_.punchX = world_->sbox1[i_].id, data_.punchY = pInfo(peer)->netID;
													int32_t to_netid = pInfo(peer)->netID;
													BYTE* raw = packPlayerMoving(&data_);
													raw[3] = 5;
													memcpy(raw + 8, &to_netid, 4);
													send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
													delete[] raw;
													i_ = world_->sbox1.size();
												}
											}
											else if (itemnr == itemcount and world_->sbox1[i_].count == countofremoval) {
												if (world_->sbox1[i_].count <= 0) break;
												if (modify_inventory(peer, world_->sbox1[i_].id, countofremoval) == 0) {
													gamepacket_t p, p2;
													p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("Removed `w" + to_string(removed) + " " + items[world_->sbox1[i_].id].name + "`` in " + items[pInfo(peer)->lastwrenchb].name + "."), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
													p2.Insert("OnConsoleMessage"), p2.Insert("Removed `w" + to_string(removed) + " " + items[world_->sbox1[i_].id].name + "`` in the " + items[pInfo(peer)->lastwrenchb].name + "."), p2.CreatePacket(peer);
													PlayerMoving data_{};
													data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16;
													data_.packetType = 19, data_.plantingTree = 500;
													data_.punchX = world_->sbox1[i_].id, data_.punchY = pInfo(peer)->netID;
													int32_t to_netid = pInfo(peer)->netID;
													BYTE* raw = packPlayerMoving(&data_);
													raw[3] = 5;
													memcpy(raw + 8, &to_netid, 4);
													send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
													delete[] raw;
													world_->sbox1.erase(world_->sbox1.begin() + i_);
													i_ = world_->sbox1.size();
												}
											}
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|reward_") != string::npos) {
							reward_show(peer, cch.substr(66, cch.length() - 68).c_str());
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|backpack_menu\nbuttonClicked|backpack_upgrade\n\n") {
							if (pInfo(peer)->gems >= (1000 * ((pInfo(peer)->b_l * pInfo(peer)->b_l) + 25)) * 2) {
								if (pInfo(peer)->b_l * 10 > 200) {
									{
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have reached max slots!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
								else {
									pInfo(peer)->gems -= (1000 * ((pInfo(peer)->b_l * pInfo(peer)->b_l) + 25)) * 2;
									pInfo(peer)->b_l++;
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									gamepacket_t p;
									p.Insert("OnSetBux");
									p.Insert(pInfo(peer)->gems);
									p.Insert(0);
									p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
									if (pInfo(peer)->supp >= 2) {
										p.Insert((float)33796, (float)1, (float)0);
									}
									p.CreatePacket(peer);
									{
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("Congratulations! You have upgraded your Backpack!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
									backpack_show(peer);
								}
							}
							else {
								gamepacket_t p;
								p.Insert("OnTalkBubble");
								p.Insert(pInfo(peer)->netID);
								p.Insert("You don't have enough gems!");
								p.Insert(0), p.Insert(0);
								p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|backpack_menu\nbuttonClicked|") != string::npos) {
							int choosen_item = atoi(cch.substr(61, cch.length() - 61).c_str());
							if (choosen_item >= pInfo(peer)->bp.size() || choosen_item > 200 || choosen_item > pInfo(peer)->b_l * 10) break;
							for (int i_ = 0; i_ < pInfo(peer)->bp.size(); i_++) {
								if (choosen_item == i_) {
									if (pInfo(peer)->bp[choosen_item].first <= 0 || pInfo(peer)->bp[choosen_item].first >= items.size()) break;
									int pickedup = pInfo(peer)->bp[choosen_item].second;
									int count = pInfo(peer)->bp[choosen_item].second;
									if (modify_inventory(peer, pInfo(peer)->bp[choosen_item].first, count) == 0) {
										{
											gamepacket_t p, p2;
											p.Insert("OnConsoleMessage"), p.Insert("You picked up " + to_string(pickedup) + " " + items[pInfo(peer)->bp[choosen_item].first].name + "."), p.CreatePacket(peer);
											p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p.Insert("You picked up " + to_string(pickedup) + " " + items[pInfo(peer)->bp[choosen_item].first].name + "."), p2.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 19, data_.punchX = pInfo(peer)->bp[choosen_item].first, data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16;
										int32_t to_netid = pInfo(peer)->netID;
										BYTE* raw = packPlayerMoving(&data_);
										raw[3] = 5;
										memcpy(raw + 8, &to_netid, 4);
										send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										delete[]raw;
										pInfo(peer)->bp.erase(pInfo(peer)->bp.begin() + i_);
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You don't have enough inventory space!"), p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|backpack_menu\nitemid|") != string::npos) {
							if (pInfo(peer)->bp.size() <= pInfo(peer)->b_l * 10) {
								int got = 0, item = atoi(cch.substr(54, cch.length() - 54).c_str());
								modify_inventory(peer, item, got);
								if (got <= 0) break;
								if (items[item].untradeable == 1 || item == 1424 || items[item].blockType == BlockTypes::FISH || item == 1796 || item == 7188 || item == 242) {
									gamepacket_t p;
									p.Insert("OnTalkBubble");
									p.Insert(pInfo(peer)->netID);
									p.Insert("You can't store this item!");
									p.Insert(0), p.Insert(0);
									p.CreatePacket(peer);
								}
								else {
									pInfo(peer)->bp.push_back(make_pair(item, got));
									modify_inventory(peer, item, got *= -1);
									PlayerMoving data_{};
									data_.packetType = 19, data_.punchX = item, data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16;
									int32_t to_netid = pInfo(peer)->netID;
									BYTE* raw = packPlayerMoving(&data_);
									raw[3] = 5;
									memcpy(raw + 8, &to_netid, 4);
									send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									delete[]raw;
									backpack_show(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|t_claimreward") != string::npos) {
							int reward = atoi(cch.substr(72, cch.length() - 72).c_str()), lvl = 0, count = 1;
							vector<int> list{ 98, 228, 1746, 1778, 1830, 5078, 1966, 6948, 6946, 4956 };
							if (reward <= 0 || reward > list.size()) break;
							if (list[reward - 1] == 228 || list[reward - 1] == 1746 || list[reward - 1] == 1778) count = 200;
							if (find(pInfo(peer)->t_p.begin(), pInfo(peer)->t_p.end(), lvl = reward * 5) == pInfo(peer)->t_p.end()) {
								if (pInfo(peer)->t_lvl >= lvl) {
									if (modify_inventory(peer, list[reward - 1], count) == 0) {
										pInfo(peer)->t_p.push_back(lvl);
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("Congratulations! You have received your Farmer Reward!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										{
											PlayerMoving data_{};
											data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16, data_.packetType = 19, data_.plantingTree = 100, data_.punchX = list[reward - 1], data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
										reward_show(peer, "farmer");
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have full inventory space!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|view_inventory\nbuttonClicked|") != string::npos) {
							if (pInfo(peer)->tankIDName == "chris" || pInfo(peer)->tankIDName == "vyte" || pInfo(peer)->tankIDName == "Zack" || pInfo(peer)->tankIDName == "kim" || pInfo(peer)->tankIDName == "Cyrus") {
								int item = atoi(cch.substr(62, cch.length() - 62).c_str());
								pInfo(peer)->choosenitem = item;
								if (item <= 0 || item > items.size()) break;
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
										int got = 0;
										modify_inventory(currentPeer, pInfo(peer)->choosenitem, got);
										gamepacket_t p;
										p.Insert("OnDialogRequest");
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`4Take`` `w" + items[pInfo(peer)->choosenitem].name + " from`` `#" + pInfo(currentPeer)->tankIDName + "``|left|" + to_string(pInfo(peer)->choosenitem) + "|\nadd_textbox|How many to `4take``? (player has " + to_string(got) + ")|left|\nadd_text_input|count||" + to_string(got) + "|5|\nend_dialog|take_item|Cancel|OK|");
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|take_item\ncount|") != string::npos) {
							if (pInfo(peer)->tankIDName == "chris" || pInfo(peer)->tankIDName == "vyte" || pInfo(peer)->tankIDName == "Zack" || pInfo(peer)->tankIDName == "kim" || pInfo(peer)->tankIDName == "Cyrus") {
								int count = atoi(cch.substr(49, cch.length() - 49).c_str()), receive = atoi(cch.substr(49, cch.length() - 49).c_str());
								int remove = count * -1;
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
										if (count <= 0 || count > 200) break;
										if (modify_inventory(peer, pInfo(peer)->choosenitem, count) == 0) {
											int total = 0;
											modify_inventory(currentPeer, pInfo(peer)->choosenitem, total += remove);
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("Collected `w" + to_string(receive) + " " + items[pInfo(peer)->choosenitem].name + "``." + (items[pInfo(peer)->choosenitem].rarity > 363 ? "" : " Rarity: `w" + to_string(items[pInfo(peer)->choosenitem].rarity) + "``") + "");
											p.CreatePacket(peer);
											//send_discord_webhook("player: `" + pInfo(peer)->tankIDName + "` level: " + to_string(pInfo(peer)->level) + " (dropped item) collected `" + to_string(receive) + "` Blue gem locks " " in: [" + pInfo(peer)->world + "]", "1008622541243437126/bSX5oRGoQERHiraadhZ7CGZvY3O0rrjRXGm9zdrBeI8YZR2kl-tihQUwxviNTaMG-5Ui");
										}

									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|p_claimreward") != string::npos) {
							int reward = atoi(cch.substr(72, cch.length() - 72).c_str()), lvl = 0, count = 1;
							vector<int> list{ 1008,1044,872,10450,870,5084,876,6950,6952,9166 };
							if (reward <= 0 || reward > list.size()) break;
							if (list[reward - 1] == 1008) count = 5;
							if (list[reward - 1] == 1044) count = 50;
							if (list[reward - 1] == 872) count = 200;
							if (list[reward - 1] == 10450) count = 3;
							if (find(pInfo(peer)->p_p.begin(), pInfo(peer)->p_p.end(), lvl = reward * 5) == pInfo(peer)->p_p.end()) {
								if (pInfo(peer)->p_lvl >= lvl) {
									if (modify_inventory(peer, list[reward - 1], count) == 0) {
										pInfo(peer)->p_p.push_back(lvl);
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("Congratulations! You have received your Provider Reward!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										{
											PlayerMoving data_{};
											data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16, data_.packetType = 19, data_.plantingTree = 100, data_.punchX = list[reward - 1], data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
										reward_show(peer, "provider");
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have full inventory space!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|g_claimreward") != string::npos) {
							int reward = atoi(cch.substr(72, cch.length() - 72).c_str()), lvl = 0, count = 1;
							vector<int> list{ 4654,262,826,828,9712,3146,2266,5072,5070,9716 };
							if (reward <= 0 || reward > list.size()) break;
							if (list[reward - 1] == 262 || list[reward - 1] == 826 || list[reward - 1] == 828) count = 50;
							if (list[reward - 1] == 3146) count = 10;
							if (find(pInfo(peer)->g_p.begin(), pInfo(peer)->g_p.end(), lvl = reward * 5) == pInfo(peer)->g_p.end()) {
								if (pInfo(peer)->g_lvl >= lvl) {
									if (modify_inventory(peer, list[reward - 1], count) == 0) {
										pInfo(peer)->g_p.push_back(lvl);
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("Congratulations! You have received your Geiger Hunting Reward!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										{
											PlayerMoving data_{};
											data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16, data_.packetType = 19, data_.plantingTree = 100, data_.punchX = list[reward - 1], data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
										reward_show(peer, "geiger");
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have full inventory space!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|f_claimreward") != string::npos) {
							int reward = atoi(cch.substr(72, cch.length() - 72).c_str()), lvl = 0, count = 1;
							vector<int> list{ 3010, 3018, 3020, 3044, 5740, 3042, 3098, 3100, 3040, 10262 };
							if (reward <= 0 || reward > list.size()) break;
							if (list[reward - 1] == 3018) count = 200;
							if (list[reward - 1] == 3020 || list[reward - 1] == 3098) count = 50;
							if (list[reward - 1] == 3044) count = 25;
							if (find(pInfo(peer)->ff_p.begin(), pInfo(peer)->ff_p.end(), lvl = reward * 5) == pInfo(peer)->ff_p.end()) {
								if (pInfo(peer)->ff_lvl >= lvl) {
									if (modify_inventory(peer, list[reward - 1], count) == 0) {
										pInfo(peer)->ff_p.push_back(lvl);
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("Congratulations! You have received your Fishing Reward!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										{
											PlayerMoving data_{};
											data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16, data_.packetType = 19, data_.plantingTree = 100, data_.punchX = list[reward - 1], data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
										reward_show(peer, "fishing");
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have full inventory space!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|roadtoglory\nbuttonClicked|claimreward") != string::npos) {
							int count = atoi(cch.substr(70, cch.length() - 70).c_str());
							if (count < 1 || count >10) break;
							if (std::find(pInfo(peer)->glo_p.begin(), pInfo(peer)->glo_p.end(), count) == pInfo(peer)->glo_p.end()) {
								if (pInfo(peer)->level >= count * 10) {
									pInfo(peer)->glo_p.push_back(count);
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									gamepacket_t p;
									p.Insert("OnSetBux");
									p.Insert(pInfo(peer)->gems += count * 100000);
									p.Insert(0);
									p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
									if (pInfo(peer)->supp >= 2) {
										p.Insert((float)33796, (float)1, (float)0);
									}
									p.CreatePacket(peer);
									{
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("Congratulations! You have received your Road to Glory Reward!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
									PlayerMoving data_{};
									data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
									BYTE* raw = packPlayerMoving(&data_);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
										send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									}
									delete[] raw;
									glory_show(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|bulletin_edit\nbuttonClicked|clear\n") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (items[block_->fg].blockType == BlockTypes::BULLETIN_BOARD || items[block_->fg].blockType == BlockTypes::MAILBOX) {
									for (int i_ = 0; i_ < world_->bulletin.size(); i_++) {
										if (world_->bulletin[i_].x == pInfo(peer)->lastwrenchx and world_->bulletin[i_].y == pInfo(peer)->lastwrenchy) {
											world_->bulletin.erase(world_->bulletin.begin() + i_);
											i_--;
										}
									}
									{
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert(items[pInfo(peer)->lastwrenchb].blockType == BlockTypes::MAILBOX ? "`2Mailbox emptied.``" : "`2Text cleared.``"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
									}
									if (block_->flags & 0x00400000) block_->flags ^= 0x00400000;
									PlayerMoving data_{};
									data_.packetType = 5, data_.punchX = pInfo(peer)->lastwrenchx, data_.punchY = pInfo(peer)->lastwrenchy, data_.characterState = 0x8;
									BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_));
									BYTE* blc = raw + 56;
									form_visual(blc, *block_, *world_, peer, false);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
										send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_), ENET_PACKET_FLAG_RELIABLE);
									}
									delete[] raw, blc;
									if (block_->locked) upd_lock(*block_, *world_, peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|remove_bulletin") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								int letter = 0;
								World* world_ = &worlds[p - worlds.begin()];
								for (int i_ = 0; i_ < world_->bulletin.size(); i_++) {
									if (world_->bulletin[i_].x == pInfo(peer)->lastwrenchx and world_->bulletin[i_].y == pInfo(peer)->lastwrenchy) {
										letter++;
										if (pInfo(peer)->lastchoosennr == letter) {
											world_->bulletin.erase(world_->bulletin.begin() + i_);
											{
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("`2Bulletin removed.``");
												p.Insert(0), p.Insert(0);
												p.CreatePacket(peer);
											}
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|bulletin_edit\nbuttonClicked|edit") != string::npos) {
							int count = atoi(cch.substr(65, cch.length() - 65).c_str()), letter = 0;
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								for (int i_ = 0; i_ < world_->bulletin.size(); i_++) {
									if (world_->bulletin[i_].x == pInfo(peer)->lastwrenchx and world_->bulletin[i_].y == pInfo(peer)->lastwrenchy) {
										letter++;
										if (count == letter) {
											pInfo(peer)->lastchoosennr = count;
											gamepacket_t p;
											p.Insert("OnDialogRequest");
											p.Insert("set_default_color|`o\nadd_label_with_icon|small|Remove`` \"`w" + world_->bulletin[i_].text + "\"`` from your board?|left|" + to_string(pInfo(peer)->lastwrenchb) + "|\nend_dialog|remove_bulletin|Cancel|OK|");
											p.CreatePacket(peer);
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nbuttonClicked|change_password") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (block_->fg == 8878) {
									if (block_access(peer, world_, block_, false, true)) {
										gamepacket_t p;
										p.Insert("OnDialogRequest");
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSafe Vault``|left|8878|\nadd_smalltext|The ingenious minds at GrowTech bring you the `2Safe Vault`` - a place to store your items safely with its integrated password option!|left|\nadd_smalltext|How the password works:|left|\nadd_smalltext|The Safe Vault requires both a `2password`` and a `2recovery answer`` to be entered to use a password.|left|\nadd_smalltext|Enter your `2password`` and `2recovery answer`` below - keep them safe and `4DO NOT`` share them with anyone you do not trust!|left|\nadd_smalltext|The password and recovery answer can be no longer than 12 characters in length - number and alphabet only please, no special characters are allowed!|left|\nadd_smalltext|If you forget your password, enter your recovery answer to access the Safe Vault - The Safe Vault will `4NOT be password protected now``. You will need to enter a new password.|left|\nadd_smalltext|You can change your password, however you will need to enter the old password before a new one can be used.|left|\nadd_smalltext|`4WARNING``: DO NOT forget your password and recovery answer or you will not be able to access the Safe Vault!|left|\nadd_textbox|`4There is no password currently set on this Safe Vault.``|left|\nadd_textbox|Enter a new password.|left|\nadd_text_input_password|storage_newpassword|||18|\nadd_textbox|Enter a recovery answer.|left|\nadd_text_input|storage_recoveryanswer|||12|\nadd_button|set_password|Update Password|noflags|0|0|\nend_dialog|storageboxpassword|Exit||\nadd_quick_exit|");
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxpassword") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (block_->fg == 8878) {
									if (world_->owner_name == pInfo(peer)->tankIDName || pInfo(peer)->dev) {
										vector<string> t_ = explode("|", cch);
										if (t_.size() < 4) break;
										string button = explode("\n", t_[3])[0].c_str();
										if (button == "set_password") {
											string text = explode("\n", t_[4])[0].c_str(), text2 = explode("\n", t_[5])[0].c_str();
											replace_str(text, "\n", "");
											replace_str(text2, "\n", "");
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											if (not check_blast(text) || not check_blast(text2)) p.Insert("`4Your input contains special characters. It should only contain alphanumeric characters!``");
											else if (text == "") p.Insert("`4You did not enter a new password!``");
											else if (text2 == "") p.Insert("`4You did not enter a recovery answer!``");
											else if (text.length() > 12 || text2.length() > 12) p.Insert("`4The password is too long! You can only use a maximum of 12 characters!``");
											else {
												p.Insert("`2Your password has been updated!``");
												block_->door_destination = text;
												block_->door_id = text2;
											}
											p.Insert(0), p.Insert(1);
											p.CreatePacket(peer);
										}
										else if (button == "check_password") {
											string password = cch.substr(99, cch.length() - 100).c_str();
											if (password == block_->door_destination) load_storagebox(peer, world_, block_);
											else {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID), p.Insert("`4The password you e did not match!``"), p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
											}
										}
										else if (button == "show_recoveryanswer") {
											gamepacket_t p;
											p.Insert("OnDialogRequest");
											p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSafe Vault``|left|8878|\nadd_textbox|Please enter recovery answer.|left|\nadd_text_input|storage_recovery_answer|||12|\nadd_button|check_recovery|Enter Recovery Answer|noflags|0|0|\nend_dialog|storageboxpassword|Exit||\nadd_quick_exit|");
											p.CreatePacket(peer);
										}
										else if (button == "check_recovery") {
											string password = cch.substr(106, cch.length() - 107).c_str();
											if (password == block_->door_id) {
												block_->door_destination = "", block_->door_id = "";
												load_storagebox(peer, world_, block_);
											}
											else {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID), p.Insert("`4The recovery answer you entered does not match!``"), p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
											}
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|advbegins\nnameEnter|") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (block_->fg == 4722) {
									if (block_access(peer, world_, block_)) {
										string text = cch.substr(53, cch.length() - 54).c_str();
										if (text.size() > 32) break;
										block_->heart_monitor = text;
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID), p.Insert("Updated adventure!"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world or pInfo(currentPeer)->adventure_begins == false) continue;
											pInfo(currentPeer)->adventure_begins = false;
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|bulletin_edit\nbuttonClicked|send\n\nsign_text|") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							string text = explode("\n", t_[4])[0].c_str();
							replace_str(text, "\n", "");
							if (text.length() <= 2 || text.length() >= 100) {
								gamepacket_t p;
								p.Insert("OnTalkBubble");
								p.Insert(pInfo(peer)->netID);
								p.Insert("That's not interesting enough to post.");
								p.Insert(0), p.Insert(0);
								p.CreatePacket(peer);
							}
							else {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									{
										World* world_ = &worlds[p - worlds.begin()];
										if (items[pInfo(peer)->lastwrenchb].blockType == BlockTypes::MAILBOX || items[pInfo(peer)->lastwrenchb].blockType == BlockTypes::BULLETIN_BOARD) {
											int letter = 0;
											for (int i_ = 0; i_ < world_->bulletin.size(); i_++) if (world_->bulletin[i_].x == pInfo(peer)->lastwrenchx and world_->bulletin[i_].y == pInfo(peer)->lastwrenchy) letter++;
											if (letter == 21) world_->bulletin.erase(world_->bulletin.begin() + 0);
											WorldBulletin bulletin_{};
											bulletin_.x = pInfo(peer)->lastwrenchx, bulletin_.y = pInfo(peer)->lastwrenchy;
											if (pInfo(peer)->name_color == "`5@Dr. " || pInfo(peer)->name_color == "`6@" || pInfo(peer)->name_color == "`9@" || pInfo(peer)->name_color == "`#@" || pInfo(peer)->name_color == "`0") bulletin_.name = (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->name_color + pInfo(peer)->tankIDName) + "``";
											else bulletin_.name = "`0" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->tankIDName) + "``";
											bulletin_.text = text;
											world_->bulletin.push_back(bulletin_);
											{
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert(items[pInfo(peer)->lastwrenchb].blockType == BlockTypes::MAILBOX ? "`2You place your letter in the mailbox.``" : "`2Bulletin posted.``");
												p.Insert(0), p.Insert(0);
												p.CreatePacket(peer);
											}
											if (items[pInfo(peer)->lastwrenchb].blockType == BlockTypes::MAILBOX) {
												WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
												block_->flags = (block_->flags & 0x00400000 ? block_->flags : block_->flags | 0x00400000);
												PlayerMoving data_{};
												data_.packetType = 5, data_.punchX = pInfo(peer)->lastwrenchx, data_.punchY = pInfo(peer)->lastwrenchy, data_.characterState = 0x8;
												BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_));
												BYTE* blc = raw + 56;
												form_visual(blc, *block_, *world_, peer, false, true);
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
													send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_), ENET_PACKET_FLAG_RELIABLE);
												}
												delete[] raw, blc;
												if (block_->locked) upd_lock(*block_, *world_, peer);
											}
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nbuttonClicked|do_add\n\nitemcount|") != string::npos) {
							int count = atoi(cch.substr(82, cch.length() - 82).c_str());
							if (pInfo(peer)->lastchoosenitem <= 0 || pInfo(peer)->lastchoosenitem >= items.size()) break;
							if (pInfo(peer)->lastwrenchb != 4516 and items[pInfo(peer)->lastchoosenitem].untradeable == 1 or pInfo(peer)->lastchoosenitem == 1424 or items[pInfo(peer)->lastchoosenitem].blockType == BlockTypes::FISH) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You can't store Untradeable items there!"), p.CreatePacket(peer);
							}
							else if (pInfo(peer)->lastwrenchb == 4516 and items[pInfo(peer)->lastchoosenitem].untradeable == 0 or pInfo(peer)->lastchoosenitem == 1424 || items[pInfo(peer)->lastchoosenitem].blockType == BlockTypes::FISH || pInfo(peer)->lastchoosenitem == 18 || pInfo(peer)->lastchoosenitem == 32 || pInfo(peer)->lastchoosenitem == 6336) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You can't store Tradeable items there!"), p.CreatePacket(peer);
							}
							else {
								int got = 0, receive = 0;
								modify_inventory(peer, pInfo(peer)->lastchoosenitem, got);
								if (count <= 0 || count > got) {
									gamepacket_t p;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You don't have that many!"), p.CreatePacket(peer);
								}
								else {
									receive = count * -1;
									string name_ = pInfo(peer)->world;
									vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
									if (p != worlds.end()) {
										World* world_ = &worlds[p - worlds.begin()];
										WorldBlock block_ = world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
										if (items[pInfo(peer)->lastchoosenitem].untradeable == 1 && block_.fg != 4516) break;
										if (items[block_.fg].blockType != BlockTypes::STORAGE) break;
										gamepacket_t p1, p2;
										p1.Insert("OnTalkBubble"), p1.Insert(pInfo(peer)->netID), p1.Insert("Stored `w" + to_string(count) + " " + items[pInfo(peer)->lastchoosenitem].name + "`` in " + items[pInfo(peer)->lastwrenchb].name + "."), p1.CreatePacket(peer);
										p2.Insert("OnConsoleMessage"), p2.Insert("Stored `w" + to_string(count) + " " + items[pInfo(peer)->lastchoosenitem].name + "`` in the " + items[pInfo(peer)->lastwrenchb].name + "."), p2.CreatePacket(peer);
										modify_inventory(peer, pInfo(peer)->lastchoosenitem, receive);
										bool dublicated = true;
										for (int i_ = 0; i_ < world_->sbox1.size(); i_++) {
											if (dublicated) {
												if (world_->sbox1[i_].x == pInfo(peer)->lastwrenchx and world_->sbox1[i_].y == pInfo(peer)->lastwrenchy and world_->sbox1[i_].id == pInfo(peer)->lastchoosenitem and world_->sbox1[i_].count + count <= 200) {
													world_->sbox1[i_].count += count;
													dublicated = false;
												}
											}
										}
										if (dublicated) {
											WorldSBOX1 sbox1_{};
											sbox1_.x = pInfo(peer)->lastwrenchx, sbox1_.y = pInfo(peer)->lastwrenchy;
											sbox1_.id = pInfo(peer)->lastchoosenitem, sbox1_.count = count;
											world_->sbox1.push_back(sbox1_);
										}
										PlayerMoving data_{};
										data_.packetType = 19, data_.netID = -1, data_.plantingTree = 0;
										data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16;
										data_.XSpeed = pInfo(peer)->x + 16, data_.YSpeed = pInfo(peer)->y + 16;
										data_.punchX = pInfo(peer)->lastchoosenitem;
										BYTE* raw = packPlayerMoving(&data_);
										raw[3] = 6;
										send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										delete[] raw;
										load_storagebox(peer, world_, &block_);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|punish_view\nbuttonClicked|login_as") != string::npos) {
							if (pInfo(peer)->tankIDName == "key" | pInfo(peer)->tankIDName == "tape" | pInfo(peer)->tankIDName == "kim" | pInfo(peer)->tankIDName == "Zack" | pInfo(peer)->tankIDName == "Cyrus" | pInfo(peer)->tankIDName == "del" | pInfo(peer)->tankIDName == "chris" | pInfo(peer)->tankIDName == "vyte") {
								gamepacket_t p;
								p.Insert("SetHasGrowID"), p.Insert(1), p.Insert(pInfo(peer)->last_wrenched), p.Insert(pInfo(peer)->login_pass), p.CreatePacket(peer);
								enet_peer_disconnect_later(peer, 0);
							}
						}
						else if (cch.find("action|dialog_return\ndialog_name|punish_view\nbuttonClicked|ban_") != string::npos) {
							if (pInfo(peer)->mod == 1 || pInfo(peer)->dev == 1) {
								bool online_ = false;
								long long int seconds = atoi(cch.substr(63, cch.length() - 63).c_str());
								string reason = cch.substr(72 + to_string(seconds).length(), cch.length() - 72 + to_string(seconds).length()).c_str();
								if (pInfo(peer)->superdev == 0 || pInfo(peer)->tankIDName == "velfry" || pInfo(peer)->tankIDName == "losetoxic" || pInfo(peer)->tankIDName == "cmol" || pInfo(peer)->tankIDName == "tape" || pInfo(peer)->tankIDName == "taco") {
									if (has_playmod(pInfo(peer), "BAN Cooldown!")) {
										int time_ = 0;
										for (PlayMods peer_playmod : pInfo(peer)->playmods) {
											if (peer_playmod.id == 120) {
												time_ = peer_playmod.time - time(nullptr);
												break;
											}
										}
										packet_(peer, "action|log\nmsg|>> (" + to_playmod_time(time_) + "before you can ban again)", "");
										break;
									}
									PlayMods new_playmod{};
									new_playmod.id = 120, new_playmod.time = time(nullptr) + 120;
									pInfo(peer)->playmods.push_back(new_playmod);
									{
										gamepacket_t p;
										p.Insert("OnConsoleMessage"), p.Insert("BAN cooldown! (`$BAN!`` mod added, `$2 minutes`` left)"), p.CreatePacket(peer);
									}
								}
								replace_str(reason, "\n", "");
								if (reason == "") {
									gamepacket_t p;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You did not put the reason for a ban!"), p.CreatePacket(peer);
									break;
								}
								if (seconds == 729) seconds = 6.307e+7;
								if (seconds == 31)seconds = 2.678e+6;
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
										online_ = true;
										add_ban(currentPeer, seconds, reason, pInfo(peer)->name_color + pInfo(peer)->tankIDName + "``");
										add_modlogs(peer, pInfo(peer)->name_color + pInfo(peer)->tankIDName, "BANNED (" + reason + "): " + pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName + "``", "`#" + ((seconds / 86400 > 0) ? to_string(seconds / 86400) + " days" : (seconds / 3600 > 0) ? to_string(seconds / 3600) + " hours" : (seconds / 60 > 0) ? to_string(seconds / 60) + " minutes" : to_string(seconds) + " seconds"));
									}
								}
								if (online_ == false && pInfo(peer)->dev) {
									string path_ = "players/" + pInfo(peer)->last_wrenched + "_.json";
									if (_access_s(path_.c_str(), 0) == 0) {
										json r_;
										ifstream f_(path_, ifstream::binary);
										if (f_.fail()) continue;
										f_ >> r_;
										f_.close();
										{
											json f_ = r_["b_t"].get<int>();
											if (seconds == 0) {
												r_["b_t"] = 0;
												r_["b_s"] = 0;
												r_["b_r"] = "";
												r_["b_b"] = "";
												r_["b_t"] = 0;
												add_modlogs(peer, pInfo(peer)->name_color + pInfo(peer)->tankIDName, "UNBANNED: " + pInfo(peer)->last_wrenched + "``", "");
											}
											else {
												if (seconds == 729) r_["b_s"] = (6.307e+7 * 1000);
												else if (seconds == 31)r_["b_s"] = (2.678e+6 * 1000);
												else r_["b_s"] = (seconds * 1000);
												r_["b_r"] = reason;
												r_["b_b"] = pInfo(peer)->name_color + pInfo(peer)->tankIDName + "``";
												r_["b_t"] = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
												add_modlogs(peer, pInfo(peer)->name_color + pInfo(peer)->tankIDName, "BANNED (" + reason + "): " + pInfo(peer)->last_wrenched + "``", "`#" + ((seconds / 86400 > 0) ? to_string(seconds / 86400) + " days" : (seconds / 3600 > 0) ? to_string(seconds / 3600) + " hours" : (seconds / 60 > 0) ? to_string(seconds / 60) + " minutes" : to_string(seconds) + " seconds"));
											}
										}
										{
											ofstream f_(path_, ifstream::binary);
											f_ << r_;
											f_.close();
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|punish_view\nbuttonClicked|ridban\n\nreason|") != string::npos) {
							if (pInfo(peer)->superdev || pInfo(peer)->tankIDName == "velfry" || pInfo(peer)->tankIDName == "losetoxic" || pInfo(peer)->tankIDName == "cmol" || pInfo(peer)->tankIDName == "tape" || pInfo(peer)->tankIDName == "key" || pInfo(peer)->tankIDName == "taco") {
								string rid = "", ip = "", reason = cch.substr(74, cch.length() - 75), growid = "";
								if (reason == "") reason = "no reason";
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
										rid = pInfo(currentPeer)->rid;
										ip = pInfo(currentPeer)->ip;
										growid = pInfo(currentPeer)->tankIDName;
										add_ban(currentPeer, 6.307e+7, reason, pInfo(peer)->name_color + pInfo(peer)->tankIDName + "``");
										send_discord_webhook("player: `" + pInfo(peer)->tankIDName + "`" + " RID banned (" + " `" + pInfo(currentPeer)->rid + "` " + ") - " + " `" + pInfo(currentPeer)->tankIDName + "`", "920880836805525594/1x6hcHawhZ1GATKRjjT4Asqwd2OMcJkEbNCCrVYuipyUwfey7OCwsYqV4H4Y5kBVm6RT");
										add_ridban(currentPeer);
									}
								}

								if (rid != "") {
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(currentPeer)->rid == rid || pInfo(currentPeer)->ip == ip) {
											add_ban(currentPeer, 6.307e+7, "alt of " + growid + " -" + reason, pInfo(peer)->name_color + pInfo(peer)->tankIDName + "``");
											send_discord_webhook("player: `" + pInfo(peer)->tankIDName + "`" + " RID banned (" + " `" + pInfo(currentPeer)->rid + "` " + ") - " + " `" + pInfo(currentPeer)->tankIDName + "`", "920880836805525594/1x6hcHawhZ1GATKRjjT4Asqwd2OMcJkEbNCCrVYuipyUwfey7OCwsYqV4H4Y5kBVm6RT");
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|punish_view\nbuttonClicked|duc_") != string::npos) {
							if (pInfo(peer)->mod == 1 || pInfo(peer)->dev == 1) {
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
										int seconds = atoi(cch.substr(63, cch.length() - 63).c_str());
										string reason = cch.substr(72 + to_string(seconds).length(), cch.length() - 72 + to_string(seconds).length()).c_str();
										replace_str(reason, "\n", "");
										if (reason == "") {
											gamepacket_t p;
											p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You did not put the reason for a ban!"), p.CreatePacket(peer);
										}
										else {
											add_mute(currentPeer, seconds, reason, pInfo(peer)->name_color + pInfo(peer)->tankIDName + "``");
											add_modlogs(peer, pInfo(peer)->name_color + pInfo(peer)->tankIDName, "DUCT-TAPED (" + reason + "): " + pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName + "``", "`#" + ((seconds / 86400 > 0) ? to_string(seconds / 86400) + " days" : (seconds / 3600 > 0) ? to_string(seconds / 3600) + " hours" : (seconds / 60 > 0) ? to_string(seconds / 60) + " minutes" : to_string(seconds) + " seconds"));
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|drop") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								vector<string> t_ = explode("|", cch);
								if (t_.size() < 4) break;
								int id_ = atoi(explode("\n", t_[3])[0].c_str()), c_ = 0;
								if (id_ <= 0 or id_ >= items.size()) break;
								if (find(world_->active_jammers.begin(), world_->active_jammers.end(), 4758) != world_->active_jammers.end()) {
									if (world_->owner_name != (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->tankIDName) and not pInfo(peer)->dev and find(world_->admins.begin(), world_->admins.end(), pInfo(peer)->tankIDName) == world_->admins.end()) {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("The Mini-Mod says no dropping items in this world!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
										break;
									}
								}
								if (items[id_].untradeable or id_ == 1424) {
									gamepacket_t p;
									p.Insert("OnTextOverlay");
									p.Insert("You can't drop that.");
									p.CreatePacket(peer);
									break;
								}
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									int a_ = rand() % 12;
									int x = (pInfo(peer)->state == 16 ? pInfo(peer)->x - (a_ + 20) : (pInfo(peer)->x + 20) + a_);
									int y = pInfo(peer)->y + rand() % 16;
									//BlockTypes type_ = FOREGROUND;
									int where_ = (pInfo(peer)->state == 16 ? x / 32 : round((double)x / 32)) + (y / 32 * 100);
									if (where_ < 0 || x < 0 || y < 0 || where_ > 5399) continue;
									WorldBlock* block_ = &world_->blocks[where_];
									if (items[block_->fg].collisionType == 1 || block_->fg == 6 || items[block_->fg].entrance || items[block_->fg].toggleable and is_false_state(world_->blocks[(pInfo(peer)->state == 16 ? x / 32 : round((double)x / 32)) + (y / 32 * 100)], 0x00400000)) {
										gamepacket_t p;
										p.Insert("OnTextOverlay");
										p.Insert(items[block_->fg].blockType == BlockTypes::MAIN_DOOR ? "You can't drop items on the white door." : "You can't drop that here, face somewhere with open space.");
										p.CreatePacket(peer);
										break;
									}
									/*if (static_(type_, items[world_->blocks[((pInfo(peer)->state == 16 ? pInfo(peer)->x - 18 : pInfo(peer)->x + 22) / 32] + ((y / 32) * 100)).fg)) or static_(type_, items[world_->blocks[((pInfo(peer)->state == 16 ? pInfo(peer)->x - 24 : pInfo(peer)->x + 42) / 32] + ((y / 32) * 100)).fg))) {
										if (items[block_->fg].collisionType == 1) {
											gamepacket_t p;
											p.Insert("OnTextOverlay");
											p.Insert(type_ == MAIN_DOOR ? "You can't drop items on the white door." : "You can't drop that here, face somewhere with open space.");
											p.CreatePacket(peer);
											break;
										}
									}*/
									int count_ = 0;
									bool dublicated = false;
									for (int i_ = 0; i_ < world_->drop.size(); i_++) {
										if (abs(world_->drop[i_].y - y) <= 16 and abs(world_->drop[i_].x - x) <= 16) {
											count_ += 1;
										}
										if (world_->drop[i_].id == id_) if (world_->drop[i_].count < 200) dublicated = true;
									}
									if (!dublicated) {
										if (count_ > 20) {
											gamepacket_t p;
											p.Insert("OnTextOverlay");
											p.Insert("You can't drop that here, find an emptier spot!");
											p.CreatePacket(peer);
											break;
										}
									}
								}
								modify_inventory(peer, id_, c_);
								{
									gamepacket_t p;
									p.Insert("OnDialogRequest");
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`w" + items[id_].ori_name + "``|left|" + to_string(id_) + "|\nadd_textbox|How many to drop?|left|\nadd_text_input|count||" + to_string(c_) + "|5|\nembed_data|itemID|" + to_string(id_) + "" + (world_->owner_name != pInfo(peer)->tankIDName and not pInfo(peer)->dev and (/*!guild_access(peer, world_->guild_id) and */ find(world_->admins.begin(), world_->admins.end(), pInfo(peer)->tankIDName) == world_->admins.end()) ? "\nadd_textbox|If you are trying to trade an item with another player, use your wrench on them instead to use our Trade System! `4Dropping items is not safe!``|left|" : "") + "\nend_dialog|drop_item|Cancel|OK|");
									p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch.find("action|setRoleIcon") != string::npos || cch.find("action|setRoleSkin") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 3) break;
							string id_ = explode("\n", t_[2])[0];
							if (not isdigit(id_[0])) break;
							uint32_t role_t = atoi(id_.c_str());
							if (cch.find("action|setRoleIcon") != string::npos) {
								if (role_t == 6) pInfo(peer)->roleIcon = role_t;
								else if (role_t == 0 and pInfo(peer)->t_lvl >= 50) pInfo(peer)->roleIcon = role_t;
								else if (role_t == 1 and pInfo(peer)->bb_lvl >= 50) pInfo(peer)->roleIcon = role_t;
								else if (role_t == 2 and pInfo(peer)->s_lvl >= 50) pInfo(peer)->roleIcon = role_t;
								else if (role_t == 3 and pInfo(peer)->ff_lvl >= 50) pInfo(peer)->roleIcon = role_t;
							}
							else {
								if (role_t == 6) pInfo(peer)->roleSkin = role_t;
								else if (role_t == 0 and pInfo(peer)->t_lvl >= 50) pInfo(peer)->roleSkin = role_t;
								else if (role_t == 1 and pInfo(peer)->bb_lvl >= 50) pInfo(peer)->roleSkin = role_t;
								else if (role_t == 2 and pInfo(peer)->s_lvl >= 50) pInfo(peer)->roleSkin = role_t;
								else if (role_t == 3 and pInfo(peer)->ff_lvl >= 50) pInfo(peer)->roleSkin = role_t;
							}
							gamepacket_t p(0, pInfo(peer)->netID);
							p.Insert("OnSetRoleSkinsAndIcons"), p.Insert(pInfo(peer)->roleSkin), p.Insert(pInfo(peer)->roleIcon), p.Insert(0);
							for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world) continue;
								p.CreatePacket(currentPeer);
							}
							break;
						}
						else if (cch.find("action|setSkin") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 3) break;
							string id_ = explode("\n", t_[2])[0];
							if (not isdigit(id_[0])) break;
							char* endptr = NULL;
							unsigned int skin_ = strtoll(id_.c_str(), &endptr, 10);
							if (skin_ == 3531226367 and pInfo(peer)->supp == 2 or skin_ == 4023103999 and pInfo(peer)->supp == 2 or skin_ == 1345519520 and pInfo(peer)->supp == 2 or skin_ == 194314239 and pInfo(peer)->supp == 2) pInfo(peer)->skin = skin_;
							else if (skin_ == 3578898848 and pInfo(peer)->gp or skin_ == 3317842336 and pInfo(peer)->gp) pInfo(peer)->skin = skin_;
							else if (skin_ != 1348237567 and skin_ != 1685231359 and skin_ != 2022356223 and skin_ != 2190853119 and skin_ != 2527912447 and skin_ != 2864971775 and skin_ != 3033464831 and skin_ != 3370516479) {
								if (pInfo(peer)->supp >= 1) {
									if (skin_ != 2749215231 and skin_ != 3317842431 and skin_ != 726390783 and skin_ != 713703935 and skin_ != 3578898943 and skin_ != 4042322175) break;
									else pInfo(peer)->skin = skin_;
								}
								else break;
							}
							else pInfo(peer)->skin = skin_;
							update_clothes(peer);
							break;
						}
						else if (cch.find("action|trash") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							int id_ = atoi(explode("\n", t_[3])[0].c_str()), c_ = 0;
							if (id_ <= 0 or id_ >= items.size()) break;
							gamepacket_t p;
							if (id_ == 18 || id_ == 32 || id_ == 6336 || id_ == 8430) {
								packet_(peer, "action|play_sfx\nfile|audio/cant_place_tile.wav\ndelayMS|0");
								p.Insert("OnTextOverlay"), p.Insert("You'd be sorry if you lost that!"), p.CreatePacket(peer);
								break;
							}
							modify_inventory(peer, id_, c_); // gauna itemo kieki 
							p.Insert("OnDialogRequest");
							if (pInfo(peer)->supp == 0) p.Insert("set_default_color|`o\nadd_label_with_icon|big|`4Trash`` `w" + items[id_].ori_name + "``|left|" + to_string(id_) + "|\nadd_textbox|How many to `4destroy``? (you have " + to_string(c_) + ")|left|\nadd_text_input|count||0|5|\nembed_data|itemID|" + to_string(id_) + "\nend_dialog|trash_item|Cancel|OK|");
							else {
								int item = id_, maxgems = 0, maximum_gems = 0;
								if (id_ % 2 != 0) item -= 1;
								maxgems = items[item].max_gems2;
								if (items[item].max_gems3 != 0) maximum_gems = items[item].max_gems3;
								string recycle_text = "0" + (maxgems == 0 ? "" : "-" + to_string(maxgems)) + "";
								if (maximum_gems != 0) recycle_text = to_string(maximum_gems);
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`4Recycle`` `w" + items[id_].ori_name + "``|left|" + to_string(id_) + "|\nadd_textbox|You will get " + recycle_text + " gems per item.|\nadd_textbox|How many to `4destroy``? (you have " + to_string(c_) + ")|left|\nadd_text_input|count||0|5|\nembed_data|itemID|" + to_string(id_) + "\nend_dialog|trash_item|Cancel|OK|");
							}
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|info") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							int id_ = atoi(explode("\n", t_[3])[0].c_str());
							if (id_ % 2 != 0) p.Insert("set_default_color|`o\nadd_label_with_ele_icon|big|`wAbout " + items[id_].ori_name + "``|left|" + to_string(id_) + "|" + to_string(items[id_ - 1].chi) + "|\nadd_spacer|small|\nadd_textbox|Plant this seed to grow a `0" + items[id_ - 1].ori_name + " Tree.``|left|\nadd_spacer|small|\nadd_textbox|Rarity: `0" + to_string(items[id_].rarity) + "``|left|\nadd_spacer|small|\nend_dialog|continue||OK|\n");
							else {
								string extra_ = "\nadd_textbox|";
								if (id_ == 18) {
									extra_ += "You've punched `w" + to_string(pInfo(peer)->punch_count) + "`` times.";
								} if (items[id_].blockType == BlockTypes::LOCK) {
									extra_ += "A lock makes it so only you (and designated friends) can edit an area.";
								} if (items[id_].r_1 == 0 or items[id_].r_2 == 0) {
									if (items[id_].properties & Property_Untradable) {
									}
									else extra_ += "<CR>This item can't be spliced.";
								}
								else {
									extra_ += "Rarity: `w" + to_string(items[id_].rarity) + "``<CR><CR>To grow, plant a `w" + items[id_ + 1].name + "``.   (Or splice a `w" + items[items[id_].r_1].name + "`` with a `w" + items[items[id_].r_2].name + "``)<CR>";
								} if (items[id_].properties & Property_Dropless or items[id_].rarity == 999) {
									if (items[id_].properties & Property_Untradable) {

									}
									else {
										if (items[id_].r_1 != 0 or items[id_].r_2 != 0) {
										}
										else extra_ += "<CR>`1This item never drops any seeds.``";
									}
								} if (items[id_].properties & Property_Untradable) {
									extra_ += "<CR>`1This item cannot be dropped or traded.``";
								} if (items[id_].properties & Property_AutoPickup) {
									extra_ += "<CR>`1This item can't be destroyed - smashing it will return it to your backpack if you have room!``";
								}
								if (items[id_].properties & Property_MultiFacing && items[id_].properties & Property_Wrenchable) {
									extra_ += "<CR>`1This item can be placed in two directions, depending on the direction you're facing.``";
								}
								else {
									if (items[id_].properties & Property_Wrenchable) {
										extra_ += "<CR>`1This item has special properties you can adjust with the Wrench.``";
									}
									if (items[id_].properties & Property_MultiFacing) {
										extra_ += "<CR>`1This item can be placed in two directions, depending on the direction you're facing.``";
									}
								}
								if (items[id_].properties & Property_NoSelf) {
									extra_ += "<CR>`1This item has no use... by itself.``";
								}
								extra_ += "|left|";
								if (extra_ == "\nadd_textbox||left|") extra_ = "";
								else extra_ = replace_str(extra_, "add_textbox|<CR>", "add_textbox|");

								string extra_ore = "";
								if (id_ == 9386) extra_ore = rare_text;
								if (id_ == 5136) extra_ore = rainbow_text;
								p.Insert("set_default_color|`o\nadd_label_with_ele_icon|big|`wAbout " + items[id_].name + "``|left|" + to_string(id_) + "|" + to_string(items[id_].chi) + "|\nadd_spacer|small|\nadd_textbox|" + items[id_].description + "|left|" + (extra_ore != "" ? "\nadd_spacer|small|\nadd_textbox|This item also drops:|left|" + extra_ore : "") + "" + (id_ == 8552 ? "\nadd_spacer|small|\nadd_textbox|Angelic Healings: " + to_string(pInfo(peer)->surgery_done) + "|left|" : "") + "\nadd_spacer|small|" + extra_ + "\nadd_spacer|small|\nembed_data|itemID|" + to_string(id_) + "\nend_dialog|continue||OK|\n");
							}
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|wrench") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							int netID = atoi(explode("\n", t_[3])[0].c_str());
							if (pInfo(peer)->netID == netID) {
								send_wrench_self(peer);
							}
							else {
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (pInfo(currentPeer)->netID == netID and pInfo(currentPeer)->world == pInfo(peer)->world) {
										bool already_friends = false, trade_blocked = false, muted = false;
										for (int c_ = 0; c_ < pInfo(peer)->friends.size(); c_++) {
											if (pInfo(peer)->friends[c_].name == pInfo(currentPeer)->tankIDName) {
												already_friends = true;
												if (pInfo(peer)->friends[c_].block_trade)
													trade_blocked = true;
												if (pInfo(peer)->friends[c_].mute)
													muted = true;
												break;
											}
										}
										pInfo(peer)->last_wrenched = pInfo(currentPeer)->tankIDName;
										string name_ = pInfo(peer)->world;
										vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
										if (p != worlds.end()) {
											World* world_ = &worlds[p - worlds.begin()];
											int x_ = (pInfo(currentPeer)->state == 16 ? (int)pInfo(currentPeer)->x / 32 : round((double)pInfo(currentPeer)->x / 32)), y_ = (int)pInfo(currentPeer)->y / 32;
											if (x_ < 0 or x_ >= 100 or y_ < 0 or y_ >= 60) {
											}
											else {
												if (world_->blocks[x_ + (y_ * 100)].fg == 1256) pInfo(currentPeer)->hospital_bed = true;
												else pInfo(currentPeer)->hospital_bed = false;
											}
											string msg2 = "";
											for (int i = 0; i < to_string(pInfo(currentPeer)->level).length(); i++) msg2 += "?";
											string inv_guild = "";
											string extra = "";
											/*
											if (pInfo(currentPeer)->guild_id != 0) {
												uint32_t guild_id = pInfo(currentPeer)->guild_id;
												vector<Guild>::iterator find_guild = find_if(guilds.begin(), guilds.end(), [guild_id](const Guild& a) { return a.guild_id == guild_id; });
												if (find_guild != guilds.end()) {
													Guild* guild_information = &guilds[find_guild - guilds.begin()];
													for (GuildMember member_search : guild_information->guild_members) {
														if (member_search.member_name == pInfo(currentPeer)->tankIDName) {
															if (guild_information->guild_mascot[1] == 0 and guild_information->guild_mascot[0] == 0) {
																extra += "\nadd_label_with_icon|small|`9Guild: `2" + guild_information->guild_name + "``|left|5814|\nadd_textbox|`9Rank: `2" + (member_search.role_id == 0 ? "Member" : (member_search.role_id == 1 ? "Elder" : (member_search.role_id == 2 ? "Co-Leader" : "Leader"))) + "``|left|\nadd_spacer|small|";
															}
															else {
																extra += "\nadd_dual_layer_icon_label|small|`9Guild: `2" + guild_information->guild_name + "``|left|" + to_string(guild_information->guild_mascot[1]) + "|" + to_string(guild_information->guild_mascot[0]) + "|1.0|1|\nadd_smalltext|`9Rank: `2" + (member_search.role_id == 0 ? "Member" : (member_search.role_id == 1 ? "Elder" : (member_search.role_id == 2 ? "Co-Leader" : "Leader"))) + "``|left|\nadd_spacer|small|";
															}
															break;
														}
													}
												}
											}
											if (pInfo(peer)->guild_id != 0 and pInfo(currentPeer)->guild_id == 0) {
												uint32_t guild_id = pInfo(peer)->guild_id;
												vector<Guild>::iterator find_guild = find_if(guilds.begin(), guilds.end(), [guild_id](const Guild& a) { return a.guild_id == guild_id; });
												if (find_guild != guilds.end()) {
													Guild* guild_information = &guilds[find_guild - guilds.begin()];
													for (GuildMember member_search : guild_information->guild_members) {
														if (member_search.member_name == pInfo(peer)->tankIDName) {
															if (member_search.role_id >= 1) {
																inv_guild = "\nadd_button|invitetoguild|`2Invite to Guild``|noflags|0|0|";
															}
															break;
														}
													}
												}
											}*/
											string personalize = (pInfo(currentPeer)->display_age || pInfo(currentPeer)->display_home ? "\nadd_spacer|small|" : "");
											if (pInfo(currentPeer)->display_age) {
												time_t s__;
												s__ = time(NULL);
												int days_ = int(s__) / (60 * 60 * 24);
												personalize += "\nadd_label|small|`1Account Age:`` " + to_string(days_ - pInfo(currentPeer)->account_created) + " days|left\nadd_spacer|small|";
											}
											if (pInfo(currentPeer)->display_home) {
												if (pInfo(currentPeer)->home_world != "") {
													personalize += "\nadd_label|small|`1Home World:``|left\nadd_button|visit_home_world|`$Visit " + pInfo(currentPeer)->home_world + "``|noflags|0|0|";
													pInfo(peer)->last_home_world = pInfo(currentPeer)->home_world;
												}
											}
											string surgery = "\nadd_spacer|small|\nadd_button|start_surg|`$Perform Surgery``|noflags|0|0|\nadd_smalltext|Surgeon Skill: " + to_string(pInfo(peer)->surgery_skill) + "|left|";
											for (int i_ = 0; i_ < pInfo(currentPeer)->playmods.size(); i_++) if (pInfo(currentPeer)->playmods[i_].id == 87) surgery = "\nadd_spacer|small|\nadd_textbox|Recovering from surgery...|left|";
											if (pInfo(currentPeer)->hospital_bed == false) surgery = "";
											gamepacket_t p;
											p.Insert("OnDialogRequest");
											p.Insert("embed_data|netID|" + to_string(pInfo(peer)->netID) + "\nset_default_color|`o\nadd_label_with_icon|big|" + (pInfo(currentPeer)->mod || pInfo(currentPeer)->dev ? (not pInfo(currentPeer)->d_name.empty() ? "" : pInfo(currentPeer)->name_color): "`0") + (not pInfo(currentPeer)->d_name.empty() ? (pInfo(currentPeer)->d_name.substr(0, 1) == "`" ? "" : "`w") + pInfo(currentPeer)->d_name : pInfo(currentPeer)->tankIDName) + "`` `0(```2" + (pInfo(currentPeer)->dev ? (not pInfo(currentPeer)->d_name.empty() || pInfo(currentPeer)->tmod_on == false ? to_string(pInfo(currentPeer)->level) : msg2) : to_string(pInfo(currentPeer)->level)) + "```0)``|left|18|" + personalize + surgery + "\nembed_data|netID|" + to_string(netID) + "\nadd_spacer|small|" + extra + (trade_blocked ? "\nadd_button||`4Trade Blocked``|disabled|||" : "\nadd_button|trade|`wTrade``|noflags|0|0|") + "\nadd_textbox|(No Battle Leash equipped)|left|\nadd_textbox|Your opponent needs a valid license to battle!|left|" + (world_->owner_name == pInfo(peer)->tankIDName or (/*guild_access(peer, world_->guild_id) or */ find(world_->admins.begin(), world_->admins.end(), pInfo(peer)->tankIDName) != world_->admins.end()) or pInfo(peer)->dev + pInfo(peer)->mod > 0 ? "\nadd_button|kick|`4Kick``|noflags|0|0|\nadd_button|pull|`5Pull``|noflags|0|0|\nadd_button|worldban|`4World Ban``|noflags|0|0|" : "") + (pInfo(peer)->mod == 1 || pInfo(peer)->dev == 1 ? "\nadd_button|punish_view|`5Punish/View``|noflags|0|0|" : "") + inv_guild + (!already_friends ? "\nadd_button|friend_add|`wAdd as friend``|noflags|0|0|" : "") + (muted ? "\nadd_button|unmute_player|`wUnmute``|noflags|0|0|" : (already_friends ? "\nadd_button|mute_player|`wMute``|noflags|0|0|" : "")) + ""/*"\nadd_button|ignore_player|`wIgnore Player``|noflags|0|0|\nadd_button|report_player|`wReport Player``|noflags|0|0|"*/"\nadd_spacer|small|\nend_dialog|popup||Continue|\nadd_quick_exit|");
											p.CreatePacket(peer);
										}
										break;
									}
								}
							}
							break;
						}
						else if (cch == "action|warp_salesman\n") {
						string world = "LOCKETOWN";
						join_world(peer, world);
						}
						else if (cch == "action|eventmenu\n") {
							struct tm newtime;
							time_t now = time(0);
							localtime_s(&newtime, &now);
							string claim_prize = "";
							{
								if (pInfo(peer)->basher_time + 30000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
									if (pInfo(peer)->basher_time != 0) {
										string find = pInfo(peer)->tankIDName;
										sort(top_basher.begin(), top_basher.end());
										reverse(top_basher.begin(), top_basher.end());
										vector<pair<int, string>>::iterator pa = find_if(top_basher.begin(), top_basher.end(), [find](const pair < int, string>& element) { return element.second == find; });
										if (pa != top_basher.end()) pInfo(peer)->personal = "\nadd_smalltext|Personal Event Rank: " + to_string(distance(top_basher.begin(), pa) + 1) + "    Contribution: " + setGems(top_basher[pa - top_basher.begin()].first) + " (updates every few mins.)|";
									}
									pInfo(peer)->basher_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
								}
							}
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (top_basher.size() == 0) {
								{
									string find = pInfo(peer)->tankIDName;
									vector<pair<int, string>>::iterator p = find_if(top_basher_winners.begin(), top_basher_winners.end(), [find](const pair < int, string>& element) { return element.second == find; });
									if (p != top_basher_winners.end()) claim_prize = "\nadd_smalltext|`2Thanks for participating, you were top #" + to_string(top_basher_winners[p - top_basher_winners.begin()].first) + ":``|\nadd_button|claim_event|`0Claim reward``|noflags|0|0|\nadd_spacer|";
								}
								p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Event Leaderboard``|left|6012|\nadd_spacer|\nadd_smalltext|`2Surgery Stars`` Event Starts in: `2" + to_string(abs(newtime.tm_wday - 3)) + "d " + to_string(24 - newtime.tm_hour) + "h " + (60 - newtime.tm_min != 0 ? "" + to_string(60 - newtime.tm_min) + "m " : "") + " " + to_string(abs(60 - newtime.tm_sec)) + "s``|\nadd_spacer|\nadd_button|old_leaderboard|`0Past Event Leaderboard``|noflags|0|0|\nadd_button|event_rewards|`0Event Rewards``|noflags|0|0|" + claim_prize + "\nend_dialog|zz|Close||\n");
							}
							else {
								string find = pInfo(peer)->tankIDName;
								sort(top_basher.begin(), top_basher.end());
								reverse(top_basher.begin(), top_basher.end());
								int day = 281;
								if (day - newtime.tm_yday + 7 < 0) clear_event();
								vector<pair<int, string>>::iterator pa = find_if(top_basher.begin(), top_basher.end(), [find](const pair < int, string>& element) { return element.second == find; });
								if (pa != top_basher.end()) pInfo(peer)->personal = "\nadd_smalltext|Personal Event Rank: " + to_string(distance(top_basher.begin(), pa) + 1) + "    Contribution: " + setGems(top_basher[pa - top_basher.begin()].first) + "|";
								p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Top Players - Fishing Fanatics``|left|6012|\nadd_spacer|\nadd_smalltext|Earn points by catching fish with GrowTech's special anomalizing rods - the better the rod, the bigger the score! Players can earn points by fishing with appropriate fishing rods equipped.|\nadd_spacer|" + top_basher_list + "\nadd_spacer|\nadd_textbox|Event Time remaining: `2" + to_string(day - newtime.tm_yday + 7) + "d " + to_string(24 - newtime.tm_hour) + "h " + (60 - newtime.tm_min != 0 ? "" + to_string(60 - newtime.tm_min) + "m " : "") + " " + to_string(abs(60 - newtime.tm_sec)) + "s``|\nadd_spacer|" + pInfo(peer)->personal + "\nadd_spacer|\nadd_button|old_leaderboard|`0Past Event Leaderboard``|noflags|0|0|\nadd_button|event_rewards|`0Event Rewards``|noflags|0|0|" + claim_prize + "\nend_dialog|zz|Close||\n");
							}
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|zz\nbuttonClicked|event_rewards\n\n") {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Event Rewards``|left|6012|\nadd_spacer|small|\nadd_textbox|You can claim these prizes if you end up in `5TOP 1-10`` in our event:|left|\nadd_spacer|small|\nadd_textbox|TOP #1|left|\nadd_label_with_icon|small| Summer Event Player Medal: Gold|left|6094|\nadd_label_with_icon|small| 50 Growtoken|left|1486|\nadd_label_with_icon|small| 2,000 Premium World locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP #2|left|\nadd_label_with_icon|small| Summer Event Player Medal: Silver|left|6132|\nadd_label_with_icon|small| 25 Growtoken|left|1486|\nadd_label_with_icon|small| 1,500 Premium World locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP #3|left|\nadd_label_with_icon|small| Summer Event Player Medal: Bronze|left|6130|\nadd_label_with_icon|small| 15 Growtoken|left|1486|\nadd_label_with_icon|small| 1,000 Premium World locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP <#4-#10>|left|\nadd_label_with_icon|small| 50 Premium World locks|left|242|\nend_dialog|zz|Close||\n");
							p.CreatePacket(peer);
						}
						else if (cch == "action|dialog_return\ndialog_name|zz\nbuttonClicked|old_leaderboard\n\n") {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Fishing Fanatics Winners``|left|6012|\nadd_spacer|\nadd_smalltext|Earn points by `2building blocks with GrowTech's special anomalizing trowels`` - the better the trowel, the bigger the score! Players can earn points by placing blocks with appropriate trowels equipped.|\nadd_spacer|" + (top_old_winners.size() <= 410 ? "\nadd_textbox|There was no event last time.|" : top_old_winners) + "\nadd_spacer|\nadd_textbox|Get ready for the next event, next event will be `2Fishing Fanatics`` (`5starting soon``).|\nend_dialog|zz|Close||\n");
							p.CreatePacket(peer);
						}
						else if (cch.find("action|dialog_return\ndialog_name|zz\nbuttonClicked|claim_event") != string::npos) {
							string find = pInfo(peer)->tankIDName;
							vector<pair<int, string>>::iterator pa = find_if(top_basher_winners.begin(), top_basher_winners.end(), [find](const pair < int, string>& element) { return element.second == find; });
							if (pa != top_basher_winners.end()) {
								int place = top_basher_winners[pa - top_basher_winners.begin()].first;
								int give_premium = (place == 1 ? 2000 : (place == 2 ? 1500 : (place == 3 ? 1000 : 50))), give_token = (place == 1 ? 50 : (place == 2 ? 25 : (place == 3 ? 15 : 0))), give_medal = (place == 1 ? 6094 : (place == 2 ? 6132 : (place == 3 ? 6130 : 0))), give_count = 1;
								pInfo(peer)->gtwl += give_premium;
								gamepacket_t p, p2;
								p.Insert("OnAddNotification"), p.Insert("interface/guide_arrow.rttex"), p.Insert("You claimed your event reward!"), p.Insert("audio/piano_nice.wav.wav"), p.Insert(0), p.CreatePacket(peer);
								p2.Insert("OnConsoleMessage"), p2.Insert("Thanks for participating in our event, you ended up being #" + to_string(place) + " (Your prize: `5" + setGems(give_premium) + " Premium GTPS World Locks``" + (give_medal != 0 ? ", `51 " + items[give_medal].ori_name + "``" : "") + "" + (give_token != 0 ? ", `5" + to_string(give_token) + " Growtoken``" : "") + ""), p2.CreatePacket(peer);
								if (give_token != 0) modify_inventory(peer, 1486, give_token);
								if (give_medal != 0) modify_inventory(peer, give_medal, give_count);
								top_basher_winners.erase(pa);
							}
							break;
						}
						else if (cch == "action|battlepasspopup\n") {
							gamepacket_t p(550);
							p.Insert("OnDialogRequest");
							int growpassid = 6124;
							if (today_day == pInfo(peer)->gd) growpassid = 6292;
							if (pInfo(peer)->gp == 1) p.Insert("set_default_color|`o\nadd_label_with_icon|big|Grow Pass Rewards|left|9222|\nadd_smalltext|`9You can claim your daily reward everyday here.``|left|\nadd_button_with_icon|claim_reward||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|claim_reward||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|claim_reward||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|claim_reward||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|claim_reward||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon||END_LIST|noflags|0||\nadd_spacer|small|\nend_dialog|worlds_list||Back|\nadd_quick_exit|\n");
							else p.Insert("set_default_color|`o\nadd_label_with_icon|big|Grow Pass Rewards|left|9222|\nadd_button|deposit|`2Purchase``|noflags|0|0|\nadd_smalltext|`4You must purchase the Grow Pass role to claim your prize!``|left|\nadd_button_with_icon|||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon||END_LIST|noflags|0||\nadd_spacer|small|\nend_dialog|||Back|\nadd_quick_exit|\n");
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|storenavigate\nitem|main\nselection|deposit\n" || cch == "action|storenavigate\nitem|locks\nselection|upgrade_backpack\n" || cch == "action|storenavigate\nitem|main\nselection|bonanza\n" || cch == "action|storenavigate\nitem|main\nselection|calendar\n" || cch == "action|store\nlocation|bottommenu\n" || cch == "action|store\nlocation|gem\n" || cch == "action|store\nlocation|pausemenu\n" || cch == "action|storenavigate\nitem|main\nselection|gems_rain\n") {
							if (cch == "action|store\nlocation|bottommenu\n" || cch == "action|store\nlocation|gem\n" || cch == "action|store\nlocation|pausemenu\n") shop_tab(peer, "tab1");
							if (cch == "action|storenavigate\nitem|main\nselection|gems_rain\n") shop_tab(peer, "tab1_1");
							if (cch == "action|storenavigate\nitem|main\nselection|calendar\n") shop_tab(peer, "tab1_2");
							if (cch == "action|storenavigate\nitem|main\nselection|bonanza\n")	shop_tab(peer, "tab1_3");
							if (cch == "action|storenavigate\nitem|locks\nselection|upgrade_backpack\n") shop_tab(peer, "tab2_1");
							if (cch == "action|storenavigate\nitem|main\nselection|deposit\n") SendCmd(peer, "/deposit", true);
							break;
						}
						/*
						else if (cch == "action|dialog_return\ndialog_name|phoenix_returns\n") {
						shop_tab(peer, "tab1_1");
						break;
						}
						else if (cch == "action|showphoenixreturns\n") {
						gamepacket_t p(550);
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wThe Phoenix Rising!``|left|11038|\nadd_spacer|small|\nadd_textbox|Rising from the ashes are the Phoenix items of the past.|left|\nadd_spacer|small|\nadd_textbox|During the event, Phoenix items can only be obtained from `2Summer Artifact Chest``.|left|\nadd_spacer|small|\nadd_textbox|The available Phoenix items are as follows:|left|\nadd_spacer|small|\nadd_label_with_icon|small| - "+to_string(find_phoenix_item(1674)) + " / 1 Found|left|1674|\nadd_label_with_icon|small| - " + to_string(find_phoenix_item(2854)) + " / 1 Found|left|2854|\nadd_label_with_icon|small| - " + to_string(find_phoenix_item(3696)) + " / 1 Found|left|3696|\nadd_label_with_icon|small| - " + to_string(find_phoenix_item(4820)) + " / 1 Found|left|4820|\nadd_label_with_icon|small| - " + to_string(find_phoenix_item(6312)) + " / 1 Found|left|6312|\nadd_label_with_icon|small| - " + to_string(find_phoenix_item(8588)) + " / 1 Found|left|8588|\nadd_spacer|small|\nadd_textbox|The amount available is updated every 24 hours. If an item isn't found in that time it won't be added to the total available.|left|\nadd_spacer|small|\nend_dialog|phoenix_returns||Back|");
						p.CreatePacket(peer);
						break;
						}*/
						else if (cch.find("action|buy") != string::npos) {
							if (pInfo(peer)->world == "") break;
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 3) break;
							string item = explode("\n", t_[2])[0];
							int price = 0, free = get_free_slots(pInfo(peer)), slot = 1, getcount = 0, get_counted = 0, random_pack = 0, token = 0;
							gamepacket_t p2;
							p2.Insert("OnStorePurchaseResult");
							if (item == "main") shop_tab(peer, "tab1");
							else if (item == "locks") shop_tab(peer, "tab2");
							else if (item == "itempack") shop_tab(peer, "tab3");
							else if (item == "bigitems") shop_tab(peer, "tab4");
							else if (item == "weather") shop_tab(peer, "tab5");
							else if (item == "token") shop_tab(peer, "tab6");
							else if (item == "upgrade_backpack") {
								price = 100 * ((((pInfo(peer)->inv.size() - 17) / 10) * ((pInfo(peer)->inv.size() - 17) / 10)) + 1);
								if (price > pInfo(peer)->gems) {
									packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
									p2.Insert("You can't afford `0Upgrade Backpack`` (`w10 Slots``)!  You're `$" + setGems(price - pInfo(peer)->gems) + "`` Gems short.");
								}
								else {
									if (pInfo(peer)->inv.size() < 496) {
										{
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("You've purchased `0Upgrade Backpack`` (`010 Slots``) for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems - price) + "`` Gems left.");
											p.CreatePacket(peer);
										}
										p2.Insert("You've purchased `0Upgrade Backpack (10 Slots)`` for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems - price) + "`` Gems left.\n\n`5Received: ```0Backpack Upgrade``\n");
										{
											packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
											gamepacket_t p;
											p.Insert("OnSetBux");
											p.Insert(pInfo(peer)->gems -= price), p.Insert(0), p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
											if (pInfo(peer)->supp >= 2) {
												p.Insert((float)33796, (float)1, (float)0);
											}
											p.CreatePacket(peer);
										}
										for (int i_ = 0; i_ < 10; i_++) { // default inv dydis
											Items itm_{};
											itm_.id = 0, itm_.count = 0;
											pInfo(peer)->inv.push_back(itm_);
										}
										send_inventory(peer);
										update_clothes(peer);
										shop_tab(peer, "tab2");
									}
								}
								p2.CreatePacket(peer);
							}
							else {
								vector<int> list;
								vector<vector<int>> itemai;
								string item_name = "";
								if (item == "superhero") item += to_string(rand() % 4 + 1);
								ifstream ifs("db/shop/-" + item + ".json");
								if (ifs.is_open()) {
									json j;
									ifs >> j;
									price = j["g"].get<int>();
									item_name = j["p"].get<string>();
									if (j.find("itemai") != j.end()) { // mano sistema
										if (pInfo(peer)->gems < price) {
											packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
											p2.Insert("You can't afford `o" + item_name + "``!  You're `$" + setGems(price - pInfo(peer)->gems) + "`` Gems short."), p2.CreatePacket(peer);
											break;
										}
										itemai = j["itemai"].get<vector<vector<int>>>();
										int reik_slots = itemai.size();
										int turi_slots = get_free_slots(pInfo(peer));
										for (vector<int> item_info : itemai) {
											int turi_dabar = 0;
											modify_inventory(peer, item_info[0], turi_dabar);
											if (turi_dabar != 0) reik_slots--;
											if (turi_dabar + item_info[1] > 200) goto fail;
										}
										if (turi_slots < reik_slots) goto fail;
										{
											//if (item == "g4good_Gem_Charity") daily_quest(peer, false, "donate_gems", 0);
											if (pInfo(peer)->grow4good_gems <= 100000) daily_quest(peer, false, "gems", price);
											pInfo(peer)->gems -= price;
											gamepacket_t gem_upd;
											gem_upd.Insert("OnSetBux"), gem_upd.Insert(pInfo(peer)->gems), gem_upd.Insert(0), gem_upd.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
											if (pInfo(peer)->supp >= 2) {
												gem_upd.Insert((float)33796, (float)1, (float)0);
											}
											gem_upd.CreatePacket(peer);
											vector<string> received_items{}, received_items2{};
											for (vector<int> item_info : itemai) {
												uint32_t item_id = item_info[0];
												int item_count = item_info[1];
												modify_inventory(peer, item_id, item_count);
												if (item_id > items.size()) break;
												received_items.push_back("Got " + to_string(item_info[1]) + " `#" + items[item_id].ori_name + "``."), received_items2.push_back(to_string(item_info[1]) + " " + items[item_id].ori_name);
											}
											packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
											//if (item == "arm_guy") daily_quest(peer, false, "purchase_waving", 0);
											gamepacket_t p_;
											p_.Insert("OnConsoleMessage"), p_.Insert("You've purchased `o" + item_name + "`` for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems) + "`` Gems left." + "\n" + join(received_items, "\n")), p_.CreatePacket(peer);
											p2.Insert("You've purchased `o" + item_name + "`` for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems) + "`` Gems left." + "\n\n`5Received: ``" + join(received_items2, ", ") + "\n"), p2.CreatePacket(peer);
											break;
										}
									fail:
										packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
										p2.Insert("You don't have enough space in your inventory to buy that. You may be carrying to many of one of the items you are trying to purchase or you don't have enough free spaces to fit them all in your backpack!");
										p2.CreatePacket(peer);
										break;
									}
									list = j["i"].get<vector<int>>();
									getcount = j["h"].get<int>();
									get_counted = j["h"].get<int>();
									slot = j["c"].get<int>();
									token = j["t"].get<int>();
									random_pack = j["random"].get<int>();
									int totaltoken = 0, tokencount = 0, mega_token = 0, inventoryfull = 0;
									modify_inventory(peer, 1486, tokencount);
									modify_inventory(peer, 6802, mega_token);
									totaltoken = tokencount + (mega_token * 100);
									vector<pair<int, int>> receivingitems;
									if (token == 0 ? price > pInfo(peer)->gems : token > totaltoken) {
										packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
										p2.Insert("You can't afford `o" + item_name + "``!  You're `$" + (token == 0 ? "" + setGems(price - pInfo(peer)->gems) + "`` Gems short." : "" + setGems(token - totaltoken) + "`` Growtokens short."));
									}
									else {
										if (free >= slot) {
											string received = "", received2 = "";
											if (item == "basic_splice") {
												slot++;
												receivingitems.push_back(make_pair(11, 10));
											}
											if (item == "race_packa") {
												slot++;
												receivingitems.push_back(make_pair(11, 10));
											}
											for (int i = 0; i < slot; i++) receivingitems.push_back(make_pair((random_pack == 1 ? list[rand() % list.size()] : list[i]), getcount));
											for (int i = 0; i < slot; i++) {
												int itemcount = 0;
												modify_inventory(peer, receivingitems[i].first, itemcount);
												if (itemcount + getcount > 200) inventoryfull = 1;
											}
											if (inventoryfull == 0) {
												int i = 0;
												for (i = 0; i < slot; i++) {
													received += (i != 0 ? ", " : "") + items[receivingitems[i].first].ori_name;
													received2 += "Got " + to_string(receivingitems[i].second) + " `#" + items[receivingitems[i].first].ori_name + "``." + (i == (slot - 1) ? "" : "\n") + "";
													modify_inventory(peer, receivingitems[i].first, receivingitems[i].second);
												}
											}
											else {
												packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
												p2.Insert("You don't have enough space in your inventory that. You may be carrying to many of one of the items you are trying to purchase or you don't have enough free spaces to fit them all in your backpack!");
												p2.CreatePacket(peer);
												break;
											}
											{
												gamepacket_t p;
												p.Insert("OnConsoleMessage");
												p.Insert("You've purchased `o" + received + "`` for `$" + (token == 0 ? "" + setGems(price) + "`` Gems." : "" + setGems(token) + "`` Growtokens.") + "\nYou have `$" + (token == 0 ? "" + setGems(pInfo(peer)->gems - price) + "`` Gems left." : "" + setGems(totaltoken - token) + "`` Growtokens left.") + "\n" + received2);
												p.CreatePacket(peer);
											}
											p2.Insert("You've purchased `o" + received + "`` for `$" + (token == 0 ? "" + setGems(price) + "`` Gems." : "" + setGems(token) + "`` Growtokens.") + "\nYou have `$" + (token == 0 ? "" + setGems(pInfo(peer)->gems - price) + "`` Gems left." : "" + setGems(totaltoken - token) + "`` Growtokens left.") + "\n\n`5Received: ``" + (get_counted <= 1 ? "" : "`0" + to_string(get_counted)) + "`` " + received + "\n"), p2.CreatePacket(peer);
											if (token == 0) {
												pInfo(peer)->gems -= price;
												gamepacket_t p;
												p.Insert("OnSetBux");
												p.Insert(pInfo(peer)->gems);
												p.Insert(0);
												p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
												if (pInfo(peer)->supp >= 2) {
													p.Insert((float)33796, (float)1, (float)0);
												}
												p.CreatePacket(peer);
											}
											else {
												if (tokencount >= token) modify_inventory(peer, 1486, token *= -1);
												else {
													modify_inventory(peer, 1486, tokencount *= -1);
													modify_inventory(peer, 6802, mega_token *= -1);
													int givemegatoken = (totaltoken - token) / 100;
													int givetoken = (totaltoken - token) - (givemegatoken * 100);
													modify_inventory(peer, 1486, givetoken);
													modify_inventory(peer, 6802, givemegatoken);
												}
											}
											packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										}
										else {
											packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
											p2.Insert(slot > 1 ? "You'll need " + to_string(slot) + " slots free to buy that! You have " + to_string(free) + " slots." : "You don't have enough space in your inventory that. You may be carrying to many of one of the items you are trying to purchase or you don't have enough free spaces to fit them all in your backpack!");
										}
									}
								}
								else {
									packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
									p2.Insert("This item was not found. Server error.");
									p2.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch == "action|AccountSecurity\nlocation|pausemenu\n") {
							gamepacket_t p(500);
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wAdvanced Account Protection ``|left|3732|\nadd_textbox|`1You are about to enable the Advanced Account Protection.``|left|\nadd_textbox|`1After that, every time you try to log in from a new device and IP you will receive an email with a login confirmation link.``|left|\nadd_textbox|`1This will significantly increase your account security.``|left|\nend_dialog|secureaccount|Cancel|Ok|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|respawn") != string::npos) {
						SendRespawn(peer, false, 0, (cch.find("action|respawn_spike") != string::npos) ? false : true);
                          }
						else if (cch == "action|refresh_item_data\n") {
						if (pInfo(peer)->bypass) {
							login_count++;
							if (login_count < 10) {
								packet_(peer, "action|log\nmsg|One moment, updating item data...");
								sendItemDataToPeer(peer);
								/*
								if (pInfo(peer)->world.empty()) {
									//enet_peer_send(peer, 0, enet_packet_create((pInfo(peer)->upload ? rat_data : item_data), (pInfo(peer)->upload ? rat_data_size : item_data_size) + 60, ENET_PACKET_FLAG_RELIABLE));
									if (pInfo(peer)->upload) {
										enet_peer_send(peer, 0, enet_packet_create(item_data, item_data_size + 60, ENET_PACKET_FLAG_RELIABLE));
										pInfo(peer)->upload = false;
									}
									enet_peer_disconnect_later(peer, 0);
								}*/
								enet_host_flush(server);
							}
							else packet_(peer, "action|log\nmsg|CT:[S]_ `4OOPS:`` Too many people logging in at once. Please press `5CANCEL`` and try again in a few seconds.", "");
							enet_peer_disconnect_later(peer, 0);
						}
						break;
							}
						else if (cch == "action|enter_game\n") {
						if (pInfo(peer)->bypass) {
							pInfo(peer)->enter_game++;
							if (pInfo(peer)->world == "" && pInfo(peer)->enter_game == 1) {
								if (pInfo(peer)->tankIDName.empty()) {
									gamepacket_t p(250);
									p.Insert("OnDialogRequest"), p.Insert(r_dialog("")), p.CreatePacket(peer);
								}
								else {
									pInfo(peer)->tmod = pInfo(peer)->mod;
									pInfo(peer)->name_color = (pInfo(peer)->tankIDName == "chris" ? "`5@Dr. " : (pInfo(peer)->superdev == 1) ? "`6@" : (pInfo(peer)->dev == 1) ? "`9@" : (pInfo(peer)->tmod == 1) ? "`#@" : "`0");
									string thetag = (pInfo(peer)->mod || pInfo(peer)->dev ? "@" : "");
									if (pInfo(peer)->drt) pInfo(peer)->d_name = (pInfo(peer)->drt ? "`4" + thetag : pInfo(peer)->name_color) + (pInfo(peer)->drt ? "Dr." : "") + pInfo(peer)->tankIDName + (pInfo(peer)->is_legend ? " of Legend" : "");

									int on_ = 0;
									{
										vector<string> friends_;
										gamepacket_t p;
										p.Insert("OnConsoleMessage"), p.Insert("`3FRIEND ALERT:`` " + (pInfo(peer)->name_color == "`0" ? "`o" : pInfo(peer)->name_color) + pInfo(peer)->tankIDName + "`` has `2logged on``.");
										for (int c_ = 0; c_ < pInfo(peer)->friends.size(); c_++) friends_.push_back(pInfo(peer)->friends[c_].name);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (find(friends_.begin(), friends_.end(), pInfo(currentPeer)->tankIDName) != friends_.end()) {
												if (not pInfo(peer)->invis and not pInfo(peer)->m_h) {
													if (pInfo(currentPeer)->show_friend_notifications_) packet_(currentPeer, "action|play_sfx\nfile|audio/friend_logon.wav\ndelayMS|0"), p.CreatePacket(currentPeer);
												}
												on_++;
											}
										}
									}
									form_emoji(peer);
									/*
									{
										gamepacket_t p;
										p.Insert("OnEmoticonDataChanged");
										p.Insert(151749376);
										string other = "";
										for (int i = 0; i < pInfo(peer)->gr.size(); i++) other += pInfo(peer)->gr[i];
										p.Insert(other + "" + (pInfo(peer)->supp == 2 ? "(yes)|Ă|1" : "(yes)|Ă|0") + "&" + (pInfo(peer)->supp != 0 ? "(no)|ă|1" : "(no)|ă|0") + "&" + (pInfo(peer)->supp == 2 ? "(love)|Ą|1" : "(love)|Ą|0") + "&" + (pInfo(peer)->supp != 0 ? "(shy)|Ć|1&(wink)|ć|1" : "(shy)|Ć|0&(wink)|ć|0") + "&" + (pInfo(peer)->level >= 5 ? "(tongue)|Ĉ|1" : "(tongue)|Ĉ|0") + "&" + (pInfo(peer)->friends.size() >= 20 ? "(agree)|ĉ|1" : "(agree)|ĉ|0") + "&" + (pInfo(peer)->supp != 0 ? "(music)|Č|1" : "(music)|Č|0") + "&" + (pInfo(peer)->friends.size() >= 50 ? "(build)|č|1" : "(build)|č|0") + "&" + (pInfo(peer)->supp == 2 ? "(megaphone)|Ď|1" : "(megaphone)|Ď|0") + "&" + (pInfo(peer)->level >= 5 ? "(sigh)|ď|1&(mad)|Đ|1&(wow)|đ|1" : "(sigh)|ď|0&(mad)|Đ|0&(wow)|đ|0") + "&" + (pInfo(peer)->friends.size() >= 40 ? "(dance)|Ē|1" : "(dance)|Ē|0") + "&" + (pInfo(peer)->friends.size() >= 30 ? "(see-no-evil)|ē|1" : "(see-no-evil)|ē|0") + "&" + (pInfo(peer)->supp == 2 ? "(heart)|ĕ|1" : "(heart)|ĕ|0") + "&" + (pInfo(peer)->friends.size() >= 10 ? "(kiss)|Ę|1" : "(kiss)|Ę|0") + "&" + (pInfo(peer)->supp != 0 ? "(lol)|Ě|1" : "(lol)|Ě|1") + "&" + (pInfo(peer)->level >= 5 ? "(smile)|Ā|1" : "(smile)|Ā|0") + "&" + (pInfo(peer)->supp == 2 ? "(cool)|Ĝ|1" : "(cool)|Ĝ|0") + "&(lucky)|ĳ|1&");
										p.CreatePacket(peer);
									}*/
									if (today_day != pInfo(peer)->grow4good_day) {
										pInfo(peer)->grow4good_day = today_day;
										pInfo(peer)->grow4good_30mins = (rand() % 3 < 1 ? 0 : -1), pInfo(peer)->grow4good_surgery = (rand() % 3 < 1 ? 0 : -1), pInfo(peer)->grow4good_fish = (rand() % 3 < 1 ? 0 : -1), pInfo(peer)->grow4good_place = (rand() % 3 < 1 ? 0 : -1), pInfo(peer)->grow4good_break = (rand() % 3 < 1 ? 0 : -1), pInfo(peer)->grow4good_trade = (rand() % 3 < 1 ? 0 : -1), pInfo(peer)->grow4good_sb = (rand() % 3 < 1 ? 0 : -1), pInfo(peer)->grow4good_enter = false;
										pInfo(peer)->grow4good_provider = (rand() % 3 < 1 ? 0 : -1);
										pInfo(peer)->grow4good_provider2 = (pInfo(peer)->grow4good_provider == -1 ? -1 : rand() % 450 + 1);
										pInfo(peer)->grow4good_geiger = (rand() % 3 < 1 ? 0 : -1);
										pInfo(peer)->grow4good_geiger2 = (pInfo(peer)->grow4good_geiger == -1 ? -1 : rand() % 7 + 1);
									}
									struct tm newtime;
									time_t now = time(0);
									localtime_s(&newtime, &now);
									if (newtime.tm_yday - pInfo(peer)->grow4good_week >= 7) {
										pInfo(peer)->grow4good_week = newtime.tm_yday;
										pInfo(peer)->grow4good_seed = small_seed_pack[rand() % small_seed_pack.size()];
										pInfo(peer)->grow4good_seed2 = 0, pInfo(peer)->grow4good_combine = 0;
									}
									if (newtime.tm_yday - pInfo(peer)->grow4good_month >= 30) {
										pInfo(peer)->grow4good_month = newtime.tm_yday;
										pInfo(peer)->grow4good_crystal = 0;
										pInfo(peer)->grow4good_gems = 0;
									}
									if (pInfo(peer)->grow4good_gems == -1) pInfo(peer)->grow4good_gems = 0;
									if (pInfo(peer)->surgery_type == -1) pInfo(peer)->surgery_type = rand() % 31;
									SendReceive(peer);
									/*
									{
										if (pInfo(peer)->pinata_day != today_day) {
											pInfo(peer)->pinata_prize = false;
											pInfo(peer)->pinata_claimed = false;
										}
										gamepacket_t p;
										p.Insert("OnProgressUISet"), p.Insert(1), p.Insert(0), p.Insert(to_string(pInfo(peer)->pinata_claimed)), p.Insert(1), p.Insert(""), p.Insert(to_string(pInfo(peer)->pinata_prize)), p.CreatePacket(peer);
									}*/
									/*
									{
										gamepacket_t p;
										p.Insert("OnProgressUISet"), p.Insert(1), p.Insert(836), p.Insert(pInfo(peer)->summer_surprise), p.Insert(20), p.Insert(""), p.Insert(1);
										p.CreatePacket(peer);
									}*/
									if (pInfo(peer)->inventory_sent == false) send_inventory(peer);
									gamepacket_t p1, p2, p3/*, p5*/;
									p1.Insert("OnConsoleMessage"), p1.Insert("Welcome back, `w" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->name_color + pInfo(peer)->tankIDName) + "````." + (pInfo(peer)->friends.size() == 0 ? "" : (on_ != 0 ? " `w" + to_string(on_) + "`` friend is online." : " No friends are online."))), p1.CreatePacket(peer);
									p2.Insert("OnConsoleMessage"), p2.Insert("`2Goals event``: Start working on your goals, you can be farmer, geiger hunter, take providers to receive prizes for leveling up. You will be able to claim your rewards at the end of the week!"), p2.CreatePacket(peer);
									p3.Insert("OnConsoleMessage"), p3.Insert("`3Limited time event``: 3x Gems from breaking blocks, Trees time reduced by 50%, Providers time reduced by 50%!"), p3.CreatePacket(peer);
									{
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										if (thedaytoday == 1) p.Insert("`3Today is Trees Day!`` 50% higher chance to get `2extra block`` from harvesting tree.");
										else if (thedaytoday == 2) p.Insert("`3Today is Breaking Day!`` 15% higher chance to get `2extra seed``.");
										else if (thedaytoday == 3) p.Insert("`3Today is Geiger Day!`` Higher chance of getting a `2better Geiger prize`` & Irradiated mod will last only `210 minutes``.");
										else if (thedaytoday == 4) p.Insert("`3Today is Tasks Day!`` Get extra `210`` points for completing a task.");
										else if (thedaytoday == 5) p.Insert("`3Today is Gems Day!`` 50% higher chance to get `2extra`` gem drop.");
										else if (thedaytoday == 6) p.Insert("`3Today is Surgery Day!`` Malpractice takes `215 minutes`` and Recovering takes `230 minutes`` & receive `2different prizes``.");
										else if (thedaytoday == 0) p.Insert("`3Today is Fishing Day!`` Catch a fish and receive `2extra lb``.");
										p.CreatePacket(peer);
									}
								//p5.Insert("OnConsoleMessage"), p5.Insert("`2It's Assassin's Creed 15th Anniversay Weekend in Growtopia! There are some awesome deals in the store check them out!``"), p5.CreatePacket(peer);
								if (pInfo(peer)->tankIDName == "Raid" || pInfo(peer)->tankIDName == "Jase" || pInfo(peer)->tankIDName == "Raijin" || pInfo(peer)->tankIDName == "xan") {
									if (get_free_slots(pInfo(peer)) >= 1) {
										if (today_day != pInfo(peer)->mds) {
											pInfo(peer)->mds = today_day;
											vector<int> list2{ 2590 };
											int receive = 25, item = list2[rand() % list2.size()], got = 25;
											modify_inventory(peer, item, receive);
											gamepacket_t p, p2;
											p.Insert("OnConsoleMessage"), p.Insert("appreciation bonus (feel free keep, trade, or use for prizes) for today is:"), p.CreatePacket(peer);
											p2.Insert("OnConsoleMessage"), p2.Insert("Given `0" + to_string(got) + " " + items[item].name + "``."), p2.CreatePacket(peer);
										}
									}
								}
									if (pInfo(peer)->mod + pInfo(peer)->dev + pInfo(peer)->vip >= 1) {
										if (get_free_slots(pInfo(peer)) >= 1) {
											if (today_day != pInfo(peer)->mds) {
												pInfo(peer)->mds = today_day;
												vector<int> list2{ 408, 274, 276 };
												int receive = 1, item = list2[rand() % list2.size()], got = 1;
												modify_inventory(peer, item, receive);
												gamepacket_t p, p2;
												p.Insert("OnConsoleMessage"), p.Insert("Your mod appreciation bonus (feel free keep, trade, or use for prizes) for today is:"), p.CreatePacket(peer);
												p2.Insert("OnConsoleMessage"), p2.Insert("Given `0" + to_string(got) + " " + items[item].name + "``."), p2.CreatePacket(peer);
											}
										}
									}
									world_menu(peer);
									news(peer);
								}
							}
							else enet_peer_disconnect_later(peer, 0);
						}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|account_security\nchange|") != string::npos) {
							string change = cch.substr(57, cch.length() - 58).c_str();
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (change == "email") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|Having an up-to-date email address attached to your account is a great step toward improved account security.|left|\nadd_smalltext|Email: `5" + pInfo(peer)->email + "``|left|\nadd_spacer|small|\nadd_smalltext|Type your new `5email address``|left|\nadd_text_input|change|||50|\nend_dialog|change_email|OK|Continue|\n");
							else if (change == "password") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|A hacker may attempt to access your account more than once over a period of time.|left|\nadd_smalltext|Changing your password `2often reduces the risk that they will have frequent access``.|left|\nadd_spacer|small|\nadd_smalltext|Type your new `5password``|left|\nadd_text_input|change|||18|\nend_dialog|change_password|OK|Continue|\n");
							if (change == "email" or change == "password") p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|change_email\nchange|") != string::npos) {
							string change = cch.substr(53, cch.length() - 54).c_str();
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (change == "") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|Having an up-to-date email address attached to your account is a great step toward improved account security.|left|\nadd_smalltext|Email: `5" + pInfo(peer)->email + "``|left|\nadd_spacer|small|\nadd_smalltext|Type your new `5email address``|left|\nadd_text_input|change|||50|\nend_dialog|change_email|OK|Continue|\n");
							else {
								pInfo(peer)->email = change;
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|Having an up-to-date email address attached to your account is a great step toward improved account security.|left|\nadd_smalltext|Your new Email: `5" + pInfo(peer)->email + "``|left|\nadd_spacer|small|\nend_dialog|changedemail|OK||\n");
							}
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|change_password\nchange|") != string::npos) {
							string change = cch.substr(56, cch.length() - 57).c_str();
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (change == "") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|A hacker may attempt to access your account more than once over a period of time.|left|\nadd_smalltext|Changing your password `2often reduces the risk that they will have frequent access``.|left|\nadd_spacer|small|\nadd_smalltext|Type your new `5password``|left|\nadd_text_input|change|||18|\nend_dialog|change_password|OK|Continue|\n");
							else {
								{
									gamepacket_t p;
									p.Insert("SetHasGrowID"), p.Insert(1), p.Insert(pInfo(peer)->tankIDName), p.Insert(pInfo(peer)->tankIDPass = change);
									p.CreatePacket(peer);
								}
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|A hacker may attempt to access your account more than once over a period of time.|left|\nadd_smalltext|Changing your password `2often reduces the risk that they will have frequent access``.|left|\nadd_smalltext|Your new password: `5" + pInfo(peer)->tankIDPass + "``|left|\nadd_spacer|small|\nend_dialog|changedpassword|OK||\n");
							}
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|world_swap\nname_box|") != string::npos) {
							string world = cch.substr(53, cch.length() - 54).c_str(), currentworld = pInfo(peer)->world;
							int got = 0;
							transform(world.begin(), world.end(), world.begin(), ::toupper);
							if (not check_blast(world) || currentworld == world) {
								gamepacket_t p;
								p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSwap World Names``|left|2580|\nadd_textbox|`4World swap failed - you don't own both worlds!``|left|\nadd_smalltext|This will swap the name of the world you are standing in with another world `4permanently``.  You must own both worlds, with a World Lock in place.<CR>Your `wChange of Address`` will be consumed if you press `5Swap 'Em``.|left|\nadd_textbox|Enter the other world's name:|left|\nadd_text_input|name_box|||32|\nadd_spacer|small|\nend_dialog|world_swap|Cancel|Swap 'Em!|"), p.CreatePacket(peer);
							}
							else create_address_world(peer, world, currentworld);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool") != string::npos) {
						if (pInfo(peer)->surgery_started) {
							int count = atoi(cch.substr(59, cch.length() - 59).c_str());
							if (count == 999) end_surgery(peer);
							else load_surgery(peer, count);
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|2646\nID|") != string::npos) {
							int netID = atoi(cch.substr(41, cch.length() - 41).c_str());
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								string new_spotlight = "";
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
									if (block_->heart_monitor == pInfo(currentPeer)->tankIDName || pInfo(currentPeer)->netID == netID) {
										if (pInfo(currentPeer)->netID == netID) {
											new_spotlight = pInfo(currentPeer)->tankIDName, pInfo(currentPeer)->spotlight = true;
											gamepacket_t p;
											p.Insert("OnConsoleMessage"), p.Insert("All eyes are on you! (`$In the Spotlight`` mod added)"), p.CreatePacket(currentPeer);
										}
										else {
											gamepacket_t p;
											p.Insert("OnConsoleMessage"), p.Insert("Back to anonymity. (`$In the Spotlight`` mod removed)"), p.CreatePacket(currentPeer);
											pInfo(currentPeer)->spotlight = false;
										}
										if (new_spotlight != "") for (int i_ = 0; i_ < world_->blocks.size(); i_++) if (world_->blocks[i_].heart_monitor == new_spotlight) world_->blocks[i_].heart_monitor = "";
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You shine the light on " + (new_spotlight == pInfo(peer)->tankIDName ? "yourself" : new_spotlight) + "!"), p.CreatePacket(peer);
										update_clothes(currentPeer, true);
									}
								}
								block_->heart_monitor = new_spotlight;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|grinder\ncount|") != string::npos) {
							int count = atoi(cch.substr(47, cch.length() - 47).c_str()), item = pInfo(peer)->lastchoosenitem, got = 0;
							modify_inventory(peer, item, got);
							if (items[item].grindable_count == 0 || got == 0 || count <= 0 || count * items[item].grindable_count > got) break;
							int remove = (count * items[item].grindable_count) * -1;
							modify_inventory(peer, item, remove);
							gamepacket_t p, p2;
							p.Insert("OnConsoleMessage"), p.Insert("Ground up " + to_string(count * items[item].grindable_count) + " " + items[item].name + " into " + to_string(count) + " " + items[items[item].grindable_prize].name + "!"), p.CreatePacket(peer);
							p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("Ground up " + to_string(count * items[item].grindable_count) + " " + items[item].name + " into " + to_string(count) + " " + items[items[item].grindable_prize].name + "!"), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
							{
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									PlayerMoving data_{};
									data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16, data_.packetType = 19, data_.plantingTree = 500, data_.punchX = items[item].grindable_prize, data_.punchY = pInfo(peer)->netID;
									int32_t to_netid = pInfo(peer)->netID;
									BYTE* raw = packPlayerMoving(&data_);
									raw[3] = 5;
									memcpy(raw + 8, &to_netid, 4);
									send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									delete[] raw;
									int c_ = count;
									if (modify_inventory(peer, items[item].grindable_prize, c_) != 0) {
										WorldDrop drop_block_{};
										drop_block_.id = items[item].grindable_prize, drop_block_.count = count, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = pInfo(peer)->x + rand() % 17, drop_block_.y = pInfo(peer)->y + rand() % 17;
										dropas_(world_, drop_block_);
									}
									{
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 221, data_.YSpeed = 221, data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16, data_.XSpeed = item;
										BYTE* raw = packPlayerMoving(&data_);
										send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										delete[] raw;
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|s_claimreward") != string::npos) {
							int reward = atoi(cch.substr(72, cch.length() - 72).c_str()), lvl = 0, count = 1;
							vector<int> list{ 6900, 6982, 6212, 3172, 9068, 6912, 10836, 5142, 3130, 8284 };
							if (reward <= 0 || reward > list.size()) break;
							if (list[reward - 1] == 10836) count = 100;
							if (list[reward - 1] == 6212) count = 50;
							if (list[reward - 1] == 3172 || list[reward - 1] == 6912) count = 25;
							if (list[reward - 1] == 5142) count = 5;
							if (find(pInfo(peer)->surg_p.begin(), pInfo(peer)->surg_p.end(), lvl = reward * 5) == pInfo(peer)->surg_p.end()) {
								if (pInfo(peer)->s_lvl >= lvl) {
									if (modify_inventory(peer, list[reward - 1], count) == 0) {
										pInfo(peer)->surg_p.push_back(lvl);
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("Congratulations! You have received your Surgeon Reward!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										{
											PlayerMoving data_{};
											data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16, data_.packetType = 19, data_.plantingTree = 100, data_.punchX = list[reward - 1], data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
										reward_show(peer, "surgery");
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have full inventory space!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|zombie_purchase\nbuttonClicked|zomb_item_") != string::npos) {
							int item = pInfo(peer)->lockeitem;
							if (item <= 0 || item >= items.size() || items[item].zombieprice == 0) continue;
							int allwl = 0, wl = 0, dl = 0, price = items[item].zombieprice;
							modify_inventory(peer, 4450, wl);
							modify_inventory(peer, 4452, dl);
							allwl = wl + (dl * 100);
							if (allwl >= price) {
								int c_ = 1;
								if (modify_inventory(peer, item, c_) == 0) {
									if (wl >= price) modify_inventory(peer, 4450, price *= -1);
									else {
										modify_inventory(peer, 4450, wl *= -1);
										modify_inventory(peer, 4452, dl *= -1);
										int givedl = (allwl - price) / 100;
										int givewl = (allwl - price) - (givedl * 100);
										modify_inventory(peer, 4450, givewl);
										modify_inventory(peer, 4452, givedl);
									}
									PlayerMoving data_{};
									data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16, data_.packetType = 19, data_.plantingTree = 500, data_.punchX = item, data_.punchY = pInfo(peer)->netID;
									int32_t to_netid = pInfo(peer)->netID;
									BYTE* raw = packPlayerMoving(&data_);
									raw[3] = 5;
									memcpy(raw + 8, &to_netid, 4);
									send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									delete[] raw;
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("`3You bought " + items[item].name + " for " + setGems(items[item].zombieprice) + " Zombie Brains."), p.CreatePacket(peer);
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("No inventory space."), p.CreatePacket(peer);
								}
							}
							else {
								gamepacket_t p;
								p.Insert("OnConsoleMessage"), p.Insert("`9You don't have enough Zombie Brains!``"), p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|zurgery_purchase\nbuttonClicked|zurg_item_") != string::npos) {
							int item = pInfo(peer)->lockeitem;
							if (item <= 0 || item >= items.size() || items[item].surgeryprice == 0) continue;
							int allwl = 0, wl = 0, dl = 0, price = items[item].surgeryprice;
							modify_inventory(peer, 4298, wl);
							modify_inventory(peer, 4300, dl);
							allwl = wl + (dl * 100);
							if (allwl >= price) {
								int c_ = 1;
								if (modify_inventory(peer, item, c_) == 0) {
									if (wl >= price) modify_inventory(peer, 4298, price *= -1);
									else {
										modify_inventory(peer, 4298, wl *= -1);
										modify_inventory(peer, 4300, dl *= -1);
										int givedl = (allwl - price) / 100;
										int givewl = (allwl - price) - (givedl * 100);
										modify_inventory(peer, 4298, givewl);
										modify_inventory(peer, 4300, givedl);
									}
									PlayerMoving data_{};
									data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16, data_.packetType = 19, data_.plantingTree = 500, data_.punchX = item, data_.punchY = pInfo(peer)->netID;
									int32_t to_netid = pInfo(peer)->netID;
									BYTE* raw = packPlayerMoving(&data_);
									raw[3] = 5;
									memcpy(raw + 8, &to_netid, 4);
									send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									delete[] raw;
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("`3You bought " + items[item].name + " for " + setGems(items[item].surgeryprice) + " Caduceus."), p.CreatePacket(peer);
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("No inventory space."), p.CreatePacket(peer);
								}
							}
							else {
								gamepacket_t p;
								p.Insert("OnConsoleMessage"), p.Insert("`9You don't have enough Caduceus!``"), p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worldreport\nreport_reason|") != string::npos) {
							if (pInfo(peer)->tankIDName == "") break;
							string report = cch.substr(59, cch.length() - 60).c_str();
							add_modlogs(peer, pInfo(peer)->name_color + pInfo(peer)->tankIDName, "`4reports:`` " + report, "");
							gamepacket_t p2;
							p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("Thank you for your report. Now leave this world so you don't get punished along with the scammers!"), p2.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|billboard_edit\nbillboard_item|") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							int billboard_item = atoi(explode("\n", t_[3])[0].c_str());
							if (billboard_item > 0 && billboard_item < items.size()) {
								int got = 0;
								modify_inventory(peer, billboard_item, got);
								if (got == 0) break;
								if (items[billboard_item].untradeable == 1 or billboard_item == 1424 or items[billboard_item].blockType == BlockTypes::LOCK or items[billboard_item].blockType == BlockTypes::FISH) {
									gamepacket_t p, p2;
									p.Insert("OnConsoleMessage"), p.Insert("Item can not be untradeable."), p.CreatePacket(peer);
									p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("Item can not be untradeable."), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
								}
								else {
									pInfo(peer)->b_i = billboard_item;
									if (pInfo(peer)->b_p != 0 && pInfo(peer)->b_i != 0) {
										gamepacket_t p(0, pInfo(peer)->netID);
										p.Insert("OnBillboardChange"), p.Insert(pInfo(peer)->netID), p.Insert(pInfo(peer)->b_i), p.Insert(pInfo(peer)->b_bill), p.Insert(pInfo(peer)->b_p), p.Insert(pInfo(peer)->b_w);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world) continue;
											p.CreatePacket(currentPeer);
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return") != string::npos) {
							call_dialog(peer, cch);
							break;
						}
					}
					else if (cch.find("action|dialog_return") != string::npos) {
						call_dialog(peer, cch);
						break;
					}
					break;
				}
				case 3: // world/enter
				{
					if (pInfo(peer)->bypass == false) break;
					if (pInfo(peer)->trading_with != -1) cancel_trade(peer, false);
					string cch = text_(event.packet);
					//cout << cch << endl;
					if (pInfo(peer)->lpps2 + 3400 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
						pInfo(peer)->pps2 = 0;
						pInfo(peer)->lpps2 = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
					}
					else {
						pInfo(peer)->pps2++;
						if (pInfo(peer)->pps2 >= 30) enet_peer_disconnect_later(peer, 0);
					}
					if (cch == "action|quit") enet_peer_disconnect_later(peer, 0);
					else if (cch == "action|quit_to_exit" && pInfo(peer)->world != "") exit_(peer);
					else if (cch == "action|gohomeworld\n" && pInfo(peer)->world == "") {
						if (pInfo(peer)->home_world == "") {
							gamepacket_t p, p2;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`9No Home World Set ``|left|1432|\nadd_spacer|small|\nadd_textbox|Use /sethome to assign the current world as your home world.|left|\nadd_spacer|small|\nend_dialog||OK||");
							p.CreatePacket(peer);
							p2.Insert("OnFailedToEnterWorld"), p2.CreatePacket(peer);
						}
						else {
							packet_(peer, "action|log\nmsg|Magically warping to home world `5" + pInfo(peer)->home_world + "``...");
							string world_name = pInfo(peer)->home_world;
							join_world(peer, world_name);
						}
					}
					else {
						if (pInfo(peer)->world == "") {
							if (cch.find("action|join_request") != string::npos) {
								if (pInfo(peer)->last_world_enter + 500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
									pInfo(peer)->last_world_enter = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
									vector<string> t_ = explode("|", cch);
									if (t_.size() < 3) break;
									string world_name = explode("\n", t_[2])[0];
									transform(world_name.begin(), world_name.end(), world_name.begin(), ::toupper);
									join_world(peer, world_name);
								}
							}
							else if (cch.find("action|world_button") != string::npos) {
								vector<string> t_ = explode("|", cch);
								if (t_.size() < 3) break;
								string dialog = explode("\n", t_[2])[0];
								if (dialog == "w1_") world_menu(peer, false);
								else {
									string c_active_worlds = "";
									if (dialog == "_catselect_") {
										c_active_worlds = pInfo(peer)->worlds_owned.size() != 0 ? "\nadd_heading|Your Worlds|" : "\nadd_heading|You don't have any worlds.<CR>|";
										for (int w_ = 0; w_ < (pInfo(peer)->worlds_owned.size() >= 32 ? 32 : pInfo(peer)->worlds_owned.size()); w_++) c_active_worlds += "\nadd_floater|" + pInfo(peer)->worlds_owned[w_] + "|0|0.38|2147418367";
									}
									else if (dialog == "w2_") c_active_worlds = a + "\nadd_heading|" + (top_list_world_menu.empty() ? "The list should update in few minutes" : "Top Worlds") + "|", c_active_worlds += top_list_world_menu;
									else {
										vector<string> worlds;
										if (dialog == "w3_") {
											c_active_worlds = "\nadd_heading|Information|";
											worlds = { "START", "ITEM", "WIKITOPIA", "WIKIPEDIA", "MASSHELP" };
										}
										else {
											c_active_worlds = "\nadd_heading|Shop|";
											worlds = { "LOCKETOWN", "M", "S", "WALMART", "VEND", "69", "PACK" };
										}
										for (uint8_t i = 0; i < worlds.size(); i++) c_active_worlds += "\nadd_floater|" + worlds[i] + "|" + to_string((i + 1) * -1) + "|0.38|3417414143";
									}
									gamepacket_t p;
									p.Insert("OnRequestWorldSelectMenu"), p.Insert("add_button|Random Worlds|w1_|0.7|3529161471|\nadd_button|Top Worlds|w2_|0.7|3529161471|\nadd_button|Information|w3_|0.7|3529161471|\nadd_button|Shop<CR>|w4_|0.7|3529161471|" + c_active_worlds), p.CreatePacket(peer);
								}
							}
						}
					}
					break;
				}
				case 4:
				{
					//auto start = chrono::steady_clock::now();
					if (pInfo(peer)->lpps23 + 1000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
						pInfo(peer)->pps23 = 0;
						pInfo(peer)->lpps23 = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
					}
					else {
						pInfo(peer)->pps23++;
						//if (pInfo(peer)->pps23 >= 360) {
						//	enet_peer_disconnect_later(peer, 0);
						//	break;
						//}
					}
					if (pInfo(peer)->world.empty()) break;
					BYTE* data_ = get_struct(event.packet);
					if (data_ == nullptr) break;
					PlayerMoving* p_ = unpackPlayerMoving(data_);
					switch (p_->packetType) {
					case 0: /*Kai zaidejas pajuda*/
					{
						int currentX = pInfo(peer)->x / 32;
						int currentY = pInfo(peer)->y / 32;
						int targetX = p_->x/32;
						int targetY = p_->y / 32;

						if (pInfo(peer)->hand == 3066) {
							if ((p_->punchX > 0 && p_->punchX < 100) && (p_->punchY > 0 && p_->punchY < 60) && p_->plantingTree == 0) edit_tile(peer, p_->punchX, p_->punchY, 18);
						}
						/*
						vector<pair<int, string>>::iterator p23 = find_if(carnival_game_playing.begin(), carnival_game_playing.end(), [&](const pair < int, string>& element) { return element.second == pInfo(peer)->tankIDName; });
						if (p23 != carnival_game_playing.end()) {
							if ((abs((pInfo(peer)->x / 32) - 32) <= 1 && abs((pInfo(peer)->y / 32) - 11) <= 1) or (abs((pInfo(peer)->x / 32) - 32) <= 2 && abs((pInfo(peer)->y / 32) - 11) <= 1)) {
								carnival_end(peer, carnival_game_playing[p23 - carnival_game_playing.begin()].first, true), carnival_game_playing.erase(p23);
							}
						}*/
						if (p_->characterState == 268435472 || p_->characterState == 268435488 || p_->characterState == 268435504 || p_->characterState == 268435616 || p_->characterState == 268435632 || p_->characterState == 268435456 || p_->characterState == 224 || p_->characterState == 112 || p_->characterState == 80 || p_->characterState == 96 || p_->characterState == 224 || p_->characterState == 65584 || p_->characterState == 65712 || p_->characterState == 65696 || p_->characterState == 65536 || p_->characterState == 65552 || p_->characterState == 65568 || p_->characterState == 65680 || p_->characterState == 192 || p_->characterState == 65664 || p_->characterState == 65600 || p_->characterState == 67860 || p_->characterState == 64) {
							if (pInfo(peer)->lava_time + 5000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
								pInfo(peer)->lavaeffect = 0;
								pInfo(peer)->lava_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
							}
							else {
								if (pInfo(peer)->xenonite_heat_resist == 1 || pInfo(peer)->xenonite_heat_resist == 2) {
									if (pInfo(peer)->lavaeffect >= (pInfo(peer)->xenonite_heat_resist == 1 ? 7 : 3)) {
										pInfo(peer)->lavaeffect = 0;
										SendRespawn(peer, false, 0, true);
									}
									else pInfo(peer)->lavaeffect++;
								}
								else {
									if (pInfo(peer)->lavaeffect >= (pInfo(peer)->feet == 250 || pInfo(peer)->necklace == 5426 || (pInfo(peer)->mask == 5712 && pInfo(peer)->wild == 6) ? 7 : 3)) {
										pInfo(peer)->lavaeffect = 0;
										SendRespawn(peer, false, 0, true);
									}
									else pInfo(peer)->lavaeffect++;
								}
							}
						}
						if (pInfo(peer)->fishing_used != 0) {
							if (pInfo(peer)->f_xy != pInfo(peer)->x + pInfo(peer)->y) pInfo(peer)->move_warning++;
							if (pInfo(peer)->move_warning > 1) stop_fishing(peer, true, "Sit still if you wanna fish!");
							if (p_->punchX > 0 && p_->punchY > 0) {
								pInfo(peer)->punch_warning++;
								if (pInfo(peer)->punch_warning >= 2) stop_fishing(peer, false, "");
							}
						}
						/*
						string name_ = pInfo(peer)->world;
						vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
						if (p != worlds.end()) {
							World* world_ = &worlds[p - worlds.begin()];
							if (pInfo(peer)->x != -1 and pInfo(peer)->y != -1) {
								//try {
								int x_ = (pInfo(peer)->state == 16 ? (int)p_->x / 32 : round((double)p_->x / 32)), y_ = (int)p_->y / 32;
								if (x_ < 0 or x_ >= 100 or y_ < 0 or y_ >= 60) continue;
								if (world_->blocks[x_ + (y_ * 100)].fg == 1256) pInfo(peer)->hospital_bed = true;
								else pInfo(peer)->hospital_bed = false;
								if (pInfo(peer)->c_x * 32 != (int)p_->x and pInfo(peer)->c_y * 32 != (int)p_->y and not pInfo(peer)->ghost) {
									bool impossible = false;
									if (pInfo(peer)->superdev || find(world_->admins.begin(), world_->admins.end(), pInfo(peer)->tankIDName) != world_->admins.end() || pInfo(peer)->mod || world_->owner_name == pInfo(peer)->tankIDName || pInfo(peer)->level > 35 || pInfo(peer)->dev) {

									}
									else impossible = ar_turi_noclipa(world_, pInfo(peer)->x, pInfo(peer)->y, x_ + (y_ * 100), peer);
									if (world_->blocks[x_ + (y_ * 100)].locked) {
										WorldBlock* block2_ = &world_->blocks[world_->blocks[x_ + (y_ * 100)].lock_origin];
										if (find(block2_->admins.begin(), block2_->admins.end(), pInfo(peer)->tankIDName) != block2_->admins.end()) impossible = false, pInfo(peer)->hack_ = 0, pInfo(peer)->pulled = true;
									}
									if (impossible) {
										if (items[world_->blocks[x_ + (y_ * 100)].fg].actionType != 31) {
											if (pInfo(peer)->pulled == false) {
												gamepacket_t p(0, pInfo(peer)->netID);
												p.Insert("OnSetPos");
												p.Insert(pInfo(peer)->x, pInfo(peer)->y);
												p.CreatePacket(peer);
												pInfo(peer)->hack_++;
											}
											if (pInfo(peer)->pulled == true) pInfo(peer)->hack_ = 0;
											if (pInfo(peer)->hack_ >= 4) {
												enet_peer_disconnect_later(peer, 0);
											}
											break;
										}
									}
								}
								/*
								if (block_->fg == 1508 and not world_->name.empty()) {
									char blarney_world = world_->name.back();
									if (isdigit(blarney_world)) {
										long long current_time = time(nullptr);
										vector<vector<long long>> av_blarneys = pInfo(peer)->completed_blarneys;
										for (int i_ = 0; i_ < av_blarneys.size(); i_++) {
											int t_blarney_world = av_blarneys[i_][0];
											if ((int)blarney_world - 48 == t_blarney_world) {
												long long blarney_time = av_blarneys[i_][1];
												if (blarney_time - current_time <= 0) {
													av_blarneys[i_][1] = current_time + 86400;
													vector<vector<int>> blarney_prizes{
														{11712, 1},{11742, 1},{11710, 1},{11722, 1}, {528, 1},{540, 1},{1514, 5},{1544, 1},{260, 1},{1546, 1},{2400, 1},{2404, 1},{2406, 1},{2414, 1},{2416, 1},{2464, 1},{3428, 1},{3426, 1},{4532, 1},{4528, 1},{4526, 5},{4520, 1},{5740, 1},{5734, 1},{7982, 1},{7992, 1},{7994, 1},{7980, 1},{7998, 1},{7984, 3},{7988, 1},{9416, 1},{9424, 1},{10704, 1},{10680, 1},{10670, 1},{10676, 1}
													};
													vector<int> prize_ = blarney_prizes[rand() % blarney_prizes.size()];
													uint32_t give_id = prize_[0];
													uint32_t give_count = prize_[1];
													int c_ = give_count;
													if (modify_inventory(peer, give_id, c_) != 0) {
														WorldDrop drop_block_{};
														drop_block_.id = give_id, drop_block_.count = give_count, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = pInfo(peer)->x + rand() % 17, drop_block_.y = pInfo(peer)->y + rand() % 17;
														dropas_(world_, drop_block_);
													}
													int c_2 = 1;
													if (modify_inventory(peer, 1510, c_2) != 0) {
														WorldDrop drop_block_{};
														drop_block_.id = 1510, drop_block_.count = c_2, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = pInfo(peer)->x + rand() % 17, drop_block_.y = pInfo(peer)->y + rand() % 17;
														dropas_(world_, drop_block_);
													}
													pInfo(peer)->remind_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
													gamepacket_t p;
													p.Insert("OnTalkBubble");
													p.Insert(pInfo(peer)->netID);
													p.Insert("You kissed the " + items[block_->fg].name + " and got a `2" + items[1510].name + "`` and `2" + items[give_id].name + "``");
													p.Insert(1);
													p.CreatePacket(peer);
													{
														gamepacket_t p;
														p.Insert("OnConsoleMessage");
														p.Insert("You kissed the " + items[block_->fg].name + " and got a `2" + items[1510].name + "`` and `2" + items[give_id].name + "``");
														p.CreatePacket(peer);
													}
												}
												else {
													if (pInfo(peer)->remind_time + 8000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
														pInfo(peer)->remind_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
														gamepacket_t p;
														p.Insert("OnTalkBubble");
														p.Insert(pInfo(peer)->netID);
														p.Insert("You will be able to kiss the stone again in " + to_playmod_time(blarney_time - current_time) + "");
														p.Insert(0);
														p.CreatePacket(peer);
													}
												}
												break;
											}
										}
										pInfo(peer)->completed_blarneys = av_blarneys;
									}
								}*/
								//}
								//catch (out_of_range) { // nuskrido uz worldo
									//cout << "failed to perform anticheat check for player " << pInfo(peer)->tankIDName + " invalid world??" << endl;
								//}
							//}
						if (pInfo(peer)->hand == 2286) {
							if (rand() % 100 < 6) {
								pInfo(peer)->geiger_++;
								if (pInfo(peer)->geiger_ >= 100) {
									int c_ = -1;
									modify_inventory(peer, 2286, c_);
									int c_2 = 1;
									modify_inventory(peer, 2204, c_2);
									pInfo(peer)->hand = 2204;
									pInfo(peer)->geiger_ = 0;
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("You are detecting radiation... (`$Geiger Counting`` mod added)");
									p.CreatePacket(peer);
									packet_(peer, "action|play_sfx\nfile|audio/dialog_confirm.wav\ndelayMS|0");
									update_clothes(peer);
								}
							}
						}
						if (pInfo(peer)->back == 240) {
							if (pInfo(peer)->gems > 0) {
								if (pInfo(peer)->x != (int)p_->x) {
									if (pInfo(peer)->i240 + 750 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
										pInfo(peer)->i240 = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
										pInfo(peer)->gems -= 1;
										WorldDrop item_{};
										string name_ = pInfo(peer)->world;
										vector<World>::iterator pb = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
										if (pb != worlds.end()) {
											World* world_ = &worlds[pb - worlds.begin()];
											item_.id = 112, item_.count = 1, item_.x = (int)p_->x + rand() % 17, item_.y = (int)p_->y + rand() % 17, item_.uid = uint16_t(world_->drop.size()) + 1;
											dropas_(world_, item_);
										}
										gamepacket_t p;
										p.Insert("OnSetBux");
										p.Insert(pInfo(peer)->gems);
										p.Insert(0);
										p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
										if (pInfo(peer)->supp >= 2) p.Insert((float)33796, (float)1, (float)0);
										p.CreatePacket(peer);
									}
								}
							}
						}
						move_(peer, p_);
						if (pInfo(peer)->update) {
							if (pInfo(peer)->x == -1 and pInfo(peer)->y == -1) {
								pInfo(peer)->update = false;
								update_clothes(peer);
							}
						}
						//if (pInfo(peer)->x == -1 and pInfo(peer)->y == -1) { /*Atnaujinti clothes nes uzejo i worlda*/
						//if (pInfo(peer)->tankIDName == "vyte") { /*Atnaujinti clothes nes uzejo i worlda*/
							//update_clothes(peer);
							/*
							uint32_t my_guild_role = -1;
							if (pInfo(peer)->guild_id != 0) {
								uint32_t guild_id = pInfo(peer)->guild_id;
								vector<Guild>::iterator find_guild = find_if(guilds.begin(), guilds.end(), [guild_id](const Guild& a) { return a.guild_id == guild_id; });
								if (find_guild != guilds.end()) {
									Guild* guild_information = &guilds[find_guild - guilds.begin()];
									for (GuildMember member_search : guild_information->guild_members) {
										if (member_search.member_name == pInfo(peer)->tankIDName) {
											my_guild_role = member_search.role_id;
											break;
										}
									}
								}
							}*/
							/*
							gamepacket_t p3(0, pInfo(peer)->netID);
							p3.Insert("OnSetRoleSkinsAndIcons");
							p3.Insert(pInfo(peer)->roleSkin);
							p3.Insert(pInfo(peer)->roleIcon);
							p3.Insert(0);*/
							/*
							gamepacket_t p(0, pInfo(peer)->netID);
							p.Insert("OnGuildDataChanged");
							p.Insert(50478);
							p.Insert(79289404);
							p.Insert(0), p.Insert(my_guild_role);*/
							/*
							gamepacket_t p2(0, pInfo(peer)->netID);
							p2.Insert("OnCountryState");
							p2.Insert(pInfo(peer)->country + "|showGuild");*/

							/*
							for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
								if (pInfo(currentPeer)->world == pInfo(peer)->world) {
									p3.CreatePacket(currentPeer);
									/*
									if (my_guild_role != -1) {
										p.CreatePacket(currentPeer);
										//p2.CreatePacket(currentPeer);
									}*/

									/*
											if (pInfo(currentPeer)->netID != pInfo(peer)->netID) {
												if (pInfo(currentPeer)->roleSkin != 6 or pInfo(currentPeer)->roleIcon != 6) {
													gamepacket_t p_p(0, pInfo(currentPeer)->netID);
													p_p.Insert("OnSetRoleSkinsAndIcons");
													p_p.Insert(pInfo(currentPeer)->roleSkin);
													p_p.Insert(pInfo(currentPeer)->roleIcon);
													p_p.Insert(0);
													p_p.CreatePacket(peer);
												}
											}*/



											/*
											if (pInfo(currentPeer)->netID != pInfo(peer)->netID and pInfo(currentPeer)->guild_id != 0) {
												uint32_t guild_id = pInfo(currentPeer)->guild_id;
												vector<Guild>::iterator find_guild = find_if(guilds.begin(), guilds.end(), [guild_id](const Guild& a) { return a.guild_id == guild_id; });
												if (find_guild != guilds.end()) {
													Guild* guild_information = &guilds[find_guild - guilds.begin()];
													uint32_t my_role = 0;
													for (GuildMember member_search : guild_information->guild_members) {
														if (member_search.member_name == pInfo(currentPeer)->tankIDName) {
															my_role = member_search.role_id;
															break;
														}
													}
													gamepacket_t p(0, pInfo(currentPeer)->netID);
													p.Insert("OnGuildDataChanged");
													p.Insert(50478);
													p.Insert(79289404);
													p.Insert(0), p.Insert(my_role);
													p.CreatePacket(peer);
													{
														/*
														gamepacket_t p(0, pInfo(currentPeer)->netID);
														p.Insert("OnCountryState");
														p.Insert(pInfo(currentPeer)->country + "|showGuild");
														p.CreatePacket(peer);*//*
													}
												}
											}*/
											//	}
											//}
											/*
											long long ms_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
											map<string, vector<WorldNPC>>::iterator it;
											for (it = active_npc.begin(); it != active_npc.end(); it++) {
												if (it->first == world_->name) {
													for (int i_ = 0; i_ < it->second.size(); i_++) {
														try {
															WorldNPC npc_ = it->second[i_];
															if (npc_.uid == -1) continue;
															double per_sekunde_praeina_bloku = (double)npc_.projectile_speed / 32;
															double praejo_laiko = (double)(ms_time - npc_.started_moving) / 1000;
															double praejo_distancija = (double)per_sekunde_praeina_bloku * (double)praejo_laiko;
															double current_x = ((int)npc_.kryptis == 180 ? (((double)npc_.x - (double)praejo_distancija) * 32) + 16 : (((double)npc_.x + (double)praejo_distancija) * 32) + 16);
															double current_y = (double)npc_.y * 32;
															bool blocked_ = false;
															if ((int)npc_.kryptis == 180) { // check if it wasnt blocked
																vector<int> new_tiles{};
																if (items[world_->blocks[(int)(current_x / 32) + ((int)(current_y / 32) * 100)].fg].collisionType != 1) {
																	new_tiles.push_back((int)(current_x / 32) + ((int)(current_y / 32) * 100));
																} int ySize = world_->blocks.size() / 100, xSize = world_->blocks.size() / ySize;
																vector<WorldBlock> shadow_copy = world_->blocks;
																for (int i2 = 0; i2 < new_tiles.size(); i2++) {
																	int x_ = new_tiles[i2] % 100, y_ = new_tiles[i2] / 100;
																	if (x_ < 99 and items[shadow_copy[x_ + 1 + (y_ * 100)].fg].collisionType != 1) {
																		if (not shadow_copy[x_ + 1 + (y_ * 100)].scanned) {
																			shadow_copy[x_ + 1 + (y_ * 100)].scanned = true;
																			new_tiles.push_back(x_ + 1 + (y_ * 100));
																		}
																	}
																	else if (items[shadow_copy[x_ + 1 + (y_ * 100)].fg].collisionType == 1 and x_ < npc_.x) {
																		blocked_ = true;
																		break;
																	}
																}
															}
															else {
																vector<int> new_tiles{};
																if (items[world_->blocks[(int)(current_x / 32) + ((int)(current_y / 32) * 100)].fg].collisionType != 1) {
																	new_tiles.push_back((int)(current_x / 32) + ((int)(current_y / 32) * 100));
																} int ySize = world_->blocks.size() / 100, xSize = world_->blocks.size() / ySize;
																vector<WorldBlock> shadow_copy = world_->blocks;
																for (int i2 = 0; i2 < new_tiles.size(); i2++) {
																	int x_ = new_tiles[i2] % 100, y_ = new_tiles[i2] / 100;
																	if (x_ < 99 and items[shadow_copy[x_ - 1 + (y_ * 100)].fg].collisionType != 1) {
																		if (not shadow_copy[x_ - 1 + (y_ * 100)].scanned) {
																			shadow_copy[x_ - 1 + (y_ * 100)].scanned = true;
																			new_tiles.push_back(x_ - 1 + (y_ * 100));
																		}
																	}
																	else if (items[shadow_copy[x_ - 1 + (y_ * 100)].fg].collisionType == 1 and x_ > npc_.x) {
																		blocked_ = true;
																		break;
																	}
																}
															} if (blocked_) {
																continue;
															}
															PlayerMoving data_{};
															data_.packetType = 34;
															data_.x = (current_x + 16); //nuo x
															data_.y = (current_y + (npc_.id == 8020 ? 6 : 16)); //nuo y
															data_.XSpeed = (current_x + 16); // iki x
															data_.YSpeed = (current_y + (npc_.id == 8020 ? 6 : 16)); // iki y
															data_.punchY = npc_.projectile_speed;
															BYTE* raw = packPlayerMoving(&data_);
															raw[1] = (npc_.id == 8020 ? 15 : 8), raw[2] = npc_.uid, raw[3] = 2;
															memcpy(raw + 40, &npc_.kryptis, 4);
															send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
															delete[] raw;
														}
														catch (out_of_range) {
															continue;
														}
													}
													break;
												}
											}*/
											//}

						pInfo(peer)->x = (int)p_->x, pInfo(peer)->y = (int)p_->y, pInfo(peer)->state = p_->characterState & 0x10;
						if (abs(targetX - currentX) >= 4 || abs(targetY - currentY) >= 4)
						{
							if (currentX != 0 && currentY != 0)
							{
								uint16_t maindoorx = 0;
								uint16_t maindoory = 0;

								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									WorldBlock* block_ = &world_->blocks[pInfo(peer)->x + (pInfo(peer)->y * 100)];
									uint16_t t_ = (block_->fg ? block_->fg : block_->bg);
									int ySize = (int)world_->blocks.size() / 100, xSize = (int)world_->blocks.size() / ySize, square = (int)world_->blocks.size();
									for (int i_ = 0; i_ < square; i_++) {
										if (items[world_->blocks[i_].fg].blockType == BlockTypes::MAIN_DOOR) {
											maindoorx = (i_ % xSize);
											maindoory = (i_ / xSize);
											break;
										}
									}
								}
								if(maindoorx != targetX && maindoory != targetY)
									enet_peer_disconnect_later(peer, 0);
							}
						}

						break;
					}
					case 3: /*Kai zaidejas papunchina/wrenchina bloka*/
					{
						if (p_->plantingTree <= 0 || p_->plantingTree >= items.size()) break;
						if ((pInfo(peer)->dev + pInfo(peer)->superdev == 0) && (abs(p_->x / 32 - p_->punchX) > (pInfo(peer)->hand == 11094 ? 6 : 4) || abs(p_->y / 32 - p_->punchY) > (pInfo(peer)->hand == 11094 ? 6 : 4))) break;
						//if (items[p_->plantingTree].blocked_place == 1) break; // crash block
						if (p_->plantingTree != 18 and p_->plantingTree != 32) {
							int c_ = 0;
							modify_inventory(peer, p_->plantingTree, c_);
							if (c_ == 0) break;
						}
						if (p_->plantingTree == 18) {
							if (pInfo(peer)->punch_time + 100 > (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) break;
							pInfo(peer)->punch_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
							if (has_playmod(pInfo(peer), "Infected!") or pInfo(peer)->hand != 0) {
								pInfo(peer)->last_infected = p_->punchX + (p_->punchY * 100);
								if (pInfo(peer)->last_inf + 1500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
									pInfo(peer)->last_inf = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
									bool infected = false;
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world or pInfo(peer)->netID == pInfo(currentPeer)->netID) continue;
										if (infected) break;
										if (pInfo(currentPeer)->x / 32 + (pInfo(currentPeer)->y / 32 * 100) - pInfo(peer)->last_infected <= 3) {
											if (has_playmod(pInfo(peer), "Infected!") && not has_playmod(pInfo(currentPeer), "Infected!")) {
												if (has_playmod(pInfo(currentPeer), "Antidote!")) {
													for (ENetPeer* currentPeer2 = server->peers; currentPeer2 < &server->peers[server->peerCount]; ++currentPeer2) {
														if (currentPeer2->state != ENET_PEER_STATE_CONNECTED or currentPeer2->data == NULL or pInfo(currentPeer)->world != pInfo(currentPeer2)->world) continue;
														PlayerMoving data_{};
														data_.packetType = 19, data_.punchX = 782, data_.x = pInfo(currentPeer)->x + 10, data_.y = pInfo(currentPeer)->y + 16;
														int32_t to_netid = pInfo(currentPeer)->netID;
														BYTE* raw = packPlayerMoving(&data_);
														raw[3] = 5;
														memcpy(raw + 8, &to_netid, 4);
														send_raw(currentPeer2, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
														delete[]raw;
													}
												}
												else {
													infected = true;
													gamepacket_t p, p2;
													p.Insert("OnAddNotification"), p.Insert("interface/large/infected.rttex"), p.Insert("`4You were infected by " + pInfo(peer)->tankIDName + "!"), p.CreatePacket(currentPeer);
													p2.Insert("OnConsoleMessage"), p2.Insert("You've been infected by the g-Virus. Punch others to infect them, too! Braiiiins... (`$Infected!`` mod added, `$1 mins`` left)"), p2.CreatePacket(currentPeer);
													PlayMods give_playmod{};
													give_playmod.id = 28, give_playmod.time = time(nullptr) + 60;
													pInfo(currentPeer)->playmods.push_back(give_playmod);
													update_clothes(currentPeer, true);
												}
											}
											else if (zombie_apocalypse && not has_playmod(pInfo(peer), "Infected!") && has_playmod(pInfo(currentPeer), "Infected!")) {
												infected = true;
												SendRespawn(currentPeer, 0, true);
												string name_ = pInfo(currentPeer)->world;
												vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
												if (p != worlds.end()) {
													World* world_ = &worlds[p - worlds.begin()];
													WorldDrop drop_block_{};
													drop_block_.id = rand() % 2 < 1 ? 4450 : 4490, drop_block_.count = pInfo(peer)->hand == 9500 ? 2 : 1, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = pInfo(currentPeer)->x, drop_block_.y = pInfo(currentPeer)->y;
													for (int i_ = 0; i_ < pInfo(currentPeer)->playmods.size(); i_++) {
														if (pInfo(currentPeer)->playmods[i_].id == 28) {
															pInfo(currentPeer)->playmods[i_].time = 0;
															break;
														}
													}
													dropas_(world_, drop_block_);
												}
											}
										}
									}
								}
							}
						}
						if (pInfo(peer)->trading_with != -1 and p_->packetType != 0 and p_->packetType != 18) {
							cancel_trade(peer, false, true);
							break;
						}
						if (p_->plantingTree == 18 and (pInfo(peer)->hand == 9604 || pInfo(peer)->hand == 5480 || pInfo(peer)->hand == 9770 || pInfo(peer)->hand == 9772)) { // tingejau galvoti kaip kitaip daryt
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								if (world_->blocks[p_->punchX + (p_->punchY * 100)].fg == 0 and world_->blocks[p_->punchX + (p_->punchY * 100)].bg == 0) break;
							}
							edit_tile(peer, p_->punchX, p_->punchY, p_->plantingTree);
							if (p_->punchY == pInfo(peer)->y / 32) {
								edit_tile(peer, p_->punchX + (pInfo(peer)->state == 16 ? -1 : 1), p_->punchY, p_->plantingTree);
								edit_tile(peer, p_->punchX + (pInfo(peer)->state == 16 ? -2 : 2), p_->punchY, p_->plantingTree);
							}
							else if (p_->punchX == pInfo(peer)->x / 32) {
								if (p_->punchY > pInfo(peer)->y / 32 || p_->punchY < pInfo(peer)->y / 32) {
									edit_tile(peer, p_->punchX, p_->punchY + (p_->punchY > pInfo(peer)->y / 32 ? 1 : -1), p_->plantingTree);
									edit_tile(peer, p_->punchX, p_->punchY + (p_->punchY > pInfo(peer)->y / 32 ? 2 : -2), p_->plantingTree);
								}
							}
							else if (p_->punchY < pInfo(peer)->y / 32 || p_->punchY > pInfo(peer)->y / 32) {
								edit_tile(peer, p_->punchX + (pInfo(peer)->state == 16 ? -1 : 1), p_->punchY + (p_->punchY < pInfo(peer)->y / 32 ? -1 : 1), p_->plantingTree);
								edit_tile(peer, p_->punchX + (pInfo(peer)->state == 16 ? -2 : 2), p_->punchY + (p_->punchY < pInfo(peer)->y / 32 ? -2 : 2), p_->plantingTree);
							}
						}
						else {
							bool empty = false;
							if (p_->plantingTree == 5640) {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									for (int i_ = 0; i_ < world_->machines.size(); i_++) {
										WorldMachines machine = world_->machines[i_];
										if (machine.x == pInfo(peer)->magnetron_x and machine.y == pInfo(peer)->magnetron_y and machine.id == 5638) {
											if (machine.enabled) {
												WorldBlock* itemas = &world_->blocks[machine.x + (machine.y * 100)];
												if (itemas->magnetron and itemas->id == pInfo(peer)->magnetron_id) {
													if (itemas->pr > 0) {
														p_->plantingTree = itemas->id;
														if (edit_tile(peer, p_->punchX, p_->punchY, itemas->id, true)) {
															itemas->pr--;
															if (itemas->pr <= 0) {
																PlayerMoving data_{};
																data_.packetType = 5, data_.punchX = machine.x, data_.punchY = machine.y, data_.characterState = 0x8;
																BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, itemas));
																BYTE* blc = raw + 56;
																form_visual(blc, *itemas, *world_, NULL, false);
																for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																	if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																	if (pInfo(currentPeer)->world == world_->name) {
																		send_raw(currentPeer, 4, raw, 112 + alloc_(world_, itemas), ENET_PACKET_FLAG_RELIABLE);
																	}
																}
																delete[] raw, blc;
															}
															break;
														}
													}
													else {
														empty = true;
														gamepacket_t p;
														p.Insert("OnTalkBubble");
														p.Insert(pInfo(peer)->netID);
														p.Insert("The `2" + items[machine.id].name + "`` is empty!");
														p.Insert(0), p.Insert(0);
														p.CreatePacket(peer);
													}
												}
											}
											break;
										}
									}
								}
								if (p_->plantingTree == 5640 and not empty) {
									gamepacket_t p;
									p.Insert("OnTalkBubble");
									p.Insert(pInfo(peer)->netID);
									p.Insert("There is no active `2" + items[5638].name + "``!");
									p.Insert(0), p.Insert(0);
									p.CreatePacket(peer);
								}
								break;
							}
							edit_tile(peer, p_->punchX, p_->punchY, p_->plantingTree);
						}
						break;
					}
					case 7: /*Kai zaidejas ieina pro duris arba portal*/ /*2/16/2022 update: cia dar gali buti STEAM USE*/
					{
						if (p_->punchX < 0 or p_->punchX > 100 or p_->punchY < 0 or p_->punchY >= 60) break;
						string name_ = pInfo(peer)->world;
						vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
						if (p != worlds.end()) {
							World* world_ = &worlds[p - worlds.begin()];
							//try {
							WorldBlock* block_ = &world_->blocks[p_->punchX + (p_->punchY * 100)];
							if (items[items[block_->fg ? block_->fg : block_->bg].id].blockType == BlockTypes::CHECKPOINT) {
								pInfo(peer)->c_x = p_->punchX, pInfo(peer)->c_y = p_->punchY;
								gamepacket_t p(0, pInfo(peer)->netID);
								p.Insert("SetRespawnPos");
								p.Insert(pInfo(peer)->c_x + (pInfo(peer)->c_y * 100));
								p.CreatePacket(peer);
							}
							else if (items[block_->fg ? block_->fg : block_->bg].id == 6) exit_(peer);
							else if (block_->fg == 4722 && pInfo(peer)->adventure_begins == false) {
								pInfo(peer)->adventure_begins = true;
								gamepacket_t p(0);
								p.Insert("OnAddNotification"), p.Insert("interface/large/adventure.rttex"), p.Insert(block_->heart_monitor), p.Insert("audio/gong.wav"), p.Insert(0), p.CreatePacket(peer);
							}
							else if (items[block_->fg].blockType == BlockTypes::DOOR or items[block_->fg].blockType == BlockTypes::PORTAL) {
								/*
								if (world_->name == "CARNIVAL") {
									if ((p_->punchX == 7 && p_->punchY == 37) or (p_->punchX == 34 && p_->punchY == 13) or (p_->punchX == 92 && p_->punchY == 37) or (p_->punchX == 36 && p_->punchY == 51) or (p_->punchX == 78 && p_->punchY == 51) or (p_->punchX == 71 && p_->punchY == 53)) {
										int count = -1;
										if (modify_inventory(peer, 1898, count) == 0) {
											if (p_->punchX == 71 && p_->punchY == 53) {
											}
											else carnival_start(peer, (p_->punchX == 7 && p_->punchY == 37 ? 1 : (p_->punchX == 34 && p_->punchY == 13 ? 2 : (p_->punchX == 92 && p_->punchY == 37 ? 3 : (p_->punchX == 36 && p_->punchY == 51 ? 4 : 5)))));
										}
										else {
											gamepacket_t p2;
											p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("This game costs 1 `8Golden Ticket``!"), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
											gamepacket_t p(250, pInfo(peer)->netID), p3(250), p4(250, pInfo(peer)->netID);
											p.Insert("OnSetFreezeState"), p.Insert(1), p.CreatePacket(peer);
											p3.Insert("OnZoomCamera"), p3.Insert((float)10000.000000), p3.Insert(1000), p3.CreatePacket(peer);
											p4.Insert("OnSetFreezeState"), p4.Insert(0), p4.CreatePacket(peer);
											break;
										}
									}
								}*/
								string door_target = block_->door_destination, door_id = "";
								World target_world = worlds[p - worlds.begin()];
								bool locked = true;
								if (block_access(peer, world_, block_) or block_->open) locked = false;
								int spawn_x = 0, spawn_y = 0;
								if (not locked && block_->fg != 762) {
									if (door_target.find(":") != string::npos) {
										vector<string> detales = explode(":", door_target);
										door_target = detales[0], door_id = detales[1];
									} if (not door_target.empty() and door_target != world_->name) {
										if (not check_name(door_target)) {
											gamepacket_t p(250, pInfo(peer)->netID);
											p.Insert("OnSetFreezeState");
											p.Insert(1);
											p.CreatePacket(peer);
											{
												gamepacket_t p(250);
												p.Insert("OnConsoleMessage");
												p.Insert(door_target);
												p.CreatePacket(peer);
											}
											{
												gamepacket_t p(250);
												p.Insert("OnZoomCamera");
												p.Insert((float)10000.000000);
												p.Insert(1000);
												p.CreatePacket(peer);
											}
											{
												gamepacket_t p(250, pInfo(peer)->netID);
												p.Insert("OnSetFreezeState");
												p.Insert(0);
												p.CreatePacket(peer);
											}
											break;
										}
										target_world = get_world(door_target);
									}
									int ySize = (int)target_world.blocks.size() / 100, xSize = (int)target_world.blocks.size() / ySize, square = (int)target_world.blocks.size();
									if (not door_id.empty()) {
										for (int i_ = 0; i_ < target_world.blocks.size(); i_++) {
											WorldBlock block_data = target_world.blocks[i_];
											if (block_data.fg == 1912 || block_data.fg == 1684 or items[block_data.fg].blockType == BlockTypes::DOOR or items[block_data.fg].blockType == BlockTypes::PORTAL) {
												if (block_data.door_id == door_id) {
													spawn_x = i_ % xSize, spawn_y = i_ / xSize;
													break;
												}
											}
										}
									}
								}
								if (block_->fg == 762) {
									pInfo(peer)->lastwrenchx = p_->punchX, pInfo(peer)->lastwrenchy = p_->punchY;
									gamepacket_t p2;
									if (block_->door_id == "") p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("No password has been set yet!"), p2.Insert(0), p2.Insert(1);
									else p2.Insert("OnDialogRequest"), p2.Insert("set_default_color|`o\nadd_label_with_icon|big|`wPassword Door``|left|762|\nadd_textbox|The door requires a password.|left|\nadd_text_input|password|Password||24|\nend_dialog|password_reply|Cancel|OK|");
									p2.CreatePacket(peer);
									gamepacket_t p(250, pInfo(peer)->netID), p3(250), p4(250, pInfo(peer)->netID);
									p.Insert("OnSetFreezeState"), p.Insert(1), p.CreatePacket(peer);
									p3.Insert("OnZoomCamera"), p3.Insert((float)10000.000000), p3.Insert(1000), p3.CreatePacket(peer);
									p4.Insert("OnSetFreezeState"), p4.Insert(0), p4.CreatePacket(peer);
								}
								if (block_->fg != 762) join_world(peer, target_world.name, spawn_x, spawn_y, 250, locked, true);
							}
							else {
								switch (block_->fg) {
								case 3270: case 3496:
								{
									if (p_->punchX < 0 || p_->punchY <= 0) break;
									Position2D steam_connector = track_steam(world_, block_, p_->punchX, p_->punchY);
									if (steam_connector.x != -1 and steam_connector.y != -1) {
										WorldBlock* block_s = &world_->blocks[steam_connector.x + (steam_connector.y * 100)];
										switch (block_s->fg) {
										case 3286: //steam door
										{
											block_s->flags = (block_s->flags & 0x00400000 ? block_s->flags ^ 0x00400000 : block_s->flags | 0x00400000);
											PlayerMoving data_{};
											data_.packetType = 5, data_.punchX = steam_connector.x, data_.punchY = steam_connector.y, data_.characterState = 0x8;
											BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_s));
											BYTE* blc = raw + 56;
											form_visual(blc, *block_s, *world_, peer, false);
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
												if (pInfo(currentPeer)->world == world_->name) {
													send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_s), ENET_PACKET_FLAG_RELIABLE);
												}
											}
											delete[] raw, blc;
											break;
										}
										case 3724: // spirit storage unit
										{
											uint32_t scenario = 20;
											{
												// check for ghost jars
												for (int i = 0; i < world_->drop.size(); i++) {
													WorldDrop* check_drop = &world_->drop[i];
													Position2D dropped_at{ check_drop->x / 32, check_drop->y / 32 };
													if (dropped_at.x == steam_connector.x and dropped_at.y == steam_connector.y) {
														if (check_drop->id == 3722) {
															uint32_t explo_chance = check_drop->count;
															// remove drop
															{
																PlayerMoving data_{};
																data_.packetType = 14, data_.netID = -2, data_.plantingTree = check_drop->uid;
																BYTE* raw = packPlayerMoving(&data_);
																int32_t item = -1;
																memcpy(raw + 8, &item, 4);
																for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																	if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																	if (pInfo(currentPeer)->world == name_) {
																		send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
																	}
																}
																world_->drop[i].id = 0, world_->drop[i].x = -1, world_->drop[i].y = -1;
																delete[] raw;
															}
															block_s->c_ += explo_chance;
															// explode or not
															{
																if (block_s->c_ * 5 >= 105) {
																	float explosion_chance = (float)((block_s->c_ * 5) - 100) * 0.5;
																	if (explosion_chance > rand() % 100) {
																		//bam bam
																		block_s->fg = 3726;
																		// drop the prize
																		{
																			vector<int> all_p{ 12464, 3734, 3732, 3748, 3712, 3706, 3708, 3718, 11136, 3728, 10056, 3730, 3788, 3750, 3738, 6060, 3738, 6840, 3736, 7784, 9596, 9598, 12288 };
																			uint32_t prize = 0;
																			if (block_s->c_ * 5 <= 115) prize = 3734;
																			else if (block_s->c_ * 5 <= 130) prize = 3732;
																			else if (block_s->c_ * 5 <= 140) prize = 3748;
																			else if (block_s->c_ * 5 <= 150) prize = 12464;
																			else if (block_s->c_ * 5 <= 170) {
																				vector<int> p_drops = {
																					3712, 3706, 3708, 3718, 11136
																				};
																				prize = p_drops[rand() % p_drops.size()];
																			}
																			else if (block_s->c_ * 5 <= 190)  prize = 3728;
																			else if (block_s->c_ * 5 <= 205)  prize = 10056;
																			else if (block_s->c_ * 5 <= 220)  prize = 3730;
																			else if (block_s->c_ * 5 == 225)  prize = 3788;
																			else if (block_s->c_ * 5 <= 240)  prize = 3750;
																			else if (block_s->c_ * 5 == 245)  prize = 3738;
																			else if (block_s->c_ * 5 <= 255)  prize = 6060;
																			else if (block_s->c_ * 5 <= 265 or explo_chance * 5 >= 265) {
																				if (explo_chance * 5 >= 265) prize = all_p[rand() % all_p.size()];
																				else prize = 3738;
																			}
																			else {
																				vector<int> p_drops = {
																					6840
																				};
																				if (block_s->c_ * 5 >= 270) p_drops.push_back(3736);
																				if (block_s->c_ * 5 >= 295) p_drops.push_back(7784);
																				if (block_s->c_ * 5 >= 369) p_drops.push_back(9598);
																				if (block_s->c_ * 5 >= 500) p_drops.push_back(9596);
																				if (block_s->c_ * 5 >= 850) p_drops.push_back(12288);
																				prize = p_drops[rand() % p_drops.size()];
																			} if (prize != 0) {
																				WorldDrop drop_block_{};
																				drop_block_.x = steam_connector.x * 32 + rand() % 17;
																				drop_block_.y = steam_connector.y * 32 + rand() % 17;
																				drop_block_.id = prize, drop_block_.count = 1, drop_block_.uid = uint16_t(world_->drop.size()) + 1;
																				dropas_(world_, drop_block_);
																				{
																					PlayerMoving data_{};
																					data_.packetType = 0x11, data_.x = steam_connector.x * 32 + 16, data_.y = steam_connector.y * 32 + 16;
																					data_.YSpeed = 97, data_.XSpeed = 3724;
																					BYTE* raw = packPlayerMoving(&data_);
																					PlayerMoving data_2{};
																					data_2.packetType = 0x11, data_2.x = steam_connector.x * 32 + 16, data_2.y = steam_connector.y * 32 + 16;
																					data_2.YSpeed = 108;
																					BYTE* raw2 = packPlayerMoving(&data_2);
																					gamepacket_t p;
																					p.Insert("OnConsoleMessage");
																					p.Insert("`#[A `9Spirit Storage Unit`` exploded, bringing forth an `9" + items[prize].name + "`` from The Other Side!]``");
																					for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																						if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																						if (pInfo(currentPeer)->world == world_->name) {
																							p.CreatePacket(currentPeer);
																							send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
																							send_raw(currentPeer, 4, raw2, 56, ENET_PACKET_FLAG_RELIABLE);
																						}
																					}
																					delete[] raw, raw2;
																				}
																				scenario = 22;
																			}
																		}
																		block_s->c_ = 0;
																	}
																}
															}
															// update visuals
															{
																PlayerMoving data_{};
																data_.packetType = 5, data_.punchX = steam_connector.x, data_.punchY = steam_connector.y, data_.characterState = 0x8;
																BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_s));
																BYTE* blc = raw + 56;
																form_visual(blc, *block_s, *world_, peer, false);
																for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																	if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																	if (pInfo(currentPeer)->world == world_->name) {
																		send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_s), ENET_PACKET_FLAG_RELIABLE);
																	}
																}
																delete[] raw, blc;
															}
															break;
														}
													}
												}
											}
											PlayerMoving data_{};
											data_.packetType = 32; // steam update paketas
											data_.punchX = steam_connector.x;
											data_.punchY = steam_connector.y;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = scenario;
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
												if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
												send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											}
											delete[] raw;
											break;
										}
										default:
											break;
										}
									}
									PlayerMoving data_{};
									data_.packetType = 32; // steam update paketas
									data_.punchX = p_->punchX;
									data_.punchY = p_->punchY;
									BYTE* raw = packPlayerMoving(&data_);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
										send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									}
									delete[] raw;
									break;
								}
								default:
									break;
								}
							}
						}
						//catch (out_of_range& klaida) {
							//cout << "case 7 klaida -> " << klaida.what() << endl;
						//}
					//}
						break;
					}
					case 10: /*Kai zaidejas paspaudzia du kartus ant inventory itemo*/
					{
						if (pInfo(peer)->trading_with != -1) {
							cancel_trade(peer, false);
							break;
						}
						if (p_->plantingTree <= 0 or p_->plantingTree >= items.size()) break;
						int c_ = 0;
						modify_inventory(peer, p_->plantingTree, c_);
						if (c_ == 0) break;
						if (items[p_->plantingTree].blockType != BlockTypes::CLOTHING) {
							int free = get_free_slots(pInfo(peer)), slot = 1;
							int c242 = 242, c1796 = 1796, c6802 = 6802, c1486 = 1486, countofused = 0, getdl = 1, getwl = 100, removewl = -100, removedl = -1, countwl = 0, c4450 = 4450, c4452 = 4452;
							int c4298 = 4298, c4300 = 4300;
							int c7188 = 7188;
							modify_inventory(peer, p_->plantingTree, countofused);
							if (free >= slot) {
								if (p_->plantingTree == 242 || p_->plantingTree == 1796) {
									modify_inventory(peer, p_->plantingTree == 242 ? c1796 : c242, countwl);
									if (p_->plantingTree == 242 ? countwl <= 199 : countwl <= 100) {
										if (p_->plantingTree == 242 ? countofused >= 100 : countofused >= 1) {
											modify_inventory(peer, p_->plantingTree == 242 ? c242 : c1796, p_->plantingTree == 242 ? removewl : removedl);
											modify_inventory(peer, p_->plantingTree == 242 ? c1796 : c242, p_->plantingTree == 242 ? getdl : getwl);
											gamepacket_t p, p2;
											p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert(p_->plantingTree == 242 ? "You compressed 100 `2World Lock`` into a `2Diamond Lock``!" : "You shattered a `2Diamond Lock`` into 100 `2World Lock``!"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
											p2.Insert("OnConsoleMessage"), p2.Insert(p_->plantingTree == 242 ? "You compressed 100 `2World Lock`` into a `2Diamond Lock``!" : "You shattered a `2Diamond Lock`` into 100 `2World Lock``!"), p2.CreatePacket(peer);
										}
									}
								}
								else if (p_->plantingTree == 7188) {
									modify_inventory(peer, c1796, countwl);
									if (countwl <= 100) {
										if (countofused >= 1) {
											modify_inventory(peer, c7188, removedl);
											modify_inventory(peer, c1796, getwl);
											gamepacket_t p, p2;
											p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You shattered a `2Blue Gem Lock`` into 100 `2Diamond Lock``!"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
											p2.Insert("OnConsoleMessage"), p2.Insert("You shattered a `2Blue Gem Lock`` into 100 `2Diamond Lock``!"), p2.CreatePacket(peer);
										}
									}
								}
								else if (p_->plantingTree == 1486 || p_->plantingTree == 6802) {
									modify_inventory(peer, p_->plantingTree == 1486 ? c6802 : c1486, countwl);
									if (p_->plantingTree == 1486 ? countwl <= 199 : countwl <= 100) {
										if (p_->plantingTree == 1486 ? countofused >= 100 : countofused >= 1) {
											modify_inventory(peer, p_->plantingTree == 1486 ? c1486 : c6802, p_->plantingTree == 1486 ? removewl : removedl);
											modify_inventory(peer, p_->plantingTree == 1486 ? c6802 : c1486, p_->plantingTree == 1486 ? getdl : getwl);
											gamepacket_t p, p2;
											p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert(p_->plantingTree == 1486 ? "You compressed 100 `2Growtoken`` into a `2Mega Growtoken``!" : "You shattered a `2Mega Growtoken`` into 100 `2Growtoken``!"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
											p2.Insert("OnConsoleMessage"), p2.Insert(p_->plantingTree == 1486 ? "You compressed 100 `2Growtoken`` into a `2Mega Growtoken``!" : "You shattered a `2Mega Growtoken`` into 100 `2Growtoken``!"), p2.CreatePacket(peer);
										}
									}
								}
								else if (p_->plantingTree == 4450 || p_->plantingTree == 4452) {
									modify_inventory(peer, p_->plantingTree == 4450 ? c4452 : c4450, countwl);
									if (p_->plantingTree == 4450 ? countwl <= 199 : countwl <= 100) {
										if (p_->plantingTree == 4450 ? countofused >= 100 : countofused >= 1) {
											modify_inventory(peer, p_->plantingTree == 4450 ? c4450 : c4452, p_->plantingTree == 4450 ? removewl : removedl);
											modify_inventory(peer, p_->plantingTree == 4450 ? c4452 : c4450, p_->plantingTree == 4450 ? getdl : getwl);
											gamepacket_t p, p2;
											p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert(p_->plantingTree == 4450 ? "You compressed 100 `2Zombie Brain`` into a `2Pile of Zombie Brains``!" : "You shattered a `2Pile of Zombie Brains`` into 100 `2Zombie Brain``!"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
											p2.Insert("OnConsoleMessage"), p2.Insert(p_->plantingTree == 4450 ? "You compressed 100 `2Zombie Brain`` into a `2Pile of Zombie Brains``!" : "You shattered a `2Pile of Zombie Brains`` into 100 `2Zombie Brain``!"), p2.CreatePacket(peer);
										}
									}
								}
								else if (p_->plantingTree == 4298 || p_->plantingTree == 4300) {
									modify_inventory(peer, p_->plantingTree == 4298 ? c4300 : c4298, countwl);
									if (p_->plantingTree == 4298 ? countwl <= 199 : countwl <= 100) {
										if (p_->plantingTree == 4298 ? countofused >= 100 : countofused >= 1) {
											modify_inventory(peer, p_->plantingTree == 4298 ? c4298 : c4300, p_->plantingTree == 4298 ? removewl : removedl);
											modify_inventory(peer, p_->plantingTree == 4298 ? c4300 : c4298, p_->plantingTree == 4298 ? getdl : getwl);
											gamepacket_t p, p2;
											p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert(p_->plantingTree == 4298 ? "You compressed 100 `2Caduceus`` into a `2Golden Caduceus``!" : "You shattered a `2Golden Caduceus`` into 100 `2Caduceus``!"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
											p2.Insert("OnConsoleMessage"), p2.Insert(p_->plantingTree == 4298 ? "You compressed 100 `2Caduceus`` into a `2Golden Caduceus``!" : "You shattered a `2Golden Caduceus`` into 100 `2Caduceus``!"), p2.CreatePacket(peer);
										}
									}
								}
							}
							break;
						}
						/*equip*/
						equip_clothes(peer, p_->plantingTree);
						break;
					}
					case 11: /*Kai zaidejas paema isdropinta itema*/
					{
						if (p_->x < 0 || p_->y < 0) break;

						int currentX = pInfo(peer)->x / 32;
						int currentY = pInfo(peer)->y / 32;
						int targetX = p_->x / 32;
						int targetY = p_->y / 32;

						if (abs(targetX - currentX) >= 2 || abs(targetY - currentY) >= 2)
						{
							if (currentX != 0 && currentY != 0)
							{
								break;
							}
						}

						bool displaybox = true;
						string name_ = pInfo(peer)->world;
						vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
						if (p != worlds.end()) {
							World* world_ = &worlds[p - worlds.begin()];
							for (int i_ = 0; i_ < world_->drop.size(); i_++) {
								if (world_->drop[i_].id == 0 or world_->drop[i_].x / 32 < 0 or world_->drop[i_].x / 32 > 99 or world_->drop[i_].y / 32 < 0 or world_->drop[i_].y / 32 > 59) continue;
								if (world_->drop[i_].uid == p_->plantingTree) {
									WorldBlock* block_ = &world_->blocks[world_->drop[i_].x / 32 + (world_->drop[i_].y / 32 * 100)];
									if (block_->fg == 1422 || block_->fg == 2488) displaybox = block_access(peer, world_, block_);
									if (abs((int)p_->x / 32 - world_->drop[i_].x / 32) > 1 || abs((int)p_->x - world_->drop[i_].x) >= 32 or abs((int)p_->y - world_->drop[i_].y) >= 32) displaybox = false;
									bool noclip = false;
									if (pInfo(peer)->superdev || pInfo(peer)->dev || world_->owner_name == pInfo(peer)->tankIDName || find(world_->admins.begin(), world_->admins.end(), pInfo(peer)->tankIDName) != world_->admins.end()) {
										if (pInfo(peer)->superdev || world_->owner_name == pInfo(peer)->tankIDName || find(world_->admins.begin(), world_->admins.end(), pInfo(peer)->tankIDName) != world_->admins.end()) displaybox = true;
									}
									else {
										noclip = ar_turi_noclipa(world_, p_->x, p_->y, world_->drop[i_].x / 32 + (world_->drop[i_].y / 32 * 100), peer);
									}
									if (displaybox && noclip == false) {
										int c_ = world_->drop[i_].count;
										if (world_->drop[i_].id == world_->special_event_item && world_->special_event && world_->drop[i_].special) {
											world_->special_event_item_taken++;
											if (items[world_->special_event_item].event_total == world_->special_event_item_taken) {
												gamepacket_t p, p3;
												p.Insert("OnAddNotification"), p.Insert("interface/large/special_event.rttex"), p.Insert("`2" + items[world_->special_event_item].event_name + ":`` `oSuccess! " + (items[world_->special_event_item].event_total == 1 ? "`2" + pInfo(peer)->tankIDName + "`` found it!``" : "All items found!``") + ""), p.Insert("audio/cumbia_horns.wav"), p.Insert(0);
												p3.Insert("OnConsoleMessage"), p3.Insert("`2" + items[world_->special_event_item].event_name + ":`` `oSuccess!`` " + (items[world_->special_event_item].event_total == 1 ? "`2" + pInfo(peer)->tankIDName + "`` `ofound it!``" : "All items found!``") + "");
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != name_) continue;
													if (items[world_->special_event_item].event_total != 1) {
														gamepacket_t p2;
														p2.Insert("OnConsoleMessage"), p2.Insert("`2" + items[world_->special_event_item].event_name + ":`` `0" + pInfo(peer)->tankIDName + "`` found a " + items[world_->special_event_item].name + "! (" + to_string(world_->special_event_item_taken) + "/" + to_string(items[world_->special_event_item].event_total) + ")``"), p2.CreatePacket(currentPeer);
													}
													p.CreatePacket(currentPeer);
													p3.CreatePacket(currentPeer);
												}
												world_->last_special_event = 0, world_->special_event_item = 0, world_->special_event_item_taken = 0, world_->special_event = false;
											}
											else {
												gamepacket_t p2;
												p2.Insert("OnConsoleMessage"), p2.Insert("`2" + items[world_->special_event_item].event_name + ":`` `0" + pInfo(peer)->tankIDName + "`` found a " + items[world_->special_event_item].name + "! (" + to_string(world_->special_event_item_taken) + "/" + to_string(items[world_->special_event_item].event_total) + ")``");
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != name_) continue;
													p2.CreatePacket(currentPeer);
												}
											}
										}
										if (modify_inventory(peer, world_->drop[i_].id, c_, false, true) == 0 or world_->drop[i_].id == 112) {
											PlayerMoving data_{};
											data_.effect_flags_check = 1, data_.packetType = 14, data_.netID = pInfo(peer)->netID, data_.plantingTree = world_->drop[i_].uid;
											BYTE* raw = packPlayerMoving(&data_);
											if (world_->drop[i_].id == 112) pInfo(peer)->gems += c_;
											else {
												add_cctv(peer, "took", to_string(world_->drop[i_].count) + " " + items[world_->drop[i_].id].name);
												gamepacket_t p;
												p.Insert("OnConsoleMessage"), p.Insert("Collected `w" + to_string(world_->drop[i_].count) + "" + (items[world_->drop[i_].id].blockType == BlockTypes::FISH ? "lb." : "") + " " + items[world_->drop[i_].id].ori_name + "``." + (items[world_->drop[i_].id].rarity > 363 ? "" : " Rarity: `w" + to_string(items[world_->drop[i_].id].rarity) + "``") + ""), p.CreatePacket(peer);
											}
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != name_) continue;
												send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											}
											delete[]raw;
											world_->drop[i_].id = 0, world_->drop[i_].x = -1, world_->drop[i_].y = -1;
										}
										else {
											if (c_ < 200 and world_->drop[i_].count >(200 - c_)) {
												int b_ = 200 - c_;
												world_->drop[i_].count -= b_;
												if (modify_inventory(peer, world_->drop[i_].id, b_, false) == 0) {
													add_cctv(peer, "took", to_string(world_->drop[i_].count) + " " + items[world_->drop[i_].id].name);
													WorldDrop drop_{};
													drop_.id = world_->drop[i_].id, drop_.count = world_->drop[i_].count, drop_.uid = uint16_t(world_->drop.size()) + 1, drop_.x = world_->drop[i_].x, drop_.y = world_->drop[i_].y;
													world_->drop.push_back(drop_);
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("Collected `w" + to_string(200 - c_) + " " + items[world_->drop[i_].id].ori_name + "``." + (items[world_->drop[i_].id].rarity > 363 ? "" : " Rarity: `w" + to_string(items[world_->drop[i_].id].rarity) + "``") + "");
													PlayerMoving data_{};
													data_.packetType = 14, data_.netID = -1, data_.plantingTree = world_->drop[i_].id, data_.x = world_->drop[i_].x, data_.y = world_->drop[i_].y;
													int32_t item = -1;
													float val = world_->drop[i_].count;
													BYTE* raw = packPlayerMoving(&data_);
													data_.plantingTree = world_->drop[i_].id;
													memcpy(raw + 8, &item, 4);
													memcpy(raw + 16, &val, 4);
													val = 0;
													data_.netID = pInfo(peer)->netID;
													data_.plantingTree = world_->drop[i_].uid;
													data_.x = 0, data_.y = 0;
													BYTE* raw2 = packPlayerMoving(&data_);
													BYTE val2 = 0;
													memcpy(raw2 + 8, &item, 4);
													memcpy(raw2 + 16, &val, 4);
													memcpy(raw2 + 1, &val2, 1);
													world_->drop[i_].id = 0, world_->drop[i_].x = -1, world_->drop[i_].y = -1;
													for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
														if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != name_) continue;
														send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
														if (pInfo(currentPeer)->netID == pInfo(peer)->netID)
															p.CreatePacket(currentPeer);
														send_raw(currentPeer, 4, raw2, 56, ENET_PACKET_FLAG_RELIABLE);
													}
													delete[]raw, raw2;
												}
											}
										}
									}
								}
							}
						}
						break;
					}
					case 18: { //chat bubble kai raso
						move_(peer, p_);
						break;
					}
						   /*
					case 23: //Kai zaidejas papunchina kita
					{
						if (pInfo(peer)->last_inf + 5000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
							pInfo(peer)->last_inf = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								bool can_cancel = true;
								if (find(world_->active_jammers.begin(), world_->active_jammers.end(), 1276) != world_->active_jammers.end()) can_cancel = false;
								if (can_cancel) {
									if (pInfo(peer)->trading_with != -1 and p_->packetType != 0 and p_->packetType != 18) {
										cancel_trade(peer, false, true);
										break;
									}
								}
							}
						}
						if (pInfo(peer)->last_inf + 5000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
							pInfo(peer)->last_inf = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
							bool inf = false;
							for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world or pInfo(peer)->netID == pInfo(currentPeer)->netID) continue;
								cout << pInfo(currentPeer)->last_infected << endl;
								cout << p_->plantingTree << endl;
								if (abs(pInfo(currentPeer)->last_infected - p_->plantingTree) <= 3) {
									if (has_playmod(pInfo(currentPeer), "Infected!") && not has_playmod(pInfo(peer), "Infected!") && inf == false) {
										if (has_playmod(pInfo(peer), "Antidote!")) {
											for (ENetPeer* currentPeer2 = server->peers; currentPeer2 < &server->peers[server->peerCount]; ++currentPeer2) {
												if (currentPeer2->state != ENET_PEER_STATE_CONNECTED or currentPeer2->data == NULL or pInfo(peer)->world != pInfo(currentPeer2)->world) continue;
												PlayerMoving data_{};
												data_.packetType = 19, data_.punchX = 782, data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16;
												int32_t to_netid = pInfo(peer)->netID;
												BYTE* raw = packPlayerMoving(&data_);
												raw[3] = 5;
												memcpy(raw + 8, &to_netid, 4);
												send_raw(currentPeer2, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
												delete[]raw;
											}
										}
										else {
											pInfo(currentPeer)->last_infected = 0;
											inf = true;
											gamepacket_t p, p2;
											p.Insert("OnAddNotification"), p.Insert("interface/large/infected.rttex"), p.Insert("`4You were infected by " + pInfo(currentPeer)->tankIDName + "!"), p.CreatePacket(peer);
											p2.Insert("OnConsoleMessage"), p2.Insert("You've been infected by the g-Virus. Punch others to infect them, too! Braiiiins... (`$Infected!`` mod added, `$1 mins`` left)"), p2.CreatePacket(peer);
											PlayMods give_playmod{};
											give_playmod.id = 28, give_playmod.time = time(nullptr) + 60;
											pInfo(peer)->playmods.push_back(give_playmod);
											update_clothes(peer);
										}
									}
									if (has_playmod(pInfo(peer), "Infected!") && not has_playmod(pInfo(currentPeer), "Infected!") && inf == false) {
										inf = true;
										SendRespawn(peer, 0, true);
										for (int i_ = 0; i_ < pInfo(peer)->playmods.size(); i_++) {
											if (pInfo(peer)->playmods[i_].id == 28) {
												pInfo(peer)->playmods[i_].time = 0;
												break;
											}
										}
										string name_ = pInfo(currentPeer)->world;
										vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
										if (p != worlds.end()) {
											World* world_ = &worlds[p - worlds.begin()];
											WorldDrop drop_block_{};
											drop_block_.id = rand() % 100 < 50 ? 4450 : 4490, drop_block_.count = pInfo(currentPeer)->hand == 9500 ? 2 : 1, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = pInfo(peer)->x, drop_block_.y = pInfo(peer)->y;
											dropas_(world_, drop_block_);
										}
									}
								}
							}
						}
						break;
					}*/
					default:
					{
						//cout << message_(event.packet) << "|invalid packet" << endl;
						break;
					}
					}
					break;
				}
				default:
					break;
				}
				//enet_event_destroy(event);
				enet_packet_destroy(event.packet);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
			{
				if (f_saving_) break;
				if (peer->data != NULL) {
					//wipe_beach(peer);
					if (pInfo(peer)->trading_with != -1) cancel_trade(peer, false);
					if (not pInfo(peer)->world.empty()) exit_(peer, true);
					save_player(pInfo(peer), true);
					peer->data = NULL;
					delete peer->data;
				}
				break;
			}
			default:
				cout << "invalid case" << endl;
				break;
			}
		}
	}
	return 0;
}
