#pragma once
struct Items {
	uint16_t id = 0;
	uint8_t count = 0;
};
struct Friends {
	string name = "";
	bool mute = false, block_trade = false;
	long long last_seen = 0;
};
struct PlayMods {
	uint16_t id = 0;
	long long time = 0;
	string user = "";
};
struct Player {
	string ip = "", fa_ip = "";//real ip 
	vector<string> last_visited_worlds, worlds_owned, trade_history;
	vector<map<int, int>> trade_items{};
	string name_color = "`0";
	uint32_t hair_color = 0xFFFFFFFF, eye_drop = 0xFFFFFFFF, eye_lenses = 0xFFFFFFFF;
	int id = 0, netID = 0, state = 0, trading_with = -1;
	bool trade_accept = false, accept_the_offer = false;
	int x = -1, y = -1;
	int gems = 0;
	bool growid = false;
	long long int xp = 0;
	bool usedmegaphone = 0;
	bool inventory_sent = false;
	map<int, int> ac_{};
	bool hit1 = false;
	int skin = 0x8295C3FF;
	uint16_t punched = 0;
	bool b_logs = false;
	int enter_game = 0;
	int lock = 0;
	int flagmay = 256;
	int lockeitem = 0;
	bool ghost = false, invis = false;
	uint8_t n = 0; //newbie passed save
	string note = "";
	uint8_t supp = 0, hs = 0, mod = 0, dev = 0, m_h = 0;
	int wls = 0;
	uint8_t csn = -1;
	bool pulled = false;
	bool upload = false;
	bool mini_you = false;
	bool While_Respawning = false;
	long long int b_t = 0, b_s = 0; // ban seconds
	int last_infected = 0;
	string b_r = "", b_b = ""; // ban reason ir banned by
	string m_r = "", m_b = "";
	uint16_t hair = 0, shirt = 0, pants = 0, feet = 0, face = 0, hand = 0, back = 0, mask = 0, necklace = 0, ances = 0; /*clothes*/
	string tankIDName = "", tankIDPass = "", requestedName = "", world = "", email = "", country = "", home_world = "", last_wrenched = "", last_msg_wrenched = "";
	string d_name = "";
	vector<Items> inv{};
	vector<Friends> friends{};
	vector<int> last_friends_selection{};
	vector<string> pending_friends{};
	vector<string> bans{};
	string last_edit = "";

	bool pc_player = false;

	uint8_t pure_shadow = 0;

	bool bypass = false, update_player = false, bypass2 = false;
	int new_packets = 0;
	int captcha_type = 0;

	uint8_t i_11818_1 = 0, i_11818_2 = 0;
	int8_t random_fossil = rand() % 3 + 4;
	int page_number = 0;
	string page_item = "";

	uint16_t opc = 0;
	int cc = 0;
	bool bb = false;
	uint8_t vip = 0, rb = 0, summer_surprise = 0;
	uint8_t gp = 0, gd = 0, glo = 0;
	uint8_t w_w = 0, w_d = 0;
	uint8_t mds = 0;
	int offergems = 0;
	bool  show_location_ = true, show_friend_notifications_ = true;
	int confirm_reset = 0;
	int firesputout = 0, carnivalgameswon = 0;
	uint8_t level = 1;
	long long playtime = 0;
	long long int account_created = 0, seconds = 0;
	string rid = "", mac = "", meta = "", vid = "";
	map<string, int> achievements{};
	string lastmsg = "", lastmsgworld = "";
	int gtwl = 0;
	uint16_t c_x = 0, c_y = 0;
	uint8_t lavaeffect = 0;
	//cooldowns
	long long int i240 = 0, i756 = 0, i758 = 0;
	bool tmod = 0;
	//billboard
	int b_i = 0, b_w = 0, b_p = 0;

	string b_bill = "0,0";
	//
	int same_input = 0, not_same = 0;
	string last_input_text = "";
	long long int last_inf = 0, last_text_input = 0, last_spam_detection = 0, last_consumable = 0, last_world_enter = 0, last_kickall = 0, last_fish_catch = 0, respawn_time = 0, hand_torch = 0, punch_time = 0, lava_time = 0, world_time = 0, geiger_time = 0, valentine_time = 0, remind_time = 0, special_time = 0, warp_time = 0, name_time = 0;
	int hack_ = 0;
	bool cancel_enter = false;
	int lastwrenchx = 0, lastwrenchy = 0, lastwrenchb = 0, lastchoosenitem = 0, lastchoosennr = 0;
	int pps = 0;
	long long lpps = 0;
	long long int punch_count = 0;
	bool tmod_on = true;

