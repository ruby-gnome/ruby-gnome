/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome.c,v 1.15 2002/10/19 16:36:25 tkubo Exp $ */

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
gnome_s_is_program_in_path(self, program)
    VALUE self, program;
{
    VALUE res;
    char* path = gnome_is_program_in_path(RVAL2CSTR(program));
    SET_STR_AND_GFREE(res, path);
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
    argv[0] = RVAL2CSTR(rb_app_id);
    for (i = 1; i < argc; i++) {
        rb_str = rb_ary_entry(rb_argv, i - 1);
        Check_Type(rb_str, T_STRING);
        argv[i] = RVAL2CSTR(rb_str);
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
            options[i].longName = RVAL2CSTR(rb_lname);
      
            rb_sname = rb_ary_entry(rb_option, 1);
            Check_Type(rb_sname, T_STRING);
            if (NIL_P(rb_sname)){
                options[i].shortName = '\0';
            } else {
                sname = RVAL2CSTR(rb_sname);
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
                result_s[i] = RVAL2CSTR(rb_arg);
                break;
            }   
            options[i].arg = &result_s[i];

            options[i].val = 0;
      
            rb_desc = rb_ary_entry(rb_option, 4);
            if (NIL_P(rb_desc)){
                options[i].descrip = NULL;
            } else {
                Check_Type(rb_desc, T_STRING);
                options[i].descrip = NIL_P(rb_desc)?NULL:RVAL2CSTR(rb_desc);
            }
      
            rb_argDesc = rb_ary_entry(rb_option, 5);
            if (NIL_P(rb_argDesc)){
                options[i].argDescrip = NULL;
            } else {
                Check_Type(rb_argDesc, T_STRING);
                options[i].argDescrip =  NIL_P(rb_argDesc)?NULL:RVAL2CSTR(rb_argDesc);
            }
        }
    
        options[i].longName = NULL;
        options[i].shortName = '\0';
        options[i].argInfo = 0;
        options[i].arg = NULL;
        options[i].val = 0;
    }

    gnome_init_with_popt_table(RVAL2CSTR(rb_app_id), 
                               RVAL2CSTR(rb_app_version),
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

void
Init_gnome2()
{
    VALUE mGnome = rb_define_module("Gnome");
    VALUE mBonobo = rb_define_module("Bonobo");

    rb_define_module_function(mGnome, "init", gnome_s_init, -1);
    rb_define_module_function(mGnome, "is_program_in_path", gnome_s_is_program_in_path, 1);

    /* popt */
    rb_define_const(mGnome, "POPT_ARG_NONE", INT2FIX(POPT_ARG_NONE));
    rb_define_const(mGnome, "POPT_ARG_STRING", INT2FIX(POPT_ARG_STRING));
    rb_define_const(mGnome, "POPT_ARG_INT", INT2FIX(POPT_ARG_INT));
    /* POPT_ARGS is for Ruby/GNOME only */
    rb_define_const(mGnome, "POPT_ARGS", rb_str_new2(""));

    /* GnomePreferencesType */
    rb_define_const(mGnome, "PREFERENCES_NEVER", INT2FIX(GNOME_PREFERENCES_NEVER));
    rb_define_const(mGnome, "PREFERENCES_USER", INT2FIX(GNOME_PREFERENCES_USER));
    rb_define_const(mGnome, "PREFERENCES_ALWAYS", INT2FIX(GNOME_PREFERENCES_ALWAYS));

    Init_bonobo_dock(mBonobo);
    Init_bonobo_dock_layout(mBonobo);
    Init_bonobo_dock_item(mBonobo);
    Init_bonobo_dock_band(mBonobo);

    Init_gnome_about(mGnome);
    Init_gnome_app(mGnome);
    Init_gnome_appbar(mGnome);
    Init_gnome_app_helper(mGnome);
    Init_gnome_client(mGnome);
    Init_gnome_color_picker(mGnome);
    Init_gnome_config(mGnome);
    Init_gnome_dateedit(mGnome);
    Init_gnome_entry(mGnome);
    Init_gnome_file_entry(mGnome);
    Init_gnome_font_picker(mGnome);
    Init_gnome_href(mGnome);
    Init_gnome_i18n(mGnome);
    Init_gnome_icon_entry(mGnome);
    Init_gnome_icon_item(mGnome);
    Init_gnome_icon_list(mGnome);
    Init_gnome_icon_selection(mGnome);
    Init_gnome_pixmap_entry(mGnome);
    Init_gnome_program(mGnome);
    Init_gnome_scores(mGnome);
    Init_gnome_stock_icons(mGnome);
    Init_gnome_uidefs(mGnome);
    Init_gnome_url(mGnome);
    Init_gnome_util(mGnome);
}
