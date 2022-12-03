#include "../infrared_i.h"

static void infrared_scene_pinned_submenu_callback(void* context, uint32_t index) {
    Infrared* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void infrared_scene_remote_pinned_on_enter(void* context) {
    Infrared* infrared = context;
    Submenu* submenu = infrared->submenu;

    for(size_t i = 0; i < 10; i++) {
        if(strcmp(infrared->settings.saved_remotes[i].remote_name, "") != 0) {
            submenu_add_item(
                submenu,
                infrared->settings.saved_remotes[i].remote_name,
                i,
                infrared_scene_pinned_submenu_callback,
                infrared);
        }
    }

    submenu_set_header(submenu, "Pinned Remotes");
    submenu_set_selected_item(submenu, 0);
    view_dispatcher_switch_to_view(infrared->view_dispatcher, InfraredViewSubmenu);
}

bool infrared_scene_remote_pinned_on_event(void* context, SceneManagerEvent event) {
    Infrared* infrared = context;
    bool consumed = false;
    bool success = false;

    FuriString* file_path =
        furi_string_alloc_set(infrared->settings.saved_remotes[event.event].file_path);

    if(event.type == SceneManagerEventTypeCustom) {
        view_set_orientation(view_stack_get_view(infrared->view_stack), ViewOrientationVertical);
        view_dispatcher_switch_to_view(infrared->view_dispatcher, InfraredViewStack);

        infrared_show_loading_popup(infrared, true);
        success = infrared_remote_load(infrared->remote, file_path);
        infrared_show_loading_popup(infrared, false);

        if(success) {
            scene_manager_next_scene(infrared->scene_manager, InfraredSceneRemote);
        } else {
            scene_manager_previous_scene(infrared->scene_manager);
        }

        consumed = true;
    }

    furi_string_free(file_path);
    return consumed;
}

void infrared_scene_remote_pinned_on_exit(void* context) {
    Infrared* infrared = context;
    submenu_reset(infrared->submenu);
}
