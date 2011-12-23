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

#define RG_TARGET_NAMESPACE cFileInfo
#define _SELF(value) RVAL2GFILEINFO(value)

#define RVAL2FILEINFONAMESPACEDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), RVAL2CSTR, "*")

static VALUE
rg_initialize(VALUE self)
{
        G_INITIALIZE(self, g_file_info_new());

        return Qnil;
}

static VALUE
rg_dup(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_file_info_dup(_SELF(self)));
}

static VALUE
rg_copy_into(VALUE self, VALUE other)
{
        g_file_info_copy_into(_SELF(self), _SELF(other));

        return self;
}

static VALUE
rg_has_attribute_p(VALUE self, VALUE attribute)
{
        return CBOOL2RVAL(g_file_info_has_attribute(_SELF(self), RVAL2CSTR(attribute)));
}

static VALUE
rg_has_namespace_p(VALUE self, VALUE namespace)
{
        return CBOOL2RVAL(g_file_info_has_namespace(_SELF(self), RVAL2CSTR(namespace)));
}

static VALUE
rg_set_attribute_status(VALUE self, VALUE attribute, VALUE status)
{
        return CBOOL2RVAL(g_file_info_set_attribute_status(_SELF(self),
                                                           RVAL2CSTR(attribute),
                                                           RVAL2GFILEATTRIBUTESTATUS(status)));
}

static VALUE
rg_list_attributes(int argc, VALUE *argv, VALUE self)
{
        VALUE name_space;

        rb_scan_args(argc, argv, "01", &name_space);

        return STRV2RVAL_FREE(g_file_info_list_attributes(_SELF(self),
                                                          RVAL2FILEINFONAMESPACEDEFAULT(name_space)));
}

static VALUE
rg_get_attribute_type(VALUE self, VALUE attribute)
{
        return GFILEATTRIBUTETYPE2RVAL(g_file_info_get_attribute_type(_SELF(self), RVAL2CSTR(attribute)));
}

static VALUE
rg_remove_attribute(VALUE self, VALUE attribute)
{
        g_file_info_remove_attribute(_SELF(self), RVAL2CSTR(attribute));

        return self;
}

static VALUE
rg_get_attribute_as_string(VALUE self, VALUE attribute)
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
                return STRV2RVAL_FREE((gchar **)value);
        case G_FILE_ATTRIBUTE_TYPE_INVALID:
                rb_raise(rb_eArgError, "invalid file attribute type");
                break;
        default:
                rb_raise(rb_eArgError, "unknown file attribute type");
                break;
        }
}

static VALUE
rg_get_attribute_data(VALUE self, VALUE attribute)
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
rg_get_attribute_status(VALUE self, VALUE attribute)
{
        return GFILEATTRIBUTESTATUS2RVAL(g_file_info_get_attribute_status(_SELF(self),
                                                                          RVAL2CSTR(attribute)));
}

static VALUE
rg_get_attribute_string(VALUE self, VALUE attribute)
{
        return CSTR2RVAL(g_file_info_get_attribute_string(_SELF(self),
                                                          RVAL2CSTR(attribute)));
}

static VALUE
rg_get_attribute_stringv(VALUE self, VALUE attribute)
{
        return STRV2RVAL((const gchar **)g_file_info_get_attribute_stringv(_SELF(self),
                                                                           RVAL2CSTR(attribute)));
}

static VALUE
rg_get_attribute_byte_string(VALUE self, VALUE attribute)
{
        return CSTR2RVAL(g_file_info_get_attribute_byte_string(_SELF(self),
                                                               RVAL2CSTR(attribute)));
}

static VALUE
rg_get_attribute_boolean(VALUE self, VALUE attribute)
{
        return CBOOL2RVAL(g_file_info_get_attribute_boolean(_SELF(self),
                                                            RVAL2CSTR(attribute)));
}

static VALUE
rg_get_attribute_uint32(VALUE self, VALUE attribute)
{
        return GUINT322RVAL(g_file_info_get_attribute_uint32(_SELF(self),
                                                             RVAL2CSTR(attribute)));
}

static VALUE
rg_get_attribute_int32(VALUE self, VALUE attribute)
{
        return GINT322RVAL(g_file_info_get_attribute_int32(_SELF(self),
                                                           RVAL2CSTR(attribute)));
}

