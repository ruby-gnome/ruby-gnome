/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-program.c,v 1.2 2002/10/14 13:56:24 tkubo Exp $ */
/* based on libgnome/gnome-program.h */

/* Gnome::Program module for Ruby/GNOME2
 *
 * Copyright (C) 2002 KUBO Takehiro <kubo@jiubao.org>
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
#include <gmodule.h> /* g_module_error() */

#define _SELF(self) GNOME_PROGRAM(RVAL2GOBJ(self))
#define _WRAP(gmi) BOXED2RVAL((gpointer)(gmi), GNOME_TYPE_MODULE_INFO)

static VALUE
make_poptoption(options)
    struct poptOption *options;
{
    VALUE obj;
    VALUE entry;
    VALUE arg;
    int i;

    obj = rb_ary_new();
    for (i = 0;;i++) {
        if (options[i].longName == NULL
            && options[i].shortName == '\0'
            && options[i].argInfo == 0
            && options[i].arg == NULL
            && options[i].val == 0)
            break;
        switch (options[i].argInfo & POPT_ARG_MASK) {
          case POPT_ARG_NONE:
            arg = options[i].arg ? (*((int *)options[i].arg) ? Qtrue : Qfalse) : Qnil;
            break;
          case POPT_ARG_STRING:
            arg = options[i].arg ? rb_str_new2(options[i].arg) : Qnil;
            break;
          case POPT_ARG_INT:
            arg = options[i].arg ? INT2NUM(*((int *)options[i].arg)) : Qnil;
            break;
          case POPT_ARG_LONG:
            arg = options[i].arg ? LONG2NUM(*((long *)options[i].arg)) : Qnil;
            break;
          case POPT_ARG_INCLUDE_TABLE:
            arg = make_poptoption(options[i].arg);
            break;
          case POPT_ARG_CALLBACK:
            continue;
            break;
          case POPT_ARG_INTL_DOMAIN:
            arg = rb_str_new2(options[i].arg);
            break;
          case POPT_ARG_VAL:
            arg = options[i].arg ? INT2NUM(*((int *)options[i].arg)) : Qnil;
            break;
          case POPT_ARG_FLOAT:
            arg = options[i].arg ? rb_float_new(*((float *)options[i].arg)) : Qnil;
            break;
          case POPT_ARG_DOUBLE:
            arg = options[i].arg ? rb_float_new(*((double *)options[i].arg)) : Qnil;
            break;
          default:
            rb_raise(rb_eRuntimeError, "Unknown arginfo %d", options[i].argInfo & POPT_ARG_MASK);
        }
        entry = rb_ary_new();
        rb_ary_push(entry, options[i].longName ? rb_str_new2(options[i].longName) : Qnil);
        rb_ary_push(entry, options[i].shortName ? rb_str_new(&(options[i].shortName), 1) : Qnil);
        rb_ary_push(entry, UINT2NUM(options[i].argInfo));
        rb_ary_push(entry, arg);
        rb_ary_push(entry, options[i].descrip ? rb_str_new2(options[i].descrip) : Qnil);
        rb_ary_push(entry, options[i].argDescrip ? rb_str_new2(options[i].argDescrip) : Qnil);
        rb_ary_push(obj, entry);
    }
    return obj;
}

static VALUE
popt_context_g2r_func(from)
    const GValue *from;
{
    poptContext ctx = g_value_get_pointer(from);
    const char **args = poptGetArgs(ctx);
    VALUE ary;

    if (args == NULL)
        return Qnil;
    ary = rb_ary_new();
    for (;*args != NULL;args++)
        rb_ary_push(ary, rb_str_new2(*args));
    return ary;
}

static VALUE
program_initialize(self, app_id, app_version, module_info)
    VALUE self, app_id, app_version, module_info;
{
    GnomeProgram *program;
    int i, cargc;
    char **cargv;

    cargc = RARRAY(rb_argv)->len + 1;
    cargv = ALLOCA_N(char*, cargc + 1);
    cargv[0] = RVAL2CSTR(app_id);
    for (i = 1; i < cargc; i++) {
        cargv[i] = RVAL2CSTR(rb_ary_entry(rb_argv, i - 1));
    }

    program = gnome_program_init(cargv[0], RVAL2CSTR(app_version),
                                 RVAL2BOXED(module_info, GNOME_TYPE_MODULE_INFO),
                                 cargc, cargv, NULL);
    G_INITIALIZE(self, program);
    return Qnil;
}