	uint8_t first_world = 0;
	uint8_t geiger_ = 0;
	uint8_t all_in = 0;
	int fishing_used = 0, f_x = 0, f_y = 0, move_warning = 0, f_xy = 0, punch_warning = 0, fish_seconds = 0;
	vector<int> glo_p;
	int geiger_x = -1, geiger_y = -1;
	int t_xp = 0, bb_xp = 0, g_xp = 0, p_xp = 0, ff_xp = 0, s_xp = 0;
	uint8_t t_lvl = 0, bb_lvl = 0, g_lvl = 0, p_lvl = 0, ff_lvl = 0, s_lvl = 0;
	vector<uint8_t> t_p, bb_p, p_p, g_p, ff_p, surg_p;
	uint8_t booty_broken = 0;
	uint8_t dd = 0;
	vector<PlayMods> playmods{};
	uint16_t b_l = 1;
	vector<pair<uint16_t, uint8_t>> bp;
	int choosenitem = 0;
	uint8_t radio = 0;
	bool temp_radio = true;
	int b_ra = 0, b_lvl = 1;
	int magnetron_id = 0;
	int magnetron_x = 0;
	int magnetron_y = 0;
	string growmoji = "(wl)|ā|0&(oops)|ą|0&(sleep)|Ċ|0&(punch)|ċ|0&(bheart)|Ĕ|0&(grow)|Ė|0&(gems)|ė|0&(gtoken)|ę|0&(cry)|ĝ|0&(vend)|Ğ|0&(bunny)|ě|0&(cactus)|ğ|0&(pine)|Ĥ|0&(peace)|ģ|0&(terror)|ġ|0&(troll)|Ġ|0&(evil)|Ģ|0&(fireworks)|Ħ|0&(football)|ĥ|0&(alien)|ħ|0&(party)|Ĩ|0&(pizza)|ĩ|0&(clap)|Ī|0&(song)|ī|0&(ghost)|Ĭ|0&(nuke)|ĭ|0&(halo)|Į|0&(turkey)|į|0&(gift)|İ|0&(cake)|ı|0&(heartarrow)|Ĳ|0&(lucky)|ĳ|0&(shamrock)|Ĵ|0&(grin)|ĵ|0&(ill)|Ķ|0&(eyes)|ķ|0&(weary)|ĸ|0&(moyai)|ļ|0&(plead)|Ľ|0&";
	long long int save_time = 0, basher_time = 0;
	vector<vector<long long>> completed_blarneys{
		{1, 0},
		{2, 0},
		{3, 0},
		{4, 0},
		{5, 0},
		{6, 0},
		{7, 0},
		{8, 0},
		{9, 0},
	};
	bool block_trade = false;
	int p_x = 0;
	int p_y = 0;
//	int guild_id = 0;
//	uint32_t pending_guild = 0;
	bool is_legend = false;
	uint8_t pps2 = 0;
	uint16_t pps23 = 0;
	long long lpps2 = 0, lpps23 = 0;
	uint8_t superdev = 0, roleIcon = 6, roleSkin = 6;

	uint8_t fires = 0;

	uint32_t total_pineapple = 0;
	//uint16_t stage_pineapple = 0;

	uint8_t garuda = 0;

	string personal = "\nadd_smalltext|Your personal rank did not update yet, wait few minutes!|";


	//surgery
	vector<uint16_t> available_surg_items;
	bool sounded = false;
	bool labworked = false;
	bool fixed = false;
	bool fixable = false;
	bool flu = false;
	uint8_t pulse = 40, site = 0, sleep = 0, dirt = 0, broken = 0, shattered = 0, incisions = 0, bleeding = 0, incneeded = 0, heart = 0;
	double temp = 98.6;
	double fever = 0.0;
	string pretext = "";
	string fixtext = "";
	string postext = "";
	string scantext = "";
	string tooltext = "The patient is prepped for surgery.";
	string endtext = "";

	bool update = false;
	bool s = true;
	uint8_t surgery_skill = 0, spongUsed = 0, scalpUsed = 0, stitcUsed = 0, antibUsed = 0, antisUsed = 0, ultraUsed = 0, labkiUsed = 0, anestUsed = 0, defibUsed = 0, splinUsed = 0, pinsUsed = 0, clampUsed = 0, transUsed = 0;
	uint16_t surgery_done = 0, su_8552_1 = 0, su_8552_2 = 0;
	bool antibs = false, surgery_started = false;
	string surgery_world = "";

