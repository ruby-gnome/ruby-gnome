/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangologattr.c -

  $Author: mutoh $
  $Date: 2005/02/13 17:31:33 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbpango.h"

#define _SELF(s) ((PangoLogAttr*)RVAL2BOXED(s, PANGO_TYPE_LOG_ATTR))

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
log_get_ ## name (self)\
    VALUE self;\
{\
    return CBOOL2RVAL(_SELF(self)->name);\
}\
static VALUE \
log_set_ ## name (self, val)\
    VALUE self, val;\
{\
    _SELF(self)->name = RTEST(val);\
    return self;\
}

static VALUE
log_initialize(self)
    VALUE self;
{
    PangoLogAttr log;
    G_INITIALIZE(self, &log);
    return Qnil;
}

#define DEFINE_ACCESSOR(logattr, name) \
    rb_define_method(logattr, G_STRINGIFY(name?), log_get_is_ ## name, 0);\
    rb_define_method(logattr, G_STRINGIFY(set_ ## name), log_set_is_ ## name, 1);

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
ATTR_BOOL(backspace_deletes_character);

void
Init_pango_logattr()
{
    VALUE logattr = G_DEF_CLASS(PANGO_TYPE_LOG_ATTR, "LogAttr", mPango);
    
    rb_define_method(logattr, "initialize", log_initialize, 0);

    DEFINE_ACCESSOR(logattr, line_break);
    DEFINE_ACCESSOR(logattr, mandatory_break);
    DEFINE_ACCESSOR(logattr, char_break);
    DEFINE_ACCESSOR(logattr, white);
    DEFINE_ACCESSOR(logattr, cursor_position);
    DEFINE_ACCESSOR(logattr, word_start);
    DEFINE_ACCESSOR(logattr, word_end);
    DEFINE_ACCESSOR(logattr, sentence_boundary);
    DEFINE_ACCESSOR(logattr, sentence_start);
    DEFINE_ACCESSOR(logattr, sentence_end);

    rb_define_method(logattr, "backspace_deletes_character?", log_get_backspace_deletes_character, 0); 
    rb_define_method(logattr, "set_backspace_deletes_character", log_set_backspace_deletes_character, 1); 

    G_DEF_SETTERS(logattr);
}