static VALUE
program_s_get(klass)
    VALUE klass;
{
    return GOBJ2RVAL(gnome_program_get());
}

static VALUE
program_human_readable_name(self)
    VALUE self;
{
    return rb_str_new2(gnome_program_get_human_readable_name(_SELF(self)));
}

static VALUE
program_app_id(self)
    VALUE self;
{
    return rb_str_new2(gnome_program_get_app_id(_SELF(self)));
}

static VALUE
program_app_version(self)
    VALUE self;
{
    return rb_str_new2(gnome_program_get_app_version(_SELF(self)));
}

static VALUE
program_locate_file(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE domain, file_name, only_if_exists, multi;
    char *result;
    GSList *ret_locations, *list;
    VALUE obj;

    rb_scan_args(argc, argv, "31", &domain, &file_name, &only_if_exists, &multi);

    ret_locations = NULL;
    result = gnome_program_locate_file(NIL_P(self) ? NULL : _SELF(self),
                                       NUM2INT(domain), RVAL2CSTR(file_name),
                                       RTEST(only_if_exists),
                                       RTEST(multi) ? &ret_locations : NULL);
    if (RTEST(multi)) {
        obj = rb_ary_new();
        for (list = ret_locations; list != NULL; list = list->next) {
            rb_ary_push(obj, rb_str_new2(list->data));
            g_free(list->data);
            list->data = NULL;
        }
        if (ret_locations)
            g_slist_free(ret_locations);
    } else {
        obj = result ? rb_str_new2(result) : Qnil;
        if (result)
            g_free(result);
    }
    return obj;
}

static VALUE
program_s_locate_file(argc, argv, klass)
    int argc;
    VALUE *argv, klass;
{
    return program_locate_file(argc, argv, Qnil);
}

static VALUE
module_get_name(self)
    VALUE self;
{
    GnomeModuleInfo *gmi = RVAL2BOXED(self, GNOME_TYPE_MODULE_INFO);
    return rb_str_new2(gmi->name);
}

static VALUE
module_get_version(self)
    VALUE self;
{
    GnomeModuleInfo *gmi = RVAL2BOXED(self, GNOME_TYPE_MODULE_INFO);
    return rb_str_new2(gmi->version);
}

static VALUE
module_get_description(self)
    VALUE self;
{
    GnomeModuleInfo *gmi = RVAL2BOXED(self, GNOME_TYPE_MODULE_INFO);
    return rb_str_new2(gmi->description);
}

static VALUE
module_get_requirements(self)
    VALUE self;
{
    GnomeModuleInfo *gmi = RVAL2BOXED(self, GNOME_TYPE_MODULE_INFO);
    GnomeModuleRequirement *req;
    VALUE obj = rb_ary_new();

    for (req = gmi->requirements; req->module_info != NULL; req++) {
        rb_ary_push(obj, rb_ary_new3(2, _WRAP(req->module_info),
                                     rb_str_new2(req->required_version)));
    }
    return obj;
}

static VALUE
module_get_options(self)
    VALUE self;
{
    GnomeModuleInfo *gmi = RVAL2BOXED(self, GNOME_TYPE_MODULE_INFO);
    return gmi->options ? make_poptoption(gmi->options) : Qnil;
}

static VALUE
module_get_opt_prefix(self)
    VALUE self;
{
    GnomeModuleInfo *gmi = RVAL2BOXED(self, GNOME_TYPE_MODULE_INFO);
    return gmi->opt_prefix ? rb_str_new2(gmi->opt_prefix) : Qnil;
}

static VALUE
module_s_load(klass, mod_name)
    VALUE klass, mod_name;
{
    const GnomeModuleInfo *gmi = gnome_program_module_load(RVAL2CSTR(mod_name));
    const char *error;
    if (gmi == NULL) {
        error = g_module_error();
        if (error)
            rb_raise(rb_eRuntimeError, "%s", error);
        else
            rb_raise(rb_eRuntimeError, "Cannot load module %s", RVAL2CSTR(mod_name));
    }
    return _WRAP(gmi);
}

static VALUE
module_get_to_s(self)
    VALUE self;
{
    GnomeModuleInfo *gmi = RVAL2BOXED(self, GNOME_TYPE_MODULE_INFO);
    char buf[256];
    g_snprintf(buf, sizeof(buf), "#<Gnome::Module:%s %s (%s)>", gmi->name, gmi->version, gmi->description);
    return rb_str_new2(buf);
}

