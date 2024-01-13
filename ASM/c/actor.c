#include "gfx.h"
#include "actor.h"

extern uint16_t CURR_ACTOR_SPAWN_INDEX;
extern uint16_t play_sfx;

uint16_t currentHealth = 0;

void get_health(z64_actor_t* actor) {
    if (!SAVE_SHOW_HEALTH || z64_game.pause_ctxt.state != 0 || actor == NULL) {
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
    if (!SAVE_SHOW_HEALTH || currentHealth == 0 || z64_file.hud_visibility_mode == 1 || !CAN_DRAW_HUD || !CAN_CONTROL_LINK || z64_textbox != 0)
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
    sprite_load(db, &linkhead_skull_sprite, 1, 1);
    sprite_draw(db, &linkhead_skull_sprite, 0, x, y, 16, 16);
    
    for (uint8_t i=0; i<count; i++) {
        sprite_load(db, &counter_digit_sprite, healthArray[count-i-1], 1);
        sprite_draw(db, &counter_digit_sprite, 0, x + 16 + 8 * i,  y + 1, 8, 16);
    }
}

void elite_enemies(z64_game_t *game) {
    if (!SAVE_RANDOM_ENEMIES)
        return;
    if (!CAN_CONTROL_LINK)
        return;
    
    z64_actor_t *enemy = z64_game.actor_list[ACTORTYPE_ENEMY].first;
    while (enemy != NULL) {
        if (enemy->rot_init.z < 0x8000 && enemy->health != 0) {
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
    if (!SAVE_RANDOM_ENEMIES)
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