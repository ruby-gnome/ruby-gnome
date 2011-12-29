/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
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

#include "rbatkprivate.h"

#define RG_TARGET_NAMESPACE cUtil
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
rg_s_focus_tracker_notify(VALUE self, VALUE obj)
{
    atk_focus_tracker_notify(RVAL2ATKOBJECT(obj));
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
rg_s_add_key_event_listener(VALUE self)
{
    guint ret;
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    ret = atk_add_key_event_listener((AtkKeySnoopFunc)key_snoop_func, (gpointer)func);
    return UINT2NUM(ret);
}

static VALUE
rg_s_remove_key_event_listener(VALUE self, VALUE id)
{
    atk_remove_key_event_listener(NUM2UINT(id));
    return self;
}

static VALUE
rg_s_root(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(atk_get_root());
}

#if ATK_CHECK_VERSION(1,6,0)
static VALUE
rg_s_focus_object(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(atk_get_focus_object());
}
#endif

static VALUE
rg_s_toolkit_name(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(atk_get_toolkit_name());
}

static VALUE
rg_s_toolkit_version(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(atk_get_toolkit_version());
}

void
Init_atk_util(VALUE mAtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(ATK_TYPE_UTIL, "Util", mAtk);

    id_call = rb_intern("call");

    RG_DEF_SMETHOD(focus_tracker_notify, 1);
    RG_DEF_SMETHOD(add_key_event_listener, 0);
    RG_DEF_SMETHOD(remove_key_event_listener, 1);

    RG_DEF_SMETHOD(root, 0);
#if ATK_CHECK_VERSION(1,6,0)
    RG_DEF_SMETHOD(focus_object, 0);
#endif
    RG_DEF_SMETHOD(toolkit_name, 0);
    RG_DEF_SMETHOD(toolkit_version, 0);

    /* AtkCoordType */
    G_DEF_CLASS(ATK_TYPE_COORD_TYPE, "CoordType", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, ATK_TYPE_COORD_TYPE, "ATK_");

    /* AtkKeyEventType */
    G_DEF_CLASS(ATK_TYPE_KEY_EVENT_TYPE, "KeyEventType", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, ATK_TYPE_KEY_EVENT_TYPE, "ATK_");

}
