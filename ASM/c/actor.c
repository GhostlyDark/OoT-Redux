#include "gfx.h"
#include "actor.h"

extern uint16_t CURR_ACTOR_SPAWN_INDEX;
extern uint16_t play_sfx;

uint16_t currentHealth = 0;
actor_backup_t actorBackup = { .scene = -1, .id = 0, .x = 0, .y = 0, .z = 0, .xr = 0, .yr = 0, .zr = 0, .var = 0 };

void get_health(z64_actor_t* actor) {
    if (!OPTION_ACTIVE(1, SAVE_SHOW_HEALTH, CFG_DEFAULT_SHOW_HEALTH) || z64_game.pause_ctxt.state != 0 || actor == NULL) {
        currentHealth = 0;
        return;
    }
    
    uint16_t id  = actor->actor_id;
    uint8_t type = actor->actor_type;
    if (type != ACTORTYPE_ENEMY && type != ACTORTYPE_BOSS && id != EN_IK) {
        currentHealth = 0;
        return;
    }
    
    if (id == BOSS_DODONGO)
        currentHealth = HP_KING_DODONGO;
    else if (id == BOSS_VA)
        currentHealth = HP_BARINADE;
    else if (id == BOSS_GANONDROF)
        currentHealth = HP_PHANTOM_GANON;
    else if (id == BOSS_MO)
        currentHealth = HP_MORPHA;
    else if (id == BOSS_TW)
        currentHealth = HP_TWINROVA;
    else if (id == BOSS_GANON)
        currentHealth = HP_GANONDORF;
    else if (id == BOSS_GANON2)
        currentHealth = HP_GANON;
    else if (id == EN_FD) {
        if (actor->child)
            currentHealth = actor->child->health;
    }
    else if (id == BOSS_FD2) {
        if (actor->parent)
            currentHealth = actor->parent->health;
    }
    else currentHealth = actor->health;
    
    currentHealth = currentHealth & 0xFF;
}

void draw_health(z64_disp_buf_t *db) {
    if (!OPTION_ACTIVE(1, SAVE_SHOW_HEALTH, CFG_DEFAULT_SHOW_HEALTH) || currentHealth == 0 || z64_file.hud_visibility_mode == 1 || !CAN_DRAW_HUD || !CAN_CONTROL_LINK || z64_textbox != 0)
        return;
    
    uint16_t x = 70;
    uint16_t y = 205;
    
    uint8_t count = 0;
    int16_t health = currentHealth;
            
    while (health != 0) {
        health /= 10;
        count++;
    }
            
    uint8_t healthArray[count];
    count = 0;    
    health = currentHealth;
    while (health != 0) {
        healthArray[count] = health % 10;
        health /= 10;
        count++;
    }
    
    gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    sprite_load_and_draw(db, &linkhead_skull_sprite, 1, x, y, 16, 16);
    
    for (uint8_t i=0; i<count; i++)
        sprite_load_and_draw(db, &counter_digit_sprite, healthArray[count-i-1], x + 16 + 8 * i, y + 1, 8, 16);
}

void restore_spawner_backup(z64_game_t *game) {
    if (actorBackup.scene < 0 || z64_link.current_mask == PLAYER_MASK_SKULL || z64_link.current_mask == PLAYER_MASK_GORON)
        return;
    else if (actorBackup.scene != z64_game.scene_index)
        actorBackup.scene = -1;
    else {
        z64_SpawnActor(&game->actor_ctxt, game, actorBackup.id, actorBackup.x, actorBackup.y, actorBackup.z, actorBackup.xr, actorBackup.yr, actorBackup.zr, actorBackup.var);
        actorBackup.scene = -1;
    }
}

void remove_enemy_spawner() {
    if (actorBackup.scene >= 0 || z64_link.current_mask != PLAYER_MASK_SKULL)
        return;
    
    z64_actor_t *actor = z64_game.actor_list[ACTORTYPE_PROP].first;
    while (actor != NULL) {
        if (actor->actor_id == EN_ENCOUNT1) {
            set_actor_backup(actor);
            break;
        }
        actor = actor->next;
    }
}

void remove_falling_rocks_spawner() {
    if (actorBackup.scene >= 0 || z64_link.current_mask != PLAYER_MASK_GORON)
        return;
    
    z64_actor_t *actor = z64_game.actor_list[ACTORTYPE_ENEMY].first;
    while (actor != NULL) {
        if (actor->actor_id == EN_ENCOUNT2) {
            set_actor_backup(actor);
            break;
        }
        actor = actor->next;
    }
}

void set_actor_backup(z64_actor_t *actor) {
    actorBackup.scene = z64_game.scene_index;
    actorBackup.id    = actor->actor_id;
    actorBackup.x     = actor->pos_1.x;
    actorBackup.y     = actor->pos_1.y;
    actorBackup.z     = actor->pos_1.z;
    actorBackup.xr    = actor->rot_init.x;
    actorBackup.yr    = actor->rot_init.y;
    actorBackup.zr    = actor->rot_init.z;
    actorBackup.var   = actor->variable;
    z64_ActorKill(actor);
}

// #define rd_obj_idx(actor) ((int8_t*)actor)[0x90] // redead_actor->rd_obj_idx
// #define OBJECT_RD 0x0098

//typedef struct {
//    char unk_00_[0x14ED];   /* 0x0000 */
//    uint16_t freeze_timer;  /* 0x14EE */
//} redead_data_t;

//uint8_t has_redead_actor_file(z64_actor_t* actor, z64_game_t* game) {
//    //return (rd_obj_idx(actor) > 0) && ((rd_obj_idx(actor) = z64_ObjectIndex(&game->obj_ctxt, OBJECT_RD)) > 0) && z64_ObjectIsLoaded(&game->obj_ctxt, rd_obj_idx(actor));
//}

