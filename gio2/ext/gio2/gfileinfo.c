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

#define _SELF(value) RVAL2GFILEINFO(value)

#define GFILEATTRIBUTESTATUS2RVAL(value) \
        GENUM2RVAL((value), G_TYPE_FILE_ATTRIBUTE_STATUS)

#define RVAL2GFILEATTRIBUTESTATUS(value) \
        GENUM2RVAL((value), G_TYPE_FILE_ATTRIBUTE_STATUS)

#define RVAL2FILEINFONAMESPACEDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), RVAL2CSTR, "*")

static VALUE
fileinfo_initialize(VALUE self)
{
        G_INITIALIZE(self, g_file_info_new());

        return Qnil;
}

static VALUE
fileinfo_dup(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_file_info_dup(_SELF(self)));
}

/* TODO: Does this make sense from the Ruby side? */
static VALUE
fileinfo_copy_into(VALUE self, VALUE other)
{
        g_file_info_copy_into(_SELF(self), _SELF(other));

        return self;
}

static VALUE
fileinfo_has_attribute(VALUE self, VALUE attribute)
{
        return CBOOL2RVAL(g_file_info_has_attribute(_SELF(self), RVAL2CSTR(attribute)));
}

static VALUE
fileinfo_has_namespace(VALUE self, VALUE namespace)
{
        return CBOOL2RVAL(g_file_info_has_namespace(_SELF(self), RVAL2CSTR(namespace)));
}

static VALUE
fileinfo_set_attribute_status(VALUE self, VALUE attribute, VALUE status)
{
        return CBOOL2RVAL(g_file_info_set_attribute_status(_SELF(self),
                                                           RVAL2CSTR(attribute),
                                                           RVAL2GFILEATTRIBUTESTATUS(status)));
}

static VALUE
fileinfo_list_attributes(int argc, VALUE *argv, VALUE self)
{
        VALUE name_space;

        rb_scan_args(argc, argv, "01", &name_space);

        return STRVECTOR2ARY_FREE(g_file_info_list_attributes(_SELF(self),
                                                              RVAL2FILEINFONAMESPACEDEFAULT(name_space)));
}

static VALUE
fileinfo_get_attribute_type(VALUE self, VALUE attribute)
{
        return GFILEATTRIBUTETYPE2RVAL(g_file_info_get_attribute_type(_SELF(self), RVAL2CSTR(attribute)));
}

static VALUE
fileinfo_remove_attribute(VALUE self, VALUE attribute)
{
        g_file_info_remove_attribute(_SELF(self), RVAL2CSTR(attribute));

        return self;
}

static VALUE
fileinfo_get_attribute_as_string(VALUE self, VALUE attribute)
{
        return CSTR2RVAL_FREE(g_file_info_get_attribute_as_string(_SELF(self), RVAL2CSTR(attribute)));
}

static VALUE
file_attribute_to_value(GFileAttributeType type, gpointer value)
{
        switch (type) {
        case G_FILE_ATTRIBUTE_TYPE_STRING:
        case G_FILE_ATTRIBUTE_TYPE_BYTE_STRING:
                return CSTR2RVAL(value);
        case G_FILE_ATTRIBUTE_TYPE_BOOLEAN:
                return CBOOL2RVAL(*(gboolean *)value);
        case G_FILE_ATTRIBUTE_TYPE_UINT32:
                return GUINT322RVAL(*(guint32 *)value);
        case G_FILE_ATTRIBUTE_TYPE_INT32:
                return GINT322RVAL(*(gint32 *)value);
        case G_FILE_ATTRIBUTE_TYPE_UINT64:
                return GUINT642RVAL(*(guint64 *)value);
        case G_FILE_ATTRIBUTE_TYPE_INT64:
                return GINT642RVAL(*(gint64 *)value);
        case G_FILE_ATTRIBUTE_TYPE_OBJECT:
                return GOBJ2RVAL((GObject *)value);
        case G_FILE_ATTRIBUTE_TYPE_STRINGV:
                return STRVECTOR2ARY_FREE((char **)value);
        case G_FILE_ATTRIBUTE_TYPE_INVALID:
                rb_raise(rb_eArgError, "invalid file attribute type");
                break;
        default:
                rb_raise(rb_eArgError, "unknown file attribute type");
                break;
        }
}

