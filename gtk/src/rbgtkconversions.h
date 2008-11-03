/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkconversions.h -

  Copyright (C) 2008 Ruby-GNOME2 Project Team
************************************************/

#ifndef _RBGTKCONVERSIONS_H
#define _RBGTKCONVERSIONS_H

#include <rbgobject.h>
#include <gtk/gtk.h>

#include <rbgdkconversions.h>

#define GTKTREEPATH2RVAL(t) (BOXED2RVAL(t, GTK_TYPE_TREE_PATH))
#define RVAL2GTKTREEPATH(t) ((GtkTreePath *)RVAL2BOXED(t, GTK_TYPE_TREE_PATH))
#define GTKTREEITER2RVAL(i) (BOXED2RVAL(i, GTK_TYPE_TREE_ITER))
#define RVAL2GTKTREEITER(i) ((GtkTreeIter *)RVAL2BOXED(i, GTK_TYPE_TREE_ITER))

#define RVAL2GTKSTATETYPE(type) (RVAL2GENUM(type, GTK_TYPE_STATE_TYPE))
#define GTKRCFLAGS2RVAL(flags) (GFLAGS2RVAL(flags, GTK_TYPE_RC_FLAGS))
#define RVAL2GTKRCFLAGS(flags) (RVAL2GFLAGS(flags, GTK_TYPE_RC_FLAGS))

#endif /* _RBGTKCONVERSIONS_H */
