/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#include "rbvte3private.h"

#define RG_TARGET_NAMESPACE cPty
#define _SELF(s) (RVAL2VTEPTY(s))

static VALUE
rg_initialize(VALUE self, VALUE flags_or_fd)
{
    VtePty *pty = NULL;
    GError *error = NULL;

    if (TYPE(flags_or_fd) == T_FIXNUM) {
        pty = vte_pty_new_foreign(NUM2INT(flags_or_fd), &error);
    } else {
        pty = vte_pty_new(RVAL2VTEPTYFLAGS(flags_or_fd), &error);
    }
    if (error)
        RAISE_GERROR(error);

    G_INITIALIZE(self, pty);

    return Qnil;
}

static VALUE
rg_child_setup(VALUE self)
{
    vte_pty_child_setup(_SELF(self));

    return self;
}

static VALUE
rg_close(VALUE self)
{
    vte_pty_close(_SELF(self));

    return self;
}

static VALUE
rg_size(VALUE self)
{
    int rows, columns;
    gboolean result;
    GError *error = NULL;

    result = vte_pty_get_size(_SELF(self), &rows, &columns, &error);
    if (error)
        RAISE_GERROR(error);

    return rb_ary_new3(2, INT2NUM(rows), INT2NUM(columns));
}

static VALUE
rg_set_size(VALUE self, VALUE rows, VALUE columns)
{
    gboolean result;
    GError *error = NULL;

    result = vte_pty_set_size(_SELF(self), NUM2INT(rows), NUM2INT(columns), &error);
    if (error)
        RAISE_GERROR(error);

    return self;
}

static VALUE
rg_set_utf8(VALUE self, VALUE utf8)
{
    gboolean result;
    GError *error = NULL;

    result = vte_pty_set_utf8(_SELF(self), RVAL2CBOOL(utf8), &error);
    if (error)
        RAISE_GERROR(error);

    return self;
}

void
Init_vte_pty(VALUE mVte)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(VTE_TYPE_PTY, "Pty", mVte);
    G_DEF_CLASS(VTE_TYPE_PTY_ERROR, "Error", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(VTE_TYPE_PTY_FLAGS, "Flags", RG_TARGET_NAMESPACE);

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(child_setup, 0);
    RG_DEF_METHOD(close, 0);
    RG_DEF_METHOD(size, 0);
    RG_DEF_METHOD(set_size, 2);
    RG_DEF_METHOD(set_utf8, 1);
}