static VALUE
fileinfo_get_attribute_data(VALUE self, VALUE attribute)
{
        GFileAttributeType type;
        gpointer value;
        GFileAttributeStatus status;

        if (!g_file_info_get_attribute_data(_SELF(self),
                                            RVAL2CSTR(attribute),
                                            &type,
                                            &value,
                                            &status))
                return Qnil;

        return rb_ary_new3(3,
                           GFILEATTRIBUTETYPE2RVAL(type),
                           file_attribute_to_value(type, value),
                           GFILEATTRIBUTESTATUS2RVAL(status));
}

static VALUE
fileinfo_get_attribute_status(VALUE self, VALUE attribute)
{
        return GFILEATTRIBUTESTATUS2RVAL(g_file_info_get_attribute_status(_SELF(self),
                                                                          RVAL2CSTR(attribute)));
}

static VALUE
fileinfo_get_attribute_string(VALUE self, VALUE attribute)
{
        return CSTR2RVAL(g_file_info_get_attribute_string(_SELF(self),
                                                          RVAL2CSTR(attribute)));
}

static VALUE
fileinfo_get_attribute_stringv(VALUE self, VALUE attribute)
{
        return STRVECTOR2ARY(g_file_info_get_attribute_stringv(_SELF(self),
                                                               RVAL2CSTR(attribute)));
}

static VALUE
fileinfo_get_attribute_byte_string(VALUE self, VALUE attribute)
{
        return CSTR2RVAL(g_file_info_get_attribute_byte_string(_SELF(self),
                                                               RVAL2CSTR(attribute)));
}

static VALUE
fileinfo_get_attribute_boolean(VALUE self, VALUE attribute)
{
        return CBOOL2RVAL(g_file_info_get_attribute_boolean(_SELF(self),
                                                            RVAL2CSTR(attribute)));
}

static VALUE
fileinfo_get_attribute_uint32(VALUE self, VALUE attribute)
{
        return GUINT322RVAL(g_file_info_get_attribute_uint32(_SELF(self),
                                                             RVAL2CSTR(attribute)));
}

static VALUE
fileinfo_get_attribute_int32(VALUE self, VALUE attribute)
{
        return GINT322RVAL(g_file_info_get_attribute_int32(_SELF(self),
                                                           RVAL2CSTR(attribute)));
}

static VALUE
fileinfo_get_attribute_uint64(VALUE self, VALUE attribute)
{
        return GUINT642RVAL(g_file_info_get_attribute_uint64(_SELF(self),
                                                             RVAL2CSTR(attribute)));
}

static VALUE
fileinfo_get_attribute_int64(VALUE self, VALUE attribute)
{
        return GINT642RVAL(g_file_info_get_attribute_int64(_SELF(self),
                                                           RVAL2CSTR(attribute)));
}

static VALUE
fileinfo_get_attribute_object(VALUE self, VALUE attribute)
{
        return GOBJ2RVAL(g_file_info_get_attribute_object(_SELF(self),
                                                          RVAL2CSTR(attribute)));
}

static VALUE
fileinfo_get_attribute_value(VALUE self, VALUE attribute)
{
        GFileAttributeType type;
        gpointer value;
        GFileAttributeStatus status;

        if (!g_file_info_get_attribute_data(_SELF(self),
                                            RVAL2CSTR(attribute),
                                            &type,
                                            &value,
                                            &status))
                return Qnil;

        return file_attribute_to_value(type, value);
}

/* TODO: Do we need set_attribute? */

static VALUE
fileinfo_set_attribute_string(VALUE self, VALUE attribute, VALUE value)
{
        g_file_info_set_attribute_string(_SELF(self),
                                         RVAL2CSTR(attribute),
                                         RVAL2CSTR(value));

        return self;
}

static VALUE
fileinfo_set_attribute_byte_string(VALUE self, VALUE attribute, VALUE value)
{
        g_file_info_set_attribute_byte_string(_SELF(self),
                                              RVAL2CSTR(attribute),
                                              RVAL2CSTR(value));

        return self;
}

static VALUE
fileinfo_set_attribute_boolean(VALUE self, VALUE attribute, VALUE value)
{
        g_file_info_set_attribute_boolean(_SELF(self),
                                          RVAL2CSTR(attribute),
                                          RVAL2CBOOL(value));

        return self;
}

static VALUE
fileinfo_set_attribute_uint32(VALUE self, VALUE attribute, VALUE value)
{
        g_file_info_set_attribute_uint32(_SELF(self),
                                         RVAL2CSTR(attribute),
                                         RVAL2GUINT32(value));

        return self;
}

static VALUE
fileinfo_set_attribute_int32(VALUE self, VALUE attribute, VALUE value)
{
        g_file_info_set_attribute_int32(_SELF(self),
                                        RVAL2CSTR(attribute),
                                        RVAL2GINT32(value));

        return self;
}

