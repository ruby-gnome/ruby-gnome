/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#include "rbgdk3private.h"

#define RG_TARGET_NAMESPACE mProperty

/* deprecated
static VALUE
rg_s_text_property_to_text_list(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    gint num, i;
    gchar** list;
    VALUE ret = Qnil;

    if (argc == 3) {
        VALUE encoding, format, text;
        rb_scan_args(argc, argv, "30", &encoding, &format, &text);
        StringValue(text);

        num = gdk_text_property_to_text_list(RVAL2ATOM(encoding),
                                             NUM2INT(format),
                                             (const guchar*)RVAL2CSTR(text), 
                                             RSTRING_LEN(text), &list);
    } else {
        VALUE display, encoding, format, text;
        rb_scan_args(argc, argv, "40", &display, &encoding, &format, &text);
        StringValue(text);

        num = gdk_text_property_to_text_list_for_display(RVAL2GDKDISPLAYOBJECT(display),
                                                         RVAL2ATOM(encoding),
                                                         NUM2INT(format),
                                                         (const guchar*)RVAL2CSTR(text),
                                                         RSTRING_LEN(text),
                                                         &list);
    }

    ret = rb_ary_new2(num);
    for (i =0; i < num; i++){
        rb_ary_push(ret, CSTR2RVAL(list[i]));
    }
    gdk_free_text_list(list);
    return ret;
}
*/

/* TODO
static VALUE
rg_s_text_property_to_utf8_list(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    gint num, i;
    gchar** list;
    VALUE ret = Qnil;

    if (argc == 3) {
        VALUE encoding, format, text;
        rb_scan_args(argc, argv, "30", &encoding, &format, &text);
        StringValue(text);

        num = gdk_text_property_to_utf8_list(RVAL2ATOM(encoding),
                                             NUM2INT(format),
                                             (const guchar*)RVAL2CSTR(text),
                                             RSTRING_LEN(text), &list);
    } else {
        VALUE display, encoding, format, text;
        rb_scan_args(argc, argv, "40", &display, &encoding, &format, &text);
        StringValue(text);

        num = gdk_text_property_to_utf8_list_for_display(RVAL2GDKDISPLAYOBJECT(display),
                                                         RVAL2ATOM(encoding),
                                                         NUM2INT(format),
                                                         (const guchar*)RVAL2CSTR(text),
                                                         RSTRING_LEN(text),
                                                         &list);
    }

    ret = rb_ary_new2(num);
    for (i =0; i < num; i++){
        rb_ary_push(ret, CSTR2RVAL(list[i]));
    }
    g_strfreev(list);
    return ret;
}
*/

/* deprecated
static VALUE
rg_s_string_to_compound_text(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    gint num;
    GdkAtom encoding;
    gint format;
    guchar *ctext;
    gint length;

    if (argc == 1) {
        VALUE str;
        rb_scan_args(argc, argv, "10", &str);
        num = gdk_string_to_compound_text(RVAL2CSTR(str),
                                          &encoding, &format,
                                          &ctext, &length);
    } else {
        VALUE display, str;

        rb_scan_args(argc, argv, "20", &display, &str);
        num = gdk_string_to_compound_text_for_display(RVAL2GDKDISPLAYOBJECT(display),
                                                      RVAL2CSTR(str),
                                                      &encoding, &format,
                                                      &ctext, &length);
    }

    if (num == 0){
        VALUE ret = CSTR2RVAL((const char*)ctext);
        gdk_free_compound_text(ctext);
        return rb_ary_new3(3, GDKATOM2RVAL(encoding),
                           INT2NUM(format), ret);
    } else {
        rb_raise(rb_eRuntimeError, "failed to converts a string %d\n", num);
    }
    return Qnil;
}
*/

static VALUE
rg_s_utf8_to_string_target(G_GNUC_UNUSED VALUE self, VALUE str)
{
    return CSTR2RVAL((const char*)gdk_utf8_to_string_target(RVAL2CSTR(str)));
}

