/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtk.c -

  $Author: mutoh $
  $Date: 2002/07/06 20:56:15 $

  Copyright (C) 1998-2001 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"
#include "st.h"
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif

VALUE mGtk;
VALUE gError;
VALUE gObject;
VALUE gWidget;
VALUE gContainer;
VALUE gBin;
VALUE gAlignment;
VALUE gMisc;
VALUE gArrow;
VALUE gFrame;
VALUE gAspectFrame;
VALUE gData;
VALUE gAdjustment;
VALUE gBox;
VALUE gButton;
VALUE gTButton;
VALUE gCButton;
VALUE gRButton;
VALUE gBBox;
VALUE gWindow;
VALUE gDialog;
VALUE gFileSel;
VALUE gVBox;
VALUE gColorSel;
VALUE gColorSelDialog;
VALUE gCombo;
VALUE gImage;
VALUE gDrawArea;
VALUE gEditable;
VALUE gEntry;
VALUE gSButton;
VALUE gEventBox;
VALUE gFixed;
VALUE gGamma;
VALUE gCurve;
VALUE gHBBox;
VALUE gVBBox;
VALUE gHBox;
VALUE gPaned;
VALUE gHPaned;
VALUE gVPaned;
VALUE gRuler;
VALUE gHRuler;
VALUE gVRuler;
VALUE gRange;
VALUE gScale;
VALUE gHScale;
VALUE gVScale;
VALUE gScrollbar;
VALUE gHScrollbar;
VALUE gVScrollbar;
VALUE gSeparator;
VALUE gHSeparator;
VALUE gVSeparator;
VALUE gInputDialog;
VALUE gLabel;
VALUE gItem;
VALUE gMenuShell;
VALUE gMenu;
VALUE gMenuBar;
VALUE gMenuItem;
VALUE gCMenuItem;
VALUE gRMenuItem;
VALUE gTMenuItem;
VALUE gNotebook;
VALUE gNotePage;
VALUE gOptionMenu;
VALUE gProgressBar;
VALUE gScrolledWin;
VALUE gStatusBar;
VALUE gTable;
VALUE gToolbar;
VALUE gTooltips;
VALUE gViewport;
VALUE gPlug;
VALUE gSocket;

VALUE gAccelGroup;
VALUE gAcceleratorTable;
VALUE gStyle;
VALUE gRcStyle;
VALUE gAllocation;
VALUE gRequisition;
VALUE gItemFactory;
VALUE gIFConst;
VALUE gFontSelection;
VALUE gFontSelectionDialog;
VALUE gSelectionData;

#ifndef GTK_DISABLE_DEPRECATED
VALUE gCList;
VALUE gCTree;
VALUE gCTreeNode;
VALUE gList;
VALUE gListItem;
VALUE gPixmap;
VALUE gProgress;
VALUE gPreview;
VALUE gPreviewInfo;
VALUE gTipsQuery;
#endif

#ifdef GTK_ENABLE_BROKEN
VALUE gText;
VALUE gTree;
VALUE gTreeItem;
#endif

VALUE mRC;

ID id_call;

void
rbgtk_register_class(cinfo)
    RGObjClassInfo *cinfo;
{
    rbgobj_register_class(cinfo);
}

void
rbgtk_initialize_gtkobject(obj, gtkobj)
    VALUE obj;
    GtkObject *gtkobj;
{
	gtkobj = gtk_object_ref(gtkobj);
    gtk_object_sink(gtkobj);
    RBGOBJ_INITIALIZE(obj, gtkobj);
}

void
add_relative(obj, relative)
    VALUE obj, relative;
{
    rbgobj_add_relative(obj, relative);
}

void 
add_relative_removable(obj, relative, obj_ivar_id, hash_key)
	VALUE obj, relative, hash_key;
	ID    obj_ivar_id;
{
    rbgobj_add_relative_removable(obj, relative, obj_ivar_id, hash_key);
}

void 
remove_relative(obj, obj_ivar_id, hash_key)
	VALUE obj, hash_key;
	ID    obj_ivar_id;
{
    rbgobj_remove_relative(obj, obj_ivar_id, hash_key);
}

VALUE
make_gtkselectiondata(selectiondata)
    GtkSelectionData *selectiondata;
{
    return Data_Wrap_Struct(gSelectionData, 0, 0, selectiondata);
}

