/* $Id: rbgnome.c,v 1.5 2002/07/28 04:35:25 mutoh Exp $ */

/* GNOME module for Ruby/GNOME
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "rbgnome.h"

static VALUE
get_gnome_type(gnobj)
     GtkObject* gnobj;
{
    VALUE klass;

    if (0);
    else if GTK_IS_CLOCK(gnobj) klass = gClock;
    else if GTK_IS_DIAL(gnobj) klass = gDial;
    else if GTK_IS_PIXMAP_MENU_ITEM(gnobj) klass = gPixmapMenuItem;
    else if GNOME_IS_ABOUT(gnobj) klass = gnoAbout;
    else if GNOME_IS_ANIMATOR(gnobj) klass = gnoAnimator;
    else if GNOME_IS_APP(gnobj) klass = gnoApp;
    else if GNOME_IS_APPBAR(gnobj) klass = gnoAppBar;
    else if GNOME_IS_CALCULATOR(gnobj) klass = gnoCalculator;
    else if GNOME_IS_CANVAS(gnobj) klass = gnoCanvas;
    else if GNOME_IS_CANVAS_ITEM(gnobj) klass = gnoCanvasItem;
    else if GNOME_IS_CANVAS_GROUP(gnobj) klass = gnoCanvasGroup;
    else if GNOME_IS_CLIENT(gnobj) klass = gnoClient;
    else if GNOME_IS_COLOR_PICKER(gnobj) klass = gnoColorPicker;
    else if GNOME_IS_DATE_EDIT(gnobj) klass = gnoDateEdit;
    else if GNOME_IS_DENTRY_EDIT(gnobj) klass = gnoDEntryEdit;
    else if GNOME_IS_DIALOG(gnobj) klass = gnoDialog;
    else if GNOME_IS_DOCK(gnobj) klass = gnoDock;
    else if GNOME_IS_DOCK_BAND(gnobj) klass = gnoDockBand;
    else if GNOME_IS_DOCK_ITEM(gnobj) klass = gnoDockItem;
    else if GNOME_IS_DOCK_LAYOUT(gnobj) klass = gnoDockLayout;
    else if GNOME_IS_ENTRY(gnobj) klass = gnoEntry;
    else if GNOME_IS_FILE_ENTRY(gnobj) klass = gnoFileEntry;
    else if GNOME_IS_FONT_PICKER(gnobj) klass = gnoFontPicker;
    else if GNOME_IS_HREF(gnobj) klass = gnoHRef;
    else if GNOME_IS_ICON_ENTRY(gnobj) klass = gnoIconEntry;
    else if GNOME_IS_ICON_LIST(gnobj) klass = gnoIconList;
    else if GNOME_IS_ICON_SELECTION(gnobj) klass = gnoIconEntry;
    else if GNOME_IS_LESS(gnobj) klass = gnoLess;
    else if GNOME_IS_MESSAGE_BOX(gnobj) klass = gnoMessageBox;
    else if GNOME_IS_NUMBER_ENTRY(gnobj) klass = gnoNumberEntry;
    else if GNOME_IS_PAPER_SELECTOR(gnobj) klass = gnoPaperSelector;
    else if GNOME_IS_PIXMAP(gnobj) klass = gnoPixmap;
    else if GNOME_IS_PIXMAP_ENTRY(gnobj) klass = gnoPixmapEntry;
    else if GNOME_IS_PROPERTY_BOX(gnobj) klass = gnoPropertyBox;
    else if GNOME_IS_PROC_BAR(gnobj) klass = gnoProcBar;
    else if GNOME_IS_SCORES(gnobj) klass = gnoScores;
    else if GNOME_IS_SPELL(gnobj) klass = gnoSpell;
    else if GNOME_IS_STOCK(gnobj) klass = gnoStock;
    else klass = get_gtk_type(gnobj);

    return klass;
}

VALUE
make_gnobject_auto_type(gnobj)
    GtkObject* gnobj;
{
    return make_gobject(get_gnome_type(gnobj), gnobj);
}

VALUE
get_value_from_gno_obj(gnobj)
    GtkObject* gnobj;
{
    VALUE ret = (VALUE)gtk_object_get_data(gnobj, RUBY_GTK_OBJ_KEY);
    if (!ret)
        ret = make_gnobject_auto_type(gnobj);
    return ret;
}

static VALUE
gnome_s_is_program_in_path(self, program)
    VALUE self, program;
{
    VALUE res;
    char* path = gnome_is_program_in_path(STR2CSTR(program));
    if (path) {
        res = rb_str_new2(path);
        g_free(path);
    } else {
        res = Qnil;
    }
    return res;
}

static VALUE
gnome_s_init(in_argc, in_argv, self)
	 int in_argc;
	 VALUE *in_argv;
	 VALUE self;
{
  VALUE rb_app_id, rb_app_version, rb_options;
  VALUE rb_option, rb_lname, rb_sname, rb_argInfo;
  VALUE rb_arg, rb_str, rb_desc, rb_argDesc;
  VALUE ret;
  static VALUE ary, hash;
  int i, argc, len;
  poptContext ctx;
  struct poptOption* options;
  char** argv;
  const char** p_args;
  void** result_s;
  char* sname;

  rb_scan_args(in_argc, in_argv, "21", &rb_app_id, 
			   &rb_app_version, &rb_options);
  
  argc = RARRAY(rb_argv)->len + 1;
  argv = ALLOCA_N(char*, argc + 1);
  argv[0] = STR2CSTR(rb_app_id);
  for (i = 1; i < argc; i++) {
	rb_str = rb_ary_entry(rb_argv, i - 1);
	Check_Type(rb_str, T_STRING);
	argv[i] = STR2CSTR(rb_str);
  }

  len = 0;
  options = NULL;
  if (! NIL_P(rb_options)){
	len = RARRAY(rb_options)->len;
	options = ALLOCA_N(struct poptOption, len + 1);
	result_s = ALLOCA_N(void*, len);
	for (i = 0; i < len; i++) {
	  rb_option = rb_ary_entry(rb_options, i);
	  Check_Type(rb_option, T_ARRAY);
	  
	  rb_lname = rb_ary_entry(rb_option, 0);
	  Check_Type(rb_lname, T_STRING);
	  options[i].longName = STR2CSTR(rb_lname);
	  
	  rb_sname = rb_ary_entry(rb_option, 1);
	  Check_Type(rb_sname, T_STRING);
	  if (NIL_P(rb_sname)){
		options[i].shortName = '\0';
	  } else {
		sname = STR2CSTR(rb_sname);
		options[i].shortName = sname[0];
	  }
	  
	  rb_argInfo = rb_ary_entry(rb_option, 2);
	  rb_arg = rb_ary_entry(rb_option, 3);
	  options[i].argInfo = NUM2INT(rb_argInfo);
	  result_s[i] = NIL_P(rb_arg) ? (void*)NULL : (void*)rb_arg;
	  switch(options[i].argInfo){
	  case POPT_ARG_NONE:
		result_s[i] = (void*)NULL;
		break;
	  case POPT_ARG_INT:
		result_s[i] = (void*)NUM2INT(rb_arg);
		break;
	  case POPT_ARG_STRING:
		result_s[i] = STR2CSTR(rb_arg);
		break;
	  }	
	  options[i].arg = &result_s[i];

	  options[i].val = 0;
	  
	  rb_desc = rb_ary_entry(rb_option, 4);
	  if (NIL_P(rb_desc)){
		options[i].descrip = NULL;
	  } else {
		Check_Type(rb_desc, T_STRING);
		options[i].descrip = NIL_P(rb_desc)?NULL:STR2CSTR(rb_desc);
	  }
	  
	  rb_argDesc = rb_ary_entry(rb_option, 5);
	  if (NIL_P(rb_argDesc)){
		options[i].argDescrip = NULL;
	  } else {
		Check_Type(rb_argDesc, T_STRING);
		options[i].argDescrip =  NIL_P(rb_argDesc)?NULL:STR2CSTR(rb_argDesc);
	  }
	}
	
	options[i].longName = NULL;
	options[i].shortName = '\0';
	options[i].argInfo = 0;
	options[i].arg = NULL;
	options[i].val = 0;
  }

  gnome_init_with_popt_table(STR2CSTR(rb_app_id), 
							 STR2CSTR(rb_app_version),
							 argc, argv, options, 0, &ctx);
  hash = rb_hash_new();

  for (i = 0; i < len; i++){
	rb_option = rb_ary_entry(rb_options, i);
	switch(options[i].argInfo){
	case POPT_ARG_NONE:
	  ret = ((int)result_s[i] == 1) ? Qtrue : Qfalse;
	  break;
	case POPT_ARG_INT:
	  ret = INT2NUM((long)result_s[i]);
	  break;
	case POPT_ARG_STRING:
	  ret = rb_str_new2((char*)result_s[i]);
	  break;
	}	
	rb_hash_aset(hash, rb_str_new2(options[i].longName), ret);
  }

  p_args = poptGetArgs(ctx);
  len = 0;
  if (p_args != (const char**)NULL){
	while (p_args[len] != NULL){
	  len++;
	}
  }
  ary = rb_ary_new2(len);
  for (i = 0; i < len; i++){
	rb_ary_push(ary, rb_str_new2(p_args[i]));
  }
  rb_hash_aset(hash, rb_str_new2(""), ary);
  
  poptFreeContext(ctx);
  return hash;
}

VALUE mGnome;

void
Init_gnome()
{
    mGnome = rb_define_module("Gnome");

    rb_define_module_function(mGnome, "init", gnome_s_init, -1);

    rb_define_module_function(mGnome, "is_program_in_path",
                              gnome_s_is_program_in_path, 1);

	/* popt */
    rb_define_const(mGnome, "POPT_ARG_NONE", INT2FIX(POPT_ARG_NONE));
    rb_define_const(mGnome, "POPT_ARG_STRING", INT2FIX(POPT_ARG_STRING));
    rb_define_const(mGnome, "POPT_ARG_INT",	INT2FIX(POPT_ARG_INT));
	/* POPT_ARGS is for Ruby/GNOME only */
    rb_define_const(mGnome, "POPT_ARGS", rb_str_new2(""));

    /* GnomePreferencesType */
    rb_define_const(mGnome, "REFERENCES_NEVER",
					INT2FIX(GNOME_PREFERENCES_NEVER));
    rb_define_const(mGnome, "REFERENCES_USER",
					INT2FIX(GNOME_PREFERENCES_USER));
    rb_define_const(mGnome, "REFERENCES_ALWAYS",
					INT2FIX(GNOME_PREFERENCES_ALWAYS));

    Init_gtk_dial();
    Init_gtk_clock();
    Init_gtk_pixmap_menu_item();

    Init_gnome_animator();
    Init_gnome_app();
    Init_gnome_appbar();
    Init_gnome_app_helper();
    Init_gnome_calculator();
    Init_gnome_canvas();
    Init_gnome_canvas_item();
    Init_gnome_client();
    Init_gnome_color_picker();
    Init_gnome_dateedit();
    Init_gnome_dentry_edit();
    Init_gnome_desktop_entry();
    Init_gnome_dialog();
    Init_gnome_dock();
    Init_gnome_dock_band();
    Init_gnome_dock_item();
    Init_gnome_dock_layout();
    Init_gnome_entry();
    Init_gnome_file_entry();
    Init_gnome_font_picker();
    Init_gnome_href();
    Init_gnome_icon_entry();
    Init_gnome_icon_selection();
    Init_gnome_less();
    Init_gnome_messagebox();
    Init_gnome_number_entry();
    Init_gnome_paper_selector();
    Init_gnome_pixmap();
    Init_gnome_pixmap_entry();
    Init_gnome_proc_bar();
    Init_gnome_scores();
    Init_gnome_spell();
    Init_gnome_stock();

    Init_gnome_config();
    Init_gnome_i18n();
    Init_gnome_uidefs();
    Init_gnome_url();
    Init_gnome_util();
}
