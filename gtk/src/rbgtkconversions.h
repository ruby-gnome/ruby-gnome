/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkconversions.h -

  Copyright (C) 2008 Ruby-GNOME2 Project Team
************************************************/

#ifndef _RBGTKCONVERSIONS_H
#define _RBGTKCONVERSIONS_H

#include <rbgobject.h>
#include <gtk/gtk.h>

#define GTKTREEPATH2RVAL(t) (BOXED2RVAL(t, GTK_TYPE_TREE_PATH))
#define RVAL2GTKTREEPATH(t) ((GtkTreePath *)RVAL2BOXED(t, GTK_TYPE_TREE_PATH))
#define GTKTREEITER2RVAL(i) (BOXED2RVAL(i, GTK_TYPE_TREE_ITER))
#define RVAL2GTKTREEITER(i) ((GtkTreeIter *)RVAL2BOXED(i, GTK_TYPE_TREE_ITER))

#endif /* _RBGTKCONVERSIONS_H */
