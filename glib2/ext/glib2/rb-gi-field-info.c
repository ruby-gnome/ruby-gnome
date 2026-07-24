/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2022  Ruby-GNOME Project Team
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

#include "rb-gi-conversions.h"
#include "rb-gi-private.h"
#include "rb-gi-private-arguments.h"

#define RG_TARGET_NAMESPACE rb_cGIFieldInfo
#define SELF(self) (RVAL2GI_FIELD_INFO(self))

static VALUE
rg_flags(VALUE self)
{
    GIFieldInfo *info;

    info = SELF(self);
    return GI_FIELD_INFO_FLAGS2RVAL(gi_field_info_get_flags(info));
}

static VALUE
rg_size(VALUE self)
{
    GIFieldInfo *info;

    info = SELF(self);
    return INT2NUM(gi_field_info_get_size(info));
}

static VALUE
rg_offset(VALUE self)
{
    GIFieldInfo *info;

    info = SELF(self);
    return INT2NUM(gi_field_info_get_offset(info));
}

static VALUE
rg_type(VALUE self)
{
    GIFieldInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL_WITH_UNREF(GI_BASE_INFO(info));
}

typedef struct {
    RBGIArguments args;
    GIArgument value;
    RBGIArgMetadata metadata;
    GIFieldInfo *info;
    gpointer memory;
} FieldToRubyData;

static VALUE
rb_gi_field_info_get_field_raw_body_interface(FieldToRubyData *data)
{
    GIBaseInfo* interface_info = data->metadata.type.interface_info;
    GType gtype = data->metadata.type.interface_gtype;
    gint offset = gi_field_info_get_offset(data->info);
    if (GI_IS_FUNCTION_INFO(interface_info) || GI_IS_CALLBACK_INFO(interface_info)) {
        rb_raise(rb_eNotImpError,
                 "TODO: GIField(interface)[%s](%s)",
                 gi_base_info_get_name(interface_info),
                 g_type_name(gtype));
        return Qnil;
    }

    if (GI_IS_STRUCT_INFO(interface_info)) {
      GIStructInfo *struct_info =
          (GIStructInfo *)(data->metadata.type.interface_info);
      gboolean is_pointer =
          gi_type_info_is_pointer(data->metadata.type.info);
      gpointer target = (gpointer)((guint8 *)(data->memory) + offset);
      if (is_pointer) {
          target = *((gpointer *)target);
      }
      return rb_gi_struct_info_to_ruby(struct_info, target, is_pointer);
    }

    if (G_TYPE_IS_BOXED(gtype) || GI_IS_UNION_INFO(interface_info) || GI_IS_OBJECT_INFO(interface_info)) {
      data->value.v_pointer = G_STRUCT_MEMBER(gpointer, data->memory, offset);
      return rb_gi_arguments_convert_arg(&(data->args),
                                         &(data->value),
                                         &(data->metadata),
                                         FALSE);
    }

    if (GI_IS_ENUM_INFO(interface_info)) {
      gint32 raw_value;

      raw_value = G_STRUCT_MEMBER(gint32, data->memory, offset);
      if (gtype == G_TYPE_NONE) {
          return INT2NUM(raw_value);
      } else {
          return GENUM2RVAL(raw_value, gtype);
      }
    }

    if (GI_IS_TYPE_INFO(interface_info)) {
      gint32 raw_value;

      raw_value = G_STRUCT_MEMBER(gint32, data->memory, offset);
      if (gtype == G_TYPE_NONE) {
          return INT2NUM(raw_value);
      } else {
          return GFLAGS2RVAL(raw_value, gtype);
      }
    }

    rb_raise(rb_eNotImpError,
             "TODO: GIField(interface)[%s](%s)",
             gi_base_info_get_name(interface_info),
             g_type_name(gtype));
    return Qnil;
}

static VALUE
rb_gi_field_info_get_field_raw_body(VALUE user_data)
{
    FieldToRubyData *data = (FieldToRubyData *)user_data;
    gboolean processed = FALSE;
    switch (data->metadata.type.tag) {
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
            offset = gi_field_info_get_offset(data->info);
            data->value.v_string =
                G_STRUCT_MEMBER(gchar *, data->memory, offset);
            processed = TRUE;
        }
        break;
      case GI_TYPE_TAG_FILENAME:
      case GI_TYPE_TAG_ARRAY:
        break;
      case GI_TYPE_TAG_INTERFACE:
        {
            VALUE rb_value = rb_gi_field_info_get_field_raw_body_interface(data);
            if (!NIL_P(rb_value)) {
                return rb_value;
            }
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
        if (!gi_field_info_get_field(data->info, data->memory, &(data->value))) {
            rb_raise(rb_eArgError, "failed to get field value: %s[%s]",
                     gi_base_info_get_name(GI_BASE_INFO(data->info)),
                     gi_type_tag_to_string(data->metadata.type.tag));
        }
    }

    return rb_gi_arguments_convert_arg(&(data->args),
                                       &(data->value),
                                       &(data->metadata),
                                       FALSE);
}

