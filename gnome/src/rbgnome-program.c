/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-program.c,v 1.6 2002/11/16 01:24:05 tkubo Exp $ */
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

static ID id_to_a;
static ID id_module_info;
static ID id_args;
static ID id_popt_table;
static VALUE default_module_info;

#define _SELF(self) GNOME_PROGRAM(RVAL2GOBJ(self))
#define _WRAP(gmi) BOXED2RVAL((gpointer)(gmi), GNOME_TYPE_MODULE_INFO)

enum prop_type {
    PROP_INT,
    PROP_BOOLEAN,
    PROP_STRING,
    PROP_TABLE
};

struct {
    const char *name;
    enum prop_type type;
} program_prop_table[] = {
    {GNOME_PARAM_POPT_TABLE,          PROP_TABLE},   /* "popt-table" */
    {GNOME_PARAM_POPT_FLAGS,          PROP_INT},     /* "popt-flags" */
    {GNOME_PARAM_CREATE_DIRECTORIES,  PROP_BOOLEAN}, /* "create-directories" */
    {GNOME_PARAM_ENABLE_SOUND,        PROP_BOOLEAN}, /* "enable-sound" */
    {GNOME_PARAM_ESPEAKER,            PROP_STRING},  /* "espeaker" */
    {GNOME_PARAM_GNOME_PREFIX,        PROP_STRING},  /* "gnome-prefix" */
    {GNOME_PARAM_GNOME_SYSCONFDIR,    PROP_STRING},  /* "gnome-sysconfdir" */
    {GNOME_PARAM_GNOME_DATADIR,       PROP_STRING},  /* "gnome-datadir" */
    {GNOME_PARAM_GNOME_LIBDIR,        PROP_STRING},  /* "gnome-libdir" */
    {GNOME_PARAM_APP_PREFIX,          PROP_STRING},  /* "app-prefix" */
    {GNOME_PARAM_APP_SYSCONFDIR,      PROP_STRING},  /* "app-sysconfdir" */
    {GNOME_PARAM_APP_DATADIR,         PROP_STRING},  /* "app-datadir" */
    {GNOME_PARAM_APP_LIBDIR,          PROP_STRING},  /* "app-libdir" */
    {GNOME_PARAM_HUMAN_READABLE_NAME, PROP_STRING},  /* "human-readable-name" */
    {GNOME_PARAM_GNOME_PATH,          PROP_STRING},  /* "gnome-path" */
};
#define SIZE_OF_PROP_TABLE (sizeof(program_prop_table) / sizeof(program_prop_table[0]))

enum prop_data_type {
    PROP_DATA_INT,
    PROP_DATA_PTR,
};

struct prop_data {
    const char *name;
    enum prop_data_type type;
    union {
        gint i; /* sizeof(gint) == sizeof(gboolean) */
        gpointer p;
    } value;
};

/* copy from gobject/gparam.c of glib-2.0.4. */
static void
canonalize_key(key)
    gchar *key;
{
  gchar *p;
  
  for (p = key; *p != 0; p++)
    {
      gchar c = *p;
      
      if (c != '-' &&
	  (c < '0' || c > '9') &&
	  (c < 'A' || c > 'Z') &&
	  (c < 'a' || c > 'z'))
	*p = '-';
    }
}

