#include "boss_rush.h"

extern uint8_t CFG_BOSS_RUSH_ENABLED;

void handle_boss_rush() {
	if (!CFG_BOSS_RUSH_ENABLED)
		return;
	
	boss_rush_init();
	boss_rush_set_scenes();
}

void boss_rush_init() {
	if (z64_game.scene_index == 0x34) { // Link's House
		z64_file.kokiri_sword = 1;
		z64_file.kokiri_tunic = 1;
		z64_file.kokiri_boots = 1;
	}
	else if(z64_game.scene_index == 0x11) { // Gohma
		z64_file.deku_shield = 1;
		
		z64_file.items[Z64_SLOT_STICK]			= Z64_ITEM_STICK;
		z64_file.ammo[Z64_SLOT_STICK]			= 10;
		z64_file.stick_upgrade					= 1;
		
		z64_file.items[Z64_SLOT_NUT]			= Z64_ITEM_NUT;
		z64_file.ammo[Z64_SLOT_NUT]				= 20;
		z64_file.nut_upgrade					= 1;
		
		z64_file.items[Z64_SLOT_SLINGSHOT]		= Z64_ITEM_SLINGSHOT;
		z64_file.ammo[Z64_SLOT_SLINGSHOT]		= 30;
		z64_file.bullet_bag						= 1;
	}
	else if(z64_game.scene_index == 0x12) { // King Dodongo
		z64_file.hylian_shield					= 1;
		z64_file.strength_upgrade				= 1;
		z64_file.zeldas_lullaby					= 1;
		z64_file.eponas_song					= 1;
		z64_file.suns_song						= 1;
		z64_file.sarias_song					= 1;
		
		z64_file.items[Z64_SLOT_BOMB]			= Z64_ITEM_BOMB;
		z64_file.ammo[Z64_SLOT_BOMB]			= 20;
		z64_file.bomb_bag						= 1;
		
		z64_file.items[Z64_SLOT_OCARINA]		= Z64_ITEM_FAIRY_OCARINA;
		z64_file.items[Z64_SLOT_BOTTLE_1]		= Z64_ITEM_FAIRY;
		z64_file.items[Z64_SLOT_CHILD_TRADE]	= Z64_ITEM_ZELDAS_LETTER;
	}
	else if(z64_game.scene_index == 0x13) { // Barinade
		z64_file.magic							= 1;
		z64_file.magic_acquired					= 1;
		z64_file.magic_capacity					= 0x30;
		z64_file.diving_upgrade					= 1;
		
		z64_file.items[Z64_SLOT_BOOMERANG]		= Z64_ITEM_BOOMERANG;
		z64_file.items[Z64_SLOT_BOTTLE_2]		= Z64_ITEM_FAIRY;
	}
	else if(z64_game.scene_index == 0x43) { // Temple of Time
		z64_file.link_age = 0;
		z64_game.link_age = 0;
		
		z64_file.song_of_time					= 1;
		z64_file.magic_capacity					= 0x30;
		z64_file.ammo[Z64_SLOT_BOMB]			= 20;
		
		z64_file.items[Z64_SLOT_OCARINA]		= Z64_ITEM_OCARINA_OF_TIME;
	}
	else if(z64_game.scene_index == 0x14) { // Phantom Ganon
		z64_file.master_sword					= 1;
		z64_file.minuet_of_forest				= 1;
		z64_file.song_of_storms					= 1;
		
		z64_file.items[Z64_SLOT_BOTTLE_3]		= Z64_ITEM_FAIRY;
		z64_file.items[Z64_SLOT_ADULT_TRADE]	= Z64_ITEM_POCKET_EGG;
	
		z64_file.items[Z64_SLOT_BOW]			= Z64_ITEM_BOW;
		z64_file.ammo[Z64_SLOT_BOW]				= 30;
		z64_file.quiver							= 1;
		
		z64_file.items[Z64_SLOT_HOOKSHOT]		= Z64_ITEM_HOOKSHOT;
	}
	else if(z64_game.scene_index == 0x15) { // Volvagia
		z64_file.goron_tunic					= 1;
		z64_file.bolero_of_fire					= 1;
		z64_file.prelude_of_light				= 1;
		
		z64_file.items[Z64_SLOT_BOTTLE_4]		= Z64_ITEM_FAIRY;
		z64_file.items[Z64_SLOT_HAMMER]			= Z64_ITEM_HAMMER;
		z64_file.items[Z64_SLOT_BOMBCHU]		= Z64_ITEM_BOMBCHU;
		z64_file.ammo[Z64_SLOT_BOMBCHU]			= 20;
	}
	else if(z64_game.scene_index == 0x16) { // Morpha
		z64_file.zora_tunic						= 1;
		z64_file.iron_boots						= 1;
		z64_file.serenade_of_water				= 1;
		
		z64_file.items[Z64_SLOT_HOOKSHOT]		= Z64_ITEM_LONGSHOT;
	}
	else if(z64_game.scene_index == 0x18) { // Bongo Bongo
		z64_file.hover_boots					= 1;
		z64_file.nocturne_of_shadow				= 1;
		
		z64_file.items[Z64_SLOT_FIRE_ARROW]		= Z64_ITEM_FIRE_ARROW;
		z64_file.items[Z64_SLOT_ICE_ARROW]		= Z64_ITEM_ICE_ARROW;
		z64_file.items[Z64_SLOT_LENS]			= Z64_ITEM_LENS;
	}
	else if(z64_game.scene_index == 0x17) { // Twinrova
		z64_file.mirror_shield					= 1;
		z64_file.requiem_of_spirit				= 1;
		z64_file.strength_upgrade				= 2;
	}
	else if(z64_game.scene_index == 0x19) { // Ganondorf
		z64_file.strength_upgrade				= 3;
		
		z64_file.items[Z64_SLOT_LIGHT_ARROW]	= Z64_ITEM_LIGHT_ARROW;
	}
}

void boss_rush_set_scenes() {
	if (z64_game.scene_index == 0x34 || z64_game.scene_index == 0x43) {
		z64_boss_rush_1_1 = 0x1100;
		z64_boss_rush_1_2 = 0x1100;
		z64_boss_rush_1_3 = 0x1100;
		z64_boss_rush_1_4 = 0x1100;
		
		z64_boss_rush_2_1 = 0x1200;
		z64_boss_rush_2_2 = 0x1200;
		z64_boss_rush_2_3 = 0x1200;
		z64_boss_rush_2_4 = 0x1200;
		
		z64_boss_rush_3_1 = 0x1300;
		z64_boss_rush_3_2 = 0x1300;
		z64_boss_rush_3_3 = 0x1300;
		z64_boss_rush_3_4 = 0x1300;
		
		z64_boss_rush_4_1 = 0x4300;
		z64_boss_rush_4_2 = 0x4300;
		z64_boss_rush_4_3 = 0x4300;
		z64_boss_rush_4_4 = 0x4300;
		
		z64_boss_rush_5_1 = 0x1500;
		z64_boss_rush_5_2 = 0x1500;
		z64_boss_rush_5_3 = 0x1500;
		z64_boss_rush_5_4 = 0x1500;
		
		z64_boss_rush_6_1 = 0x1600;
		z64_boss_rush_6_2 = 0x1600;
		z64_boss_rush_6_3 = 0x1600;
		z64_boss_rush_6_4 = 0x1600;
		
		z64_boss_rush_7_1 = 0x4300;
		z64_boss_rush_7_2 = 0x4300;
		
		z64_boss_rush_8_1 = 0x1400;
		z64_boss_rush_8_2 = 0x1400;
	}
	
	
}