/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_enums.c -

  $Author: geoff_youngs $
  $Date: 2003/07/28 08:51:24 $
  created at: Sat Jul 27 16:56:01 JST 2002

  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include "global.h"

typedef struct {
	char *original;
	char *replacement;
} constant_map;

static GSList *rbgobj_cmap = NULL;

static gint
rbgobj_constant_find(constant_map *a, char *name)
{
      return strcmp(a->original, name);
}

void 
rbgobj_constant_remap(const char *original, 
			const char *replacement)
{
      constant_map *map = g_new(constant_map,1);
      
      map -> original     = g_strdup(original);
      map -> replacement  = g_strdup(replacement);
      
      rbgobj_cmap = g_slist_append(rbgobj_cmap, map);
}

static char *
rbgobj_constant_lookup(const char *name)
{
      GSList *p = rbgobj_cmap;
      
      p = g_slist_find_custom(rbgobj_cmap, name, 
      			(GCompareFunc)rbgobj_constant_find);
      if (p)
      {
          char *replacement;
	  constant_map *map;
	  
	  map         = (constant_map*) p -> data;
	  rbgobj_cmap = g_slist_delete_link(rbgobj_cmap, p);
	  replacement = map -> replacement;
	  
	  g_free(map -> original);
	  g_free(map);
	  
	  return replacement;
      }
      return NULL;
}

static void 
rbgobj_define_const(VALUE mod, const char *name,
			VALUE value)
{
     if (name[0] >= 'A' && name[0] <= 'Z')
     {
         rb_define_const(mod, name, value);
     }
     else
     {
         char *new_name = rbgobj_constant_lookup(name);
	 
	 if (new_name)
	 {
	     rb_define_const(mod, new_name, value);
	     g_free(new_name);
	 }
	 else
	 {
	     rb_warn("Invalid constant name '%s' - skipped", name);
	 }
     }
}

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
            rbgobj_define_const(mod, name, INT2NUM(value->value));
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
            rbgobj_define_const(mod, name, UINT2NUM(value->value));
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
