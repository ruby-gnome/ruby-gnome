/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgnomecanvas.h -

  $Author: mutoh $
  $Date: 2005/10/15 09:30:53 $

  Copyright (C) 2004,2005  Ruby-GNOME2 Project Team
  Copyright (C) 2002 KUBO Takehiro 
************************************************/

#ifndef _RBGNOMECANVAS_H
#define _RBGNOMECANVAS_H 1
#include "ruby.h"
#include "rbgtk.h"
#include <libgnomecanvas/libgnomecanvas.h>
#include <libgnomecanvas/gnome-canvas-clipgroup.h>
#include "rbgnomecanvasversion.h"

extern void Init_gnome_canvas(VALUE);
extern void Init_gnome_canvas_util(VALUE);
extern void Init_gnome_canvas_item(VALUE);
extern void Init_gnome_canvas_rich_text(VALUE);
extern void Init_gnome_canvas_path_def(VALUE);
extern void Init_gnome_canvas_buf(VALUE);

extern GType rbgno_canvas_path_def_get_type(void);

extern GType gnome_canvas_buf_get_type(void);

#ifndef HAVE_GNOME_CANVAS_PATH_DEF_GET_TYPE
#define GNOME_TYPE_CANVAS_PATH_DEF (rbgno_canvas_path_def_get_type())
#endif

#define GNOME_TYPE_CANVAS_BUF (gnome_canvas_buf_get_type())

#endif /* _RBGNOMECANVAS_H */
