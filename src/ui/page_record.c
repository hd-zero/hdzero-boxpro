#include "page_record.h"

#include <stdlib.h>

#include <minIni.h>

#include "../core/common.hh"
#include "core/settings.h"
#include "page_common.h"
#include "ui/ui_style.h"

static btn_group_t btn_group0;
static btn_group_t btn_group1;
static btn_group_t btn_group2;
static btn_group_t btn_group3;
static btn_group_t btn_group4;

static lv_coord_t col_dsc[] = {106, 132, 132, 106, 80, 80, LV_GRID_TEMPLATE_LAST};
static lv_coord_t row_dsc[] = {40, 40, 40, 40, 40, 40, 40, 40, 40, 40, LV_GRID_TEMPLATE_LAST};

static void update_visibility() {
    btn_group_enable(&btn_group4, btn_group3.current == 0);

    if (btn_group3.current == 0) {
        lv_obj_add_flag(pp_record.p_arr.panel[4], FLAG_SELECTABLE);
    } else {
        lv_obj_clear_flag(pp_record.p_arr.panel[4], FLAG_SELECTABLE);
    }
}

static lv_obj_t *page_record_create(lv_obj_t *parent, panel_arr_t *arr) {
    lv_obj_t *page = lv_menu_page_create(parent, NULL);
    lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(page, 780, 600);
    lv_obj_add_style(page, &style_subpage, LV_PART_MAIN);
    lv_obj_set_style_pad_top(page, 62, 0);

    lv_obj_t *section = lv_menu_section_create(page);
    lv_obj_add_style(section, &style_submenu, LV_PART_MAIN);
    lv_obj_set_size(section, 780, 600);

    create_text(NULL, section, false, "Record Option:", LV_MENU_ITEM_BUILDER_VARIANT_2);

    lv_obj_t *cont = lv_obj_create(section);
    lv_obj_set_size(cont, 780, 600);
    lv_obj_set_pos(cont, 0, 0);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(cont, &style_context, LV_PART_MAIN);

    lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);

    create_select_item(arr, cont);

    create_btn_group_item(&btn_group0, cont, 2, "Record Mode", "Auto", "Manual", "", "", 0);
    create_btn_group_item(&btn_group1, cont, 2, "Record Format", "MP4", "TS", "", "", 1);
    create_btn_group_item(&btn_group2, cont, 2, "Record OSD", "Yes", "No", "", "", 2);
    create_btn_group_item(&btn_group3, cont, 2, "Record Audio", "Yes", "No", "", "", 3);
    create_btn_group_item(&btn_group4, cont, 3, "Audio Source", "Mic", "Line In", "A/V In", "", 4);
    create_label_item(cont, "< Back", 1, 5, 1);

    btn_group_set_sel(&btn_group0, g_setting.record.mode_manual ? 1 : 0);
    btn_group_set_sel(&btn_group1, g_setting.record.format_ts ? 1 : 0);
    btn_group_set_sel(&btn_group2, g_setting.record.osd ? 0 : 1);
    btn_group_set_sel(&btn_group3, g_setting.record.audio ? 0 : 1);
    btn_group_set_sel(&btn_group4, g_setting.record.audio_source);

    lv_obj_t *label2 = lv_label_create(cont);
    lv_label_set_text(label2, "MP4 format requires properly closing files or the files will be corrupt. \nTS format is highly recommended.");
    lv_obj_set_style_text_font(label2, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_align(label2, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_text_color(label2, lv_color_hex(TEXT_COLOR_DEFAULT), 0);
    lv_obj_set_style_pad_top(label2, 8, 0);
    lv_label_set_long_mode(label2, LV_LABEL_LONG_WRAP);
    lv_obj_set_grid_cell(label2, LV_GRID_ALIGN_START, 1, 4,
                         LV_GRID_ALIGN_START, 6, 3);

    update_visibility();

    return page;
}

static void page_record_on_click(uint8_t key, int sel) {
    if (sel == 0) {
        btn_group_toggle_sel(&btn_group0);
        g_setting.record.mode_manual = btn_group_get_sel(&btn_group0);
        settings_put_bool("record", "mode_manual", g_setting.record.mode_manual);
    } else if (sel == 1) {
        btn_group_toggle_sel(&btn_group1);
        g_setting.record.format_ts = btn_group_get_sel(&btn_group1);
        settings_put_bool("record", "format_ts", g_setting.record.format_ts);
        if (g_setting.record.format_ts)
            ini_puts("record", "type", "ts", REC_CONF);
        else
            ini_puts("record", "type", "mp4", REC_CONF);
    } else if (sel == 2) {
        btn_group_toggle_sel(&btn_group2);
        g_setting.record.osd = !btn_group_get_sel(&btn_group2);
        settings_put_bool("record", "osd", g_setting.record.osd);
    } else if (sel == 3) {
        btn_group_toggle_sel(&btn_group3);
        g_setting.record.audio = !btn_group_get_sel(&btn_group3);
        settings_put_bool("record", "audio", g_setting.record.audio);
        update_visibility();
    } else if (sel == 4) {
        btn_group_toggle_sel(&btn_group4);
        g_setting.record.audio_source = btn_group_get_sel(&btn_group4);
        ini_putl("record", "audio_source", g_setting.record.audio_source, SETTING_INI);
    }
}

page_pack_t pp_record = {
    .p_arr = {
        .cur = 0,
        .max = 6,
    },
    .name = "Record Option",
    .create = page_record_create,
    .enter = NULL,
    .exit = NULL,
    .on_created = NULL,
    .on_update = NULL,
    .on_roller = NULL,
    .on_click = page_record_on_click,
    .on_right_button = NULL,
};