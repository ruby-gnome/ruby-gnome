/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhtmlview.c

  $Author: mutoh $
  $Date: 2004/03/05 16:29:45 $

  Copyright (C) 2003,2004 Masao Mutoh
  Copyright (C) 2003 Martin Povolny
************************************************/
#include "rbgtkhtml.h"
#include <libgtkhtml/view/htmlevent.h>
#include <libgtkhtml/view/htmlselection.h>

#define _SELF(s) (HTML_VIEW(RVAL2GOBJ(s)))

static VALUE
rb_html_view_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE rdoc;
    GtkWidget *widget = NULL;
    HtmlDocument *document = NULL;

    widget = html_view_new ();

    /* check if we were passed a HtmlDocument to use */
    if (rb_scan_args(argc, argv, "01", &rdoc) == 1) {
        html_view_set_document (HTML_VIEW (widget), HTML_DOCUMENT(RVAL2GOBJ(rdoc)));
    } else {
        document = html_document_new ();
        html_view_set_document (HTML_VIEW (widget), document);
    }

    RBGTK_INITIALIZE(self, widget); 
    return Qnil;
}

static VALUE
rb_html_view_set_document(self, document)
    VALUE self;
    VALUE document;
{
    html_view_set_document (_SELF(self), HTML_DOCUMENT(RVAL2GOBJ(document)));
    return self;
}

static VALUE
rb_html_view_jump_to_anchor(self, anchor)
    VALUE self, anchor;
{
    html_view_jump_to_anchor(_SELF(self), RVAL2CSTR(anchor));
    return self;
}

static VALUE
rb_html_view_get_magnification(self)
    VALUE self;
{
    return rb_float_new(html_view_get_magnification(_SELF(self)));
}

static VALUE
rb_html_view_set_magnification(self, magnification)
    VALUE self, magnification;
{
    html_view_set_magnification(_SELF(self), NUM2DBL(magnification));
    return self;
}

static VALUE
rb_html_view_zoom_in(self)
    VALUE self;
{
    html_view_zoom_in(_SELF(self));
    return self;
}

static VALUE
rb_html_view_zoom_out(self)
    VALUE self;
{
    html_view_zoom_out(_SELF(self));
    return self;
}

static VALUE
rb_html_view_zoom_reset(self)
    VALUE self;
{
    html_view_zoom_reset(_SELF(self));
    return self;
}

static VALUE
rb_html_view_find_layout_box(self, node, find_parent)
    VALUE self, node, find_parent;
{
    return GOBJ2RVAL(html_view_find_layout_box(_SELF(self), DOM_NODE(RVAL2GOBJ(node)), RTEST(find_parent)));
}

#ifdef HAVE_HTML_VIEW_SCROLL_TO_NODE
static VALUE
rb_html_view_scroll_to_node(self, node, type)
    VALUE self, node, type;
{
    html_view_scroll_to_node(_SELF(self), DOM_NODE(RVAL2GOBJ(node)), NUM2INT(type));
    return self;
}
#endif

static VALUE
rb_html_view_get_document(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->document);
}

/* from htmlevent.c */
static VALUE
rb_html_event_mouse_move(self, event)
    VALUE self, event;
{
    html_event_mouse_move(_SELF(self), (GdkEventMotion*)RVAL2BOXED(event, GDK_TYPE_EVENT));
    return self;
}

static VALUE
rb_html_event_button_press(self, event)
    VALUE self, event;
{
    html_event_button_press(_SELF(self), (GdkEventButton*)RVAL2BOXED(event, GDK_TYPE_EVENT));
    return self;
}

static VALUE
rb_html_event_button_release(self, event)
    VALUE self, event;
{
    html_event_button_release(_SELF(self), (GdkEventButton*)RVAL2BOXED(event, GDK_TYPE_EVENT));
    return self;
}

static VALUE
rb_html_event_key_press(self, event)
    VALUE self, event;
{
    html_event_key_press(_SELF(self), (GdkEventKey*)RVAL2BOXED(event, GDK_TYPE_EVENT));
    return self;
}

/* from htmlselection.c */
static VALUE
rb_html_selection_start(self, event)
    VALUE self, event;
{
    html_selection_start(_SELF(self), (GdkEventButton*)RVAL2BOXED(event, GDK_TYPE_EVENT));
    return self;
}

static VALUE
rb_html_selection_end(self, event)
    VALUE self, event;
{
    html_selection_end(_SELF(self), (GdkEventButton*)RVAL2BOXED(event, GDK_TYPE_EVENT));
    return self;
}

static VALUE
rb_html_selection_update(self, event)
    VALUE self, event;
{
    html_selection_update(_SELF(self), (GdkEventMotion*)RVAL2BOXED(event, GDK_TYPE_EVENT));
    return self;
}

static VALUE
rb_html_selection_set(self, start_node, offset, len)
    VALUE self, start_node, offset, len;
{
    html_selection_set(_SELF(self), DOM_NODE(RVAL2GOBJ(start_node)), 
                       NUM2INT(offset), NUM2INT(len));
    return self;
}

void 
Init_html_view(mGtkHtml2)
    VALUE mGtkHtml2;
{
    VALUE gHtmlView = G_DEF_CLASS(html_view_get_type(), "HtmlView", mGtkHtml2);

    rb_define_method(gHtmlView, "initialize",  rb_html_view_initialize,  -1);
    rb_define_method(gHtmlView, "set_document", rb_html_view_set_document, 1);
    rb_define_method(gHtmlView, "jump_to_anchor", rb_html_view_jump_to_anchor, 1);
    rb_define_method(gHtmlView, "magnification", rb_html_view_get_magnification, 0);
    rb_define_method(gHtmlView, "set_magnification", rb_html_view_set_magnification, 1);
    rb_define_method(gHtmlView, "zoom_in", rb_html_view_zoom_in, 0);
    rb_define_method(gHtmlView, "zoom_out", rb_html_view_zoom_out, 0);
    rb_define_method(gHtmlView, "zoom_reset", rb_html_view_zoom_reset, 0);
    rb_define_method(gHtmlView, "find_layout_box", rb_html_view_find_layout_box, 2);
#ifdef HAVE_HTML_VIEW_SCROLL_TO_NODE
    rb_define_method(gHtmlView, "scroll_to_node", rb_html_view_scroll_to_node, 2);
#endif
    rb_define_method(gHtmlView, "document", rb_html_view_get_document, 0);

    rb_define_method(gHtmlView, "mouse_move", rb_html_event_mouse_move, 1);
    rb_define_method(gHtmlView, "button_press", rb_html_event_button_press, 1);
    rb_define_method(gHtmlView, "button_release", rb_html_event_button_release, 1);
    rb_define_method(gHtmlView, "key_press", rb_html_event_key_press, 1);

    rb_define_method(gHtmlView, "selection_start",  rb_html_selection_start, 1);
    rb_define_method(gHtmlView, "selection_end",  rb_html_selection_end, 1);
    rb_define_method(gHtmlView, "selection_update",  rb_html_selection_update, 1);
    rb_define_method(gHtmlView, "selection_set",  rb_html_selection_set, 3);

#ifdef HTML_VIEW_SCROLL_TO_TOP
    rb_define_const(gHtmlView, "SCROLL_TO_TOP", INT2NUM(HTML_VIEW_SCROLL_TO_TOP));
#endif
#ifdef HTML_VIEW_SCROLL_TO_BOTTOM
    rb_define_const(gHtmlView, "SCROLL_TO_BOTTOM", INT2NUM(HTML_VIEW_SCROLL_TO_BOTTOM));
#endif
    G_DEF_SETTERS(gHtmlView);
}

