/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbglib_mainloop.c -

  $Author: mutoh $
  $Date: 2005/03/11 19:14:12 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "global.h"

/*****************************************/
GType
g_main_loop_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GMainLoop",
                    (GBoxedCopyFunc)g_main_loop_ref,
                    (GBoxedFreeFunc)g_main_loop_unref);
  return our_type;
}
/*****************************************/

#define _SELF(s) ((GMainLoop*)RVAL2BOXED(s, G_TYPE_MAIN_LOOP))

static VALUE
ml_initialize(self, context, is_running)
    VALUE self, context, is_running;
{
    GMainLoop* ml;
    ml = g_main_loop_new(NIL_P(context)?NULL:RVAL2BOXED(context, G_TYPE_MAIN_CONTEXT), 
                         RTEST(is_running));
    G_INITIALIZE(self, ml);
    return Qnil;
}

static VALUE
ml_run(self)
    VALUE self;
{
    g_main_loop_run(_SELF(self));
    return self;
}

static VALUE
ml_quit(self)
    VALUE self;
{
    g_main_loop_quit(_SELF(self));
    return self;
}

static VALUE
ml_is_running(self)
    VALUE self;
{
    return CBOOL2RVAL(g_main_loop_is_running(_SELF(self)));
}

static VALUE
ml_get_context(self)
    VALUE self;
{
    return BOXED2RVAL(g_main_loop_get_context(_SELF(self)), G_TYPE_MAIN_CONTEXT);
}

void
Init_glib_main_loop()
{
    VALUE ml = G_DEF_CLASS(G_TYPE_MAIN_LOOP, "MainLoop", mGLib); 

    rb_define_method(ml, "initialize", ml_initialize, 2);
    rb_define_method(ml, "run", ml_run, 0);
    rb_define_method(ml, "quit", ml_quit, 0);
    rb_define_method(ml, "running?", ml_is_running, 0);
    rb_define_method(ml, "context", ml_get_context, 0);

}
