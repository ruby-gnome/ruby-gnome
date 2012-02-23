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

    Init_gtk_gdk_atom(mGdk);
    Init_gtk_gdk_cairo();
    Init_gtk_gdk_color(mGdk);
    Init_gtk_gdk_colormap(mGdk);
    Init_gtk_gdk_const(mGdk);
    Init_gtk_gdk_cursor(mGdk);
    Init_gtk_gdk_device(mGdk);
    Init_gtk_gdk_display(mGdk);
    Init_gtk_gdk_display_manager(mGdk);
    Init_gtk_gdk_dragcontext(mGdk);
    Init_gtk_gdk_event(mGdk);
    Init_gtk_gdk_geometry(mGdk);
    Init_gtk_gdk_image(mGdk);
    Init_gtk_gdk_input(mGdk);
    Init_gtk_gdk_keymap(mGdk);
    Init_gtk_gdk_keyval(mGdk);
    Init_gtk_gdk_pango(mGdk);
    Init_gtk_gdk_pangorenderer(mGdk);
    Init_gtk_gdk_pixbuf(mGdk);
    Init_gtk_gdk_pixmap(mGdk);
    Init_gtk_gdk_property(mGdk);
    Init_gtk_gdk_rectangle(mGdk);
    Init_gtk_gdk_region(mGdk);
    Init_gtk_gdk_rgb(mGdk);
    Init_gtk_gdk_screen(mGdk);
    Init_gtk_gdk_selection(mGdk);
    Init_gtk_gdk_threads(mGdk);
    Init_gtk_gdk_timecoord(mGdk);
    Init_gtk_gdk_visual(mGdk);
    Init_gtk_gdk_window(mGdk);
    Init_gtk_gdk_windowattr(mGdk);
    Init_gtk_gdk_x11(mGdk);
    Init_gtk_aboutdialog(mGtk);
    Init_gtk_accelerator(mGtk);
    Init_gtk_accel_group(mGtk);
    Init_gtk_accel_group_entry(mGtk);
    Init_gtk_accel_key(mGtk);
    Init_gtk_accel_label(mGtk);
    Init_gtk_accel_map(mGtk);
    Init_gtk_accessible(mGtk);
    Init_gtk_action(mGtk);
    Init_gtk_actiongroup(mGtk);
    Init_gtk_adjustment(mGtk);
    Init_gtk_alignment(mGtk);
    Init_gtk_allocation(mGtk);
    Init_gtk_arrow(mGtk);
    Init_gtk_aspect_frame(mGtk);
    Init_gtk_assistant(mGtk);
    Init_gtk_button_box(mGtk);
    Init_gtk_bin(mGtk);
    Init_gtk_bindings(mGtk);
    Init_gtk_border(mGtk);
    Init_gtk_box(mGtk);
    Init_gtk_buildable(mGtk);
    Init_gtk_builder(mGtk);
    Init_gtk_button(mGtk);
    Init_gtk_calendar(mGtk);
    Init_gtk_celleditable(mGtk);
    Init_gtk_celllayout(mGtk);
    Init_gtk_cellrenderer(mGtk);
    Init_gtk_cellrendereraccel(mGtk);
    Init_gtk_cellrenderercombo(mGtk);
    Init_gtk_cellrendererpixbuf(mGtk);
    Init_gtk_cellrendererprogress(mGtk);
    Init_gtk_cellrendererspin(mGtk);
    Init_gtk_cellrendererspinner(mGtk);
    Init_gtk_cellrenderertext(mGtk);
    Init_gtk_cellrenderertoggle(mGtk);
    Init_gtk_cellview(mGtk);
    Init_gtk_check_button(mGtk);
    Init_gtk_check_menu_item(mGtk);
    Init_gtk_clipboard(mGtk);
    Init_gtk_colorbutton(mGtk);
    Init_gtk_color_selection(mGtk);
    Init_gtk_color_selection_dialog(mGtk);
    Init_gtk_combo(mGtk);
    Init_gtk_combobox(mGtk);
    Init_gtk_comboboxentry(mGtk);
    Init_gtk_const(mGtk);
    Init_gtk_container(mGtk);
    Init_gtk_curve(mGtk);
    Init_gtk_dialog(mGtk);
    Init_gtk_drag(mGtk);
    Init_gtk_drawing_area(mGtk);
    Init_gtk_editable(mGtk);
    Init_gtk_entry(mGtk);
    Init_gtk_entry_completion(mGtk);
    Init_gtk_eventbox(mGtk);
    Init_gtk_expander(mGtk);
    Init_gtk_file_chooser(mGtk);
    Init_gtk_filesystemerror(mGtk);
    Init_gtk_filechooserbutton(mGtk);
    Init_gtk_file_chooser_dialog(mGtk);
    Init_gtk_file_chooser_widget(mGtk);
    Init_gtk_file_filter(mGtk);
    Init_gtk_file_selection(mGtk);
    Init_gtk_fixed(mGtk);
    Init_gtk_fontbutton(mGtk);
    Init_gtk_font_selection(mGtk);
    Init_gtk_font_selection_dialog(mGtk);
    Init_gtk_frame(mGtk);
    Init_gtk_gamma_curve(mGtk);
    Init_gtk_gdk_draw(mGdk);
    Init_gtk_gdk_gc(mGdk);
    Init_gtk_handle_box(mGtk);
    Init_gtk_hbutton_box(mGtk);
    Init_gtk_hbox(mGtk);
    Init_gtk_hpaned(mGtk);
    Init_gtk_hruler(mGtk);
    Init_gtk_hscale(mGtk);
    Init_gtk_hscrollbar(mGtk);
    Init_gtk_hseparator(mGtk);
    Init_gtk_icon_factory(mGtk);
    Init_gtk_iconinfo(mGtk);
    Init_gtk_icon_set(mGtk);
    Init_gtk_icon_size(mGtk);
    Init_gtk_icon_source(mGtk);
    Init_gtk_icon_theme(mGtk);
    Init_gtk_iconview(mGtk);
    Init_gtk_image(mGtk);
    Init_gtk_image_menu_item(mGtk);
    Init_gtk_imcontext(mGtk);
    Init_gtk_imcontext_simple(mGtk);
    Init_gtk_im_multicontext(mGtk);
    Init_gtk_infobar(mGtk);
    Init_gtk_input_dialog(mGtk);
    Init_gtk_invisible(mGtk);
    Init_gtk_item(mGtk);
    Init_gtk_itemfactory(mGtk);
    Init_gtk_label(mGtk);
    Init_gtk_layout(mGtk);
    Init_gtk_link_button(mGtk);
    Init_gtk_list_store(mGtk);
    Init_gtk_menu(mGtk);
    Init_gtk_menu_bar(mGtk);
    Init_gtk_menu_item(mGtk);
    Init_gtk_menu_shell(mGtk);
    Init_gtk_menutoolbutton(mGtk);
    Init_gtk_message_dialog(mGtk);
    Init_gtk_misc(mGtk);
    Init_gtk_notebook(mGtk);
    Init_gtk_object(mGtk);
    Init_gtk_option_menu(mGtk);
    Init_gtk_page_setup(mGtk);
    Init_gtk_page_setup_unix_dialog(mGtk);
    Init_gtk_paned(mGtk);
    Init_gtk_paper_size(mGtk);
    Init_gtk_plug(mGtk);
    Init_gtk_print_context(mGtk);
    Init_gtk_printer(mGtk);
    Init_gtk_print_job(mGtk);
    Init_gtk_print_operation(mGtk);
    Init_gtk_print_operation_preview(mGtk);
    Init_gtk_print_settings(mGtk);
    Init_gtk_print_unix_dialog(mGtk);
    Init_gtk_progress(mGtk);
    Init_gtk_progress_bar(mGtk);
    Init_gtk_radio_action(mGtk);
    Init_gtk_radio_button(mGtk);
    Init_gtk_radio_menu_item(mGtk);
    Init_gtk_radiotoolbutton(mGtk);
    Init_gtk_range(mGtk);
    Init_gtk_rc(mGtk);
    Init_gtk_rcstyle(mGtk);
    Init_gtk_recentaction(mGtk);
    Init_gtk_recent_chooser(mGtk);
    Init_gtk_recent_chooser_dialog(mGtk);
    Init_gtk_recent_chooser_menu(mGtk);
    Init_gtk_recent_chooser_widget(mGtk);
    Init_gtk_recent_data(mGtk);
    Init_gtk_recent_filter(mGtk);
    Init_gtk_recent_filter_info(mGtk);
    Init_gtk_recent_info(mGtk);
    Init_gtk_recent_manager(mGtk);
    Init_gtk_ruler(mGtk);
    Init_gtk_scale(mGtk);
    Init_gtk_scalebutton(mGtk);
    Init_gtk_scrollbar(mGtk);
    Init_gtk_scrolled_window(mGtk);
    Init_gtk_selection(mGtk);
    Init_gtk_selectiondata(mGtk);
    Init_gtk_separator(mGtk);
    Init_gtk_separator_menu_item(mGtk);
    Init_gtk_separatortoolitem(mGtk);
    Init_gtk_settings(mGtk);
    Init_gtk_size_group(mGtk);
    Init_gtk_socket(mGtk);
    Init_gtk_spin_button(mGtk);
    Init_gtk_spinner(mGtk);
    Init_gtk_statusbar(mGtk);
    Init_gtk_status_icon(mGtk);
    Init_gtk_stock(mGtk);
    Init_gtk_style(mGtk);
    Init_gtk_table(mGtk);
    Init_gtk_target_list(mGtk);
    Init_gtk_tearoff_menu_item(mGtk);
    Init_gtk_text_appearance(mGtk);
    Init_gtk_text_attributes(mGtk);
    Init_gtk_textbuffer(mGtk);
    Init_gtk_textchild(mGtk);
    Init_gtk_textiter(mGtk);
    Init_gtk_textmark(mGtk);
    Init_gtk_texttag(mGtk);
    Init_gtk_text_tag_table(mGtk);
    Init_gtk_textview(mGtk);
    Init_gtk_toggle_action(mGtk);
    Init_gtk_toggle_button(mGtk);
    Init_gtk_toggletoolbutton(mGtk);
    Init_gtk_toolbar(mGtk);
    Init_gtk_toolbutton(mGtk);
    Init_gtk_toolitem(mGtk);
    Init_gtk_tooltip(mGtk);
    Init_gtk_tooltips(mGtk);
    Init_gtk_treedragdest(mGtk);
    Init_gtk_treedragsource(mGtk);
    Init_gtk_treeiter(mGtk);
    Init_gtk_treemodel(mGtk);
    Init_gtk_treemodelfilter(mGtk);
    Init_gtk_tmodelsort(mGtk);
    Init_gtk_treepath(mGtk);
    Init_gtk_treerowreference(mGtk);
    Init_gtk_treeselection(mGtk);
    Init_gtk_treesortable(mGtk);
    Init_gtk_tree_store(mGtk);
    Init_gtk_treeview(mGtk);
    Init_gtk_treeviewcolumn(mGtk);
    Init_gtk_uimanager(mGtk);
    Init_gtk_vbutton_box(mGtk);
    Init_gtk_vbox(mGtk);
    Init_gtk_viewport(mGtk);
    Init_gtk_volumebutton(mGtk);
    Init_gtk_vpaned(mGtk);
    Init_gtk_vruler(mGtk);
    Init_gtk_vscale(mGtk);
    Init_gtk_vscrollbar(mGtk);
    Init_gtk_vseparator(mGtk);
    Init_gtk_widget(mGtk);
    Init_gtk_window(mGtk);
    Init_gtk_windowgroup(mGtk);
}
