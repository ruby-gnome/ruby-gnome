/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2014 Ruby-GNOME2 Project Team
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

#ifndef __RBGTK3PRIVATE_H__
#define __RBGTK3PRIVATE_H__

#ifdef HAVE_RUBY_ST_H
#  include <ruby/st.h>
#else
#  include <st.h>
#endif

#include <rb_cairo.h>
#include <cairo-gobject.h>
#include <rbgio2.h>
#include <rbpango.h>
#include <rbgdk-pixbuf.h>
#include <rbgdk3.h>
#include "rbgtk3.h"
#include "rbgtk3util.h"

#ifndef HAVE_RB_ERRINFO
#  define rb_errinfo() (ruby_errinfo)
#endif

#ifndef G_VALUE_INIT
#  define G_VALUE_INIT { 0, { { 0 } } }
#endif

extern ID id_relative_callbacks;
extern ID id_call;

G_GNUC_INTERNAL void exec_callback(GtkWidget *widget, gpointer proc);

typedef void (*rbgtkiter_set_value_func)(void *model, GtkTreeIter *iter,
                                         gint column, GValue *value);
G_GNUC_INTERNAL void rbgtk_register_treeiter_set_value_func(GType, rbgtkiter_set_value_func);

G_GNUC_INTERNAL void rbgtk_atom2selectiondata(VALUE type, VALUE size, VALUE src, GdkAtom* gtype,
                                     void** data, gint* format, gint* length);
G_GNUC_INTERNAL void rbgtk_atom2selectiondata_free(GdkAtom gtype, void* data);

#define RVAL2GTKTARGETENTRIES(value, n) rbgtk_rval2gtktargetentries(value, n)
G_GNUC_INTERNAL GtkTargetEntry *rbgtk_rval2gtktargetentries(VALUE value, long *n);
#define RVAL2GTKTARGETENTRIES_ACCEPT_NIL(value, n) rbgtk_rval2gtktargetentries_accept_nil(value, n)
G_GNUC_INTERNAL GtkTargetEntry *rbgtk_rval2gtktargetentries_accept_nil(VALUE value, long *n);

G_GNUC_INTERNAL void Init_gtk_gdk_event(void);
G_GNUC_INTERNAL void Init_gtk_drag_gdk_drag_context(void);
G_GNUC_INTERNAL void Init_gtk_style_context_gdk_screen(void);
G_GNUC_INTERNAL void Init_gtk_tooltip_gdk_display(void);

G_GNUC_INTERNAL void Init_gtk(void);
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
G_GNUC_INTERNAL void Init_gtk_activatable(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_adjustment(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_alignment(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_allocation(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_appchooser(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_appchooserbutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_appchooserdialog(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_appchooserwidget(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_application(VALUE mGtk);
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
G_GNUC_INTERNAL void Init_gtk_combobox(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_comboboxtext(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_const(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_container(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_cssprovider(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_dialog(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_drag(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_drawing_area(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_editable(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_entry(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_entrybuffer(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_entry_completion(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_eventbox(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_expander(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_file_chooser(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_filesystemerror(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_filechooserbutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_file_chooser_dialog(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_file_chooser_widget(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_file_filter(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_fixed(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_fontbutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_fontchooser(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_fontchooserdialog(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_fontchooserwidget(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_frame(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_grid(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_handle_box(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_hsv(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_icon_factory(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_icon_info(VALUE mGtk);
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
G_GNUC_INTERNAL void Init_gtk_invisible(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_label(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_layout(VALUE mGtk);
#if GTK_CHECK_VERSION(3, 6, 0)
   G_GNUC_INTERNAL void Init_gtk_level_bar(VALUE mGtk);
#endif
G_GNUC_INTERNAL void Init_gtk_link_button(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_list_store(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_lockbutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_menu(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_menu_bar(VALUE mGtk);
#if GTK_CHECK_VERSION(3, 6, 0)
   G_GNUC_INTERNAL void Init_gtk_menu_button(VALUE mGtk);
#endif
G_GNUC_INTERNAL void Init_gtk_menu_item(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_menu_shell(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_menutoolbutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_message_dialog(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_misc(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_notebook(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_numerableicon(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_orientable(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_overlay(VALUE mGtk);
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
G_GNUC_INTERNAL void Init_gtk_progress_bar(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_radio_action(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_radio_button(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_radio_menu_item(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_radiotoolbutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_range(VALUE mGtk);
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
G_GNUC_INTERNAL void Init_gtk_scale(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_scalebutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_scrollable(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_scrollbar(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_scrolled_window(VALUE mGtk);
#if GTK_CHECK_VERSION(3, 6, 0)
   G_GNUC_INTERNAL void Init_gtk_search_entry(VALUE mGtk);
#endif
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
G_GNUC_INTERNAL void Init_gtk_stylecontext(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_styleproperties(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_styleprovider(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_switch(VALUE mGtk);
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
G_GNUC_INTERNAL void Init_gtk_themingengine(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_toggle_action(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_toggle_button(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_toggletoolbutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_toolbar(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_toolbutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_toolitem(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_toolitemgroup(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_toolshell(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_tooltip(VALUE mGtk);
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
G_GNUC_INTERNAL void Init_gtk_viewport(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_volumebutton(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_widget(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_widgetpath(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_window(VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_windowgroup(VALUE mGtk);
G_GNUC_INTERNAL void Init_conversions(void);

#endif /* __RBGTK3PRIVATE_H__ */