GtkSelectionData*
get_gtkselectiondata(value)
    VALUE value;
{
    GtkSelectionData *selectiondata;

    if (NIL_P(value)) return NULL;
    if (!rb_obj_is_instance_of(value, gSelectionData)) {
        rb_raise(rb_eTypeError, "not a GtkSelectionData");
    }
    Data_Get_Struct(value, GtkSelectionData, selectiondata);

    return selectiondata;
}

#ifndef GTK_DISABLE_DEPRECATED
VALUE
make_gtkprevinfo(info)
    GtkPreviewInfo *info;
{
    return Data_Wrap_Struct(gPreviewInfo, 0, 0, info);
}

GtkPreviewInfo*
get_gtkprevinfo(value)
    VALUE value;
{
    GtkPreviewInfo *info;

    if (NIL_P(value)) return NULL;

    if (!rb_obj_is_instance_of(value, gPreviewInfo)) {
		rb_raise(rb_eTypeError, "not a PreviewInfo");
    }
    Data_Get_Struct(value, GtkPreviewInfo, info);

    return info;
}
#endif

void
exec_callback(widget, proc)
    GtkWidget *widget;
    gpointer proc;
{
    rb_funcall((VALUE)proc, id_call, 1, GOBJ2RVAL(widget));
}

/* 
 * Allocation
 */
static VALUE
gallocation_new(self, x, y, w, h)
    VALUE self, x, y, w, h;
{
    GtkAllocation a;

    a.x = NUM2INT(x);
    a.y = NUM2INT(y);
    a.width = NUM2INT(w);
    a.height = NUM2INT(h);
    return make_gallocation(&a);
}

static VALUE
gallocation_x(self)
    VALUE self;
{
    return INT2NUM(get_gallocation(self)->x);
}

static VALUE
gallocation_y(self)
    VALUE self;
{
    return INT2NUM(get_gallocation(self)->y);
}

static VALUE
gallocation_w(self)
    VALUE self;
{
    return INT2NUM(get_gallocation(self)->width);
}

static VALUE
gallocation_h(self)
    VALUE self;
{
    return INT2NUM(get_gallocation(self)->height);
}

static VALUE
gallocation_to_a(self)
    VALUE self;
{
    GtkAllocation *a;

    a = get_gallocation(self);
    return rb_ary_new3(4, INT2FIX(a->x), INT2FIX(a->y),
					   INT2FIX(a->width), INT2FIX(a->height));
}

static VALUE
gallocation_to_s(self)
    VALUE self;
{
    char str[2 +2*3 +5*4  +1]; /* member is guint16. max string size is 5 */
    GtkAllocation *a;

    a = get_gallocation(self);
    sprintf(str, "(%5d, %5d, %5d, %5d)", a->x, a->y, a->width, a->height);
    return rb_str_new2(str);
}

void Init_gtk_allocation()
{
    gAllocation = rb_define_class_under(mGtk, "Allocation", rb_cData);

    rb_define_singleton_method(gAllocation, "new", gallocation_new, 4);
    rb_define_method(gAllocation, "x", gallocation_x, 0);
    rb_define_method(gAllocation, "y", gallocation_y, 0);
    rb_define_method(gAllocation, "width", gallocation_w, 0);
    rb_define_method(gAllocation, "height", gallocation_h, 0);
    rb_define_method(gAllocation, "to_a", gallocation_to_a, 0);
    rb_define_method(gAllocation, "to_s", gallocation_to_s, 0);
}

/*
 * Requisition
 */
static VALUE grequisition_new(self, w, h)
    VALUE self, w, h;
{
    GtkRequisition r;

    r.width = NUM2INT(w);
    r.height = NUM2INT(h);
    return make_grequisition(&r);
}

static VALUE
grequisition_w(self)
    VALUE self;
{
    return INT2NUM(get_grequisition(self)->width);
}

static VALUE
grequisition_h(self)
    VALUE self;
{
    return INT2NUM(get_grequisition(self)->height);
}

static VALUE
grequisition_set_w(self, w)
    VALUE self, w;
{
    get_grequisition(self)->width = NUM2INT(w);
    return self;
}

static VALUE
grequisition_set_h(self, h)
    VALUE self, h;
{
    get_grequisition(self)->height = NUM2INT(h);
    return self;
}

static VALUE
grequisition_to_a(self)
    VALUE self;
{
    GtkRequisition *r;

    r = get_grequisition(self);
    return rb_ary_new3(2, INT2FIX(r->width), INT2FIX(r->height));
}

