#include <lib/toolbox/value_index.h>

#include "../infrared_i.h"
#include "../infrared_remote.h"

#define SCENE_EVENT_SELECT_EDIT_PIN 0
#define SCENE_EVENT_SELECT_ADD_BUTTON 1
#define SCENE_EVENT_SELECT_RENAME_BUTTON 2
#define SCENE_EVENT_SELECT_DELETE_BUTTON 3
#define SCENE_EVENT_SELECT_RENAME_REMOTE 4
#define SCENE_EVENT_SELECT_DELETE_REMOTE 5

const char* const pin_text[2] = {
    "No",
    "Yes",
};
const bool pin_value = false;

static void infrared_scene_edit_var_list_enter_callback(void* context, uint32_t index) {
    Infrared* infrared = context;
    view_dispatcher_send_custom_event(infrared->view_dispatcher, index);
}

static void infrared_edit_scene_pin_changed(VariableItem* item) {
    Infrared* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);

    variable_item_set_current_value_text(item, pin_text[index]);
    if(index == 1) {
        infrared_remote_pin(app->remote, &app->settings);
    } else {
        infrared_remote_unpin(app->remote, &app->settings);
    }
}

void infrared_scene_edit_on_enter(void* context) {
    Infrared* infrared = context;

    VariableItemList* variable_item_list = infrared->variable_item_list;
    VariableItem* item;
    uint8_t value_index;

    bool pinned = infrared_remote_pin_state_alloc(infrared->remote, infrared->settings);

    item = variable_item_list_add(
        variable_item_list, "Pin", 2, infrared_edit_scene_pin_changed, infrared);
    variable_item_list_add(variable_item_list, "Add Button", 1, NULL, NULL);
    variable_item_list_add(variable_item_list, "Rename Button", 1, NULL, NULL);
    variable_item_list_add(variable_item_list, "Delete Button", 1, NULL, NULL);
    variable_item_list_add(variable_item_list, "Rename remote", 1, NULL, NULL);
    variable_item_list_add(variable_item_list, "Delete remote", 1, NULL, NULL);

    variable_item_list_set_enter_callback(
        variable_item_list, infrared_scene_edit_var_list_enter_callback, infrared);

    value_index = pinned ? 1 : 0;

    variable_item_set_current_value_index(item, value_index);
    variable_item_set_current_value_text(item, pin_text[value_index]);
    view_dispatcher_switch_to_view(infrared->view_dispatcher, InfraredViewVarItemList);
}

bool infrared_scene_edit_on_event(void* context, SceneManagerEvent event) {
    Infrared* infrared = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case SCENE_EVENT_SELECT_EDIT_PIN:
            consumed = true;
            break;
        case SCENE_EVENT_SELECT_ADD_BUTTON:
            infrared->app_state.is_learning_new_remote = false;
            scene_manager_next_scene(infrared->scene_manager, InfraredSceneLearn);
            consumed = true;
            break;
        case SCENE_EVENT_SELECT_RENAME_BUTTON:
            infrared->app_state.edit_target = InfraredEditTargetButton;
            infrared->app_state.edit_mode = InfraredEditModeRename;
            scene_manager_next_scene(infrared->scene_manager, InfraredSceneEditButtonSelect);
            consumed = true;
            break;
        case SCENE_EVENT_SELECT_DELETE_BUTTON:
            infrared->app_state.edit_target = InfraredEditTargetButton;
            infrared->app_state.edit_mode = InfraredEditModeDelete;
            scene_manager_next_scene(infrared->scene_manager, InfraredSceneEditButtonSelect);
            consumed = true;
            break;
        case SCENE_EVENT_SELECT_RENAME_REMOTE:
            infrared->app_state.edit_target = InfraredEditTargetRemote;
            infrared->app_state.edit_mode = InfraredEditModeRename;
            scene_manager_next_scene(infrared->scene_manager, InfraredSceneEditRename);
            consumed = true;
            break;

        case SCENE_EVENT_SELECT_DELETE_REMOTE:
            infrared->app_state.edit_target = InfraredEditTargetRemote;
            infrared->app_state.edit_mode = InfraredEditModeDelete;
            scene_manager_next_scene(infrared->scene_manager, InfraredSceneEditDelete);
            consumed = true;
            break;
        }
    }

    return consumed;
}

void infrared_scene_edit_on_exit(void* context) {
    Infrared* infrared = context;
    variable_item_list_reset(infrared->variable_item_list);
    INFRARED_SETTINGS_SAVE(&infrared->settings);
}
