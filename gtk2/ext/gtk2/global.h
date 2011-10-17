/* -*- c-file-style: "ruby" -*- */
/************************************************

  global.h -

  $Author: sakai $
  $Date: 2003/03/14 03:12:19 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#ifndef _RBGTK_GLOBAL_H
#define _RBGTK_GLOBAL_H

#include "rbgtk.h"
#ifdef HAVE_RUBY_ST_H
#  include <ruby/st.h>
#else
#  include <st.h>
#endif

#ifndef HAVE_RB_ERRINFO
#  define rb_errinfo() (ruby_errinfo)
#endif

extern ID id_relative_callbacks;
extern ID id_call;
extern VALUE treeiter_set_value_table;

G_GNUC_INTERNAL void Init_gtk_gdk(void);
G_GNUC_INTERNAL void Init_gtk_gtk(void);
G_GNUC_INTERNAL void Init_gtk_gdk_atom(void);
G_GNUC_INTERNAL void Init_gtk_gdk_cairo(void);
G_GNUC_INTERNAL void Init_gtk_gdk_color(void);
G_GNUC_INTERNAL void Init_gtk_gdk_colormap(void);
G_GNUC_INTERNAL void Init_gtk_gdk_const(void);
G_GNUC_INTERNAL void Init_gtk_gdk_cursor(void);
G_GNUC_INTERNAL void Init_gtk_gdk_device(void);
G_GNUC_INTERNAL void Init_gtk_gdk_display(void);
G_GNUC_INTERNAL void Init_gtk_gdk_display_manager(void);
G_GNUC_INTERNAL void Init_gtk_gdk_dragcontext(void);
G_GNUC_INTERNAL void Init_gtk_gdk_event(void);
G_GNUC_INTERNAL void Init_gtk_gdk_geometry(void);
G_GNUC_INTERNAL void Init_gtk_gdk_image(void);
G_GNUC_INTERNAL void Init_gtk_gdk_input(void);
G_GNUC_INTERNAL void Init_gtk_gdk_keymap(void);
G_GNUC_INTERNAL void Init_gtk_gdk_keyval(void);
G_GNUC_INTERNAL void Init_gtk_gdk_pango(void);
G_GNUC_INTERNAL void Init_gtk_gdk_pangorenderer(void);
G_GNUC_INTERNAL void Init_gtk_gdk_pixbuf(void);
G_GNUC_INTERNAL void Init_gtk_gdk_pixmap(void);
G_GNUC_INTERNAL void Init_gtk_gdk_property(void);
G_GNUC_INTERNAL void Init_gtk_gdk_rectangle(void);
G_GNUC_INTERNAL void Init_gtk_gdk_region(void);
G_GNUC_INTERNAL void Init_gtk_gdk_rgb(void);
G_GNUC_INTERNAL void Init_gtk_gdk_screen(void);
G_GNUC_INTERNAL void Init_gtk_gdk_selection(void);
G_GNUC_INTERNAL void Init_gtk_gdk_threads(void);
G_GNUC_INTERNAL void Init_gtk_gdk_timecoord(void);
G_GNUC_INTERNAL void Init_gtk_gdk_visual(void);
G_GNUC_INTERNAL void Init_gtk_gdk_window(void);
G_GNUC_INTERNAL void Init_gtk_gdk_windowattr(void);
G_GNUC_INTERNAL void Init_gtk_gdk_x11(void);
G_GNUC_INTERNAL void Init_gtk_aboutdialog(void);
G_GNUC_INTERNAL void Init_gtk_accelerator(void);
G_GNUC_INTERNAL void Init_gtk_accel_group(void);
G_GNUC_INTERNAL void Init_gtk_accel_group_entry(void);
G_GNUC_INTERNAL void Init_gtk_accel_key(void);
G_GNUC_INTERNAL void Init_gtk_accel_label(void);
G_GNUC_INTERNAL void Init_gtk_accel_map(void);
G_GNUC_INTERNAL void Init_gtk_accessible(void);
G_GNUC_INTERNAL void Init_gtk_action(void);
G_GNUC_INTERNAL void Init_gtk_actiongroup(void);
G_GNUC_INTERNAL void Init_gtk_adjustment(void);
G_GNUC_INTERNAL void Init_gtk_alignment(void);
G_GNUC_INTERNAL void Init_gtk_allocation(void);
G_GNUC_INTERNAL void Init_gtk_arrow(void);
G_GNUC_INTERNAL void Init_gtk_aspect_frame(void);
G_GNUC_INTERNAL void Init_gtk_assistant(void);
G_GNUC_INTERNAL void Init_gtk_button_box(void);
G_GNUC_INTERNAL void Init_gtk_bin(void);
G_GNUC_INTERNAL void Init_gtk_bindings(void);
G_GNUC_INTERNAL void Init_gtk_border(void);
G_GNUC_INTERNAL void Init_gtk_box(void);
G_GNUC_INTERNAL void Init_gtk_buildable(void);
G_GNUC_INTERNAL void Init_gtk_builder(void);
G_GNUC_INTERNAL void Init_gtk_button(void);
G_GNUC_INTERNAL void Init_gtk_calendar(void);
G_GNUC_INTERNAL void Init_gtk_celleditable(void);
G_GNUC_INTERNAL void Init_gtk_celllayout(void);
G_GNUC_INTERNAL void Init_gtk_cellrenderer(void);
G_GNUC_INTERNAL void Init_gtk_cellrendereraccel(void);
G_GNUC_INTERNAL void Init_gtk_cellrenderercombo(void);
G_GNUC_INTERNAL void Init_gtk_cellrendererpixbuf(void);
G_GNUC_INTERNAL void Init_gtk_cellrendererprogress(void);
G_GNUC_INTERNAL void Init_gtk_cellrendererspin(void);
G_GNUC_INTERNAL void Init_gtk_cellrendererspinner(void);
G_GNUC_INTERNAL void Init_gtk_cellrenderertext(void);
G_GNUC_INTERNAL void Init_gtk_cellrenderertoggle(void);
G_GNUC_INTERNAL void Init_gtk_cellview(void);
G_GNUC_INTERNAL void Init_gtk_check_button(void);
G_GNUC_INTERNAL void Init_gtk_check_menu_item(void);
G_GNUC_INTERNAL void Init_gtk_clipboard(void);
G_GNUC_INTERNAL void Init_gtk_colorbutton(void);
G_GNUC_INTERNAL void Init_gtk_color_selection(void);
G_GNUC_INTERNAL void Init_gtk_color_selection_dialog(void);
G_GNUC_INTERNAL void Init_gtk_combo(void);
G_GNUC_INTERNAL void Init_gtk_combobox(void);
G_GNUC_INTERNAL void Init_gtk_comboboxentry(void);
G_GNUC_INTERNAL void Init_gtk_const(void);
G_GNUC_INTERNAL void Init_gtk_container(void);
G_GNUC_INTERNAL void Init_gtk_curve(void);
G_GNUC_INTERNAL void Init_gtk_dialog(void);
G_GNUC_INTERNAL void Init_gtk_drag(void);
G_GNUC_INTERNAL void Init_gtk_drawing_area(void);
G_GNUC_INTERNAL void Init_gtk_editable(void);
G_GNUC_INTERNAL void Init_gtk_entry(void);
G_GNUC_INTERNAL void Init_gtk_entry_completion(void);
G_GNUC_INTERNAL void Init_gtk_eventbox(void);
G_GNUC_INTERNAL void Init_gtk_expander(void);
G_GNUC_INTERNAL void Init_gtk_file_chooser(void);
G_GNUC_INTERNAL void Init_gtk_filechooserbutton(void);
G_GNUC_INTERNAL void Init_gtk_file_chooser_dialog(void);
G_GNUC_INTERNAL void Init_gtk_file_chooser_widget(void);
G_GNUC_INTERNAL void Init_gtk_file_filter(void);
G_GNUC_INTERNAL void Init_gtk_file_selection(void);
G_GNUC_INTERNAL void Init_gtk_fixed(void);
G_GNUC_INTERNAL void Init_gtk_fontbutton(void);
G_GNUC_INTERNAL void Init_gtk_font_selection(void);
G_GNUC_INTERNAL void Init_gtk_font_selection_dialog(void);
G_GNUC_INTERNAL void Init_gtk_frame(void);
G_GNUC_INTERNAL void Init_gtk_gamma_curve(void);
G_GNUC_INTERNAL void Init_gtk_gdk_draw(void);
G_GNUC_INTERNAL void Init_gtk_gdk_gc(void);
G_GNUC_INTERNAL void Init_gtk_handle_box(void);
G_GNUC_INTERNAL void Init_gtk_hbutton_box(void);
G_GNUC_INTERNAL void Init_gtk_hbox(void);
G_GNUC_INTERNAL void Init_gtk_hpaned(void);
G_GNUC_INTERNAL void Init_gtk_hruler(void);
G_GNUC_INTERNAL void Init_gtk_hscale(void);
G_GNUC_INTERNAL void Init_gtk_hscrollbar(void);
G_GNUC_INTERNAL void Init_gtk_hseparator(void);
G_GNUC_INTERNAL void Init_gtk_icon_factory(void);
G_GNUC_INTERNAL void Init_gtk_iconinfo(void);
G_GNUC_INTERNAL void Init_gtk_icon_set(void);
G_GNUC_INTERNAL void Init_gtk_icon_size(void);
G_GNUC_INTERNAL void Init_gtk_icon_source(void);
G_GNUC_INTERNAL void Init_gtk_icon_theme(void);
G_GNUC_INTERNAL void Init_gtk_iconview(void);
G_GNUC_INTERNAL void Init_gtk_image(void);
G_GNUC_INTERNAL void Init_gtk_image_menu_item(void);
G_GNUC_INTERNAL void Init_gtk_imcontext(void);
G_GNUC_INTERNAL void Init_gtk_imcontext_simple(void);
G_GNUC_INTERNAL void Init_gtk_im_multicontext(void);
G_GNUC_INTERNAL void Init_gtk_input_dialog(void);
G_GNUC_INTERNAL void Init_gtk_invisible(void);
G_GNUC_INTERNAL void Init_gtk_item(void);
G_GNUC_INTERNAL void Init_gtk_itemfactory(void);
G_GNUC_INTERNAL void Init_gtk_label(void);
G_GNUC_INTERNAL void Init_gtk_layout(void);
G_GNUC_INTERNAL void Init_gtk_link_button(void);
G_GNUC_INTERNAL void Init_gtk_list_store(void);
G_GNUC_INTERNAL void Init_gtk_main(void);
G_GNUC_INTERNAL void Init_gtk_menu(void);
G_GNUC_INTERNAL void Init_gtk_menu_bar(void);
G_GNUC_INTERNAL void Init_gtk_menu_item(void);
G_GNUC_INTERNAL void Init_gtk_menu_shell(void);
G_GNUC_INTERNAL void Init_gtk_menutoolbutton(void);
G_GNUC_INTERNAL void Init_gtk_message_dialog(void);
G_GNUC_INTERNAL void Init_gtk_misc(void);
G_GNUC_INTERNAL void Init_gtk_notebook(void);
G_GNUC_INTERNAL void Init_gtk_object(void);
G_GNUC_INTERNAL void Init_gtk_option_menu(void);
G_GNUC_INTERNAL void Init_gtk_page_setup(void);
G_GNUC_INTERNAL void Init_gtk_page_setup_unix_dialog(void);
G_GNUC_INTERNAL void Init_gtk_paned(void);
G_GNUC_INTERNAL void Init_gtk_paper_size(void);
G_GNUC_INTERNAL void Init_gtk_plug(void);
G_GNUC_INTERNAL void Init_gtk_print_context(void);
G_GNUC_INTERNAL void Init_gtk_printer(void);
G_GNUC_INTERNAL void Init_gtk_print_job(void);
G_GNUC_INTERNAL void Init_gtk_print_operation(void);
G_GNUC_INTERNAL void Init_gtk_print_operation_preview(void);
G_GNUC_INTERNAL void Init_gtk_print_settings(void);
G_GNUC_INTERNAL void Init_gtk_print_unix_dialog(void);
G_GNUC_INTERNAL void Init_gtk_progress(void);
G_GNUC_INTERNAL void Init_gtk_progress_bar(void);
G_GNUC_INTERNAL void Init_gtk_radio_action(void);
G_GNUC_INTERNAL void Init_gtk_radio_button(void);
G_GNUC_INTERNAL void Init_gtk_radio_menu_item(void);
G_GNUC_INTERNAL void Init_gtk_radiotoolbutton(void);
G_GNUC_INTERNAL void Init_gtk_range(void);
G_GNUC_INTERNAL void Init_gtk_rc(void);
G_GNUC_INTERNAL void Init_gtk_rcstyle(void);
G_GNUC_INTERNAL void Init_gtk_recentaction(void);
G_GNUC_INTERNAL void Init_gtk_recent_chooser(void);
G_GNUC_INTERNAL void Init_gtk_recent_chooser_dialog(void);
G_GNUC_INTERNAL void Init_gtk_recent_chooser_menu(void);
G_GNUC_INTERNAL void Init_gtk_recent_chooser_widget(void);
G_GNUC_INTERNAL void Init_gtk_recent_data(void);
G_GNUC_INTERNAL void Init_gtk_recent_filter(void);
G_GNUC_INTERNAL void Init_gtk_recent_filter_info(void);
G_GNUC_INTERNAL void Init_gtk_recent_info(void);
G_GNUC_INTERNAL void Init_gtk_recent_manager(void);
G_GNUC_INTERNAL void Init_gtk_ruler(void);
G_GNUC_INTERNAL void Init_gtk_scale(void);
G_GNUC_INTERNAL void Init_gtk_scalebutton(void);
G_GNUC_INTERNAL void Init_gtk_scrollbar(void);
G_GNUC_INTERNAL void Init_gtk_scrolled_window(void);
G_GNUC_INTERNAL void Init_gtk_selection(void);
G_GNUC_INTERNAL void Init_gtk_selectiondata(void);
G_GNUC_INTERNAL void Init_gtk_separator(void);
G_GNUC_INTERNAL void Init_gtk_separator_menu_item(void);
G_GNUC_INTERNAL void Init_gtk_separatortoolitem(void);
G_GNUC_INTERNAL void Init_gtk_settings(void);
G_GNUC_INTERNAL void Init_gtk_size_group(void);
G_GNUC_INTERNAL void Init_gtk_socket(void);
G_GNUC_INTERNAL void Init_gtk_spin_button(void);
G_GNUC_INTERNAL void Init_gtk_spinner(void);
G_GNUC_INTERNAL void Init_gtk_statusbar(void);
G_GNUC_INTERNAL void Init_gtk_status_icon(void);
G_GNUC_INTERNAL void Init_gtk_stock(void);
G_GNUC_INTERNAL void Init_gtk_style(void);
G_GNUC_INTERNAL void Init_gtk_table(void);
G_GNUC_INTERNAL void Init_gtk_target_list(void);
G_GNUC_INTERNAL void Init_gtk_tearoff_menu_item(void);
G_GNUC_INTERNAL void Init_gtk_text_appearance(void);
G_GNUC_INTERNAL void Init_gtk_text_attributes(void);
G_GNUC_INTERNAL void Init_gtk_textbuffer(void);
G_GNUC_INTERNAL void Init_gtk_textchild(void);
G_GNUC_INTERNAL void Init_gtk_textiter(void);
G_GNUC_INTERNAL void Init_gtk_textmark(void);
G_GNUC_INTERNAL void Init_gtk_texttag(void);
G_GNUC_INTERNAL void Init_gtk_text_tag_table(void);
G_GNUC_INTERNAL void Init_gtk_textview(void);
G_GNUC_INTERNAL void Init_gtk_toggle_action(void);
G_GNUC_INTERNAL void Init_gtk_toggle_button(void);
G_GNUC_INTERNAL void Init_gtk_toggletoolbutton(void);
G_GNUC_INTERNAL void Init_gtk_toolbar(void);
G_GNUC_INTERNAL void Init_gtk_toolbutton(void);
G_GNUC_INTERNAL void Init_gtk_toolitem(void);
G_GNUC_INTERNAL void Init_gtk_tooltip(void);
G_GNUC_INTERNAL void Init_gtk_tooltips(void);
G_GNUC_INTERNAL void Init_gtk_treedragdest(void);
G_GNUC_INTERNAL void Init_gtk_treedragsource(void);
G_GNUC_INTERNAL void Init_gtk_treeiter(void);
G_GNUC_INTERNAL void Init_gtk_treemodel(void);
G_GNUC_INTERNAL void Init_gtk_treemodelfilter(void);
G_GNUC_INTERNAL void Init_gtk_tmodelsort(void);
G_GNUC_INTERNAL void Init_gtk_treepath(void);
G_GNUC_INTERNAL void Init_gtk_treerowreference(void);
G_GNUC_INTERNAL void Init_gtk_treeselection(void);
G_GNUC_INTERNAL void Init_gtk_treesortable(void);
G_GNUC_INTERNAL void Init_gtk_tree_store(void);
G_GNUC_INTERNAL void Init_gtk_treeview(void);
G_GNUC_INTERNAL void Init_gtk_treeviewcolumn(void);
G_GNUC_INTERNAL void Init_gtk_uimanager(void);
G_GNUC_INTERNAL void Init_gtk_vbutton_box(void);
G_GNUC_INTERNAL void Init_gtk_vbox(void);
G_GNUC_INTERNAL void Init_gtk_viewport(void);
G_GNUC_INTERNAL void Init_gtk_volumebutton(void);
G_GNUC_INTERNAL void Init_gtk_vpaned(void);
G_GNUC_INTERNAL void Init_gtk_vruler(void);
G_GNUC_INTERNAL void Init_gtk_vscale(void);
G_GNUC_INTERNAL void Init_gtk_vscrollbar(void);
G_GNUC_INTERNAL void Init_gtk_vseparator(void);
G_GNUC_INTERNAL void Init_gtk_widget(void);
G_GNUC_INTERNAL void Init_gtk_window(void);
G_GNUC_INTERNAL void Init_gtk_windowgroup(void);

#endif /* _RBGTK_GLOBAL_H */
