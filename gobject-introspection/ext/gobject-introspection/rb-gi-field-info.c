/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2013  Ruby-GNOME2 Project Team
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

#include "rb-gi-private.h"

#define RG_TARGET_NAMESPACE rb_cGIFieldInfo
#define SELF(self) (RVAL2GI_FIELD_INFO(self))

GType
gi_field_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GIFieldInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_flags(VALUE self)
{
    GIFieldInfo *info;

    info = SELF(self);
    return GI_FIELD_INFO_FLAGS2RVAL(g_field_info_get_flags(info));
}

static VALUE
rg_size(VALUE self)
{
    GIFieldInfo *info;

    info = SELF(self);
    return INT2NUM(g_field_info_get_size(info));
}

static VALUE
rg_offset(VALUE self)
{
    GIFieldInfo *info;

    info = SELF(self);
    return INT2NUM(g_field_info_get_offset(info));
}

static VALUE
rg_type(VALUE self)
{
    GIFieldInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_field_info_get_type(info));
}

static VALUE
rb_gi_field_info_get_field_raw_interface(GIFieldInfo *info,
                                         gpointer memory,
                                         GITypeInfo *type_info)
{
    VALUE rb_field_value = Qnil;
    GIBaseInfo *interface_info;
    GIInfoType interface_type;
    GType gtype;
    gint offset;

    interface_info = g_type_info_get_interface(type_info);
    interface_type = g_base_info_get_type(interface_info);
    gtype = g_registered_type_info_get_g_type(interface_info);
    offset = g_field_info_get_offset(info);
    switch (interface_type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: GIField(interface)[%s](%s)",
                 g_info_type_to_string(interface_type),
                 g_type_name(gtype));
        break;
      case GI_INFO_TYPE_STRUCT:
        {
            GIStructInfo *struct_info = (GIStructInfo *)interface_info;
            gboolean is_pointer;
            gpointer target;

            is_pointer = g_type_info_is_pointer(type_info);
            target = (gpointer)((guint8 *)memory + offset);
            if (is_pointer) {
                target = *((gpointer *)target);
            }
            rb_field_value = rb_gi_struct_info_to_ruby(struct_info,
                                                       target,
                                                       is_pointer);
            break;
        }
      case GI_INFO_TYPE_BOXED:
      case GI_INFO_TYPE_UNION:
      case GI_INFO_TYPE_OBJECT:
        {
            GIArgument argument;

            argument.v_pointer = G_STRUCT_MEMBER(gpointer, memory, offset);
            rb_field_value = GI_ARGUMENT2RVAL(&argument, FALSE, type_info,
                                              NULL, NULL, NULL);
        }
        break;
      case GI_INFO_TYPE_ENUM:
        {
            gint32 raw_value;

            raw_value = G_STRUCT_MEMBER(gint32, memory, offset);
            if (gtype == G_TYPE_NONE) {
                rb_field_value = INT2NUM(raw_value);
            } else {
                rb_field_value = GENUM2RVAL(raw_value, gtype);
            }
        }
        break;
      case GI_INFO_TYPE_FLAGS:
        {
            gint32 raw_value;

            raw_value = G_STRUCT_MEMBER(gint32, memory, offset);
            if (gtype == G_TYPE_NONE) {
                rb_field_value = INT2NUM(raw_value);
            } else {
                rb_field_value = GFLAGS2RVAL(raw_value, gtype);
            }
        }
        break;
      case GI_INFO_TYPE_INTERFACE:
      case GI_INFO_TYPE_CONSTANT:
      case GI_INFO_TYPE_INVALID_0:
      case GI_INFO_TYPE_VALUE:
      case GI_INFO_TYPE_SIGNAL:
      case GI_INFO_TYPE_VFUNC:
      case GI_INFO_TYPE_PROPERTY:
      case GI_INFO_TYPE_FIELD:
      case GI_INFO_TYPE_ARG:
      case GI_INFO_TYPE_TYPE:
      case GI_INFO_TYPE_UNRESOLVED:
        rb_raise(rb_eNotImpError,
                 "TODO: GIField(interface)[%s](%s)",
                 g_info_type_to_string(interface_type),
                 g_type_name(gtype));
        break;
      default:
        break;
    }
    g_base_info_unref(interface_info);

    return rb_field_value;
}

