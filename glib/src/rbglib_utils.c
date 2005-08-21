/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib_utils.c -

  $Author: pterjan $
  $Date: 2005/08/21 10:31:13 $

  Copyright (C) 2004 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Pascal Terjan

**********************************************************************/
#include "global.h"

#if GLIB_CHECK_VERSION(2,2,0)
static VALUE
rbglib_m_application_name(self)
    VALUE self;
{
    return CSTR2RVAL(g_get_application_name());
}

static VALUE
rbglib_m_set_application_name(self, application_name)
    VALUE self, application_name;
{
    g_set_prgname(NIL_P(application_name) ? NULL : RVAL2CSTR(application_name));
    return self;
}
#endif

static VALUE
rbglib_m_prgname(self)
    VALUE self;
{
    return CSTR2RVAL(g_get_prgname());
}

static VALUE
rbglib_m_set_prgname(self, prgname)
    VALUE self, prgname;
{
    g_set_prgname(NIL_P(prgname) ? NULL : RVAL2CSTR(prgname));
    return self;
}

static VALUE
rbglib_m_env(self, variable)
    VALUE self, variable;
{
    return CSTR2RVAL(g_getenv(RVAL2CSTR(variable)));
}

#if GLIB_CHECK_VERSION(2,4,0)
static VALUE
rbglib_m_setenv(self, variable, value, overwrite)
    VALUE self, variable, value, overwrite;
{
    return CBOOL2RVAL(g_setenv(RVAL2CSTR(variable), 
                               NIL_P(value) ? NULL : RVAL2CSTR(value),
                               RTEST(overwrite)));
}

static VALUE
rbglib_m_unsetenv(self, variable)
    VALUE self, variable;
{
    g_unsetenv(RVAL2CSTR(variable));
    return self;
}
#endif

#if GLIB_CHECK_VERSION(2,8,0)
static VALUE
rbglib_m_listenv(self)
    VALUE self;
{
    gchar** c_list;
    gchar** c_var;
    VALUE r_list = rb_ary_new();
    c_list = g_listenv();
    c_var = c_list;
    while(*c_var) {
        rb_ary_push(r_list, rb_str_new2(*(c_var++)));
    }
    g_strfreev(c_list);
    return r_list;
}

static VALUE
rbglib_m_host_name(self)
    VALUE self;
{
    return CSTR2RVAL(g_get_host_name());
}
#endif

static VALUE
rbglib_m_user_name(self)
    VALUE self;
{
    return CSTR2RVAL(g_get_user_name());
}

static VALUE
rbglib_m_real_name(self)
    VALUE self;
{
    return CSTR2RVAL(g_get_real_name());
}

static VALUE
rbglib_m_home_dir(self)
    VALUE self;
{
    return CSTR2RVAL(g_get_home_dir());
}

static VALUE
rbglib_m_tmp_dir(self)
    VALUE self;
{
    return CSTR2RVAL(g_get_tmp_dir());
}

static VALUE
rbglib_m_current_dir(self)
    VALUE self;
{
    gchar* dir = g_get_current_dir();
    VALUE ret = CSTR2RVAL(dir);
    g_free(dir);
    return ret;
}

static VALUE
rbglib_m_path_is_absolute(self, fname)
    VALUE self, fname;
{
    return CBOOL2RVAL(g_path_is_absolute(RVAL2CSTR(fname)));
}

static VALUE
rbglib_m_path_skip_root(self, fname)
    VALUE self, fname;
{
    return CSTR2RVAL(g_path_skip_root(RVAL2CSTR(fname)));
}

static VALUE
rbglib_m_path_get_basename(self, fname)
    VALUE self, fname;
{
    return CSTR2RVAL(g_path_get_basename(RVAL2CSTR(fname)));
}

static VALUE
rbglib_m_path_get_dirname(self, fname)
    VALUE self, fname;
{
    return CSTR2RVAL(g_path_get_dirname(RVAL2CSTR(fname)));
}

/*
Use File.join()
gchar*      g_build_filename                (const gchar *first_element,
                                             ...);
gchar*      g_build_filenamev               (gchar **args);
gchar*      g_build_path                    (const gchar *separator,
                                             const gchar *first_element,
                                             ...);
gchar*      g_build_pathv                   (const gchar *separator,
                                             gchar **args);
*/

static VALUE
rbglib_m_find_program_in_path(self, program)
    VALUE self, program;
{
    gchar* path = g_find_program_in_path(RVAL2CSTR(program));
    VALUE ret = CSTR2RVAL(path);
    g_free(path);
    return ret;
}

static VALUE
rbglib_m_bit_nth_lsf(self, mask, nth_bit)
    VALUE self, mask, nth_bit;
{
    return INT2NUM(g_bit_nth_lsf(NUM2ULONG(mask), NUM2INT(nth_bit)));
}

static VALUE
rbglib_m_bit_nth_msf(self, mask, nth_bit)
    VALUE self, mask, nth_bit;
{
    return INT2NUM(g_bit_nth_msf(NUM2ULONG(mask), NUM2INT(nth_bit)));
}

