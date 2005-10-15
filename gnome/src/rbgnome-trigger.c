/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-trigger.c,v 1.2 2005/10/15 07:35:08 mutoh Exp $ */

/* Gnome::Trigger class for Ruby/GNOME2
 * Copyright (C) 2005  Ruby-GNOME2 Project Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "rbgnome.h"

/*
 Gnome::Trigger.new(Gnome::Trigger::FUNCTION, level = nil)
 Gnome::Trigger.new(Gnome::Trigger::COMMAND, command, level = nil)
 Gnome::Trigger.new(Gnome::Trigger::MEDIAPLAY, file, cache_id, level = nil)
 */

static ID id_action;
static ID id_call;
static VALUE cTrig;

/*****************************************/
GnomeTrigger*
gnome_trigger_copy(const GnomeTrigger* trig)
{
    GnomeTrigger* new;
    new = g_new(GnomeTrigger, 1);
    *new = *trig;
    return new;
}

GType
gnome_trigger_get_type (void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static ("GnomeTrigger",
                                                 (GBoxedCopyFunc)gnome_trigger_copy,
                                                 (GBoxedFreeFunc)g_free);
    return our_type;
}

#define GNOME_TYPE_TRIGGER (gnome_trigger_get_type())
#define _SELF(s) ((GnomeTrigger*)RVAL2BOXED(s, GNOME_TYPE_TRIGGER))

/*****************************************/

static void
trig_action_function(msg, level, supinfo)
    char* msg;
    char* level;
    char* supinfo[];
{
    int i = 0;
    VALUE func = rb_cvar_get(cTrig, id_action);

    if (NIL_P(func)){
        g_printerr("trigger: %s, %s\n", msg, level);
        g_printerr("         ");
        while (TRUE) {
            if (supinfo[i]){
                g_printerr("[%s]", supinfo[i]);
            } else {
                break;
            }
            i++;
        }
        g_printerr("\n");
    } else {
        VALUE ary = rb_ary_new();
        while (TRUE) {
            if (supinfo[i]){
                rb_ary_push(ary, CSTR2RVAL(supinfo[i]));
            } else {
                break;
            }
            i++;
        }
        rb_funcall(func, id_call, 3, CSTR2RVAL(msg), CSTR2RVAL2(level), ary);
    }
}

static VALUE
trig_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE type, arg1, arg2, arg3;
    GnomeTrigger trig;
    
    rb_scan_args(argc, argv, "13", &type, &arg1, &arg2, &arg3);
    
    trig.type = RVAL2GENUM(type, GNOME_TYPE_TRIGGER_TYPE);
    
    if (trig.type == GTRIG_FUNCTION){
        trig.u.function = trig_action_function;
        trig.level = NIL_P(arg1) ? NULL : RVAL2CSTR(arg1);
        G_RELATIVE(self, arg1);
    } else if (trig.type == GTRIG_COMMAND) {
        trig.u.command = RVAL2CSTR(arg1);
        G_RELATIVE(self, arg1);
        trig.level = NIL_P(arg2) ? NULL : RVAL2CSTR(arg2);
        G_RELATIVE(self, arg2);
    }   else if (trig.type == GTRIG_MEDIAPLAY) {
        trig.u.media.file = RVAL2CSTR(arg1);
        G_RELATIVE(self, arg1);
        trig.u.media.cache_id = NUM2INT(arg2);
        trig.level = NIL_P(arg3) ? NULL : RVAL2CSTR(arg3);
        G_RELATIVE(self, arg3);
    } else {
        rb_raise(rb_eRuntimeError, "Wrong trigger type: %s", RVAL2CSTR(type));
    }
    G_INITIALIZE(self, &trig);
    return Qnil;
}

static VALUE
trig_s_set_action_func(self)
    VALUE self;
{
#if RUBY_VERSION_CODE < 180
    rb_cvar_set(cTrig, id_action, G_BLOCK_PROC()); 
#else
    rb_cvar_set(cTrig, id_action, G_BLOCK_PROC(), 0); 
#endif
    return self;
}

static VALUE
trig_add_trigger(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE args;
    int i, len;
    char** supinfo;

    rb_scan_args(argc, argv, "0*", &args);
    len = RARRAY(args)->len;

    supinfo = (char**)g_new(char*, len + 1);

    for (i = 0; i < len; i++){
        supinfo[i] = g_strdup(RVAL2CSTR(RARRAY(args)->ptr[i]));
    }
    supinfo[len] = NULL;

    gnome_triggers_vadd_trigger(_SELF(self), supinfo);

    return self;
}

static VALUE
trig_s_do(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE msg, level, args;
    int i, len;
    char** supinfo;

    rb_scan_args(argc, argv, "2*", &msg, &level, &args);
    len = RARRAY(args)->len;

    supinfo = (char**)g_new(char*, len + 1);

    for (i = 0; i < len; i++){
        supinfo[i] = g_strdup(RVAL2CSTR(RARRAY(args)->ptr[i]));
    }
    supinfo[len] = NULL;

    gnome_triggers_vdo((const char*)(NIL_P(msg) ? NULL : RVAL2CSTR(msg)), 
                       (const char*)(NIL_P(level) ? NULL : RVAL2CSTR(level)), 
                       (const char**)supinfo);

    return self;
}
void
Init_gnome_trigger(mGnome)
    VALUE mGnome;
{
    cTrig = G_DEF_CLASS(GNOME_TYPE_TRIGGER, "Trigger", mGnome);

    id_call = rb_intern("call");
    id_action = rb_intern("__action_proc__");

#if RUBY_VERSION_CODE < 180
    rb_cvar_set(cTrig, id_action, Qnil);
#else
    rb_cvar_set(cTrig, id_action, Qnil, 0);
#endif
    rb_define_method(cTrig, "initialize", trig_initialize, -1);
    rb_define_singleton_method(cTrig, "set_action_func", trig_s_set_action_func, 0);

    rb_define_method(cTrig, "add_trigger", trig_add_trigger, -1);
    rb_define_singleton_method(cTrig, "do", trig_s_do, -1);

    /* GnomeTriggerType */
    G_DEF_CLASS(GNOME_TYPE_TRIGGER_TYPE, "Type", cTrig);
    G_DEF_CONSTANTS(cTrig, GNOME_TYPE_TRIGGER_TYPE, "GTRIG_");
}