VALUE
rb_gi_field_info_get_field_raw(GIFieldInfo *info, gpointer memory)
{
    GIArgument argument;
    gboolean processed = FALSE;
    GITypeInfo *type_info;
    GITypeTag type_tag;
    VALUE rb_field_value = Qnil;

    type_info = g_field_info_get_type(info);
    type_tag = g_type_info_get_tag(type_info);

    switch (type_tag) {
      case GI_TYPE_TAG_VOID:
      case GI_TYPE_TAG_BOOLEAN:
      case GI_TYPE_TAG_INT8:
      case GI_TYPE_TAG_UINT8:
      case GI_TYPE_TAG_INT16:
      case GI_TYPE_TAG_UINT16:
      case GI_TYPE_TAG_INT32:
      case GI_TYPE_TAG_UINT32:
      case GI_TYPE_TAG_INT64:
      case GI_TYPE_TAG_UINT64:
      case GI_TYPE_TAG_FLOAT:
      case GI_TYPE_TAG_DOUBLE:
      case GI_TYPE_TAG_GTYPE:
        break;
      case GI_TYPE_TAG_UTF8:
        {
            int offset;
            offset = g_field_info_get_offset(info);
            argument.v_string = G_STRUCT_MEMBER(gchar *, memory, offset);
            processed = TRUE;
        }
        break;
      case GI_TYPE_TAG_FILENAME:
      case GI_TYPE_TAG_ARRAY:
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_field_value =
            rb_gi_field_info_get_field_raw_interface(info,
                                                     memory,
                                                     type_info);
        if (!NIL_P(rb_field_value)) {
            g_base_info_unref(type_info);
            return rb_field_value;
        }
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        break;
      default:
        break;
    }

    if (!processed) {
        if (!g_field_info_get_field(info, memory, &argument)) {
            g_base_info_unref(type_info);
            rb_raise(rb_eArgError, "failed to get field value: %s[%s]",
                     g_base_info_get_name(info),
                     g_type_tag_to_string(type_tag));
        }
    }

    rb_field_value = GI_ARGUMENT2RVAL(&argument, FALSE, type_info,
                                      NULL, NULL, NULL);
    g_base_info_unref(type_info);

    return rb_field_value;
}