static VALUE
rbglib_m_bit_storage(self, number)
    VALUE self, number;
{
    return UINT2NUM(g_bit_storage(NUM2ULONG(number)));
}

static VALUE
rbglib_m_spaced_primes_closest(self, num)
    VALUE self, num;
{
    return UINT2NUM(g_spaced_primes_closest(NUM2UINT(num)));
}

/*
Use at_exit of ruby instead.
void        g_atexit                        (GVoidFunc func);
*/

static VALUE
rbglib_m_parse_debug_string(self, string, keys)
    VALUE self, string, keys;
{
    gint i, nkeys;
    VALUE ary, ret;
    GDebugKey* gkeys;

    Check_Type(keys, T_HASH);
    ary = rb_funcall(keys, rb_intern("to_a"), 0);
    nkeys = RARRAY(ary)->len;
    gkeys = g_new(GDebugKey, nkeys);
    for (i = 0; i < nkeys; i++) {
        gkeys[i].key = RVAL2CSTR(RARRAY(RARRAY(ary)->ptr[i])->ptr[0]);
        gkeys[i].value = NUM2UINT(RARRAY(RARRAY(ary)->ptr[i])->ptr[1]);
    }
   
    ret = UINT2NUM(g_parse_debug_string(RVAL2CSTR(string), gkeys, nkeys));
    g_free(gkeys);
    return ret;
}

/*
void        (*GVoidFunc)                    (void);
void        (*GFreeFunc)                    (gpointer data);

Don't need them.
void        g_qsort_with_data               (gconstpointer pbase,
                                             gint total_elems,
                                             gsize size,
                                             GCompareDataFunc compare_func,
                                             gpointer user_data);

void        g_nullify_pointer               (gpointer *nullify_location);
*/

static VALUE
rbglib_m_check_version(self, major, minor, micro)
    VALUE self, major, minor, micro;
{
    return CBOOL2RVAL(glib_major_version > NUM2UINT(major) ||
                      (glib_major_version == NUM2UINT(major) && 
                       glib_minor_version > NUM2UINT(minor)) ||
                      (glib_major_version == NUM2UINT(major) && 
                       glib_minor_version == NUM2UINT(minor) &&
                       glib_micro_version >= NUM2UINT(micro)));
}

void
Init_glib_utils()
{
    /* glib/gutils.h */
#if GLIB_CHECK_VERSION(2,2,0)
    rb_define_module_function(mGLib, "application_name", rbglib_m_application_name, 0);
    rb_define_module_function(mGLib, "set_application_name", rbglib_m_set_application_name, 1);
#endif
    rb_define_module_function(mGLib, "prgname", rbglib_m_prgname, 0);
    rb_define_module_function(mGLib, "set_prgname", rbglib_m_set_prgname, 1);
    rb_define_module_function(mGLib, "getenv", rbglib_m_env, 1);
#if GLIB_CHECK_VERSION(2,4,0)
    rb_define_module_function(mGLib, "setenv", rbglib_m_setenv, 2);
    rb_define_module_function(mGLib, "unsetenv", rbglib_m_unsetenv, 1);
#endif
#if GLIB_CHECK_VERSION(2,8,0)
    rb_define_module_function(mGLib, "listenv", rbglib_m_listenv, 0);
    rb_define_module_function(mGLib, "host_name", rbglib_m_host_name, 0);
#endif
    rb_define_module_function(mGLib, "user_name", rbglib_m_user_name, 0);
    rb_define_module_function(mGLib, "real_name", rbglib_m_real_name, 0);
    rb_define_module_function(mGLib, "home_dir", rbglib_m_home_dir, 0);
    rb_define_module_function(mGLib, "tmp_dir", rbglib_m_tmp_dir, 0);
    rb_define_module_function(mGLib, "current_dir", rbglib_m_current_dir, 0);
    rb_define_module_function(mGLib, "path_is_absolute?", rbglib_m_path_is_absolute, 1);
    rb_define_module_function(mGLib, "path_skip_root", rbglib_m_path_skip_root, 1);
    rb_define_module_function(mGLib, "path_get_basename", rbglib_m_path_get_basename, 1);
    rb_define_module_function(mGLib, "path_get_dirname", rbglib_m_path_get_dirname, 1);
    rb_define_module_function(mGLib, "find_program_in_path", rbglib_m_find_program_in_path, 1);
    rb_define_module_function(mGLib, "bit_nth_lsf", rbglib_m_bit_nth_lsf, 2);
    rb_define_module_function(mGLib, "bit_nth_msf", rbglib_m_bit_nth_msf, 2);
    rb_define_module_function(mGLib, "bit_storage", rbglib_m_bit_storage, 1);
    rb_define_module_function(mGLib, "spaced_primes_closest", rbglib_m_spaced_primes_closest, 1);
    rb_define_module_function(mGLib, "parse_debug_string", rbglib_m_parse_debug_string, 2);
    rb_define_module_function(mGLib, "check_version?", rbglib_m_check_version, 3);

}