/* deprecated
static VALUE
rg_s_utf8_to_compound_text(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    GdkAtom encoding;
    gint format;
    guchar *ctext;
    gint length;
    gint ret;

    if (argc == 1) {
        VALUE str;
        rb_scan_args(argc, argv, "10", &str);

        ret = gdk_utf8_to_compound_text(RVAL2CSTR(str),
                                        &encoding, &format,
                                        &ctext, &length);
    } else {
        VALUE display, str;

        rb_scan_args(argc, argv, "20", &display, &str);
        ret = gdk_utf8_to_compound_text_for_display(RVAL2GDKDISPLAYOBJECT(display),
                                                    RVAL2CSTR(str),
                                                    &encoding, &format,
                                                    &ctext, &length);
    }

    if (ret){
        VALUE val = CSTR2RVAL((const char*)ctext);
        gdk_free_compound_text(ctext);
        return rb_ary_new3(3, GDKATOM2RVAL(encoding),
                           INT2NUM(format), val);
    } else {
        rb_raise(rb_eRuntimeError, "failed to converts a string %d\n", ret);
    }
    return Qnil;
}
*/

/* TODO
static VALUE
rg_s_change(int argc, VALUE *argv, VALUE self)
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

    gdk_property_change(RVAL2GDKWINDOW(win), RVAL2ATOM(property), 
                        ntype, fmt, RVAL2GDKPROPMODE(mode), dat, len);

    rbgtk_atom2selectiondata_free(ntype, dat);

    return self;
}
*/

static VALUE
rg_s_get(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    /* for argument processing */
    GdkAtom     rtype;
    gint        rfmt, rlen;
    guchar* rdat;
    VALUE win, property, type, offset=INT2FIX(0), length=INT2FIX(9999), delete;

    /* for inner processing */
    gint        i;
    size_t j;
    VALUE   ret = 0;

    if(6 == argc)
        rb_scan_args(argc, argv, "60", &win, &property, &type, &offset, &length, &delete);
    else
        rb_scan_args(argc, argv, "40", &win, &property, &type, &delete);

    if(gdk_property_get(RVAL2GDKWINDOW(win), RVAL2ATOM(property), RVAL2ATOM(type),
                        NUM2INT(offset), NUM2INT(length),
                        RVAL2CBOOL(delete), &rtype, &rfmt, &rlen, &rdat) == FALSE){
        return Qnil;
    }

    switch(rfmt){
      case 8:
      default:
        ret = RBG_STRING_SET_UTF8_ENCODING(rb_str_new((const char*)rdat, rlen));
        break;

      case 16:
        ret = rb_ary_new();

        for( i = 0; i < rlen; i++){
            rb_ary_push(ret, rb_Integer(((unsigned short*)rdat)[i]));
        }
        break;

      case 32:
         ret = rb_ary_new();

         if(rtype != GDK_SELECTION_TYPE_ATOM){
           for(j = 0; j < (rlen/sizeof(unsigned long)); j++){
             rb_ary_push(ret, INT2FIX(((unsigned long*)rdat)[j]));
           }
         } else {
           for(j = 0; j < (rlen/sizeof(unsigned long)); j++){
             rb_ary_push(ret, GDKATOM2RVAL((GdkAtom)((unsigned long*)rdat)[j]));
           }
         }
        break;
    }

    return rb_ary_new3(3, GDKATOM2RVAL(rtype), 
                       ret, INT2NUM(rlen));
}

static VALUE
rg_s_delete(VALUE self, VALUE win, VALUE property)
{
    gdk_property_delete(RVAL2GDKWINDOW(win), RVAL2ATOM(property));
    return self;
}

void
Init_gdk_property(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGdk, "Property");

/* deprecated
    RG_DEF_SMETHOD(text_property_to_text_list, -1);
*/
/* TODO
    RG_DEF_SMETHOD(text_property_to_utf8_list, -1);
*/
/* deprecated
    RG_DEF_SMETHOD(string_to_compound_text, -1);
*/
    RG_DEF_SMETHOD(utf8_to_string_target, 1);
/* deprecated
    RG_DEF_SMETHOD(utf8_to_compound_text, -1);
*/
/* TODO
    RG_DEF_SMETHOD(change, -1);
*/
    RG_DEF_SMETHOD(get, -1);
    RG_DEF_SMETHOD(delete, 2);

    /* GdkPropMode from GdkProperties */
    G_DEF_CLASS(GDK_TYPE_PROP_MODE, "PropMode", RG_TARGET_NAMESPACE);
}
