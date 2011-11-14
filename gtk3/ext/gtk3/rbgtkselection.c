/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
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

#include "global.h"

#define RG_TARGET_NAMESPACE mSelection

#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

static VALUE
rg_m_owner_set(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    gboolean ret;

    if (argc == 3){
        VALUE widget, selection, time;
        rb_scan_args(argc, argv, "30", &widget, &selection, &time);
        ret = gtk_selection_owner_set(RVAL2WIDGET(widget), 
                                      RVAL2ATOM(selection), NUM2INT(time));
    } else {
#if GTK_CHECK_VERSION(2,2,0)
        VALUE display, widget, selection, time;
        rb_scan_args(argc, argv, "40", &display, &widget, &selection, &time);
        ret = gtk_selection_owner_set_for_display(GDK_DISPLAY_OBJECT(RVAL2GOBJ(display)),
                                                  RVAL2WIDGET(widget), 
                                                  RVAL2ATOM(selection), NUM2INT(time));
#else
        rb_raise(rb_eArgError, "Wrong number of arguments: %d", argc);
#endif
    }
    return CBOOL2RVAL(ret);
}

static VALUE
rg_m_add_target(VALUE self, VALUE widget, VALUE selection, VALUE target, VALUE info)
{
    gtk_selection_add_target(RVAL2WIDGET(widget), RVAL2ATOM(selection),
                             RVAL2ATOM(target), NUM2INT(info));
    return self;
}

static VALUE
rg_m_add_targets(VALUE self, VALUE rbwidget, VALUE rbselection, VALUE rbtargets)
{
    GtkWidget *widget = RVAL2WIDGET(rbwidget);
    GdkAtom selection = RVAL2ATOM(rbselection);
    long n;
    GtkTargetEntry *targets = RVAL2GTKTARGETENTRIES(rbtargets, &n);

    gtk_selection_add_targets(widget, selection, targets, n);

    g_free(targets);

    return self;
}

static VALUE
rg_m_clear_targets(VALUE self, VALUE widget, VALUE selection)
{
    gtk_selection_clear_targets(RVAL2WIDGET(widget), RVAL2ATOM(selection));
    return self;
}

static VALUE
rg_m_convert(G_GNUC_UNUSED VALUE self, VALUE widget, VALUE selection, VALUE target, VALUE time)
{
    gboolean ret = gtk_selection_convert(RVAL2WIDGET(widget), 
                                         RVAL2ATOM(selection), RVAL2ATOM(target),
                                         NUM2INT(time));
    return CBOOL2RVAL(ret);
}

static VALUE
rg_m_remove_all(VALUE self, VALUE widget)
{
    gtk_selection_remove_all(RVAL2WIDGET(widget));
    return self;
}

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
rg_m_include_image_p(G_GNUC_UNUSED VALUE self, VALUE rbtargets, VALUE rbwritable)
{
    gboolean writable = RVAL2CBOOL(rbwritable);
    long n;
    GdkAtom *targets = RVAL2GDKATOMS(rbtargets, &n);
    gboolean result;

    result = gtk_targets_include_image(targets, n, writable);

    g_free(targets);

    return result;
}

static VALUE
rg_m_include_text_p(G_GNUC_UNUSED VALUE self, VALUE rbtargets)
{
    long n;
    GdkAtom *targets = RVAL2GDKATOMS(rbtargets, &n);
    gboolean result;

    result = gtk_targets_include_text(targets, n);

    g_free(targets);

    return result;
}

static VALUE
rg_m_include_uri_p(G_GNUC_UNUSED VALUE self, VALUE rbtargets)
{
    long n;
    GdkAtom *targets = RVAL2GDKATOMS(rbtargets, &n);
    gboolean result;

    result = gtk_targets_include_uri(targets, n);

    g_free(targets);

    return result;
}

static VALUE
rg_m_include_rich_text_p(G_GNUC_UNUSED VALUE self, VALUE rbtargets, VALUE rbbuffer)
{
    GtkTextBuffer *buffer = GTK_TEXT_BUFFER(RVAL2GOBJ(rbbuffer));
    long n;
    GdkAtom *targets = RVAL2GDKATOMS(rbtargets, &n);
    gboolean result;

    result = gtk_targets_include_rich_text(targets, n, buffer);

    g_free(targets);

    return result;
}
#endif

void
Init_gtk_selection(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE =  rb_define_module_under(mGtk, "Selection");

    RG_DEF_MODFUNC(owner_set, 3);
    RG_DEF_MODFUNC(add_target, 4);
    RG_DEF_MODFUNC(add_targets, 3);
    RG_DEF_MODFUNC(clear_targets, 2);
    RG_DEF_MODFUNC(convert, 4);
    RG_DEF_MODFUNC(remove_all, 1);

#if GTK_CHECK_VERSION(2,10,0)
    RG_DEF_MODFUNC_P(include_image, 2);
    RG_DEF_MODFUNC_P(include_text, 1);
    RG_DEF_MODFUNC_P(include_uri, 1);
    RG_DEF_MODFUNC_P(include_rich_text, 2);
#endif
}
