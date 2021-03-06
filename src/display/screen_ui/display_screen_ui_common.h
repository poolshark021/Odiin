#pragma once

#ifdef __cplusplus
extern "C" {
#endif


	#include "lvgl.h"

	//////////////////////////////////////////////////////////////////////////
	// MessageBox
	typedef struct msgbox_desc_s {
		lv_obj_t* parent;
		lv_group_t* group;
		const char* message_text;
		lv_event_cb_t event_cb;
		const char** button_map;
	} msgbox_desc_t;

	lv_obj_t* ui_common_msgbox_show(msgbox_desc_t* desc);

	//////////////////////////////////////////////////////////////////////////
	// Splash Window
	typedef struct splash_window_desc_s {
		const char* title;
		const char* instruction;
		const char* icon;
		const lv_img_dsc_t* image;
		const char* header_button_text;
		lv_event_cb_t header_button_cb;
		// variables below are filled by the api
		lv_obj_t* return_header_button_obj;
		lv_obj_t* return_window_obj;
		lv_obj_t* return_icon_obj;
		lv_obj_t* return_image_obj;
	} splash_window_desc_t;

	lv_obj_t* display_screen_ui_create_splash_window_screen(splash_window_desc_t* desc);

	//////////////////////////////////////////////////////////////////////////
	// Menu Option List
	typedef struct menu_option_desc_s menu_option_desc_t; // c is weird
	typedef void (*menu_option_cb_t)(menu_option_desc_t*, lv_group_t*);
	typedef struct menu_option_desc_s {
		const char* option;
		const void* prefix;
		menu_option_cb_t click_cb;
#if LV_USE_USER_DATA
		uint32_t user_flags;
#endif // LV_USE_USER_DATA
	} menu_option_desc_t;

	//////////////////////////////////////////////////////////////////////////
	// Styles
	void ui_common_set_window_button_style(lv_obj_t* button);

	//////////////////////////////////////////////////////////////////////////
	// Label Font switching
	void ui_common_set_label_font_icons64(lv_obj_t* label);
	void ui_common_set_label_font_theme_small(lv_obj_t* label);

	//////////////////////////////////////////////////////////////////////////
	// Callback typedefs
	typedef void (*ui_common_simple_cb)(void);
	typedef void (*ui_common_float_cb)(float);
	typedef void (*ui_common_uint8t_cb)(uint8_t);

	//////////////////////////////////////////////////////////////////////////
	// Callback Helpers
	void ui_common_cb_set_active_group(lv_group_t* group);
	void ui_common_up_down_focus_cb(lv_obj_t* obj, lv_event_t e);

	//////////////////////////////////////////////////////////////////////////
	// Macros for building UI screens
#define UI_CREATE_RETURN_TYPE lv_obj_t*
#define UI_FUNCTION(uiName, functionName) display_screen_ui_ ## uiName ## _ ## functionName
#define UI_DECLARE_CREATE(uiName) UI_CREATE_RETURN_TYPE UI_FUNCTION(uiName, create) (void)
#define UI_DECLARE_ACTIVATE(uiName) void UI_FUNCTION(uiName, activate) (lv_group_t* group)
#define UI_DECLARE_FUNCTION(uiName, func) UI_FUNCTION(uiName, func)
#define UI_DECLARE(uiName) UI_DECLARE_CREATE(uiName); UI_DECLARE_ACTIVATE(uiName)


	LV_FONT_CUSTOM_DECLARE;

#ifdef __cplusplus
} /* extern "C" */
#endif
