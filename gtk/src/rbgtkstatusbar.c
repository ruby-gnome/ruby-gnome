/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkstatusbar.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE sMsg;

static VALUE
statusbar_initialize(self)
     VALUE self;
{
  set_widget(self, gtk_statusbar_new());
  return Qnil;
}

static VALUE
statusbar_push(self, id, text)
     VALUE self;
     VALUE id;
     VALUE text;
{
  gint message_id;
  message_id = gtk_statusbar_push(GTK_STATUSBAR(get_widget(self)), 
				  NUM2INT(id), STR2CSTR(text));
  return INT2FIX(message_id);
}

static VALUE
statusbar_pop(self, id)
     VALUE self;
     VALUE id;
{
  gtk_statusbar_pop(GTK_STATUSBAR(get_widget(self)), NUM2INT(id));
  return Qnil;

}

static VALUE
statusbar_get_context_id(self, text)
     VALUE self;
     VALUE text;
{
  gint context_id;
  context_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(get_widget(self)),
					    STR2CSTR(text));
  return INT2FIX(context_id);
}

static VALUE
statusbar_remove(self, cid, mid)
     VALUE self;
     VALUE cid;
     VALUE mid;
{
  gtk_statusbar_remove(GTK_STATUSBAR(get_widget(self)),
		       NUM2INT(cid), NUM2INT(mid)); 
  return Qnil;
}

void Init_gtk_statusbar()
{
    static rbgtk_class_info cinfo;

    gStatusBar = rb_define_class_under(mGtk, "Statusbar", gHBox);
    cinfo.klass = gStatusBar;
    cinfo.gtype = GTK_TYPE_STATUSBAR;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);
    
    rb_define_const(gStatusBar, "SIGNAL_TEXT_PUSHDED", rb_str_new2("text_pushed"));
    rb_define_const(gStatusBar, "SIGNAL_TEXT_POPPED", rb_str_new2("text_popped"));

    rb_define_method(gStatusBar, "initialize", statusbar_initialize, 0);
    rb_define_method(gStatusBar, "push", statusbar_push, 2);
    rb_define_method(gStatusBar, "pop", statusbar_pop, 1);
    rb_define_method(gStatusBar, "get_context_id", statusbar_get_context_id, 1);
    rb_define_method(gStatusBar, "remove", statusbar_remove, 2);

    sMsg = rb_struct_define("SMsg", "text", "context_id", "message_id", 0);   
}