void
rb_gi_field_info_set_field_raw(GIFieldInfo *info, gpointer memory,
                               VALUE rb_field_value)
{
    gint offset;
    GITypeInfo *type_info;
    GITypeTag type_tag;
    gboolean succeeded = TRUE;

    offset = g_field_info_get_offset(info);
    type_info = g_field_info_get_type(info);
    type_tag = g_type_info_get_tag(type_info);

    if ((g_field_info_get_flags(info) & GI_FIELD_IS_WRITABLE) == 0) {
        g_base_info_unref(type_info);
        rb_raise(rb_eArgError,
                 "failed to set field value: not writable: %s[%s]",
                 g_base_info_get_name(info),
                 g_type_tag_to_string(type_tag));
    }

    /* TODO: Use g_field_info_set_field() again? */
    switch (type_tag) {
      case GI_TYPE_TAG_VOID:
        succeeded = FALSE;
        break;
      case GI_TYPE_TAG_BOOLEAN:
        G_STRUCT_MEMBER(gboolean, memory, offset) = RVAL2CBOOL(rb_field_value);
        break;
      case GI_TYPE_TAG_INT8:
        G_STRUCT_MEMBER(gint8, memory, offset) = NUM2CHR(rb_field_value);
        break;
      case GI_TYPE_TAG_UINT8:
        G_STRUCT_MEMBER(guint8, memory, offset) =
            (guint8)NUM2CHR(rb_field_value);
        break;
      case GI_TYPE_TAG_INT16:
        G_STRUCT_MEMBER(gint16, memory, offset) = NUM2SHORT(rb_field_value);
        break;
      case GI_TYPE_TAG_UINT16:
        G_STRUCT_MEMBER(guint16, memory, offset) = NUM2USHORT(rb_field_value);
        break;
      case GI_TYPE_TAG_INT32:
        G_STRUCT_MEMBER(gint32, memory, offset) = NUM2INT(rb_field_value);
        break;
      case GI_TYPE_TAG_UINT32:
        G_STRUCT_MEMBER(guint32, memory, offset) = NUM2UINT(rb_field_value);
        break;
      case GI_TYPE_TAG_INT64:
        G_STRUCT_MEMBER(gint64, memory, offset) = NUM2LL(rb_field_value);
        break;
      case GI_TYPE_TAG_UINT64:
        G_STRUCT_MEMBER(guint64, memory, offset) = NUM2ULL(rb_field_value);
        break;
      case GI_TYPE_TAG_FLOAT:
        G_STRUCT_MEMBER(gfloat, memory, offset) = NUM2DBL(rb_field_value);
        break;
      case GI_TYPE_TAG_DOUBLE:
        G_STRUCT_MEMBER(gdouble, memory, offset) = NUM2DBL(rb_field_value);
        break;
      case GI_TYPE_TAG_GTYPE:
        G_STRUCT_MEMBER(GType, memory, offset) =
            rbgobj_gtype_from_ruby(rb_field_value);
        break;
      case GI_TYPE_TAG_UTF8:
        G_STRUCT_MEMBER(const gchar *, memory, offset) =
            RVAL2CSTR_ACCEPT_SYMBOL(rb_field_value);
        break;
      case GI_TYPE_TAG_FILENAME:
        /* TODO: How to free? */
        /* G_STRUCT_MEMBER(gchar *, memory, offset) = */
        /*     rbg_filename_from_ruby(rb_field_value); */
        succeeded = FALSE;
        break;
      case GI_TYPE_TAG_ARRAY:
        succeeded = FALSE;
        break;
      case GI_TYPE_TAG_INTERFACE:
        {
            GIBaseInfo *interface_info;
            GIInfoType interface_type;

            interface_info = g_type_info_get_interface(type_info);
            interface_type = g_base_info_get_type(interface_info);
            switch (interface_type) {
              case GI_INFO_TYPE_INVALID:
              case GI_INFO_TYPE_FUNCTION:
              case GI_INFO_TYPE_CALLBACK:
                succeeded = FALSE;
                break;
              case GI_INFO_TYPE_STRUCT:
                {
                    GType gtype =
                        g_registered_type_info_get_g_type(interface_info);
                    G_STRUCT_MEMBER(gpointer, memory, offset) =
                        rb_gi_struct_get_raw(rb_field_value, gtype);
                }
                break;
              case GI_INFO_TYPE_BOXED:
              case GI_INFO_TYPE_UNION:
                {
                    GType gtype =
                        g_registered_type_info_get_g_type(interface_info);
                    if (gtype == G_TYPE_NONE) {
                        succeeded = FALSE;
                    } else {
                        G_STRUCT_MEMBER(gpointer, memory, offset) =
                            RVAL2BOXED(rb_field_value, gtype);
                    }
                }
                break;
              case GI_INFO_TYPE_ENUM:
              case GI_INFO_TYPE_FLAGS:
                {
                    GType gtype =
                        g_registered_type_info_get_g_type(interface_info);
                    GITypeTag storage_type =
                        g_enum_info_get_storage_type(interface_info);
                    gint value;
                    if (gtype == G_TYPE_NONE) {
                        value = NUM2INT(rb_field_value);
                    } else {
                        if (interface_type == GI_INFO_TYPE_ENUM) {
                            value = RVAL2GENUM(rb_field_value, gtype);
                        } else {
                            value = RVAL2GFLAGS(rb_field_value, gtype);
                        }
                    }
                    switch (storage_type) {
                      case GI_TYPE_TAG_INT8:
                      case GI_TYPE_TAG_UINT8:
                        G_STRUCT_MEMBER(guint8, memory, offset) =
                            (guint8)value;
                        break;
                      case GI_TYPE_TAG_INT16:
                      case GI_TYPE_TAG_UINT16:
                        G_STRUCT_MEMBER(guint16, memory, offset) =
                            (guint16)value;
                        break;
                      case GI_TYPE_TAG_INT32:
                      case GI_TYPE_TAG_UINT32:
                        G_STRUCT_MEMBER(guint32, memory, offset) =
                            (guint32)value;
                        break;
                      case GI_TYPE_TAG_INT64:
                      case GI_TYPE_TAG_UINT64:
                        G_STRUCT_MEMBER(guint64, memory, offset) =
                            (guint64)value;
                        break;
                      default:
                        succeeded = FALSE;
                        break;
                    }
                }
                break;
              case GI_INFO_TYPE_OBJECT:
                G_STRUCT_MEMBER(gpointer, memory, offset) =
                    RVAL2GOBJ(rb_field_value);
                break;
              case GI_INFO_TYPE_INTERFACE:
              case GI_INFO_TYPE_CONSTANT:
              case GI_INFO_TYPE_INVALID_0:
              case GI_INFO_TYPE_VALUE:
              case GI_INFO_TYPE_SIGNAL:
              case GI_INFO_TYPE_VFUNC:
              case GI_INFO_TYPE_PROPERTY:
              case GI_INFO_TYPE_FIELD:
              case GI_INFO_TYPE_ARG:
              case GI_INFO_TYPE_TYPE:
              case GI_INFO_TYPE_UNRESOLVED:
                succeeded = FALSE;
                break;
              default:
                break;
            }
            g_base_info_unref(interface_info);
        }
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        succeeded = FALSE;
        break;
      default:
        break;
    }
    g_base_info_unref(type_info);

    if (!succeeded) {
        rb_raise(rb_eArgError,
                 "failed to set field value: %s[%s]",
                 g_base_info_get_name(info),
                 g_type_tag_to_string(type_tag));
    }
}

static VALUE
rg_get_field(VALUE self, VALUE rb_memory)
{
    GIFieldInfo *info;
    gpointer memory;

    info = SELF(self);
    memory = GUINT_TO_POINTER(NUM2ULONG(rb_memory));
    return rb_gi_field_info_get_field_raw(info, memory);
}

static VALUE
rg_set_field(VALUE self, VALUE rb_memory, VALUE rb_field_value)
{
    GIFieldInfo *info;
    gpointer memory;

    info = SELF(self);
    memory = GUINT_TO_POINTER(NUM2ULONG(rb_memory));
    rb_gi_field_info_set_field_raw(info, memory, rb_field_value);

    return Qnil;
}

void
rb_gi_field_info_init(VALUE rb_mGI, VALUE rb_cGIBaseInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_FIELD_INFO, "FieldInfo", rb_mGI,
				rb_cGIBaseInfo);

    RG_DEF_METHOD(flags, 0);
    RG_DEF_METHOD(size, 0);
    RG_DEF_METHOD(offset, 0);
    RG_DEF_METHOD(type, 0);
    RG_DEF_METHOD(get_field, 1);
    RG_DEF_METHOD(set_field, 2);

    G_DEF_CLASS(G_TYPE_I_FIELD_INFO_FLAGS, "FieldInfoFlags", rb_mGI);
}
