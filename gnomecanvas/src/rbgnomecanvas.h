/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgnomecanvas.h -

  $Author: tkubo $
  $Date: 2002/09/21 14:53:20 $

  Copyright (C) 2002 KUBO Takehiro 
************************************************/

#ifndef _RBGNOMECANVAS_H
#define _RBGNOMECANVAS_H 1
#include "ruby.h"
#include "rbgtk.h"
#include <libgnomecanvas/libgnomecanvas.h>

extern void Init_gnome_canvas(VALUE);
extern void Init_gnome_canvas_util(VALUE);
extern void Init_gnome_canvas_item(VALUE);
extern void Init_gnome_canvas_group(VALUE);

extern void rbgnomecanvas_do_item_set(int, VALUE *, VALUE);
#endif /* _RBGNOMECANVAS_H */
