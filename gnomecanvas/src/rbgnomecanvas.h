/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgnomecanvas.h -

  $Author: mutoh $
  $Date: 2004/02/18 16:57:30 $

  Copyright (C) 2002 KUBO Takehiro 
************************************************/

#ifndef _RBGNOMECANVAS_H
#define _RBGNOMECANVAS_H 1
#include "ruby.h"
#include "rbgtk.h"
#include <libgnomecanvas/libgnomecanvas.h>
#include <libgnomecanvas/gnome-canvas-clipgroup.h>

extern void Init_gnome_canvas(VALUE);
extern void Init_gnome_canvas_util(VALUE);
extern void Init_gnome_canvas_item(VALUE);
extern void Init_gnome_canvas_rich_text(VALUE);
extern void Init_gnome_canvas_path_def(VALUE);

extern GType rbgno_canvas_path_def_get_type(void);

#endif /* _RBGNOMECANVAS_H */
