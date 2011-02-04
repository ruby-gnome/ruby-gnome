/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkutil.c -

  $Author: sakai $
  $Date: 2007/07/08 02:51:52 $

  Copyright (C) 2004 Masao Mutoh
************************************************/

#include "rbatk.h"

static ID id_call;

/* How can I implement them?
guint       atk_add_focus_tracker           (AtkEventListener focus_tracker);
void        atk_remove_focus_tracker        (guint tracker_id);
void        atk_focus_tracker_init          (AtkEventListenerInit add_function);
void        (*AtkEventListener)             (AtkObject*);
void        (*AtkEventListenerInit)         (void);
guint       atk_add_global_event_listener   (GSignalEmissionHook listener,
                                             const gchar *event_type);
void        atk_remove_global_event_listener
                                            (guint listener_id);
*/

static VALUE
rbatk_focus_tracker_notify(VALUE self, VALUE obj)
{
    atk_focus_tracker_notify(ATK_OBJECT(RVAL2GOBJ(obj)));
    return self;
}

static gint
key_snoop_func(AtkKeyEventStruct* event, gpointer func)
{
    VALUE ret = rb_funcall((VALUE)func, id_call, 7,
                           INT2NUM(event->type), UINT2NUM(event->state),
                           UINT2NUM(event->keyval), INT2NUM(event->length),
                           CSTR2RVAL(event->string), UINT2NUM(event->keycode),
                           UINT2NUM(event->timestamp));
    return NUM2INT(ret);
}

static VALUE
rbatk_add_key_event_listener(VALUE self)
{
    guint ret;
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    ret = atk_add_key_event_listener((AtkKeySnoopFunc)key_snoop_func, (gpointer)func);
    return UINT2NUM(ret);
}

static VALUE
rbatk_remove_key_event_listener(VALUE self, VALUE id)
{
    atk_remove_key_event_listener(NUM2UINT(id));
    return self;
}

static VALUE
rbatk_get_root(VALUE self)
{
    return GOBJ2RVAL(atk_get_root());
}

#if ATK_CHECK_VERSION(1,6,0)
static VALUE
rbatk_get_focus_object(VALUE self)
{
    return GOBJ2RVAL(atk_get_focus_object());
}
#endif

static VALUE
rbatk_get_toolkit_name(VALUE self)
{
    return CSTR2RVAL(atk_get_toolkit_name());
}

static VALUE
rbatk_get_toolkit_version(VALUE self)
{
    return CSTR2RVAL(atk_get_toolkit_version());
}

void
Init_atk_util()
{
    VALUE coord;

    VALUE util = G_DEF_CLASS(ATK_TYPE_UTIL, "Util", mAtk);

    id_call = rb_intern("call");

    rb_define_singleton_method(util, "focus_tracker_notify", rbatk_focus_tracker_notify, 1);
    rb_define_singleton_method(util, "add_key_event_listener", rbatk_add_key_event_listener, 0);
    rb_define_singleton_method(util, "remove_key_event_listener", rbatk_remove_key_event_listener, 1);

    rb_define_singleton_method(util, "root", rbatk_get_root, 0);
#if ATK_CHECK_VERSION(1,6,0)
    rb_define_singleton_method(util, "focus_object", rbatk_get_focus_object, 0);
#endif
    rb_define_singleton_method(util, "toolkit_name", rbatk_get_toolkit_name, 0);
    rb_define_singleton_method(util, "toolkit_version", rbatk_get_toolkit_version, 0);
    rb_define_singleton_method(util, "toolkit_version", rbatk_get_toolkit_version, 0);

    /* AtkCoordType */
    coord = G_DEF_CLASS(ATK_TYPE_COORD_TYPE, "CoordType", util);
    G_DEF_CONSTANTS(util, ATK_TYPE_COORD_TYPE, "ATK_");

    /* AtkKeyEventType */
    coord = G_DEF_CLASS(ATK_TYPE_KEY_EVENT_TYPE, "KeyEventType", util);
    G_DEF_CONSTANTS(util, ATK_TYPE_KEY_EVENT_TYPE, "ATK_");

}