static VALUE
fileinfo_set_attribute_uint64(VALUE self, VALUE attribute, VALUE value)
{
        g_file_info_set_attribute_uint64(_SELF(self),
                                         RVAL2CSTR(attribute),
                                         RVAL2GUINT64(value));

        return self;
}

static VALUE
fileinfo_set_attribute_int64(VALUE self, VALUE attribute, VALUE value)
{
        g_file_info_set_attribute_int64(_SELF(self),
                                        RVAL2CSTR(attribute),
                                        RVAL2GINT64(value));

        return self;
}

static VALUE
fileinfo_set_attribute_object(VALUE self, VALUE attribute, VALUE value)
{
        g_file_info_set_attribute_object(_SELF(self),
                                         RVAL2CSTR(attribute),
                                         RVAL2GOBJ(value));

        return self;
}

/* TODO: How do we implement set_attribute_value? */

static VALUE
fileinfo_clear_status(VALUE self)
{
        g_file_info_clear_status(_SELF(self));

        return self;
}

static VALUE
fileinfo_get_file_type(VALUE self)
{
        return GFILETYPE2RVAL(g_file_info_get_file_type(_SELF(self)));
}

static VALUE
fileinfo_is_hidden(VALUE self)
{
        return CBOOL2RVAL(g_file_info_get_is_hidden(_SELF(self)));
}

static VALUE
fileinfo_is_backup(VALUE self)
{
        return CBOOL2RVAL(g_file_info_get_is_backup(_SELF(self)));
}

static VALUE
fileinfo_is_symlink(VALUE self)
{
        return CBOOL2RVAL(g_file_info_get_is_symlink(_SELF(self)));
}

static VALUE
fileinfo_get_name(VALUE self)
{
        return CSTR2RVAL(g_file_info_get_name(_SELF(self)));
}

static VALUE
fileinfo_get_display_name(VALUE self)
{
        return CSTR2RVAL(g_file_info_get_display_name(_SELF(self)));
}

static VALUE
fileinfo_get_edit_name(VALUE self)
{
        return CSTR2RVAL(g_file_info_get_edit_name(_SELF(self)));
}

static VALUE
fileinfo_get_icon(VALUE self)
{
        return GOBJ2RVAL(g_file_info_get_icon(_SELF(self)));
}

static VALUE
fileinfo_get_content_type(VALUE self)
{
        return CSTR2RVAL(g_file_info_get_content_type(_SELF(self)));
}

static VALUE
fileinfo_get_size(VALUE self)
{
        return GOFFSET2RVAL(g_file_info_get_size(_SELF(self)));
}

static VALUE
fileinfo_get_modification_time(VALUE self)
{
        GTimeVal time;

        g_file_info_get_modification_time(_SELF(self), &time);

        return GTIMEVAL2RVAL(&time);
}

static VALUE
fileinfo_get_symlink_target(VALUE self)
{
        return CSTR2RVAL(g_file_info_get_symlink_target(_SELF(self)));
}

static VALUE
fileinfo_get_etag(VALUE self)
{
        return CSTR2RVAL(g_file_info_get_etag(_SELF(self)));
}

static VALUE
fileinfo_get_sort_order(VALUE self)
{
        return GINT322RVAL(g_file_info_get_sort_order(_SELF(self)));
}

static VALUE
fileinfo_set_attribute_mask(VALUE self, VALUE rbmask)
{
        gboolean should_unref;
        GFileAttributeMatcher *mask;

        if (NIL_P(rbmask)) {
                g_file_info_unset_attribute_mask(_SELF(self));
                return self;
        }

        if (TYPE(rbmask) == T_STRING) {
                mask = g_file_attribute_matcher_new(RVAL2CSTR(rbmask));
                should_unref = TRUE;
        } else {
                mask = RVAL2GFILEATTRIBUTEMATCHER(rbmask);
                should_unref = FALSE;
        }

        g_file_info_set_attribute_mask(_SELF(self), mask);

        if (should_unref)
                g_file_attribute_matcher_unref(mask);

        return self;
}

static VALUE
fileinfo_unset_attribute_mask(VALUE self)
{
        g_file_info_unset_attribute_mask(_SELF(self));

        return self;
}

static VALUE
fileinfo_set_file_type(VALUE self, VALUE value)
{
        g_file_info_set_file_type(_SELF(self), RVAL2GFILETYPE(value));

        return self;
}

static VALUE
fileinfo_set_is_hidden(VALUE self, VALUE value)
{
        g_file_info_set_is_hidden(_SELF(self), RVAL2CBOOL(value));

        return self;
}

