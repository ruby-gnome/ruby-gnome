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

G_GNUC_INTERNAL void Init_atk_action(void);
G_GNUC_INTERNAL void Init_atk_component(void);
G_GNUC_INTERNAL void Init_atk_document(void);
G_GNUC_INTERNAL void Init_atk_editabletext(void);
G_GNUC_INTERNAL void Init_atk_gobjectaccessible(void);
G_GNUC_INTERNAL void Init_atk_hyperlink(void);
G_GNUC_INTERNAL void Init_atk_hypertext(void);
G_GNUC_INTERNAL void Init_atk_image(void);
G_GNUC_INTERNAL void Init_atk_implementor(void);
G_GNUC_INTERNAL void Init_atk_noopobject(void);
G_GNUC_INTERNAL void Init_atk_noopobjectfactory(void);
G_GNUC_INTERNAL void Init_atk_object(void);
G_GNUC_INTERNAL void Init_atk_object_role(VALUE cObject);
G_GNUC_INTERNAL void Init_atk_objectfactory(void);
G_GNUC_INTERNAL void Init_atk_registry(void);
G_GNUC_INTERNAL void Init_atk_relation(void);
G_GNUC_INTERNAL void Init_atk_relation_type(VALUE cRelation);
G_GNUC_INTERNAL void Init_atk_relation_set(void);
G_GNUC_INTERNAL void Init_atk_selection(void);
G_GNUC_INTERNAL void Init_atk_state(void);
G_GNUC_INTERNAL void Init_atk_state_set(void);
G_GNUC_INTERNAL void Init_atk_streamable_content(void);
G_GNUC_INTERNAL void Init_atk_table(void);
G_GNUC_INTERNAL void Init_atk_text(void);
G_GNUC_INTERNAL void Init_atk_text_attribute(VALUE mText);
G_GNUC_INTERNAL void Init_atk_text_range(void);
G_GNUC_INTERNAL void Init_atk_text_rectangle(void);
G_GNUC_INTERNAL void Init_atk_util(void);
G_GNUC_INTERNAL void Init_atk_value(void);

G_END_DECLS

#endif /* RB_ATK_PRIVATE_H */
