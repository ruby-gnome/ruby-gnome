/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Pascal Terjan
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

#include "rbgprivate.h"

#if GLIB_CHECK_VERSION(2,2,0)
static VALUE
rbglib_m_application_name(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_application_name());
}

static VALUE
rbglib_m_set_application_name(VALUE self, VALUE application_name)
{
    g_set_prgname(NIL_P(application_name) ? NULL : RVAL2CSTR(application_name));
    return self;
}
#endif

static VALUE
rbglib_m_prgname(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_prgname());
}

static VALUE
rbglib_m_set_prgname(VALUE self, VALUE prgname)
{
    g_set_prgname(NIL_P(prgname) ? NULL : RVAL2CSTR(prgname));
    return self;
}

static VALUE
rbglib_m_env(G_GNUC_UNUSED VALUE self, VALUE variable)
{
    return CSTR2RVAL(g_getenv(RVAL2CSTR(variable)));
}

#if GLIB_CHECK_VERSION(2,4,0)
static VALUE
rbglib_m_setenv(G_GNUC_UNUSED VALUE self, VALUE variable, VALUE value, VALUE overwrite)
{
    return CBOOL2RVAL(g_setenv(RVAL2CSTR(variable), 
                               NIL_P(value) ? NULL : RVAL2CSTR(value),
                               RVAL2CBOOL(overwrite)));
}

static VALUE
rbglib_m_unsetenv(VALUE self, VALUE variable)
{
    g_unsetenv(RVAL2CSTR(variable));
    return self;
}
#endif

#if GLIB_CHECK_VERSION(2,8,0)
#ifdef HAVE_G_LISTENV
static VALUE
rbglib_m_listenv(VALUE self)
{
    gchar** c_list;
    gchar** c_var;
    VALUE r_list = rb_ary_new();
    c_list = g_listenv();
    c_var = c_list;
    while(*c_var) {
        rb_ary_push(r_list, CSTR2RVAL(*(c_var++)));
    }
    g_strfreev(c_list);
    return r_list;
}
#endif

static VALUE
rbglib_m_host_name(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_host_name());
}
#endif

static VALUE
rbglib_m_user_name(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_user_name());
}

static VALUE
rbglib_m_real_name(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_real_name());
}

#if GLIB_CHECK_VERSION(2, 6, 0)
static VALUE
rbglib_m_user_cache_dir(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_user_cache_dir());
}

static VALUE
rbglib_m_user_data_dir(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_user_data_dir());
}

static VALUE
rbglib_m_user_config_dir(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_user_config_dir());
}

static VALUE
rbglib_m_system_data_dirs(G_GNUC_UNUSED VALUE self)
{
    return STRV2RVAL((const gchar **)g_get_system_data_dirs());
}

static VALUE
rbglib_m_system_config_dirs(G_GNUC_UNUSED VALUE self)
{
    return STRV2RVAL((const gchar **)g_get_system_config_dirs());
}
#endif

#if GLIB_CHECK_VERSION(2, 14, 0)
static VALUE
rbglib_m_get_user_special_dir(G_GNUC_UNUSED VALUE self, VALUE directory)
{
    return CSTR2RVAL(g_get_user_special_dir(RVAL2GENUM(directory,
						       G_TYPE_USER_DIRECTORY)));
}
#endif

static VALUE
rbglib_m_home_dir(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_home_dir());
}

static VALUE
rbglib_m_tmp_dir(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_tmp_dir());
}

static VALUE
rbglib_m_current_dir(G_GNUC_UNUSED VALUE self)
{
    gchar* dir = g_get_current_dir();
    VALUE ret = CSTR2RVAL(dir);
    g_free(dir);
    return ret;
}

static VALUE
rbglib_m_path_is_absolute(G_GNUC_UNUSED VALUE self, VALUE fname)
{
    return CBOOL2RVAL(g_path_is_absolute(RVAL2CSTR(fname)));
}

static VALUE
rbglib_m_path_skip_root(G_GNUC_UNUSED VALUE self, VALUE fname)
{
    return CSTR2RVAL(g_path_skip_root(RVAL2CSTR(fname)));
}

static VALUE
rbglib_m_path_get_basename(G_GNUC_UNUSED VALUE self, VALUE fname)
{
    return CSTR2RVAL(g_path_get_basename(RVAL2CSTR(fname)));
}

