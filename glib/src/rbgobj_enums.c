/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_enums.c -

  $Author: sakai $
  $Date: 2003/07/20 05:04:26 $
  created at: Sat Jul 27 16:56:01 JST 2002

  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include "global.h"

static void
rbgobj_enum_add_constants(VALUE mod, GType enum_type,
                          const gchar *strip_prefix)
{
    GEnumClass *gclass;
    int i;
    int prefix_len = strlen(strip_prefix);

    gclass = G_ENUM_CLASS(g_type_class_ref(enum_type));

    for (i = 0; i < gclass->n_values; i++) {
        const GEnumValue* value = &gclass->values[i];

        if (strncmp(value->value_name, strip_prefix, prefix_len)) {
            g_warning("\%s\" doesn't have prefix \"%s\"",
                      value->value_name, strip_prefix);
        } else {
            const char* name = value->value_name + prefix_len;
            rb_define_const(mod, name, INT2NUM(value->value));
        }
    }

    g_type_class_unref(gclass);
}

static void
rbgobj_flags_add_constants(VALUE mod, GType flags_type,
                           const gchar *strip_prefix)
{
    GFlagsClass *gclass;
    int i;
    int prefix_len = strlen(strip_prefix);

    gclass = G_FLAGS_CLASS(g_type_class_ref(flags_type));

    for (i = 0; i < gclass->n_values; i++) {
        const GFlagsValue* value = &gclass->values[i];

        if (strncmp(value->value_name, strip_prefix, prefix_len)) {
            g_warning("\%s\" doesn't have prefix \"%s\"",
                      value->value_name, strip_prefix);
        } else {
            const char* name = value->value_name + prefix_len;
            rb_define_const(mod, name, INT2NUM(value->value));
        }
    }

    g_type_class_unref(gclass);
}

void
rbgobj_add_constants(VALUE mod, GType type, const gchar *strip_prefix)
{
    if (G_TYPE_IS_ENUM(type)) {
        rbgobj_enum_add_constants(mod, type, strip_prefix);
    } else if (G_TYPE_IS_FLAGS(type)) {
        rbgobj_flags_add_constants(mod, type, strip_prefix);
    } else {
        g_warning("`%s' is not an enum/flags type", g_type_name(type));
    }
}

void
Init_gobject_genums()
{
} 