static int
prepare_pdata(hash, data, popt_tablep)
    VALUE hash;
    struct prop_data *data;
    VALUE *popt_tablep;
{
    VALUE ary, name, value;
    char cname[128];
    int i, j;

    if (NIL_P(hash))
        return 0;
    Check_Type(hash, T_HASH);
    ary = rb_funcall(hash, id_to_a, 0);
    if (RARRAY(ary)->len >= SIZE_OF_PROP_TABLE)
        rb_raise(rb_eArgError, "too many properties");
    *popt_tablep = Qnil;
    for (i = 0; i < RARRAY(ary)->len; i++) {
        name = RARRAY(RARRAY(ary)->ptr[i])->ptr[0];
        value = RARRAY(RARRAY(ary)->ptr[i])->ptr[1];
        if (SYMBOL_P(name)) {
            g_strlcpy(cname, rb_id2name(SYM2ID(name)), sizeof(cname));
        } else {
            g_strlcpy(cname, RVAL2CSTR(name), sizeof(cname));
        }
        canonalize_key(cname);
        for (j = 0; j < SIZE_OF_PROP_TABLE; j++) {
            if (strcmp(cname, program_prop_table[j].name) == 0) {
                data[i].name = program_prop_table[j].name;
                switch (program_prop_table[j].type) {
                  case PROP_INT:
                    data[i].type = PROP_DATA_INT;
                    data[i].value.i = NUM2INT(value);
                    break;
                  case PROP_BOOLEAN:
                    data[i].type = PROP_DATA_INT;
                    data[i].value.i = RTEST(value) ? 1 : 0;
                    break;
                  case PROP_STRING:
                    data[i].type = PROP_DATA_PTR;
                    data[i].value.p = RVAL2CSTR(value);
                    break;
                  case PROP_TABLE:
                    if (!NIL_P(*popt_tablep)) {
                        rb_raise(rb_eArgError, "popt-table is set more than once.");
                    }
                    *popt_tablep = rbgno_poptoption_array_to_obj(value);
                    data[i].type = PROP_DATA_PTR;
                    data[i].value.p = DATA_PTR(*popt_tablep);
                    break;
                }
                break;
            }
        }
        if (j == SIZE_OF_PROP_TABLE)
            rb_raise(rb_eArgError, "No such property %s", cname);
    }
    return i;
}

static GnomeProgram *
call_program_init(app_id, app_version, module_info, argc, argv, pdata_len, pdata)
    const char *app_id;
    const char *app_version;
    const GnomeModuleInfo *module_info;
    int argc;
    char **argv;
    int pdata_len;
    struct prop_data *pdata;
{
    GnomeProgram *pgm = NULL;
    struct prop_data id[SIZE_OF_PROP_TABLE]; /* int data */
    struct prop_data pd[SIZE_OF_PROP_TABLE]; /* pointer data */
    int i, ilen, plen;

    for (i = ilen = plen = 0; ilen + plen < pdata_len; i++) {
        switch (pdata[i].type) {
          case PROP_DATA_INT:
            id[ilen++] = pdata[i];
            break;
          case PROP_DATA_PTR:
            pd[plen++] = pdata[i];
            break;
        }
    }
    pd[plen].name = NULL;