static VALUE
fileinfo_set_is_symlink(VALUE self, VALUE value)
{
        g_file_info_set_is_symlink(_SELF(self), RVAL2CBOOL(value));

        return self;
}

static VALUE
fileinfo_set_name(VALUE self, VALUE value)
{
        g_file_info_set_name(_SELF(self), RVAL2CSTR(value));

        return self;
}

static VALUE
fileinfo_set_display_name(VALUE self, VALUE value)
{
        g_file_info_set_display_name(_SELF(self), RVAL2CSTR(value));

        return self;
}

static VALUE
fileinfo_set_edit_name(VALUE self, VALUE value)
{
        g_file_info_set_edit_name(_SELF(self), RVAL2CSTR(value));

        return self;
}

static VALUE
fileinfo_set_icon(VALUE self, VALUE value)
{
        g_file_info_set_icon(_SELF(self), RVAL2GICON(value));

        return self;
}

static VALUE
fileinfo_set_content_type(VALUE self, VALUE value)
{
        g_file_info_set_content_type(_SELF(self), RVAL2CSTR(value));

        return self;
}

static VALUE
fileinfo_set_size(VALUE self, VALUE value)
{
        g_file_info_set_size(_SELF(self), RVAL2GOFFSET(value));

        return self;
}

static VALUE
fileinfo_set_modification_time(VALUE self, VALUE rbtime)
{
        GTimeVal time;

        rbgio_rval_to_gtimeval(rbtime, &time);

        g_file_info_set_modification_time(_SELF(self), &time);

        return self;
}

static VALUE
fileinfo_set_symlink_target(VALUE self, VALUE value)
{
        g_file_info_set_symlink_target(_SELF(self), RVAL2CSTR(value));

        return self;
}

static VALUE
fileinfo_set_sort_order(VALUE self, VALUE value)
{
        g_file_info_set_sort_order(_SELF(self), RVAL2GINT32(value));

        return self;
}

