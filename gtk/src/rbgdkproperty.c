/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkproperty.c -

  $Author: mutoh $
  $Date: 2003/10/15 17:53:23 $


  Copyright (C) 2003 Ruby-GNOME2 Project Team

  Devided from rbgdkwindow.c
  rbgdkwindow.c - 
  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/
#include "global.h"

static VALUE
gdkprop_text_property_to_text_list(self, encoding, format, text)
    VALUE self, encoding, format, text;
{
    gint num, i;
    gchar** list;
    VALUE ret = Qnil;

    StringValue(text);

    num = gdk_text_property_to_text_list(RVAL2ATOM(encoding),
                                         NUM2INT(format),
                                         RVAL2CSTR(text), RSTRING(text)->len, &list);

    ret = rb_ary_new2(num);
    for (i =0; i < num; i++){
        rb_ary_push(ret, CSTR2RVAL(list[i]));
    }
    gdk_free_text_list(list);
    return ret;
}

static VALUE
gdkprop_text_property_to_utf8_list(self, encoding, format, text)
    VALUE self, encoding, format, text;
{
    gint num, i;
    gchar** list;
    VALUE ret = Qnil;

    StringValue(text);

    num = gdk_text_property_to_utf8_list(RVAL2ATOM(encoding),
                                         NUM2INT(format),
                                         RVAL2CSTR(text), RSTRING(text)->len, &list);

    ret = rb_ary_new2(num);
    for (i =0; i < num; i++){
        rb_ary_push(ret, CSTR2RVAL(list[i]));
    }
    g_strfreev(list);
    return ret;
}

static VALUE
gdkprop_string_to_compound_text(self, str)
    VALUE self, str;
{
    GdkAtom encoding;
    gint format;
    guchar *ctext;
    gint length;
    gint num = gdk_string_to_compound_text(RVAL2CSTR(str),
                                           &encoding, &format,
                                           &ctext, &length);
    if (num == 0){
        VALUE ret = CSTR2RVAL(ctext);
        gdk_free_compound_text(ctext);
        return rb_ary_new3(3, BOXED2RVAL(encoding, GDK_TYPE_ATOM),
                           INT2NUM(format), ret);
    } else {
        rb_raise(rb_eRuntimeError, "failed to converts a string %d\n", num);
    }
    return Qnil;
}

static VALUE
gdkprop_utf8_to_string_target(self, str)
    VALUE self, str;
{
    return CSTR2RVAL(gdk_utf8_to_string_target(RVAL2CSTR(str)));
}

static VALUE
gdkprop_utf8_to_compound_text(self, str)
    VALUE self, str;
{
    GdkAtom encoding;
    gint format;
    guchar *ctext;
    gint length;
    gint ret = gdk_utf8_to_compound_text(RVAL2CSTR(str),
                                         &encoding, &format,
                                         &ctext, &length);
    if (ret){
        VALUE val = CSTR2RVAL(ctext);
        gdk_free_compound_text(ctext);
        return rb_ary_new3(3, BOXED2RVAL(encoding, GDK_TYPE_ATOM),
                           INT2NUM(format), val);
    } else {
        rb_raise(rb_eRuntimeError, "failed to converts a string %d\n", ret);
    }
    return Qnil;
}


static VALUE
gdkprop_change(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    int        fmt, len;
    void*      dat;
    GdkAtom    ntype;
    VALUE win, property, type, size= Qnil, mode, src;
    
    if(6 == argc)
        rb_scan_args(argc, argv, "60", &win, &property, &type, &size, &mode, &src);
    else
        rb_scan_args(argc, argv, "50", &win, &property, &type, &mode, &src);
    
    rbgtk_atom2selectiondata(type, size, src, &ntype, &dat, &fmt, &len);
    
    gdk_property_change(GDK_WINDOW(RVAL2GOBJ(win)), RVAL2ATOM(property), 
                        ntype, fmt, RVAL2GENUM(mode, GDK_TYPE_PROP_MODE), dat, len);

    rbgtk_atom2selectiondata_free(ntype, dat);

    return self;
}

static VALUE
gdkprop_get(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    /* for argument processing */
    GdkAtom     rtype;
    gint        rfmt, rlen;
    void*	rdat;
    VALUE win, property, type, offset=INT2FIX(0), length=INT2FIX(9999), delete;
    
    /* for inner processing */
    int		i;
    VALUE	ret = 0;
    
    if(6 == argc)
        rb_scan_args(argc, argv, "60", &win, &property, &type, &offset, &length, &delete);
    else
        rb_scan_args(argc, argv, "40", &win, &property, &type, &delete);
    
    
    if(gdk_property_get(GDK_WINDOW(RVAL2GOBJ(win)), RVAL2ATOM(property), RVAL2ATOM(type),
                        NUM2INT(offset), NUM2INT(length),
                        RTEST(delete), &rtype, &rfmt, &rlen, (guchar**)&rdat) == FALSE){
        return Qnil;
    }
    
    switch(rfmt){
      case 8:
      default:
        ret = rb_str_new(rdat, rlen);
        break;
        
      case 16:
        ret = rb_ary_new();
        
        for( i = 0; i < rlen; i++){
            rb_ary_push(ret, rb_Integer(((unsigned short*)rdat)[i]));
        }
        break;
        
      case 32:
/*
  ret = rb_ary_new();

  if(rtype != GDK_SELECTION_TYPE_ATOM){
  for(i = 0; i < rlen; i++){
  rb_ary_push(ret, INT2FIX(((unsigned long *)rdat)[i]));
  }
  } else {
  for(i = 0; i < rlen; i++){
  rb_ary_push(ret, RVAL2BOXED((GdkAtom)(unsigned long *)rdat[i], GDK_TYPE_ATOM));
  }
  }
*/
        rb_warning("not implemented yet.");
        break;
    }
    
    return rb_ary_new3(3, BOXED2RVAL(&rtype, GDK_TYPE_ATOM), 
                       ret, INT2NUM(rlen));
}

static VALUE
gdkprop_delete(self, win, property)
    VALUE self, win, property;
{
    gdk_property_delete(GDK_WINDOW(RVAL2GOBJ(win)), RVAL2ATOM(property));
    return self;
}

void
Init_gtk_gdk_property()
{
    VALUE gdkProperty = rb_define_module_under(mGdk, "Property");

    rb_define_module_function(gdkProperty, "text_property_to_text_list", gdkprop_text_property_to_text_list, 3);
    rb_define_module_function(gdkProperty, "text_property_to_utf8_list", gdkprop_text_property_to_utf8_list, 3);
    rb_define_module_function(gdkProperty, "string_to_compound_text", gdkprop_string_to_compound_text, 1);
    rb_define_module_function(gdkProperty, "utf8_to_string_target", gdkprop_utf8_to_string_target, 1);
    rb_define_module_function(gdkProperty, "utf8_to_compound_text", gdkprop_utf8_to_compound_text, 1);
    rb_define_module_function(gdkProperty, "change", gdkprop_change, -1);
    rb_define_module_function(gdkProperty, "get", gdkprop_get, -1);
    rb_define_module_function(gdkProperty, "delete", gdkprop_delete, 2);

    /* GdkPropMode from GdkProperties */
    G_DEF_CLASS(GDK_TYPE_PROP_MODE, "PropMode", gdkProperty);
    G_DEF_CONSTANTS(gdkProperty, GDK_TYPE_PROP_MODE, "GDK_PROP_");

}