	string surged_person = "";
	string surged_display = "";
	string surgery_name = "";
	bool hospital_bed = false;
	int fa2 = 0;
	//surgery done
	// 
	//int egg = 0;

	uint8_t surgery_type = -1, started_type = 0, mercy = 0, drtitle = 0, drt = 0, lvl125 = 0, donor = 0, master = 0;
	bool spotlight = false;
	bool mask_dragon = false;

	uint8_t growtoken_day = 0;
	vector<string> growtoken_worlds;
	uint8_t grow4good_day = 0, grow4good_seed2 = 0, grow4good_combine = 0, grow4good_crystal = 0, grow4good_honors = 0;
	uint16_t grow4good_week = 0, grow4good_seed = 0, grow4good_month = 0;

	string last_home_world = "";

	int grow4good_gems = 0;
	int grow4good_surgery = 0;
	int grow4good_fish = 0;
	int grow4good_points = 0;
	uint8_t grow4good_30mins = 0;
	int grow4good_break = 0;
	int grow4good_place = 0;
	int grow4good_trade = 0;
	int grow4good_sb = 0;
	bool grow4good_enter = false;
	int grow4good_provider = 0;
	int grow4good_provider2 = 0;

	int grow4good_geiger = 0;
	int grow4good_geiger2 = 0;
	bool honors = false;

	int Riftcape_flags = 0;

	int fa_code = 0;

	string game_version = "";
	bool updated_clothes = false;

	/*

		int pinata_day = 0;
		bool pinata_prize = false;
		bool pinata_claimed = false;

		int carnival_punch = 0;
		int carnival_credit = 0;*/

	bool adventure_begins = false;

	uint8_t sprite = 0, wild = 6;

	string login_pass = "";

	int pearl = 0;

	bool display_age = false, display_home = false;
	uint8_t xenonite = 0, xenonite_double_jump = 0, xenonite_high_jump = 0, xenonite_heat_resist = 0, xenonite_strong_punch = 0, xenonite_long_punch = 0, xenonite_long_build = 0, xenonite_speedy = 0, xenonite_powerups = 0;
};
#define pInfo(peer) ((Player*)(peer->data))
struct PlayerMoving {
	int32_t netID, effect_flags_check;
	int packetType, characterState, plantingTree, punchX, punchY, secondnetID;
	float x, y, XSpeed, YSpeed;
};

struct FishMoving {
	int8_t packetType = 31, stopped_fishing;
	int32_t netID, x, y;
};

BYTE* packGemsDrop(PlayerMoving* dataStruct, int size_ = 56) {
	BYTE* data = new BYTE[size_];
	memset(data, 0, size_);
	memcpy(data + 0, &dataStruct->packetType, 4);
	memcpy(data + 4, &dataStruct->netID, 4);
	memcpy(data + 12, &dataStruct->characterState, 4);
	memcpy(data + 20, &dataStruct->plantingTree, 4);
	memcpy(data + 24, &dataStruct->x, 4);
	memcpy(data + 28, &dataStruct->y, 4);
	memcpy(data + 32, &dataStruct->XSpeed, 4);
	memcpy(data + 36, &dataStruct->YSpeed, 4);
	memcpy(data + 44, &dataStruct->punchX, 4);
	memcpy(data + 48, &dataStruct->punchY, 4);
	return data;
}

BYTE* packPlayerMoving(PlayerMoving* dataStruct, int size_ = 56) {
	BYTE* data = new BYTE[size_];
	memset(data, 0, size_);
	memcpy(data + 0, &dataStruct->packetType, 4);
	memcpy(data + 4, &dataStruct->netID, 4);
	memcpy(data + 12, &dataStruct->characterState, 4);
	memcpy(data + 20, &dataStruct->plantingTree, 4);
	memcpy(data + 24, &dataStruct->x, 4);
	memcpy(data + 28, &dataStruct->y, 4);
	memcpy(data + 32, &dataStruct->XSpeed, 4);
	memcpy(data + 36, &dataStruct->YSpeed, 4);
	memcpy(data + 44, &dataStruct->punchX, 4);
	memcpy(data + 48, &dataStruct->punchY, 4);
	return data;
}
BYTE* packFishMoving(FishMoving* dataStruct, int size_ = 56) {
	BYTE* data = new BYTE[size_];
	memset(data, 0, size_);
	memcpy(data + 0, &dataStruct->packetType, 4);
	memcpy(data + 3, &dataStruct->stopped_fishing, 4);
	memcpy(data + 4, &dataStruct->netID, 4);
	memcpy(data + 44, &dataStruct->x, 4);
	memcpy(data + 48, &dataStruct->y, 4);
	return data;
}