static VALUE
rbglib_m_path_get_dirname(G_GNUC_UNUSED VALUE self, VALUE fname)
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
rbglib_m_find_program_in_path(G_GNUC_UNUSED VALUE self, VALUE program)
{
    gchar* path = g_find_program_in_path(RVAL2CSTR(program));
    VALUE ret = CSTR2RVAL(path);
    g_free(path);
    return ret;
}

static VALUE
rbglib_m_bit_nth_lsf(G_GNUC_UNUSED VALUE self, VALUE mask, VALUE nth_bit)
{
    return INT2NUM(g_bit_nth_lsf(NUM2ULONG(mask), NUM2INT(nth_bit)));
}

static VALUE
rbglib_m_bit_nth_msf(G_GNUC_UNUSED VALUE self, VALUE mask, VALUE nth_bit)
{
    return INT2NUM(g_bit_nth_msf(NUM2ULONG(mask), NUM2INT(nth_bit)));
}

static VALUE
rbglib_m_bit_storage(G_GNUC_UNUSED VALUE self, VALUE number)
{
    return UINT2NUM(g_bit_storage(NUM2ULONG(number)));
}

static VALUE
rbglib_m_spaced_primes_closest(G_GNUC_UNUSED VALUE self, VALUE num)
{
    return UINT2NUM(g_spaced_primes_closest(NUM2UINT(num)));
}

/*
Use at_exit of ruby instead.
void        g_atexit                        (GVoidFunc func);
*/

static VALUE
rbglib_m_parse_debug_string(G_GNUC_UNUSED VALUE self, VALUE string, VALUE keys)
{
    gint i, nkeys;
    VALUE ary;
    GDebugKey* gkeys;

    Check_Type(keys, T_HASH);
    ary = rb_funcall(keys, rb_intern("to_a"), 0);
    nkeys = RARRAY_LEN(ary);
    gkeys = ALLOCA_N(GDebugKey, nkeys);
    for (i = 0; i < nkeys; i++) {
        gkeys[i].key = RVAL2CSTR(RARRAY_PTR(RARRAY_PTR(ary)[i])[0]);
        gkeys[i].value = NUM2UINT(RARRAY_PTR(RARRAY_PTR(ary)[i])[1]);
    }
   
    return UINT2NUM(g_parse_debug_string(RVAL2CSTR(string), gkeys, nkeys));
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
rbglib_m_check_version(G_GNUC_UNUSED VALUE self, VALUE major, VALUE minor, VALUE micro)
{
    return CBOOL2RVAL(glib_major_version > NUM2UINT(major) ||
                      (glib_major_version == NUM2UINT(major) && 
                       glib_minor_version > NUM2UINT(minor)) ||
                      (glib_major_version == NUM2UINT(major) && 
                       glib_minor_version == NUM2UINT(minor) &&
                       glib_micro_version >= NUM2UINT(micro)));
}

void
Init_glib_utils(void)
{
    /* glib/gutils.h */
#if GLIB_CHECK_VERSION(2, 14, 0)
    G_DEF_CLASS(G_TYPE_USER_DIRECTORY, "UserDirectory", mGLib);
    G_DEF_CONSTANTS(mGLib, G_TYPE_USER_DIRECTORY, "G_");
#endif

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
#ifdef HAVE_G_LISTENV
    rb_define_module_function(mGLib, "listenv", rbglib_m_listenv, 0);
#endif
    rb_define_module_function(mGLib, "host_name", rbglib_m_host_name, 0);
#endif
    rb_define_module_function(mGLib, "user_name", rbglib_m_user_name, 0);
    rb_define_module_function(mGLib, "real_name", rbglib_m_real_name, 0);

#if GLIB_CHECK_VERSION(2, 6, 0)
    rb_define_module_function(mGLib, "user_cache_dir",
			      rbglib_m_user_cache_dir, 0);
    rb_define_module_function(mGLib, "user_data_dir",
			      rbglib_m_user_data_dir, 0);
    rb_define_module_function(mGLib, "user_config_dir",
			      rbglib_m_user_config_dir, 0);
    rb_define_module_function(mGLib, "system_data_dirs",
			      rbglib_m_system_data_dirs, 0);
    rb_define_module_function(mGLib, "system_config_dirs",
			      rbglib_m_system_config_dirs, 0);
#endif
#if GLIB_CHECK_VERSION(2, 14, 0)
    rb_define_module_function(mGLib, "get_user_special_dir",
			      rbglib_m_get_user_special_dir, 1);
#endif
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