static VALUE
rg_get_attribute_uint64(VALUE self, VALUE attribute)
{
        return GUINT642RVAL(g_file_info_get_attribute_uint64(_SELF(self),
                                                             RVAL2CSTR(attribute)));
}

static VALUE
rg_get_attribute_int64(VALUE self, VALUE attribute)
{
        return GINT642RVAL(g_file_info_get_attribute_int64(_SELF(self),
                                                           RVAL2CSTR(attribute)));
}

static VALUE
rg_get_attribute_object(VALUE self, VALUE attribute)
{
        return GOBJ2RVAL(g_file_info_get_attribute_object(_SELF(self),
                                                          RVAL2CSTR(attribute)));
}

static VALUE
rg_get_attribute_value(VALUE self, VALUE attribute)
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

static VALUE
rg_set_attribute_string(VALUE self, VALUE attribute, VALUE value)
{
        g_file_info_set_attribute_string(_SELF(self),
                                         RVAL2CSTR(attribute),
                                         RVAL2CSTR(value));

        return self;
}

static VALUE
rg_set_attribute_byte_string(VALUE self, VALUE attribute, VALUE value)
{
        g_file_info_set_attribute_byte_string(_SELF(self),
                                              RVAL2CSTR(attribute),
                                              RVAL2CSTR(value));

        return self;
}

static VALUE
rg_set_attribute_boolean(VALUE self, VALUE attribute, VALUE value)
{
        g_file_info_set_attribute_boolean(_SELF(self),
                                          RVAL2CSTR(attribute),
                                          RVAL2CBOOL(value));

        return self;
}

static VALUE
rg_set_attribute_uint32(VALUE self, VALUE attribute, VALUE value)
{
        g_file_info_set_attribute_uint32(_SELF(self),
                                         RVAL2CSTR(attribute),
                                         RVAL2GUINT32(value));

        return self;
}

static VALUE
rg_set_attribute_int32(VALUE self, VALUE attribute, VALUE value)
{
        g_file_info_set_attribute_int32(_SELF(self),
                                        RVAL2CSTR(attribute),
                                        RVAL2GINT32(value));

        return self;
}

static VALUE
rg_set_attribute_uint64(VALUE self, VALUE attribute, VALUE value)
{
        g_file_info_set_attribute_uint64(_SELF(self),
                                         RVAL2CSTR(attribute),
                                         RVAL2GUINT64(value));

        return self;
}

static VALUE
rg_set_attribute_int64(VALUE self, VALUE attribute, VALUE value)
{
        g_file_info_set_attribute_int64(_SELF(self),
                                        RVAL2CSTR(attribute),
                                        RVAL2GINT64(value));

        return self;
}

static VALUE
rg_set_attribute_object(VALUE self, VALUE attribute, VALUE value)
{
        g_file_info_set_attribute_object(_SELF(self),
                                         RVAL2CSTR(attribute),
                                         RVAL2GOBJ(value));

        return self;
}

static VALUE
rg_clear_status(VALUE self)
{
        g_file_info_clear_status(_SELF(self));

        return self;
}

static VALUE
rg_file_type(VALUE self)
{
        return GFILETYPE2RVAL(g_file_info_get_file_type(_SELF(self)));
}

static VALUE
rg_hidden_p(VALUE self)
{
        return CBOOL2RVAL(g_file_info_get_is_hidden(_SELF(self)));
}

static VALUE
rg_backup_p(VALUE self)
{
        return CBOOL2RVAL(g_file_info_get_is_backup(_SELF(self)));
}

static VALUE
rg_symlink_p(VALUE self)
{
        return CBOOL2RVAL(g_file_info_get_is_symlink(_SELF(self)));
}

static VALUE
rg_name(VALUE self)
{
        return CSTR2RVAL(g_file_info_get_name(_SELF(self)));
}

static VALUE
rg_display_name(VALUE self)
{
        return CSTR2RVAL(g_file_info_get_display_name(_SELF(self)));
}

static VALUE
rg_edit_name(VALUE self)
{
        return CSTR2RVAL(g_file_info_get_edit_name(_SELF(self)));
}

