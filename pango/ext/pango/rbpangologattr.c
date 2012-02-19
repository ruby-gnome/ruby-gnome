/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Masao Mutoh
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

#include "rbpangoprivate.h"

#define RG_TARGET_NAMESPACE cLogAttr
#define _SELF(s) (RVAL2PANGOLOGATTR(s))

/**********************************/
static PangoLogAttr*
log_copy(const PangoLogAttr* val)
{
    PangoLogAttr* new_val;
    g_return_val_if_fail (val != NULL, NULL);
    new_val = g_new(PangoLogAttr, 1);
    *new_val = *val;
    return new_val;
}

GType
pango_log_attr_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("PangoLogAttr",
                    (GBoxedCopyFunc)log_copy,
                    (GBoxedFreeFunc)g_free);
    return our_type;
}
/**********************************/

#define ATTR_BOOL(name)\
static VALUE \
log_get_ ## name (VALUE self)\
{\
    return CBOOL2RVAL(_SELF(self)->name);\
}\
static VALUE \
log_set_ ## name (VALUE self, VALUE val)\
{\
    _SELF(self)->name = RVAL2CBOOL(val);\
    return self;\
}

static VALUE
rg_initialize(VALUE self)
{
    PangoLogAttr log;
    G_INITIALIZE(self, &log);
    return Qnil;
}

#define DEFINE_ACCESSOR(logattr, name) \
    rbg_define_method(logattr, G_STRINGIFY(name?), log_get_is_ ## name, 0);\
    rbg_define_method(logattr, G_STRINGIFY(set_ ## name), log_set_is_ ## name, 1);

ATTR_BOOL(is_line_break);
ATTR_BOOL(is_mandatory_break);
ATTR_BOOL(is_char_break);
ATTR_BOOL(is_white);
ATTR_BOOL(is_cursor_position);
ATTR_BOOL(is_word_start);
ATTR_BOOL(is_word_end);
ATTR_BOOL(is_sentence_boundary);
ATTR_BOOL(is_sentence_start);
ATTR_BOOL(is_sentence_end);

#if PANGO_CHECK_VERSION(1,4,0)
ATTR_BOOL(backspace_deletes_character);
#endif

void
Init_pango_logattr(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_LOG_ATTR, "LogAttr", mPango);

    RG_DEF_METHOD(initialize, 0);

    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, line_break);
    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, mandatory_break);
    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, char_break);
    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, white);
    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, cursor_position);
    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, word_start);
    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, word_end);
    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, sentence_boundary);
    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, sentence_start);
    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, sentence_end);

#if PANGO_CHECK_VERSION(1,4,0)
    rbg_define_method(RG_TARGET_NAMESPACE, "backspace_deletes_character?", log_get_backspace_deletes_character, 0); 
    rbg_define_method(RG_TARGET_NAMESPACE, "set_backspace_deletes_character", log_set_backspace_deletes_character, 1); 
#endif
}
