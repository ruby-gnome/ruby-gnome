/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012  Ruby-GNOME2 Project Team
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

#include "rbvlcprivate.h"

static libvlc_module_description_t *
vlc_module_description_copy(const libvlc_module_description_t *instance)
{
    libvlc_module_description_t *result = g_new(libvlc_module_description_t, 1);
    *result = *instance;
    result->psz_name        = g_strdup(instance->psz_name);
    result->psz_shortname   = g_strdup(instance->psz_shortname);
    result->psz_longname    = g_strdup(instance->psz_longname);
    result->psz_help        = g_strdup(instance->psz_help);
    return result;
}

static void
vlc_module_description_free(libvlc_module_description_t *instance)
{
    g_free(instance->psz_name);
    g_free(instance->psz_shortname);
    g_free(instance->psz_longname);
    g_free(instance->psz_help);
    g_free(instance);
}

GType
vlc_module_description_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static("VLCModuleDescription",
                                                (GBoxedCopyFunc)vlc_module_description_copy,
                                                (GBoxedFreeFunc)vlc_module_description_free);
    return our_type;
}

#define RG_TARGET_NAMESPACE cModuleDescription
#define _SELF(self) (RVAL2VLCMODULEDESCRIPTION(self))

static VALUE
rg_name(VALUE self)
{
    return CSTR2RVAL(_SELF(self)->psz_name);
}

static VALUE
rg_shortname(VALUE self)
{
    return CSTR2RVAL(_SELF(self)->psz_shortname);
}

static VALUE
rg_longname(VALUE self)
{
    return CSTR2RVAL(_SELF(self)->psz_longname);
}

static VALUE
rg_help(VALUE self)
{
    return CSTR2RVAL(_SELF(self)->psz_help);
}

void
Init_vlc_moduledescription(VALUE mVLC)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(VLC_TYPE_MODULE_DESCRIPTION, "ModuleDescription", mVLC);

    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(shortname, 0);
    RG_DEF_METHOD(longname, 0);
    RG_DEF_METHOD(help, 0);
}