/*void set_redead_freeze() {
    if (!CAN_CONTROL_LINK)
        return;
    
    z64_game_t* game = &z64_game;
    int16_t index = z64_ObjectIndex(&game->obj_ctxt, OBJECT_RD);
    if (index > 0) {
        play_sfx = 0x4813;
        
        uint32_t offset = (*(uint8_t*) game->obj_ctxt.objects[index].getfile.vrom_addr);
        z64_file.rupees = offset;
        
        //uint32_t test1 = (*(uint8_t*) 0x801DB0CD);
        //z64_file.rupees = test1;
    }
    
    // CD869E - CD71B0 = 14EE
}*/

void elite_enemies() {
    if (!OPTION_ACTIVE(2, SAVE_RANDOM_ENEMIES, CFG_DEFAULT_RANDOM_ENEMIES) || !CAN_CONTROL_LINK)
        return;
    
    z64_actor_t *enemy = z64_game.actor_list[ACTORTYPE_ENEMY].first;
    while (enemy != NULL) {
        uint16_t id = enemy->actor_id;
        if (enemy->rot_init.z < 0x8000 && enemy->health > 1 && id != EN_ST && id != EN_ANUBICE && id != EN_FZ) {
            float rand = z64_Rand_ZeroOne();
            if (rand >= 0.4) {
                float factor = 1.5;
                if (rand <= 0.8)
                    factor = 1.75;
                else if (rand <= 1.0)
                    factor = 2;
                
                uint8_t add = (enemy->health & 20) * factor;
                if (factor == 1.5 && add < 1)
                    add = 1;
                else if (factor == 1.75 && add < 2)
                    add = 2;
                else if (factor == 2 && add < 3)
                    add = 3;
                enemy->health += add;
            }
            enemy->rot_init.z += 0x8000;
        }
        enemy = enemy->next;
    }
}

z64_actor_t *replace_enemy_type(z64_actor_t *spawned) {
    if (!OPTION_ACTIVE(2, SAVE_RANDOM_ENEMIES, CFG_DEFAULT_RANDOM_ENEMIES))
        return spawned;
    if (spawned->actor_type != ACTORTYPE_ENEMY)
        return spawned;
        
    float rand = z64_Rand_ZeroOne();
    if (rand < 0.65)
        return spawned;
    
    switch (spawned->actor_id) {
        case EN_TEST:
            if (spawned->variable == EN_TEST_CEILING)
                spawned->variable == EN_TEST_INVISIBLE;
            break;
        
        case EN_FIREFLY:
            if (spawned->variable & 4 == EN_FIREFLY_NORMAL_FLY) {
                rand = z64_Rand_ZeroOne();
                if (rand >= 0.5)
                    spawned->variable = ( ( (spawned->variable >> 8) & 0xFF) << 8) + EN_FIREFLY_FIRE_FLY;
                else spawned->variable = ( ( (spawned->variable >> 8) & 0xFF) << 8) + EN_FIREFLY_ICE_FLY;
            }
            else if (spawned->variable & 4 == EN_FIREFLY_NORMAL_ROOST)
                spawned->variable = ( ( (spawned->variable >> 8) & 0xFF) << 8) + EN_FIREFLY_FIRE_ROOST;
            
            if (spawned->variable >> 8 & 0x80 == EN_FIREFLY_VISIBLE) {
                rand = z64_Rand_ZeroOne();
                if (rand >= 0.5)
                    spawned->variable = EN_FIREFLY_INVISIBLE + spawned->variable & 4;
            }
            break;
        
        case EN_TITE:
            if (spawned->variable == EN_TITE_RED)
                spawned->variable = EN_TITE_BLUE;
            spawned->unk_0D_ += 0x10;
            break;
        
        case EN_REEBA:
            if (spawned->variable == EN_REEBA_SMALL)
                spawned->variable = EN_REEBA_BIG;
            break;
        
        case EN_ZF:
            if (spawned->variable & 0xFF == EN_ZF_LIZALFOS)
                spawned->variable = ( ( (spawned->variable >> 8) & 0xFF) << 8) + EN_ZF_DINOLFOS;
            break;
        
        case EN_ST:
            if (spawned->variable == EN_ST_NORMAL)
                spawned->variable = EN_ST_INVISIBLE;
            break;
        
        case EN_FLOORMAS:
            if (spawned->variable == EN_FLOORMAS_VISIBLE)
                spawned->variable = EN_FLOORMAS_INVISIBLE;
            break;
        
        case EN_RD:
            if (spawned->variable & 0xFF == EN_RD_REDEAD)
                spawned->variable = ( ( (spawned->variable >> 8) & 0xFF) << 8) + EN_RD_GIBDO;
            break;
        
        case EN_IK:
            if (spawned->variable & 0xFF == EN_IK_BLACK)
                spawned->variable = ( ( (spawned->variable >> 8) & 0xFF) << 8) + EN_IK_WHITE;
            break;
        
        default:
            break;
    }
    
    return spawned;
}

z64_actor_t *Actor_SpawnEntry_Hack(void *actorCtx, ActorEntry *actorEntry, z64_game_t *globalCtx) {
    z64_actor_t *spawned = NULL;
    spawned = z64_SpawnActor(actorCtx, globalCtx, actorEntry->id, actorEntry->pos.x, actorEntry->pos.y, actorEntry->pos.z, actorEntry->rot.x, actorEntry->rot.y, actorEntry->rot.z, actorEntry->params);
    
    if (spawned)
        spawned = replace_enemy_type(spawned);
    return spawned;
}

//void Actor_SetWorldToHome_End(z64_actor_t *actor) {
//}

//void Actor_After_UpdateAll_Hack(z64_actor_t *actor, z64_game_t *game) {
//}