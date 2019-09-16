#ifndef RB_ATK_PRIVATE_H
#define RB_ATK_PRIVATE_H

#include "rbatk.h"

#ifndef HAVE_RB_ERRINFO
#  define rb_errinfo() (ruby_errinfo)
#endif

#ifndef G_VALUE_INIT
#  define G_VALUE_INIT { 0, { { 0 } } }
#endif

G_BEGIN_DECLS

G_GNUC_INTERNAL void Init_atk_action(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_component(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_document(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_editabletext(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_gobjectaccessible(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_hyperlink(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_hypertext(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_image(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_implementor(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_noopobject(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_noopobjectfactory(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_object(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_object_role(VALUE cObject);
G_GNUC_INTERNAL void Init_atk_objectfactory(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_registry(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_relation(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_relation_type(VALUE cRelation);
G_GNUC_INTERNAL void Init_atk_relation_set(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_selection(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_state(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_state_set(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_streamable_content(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_table(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_text(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_text_attribute(VALUE mText);
G_GNUC_INTERNAL void Init_atk_text_range(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_text_rectangle(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_util(VALUE mAtk);
G_GNUC_INTERNAL void Init_atk_value(VALUE mAtk);

G_END_DECLS

#endif /* RB_ATK_PRIVATE_H */
