//#include "triforce.h"
//#include "dungeon_info.h"
//#include "file_select.h"
//#include "get_items.h"
//#include "models.h"
#include "gfx.h"
#include "text.h"
#include "util.h"
#include "dpad.h"
#include "actor.h"
#include "arrow_cycle.h"
#include "misc_colors.h"
#include "hud_colors.h"
#include "z64.h"
//#include "chests.h"
//#include "ganon.h"
//#include "twinrova.h"
//#include "ganon_boss_key.h"
//#include "extern_ctxt.h"
//#include "weather.h"

void Gameplay_InitSkybox(z64_game_t* globalCtx, int16_t skyboxId);

void c_init() {
    heap_init();
    gfx_init();
    text_init();
    //item_overrides_init();
    //models_init();
}

void before_game_state_update() {
    arrow_cycle_handle(&z64_link, &z64_game);
    //handle_pending_items();
    handle_dpad();
    update_misc_colors();
    update_hud_colors();
    //process_extern_ctxt();
}

void after_game_state_update() {
    get_health(z64_game.target_actor);
    elite_enemies(&z64_game);
    //draw_dungeon_info(&(z64_ctxt.gfx->overlay));
    //draw_triforce_count(&(z64_ctxt.gfx->overlay));
    //give_ganon_boss_key();
}

void before_skybox_init(z64_game_t* game, int16_t skyboxId) {
    //override_weather_state();
    Gameplay_InitSkybox(game, skyboxId);
}

void after_scene_init() {
    //check_ganon_entry();
    //clear_twinrova_vars();
    //models_reset();
    //extern_scene_init();
}
