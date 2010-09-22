/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcellrenderer.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:31 $

  Copyright (C) 2002-2005 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(s) (GTK_CELL_RENDERER(RVAL2GOBJ(s)))
#define RVAL2RECT(r) ((GdkRectangle*)RVAL2BOXED(r, GDK_TYPE_RECTANGLE))
#define RECT2RVAL(r) (BOXED2RVAL(r, GDK_TYPE_RECTANGLE))

static VALUE
cellrenderer_get_size(self, widget, cell_area)
    VALUE self, widget, cell_area;
{
    GdkRectangle ret;

    gtk_cell_renderer_get_size(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)),
                               RVAL2RECT(cell_area),
                               &ret.x, &ret.y, &ret.width, &ret.height);
    return RECT2RVAL(&ret);
}

static VALUE
cellrenderer_render(self, window, widget, background_area, cell_area, 
                    expose_area, flags)
    VALUE self, window, widget, background_area, cell_area, expose_area, flags;
{
    gtk_cell_renderer_render(_SELF(self), GDK_WINDOW(RVAL2GOBJ(window)),
                             GTK_WIDGET(RVAL2GOBJ(widget)),
                             RVAL2RECT(background_area),
                             RVAL2RECT(cell_area),
                             RVAL2RECT(expose_area),
                             RVAL2GFLAGS(flags, GTK_TYPE_CELL_RENDERER_STATE));
    return self;
}

static VALUE
cellrenderer_activate(self, event, widget, path, background_area, 
                      cell_area, flags)
    VALUE self, event, widget, path, background_area, cell_area, flags;
{
    gboolean ret =
    gtk_cell_renderer_activate(_SELF(self), (GdkEvent*)RVAL2GEV(event),
                               GTK_WIDGET(RVAL2GOBJ(widget)),
                               RVAL2CSTR(path), RVAL2RECT(background_area),
                               RVAL2RECT(cell_area), 
                               RVAL2GFLAGS(flags, GTK_TYPE_CELL_RENDERER_STATE));
    return CBOOL2RVAL(ret);
}

static VALUE
cellrenderer_start_editing(self, event, widget, path, background_area, 
                           cell_area, flags)
    VALUE self, event, widget, path, background_area, cell_area, flags;
{
    GtkCellEditable* edit =
    gtk_cell_renderer_start_editing(_SELF(self), (GdkEvent*)RVAL2GEV(event),
                               GTK_WIDGET(RVAL2GOBJ(widget)),
                               RVAL2CSTR(path), RVAL2RECT(background_area),
                               RVAL2RECT(cell_area), 
                               RVAL2GFLAGS(flags, GTK_TYPE_CELL_RENDERER_STATE));
    return edit ? GOBJ2RVAL(edit) : Qnil;
}

#if GTK_CHECK_VERSION(2,4,0)
#ifndef GTK_DISABLE_DEPRECATED
static VALUE
cellrenderer_editing_canceled(self)
    VALUE self;
{
    gtk_cell_renderer_editing_canceled(_SELF(self));
    return self;
}
#endif
#endif

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
cellrenderer_stop_editing(self, canceled)
    VALUE self, canceled;
{
    gtk_cell_renderer_stop_editing(_SELF(self), RVAL2CBOOL(canceled));
    return self;
}
#endif

static VALUE
cellrenderer_get_fixed_size(self)
    VALUE self;
{
    int width, height;
    gtk_cell_renderer_get_fixed_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
cellrenderer_set_fixed_size(self, width, height)
    VALUE self, width, height;
{
    gtk_cell_renderer_set_fixed_size(_SELF(self), NUM2INT(width), 
                                     NUM2INT(height));
    return self;
}

void
Init_gtk_cellrenderer()
{
    VALUE renderer = G_DEF_CLASS(GTK_TYPE_CELL_RENDERER, "CellRenderer", mGtk);
    
    rb_define_method(renderer, "get_size", cellrenderer_get_size, 2);
    rb_define_method(renderer, "render", cellrenderer_render, 6);
    rb_define_method(renderer, "activate", cellrenderer_activate, 6);
    rb_define_method(renderer, "start_editing", cellrenderer_start_editing, 6);
#if GTK_CHECK_VERSION(2,4,0)
#ifndef GTK_DISABLE_DEPRECATED
    rb_define_method(renderer, "editing_canceled", cellrenderer_editing_canceled, 0);
#endif
#endif
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(renderer, "stop_editing", cellrenderer_stop_editing, 1);
#endif
    rb_define_method(renderer, "fixed_size", cellrenderer_get_fixed_size, 0);
    rb_define_method(renderer, "set_fixed_size", cellrenderer_set_fixed_size, 2);

    /* GtkCellRendererState */
    G_DEF_CLASS(GTK_TYPE_CELL_RENDERER_STATE, "State", renderer);
    G_DEF_CONSTANTS(renderer, GTK_TYPE_CELL_RENDERER_STATE, "GTK_CELL_RENDERER_");

    /* GtkCellRendererMode */
    G_DEF_CLASS(GTK_TYPE_CELL_RENDERER_MODE, "Mode", renderer);
    G_DEF_CONSTANTS(renderer, GTK_TYPE_CELL_RENDERER_MODE, "GTK_CELL_RENDERER_");
}
