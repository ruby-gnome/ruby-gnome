/* $Id: rbgnome-desktop-entry.c,v 1.2 2002/05/19 15:48:28 mutoh Exp $ */

/* Gnome::DesktopEntry class for Ruby/Gnome
 * Copyright (C) 2002 Neil Conway <neilconway@rogers.com>
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

VALUE gnoDesktopEntry;

static VALUE
dentry_s_load(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE clean_mem, file;
    GnomeDesktopEntry* entry;

    rb_scan_args(argc, argv, "11", &file, &clean_mem);
    if (NIL_P(clean_mem)) {
        entry = gnome_desktop_entry_load(STR2CSTR(file));
    } else {
        entry = gnome_desktop_entry_load_flags(STR2CSTR(file),
                                               NUM2INT(clean_mem));
    }
    return make_dentry(entry);
}

static VALUE
dentry_s_load_unconditional(self, file)
    VALUE self, file;
{
    return make_dentry(gnome_desktop_entry_load_unconditional(STR2CSTR(file)));
}

static VALUE
dentry_s_load_conditional(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE file, clean_mem, unconditional;
    rb_scan_args(argc, argv, "12", &file, &clean_mem, &unconditional);

    if (NIL_P(clean_mem)) {
        clean_mem = Qfalse;
    }
    if (NIL_P(unconditional)) {
        unconditional = Qfalse;
    }

    return make_dentry(
            gnome_desktop_entry_load_flags_conditional(STR2CSTR(file),
                                                       NUM2INT(clean_mem),
                                                       NUM2INT(unconditional)));
}

static VALUE
dentry_save(self)
    VALUE self;
{
    gnome_desktop_entry_save(get_dentry(self));
    return Qnil;
}

static VALUE
dentry_destroy(self)
    VALUE self;
{
    gnome_desktop_entry_destroy(get_dentry(self));
    return Qnil;
}

static VALUE
dentry_launch(self, args)
    VALUE self, args;
{
    if (NIL_P(args)) {
        gnome_desktop_entry_launch(get_dentry(self));
    } else {
        /* TODO */
        rb_notimplement();
    }
    return Qnil;
}

static VALUE
dentry_copy(self)
    VALUE self;
{
    return make_dentry(gnome_desktop_entry_copy(get_dentry(self)));
}

static VALUE
dentry_name(self)
    VALUE self;
{
    return rb_str_new2(get_dentry(self)->name);
}

static VALUE
dentry_comment(self)
    VALUE self;
{
    return rb_str_new2(get_dentry(self)->comment);
}

static VALUE
dentry_exec_length(self)
    VALUE self;
{
    return INT2NUM(get_dentry(self)->exec_length);
}

static VALUE
dentry_tryexec(self)
    VALUE self;
{
    return rb_str_new2(get_dentry(self)->tryexec);
}

static VALUE
dentry_icon(self)
    VALUE self;
{
    return rb_str_new2(get_dentry(self)->icon);
}

static VALUE
dentry_docpath(self)
    VALUE self;
{
    return rb_str_new2(get_dentry(self)->docpath);
}

static VALUE
dentry_is_terminal(self)
    VALUE self;
{
    return (get_dentry(self)->terminal) ? Qtrue: Qfalse;
}

static VALUE
dentry_type(self)
    VALUE self;
{
    return rb_str_new2(get_dentry(self)->type);
}

static VALUE
dentry_location(self)
    VALUE self;
{
    return rb_str_new2(get_dentry(self)->location);
}

static VALUE
dentry_geometry(self)
    VALUE self;
{
    return rb_str_new2(get_dentry(self)->geometry);
}

static VALUE
dentry_is_multiple_args(self)
    VALUE self;
{
    return (get_dentry(self)->multiple_args) ? Qtrue : Qfalse;
}

static VALUE
dentry_is_kde(self)
    VALUE self;
{
    return (get_dentry(self)->is_kde) ? Qtrue : Qfalse;
}

GnomeDesktopEntry*
get_dentry(self)
    VALUE self;
{
    GnomeDesktopEntry* dentry;
    Data_Get_Struct(self, GnomeDesktopEntry, dentry);
    return dentry;
}

VALUE
make_dentry(dentry)
    GnomeDesktopEntry* dentry;
{
    VALUE res;
    if (dentry) {
        res = Data_Wrap_Struct(gnoDesktopEntry, 0, g_free, dentry);
        rb_obj_call_init(res, 0, 0);
    } else {
        res = Qnil;
    }
    return res;
}

void
Init_gnome_desktop_entry()
{
    gnoDesktopEntry = rb_define_class_under(mGnome, "DesktopEntry", rb_cObject);

    rb_define_singleton_method(gnoDesktopEntry, "load", dentry_s_load, -1);
    /* effectively an alias */
    rb_define_singleton_method(gnoDesktopEntry, "new", dentry_s_load, -1);
    rb_define_singleton_method(gnoDesktopEntry, "load_flags",
                               dentry_s_load, -1);
    rb_define_singleton_method(gnoDesktopEntry, "load_conditional",
                               dentry_s_load_conditional, -1);
    /* effectively an alias */
    rb_define_singleton_method(gnoDesktopEntry, "load_conditional_flags",
                               dentry_s_load_conditional, -1);
    rb_define_singleton_method(gnoDesktopEntry, "load_unconditional",
                               dentry_s_load_unconditional, 1);

    rb_define_method(gnoDesktopEntry, "save", dentry_save, 0);
    rb_define_method(gnoDesktopEntry, "destroy", dentry_destroy, 0);
    rb_define_method(gnoDesktopEntry, "launch", dentry_launch, -2);
    rb_define_method(gnoDesktopEntry, "copy", dentry_copy, 0);
    rb_define_method(gnoDesktopEntry, "name", dentry_name, 0);
    rb_define_method(gnoDesktopEntry, "comment", dentry_comment, 0);
    rb_define_method(gnoDesktopEntry, "exec_length", dentry_exec_length, 0);
    rb_define_method(gnoDesktopEntry, "tryexec", dentry_tryexec, 0);
    rb_define_method(gnoDesktopEntry, "icon", dentry_icon, 0);
    rb_define_method(gnoDesktopEntry, "docpath", dentry_docpath, 0);
    rb_define_method(gnoDesktopEntry, "terminal?", dentry_is_terminal, 0);
    rb_define_method(gnoDesktopEntry, "type", dentry_type, 0);
    rb_define_method(gnoDesktopEntry, "location", dentry_location, 0);
    rb_define_method(gnoDesktopEntry, "geometry", dentry_geometry, 0);
    rb_define_method(gnoDesktopEntry, "multiple_args?", dentry_is_multiple_args, 0);
    rb_define_method(gnoDesktopEntry, "kde?", dentry_is_kde, 0);

    rb_define_alias(gnoDesktopEntry, "launch_with_args", "launch");
}
