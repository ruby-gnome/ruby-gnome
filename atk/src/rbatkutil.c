/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkutil.c -

  $Author: mutoh $
  $Date: 2004/02/19 17:16:54 $

  Copyright (C) 2004 Masao Mutoh
************************************************/

#include "rbatk.h"

/*
struct      AtkUtil;
guint       atk_add_focus_tracker           (AtkEventListener focus_tracker);
void        atk_remove_focus_tracker        (guint tracker_id);
void        atk_focus_tracker_init          (AtkEventListenerInit add_function);
void        atk_focus_tracker_notify        (AtkObject *object);
void        (*AtkEventListener)             (AtkObject*);
void        (*AtkEventListenerInit)         (void);
guint       atk_add_global_event_listener   (GSignalEmissionHook listener,
                                             const gchar *event_type);
void        atk_remove_global_event_listener
                                            (guint listener_id);
struct      AtkKeyEventStruct;
enum        AtkKeyEventType;
gint        (*AtkKeySnoopFunc)              (AtkKeyEventStruct *event,
                                             gpointer func_data);
guint       atk_add_key_event_listener      (AtkKeySnoopFunc listener,
                                             gpointer data);
void        atk_remove_key_event_listener   (guint listener_id);
*/

static VALUE
rbatk_get_root(self)
    VALUE self;
{
    return GOBJ2RVAL(atk_get_root());
}

static VALUE
rbatk_get_toolkit_name(self)
    VALUE self;
{
    return CSTR2RVAL(atk_get_toolkit_name());
}

static VALUE
rbatk_get_toolkit_version(self)
    VALUE self;
{
    return CSTR2RVAL(atk_get_toolkit_version());
}

void
Init_atk_util()
{
    VALUE coord;

    rb_define_singleton_method(mAtk, "root", rbatk_get_root, 0);
    rb_define_singleton_method(mAtk, "toolkit_name", rbatk_get_toolkit_name, 0);
    rb_define_singleton_method(mAtk, "toolkit_version", rbatk_get_toolkit_version, 0);

    /* AtkCoordType */
    coord = G_DEF_CLASS(ATK_TYPE_COORD_TYPE, "CoordType", mAtk);
    G_DEF_CONSTANTS(mAtk, ATK_TYPE_COORD_TYPE, "ATK_");
}
