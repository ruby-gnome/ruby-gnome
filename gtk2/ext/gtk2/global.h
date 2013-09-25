/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
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

#ifndef _RBGTK_GLOBAL_H
#define _RBGTK_GLOBAL_H

#include "rbgtk.h"
#ifdef HAVE_RUBY_ST_H
#  include <ruby/st.h>
#else
#  include <st.h>
#endif

#undef RG_DEF_METHOD
#define RG_DEF_METHOD(method, argc) \
        rb_define_method(RG_TARGET_NAMESPACE, #method, rg_ ## method, argc)
#undef RG_DEF_SMETHOD
#define RG_DEF_SMETHOD(method, argc) \
        rb_define_singleton_method(RG_TARGET_NAMESPACE, #method, rg_s_ ## method, argc)

#ifndef HAVE_RB_ERRINFO
#  define rb_errinfo() (ruby_errinfo)
#endif

#ifndef G_VALUE_INIT
#  define G_VALUE_INIT { 0, { { 0 } } }
#endif

#if GTK_CHECK_VERSION(2,8,0) && defined(HAVE_RB_CAIRO_H)
#  define CAIRO_AVAILABLE 1
#else
#  define CAIRO_AVAILABLE 0
#endif

extern ID id_relative_callbacks;
extern ID id_call;
extern VALUE treeiter_set_value_table;

G_GNUC_INTERNAL void Init_gtk_gdk(void);
G_GNUC_INTERNAL void Init_gtk_gtk(void);
G_GNUC_INTERNAL void Init_gtk_gdk_atom(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_cairo(void);
G_GNUC_INTERNAL void Init_gtk_gdk_color(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_colormap(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_const(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_cursor(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_device(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_display(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_display_manager(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_dragcontext(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_event(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_geometry(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_image(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_input(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_keymap(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_keyval(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_pango(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_pangorenderer(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_pixbuf(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_pixmap(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_property(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_rectangle(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_region(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_rgb(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_screen(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_selection(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_threads(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_timecoord(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_visual(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_window(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_windowattr(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_x11(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_aboutdialog(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_accelerator(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_accel_group(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_accel_group_entry(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_accel_key(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_accel_label(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_accel_map(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_accessible(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_action(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_actiongroup(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_adjustment(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_alignment(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_allocation(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_arrow(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_aspect_frame(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_assistant(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_button_box(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_bin(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_bindings(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_border(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_box(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_buildable(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_builder(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_button(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_calendar(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_celleditable(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_celllayout(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_cellrenderer(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_cellrendereraccel(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_cellrenderercombo(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_cellrendererpixbuf(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_cellrendererprogress(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_cellrendererspin(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_cellrendererspinner(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_cellrenderertext(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_cellrenderertoggle(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_cellview(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_check_button(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_check_menu_item(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_clipboard(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_colorbutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_color_selection(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_color_selection_dialog(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_combo(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_combobox(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_comboboxentry(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_const(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_container(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_curve(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_dialog(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_drag(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_drawing_area(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_editable(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_entry(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_entry_completion(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_eventbox(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_expander(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_file_chooser(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_filesystemerror(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_filechooserbutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_file_chooser_dialog(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_file_chooser_widget(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_file_filter(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_file_selection(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_fixed(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_fontbutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_font_selection(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_font_selection_dialog(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_frame(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_gamma_curve(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_gdk_draw(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_gc(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_handle_box(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_hbutton_box(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_hbox(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_hpaned(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_hruler(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_hscale(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_hscrollbar(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_hseparator(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_icon_factory(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_iconinfo(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_icon_set(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_icon_size(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_icon_source(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_icon_theme(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_iconview(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_image(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_image_menu_item(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_imcontext(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_imcontext_simple(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_im_multicontext(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_infobar(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_input_dialog(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_invisible(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_item(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_itemfactory(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_label(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_layout(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_link_button(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_list_store(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_menu(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_menu_bar(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_menu_item(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_menu_shell(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_menutoolbutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_message_dialog(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_misc(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_notebook(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_object(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_option_menu(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_page_setup(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_page_setup_unix_dialog(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_paned(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_paper_size(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_plug(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_print_context(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_printer(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_print_job(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_print_operation(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_print_operation_preview(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_print_settings(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_print_unix_dialog(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_progress(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_progress_bar(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_radio_action(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_radio_button(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_radio_menu_item(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_radiotoolbutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_range(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_rc(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_rcstyle(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_recentaction(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_recent_chooser(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_recent_chooser_dialog(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_recent_chooser_menu(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_recent_chooser_widget(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_recent_data(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_recent_filter(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_recent_filter_info(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_recent_info(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_recent_manager(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_ruler(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_scale(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_scalebutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_scrollbar(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_scrolled_window(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_selection(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_selectiondata(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_separator(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_separator_menu_item(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_separatortoolitem(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_settings(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_size_group(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_socket(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_spin_button(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_spinner(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_statusbar(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_status_icon(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_stock(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_style(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_table(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_target_list(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_tearoff_menu_item(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_text_appearance(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_text_attributes(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_textbuffer(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_textchild(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_textiter(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_textmark(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_texttag(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_text_tag_table(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_textview(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_toggle_action(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_toggle_button(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_toggletoolbutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_toolbar(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_toolbutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_toolitem(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_tooltip(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_tooltips(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_treedragdest(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_treedragsource(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_treeiter(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_treemodel(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_treemodelfilter(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_tmodelsort(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_treepath(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_treerowreference(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_treeselection(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_treesortable(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_tree_store(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_treeview(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_treeviewcolumn(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_uimanager(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_vbutton_box(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_vbox(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_viewport(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_volumebutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_vpaned(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_vruler(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_vscale(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_vscrollbar(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_vseparator(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_widget(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_window(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_windowgroup(VALUE mGtk);

#endif /* _RBGTK_GLOBAL_H */
