/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkeditable.c -

  $Author: sakai $
  $Date: 2003/07/18 13:41:41 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
edit_sel_region(self, start, end)
    VALUE self, start, end;
{
    gtk_editable_select_region(GTK_EDITABLE(RVAL2GOBJ(self)),
			       NUM2INT(start), NUM2INT(end));
    return self;
}

static VALUE
edit_get_sel_bounds(self)
    VALUE self;
{
    gint start, end;
    gboolean ret;
    ret = gtk_editable_get_selection_bounds(GTK_EDITABLE(RVAL2GOBJ(self)),
                                            &start, &end);
    return ret ? rb_ary_new3(2, INT2NUM(start), INT2NUM(end)) : Qnil;
}

static VALUE
edit_insert_text(self, new_text, pos)
    VALUE self, new_text, pos;
{
    gint p = NUM2INT(pos);

    StringValue(new_text);
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
edit_get_editable(self)
    VALUE self;
{
    return gtk_editable_get_editable(GTK_EDITABLE(RVAL2GOBJ(self))) ? Qtrue : Qfalse;
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

void 
Init_gtk_editable()
{
    VALUE mEditable = G_DEF_INTERFACE(GTK_TYPE_EDITABLE, "Editable", mGtk);

    rb_define_method(mEditable, "select_region", edit_sel_region, 2);
    rb_define_method(mEditable, "selection_bounds", edit_get_sel_bounds, 0);
    rb_define_method(mEditable, "insert_text", edit_insert_text, 2);
    rb_define_method(mEditable, "delete_text", edit_delete_text, 2);
    rb_define_method(mEditable, "get_chars", edit_get_chars, 2);
    rb_define_method(mEditable, "delete_selection", edit_delete_selection, 0);
    rb_define_method(mEditable, "position", edit_get_position, 0);
    rb_define_method(mEditable, "set_position", edit_set_position, 1);
    rb_define_method(mEditable, "set_editable", edit_set_editable, 1);
    rb_define_method(mEditable, "editable?", edit_get_editable, 0);
    rb_define_method(mEditable, "copy_clipboard", edit_copy_clipboard, 0);
    rb_define_method(mEditable, "cut_clipboard", edit_cut_clipboard, 0);
    rb_define_method(mEditable, "paste_clipboard", edit_paste_clipboard, 0);

    G_DEF_SETTERS(mEditable);
}
