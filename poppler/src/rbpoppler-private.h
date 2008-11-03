#ifndef __RBPOPPLER_PRIVATE_H__
#define __RBPOPPLER_PRIVATE_H__

#include "rbpoppler.h"

#ifdef POPPLER_WITH_GDK
#  include <rbgdk.h>
#endif

extern void Init_poppler_document(VALUE mPoppler);
extern void Init_poppler_page(VALUE mPoppler);
extern void Init_poppler_attachment(VALUE mPoppler);
extern void Init_poppler_action(VALUE mPoppler);
extern void Init_poppler_annotation(VALUE mPoppler);
extern void Init_poppler_form_field(VALUE mPoppler);

#define DEF_READER(prefix, name, member, self_to_c, member_to_rb)       \
static VALUE                                                            \
prefix ## _get_ ## name(VALUE self)                                     \
{                                                                       \
    return member_to_rb((self_to_c(self))->member);                     \
}

#define DEF_WRITER(prefix, name, member, self_to_c, value_to_c) \
static VALUE                                                    \
prefix ## _set_ ## name(VALUE self, VALUE value)                \
{                                                               \
    (self_to_c(self))->member = value_to_c(value);              \
    return Qnil;                                                \
}

#define DEF_WRITER_WITH_SETTER(prefix, name, member,    \
                               self_to_c, value_setter) \
static VALUE                                            \
prefix ## _set_ ## name(VALUE self, VALUE value)        \
{                                                       \
    value_setter((self_to_c(self))->member, value);     \
    return Qnil;                                        \
}

#define DEF_ACCESSOR(prefix, member, self_to_c, member_to_rb, value_to_c) \
    DEF_READER(prefix, member, member, self_to_c, member_to_rb)         \
    DEF_WRITER(prefix, member, member, self_to_c, value_to_c)           \

#define DEF_ACCESSOR_WITH_SETTER(prefix, member, self_to_c,             \
                                 member_to_rb, value_setter)            \
    DEF_READER(prefix, member, member, self_to_c, member_to_rb)         \
    DEF_WRITER_WITH_SETTER(prefix, member, member, self_to_c, value_setter)

#define DEF_ACCESSOR_WITH_NAME(prefix, name, member, self_to_c, \
                               member_to_rb, value_to_c)        \
    DEF_READER(prefix, name, member, self_to_c, member_to_rb)   \
    DEF_WRITER(prefix, name, member, self_to_c, value_to_c)


#endif
