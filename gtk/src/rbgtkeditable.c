/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkeditable.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

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
    gtk_editable_select_region(GTK_EDITABLE(RVAL2GOBJ(self)),
			       NUM2INT(start), NUM2INT(end));
    return self;
}

static VALUE
edit_insert_text(self, new_text, pos)
    VALUE self, new_text, pos;
{
    gint p = NUM2INT(pos);

    Check_Type(new_text, T_STRING);
    gtk_editable_insert_text(GTK_EDITABLE(RVAL2GOBJ(self)),
			     RSTRING(new_text)->ptr,
			     RSTRING(new_text)->len,
			     &p);
    return INT2NUM(p);
}

static VALUE
edit_delete_text(self, start, end)
    VALUE self, start, end;
{
    gtk_editable_delete_text(GTK_EDITABLE(RVAL2GOBJ(self)),
			     NUM2INT(start), NUM2INT(end));
    return self;
}

static VALUE
edit_get_chars(self, start, end)
    VALUE self, start, end;
{
    gchar *s;
    VALUE ret;

    s = gtk_editable_get_chars(GTK_EDITABLE(RVAL2GOBJ(self)), /* check s */
			       NUM2INT(start), NUM2INT(end));  /* check start,end */
    if (s) {
        ret = rb_str_new2(s);
        g_free(s);
    } else {
        ret = Qnil;
    }
    return ret;
}

static VALUE
edit_delete_selection(self)
    VALUE self;
{
    gtk_editable_delete_selection(GTK_EDITABLE(RVAL2GOBJ(self)));
    return self;
}

static VALUE
edit_get_position(self)
    VALUE self;
{
    return INT2NUM(gtk_editable_get_position(GTK_EDITABLE(RVAL2GOBJ(self))));
}

static VALUE
edit_set_position(self, position)
    VALUE self, position;
{
    gtk_editable_set_position(GTK_EDITABLE(RVAL2GOBJ(self)),
			      NUM2INT(position));
    return self;
}

static VALUE
edit_set_editable(self, editable)
    VALUE self, editable;
{
    gtk_editable_set_editable(GTK_EDITABLE(RVAL2GOBJ(self)), RTEST(editable));
    return self;
}

static VALUE
edit_cut_clipboard(self)
    VALUE self;
{
    gtk_editable_cut_clipboard(GTK_EDITABLE(RVAL2GOBJ(self)));
    return self;
}

static VALUE
edit_copy_clipboard(self)
    VALUE self;
{
    gtk_editable_copy_clipboard(GTK_EDITABLE(RVAL2GOBJ(self)));
    return self;
}

static VALUE
edit_paste_clipboard(self)
    VALUE self;
{
    gtk_editable_paste_clipboard(GTK_EDITABLE(RVAL2GOBJ(self)));
    return self;
}

void Init_gtk_editable()
{
    static RGObjClassInfo cinfo;

    gEditable = rb_define_class_under(mGtk, "Editable", gWidget);
    cinfo.klass = gEditable;
    cinfo.gtype = GTK_TYPE_EDITABLE;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_const(gEditable, "SIGNAL_ACTIVATE", rb_str_new2("activate"));
    rb_define_const(gEditable, "SIGNAL_CHANGED", rb_str_new2("changed"));

    rb_define_method(gEditable, "select_region", edit_sel_region, 2);
    rb_define_method(gEditable, "insert_text", edit_insert_text, 2);
    rb_define_method(gEditable, "delete_text", edit_delete_text, 2);
    rb_define_method(gEditable, "get_chars", edit_get_chars, 2);
    rb_define_method(gEditable, "delete_selection", edit_delete_selection, 0);
    rb_define_method(gEditable, "position", edit_get_position, 0);
    rb_define_method(gEditable, "set_position", edit_set_position, 1);
    rb_define_alias(gEditable, "position=", "set_position");
    rb_define_method(gEditable, "set_editable", edit_set_editable, 1);
    rb_define_method(gEditable, "copy_clipboard", edit_copy_clipboard, 0);
    rb_define_method(gEditable, "cut_clipboard", edit_cut_clipboard, 0);
    rb_define_method(gEditable, "paste_clipboard", edit_paste_clipboard, 0);

    /* child initialization */
    Init_gtk_entry();
#ifdef GTK_ENABLE_BROKEN
    Init_gtk_text();
#endif
}
