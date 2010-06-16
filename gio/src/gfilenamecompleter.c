/* -*- c-file-style: "ruby" -*- */
/*
 * Ruby/GIO: a Ruby binding of gio-2.0.x.
 * Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gio2.h"

#define _SELF(value) G_FILENAME_COMPLETER(RVAL2GOBJ(value))

static VALUE
filenamecompleter_initialize(VALUE self)
{
        G_INITIALIZE(self, g_filename_completer_new());

        return Qnil;
}

static VALUE
filenamecompleter_get_completion_suffix(VALUE self, VALUE initial_text)
{
	return CSTR2RVAL_FREE(g_filename_completer_get_completion_suffix(_SELF(self), RVAL2CSTR(initial_text)));
}

static VALUE
filenamecompleter_get_completions(VALUE self, VALUE initial_text)
{
	return str_vector_to_ary(g_filename_completer_get_completions(_SELF(self), RVAL2CSTR(initial_text)));
}

static VALUE
filenamecompleter_set_dirs_only(VALUE self, VALUE dirs_only)
{
        g_filename_completer_set_dirs_only(_SELF(self), RVAL2CBOOL(dirs_only));

        return self;
}

void
Init_gfilenamecompleter(VALUE glib)
{
        VALUE filenamecompleter = G_DEF_INTERFACE(G_TYPE_FILENAME_COMPLETER, "FilenameCompleter", glib);

        rb_define_method(filenamecompleter, "initialize", filenamecompleter_initialize, 0);
        rb_define_method(filenamecompleter, "get_completion_suffix", filenamecompleter_get_completion_suffix, 1);
        rb_define_method(filenamecompleter, "get_completions", filenamecompleter_get_completions, 1);
        /* TODO: Allow this attribute to be set from .new. */
        rb_define_method(filenamecompleter, "set_dirs_only", filenamecompleter_set_dirs_only, 1);
	G_DEF_SETTER(filenamecompleter, "dirs_only");
}
