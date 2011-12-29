/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
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

#include "rbgio2private.h"

#define RG_TARGET_NAMESPACE cFilenameCompleter
#define _SELF(value) RVAL2GFILENAMECOMPLETER(value)

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
        VALUE dirs_only;
        GFilenameCompleter *completer;

        rb_scan_args(argc, argv, "01", &dirs_only);
        completer = g_filename_completer_new();
        g_filename_completer_set_dirs_only(completer, RVAL2CBOOL(dirs_only));

        G_INITIALIZE(self, completer);

        return Qnil;
}

static VALUE
rg_get_completion_suffix(VALUE self, VALUE initial_text)
{
        return CSTR2RVAL_FREE(g_filename_completer_get_completion_suffix(_SELF(self),
                                                                         RVAL2CSTR(initial_text)));
}

static VALUE
rg_get_completions(VALUE self, VALUE initial_text)
{
        return STRV2RVAL_FREE(g_filename_completer_get_completions(_SELF(self),
                                                                   RVAL2CSTR(initial_text)));
}

static VALUE
rg_set_dirs_only(VALUE self, VALUE dirs_only)
{
        g_filename_completer_set_dirs_only(_SELF(self), RVAL2CBOOL(dirs_only));

        return self;
}

void
Init_gfilenamecompleter(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_FILENAME_COMPLETER, "FilenameCompleter", mGio);

        RG_DEF_METHOD(initialize, -1);
        RG_DEF_METHOD(get_completion_suffix, 1);
        RG_DEF_METHOD(get_completions, 1);
        RG_DEF_METHOD(set_dirs_only, 1);
        G_DEF_SETTER(RG_TARGET_NAMESPACE, "dirs_only");
}
