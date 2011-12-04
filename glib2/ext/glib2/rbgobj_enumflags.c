/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004-2006  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003  Masahiro Sakai
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

#include "rbgprivate.h"
#include <ctype.h>

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
rbgobj_constant_remap(const char *original, const char *replacement)
{
    constant_map *map = g_new(constant_map,1);

    map->original     = g_strdup(original);
    map->replacement  = g_strdup(replacement);

    rbgobj_cmap = g_slist_append(rbgobj_cmap, map);
}

char *
rg_obj_constant_lookup(const char *name)
{
    GSList *p = rbgobj_cmap;

    p = g_slist_find_custom(rbgobj_cmap, name,
                            (GCompareFunc)rbgobj_constant_find);
    if (p) {
        char *replacement;
        constant_map *map;

        map         = (constant_map *)p->data;
        rbgobj_cmap = g_slist_delete_link(rbgobj_cmap, p);
        replacement = map->replacement;

        g_free(map->original);
        g_free(map);

        return replacement;
    }

    return NULL;
}

void
rbgobj_define_const(VALUE mod, const char *name,
			VALUE value)
{
    if (name[0] >= 'A' && name[0] <= 'Z') {
        rb_define_const(mod, name, value);
    } else {
        char *new_name = rg_obj_constant_lookup(name);

        if (new_name) {
            rb_define_const(mod, new_name, value);
            g_free(new_name);
        } else {
            rb_warn("Invalid constant name '%s' - skipped", name);
        }
    }
}

void
rbgobj_add_constants(VALUE mod, GType type, const gchar *strip_prefix)
{
    if (G_TYPE_IS_ENUM(type)) {
        rg_enum_add_constants(mod, type, strip_prefix);
    } else if (G_TYPE_IS_FLAGS(type)) {
        rg_flags_add_constants(mod, type, strip_prefix);
    } else {
        g_warning("`%s' is not an enum/flags type", g_type_name(type));
    }
}

void
Init_gobject_genumflags(void)
{
    Init_gobject_genums();
    Init_gobject_gflags();
}