void
Init_gnome_program(mGnome)
    VALUE mGnome;
{
    VALUE gnoProgram = G_DEF_CLASS(GNOME_TYPE_PROGRAM, "Program", mGnome);
    VALUE gnoModuleInfo = G_DEF_CLASS(GNOME_TYPE_MODULE_INFO, "ModuleInfo", mGnome);
    rbgobj_register_property_getter(GNOME_TYPE_PROGRAM, GNOME_PARAM_POPT_CONTEXT, popt_context_g2r_func);

    rb_define_method(gnoProgram, "initialize", program_initialize, 3);
    rb_define_singleton_method(gnoProgram, "get", program_s_get, 0);
    rb_define_method(gnoProgram, "human_readable_name", program_human_readable_name, 0);
    rb_define_method(gnoProgram, "app_id", program_app_id, 0);
    rb_define_method(gnoProgram, "app_version", program_app_version, 0);
    rb_define_method(gnoProgram, "locate_file", program_locate_file, -1);
    rb_define_singleton_method(gnoProgram, "locate_file", program_s_locate_file, -1);

    /* Gnome installed files */
    rb_define_const(mGnome, "FILE_DOMAIN_LIBDIR", INT2FIX(GNOME_FILE_DOMAIN_LIBDIR));
    rb_define_const(mGnome, "FILE_DOMAIN_DATADIR", INT2FIX(GNOME_FILE_DOMAIN_DATADIR));
    rb_define_const(mGnome, "FILE_DOMAIN_SOUND", INT2FIX(GNOME_FILE_DOMAIN_SOUND));
    rb_define_const(mGnome, "FILE_DOMAIN_PIXMAP", INT2FIX(GNOME_FILE_DOMAIN_PIXMAP));
    rb_define_const(mGnome, "FILE_DOMAIN_CONFIG", INT2FIX(GNOME_FILE_DOMAIN_CONFIG));
    rb_define_const(mGnome, "FILE_DOMAIN_HELP", INT2FIX(GNOME_FILE_DOMAIN_HELP));

    /* Application files */
    rb_define_const(mGnome, "FILE_DOMAIN_APP_LIBDIR", INT2FIX(GNOME_FILE_DOMAIN_APP_LIBDIR));
    rb_define_const(mGnome, "FILE_DOMAIN_APP_DATADIR", INT2FIX(GNOME_FILE_DOMAIN_APP_DATADIR));
    rb_define_const(mGnome, "FILE_DOMAIN_APP_SOUND", INT2FIX(GNOME_FILE_DOMAIN_APP_SOUND));
    rb_define_const(mGnome, "FILE_DOMAIN_APP_PIXMAP", INT2FIX(GNOME_FILE_DOMAIN_APP_PIXMAP));
    rb_define_const(mGnome, "FILE_DOMAIN_APP_CONFIG", INT2FIX(GNOME_FILE_DOMAIN_APP_CONFIG));
    rb_define_const(mGnome, "FILE_DOMAIN_APP_HEL", INT2FIX(GNOME_FILE_DOMAIN_APP_HELP));

    rb_define_method(gnoModuleInfo, "name", module_get_name, 0);
    rb_define_method(gnoModuleInfo, "version", module_get_version, 0);
    rb_define_method(gnoModuleInfo, "description", module_get_description, 0);
    rb_define_method(gnoModuleInfo, "requirements", module_get_requirements, 0);
    rb_define_method(gnoModuleInfo, "options", module_get_options, 0);
    rb_define_method(gnoModuleInfo, "opt_prefix", module_get_opt_prefix, 0);
    rb_define_singleton_method(gnoModuleInfo, "load", module_s_load, 1);
    rb_define_method(gnoModuleInfo, "to_s", module_get_to_s, 0);

    rb_define_const(gnoModuleInfo, "LIBGNOME", _WRAP(LIBGNOME_MODULE));
    rb_define_const(gnoModuleInfo, "LIBGNOMEUI", _WRAP(LIBGNOMEUI_MODULE));
    rb_define_const(gnoModuleInfo, "GNOME_BONOBO", _WRAP(GNOME_BONOBO_MODULE));
    rb_define_const(gnoModuleInfo, "GNOME_GTK", _WRAP(gnome_gtk_module_info_get()));
    rb_define_const(gnoModuleInfo, "GNOME_CLIENT", _WRAP(GNOME_CLIENT_MODULE));
}
