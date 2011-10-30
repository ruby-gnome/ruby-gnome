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

#define RG_TARGET_NAMESPACE mGLib

#if GLIB_CHECK_VERSION(2,2,0)
static VALUE
rg_m_application_name(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_application_name());
}

static VALUE
rg_m_set_application_name(VALUE self, VALUE application_name)
{
    g_set_prgname(NIL_P(application_name) ? NULL : RVAL2CSTR(application_name));
    return self;
}
#endif

static VALUE
rg_m_prgname(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_prgname());
}

static VALUE
rg_m_set_prgname(VALUE self, VALUE prgname)
{
    g_set_prgname(NIL_P(prgname) ? NULL : RVAL2CSTR(prgname));
    return self;
}

static VALUE
rg_m_getenv(G_GNUC_UNUSED VALUE self, VALUE variable)
{
    return CSTR2RVAL(g_getenv(RVAL2CSTR(variable)));
}

#if GLIB_CHECK_VERSION(2,4,0)
static VALUE
rg_m_setenv(G_GNUC_UNUSED VALUE self, VALUE variable, VALUE value, VALUE overwrite)
{
    return CBOOL2RVAL(g_setenv(RVAL2CSTR(variable), 
                               NIL_P(value) ? NULL : RVAL2CSTR(value),
                               RVAL2CBOOL(overwrite)));
}

static VALUE
rg_m_unsetenv(VALUE self, VALUE variable)
{
    g_unsetenv(RVAL2CSTR(variable));
    return self;
}
#endif

#if GLIB_CHECK_VERSION(2,8,0)
#ifdef HAVE_G_LISTENV
static VALUE
rg_m_listenv(VALUE self)
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
rg_m_host_name(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_host_name());
}
#endif

static VALUE
rg_m_user_name(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_user_name());
}

static VALUE
rg_m_real_name(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_real_name());
}

#if GLIB_CHECK_VERSION(2, 6, 0)
static VALUE
rg_m_user_cache_dir(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_user_cache_dir());
}

static VALUE
rg_m_user_data_dir(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_user_data_dir());
}

static VALUE
rg_m_user_config_dir(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_user_config_dir());
}

static VALUE
rg_m_system_data_dirs(G_GNUC_UNUSED VALUE self)
{
    return STRV2RVAL((const gchar **)g_get_system_data_dirs());
}

static VALUE
rg_m_system_config_dirs(G_GNUC_UNUSED VALUE self)
{
    return STRV2RVAL((const gchar **)g_get_system_config_dirs());
}
#endif

#if GLIB_CHECK_VERSION(2, 14, 0)
static VALUE
rg_m_get_user_special_dir(G_GNUC_UNUSED VALUE self, VALUE directory)
{
    return CSTR2RVAL(g_get_user_special_dir(RVAL2GENUM(directory,
                                           G_TYPE_USER_DIRECTORY)));
}
#endif

static VALUE
rg_m_home_dir(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_home_dir());
}

static VALUE
rg_m_tmp_dir(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(g_get_tmp_dir());
}

static VALUE
rg_m_current_dir(G_GNUC_UNUSED VALUE self)
{
    gchar* dir = g_get_current_dir();
    VALUE ret = CSTR2RVAL(dir);
    g_free(dir);
    return ret;
}

static VALUE
rg_m_path_is_absolute_p(G_GNUC_UNUSED VALUE self, VALUE fname)
{
    return CBOOL2RVAL(g_path_is_absolute(RVAL2CSTR(fname)));
}

static VALUE
rg_m_path_skip_root(G_GNUC_UNUSED VALUE self, VALUE fname)
{
    return CSTR2RVAL(g_path_skip_root(RVAL2CSTR(fname)));
}

static VALUE
rg_m_path_get_basename(G_GNUC_UNUSED VALUE self, VALUE fname)
{
    return CSTR2RVAL(g_path_get_basename(RVAL2CSTR(fname)));
}

