/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkstatusbar.c -

  $Author: mutoh $
  $Date: 2002/05/19 13:59:10 $

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

#if GTK_MAJOR_VERSION < 2
static VALUE
gSMsg2ary(slist)
    GSList *slist;
{
    VALUE msg;
    VALUE ary = rb_ary_new();

    while (slist) {
	GtkStatusbarMsg *gSMsg = slist->data;
	msg = rb_struct_new(sMsg,
			    rb_tainted_str_new2(gSMsg->text),
			    INT2FIX(gSMsg->context_id),
			    INT2FIX(gSMsg->message_id));
	rb_ary_push(ary, msg);
	slist = slist->next;
    }

    return ary;
}

static VALUE
statusbar_messages(self)
    VALUE self;
{
    return gSMsg2ary(GTK_STATUSBAR(get_widget(self))->messages);
}
#endif

void Init_gtk_statusbar()
{
    gStatusBar = rb_define_class_under(mGtk, "Statusbar", gHBox);
    
    rb_define_const(gStatusBar, "SIGNAL_TEXT_PUSHDED", rb_str_new2("text_pushed"));
    rb_define_const(gStatusBar, "SIGNAL_TEXT_POPPED", rb_str_new2("text_popped"));

    rb_define_method(gStatusBar, "initialize", statusbar_initialize, 0);
    rb_define_method(gStatusBar, "push", statusbar_push, 2);
    rb_define_method(gStatusBar, "pop", statusbar_pop, 1);
    rb_define_method(gStatusBar, "get_context_id", statusbar_get_context_id, 1);
    rb_define_method(gStatusBar, "remove", statusbar_remove, 2);
#if GTK_MAJOR_VERSION < 2
    rb_define_method(gStatusBar, "messages", statusbar_messages, 0);
#endif

    sMsg = rb_struct_define("SMsg", "text", "context_id", "message_id", 0);   
}