    if (ilen == 0) {
        pgm = gnome_program_init(app_id, app_version, module_info, argc, argv,
                                 /* PROP_PTR */
                                 pd[0].name, pd[0].value.p,
                                 pd[1].name, pd[1].value.p,
                                 pd[2].name, pd[2].value.p,
                                 pd[3].name, pd[3].value.p,
                                 pd[4].name, pd[4].value.p,
                                 pd[5].name, pd[5].value.p,
                                 pd[6].name, pd[6].value.p,
                                 pd[7].name, pd[7].value.p,
                                 pd[8].name, pd[8].value.p,
                                 pd[9].name, pd[9].value.p,
                                 pd[10].name, pd[10].value.p,
                                 pd[11].name, pd[11].value.p,
                                 pd[12].name, pd[12].value.p,
                                 pd[13].name, pd[13].value.p,
                                 pd[14].name, pd[14].value.p,
                                 NULL);
    } else if (ilen == 1) {
        pgm = gnome_program_init(app_id, app_version, module_info, argc, argv,
                                 /* PROP_DATA_INT */
                                 id[0].name, id[0].value.i,
                                 /* PROP_PTR */
                                 pd[0].name, pd[0].value.p,
                                 pd[1].name, pd[1].value.p,
                                 pd[2].name, pd[2].value.p,
                                 pd[3].name, pd[3].value.p,
                                 pd[4].name, pd[4].value.p,
                                 pd[5].name, pd[5].value.p,
                                 pd[6].name, pd[6].value.p,
                                 pd[7].name, pd[7].value.p,
                                 pd[8].name, pd[8].value.p,
                                 pd[9].name, pd[9].value.p,
                                 pd[10].name, pd[10].value.p,
                                 pd[11].name, pd[11].value.p,
                                 pd[12].name, pd[12].value.p,
                                 pd[13].name, pd[13].value.p,
                                 pd[14].name, pd[14].value.p,
                                 NULL);
    } else if (ilen == 2) {
        pgm = gnome_program_init(app_id, app_version, module_info, argc, argv,
                                 /* PROP_DATA_INT */
                                 id[0].name, id[0].value.i,
                                 id[1].name, id[1].value.i,
                                 /* PROP_PTR */
                                 pd[0].name, pd[0].value.p,
                                 pd[1].name, pd[1].value.p,
                                 pd[2].name, pd[2].value.p,
                                 pd[3].name, pd[3].value.p,
                                 pd[4].name, pd[4].value.p,
                                 pd[5].name, pd[5].value.p,
                                 pd[6].name, pd[6].value.p,
                                 pd[7].name, pd[7].value.p,
                                 pd[8].name, pd[8].value.p,
                                 pd[9].name, pd[9].value.p,
                                 pd[10].name, pd[10].value.p,
                                 pd[11].name, pd[11].value.p,
                                 pd[12].name, pd[12].value.p,
                                 pd[13].name, pd[13].value.p,
                                 pd[14].name, pd[14].value.p,
                                 NULL);
    } else if (ilen == 3) { /* up to MAX_PROP_INT. */
        pgm = gnome_program_init(app_id, app_version, module_info, argc, argv,
                                 /* PROP_DATA_INT */
                                 id[0].name, id[0].value.i,
                                 id[1].name, id[1].value.i,
                                 id[2].name, id[2].value.i,
                                 /* PROP_DATA_PTR */
                                 pd[0].name, pd[0].value.p,
                                 pd[1].name, pd[1].value.p,
                                 pd[2].name, pd[2].value.p,
                                 pd[3].name, pd[3].value.p,
                                 pd[4].name, pd[4].value.p,
                                 pd[5].name, pd[5].value.p,
                                 pd[6].name, pd[6].value.p,
                                 pd[7].name, pd[7].value.p,
                                 pd[8].name, pd[8].value.p,
                                 pd[9].name, pd[9].value.p,
                                 pd[10].name, pd[10].value.p,
                                 pd[11].name, pd[11].value.p,
                                 pd[12].name, pd[12].value.p,
                                 pd[13].name, pd[13].value.p,
                                 pd[14].name, pd[14].value.p,
                                 NULL);
    } else {
        rb_bug("Umm. Please report to Ruby/GNOME2 maintainer (call_program_init:%d)", ilen);
    }
    return pgm;
}