static VALUE
grequisition_to_s(self)
    VALUE self;
{
    char str[2 +2*1 +5*2  +1]; /* member is guint16. max string size is 5 */
    GtkRequisition *r;

    r = get_grequisition(self);
    sprintf(str, "(%5d, %5d)", r->width, r->height);
    return rb_str_new2(str);
}

void Init_gtk_requisiton()
{
    gRequisition = rb_define_class_under(mGtk, "Requisition", rb_cData);

    rb_define_singleton_method(gRequisition, "new", grequisition_new, 2);
    rb_define_method(gRequisition, "width", grequisition_w, 0);
    rb_define_method(gRequisition, "height", grequisition_h, 0);
    rb_define_method(gRequisition, "width=", grequisition_set_w, 1);
    rb_define_method(gRequisition, "height=", grequisition_set_h, 1);
    rb_define_method(gRequisition, "to_a", grequisition_to_a, 0);
    rb_define_method(gRequisition, "to_s", grequisition_to_s, 0);
}


/*
 * Gtk module
 */
#include <version.h>
#if RUBY_VERSION_CODE >= 154
# define USE_POLL_FUNC 1
#else
# undef USE_POLL_FUNC /* rb_thread_select() may cause busy wait */
#endif

#ifdef NATIVE_WIN32
# undef USE_POLL_FUNC /* I don't know whether this works for win32. */
#endif

#ifdef USE_POLL_FUNC

#ifdef HAVE_SYS_SELECT_H
# include <sys/select.h>
#endif

static gint 
rbgtk_poll (GPollFD *fds,
			guint    nfds,
			gint     timeout)
{
    struct timeval tv;
    fd_set rset, wset, xset;
    GPollFD *f;
    int ready;
    int maxfd = 0;

    FD_ZERO (&rset);
    FD_ZERO (&wset);
    FD_ZERO (&xset);

    for (f = fds; f < &fds[nfds]; ++f) {
		if (f->fd >= 0)	{
			if (f->events & G_IO_IN)
				FD_SET (f->fd, &rset);
			if (f->events & G_IO_OUT)
				FD_SET (f->fd, &wset);
			if (f->events & G_IO_PRI)
				FD_SET (f->fd, &xset);
			if (f->fd > maxfd && (f->events & (G_IO_IN|G_IO_OUT|G_IO_PRI)))
				maxfd = f->fd;
		}
    }
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    ready = rb_thread_select (maxfd + 1, &rset, &wset, &xset,
							  timeout == -1 ? NULL : &tv);
    if (ready > 0) {
		for (f = fds; f < &fds[nfds]; ++f) {
			f->revents = 0;
			if (f->fd >= 0) {
				if (FD_ISSET (f->fd, &rset))
					f->revents |= G_IO_IN;
				if (FD_ISSET (f->fd, &wset))
					f->revents |= G_IO_OUT;
				if (FD_ISSET (f->fd, &xset))
					f->revents |= G_IO_PRI;
			}
		}
    }

    return ready;
}

#else /* !USE_POLL_FUNC */

static gint
idle()
{
    struct timeval wait;

    wait.tv_sec  = 0;
    wait.tv_usec = 100000; /* 100ms */

    CHECK_INTS;
    if (!rb_thread_critical) rb_thread_wait_for(wait);

    return Qtrue;
}

#endif /* !USE_POLL_FUNC */
 
/*
 * Init
 */
void Init_gtk_gtk()
{
    id_call = rb_intern("call");

    mGtk = rb_define_module("Gtk");
    rb_ivar_set(mGtk, id_relatives, Qnil);
    rb_ivar_set(mGtk, id_relative_callbacks, Qnil);
    gError = rb_define_class_under(mGtk, "Error", rb_eRuntimeError);
    Init_gtk_const();
    Init_gtk_main();
    Init_gtk_rc();
    Init_gtk_style();
    Init_gtk_selectiondata();
    Init_gtk_drag();
	Init_gtk_windowgroup();
    Init_gtk_object();

    Init_gtk_accel_group();
    gRcStyle = rb_define_class_under(mGtk, "RcStyle", rb_cData);
#ifndef GTK_DISABLE_DEPRECATED
    gPreviewInfo = rb_define_class_under(mGtk, "PreviewInfo", rb_cData);
#endif
    Init_gtk_requisiton();
    Init_gtk_allocation();

#ifdef USE_POLL_FUNC
    g_main_set_poll_func(rbgtk_poll);
#else
    gtk_idle_add((GtkFunction)idle, 0);
#endif
}