static VALUE
rg_icon(VALUE self)
{
        return GOBJ2RVAL(g_file_info_get_icon(_SELF(self)));
}

static VALUE
rg_content_type(VALUE self)
{
        return CSTR2RVAL(g_file_info_get_content_type(_SELF(self)));
}

static VALUE
rg_size(VALUE self)
{
        return GOFFSET2RVAL(g_file_info_get_size(_SELF(self)));
}

static VALUE
rg_modification_time(VALUE self)
{
        GTimeVal time;

        g_file_info_get_modification_time(_SELF(self), &time);

        return GTIMEVAL2RVAL(&time);
}

static VALUE
rg_symlink_target(VALUE self)
{
        return CSTR2RVAL(g_file_info_get_symlink_target(_SELF(self)));
}

static VALUE
rg_etag(VALUE self)
{
        return CSTR2RVAL(g_file_info_get_etag(_SELF(self)));
}

static VALUE
rg_sort_order(VALUE self)
{
        return GINT322RVAL(g_file_info_get_sort_order(_SELF(self)));
}

static VALUE
rg_set_attribute_mask(VALUE self, VALUE rbmask)
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
rg_unset_attribute_mask(VALUE self)
{
        g_file_info_unset_attribute_mask(_SELF(self));

        return self;
}

static VALUE
rg_set_file_type(VALUE self, VALUE value)
{
        g_file_info_set_file_type(_SELF(self), RVAL2GFILETYPE(value));

        return self;
}

static VALUE
rg_set_hidden(VALUE self, VALUE value)
{
        g_file_info_set_is_hidden(_SELF(self), RVAL2CBOOL(value));

        return self;
}

static VALUE
rg_set_symlink(VALUE self, VALUE value)
{
        g_file_info_set_is_symlink(_SELF(self), RVAL2CBOOL(value));

        return self;
}

static VALUE
rg_set_name(VALUE self, VALUE value)
{
        g_file_info_set_name(_SELF(self), RVAL2CSTR(value));

        return self;
}

static VALUE
rg_set_display_name(VALUE self, VALUE value)
{
        g_file_info_set_display_name(_SELF(self), RVAL2CSTR(value));

        return self;
}

static VALUE
rg_set_edit_name(VALUE self, VALUE value)
{
        g_file_info_set_edit_name(_SELF(self), RVAL2CSTR(value));

        return self;
}

static VALUE
rg_set_icon(VALUE self, VALUE value)
{
        g_file_info_set_icon(_SELF(self), RVAL2GICON(value));

        return self;
}

static VALUE
rg_set_content_type(VALUE self, VALUE value)
{
        g_file_info_set_content_type(_SELF(self), RVAL2CSTR(value));

        return self;
}

static VALUE
rg_set_size(VALUE self, VALUE value)
{
        g_file_info_set_size(_SELF(self), RVAL2GOFFSET(value));

        return self;
}

static VALUE
rg_set_modification_time(VALUE self, VALUE rbtime)
{
        GTimeVal time;

        rbgio_rval_to_gtimeval(rbtime, &time);

        g_file_info_set_modification_time(_SELF(self), &time);

        return self;
}

static VALUE
rg_set_symlink_target(VALUE self, VALUE value)
{
        g_file_info_set_symlink_target(_SELF(self), RVAL2CSTR(value));

        return self;
}

static VALUE
rg_set_sort_order(VALUE self, VALUE value)
{
        g_file_info_set_sort_order(_SELF(self), RVAL2GINT32(value));

        return self;
}