static VALUE
program_initialize(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE app_id, app_version, module_info, arg0, args, prop;
    GnomeProgram *program;
    int cargc;
    char **cargv;
    struct prop_data pdata[SIZE_OF_PROP_TABLE];
    VALUE popt_table;
    EXTERN VALUE rb_progname;
    int i;

    rb_scan_args(argc, argv, "24", &app_id, &app_version, &module_info, &arg0, &args, &prop);

    /* prepare module_info */
    if (NIL_P(module_info))
        module_info = default_module_info;
    rb_ivar_set(self, id_module_info, module_info);

    /* prepare arguments */
    if (NIL_P(arg0))
        arg0 = rb_progname;
    if (NIL_P(args)) {
        args = rb_argv;
    } else {
        Check_Type(args, T_ARRAY);
    }
    args = rb_obj_dup(args); /* clone to attach to 'self'. */
    rb_ary_unshift(args, arg0);
    cargc = RARRAY(args)->len;
    cargv = ALLOCA_N(char*, cargc + 1);
    for (i = 0; i < cargc; i++) {
        cargv[i] = RVAL2CSTR(RARRAY(args)->ptr[i]);
    }
    cargv[i] = NULL;
    rb_ivar_set(self, id_args, args);

    /* prepare properties */
    i = prepare_pdata(prop, pdata, &popt_table);
    rb_ivar_set(self, id_popt_table, popt_table);

    program = call_program_init(RVAL2CSTR(app_id), RVAL2CSTR(app_version),
                                RVAL2BOXED(module_info, GNOME_TYPE_MODULE_INFO),
                                cargc, cargv, i, pdata);
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
program_get_args(self)
    VALUE self;
{
    GValue value = {0,};
    const char **args;
    VALUE ary = rb_ary_new();

    g_value_init(&value, G_TYPE_POINTER);
    g_object_get_property(RVAL2GOBJ(self), GNOME_PARAM_POPT_CONTEXT, &value);
    args = poptGetArgs(g_value_get_pointer(&value));
    if (args != NULL)
        for (;*args != NULL;args++)
            rb_ary_push(ary, rb_str_new2(*args));
	g_value_unset(&value);
    return ary;
}

static VALUE
program_get_options(self)
    VALUE self;
{
    VALUE popt_table = rb_ivar_get(self, id_popt_table);
    VALUE obj = rb_hash_new();

    if (NIL_P(popt_table))
        return obj;
    rbgno_poptoption_arg_to_hash(obj, DATA_PTR(popt_table));
    return obj;
}

static void
program_print_common(self, arg)
    VALUE self;
    const char *arg;
{
    GnomeProgram *pgm = _SELF(self);
    const char *app_id = gnome_program_get_app_id(pgm);
    const char *app_version = gnome_program_get_app_version(pgm);
    VALUE minfo = rb_ivar_get(self, id_module_info);
    VALUE args = rb_ivar_get(self, id_args);
    VALUE popt_table = rb_ivar_get(self, id_popt_table);
    char *argv[3];

    argv[0] = RVAL2CSTR(RARRAY(args)->ptr[0]);
    argv[1] = (char *)arg;
    argv[2] = NULL;
    /* ugly hack!
     * I cannot use poptPrintHelp() or poptPrintUsage(),
     * because the status of pgm->_priv->arg_context becomes invalid
     * after GnomeProgram has been initialized.
     */
    gnome_program_init(app_id, app_version,
                       RVAL2BOXED(minfo, GNOME_TYPE_MODULE_INFO), 2, argv,
                       "popt-table", NIL_P(popt_table) ? NULL : DATA_PTR(popt_table),
                       NULL);
}

static VALUE
program_print_help(self)
    VALUE self;
{
    program_print_common(self, "--help");
    rb_bug("program_print_help: the spec of gnome_program_init changed?");
}

static VALUE
program_print_usage(self)
    VALUE self;
{
    program_print_common(self, "--usage");
    rb_bug("program_print_usage: the spec of gnome_program_init changed?");
}

#if 0 /* TODO or not TODO... */
void
gnome_program_module_register (const GnomeModuleInfo *module_info);

gboolean
gnome_program_module_registered (const GnomeModuleInfo *module_info);
#endif

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

    if (gmi->requirements == NULL)
        return obj;
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
    return gmi->options ? rbgno_poptoption_raw_to_arary(gmi->options) : rb_ary_new();
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

static VALUE
module_equal(mod1, mod2)
    VALUE mod1, mod2;
{
    GnomeModuleInfo *gmi1 = RVAL2BOXED(mod1, GNOME_TYPE_MODULE_INFO);
    GnomeModuleInfo *gmi2 = RVAL2BOXED(mod2, GNOME_TYPE_MODULE_INFO);
    return memcmp(gmi1, gmi2, sizeof(GnomeModuleInfo)) ? Qfalse : Qtrue;
}

static VALUE
module_cmp(mod1, mod2)
    VALUE mod1, mod2;
{
    GnomeModuleInfo *gmi1 = RVAL2BOXED(mod1, GNOME_TYPE_MODULE_INFO);
    GnomeModuleInfo *gmi2 = RVAL2BOXED(mod2, GNOME_TYPE_MODULE_INFO);
    int result = strcmp(gmi1->name, gmi2->name);
    if (result == 0) /* nervous check? */
        result = memcmp(gmi1, gmi2, sizeof(GnomeModuleInfo));
    return INT2FIX(result);
}

static VALUE
module_hash(self)
    VALUE self;
{
    GnomeModuleInfo *gmi = RVAL2BOXED(self, GNOME_TYPE_MODULE_INFO);
    register int key = 0;
    register unsigned int g;
    const char *p = gmi->name;

    /* elf hash */
    while (*p) {
        key = (key << 4) + *p++;
        g = key & 0xF0000000;
        if (g)
            key ^= g >> 24;
        key &= ~g;
    }
    return INT2FIX(key);
}

void
Init_gnome_program(mGnome)
    VALUE mGnome;
{
    VALUE gnoProgram = G_DEF_CLASS(GNOME_TYPE_PROGRAM, "Program", mGnome);
    VALUE gnoModuleInfo = G_DEF_CLASS(GNOME_TYPE_MODULE_INFO, "ModuleInfo", mGnome);

    id_to_a = rb_intern("to_a");
    id_popt_table = rb_intern("___popt_table___");
    id_args = rb_intern("___args___");
    id_module_info = rb_intern("___module_info___");

    rb_define_method(gnoProgram, "initialize", program_initialize, -1);
    rb_define_singleton_method(gnoProgram, "get", program_s_get, 0);
    rb_define_method(gnoProgram, "human_readable_name", program_human_readable_name, 0);
    rb_define_method(gnoProgram, "app_id", program_app_id, 0);
    rb_define_method(gnoProgram, "app_version", program_app_version, 0);
    rb_define_method(gnoProgram, "locate_file", program_locate_file, -1);
    rb_define_singleton_method(gnoProgram, "locate_file", program_s_locate_file, -1);
    /* Ruby/GNOME2 original methods */
    rb_define_method(gnoProgram, "args", program_get_args, 0);
    rb_define_method(gnoProgram, "options", program_get_options, 0);
    rb_define_method(gnoProgram, "print_help", program_print_help, 0);
    rb_define_method(gnoProgram, "print_usage", program_print_usage, 0);

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
    rb_define_const(mGnome, "FILE_DOMAIN_APP_HELP", INT2FIX(GNOME_FILE_DOMAIN_APP_HELP));

    rb_define_method(gnoModuleInfo, "name", module_get_name, 0);
    rb_define_method(gnoModuleInfo, "version", module_get_version, 0);
    rb_define_method(gnoModuleInfo, "description", module_get_description, 0);
    rb_define_method(gnoModuleInfo, "requirements", module_get_requirements, 0);
    rb_define_method(gnoModuleInfo, "options", module_get_options, 0);
    rb_define_method(gnoModuleInfo, "opt_prefix", module_get_opt_prefix, 0);
    rb_define_singleton_method(gnoModuleInfo, "load", module_s_load, 1);
    rb_define_method(gnoModuleInfo, "to_s", module_get_to_s, 0);
    rb_define_method(gnoModuleInfo, "==", module_equal, 1);
    rb_define_method(gnoModuleInfo, "===", module_equal, 1);
    rb_define_method(gnoModuleInfo, "eql?", module_equal, 1);
    rb_define_method(gnoModuleInfo, "<=>", module_cmp, 1);
    rb_define_method(gnoModuleInfo, "hash", module_hash, 0);

    rb_define_const(gnoModuleInfo, "LIBGNOME", _WRAP(LIBGNOME_MODULE));
    rb_define_const(gnoModuleInfo, "LIBGNOMEUI", (default_module_info = _WRAP(LIBGNOMEUI_MODULE)));
    rb_define_const(gnoModuleInfo, "GNOME_BONOBO", _WRAP(GNOME_BONOBO_MODULE));
    rb_define_const(gnoModuleInfo, "GNOME_GTK", _WRAP(gnome_gtk_module_info_get()));
    rb_define_const(gnoModuleInfo, "GNOME_CLIENT", _WRAP(GNOME_CLIENT_MODULE));
}