void send_raw(ENetPeer* peer, int a1, void* packetData, int packetDataSize, int packetFlag) {
	ENetPacket* p;
	if (peer) {
		if (a1 == 4 && *((BYTE*)packetData + 12) & 8) {
			p = enet_packet_create(0, packetDataSize + *((DWORD*)packetData + 13) + 5, packetFlag);
			int four = 4;
			memcpy(p->data, &four, 4);
			memcpy((char*)p->data + 4, packetData, packetDataSize);
			memcpy((char*)p->data + packetDataSize + 4, 0, *((DWORD*)packetData + 13));
		}
		else {
			p = enet_packet_create(0, packetDataSize + 5, packetFlag);
			memcpy(p->data, &a1, 4);
			memcpy((char*)p->data + 4, packetData, packetDataSize);
		}
		enet_peer_send(peer, 0, p);
	}
}

void send_inventory(ENetPeer* peer) {
	if (pInfo(peer)->inventory_sent == false) pInfo(peer)->inventory_sent = true;
	__int16 inv_size = pInfo(peer)->inv.size(), packetLen = 66 + (inv_size * 4) + 4;
	BYTE* d_ = new BYTE[packetLen];
	memset(d_, 0, packetLen);
	*(__int8*)(d_) = 4;
	*(__int8*)(d_ + 4) = 9;
	*(__int8*)(d_ + 8) = -1;
	*(__int8*)(d_ + 16) = 8;
	*(int*)(d_ + 56) = 6 + (inv_size * 4) + 4;
	*(__int8*)(d_ + 60) = 1;
	*(int*)(d_ + 61) = inv_size;
	*(int*)(d_ + 65) = inv_size;
	vector<pair<uint16_t, uint8_t>> send_later;
	int offset = 67;
	for (__int16 i_ = 0; i_ < inv_size; ++i_) {
		if (pInfo(peer)->inv[i_].id == 7782 || pInfo(peer)->inv[i_].id == 9636 || pInfo(peer)->inv[i_].id == 9500 || pInfo(peer)->inv[i_].id == 9584 || pInfo(peer)->inv[i_].id == 9586 || pInfo(peer)->inv[i_].id == 9588 || pInfo(peer)->inv[i_].id == 9590 || pInfo(peer)->inv[i_].id == 9550 || pInfo(peer)->inv[i_].id == 9582 || pInfo(peer)->inv[i_].id == 9580 || pInfo(peer)->inv[i_].id == 9578 || pInfo(peer)->inv[i_].id == 9572 || pInfo(peer)->inv[i_].id == 9570 || pInfo(peer)->inv[i_].id == 9568 || pInfo(peer)->inv[i_].id == 9566 || pInfo(peer)->inv[i_].id == 9564 || pInfo(peer)->inv[i_].id == 9562 || pInfo(peer)->inv[i_].id == 9560 || pInfo(peer)->inv[i_].id == 9558 || pInfo(peer)->inv[i_].id == 9490 || pInfo(peer)->inv[i_].id == 9492 || pInfo(peer)->inv[i_].id == 9490 || pInfo(peer)->inv[i_].id == 9492 || pInfo(peer)->inv[i_].id == 9516 || pInfo(peer)->inv[i_].id == 9540) {
			send_later.push_back(make_pair(pInfo(peer)->inv[i_].id, pInfo(peer)->inv[i_].count));
		}
		else {
			*(__int16*)(d_ + offset) = pInfo(peer)->inv[i_].id; offset += 2;
			*(__int8*)(d_ + offset) = pInfo(peer)->inv[i_].count; offset += 2;
		}
	}
	ENetPacket* const p_ = enet_packet_create(d_, packetLen, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, p_);
	delete[] d_;
	if (send_later.size() > 0) {
		PlayerMoving data_{};
		data_.packetType = 13;
		for (int i = 0; i < send_later.size(); i++) {
			data_.plantingTree = send_later[i].first;
			BYTE* raw = packPlayerMoving(&data_);
			raw[3] = send_later[i].second;
			send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
			delete[]raw;
		}
	}
}