static VALUE
rg_m_path_get_dirname(G_GNUC_UNUSED VALUE self, VALUE fname)
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
rg_m_find_program_in_path(G_GNUC_UNUSED VALUE self, VALUE program)
{
    gchar* path = g_find_program_in_path(RVAL2CSTR(program));
    VALUE ret = CSTR2RVAL(path);
    g_free(path);
    return ret;
}

static VALUE
rg_m_bit_nth_lsf(G_GNUC_UNUSED VALUE self, VALUE mask, VALUE nth_bit)
{
    return INT2NUM(g_bit_nth_lsf(NUM2ULONG(mask), NUM2INT(nth_bit)));
}

static VALUE
rg_m_bit_nth_msf(G_GNUC_UNUSED VALUE self, VALUE mask, VALUE nth_bit)
{
    return INT2NUM(g_bit_nth_msf(NUM2ULONG(mask), NUM2INT(nth_bit)));
}

static VALUE
rg_m_bit_storage(G_GNUC_UNUSED VALUE self, VALUE number)
{
    return UINT2NUM(g_bit_storage(NUM2ULONG(number)));
}

static VALUE
rg_m_spaced_primes_closest(G_GNUC_UNUSED VALUE self, VALUE num)
{
    return UINT2NUM(g_spaced_primes_closest(NUM2UINT(num)));
}

/*
Use at_exit of ruby instead.
void        g_atexit                        (GVoidFunc func);
*/

static VALUE
rg_m_parse_debug_string(G_GNUC_UNUSED VALUE self, VALUE string, VALUE keys)
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
rg_m_check_version_p(G_GNUC_UNUSED VALUE self, VALUE major, VALUE minor, VALUE micro)
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
    G_DEF_CLASS(G_TYPE_USER_DIRECTORY, "UserDirectory", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_USER_DIRECTORY, "G_");
#endif

#if GLIB_CHECK_VERSION(2,2,0)
    RG_DEF_MODFUNC(application_name, 0);
    RG_DEF_MODFUNC(set_application_name, 1);
#endif
    RG_DEF_MODFUNC(prgname, 0);
    RG_DEF_MODFUNC(set_prgname, 1);
    RG_DEF_MODFUNC(getenv, 1);
#if GLIB_CHECK_VERSION(2,4,0)
    RG_DEF_MODFUNC(setenv, 2);
    RG_DEF_MODFUNC(unsetenv, 1);
#endif
#if GLIB_CHECK_VERSION(2,8,0)
#ifdef HAVE_G_LISTENV
    RG_DEF_MODFUNC(listenv, 0);
#endif
    RG_DEF_MODFUNC(host_name, 0);
#endif
    RG_DEF_MODFUNC(user_name, 0);
    RG_DEF_MODFUNC(real_name, 0);

#if GLIB_CHECK_VERSION(2, 6, 0)
    RG_DEF_MODFUNC(user_cache_dir, 0);
    RG_DEF_MODFUNC(user_data_dir, 0);
    RG_DEF_MODFUNC(user_config_dir, 0);
    RG_DEF_MODFUNC(system_data_dirs, 0);
    RG_DEF_MODFUNC(system_config_dirs, 0);
#endif
#if GLIB_CHECK_VERSION(2, 14, 0)
    RG_DEF_MODFUNC(get_user_special_dir, 1);
#endif
    RG_DEF_MODFUNC(home_dir, 0);
    RG_DEF_MODFUNC(tmp_dir, 0);
    RG_DEF_MODFUNC(current_dir, 0);

    RG_DEF_MODFUNC_P(path_is_absolute, 1);
    RG_DEF_MODFUNC(path_skip_root, 1);
    RG_DEF_MODFUNC(path_get_basename, 1);
    RG_DEF_MODFUNC(path_get_dirname, 1);
    RG_DEF_MODFUNC(find_program_in_path, 1);
    RG_DEF_MODFUNC(bit_nth_lsf, 2);
    RG_DEF_MODFUNC(bit_nth_msf, 2);
    RG_DEF_MODFUNC(bit_storage, 1);
    RG_DEF_MODFUNC(spaced_primes_closest, 1);
    RG_DEF_MODFUNC(parse_debug_string, 2);
    RG_DEF_MODFUNC_P(check_version, 3);

}