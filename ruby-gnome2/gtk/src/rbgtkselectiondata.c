/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkselectiondata.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:35 $

  Copyright (C) 2002 MUTOH Masao
************************************************/


#include "global.h"

/* Gtkselectiondata */
static VALUE
gtkselectiondata_selection(self)
    VALUE self;
{
    return make_gdkatom(get_gtkselectiondata(self)->selection);
}

static VALUE
gtkselectiondata_target(self)
    VALUE self;
{
    return make_gdkatom(get_gtkselectiondata(self)->target);
}

static VALUE
gtkselectiondata_type(self)
    VALUE self;
{
    return make_gdkatom(get_gtkselectiondata(self)->type);
}

static VALUE
gtkselectiondata_format(self)
    VALUE self;
{
    return INT2NUM(get_gtkselectiondata(self)->format);
}

static VALUE
gtkselectiondata_data(self)
    VALUE self;
{
    return rb_str_new(get_gtkselectiondata(self)->data, 
					  get_gtkselectiondata(self)->length);
}

/* Instance Methods */
static VALUE
gtkselectiondata_set(self, type, format, data)
    VALUE self, type, format, data;
{
    gtk_selection_data_set(get_gtkselectiondata(self), get_gdkatom(type),
						   NUM2INT(format), RSTRING(data)->ptr, 
						   RSTRING(data)->len);
    return self;
}

static VALUE
gtkselectiondata_copy(self)
    VALUE self;
{
    return make_gtkselectiondata(gtk_selection_data_copy(get_gtkselectiondata(self)));
}

void
Init_gtk_selectiondata()
{
    gSelectionData = rb_define_class_under(mGtk, "SelectionData", rb_cData);
    rb_define_method(gSelectionData, "selection", gtkselectiondata_selection, 0);
    rb_define_method(gSelectionData, "target", gtkselectiondata_target, 0);
    rb_define_method(gSelectionData, "type", gtkselectiondata_type, 0);
    rb_define_method(gSelectionData, "format", gtkselectiondata_format, 0);
    rb_define_method(gSelectionData, "data", gtkselectiondata_data, 0);

    rb_define_method(gSelectionData, "set", gtkselectiondata_set, 3);
    rb_define_method(gSelectionData, "copy", gtkselectiondata_copy, 0);

} 