static VALUE
rb_gi_field_info_get_field_raw_ensure(VALUE user_data)
{
    FieldToRubyData *data = (FieldToRubyData *)user_data;
    rb_gi_arguments_clear(&(data->args));
    rb_gi_arg_metadata_clear(&(data->metadata));
    return Qnil;
}

VALUE
rb_gi_field_info_get_field_raw(GIFieldInfo *info,
                               GIStructInfo *struct_info,
                               gpointer memory)
{
    FieldToRubyData data;
    rb_gi_arguments_init(&(data.args), NULL, Qnil, Qnil, NULL);
    GITypeInfo *type_info = gi_field_info_get_type_info(info);
    rb_gi_arg_metadata_init_type_info(&(data.metadata), type_info);
    rb_gi_arg_metadata_init_struct_info(&(data.metadata), struct_info, memory);
    data.info = info;
    data.memory = memory;

    return rb_ensure(rb_gi_field_info_get_field_raw_body, (VALUE)&data,
                     rb_gi_field_info_get_field_raw_ensure, (VALUE)&data);
}

void
rb_gi_field_info_set_field_raw(GIFieldInfo *info,
                               /* TODO: Use this to set length field. */
                               GIStructInfo *struct_info,
                               gpointer memory,
                               VALUE rb_field_value)
{
    gint offset;
    GITypeInfo *type_info;
    GITypeTag type_tag;
    gboolean succeeded = TRUE;

    offset = gi_field_info_get_offset(info);
    type_info = gi_field_info_get_type_info(info);
    type_tag = gi_type_info_get_tag(type_info);

    if ((gi_field_info_get_flags(info) & GI_FIELD_IS_WRITABLE) == 0) {
        gi_base_info_unref(type_info);
        rb_raise(rb_eArgError,
                 "failed to set field value: not writable: %s[%s]",
                 gi_base_info_get_name(GI_BASE_INFO(info)),
                 gi_type_tag_to_string(type_tag));
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

            interface_info = gi_type_info_get_interface(type_info);
            GType gtype =
                gi_registered_type_info_get_g_type(GI_REGISTERED_TYPE_INFO(interface_info));

            if (GI_IS_FUNCTION_INFO(interface_info) || GI_IS_CALLBACK_INFO(interface_info)) {
                succeeded = FALSE;
            } else if (GI_IS_STRUCT_INFO(interface_info)) {
              G_STRUCT_MEMBER(gpointer, memory, offset) =
                  rb_gi_struct_get_raw(rb_field_value, gtype);
            } else if (GI_IS_UNION_INFO(info) || G_TYPE_IS_BOXED(gtype)) {
              if (gtype == G_TYPE_NONE) {
                  succeeded = FALSE;
              } else {
                  G_STRUCT_MEMBER(gpointer, memory, offset) =
                      RVAL2BOXED(rb_field_value, gtype);
              }
            } else if (GI_IS_ENUM_INFO(interface_info) || GI_IS_FLAGS_INFO(interface_info)) {
              GITypeTag storage_type =
                  gi_enum_info_get_storage_type(GI_ENUM_INFO(interface_info));
              gint value;
              if (gtype == G_TYPE_NONE) {
                  value = NUM2INT(rb_field_value);
              } else {
                  if (GI_IS_ENUM_INFO(interface_info)) {
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
            } else if (GI_IS_OBJECT_INFO(interface_info)) {
                G_STRUCT_MEMBER(gpointer, memory, offset) =
                    RVAL2GOBJ(rb_field_value);
            } else {
                succeeded = FALSE;
            }
            gi_base_info_unref(interface_info);
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
    gi_base_info_unref(type_info);

    if (!succeeded) {
        rb_raise(rb_eArgError,
                 "failed to set field value: %s[%s]",
                 gi_base_info_get_name(GI_BASE_INFO(info)),
                 gi_type_tag_to_string(type_tag));
    }
}

static VALUE
rg_get_field(VALUE self, VALUE rb_struct_info, VALUE rb_memory)
{
    GIFieldInfo *info = SELF(self);
    GIStructInfo *struct_info = RVAL2GI_STRUCT_INFO(rb_struct_info);
    gpointer memory = GUINT_TO_POINTER(NUM2ULONG(rb_memory));
    return rb_gi_field_info_get_field_raw(info, struct_info, memory);
}

static VALUE
rg_set_field(VALUE self,
             VALUE rb_struct_info,
             VALUE rb_memory,
             VALUE rb_field_value)
{
    GIFieldInfo *info = SELF(self);
    GIStructInfo *struct_info = RVAL2GI_STRUCT_INFO(rb_struct_info);
    gpointer memory = GUINT_TO_POINTER(NUM2ULONG(rb_memory));
    rb_gi_field_info_set_field_raw(info, struct_info, memory, rb_field_value);
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
    RG_DEF_METHOD(get_field, 2);
    RG_DEF_METHOD(set_field, 3);

    G_DEF_CLASS(G_TYPE_I_FIELD_INFO_FLAGS, "FieldInfoFlags", rb_mGI);
}
