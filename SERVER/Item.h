#pragma once
using namespace std;
#define Property_Zero 0
#define Property_NoSeed 1
#define Property_Dropless 2
#define Property_Beta 4
#define Property_Mod 8
#define Property_Chemical 12
#define Property_Untradable 16
#define Property_Wrenchable 32
#define Property_MultiFacing 64
#define Property_Permanent 128
#define Property_AutoPickup 256
#define Property_WorldLock 512
#define Property_NoSelf 1024
#define Property_RandomGrow 2048
#define Property_Public 4096
#define Property_Foreground 8192
enum ClothTypes {
	HAIR,
	SHIRT,
	PANTS,
	FEET,
	FACE,
	HAND,
	BACK,
	MASK,
	NECKLACE,
	ANCES,
	NONE
};
enum BlockTypes
{
	FOREGROUND,
	BACKGROUND,
	CONSUMABLE,
	SEED,
	PAIN_BLOCK,
	BEDROCK,
	MAIN_DOOR,
	SIGN,
	DOOR,
	CLOTHING,
	FIST,
	WRENCH,
	CHECKPOINT,
	LOCK,
	GATEWAY,
	TREASURE,
	WEATHER,
	TRAMPOLINE,
	TOGGLE_FOREGROUND,
	CHEMICAL_COMBINER,
	SWITCH_BLOCK,
	SFX_FOREGROUND,
	RANDOM_BLOCK,
	PORTAL,
	PLATFORM,
	MAILBOX,
	MAGIC_EGG,
	CRYSTAL,
	GEMS,
	DEADLY,
	CHEST,
	FACTION,
	BULLETIN_BOARD,
	BOUNCY,
	ANIM_FOREGROUND,
	COMPONENT,
	SUCKER,
	FISH,
	STEAM,
	GROUND_BLOCK,
	DISPLAY,
	STORAGE,
	VENDING,
	DONATION,
	PHONE,
	SEWINGMACHINE,
	CRIME_VILLAIN,
	PROVIDER,
	ADVENTURE,
	UNKNOWN
};
struct ItemDB
{

	int grindable_count = 0;
	int grindable_prize = 0;
	string event_name = "";
	int event_total = 0;
	int oprc = 0;
	int flagmay = 256;
	int zombieprice = 0;
	int surgeryprice = 0;
	int pwl = 0;
	bool blocked_place = false, unobtainable = false;
	int newdropchance = 0;
	int gtwl = 0;
	int u_gtwl = 0;
	int chance = 0;
	int max_gems2 = 0;
	int max_gems3 = 0;
	uint8_t box_size = 0;
	vector<int> randomitem;


	vector<int> epic, rare, uncommon;

	vector<pair<int, int>> noob_item, rare_item;
	int fish_max_lb = 0;
	string hand_scythe_text = "";
	int consume_needed = 0;
	int consume_prize = 0;
	string emoji = "";
	bool farmable = false;
	int textureHash = 0;
	int val1;
	int dropChance = 0;
	int seedColor = 0;
	int seedOverlayColor = 0;
	int extraFileHash = 0;
	int audioVolume = 0;
	bool mooncake = false;
	int effect = 0;
	int id = 0;
	bool untradeable = 0;
	int breakHits = 25;
	int actionType = 0;
	int chi = 4;
	int ext_weather = 0;
	int growTime = 0;
	int properties = 0;
	bool multiple_sides = 0;
	bool spirit = 0;
	bool donation = 0;
	bool solid = 0;
	bool infinitymachine = 0;
	bool entrance = 0;
	bool vipentrance = 0;
	bool timer = 0;
	bool dshelf = 0;
	bool trans = 0;
	bool xeno = 0;
	bool bulletin_board = 0;
	bool magic_egg = 0;
	bool bunny_egg = 0;
	bool growscan = 0;
	bool security_camera = 0;
	bool storage_box = 0;
	bool heart_monitor = 0;
	bool mannequin = 0;
	bool charger = 0;
	bool well_of_love = 0;
	bool portrait = 0;
	bool easel = 0;
	bool mailbox = 0;
	bool fish_port = 0;
	bool spotlight = 0;
	bool vall_mount = 0;
	bool magplant = 0;
	bool item_sucker = 0;
	bool mag_multiple = 0;
	bool can_be_changed_to_public = 0;
	bool can_be_changed_two_types = 0;
	bool toggleable = 0;
	bool trickster = 0;
	char seedBase = 0;
	char seedOverlay = 0;
	char treeBase = 0;
	char treeLeaves = 0;
	char editableType = 0;
	char hitSoundType = 0;
	char itemCategory = 0;
	char itemKind = 0;
	char textureX = 0;
	char textureY = 0;
	char spreadType = 0;
	char isStripeyWallpaper = 0;
	int collisionType = 0; // buvo char collisionType = 0;
	char clothingType = 0;
	short val2;
	short isRayman = 0;
	int r_1 = 0, r_2 = 0, max_gems = 0;
	unsigned char maxAmount = 0;
	string on_equip = "", on_remove = "", playmod = "";
	string extraFile = "";
	string petName = "";
	string petPrefix = "";
	string petSuffix = "";
	string petAbility = "";
	string extraOptions = "";
	string texture2 = "";
	string extraOptions2 = "";
	string punchOptions = "";
	string name = "";
	string texture = "";
	string description = "This item has no description.";
	string MultiFacing = "This item can be placed in both directions.";
	int16_t rarity = 0;
	ClothTypes clothType{};
	BlockTypes blockType{};
	string ori_name = "";
};


vector<ItemDB> items;