void
Init_gfileinfo(VALUE glib)
{
        VALUE fileinfo = G_DEF_CLASS(G_TYPE_FILE_INFO, "FileInfo", glib);

        G_DEF_CLASS(G_TYPE_FILE_TYPE, "FileType", glib);
        G_DEF_CONSTANTS(glib, G_TYPE_FILE_TYPE, "G_FILE_");

        rb_define_method(fileinfo, "initialize", fileinfo_initialize, 0);
        rb_define_method(fileinfo, "dup", fileinfo_dup, 0);
        rb_define_method(fileinfo, "copy_into", fileinfo_copy_into, 1);
        rb_define_method(fileinfo, "has_attribute?", fileinfo_has_attribute, 1);
        rb_define_method(fileinfo, "has_namespace?", fileinfo_has_namespace, 1);
        rb_define_method(fileinfo, "list_attributes", fileinfo_list_attributes, -1);
        rb_define_alias(fileinfo, "attributes", "list_attributes");
        rb_define_method(fileinfo, "get_attribute_type", fileinfo_get_attribute_type, 1);
        rb_define_method(fileinfo, "remove_attribute", fileinfo_remove_attribute, 1);
        rb_define_method(fileinfo, "get_attribute_as_string", fileinfo_get_attribute_as_string, 1);
        rb_define_method(fileinfo, "get_attribute_data", fileinfo_get_attribute_data, 1);
        rb_define_method(fileinfo, "get_attribute_status", fileinfo_get_attribute_status, 1);
        rb_define_method(fileinfo, "get_attribute_string", fileinfo_get_attribute_string, 1);
        rb_define_method(fileinfo, "get_attribute_stringv", fileinfo_get_attribute_stringv, 1);
        rb_define_method(fileinfo, "get_attribute_byte_string", fileinfo_get_attribute_byte_string, 1);
        rb_define_method(fileinfo, "get_attribute_boolean", fileinfo_get_attribute_boolean, 1);
        rb_define_method(fileinfo, "get_attribute_uint32", fileinfo_get_attribute_uint32, 1);
        rb_define_method(fileinfo, "get_attribute_int32", fileinfo_get_attribute_int32, 1);
        rb_define_method(fileinfo, "get_attribute_uint64", fileinfo_get_attribute_uint64, 1);
        rb_define_method(fileinfo, "get_attribute_int64", fileinfo_get_attribute_int64, 1);
        rb_define_method(fileinfo, "get_attribute_object", fileinfo_get_attribute_object, 1);
        rb_define_method(fileinfo, "get_attribute_value", fileinfo_get_attribute_value, 1);
        rb_define_alias(fileinfo, "[]", "get_attribute_value");
        rb_define_method(fileinfo, "set_attribute_status", fileinfo_set_attribute_status, 2);
        rb_define_method(fileinfo, "set_attribute_string", fileinfo_set_attribute_string, 2);
        rb_define_method(fileinfo, "set_attribute_byte_string", fileinfo_set_attribute_byte_string, 2);
        rb_define_method(fileinfo, "set_attribute_boolean", fileinfo_set_attribute_boolean, 2);
        rb_define_method(fileinfo, "set_attribute_uint32", fileinfo_set_attribute_uint32, 2);
        rb_define_method(fileinfo, "set_attribute_int32", fileinfo_set_attribute_int32, 2);
        rb_define_method(fileinfo, "set_attribute_uint64", fileinfo_set_attribute_uint64, 2);
        rb_define_method(fileinfo, "set_attribute_int64", fileinfo_set_attribute_int64, 2);
        rb_define_method(fileinfo, "set_attribute_object", fileinfo_set_attribute_object, 2);
        rb_define_method(fileinfo, "clear_status", fileinfo_clear_status, 0);
        rb_define_method(fileinfo, "file_type", fileinfo_get_file_type, 0);
        rb_define_method(fileinfo, "hidden?", fileinfo_is_hidden, 0);
        rb_define_method(fileinfo, "backup?", fileinfo_is_backup, 0);
        rb_define_method(fileinfo, "symlink?", fileinfo_is_symlink, 0);
        rb_define_method(fileinfo, "name", fileinfo_get_name, 0);
        rb_define_method(fileinfo, "display_name", fileinfo_get_display_name, 0);
        rb_define_method(fileinfo, "edit_name", fileinfo_get_edit_name, 0);
        rb_define_method(fileinfo, "icon", fileinfo_get_icon, 0);
        rb_define_method(fileinfo, "content_type", fileinfo_get_content_type, 0);
        rb_define_method(fileinfo, "size", fileinfo_get_size, 0);
        rb_define_method(fileinfo, "modification_time", fileinfo_get_modification_time, 0);
        rb_define_method(fileinfo, "symlink_target", fileinfo_get_symlink_target, 0);
        rb_define_method(fileinfo, "etag", fileinfo_get_etag, 0);
        rb_define_method(fileinfo, "sort_order", fileinfo_get_sort_order, 0);
        rb_define_method(fileinfo, "set_attribute_mask", fileinfo_set_attribute_mask, 1);
        G_DEF_SETTER(fileinfo, "attribute_mask");
        rb_define_method(fileinfo, "unset_attribute_mask", fileinfo_unset_attribute_mask, 0);
        rb_define_method(fileinfo, "set_file_type", fileinfo_set_file_type, 1);
        G_DEF_SETTER(fileinfo, "file_type");
        rb_define_method(fileinfo, "set_hidden", fileinfo_set_is_hidden, 1);
        G_DEF_SETTER(fileinfo, "hidden");
        rb_define_method(fileinfo, "set_symlink", fileinfo_set_is_symlink, 1);
        G_DEF_SETTER(fileinfo, "symlink");
        rb_define_method(fileinfo, "set_name", fileinfo_set_name, 1);
        G_DEF_SETTER(fileinfo, "name");
        rb_define_method(fileinfo, "set_display_name", fileinfo_set_display_name, 1);
        G_DEF_SETTER(fileinfo, "display_name");
        rb_define_method(fileinfo, "set_edit_name", fileinfo_set_edit_name, 1);
        G_DEF_SETTER(fileinfo, "edit_name");
        rb_define_method(fileinfo, "set_icon", fileinfo_set_icon, 1);
        G_DEF_SETTER(fileinfo, "icon");
        rb_define_method(fileinfo, "set_content_type", fileinfo_set_content_type, 1);
        G_DEF_SETTER(fileinfo, "content_type");
        rb_define_method(fileinfo, "set_size", fileinfo_set_size, 1);
        G_DEF_SETTER(fileinfo, "size");
        rb_define_method(fileinfo, "set_modification_time", fileinfo_set_modification_time, 1);
        G_DEF_SETTER(fileinfo, "modification_time");
        rb_define_method(fileinfo, "set_symlink_target", fileinfo_set_symlink_target, 1);
        G_DEF_SETTER(fileinfo, "symlink_target");
        rb_define_method(fileinfo, "set_sort_order", fileinfo_set_sort_order, 1);
        G_DEF_SETTER(fileinfo, "sort_order");
}