void
Init_gfileinfo(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_FILE_INFO, "FileInfo", mGio);

        RG_DEF_METHOD(initialize, 0);
        RG_DEF_METHOD(dup, 0);
        RG_DEF_METHOD(copy_into, 1);
        RG_DEF_METHOD_P(has_attribute, 1);
        RG_DEF_METHOD_P(has_namespace, 1);
        RG_DEF_METHOD(list_attributes, -1);
        RG_DEF_ALIAS("attributes", "list_attributes");
        RG_DEF_METHOD(get_attribute_type, 1);
        RG_DEF_METHOD(remove_attribute, 1);
        RG_DEF_METHOD(get_attribute_as_string, 1);
        RG_DEF_METHOD(get_attribute_data, 1);
        RG_DEF_METHOD(get_attribute_status, 1);
        RG_DEF_METHOD(get_attribute_string, 1);
        RG_DEF_METHOD(get_attribute_stringv, 1);
        RG_DEF_METHOD(get_attribute_byte_string, 1);
        RG_DEF_METHOD(get_attribute_boolean, 1);
        RG_DEF_METHOD(get_attribute_uint32, 1);
        RG_DEF_METHOD(get_attribute_int32, 1);
        RG_DEF_METHOD(get_attribute_uint64, 1);
        RG_DEF_METHOD(get_attribute_int64, 1);
        RG_DEF_METHOD(get_attribute_object, 1);
        RG_DEF_METHOD(get_attribute_value, 1);
        RG_DEF_ALIAS("[]", "get_attribute_value");
        RG_DEF_METHOD(set_attribute_status, 2);
        RG_DEF_METHOD(set_attribute_string, 2);
        RG_DEF_METHOD(set_attribute_byte_string, 2);
        RG_DEF_METHOD(set_attribute_boolean, 2);
        RG_DEF_METHOD(set_attribute_uint32, 2);
        RG_DEF_METHOD(set_attribute_int32, 2);
        RG_DEF_METHOD(set_attribute_uint64, 2);
        RG_DEF_METHOD(set_attribute_int64, 2);
        RG_DEF_METHOD(set_attribute_object, 2);
        RG_DEF_METHOD(clear_status, 0);
        RG_DEF_METHOD(file_type, 0);
        RG_DEF_METHOD_P(hidden, 0);
        RG_DEF_METHOD_P(backup, 0);
        RG_DEF_METHOD_P(symlink, 0);
        RG_DEF_METHOD(name, 0);
        RG_DEF_METHOD(display_name, 0);
        RG_DEF_METHOD(edit_name, 0);
        RG_DEF_METHOD(icon, 0);
        RG_DEF_METHOD(content_type, 0);
        RG_DEF_METHOD(size, 0);
        RG_DEF_METHOD(modification_time, 0);
        RG_DEF_METHOD(symlink_target, 0);
        RG_DEF_METHOD(etag, 0);
        RG_DEF_METHOD(sort_order, 0);
        RG_DEF_METHOD(set_attribute_mask, 1);
        G_DEF_SETTER(RG_TARGET_NAMESPACE, "attribute_mask");
        RG_DEF_METHOD(unset_attribute_mask, 0);
        RG_DEF_METHOD(set_file_type, 1);
        G_DEF_SETTER(RG_TARGET_NAMESPACE, "file_type");
        RG_DEF_METHOD(set_hidden, 1);
        G_DEF_SETTER(RG_TARGET_NAMESPACE, "hidden");
        RG_DEF_METHOD(set_symlink, 1);
        G_DEF_SETTER(RG_TARGET_NAMESPACE, "symlink");
        RG_DEF_METHOD(set_name, 1);
        G_DEF_SETTER(RG_TARGET_NAMESPACE, "name");
        RG_DEF_METHOD(set_display_name, 1);
        G_DEF_SETTER(RG_TARGET_NAMESPACE, "display_name");
        RG_DEF_METHOD(set_edit_name, 1);
        G_DEF_SETTER(RG_TARGET_NAMESPACE, "edit_name");
        RG_DEF_METHOD(set_icon, 1);
        G_DEF_SETTER(RG_TARGET_NAMESPACE, "icon");
        RG_DEF_METHOD(set_content_type, 1);
        G_DEF_SETTER(RG_TARGET_NAMESPACE, "content_type");
        RG_DEF_METHOD(set_size, 1);
        G_DEF_SETTER(RG_TARGET_NAMESPACE, "size");
        RG_DEF_METHOD(set_modification_time, 1);
        G_DEF_SETTER(RG_TARGET_NAMESPACE, "modification_time");
        RG_DEF_METHOD(set_symlink_target, 1);
        G_DEF_SETTER(RG_TARGET_NAMESPACE, "symlink_target");
        RG_DEF_METHOD(set_sort_order, 1);
        G_DEF_SETTER(RG_TARGET_NAMESPACE, "sort_order");
}
