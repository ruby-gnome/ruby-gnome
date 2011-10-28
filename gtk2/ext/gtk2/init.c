/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2001 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "global.h"
#include "gmodule.h"

VALUE treeiter_set_value_table;

extern void Init_gtk2(void);

void
Init_gtk2(void)
{
    /*
     * For Gtk::TreeModel, Gtk::TreeIter. 
     * They should be initialized on this timing.
     */
    treeiter_set_value_table = rb_hash_new();
    rb_global_variable(&treeiter_set_value_table);

    Init_gtk_gdk();
    Init_gtk_gtk();

    Init_gtk_gdk_atom();
    Init_gtk_gdk_cairo();
    Init_gtk_gdk_color();
    Init_gtk_gdk_colormap();
    Init_gtk_gdk_const();
    Init_gtk_gdk_cursor();
    Init_gtk_gdk_device();
    Init_gtk_gdk_display();
    Init_gtk_gdk_display_manager();
    Init_gtk_gdk_dragcontext();
    Init_gtk_gdk_event();
    Init_gtk_gdk_geometry();
    Init_gtk_gdk_image();
    Init_gtk_gdk_input();
    Init_gtk_gdk_keymap();
    Init_gtk_gdk_keyval();
    Init_gtk_gdk_pango();
    Init_gtk_gdk_pangorenderer();
    Init_gtk_gdk_pixbuf();
    Init_gtk_gdk_pixmap();
    Init_gtk_gdk_property();
    Init_gtk_gdk_rectangle();
    Init_gtk_gdk_region();
    Init_gtk_gdk_rgb();
    Init_gtk_gdk_screen();
    Init_gtk_gdk_selection();
    Init_gtk_gdk_threads();
    Init_gtk_gdk_timecoord();
    Init_gtk_gdk_visual();
    Init_gtk_gdk_window();
    Init_gtk_gdk_windowattr();
    Init_gtk_gdk_x11();
    Init_gtk_aboutdialog();
    Init_gtk_accelerator();
    Init_gtk_accel_group();
    Init_gtk_accel_group_entry();
    Init_gtk_accel_key();
    Init_gtk_accel_label();
    Init_gtk_accel_map();
    Init_gtk_accessible();
    Init_gtk_action();
    Init_gtk_actiongroup();
    Init_gtk_adjustment();
    Init_gtk_alignment();
    Init_gtk_allocation();
    Init_gtk_arrow();
    Init_gtk_aspect_frame();
    Init_gtk_assistant();
    Init_gtk_button_box();
    Init_gtk_bin();
    Init_gtk_bindings();
    Init_gtk_border();
    Init_gtk_box();
    Init_gtk_buildable();
    Init_gtk_builder();
    Init_gtk_button();
    Init_gtk_calendar();
    Init_gtk_celleditable();
    Init_gtk_celllayout();
    Init_gtk_cellrenderer();
    Init_gtk_cellrendereraccel();
    Init_gtk_cellrenderercombo();
    Init_gtk_cellrendererpixbuf();
    Init_gtk_cellrendererprogress();
    Init_gtk_cellrendererspin();
    Init_gtk_cellrendererspinner();
    Init_gtk_cellrenderertext();
    Init_gtk_cellrenderertoggle();
    Init_gtk_cellview();
    Init_gtk_check_button();
    Init_gtk_check_menu_item();
    Init_gtk_clipboard();
    Init_gtk_colorbutton();
    Init_gtk_color_selection();
    Init_gtk_color_selection_dialog();
    Init_gtk_combo();
    Init_gtk_combobox();
    Init_gtk_comboboxentry();
    Init_gtk_const();
    Init_gtk_container();
    Init_gtk_curve();
    Init_gtk_dialog();
    Init_gtk_drag();
    Init_gtk_drawing_area();
    Init_gtk_editable();
    Init_gtk_entry();
    Init_gtk_entry_completion();
    Init_gtk_eventbox();
    Init_gtk_expander();
    Init_gtk_file_chooser();
    Init_gtk_filesystemerror();
    Init_gtk_filechooserbutton();
    Init_gtk_file_chooser_dialog();
    Init_gtk_file_chooser_widget();
    Init_gtk_file_filter();
    Init_gtk_file_selection();
    Init_gtk_fixed();
    Init_gtk_fontbutton();
    Init_gtk_font_selection();
    Init_gtk_font_selection_dialog();
    Init_gtk_frame();
    Init_gtk_gamma_curve();
    Init_gtk_gdk_draw();
    Init_gtk_gdk_gc();
    Init_gtk_handle_box();
    Init_gtk_hbutton_box();
    Init_gtk_hbox();
    Init_gtk_hpaned();
    Init_gtk_hruler();
    Init_gtk_hscale();
    Init_gtk_hscrollbar();
    Init_gtk_hseparator();
    Init_gtk_icon_factory();
    Init_gtk_iconinfo();
    Init_gtk_icon_set();
    Init_gtk_icon_size();
    Init_gtk_icon_source();
    Init_gtk_icon_theme();
    Init_gtk_iconview();
    Init_gtk_image();
    Init_gtk_image_menu_item();
    Init_gtk_imcontext();
    Init_gtk_imcontext_simple();
    Init_gtk_im_multicontext();
    Init_gtk_input_dialog();
    Init_gtk_invisible();
    Init_gtk_item();
    Init_gtk_itemfactory();
    Init_gtk_label();
    Init_gtk_layout();
    Init_gtk_link_button();
    Init_gtk_list_store();
    Init_gtk_main();
    Init_gtk_menu();
    Init_gtk_menu_bar();
    Init_gtk_menu_item();
    Init_gtk_menu_shell();
    Init_gtk_menutoolbutton();
    Init_gtk_message_dialog();
    Init_gtk_misc();
    Init_gtk_notebook();
    Init_gtk_object();
    Init_gtk_option_menu();
    Init_gtk_page_setup();
    Init_gtk_page_setup_unix_dialog();
    Init_gtk_paned();
    Init_gtk_paper_size();
    Init_gtk_plug();
    Init_gtk_print_context();
    Init_gtk_printer();
    Init_gtk_print_job();
    Init_gtk_print_operation();
    Init_gtk_print_operation_preview();
    Init_gtk_print_settings();
    Init_gtk_print_unix_dialog();
    Init_gtk_progress();
    Init_gtk_progress_bar();
    Init_gtk_radio_action();
    Init_gtk_radio_button();
    Init_gtk_radio_menu_item();
    Init_gtk_radiotoolbutton();
    Init_gtk_range();
    Init_gtk_rc();
    Init_gtk_rcstyle();
    Init_gtk_recentaction();
    Init_gtk_recent_chooser();
    Init_gtk_recent_chooser_dialog();
    Init_gtk_recent_chooser_menu();
    Init_gtk_recent_chooser_widget();
    Init_gtk_recent_data();
    Init_gtk_recent_filter();
    Init_gtk_recent_filter_info();
    Init_gtk_recent_info();
    Init_gtk_recent_manager();
    Init_gtk_ruler();
    Init_gtk_scale();
    Init_gtk_scalebutton();
    Init_gtk_scrollbar();
    Init_gtk_scrolled_window();
    Init_gtk_selection();
    Init_gtk_selectiondata();
    Init_gtk_separator();
    Init_gtk_separator_menu_item();
    Init_gtk_separatortoolitem();
    Init_gtk_settings();
    Init_gtk_size_group();
    Init_gtk_socket();
    Init_gtk_spin_button();
    Init_gtk_spinner();
    Init_gtk_statusbar();
    Init_gtk_status_icon();
    Init_gtk_stock();
    Init_gtk_style();
    Init_gtk_table();
    Init_gtk_target_list();
    Init_gtk_tearoff_menu_item();
    Init_gtk_text_appearance();
    Init_gtk_text_attributes();
    Init_gtk_textbuffer();
    Init_gtk_textchild();
    Init_gtk_textiter();
    Init_gtk_textmark();
    Init_gtk_texttag();
    Init_gtk_text_tag_table();
    Init_gtk_textview();
    Init_gtk_toggle_action();
    Init_gtk_toggle_button();
    Init_gtk_toggletoolbutton();
    Init_gtk_toolbar();
    Init_gtk_toolbutton();
    Init_gtk_toolitem();
    Init_gtk_tooltip();
    Init_gtk_tooltips();
    Init_gtk_treedragdest();
    Init_gtk_treedragsource();
    Init_gtk_treeiter();
    Init_gtk_treemodel();
    Init_gtk_treemodelfilter();
    Init_gtk_tmodelsort();
    Init_gtk_treepath();
    Init_gtk_treerowreference();
    Init_gtk_treeselection();
    Init_gtk_treesortable();
    Init_gtk_tree_store();
    Init_gtk_treeview();
    Init_gtk_treeviewcolumn();
    Init_gtk_uimanager();
    Init_gtk_vbutton_box();
    Init_gtk_vbox();
    Init_gtk_viewport();
    Init_gtk_volumebutton();
    Init_gtk_vpaned();
    Init_gtk_vruler();
    Init_gtk_vscale();
    Init_gtk_vscrollbar();
    Init_gtk_vseparator();
    Init_gtk_widget();
    Init_gtk_window();
    Init_gtk_windowgroup();
}
