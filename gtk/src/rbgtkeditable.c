/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkeditable.c -

  $Author: mutoh $
  $Date: 2002/05/19 13:59:10 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

/*
 * Editable
 */
static VALUE
edit_sel_region(self, start, end)
    VALUE self, start, end;
{
    gtk_editable_select_region(GTK_EDITABLE(get_widget(self)),
			       NUM2INT(start), NUM2INT(end));
    return self;
}

static VALUE
edit_insert_text(self, new_text, pos)
    VALUE self, new_text, pos;
{
    gint p = NUM2INT(pos);

    Check_Type(new_text, T_STRING);
    gtk_editable_insert_text(GTK_EDITABLE(get_widget(self)),
			     RSTRING(new_text)->ptr,
			     RSTRING(new_text)->len,
			     &p);
    return INT2NUM(p);
}

static VALUE
edit_delete_text(self, start, end)
    VALUE self, start, end;
{
    gtk_editable_delete_text(GTK_EDITABLE(get_widget(self)),
			     NUM2INT(start), NUM2INT(end));
    return self;
}

static VALUE
edit_get_chars(self, start, end)
    VALUE self, start, end;
{
    gchar *s;
    VALUE ret;

    s = gtk_editable_get_chars(GTK_EDITABLE(get_widget(self)), /* check s */
			       NUM2INT(start), NUM2INT(end));  /* check start,end */
    if (s) {
        ret = rb_str_new2(s);
        g_free(s);
    } else {
        ret = Qnil;
    }
    return ret;
}

/*
static VALUE
edit_cut_clipboard(self, time)
    VALUE self, time;
{
    gtk_editable_cut_clipboard(GTK_EDITABLE(get_widget(self)),NUM2INT(time));
    return self;
}

static VALUE
edit_copy_clipboard(self, time)
    VALUE self, time;
{
    gtk_editable_copy_clipboard(GTK_EDITABLE(get_widget(self)),NUM2INT(time));
    return self;
}
	
static VALUE
edit_paste_clipboard(self, time)
    VALUE self, time;
{
    gtk_editable_paste_clipboard(GTK_EDITABLE(get_widget(self)),NUM2INT(time));
    return self;
}
*/

#if GTK_MAJOR_VERSION < 2
	
static VALUE
edit_claim_selection(self, claim, time)
    VALUE self, claim, time;
{
    gtk_editable_claim_selection(GTK_EDITABLE(get_widget(self)),
				 RTEST(claim), NUM2INT(time));
    return self;
}

#endif
	
static VALUE
edit_delete_selection(self)
    VALUE self;
{
    gtk_editable_delete_selection(GTK_EDITABLE(get_widget(self)));
    return self;
}

#if GTK_MAJOR_VERSION < 2
static VALUE
edit_changed(self)
    VALUE self;
{
    gtk_editable_changed(GTK_EDITABLE(get_widget(self)));
    return self;
}

static VALUE
edit_is_editable(self)
    VALUE self;
{
    return GTK_EDITABLE(get_widget(self))->editable? Qtrue: Qfalse;
}

#endif

static VALUE
edit_get_position(self)
    VALUE self;
{
    return INT2NUM(gtk_editable_get_position(GTK_EDITABLE(get_widget(self))));
}

static VALUE
edit_set_position(self, position)
    VALUE self, position;
{
    gtk_editable_set_position(GTK_EDITABLE(get_widget(self)),
			      NUM2INT(position));
    return self;
}

static VALUE
edit_set_editable(self, editable)
    VALUE self, editable;
{
    gtk_editable_set_editable(GTK_EDITABLE(get_widget(self)), RTEST(editable));
    return self;
}

static VALUE
edit_cut_clipboard(self)
    VALUE self;
{
    gtk_editable_cut_clipboard(GTK_EDITABLE(get_widget(self)));
    return self;
}

static VALUE
edit_copy_clipboard(self)
    VALUE self;
{
    gtk_editable_copy_clipboard(GTK_EDITABLE(get_widget(self)));
    return self;
}

static VALUE
edit_paste_clipboard(self)
    VALUE self;
{
    gtk_editable_paste_clipboard(GTK_EDITABLE(get_widget(self)));
    return self;
}

#if GTK_MAJOR_VERSION < 2
static VALUE
edit_has_selection_p(self)
    VALUE self;
{
    return GTK_EDITABLE(get_widget(self))->has_selection? Qtrue: Qfalse;
}

static VALUE
edit_selection_start_pos(self)
    VALUE self;
{
    return INT2NUM(GTK_EDITABLE(get_widget(self))->selection_start_pos);
}

static VALUE
edit_selection_end_pos(self)
    VALUE self;
{
    return INT2NUM(GTK_EDITABLE(get_widget(self))->selection_end_pos);
}

#endif

void Init_gtk_editable()
{
    gEditable = rb_define_class_under(mGtk, "Editable", gWidget);

    rb_define_const(gEditable, "SIGNAL_ACTIVATE", rb_str_new2("activate"));
    rb_define_const(gEditable, "SIGNAL_CHANGED", rb_str_new2("changed"));

    rb_define_method(gEditable, "select_region", edit_sel_region, 2);
    rb_define_method(gEditable, "insert_text", edit_insert_text, 2);
    rb_define_method(gEditable, "delete_text", edit_delete_text, 2);
    rb_define_method(gEditable, "get_chars", edit_get_chars, 2);
    /*
      rb_define_method(gEditable, "cut_clipboard", edit_cut_clipboard, 1);
      rb_define_method(gEditable, "copy_clipboard", edit_copy_clipboard, 1);
      rb_define_method(gEditable, "paste_clipboard", edit_paste_clipboard, 1);
    */
#if GTK_MAJOR_VERSION < 2
    rb_define_method(gEditable, "claim_selection", edit_claim_selection, 2);
#endif
    rb_define_method(gEditable, "delete_selection", edit_delete_selection, 0);
#if GTK_MAJOR_VERSION < 2
    rb_define_method(gEditable, "changed", edit_changed, 0);
    rb_define_method(gEditable, "editable?", edit_is_editable, 0);
#endif
    rb_define_method(gEditable, "position", edit_get_position, 0);
    rb_define_method(gEditable, "set_position", edit_set_position, 1);
    rb_define_alias(gEditable, "position=", "set_position");
    rb_define_method(gEditable, "set_editable", edit_set_editable, 1);
    rb_define_method(gEditable, "copy_clipboard", edit_copy_clipboard, 0);
    rb_define_method(gEditable, "cut_clipboard", edit_cut_clipboard, 0);
    rb_define_method(gEditable, "paste_clipboard", edit_paste_clipboard, 0);
#if GTK_MAJOR_VERSION < 2
    rb_define_method(gEditable, "has_selection?", edit_has_selection_p, 0);
    rb_define_method(gEditable, "selection_start_pos", edit_selection_start_pos, 0);
    rb_define_method(gEditable, "selection_end_pos", edit_selection_end_pos, 0);
#endif

    /* child initialization */
    Init_gtk_entry();
    Init_gtk_text();
}
