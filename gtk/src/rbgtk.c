/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtk.c -

  $Author: mutoh $
  $Date: 2002/05/19 13:59:10 $

  Copyright (C) 1998-2001 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"
#include "st.h"
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif

static const char* const ruby_object_key = "__ruby_gtk_object__";

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
VALUE gCList;
VALUE gCTree;
VALUE gCTreeNode;
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
VALUE gList;
VALUE gItem;
VALUE gListItem;
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
VALUE gPixmap;
VALUE gPreview;
VALUE gProgress;
VALUE gProgressBar;
VALUE gScrolledWin;
VALUE gStatusBar;
VALUE gTable;
VALUE gText;
VALUE gToolbar;
VALUE gTooltips;
VALUE gTipsQuery;
VALUE gTree;
VALUE gTreeItem;
VALUE gViewport;
VALUE gPlug;
VALUE gSocket;

VALUE gAccelGroup;
VALUE gAcceleratorTable;
VALUE gStyle;
VALUE gRcStyle;
VALUE gPreviewInfo;
VALUE gAllocation;
VALUE gRequisition;
VALUE gItemFactory;
VALUE gIFConst;
VALUE gFontSelection;
VALUE gFontSelectionDialog;
VALUE gSelectionData;

VALUE mRC;

static st_table *gtk_object_list;
static VALUE gtk_object_list_v;
static VALUE gtk_type_hash;

ID id_gtkdata;
ID id_relatives;
ID id_relative_callbacks;
ID id_call;
ID id_class_info;

void
rbgtk_register_class(cinfo)
    rbgtk_class_info *cinfo;
{
    VALUE data = Data_Wrap_Struct(rb_cData, 0, 0, cinfo);
    rb_ivar_set(cinfo->klass, id_class_info, data);
    rb_hash_aset(gtk_type_hash, INT2NUM(cinfo->gtype), cinfo->klass);
}

rbgtk_class_info *
rbgtk_lookup_class(klass)
    VALUE klass;
{
    rbgtk_class_info *cinfo = NULL;
    if (RTEST(rb_ivar_defined(klass, id_class_info))) {
	VALUE data = rb_ivar_get(klass, id_class_info);
	if (!NIL_P(data))
	    Data_Get_Struct(data, rbgtk_class_info, cinfo);
    }
    return cinfo;
}

rbgtk_class_info *
rbgtk_lookup_class_by_gtype(gtype)
    GtkType gtype;
{
    VALUE klass = rb_hash_aref(gtk_type_hash, INT2NUM(gtype));

    if (NIL_P(klass))
        return NULL;
    else
        return rbgtk_lookup_class(klass);
}

static void
gobj_mark(obj)
    GtkObject *obj;
{
    /* just for type mark */
}

GtkObject*
get_gobject(obj)
    VALUE obj;
{
    struct RData *data;
    GtkObject *gtkp;

    if (NIL_P(obj)) { 
	rb_raise(rb_eTypeError, "wrong argument type nil");
    }

    Check_Type(obj, T_OBJECT);
    data = RDATA(rb_ivar_get(obj, id_gtkdata));
    /* if (NIL_P(data) || data->dmark != gobj_mark) { */
    if (NIL_P(data)) {
	rb_raise(rb_eTypeError, "not a Gtk object");
    }
    Data_Get_Struct(data, GtkObject, gtkp);
    if (!gtkp) {
	rb_raise(rb_eArgError, "destroyed GtkObject");
    }
    if (!GTK_IS_OBJECT(gtkp)) {
	rb_raise(rb_eTypeError, "not a GtkObject");
    }

    return gtkp;
}

static void
delete_gobject(gtkobj, obj)
    GtkObject *gtkobj;
    VALUE obj;
{
    struct RData *data;

    if (!st_delete(gtk_object_list, (char**)&obj, 0))
        rb_bug("ruby-gtk: already freed object is freed again");
    data = RDATA(rb_ivar_get(obj, id_gtkdata));
    data->dfree = 0;
    data->data = 0;
}

void
set_gobject(obj, gtkobj)
    VALUE obj;
    GtkObject *gtkobj;
{
    VALUE data;

    rbgtk_class_info *cinfo = rbgtk_lookup_class(rb_class_of(obj));
    if (cinfo)
	data = Data_Wrap_Struct(rb_cData, cinfo->mark, cinfo->free, gtkobj);
    else
	data = Data_Wrap_Struct(rb_cData, gobj_mark, 0, gtkobj);
    gtk_object_set_data(gtkobj, ruby_object_key, (gpointer)obj);

    rb_ivar_set(obj, id_relatives, Qnil);

    rb_ivar_set(obj, id_gtkdata, data);
    gtk_signal_connect(gtkobj, "destroy",
		       (GtkSignalFunc)delete_gobject, (gpointer)obj);
    st_add_direct(gtk_object_list, (char*)obj, (char*)obj);
}

GtkWidget*
get_widget(obj)
    VALUE obj;
{
    GtkObject *data = get_gobject(obj);

    return GTK_WIDGET(data);
}

#if 0 /* FIXME: where is this function called from? -- yashi Feb 28, 2000*/
static GtkWidget*
force_get_widget(obj)
    VALUE obj;
{
    GtkObject *data = force_get_gobject(obj);

    return GTK_WIDGET(data);
}
#endif

VALUE
get_value_from_gobject(obj)
    GtkObject *obj;
{
    VALUE  ret;
    ret = (VALUE)gtk_object_get_data(obj, ruby_object_key);
    if ( ! ret )
	ret = make_gobject_auto_type(obj);
    return ret;
}

void
add_relative(obj, relative)
    VALUE obj, relative;
{
    VALUE ary = rb_ivar_get(obj, id_relatives);

    if (NIL_P(ary) || TYPE(ary) != T_ARRAY) {
	ary = rb_ary_new();
	rb_ivar_set(obj, id_relatives, ary);
    }
    rb_ary_push(ary, relative);
}

void add_relative_removable(obj, relative, obj_ivar_id, hash_key)
     VALUE obj, relative, hash_key;
     ID    obj_ivar_id;
{
    VALUE hash = rb_ivar_get(obj, obj_ivar_id);

    if (NIL_P(hash) || TYPE(hash) != T_HASH) {
        hash = rb_hash_new();
        rb_ivar_set(obj, obj_ivar_id, hash);
    }
    rb_hash_aset(hash, hash_key, relative);
}

void remove_relative(obj, obj_ivar_id, hash_key)
     VALUE obj, hash_key;
     ID    obj_ivar_id;
{
    VALUE hash = rb_ivar_get(obj, obj_ivar_id);

    if (NIL_P(hash) || TYPE(hash) != T_HASH) {
        /* should not happen. */
    } else {
        rb_funcall(hash, rb_intern("delete"), 1, hash_key);
    }
}

VALUE
make_gobject(klass, gtkobj)
    VALUE klass;
    GtkObject *gtkobj;
{
    VALUE obj = rb_obj_alloc(klass);

    set_gobject(obj, gtkobj);
    return obj;
}

void
set_widget(obj, widget)
    VALUE obj;
    GtkWidget *widget;
{
    set_gobject(obj, GTK_OBJECT(widget));
}

VALUE
make_widget(klass, widget)
    VALUE klass;
    GtkWidget *widget;
{
    return make_gobject(klass, GTK_OBJECT(widget));
}

VALUE
get_gtk_type(gtkobj)
    GtkObject *gtkobj;
{
    VALUE klass = 0;
    GtkType gtype;

    for (gtype = GTK_OBJECT_TYPE(gtkobj);
         gtype != GTK_TYPE_INVALID;
         gtype = gtk_type_parent(gtype))
    {
        VALUE klass = rb_hash_aref(gtk_type_hash, INT2NUM(gtype));
        if (!NIL_P(klass))
            return klass;
    }

    if GTK_IS_ITEM_FACTORY(gtkobj) klass = gItemFactory;
    else if GTK_IS_HSEPARATOR(gtkobj) klass = gHSeparator;
    else if GTK_IS_VSEPARATOR(gtkobj) klass = gVSeparator;
    else if GTK_IS_SEPARATOR(gtkobj) klass = gSeparator;
    else if GTK_IS_HRULER(gtkobj) klass = gHRuler;
    else if GTK_IS_VRULER(gtkobj) klass = gVRuler;
    else if GTK_IS_RULER(gtkobj) klass = gRuler;
    else if GTK_IS_HSCROLLBAR(gtkobj) klass = gHScrollbar;
    else if GTK_IS_VSCROLLBAR(gtkobj) klass = gVScrollbar;
    else if GTK_IS_SCROLLBAR(gtkobj) klass = gScrollbar;
    else if GTK_IS_HSCALE(gtkobj) klass = gHScale;
    else if GTK_IS_VSCALE(gtkobj) klass = gVScale;
    else if GTK_IS_SCALE(gtkobj) klass = gScale;
    else if GTK_IS_RANGE(gtkobj) klass = gRange;
    else if GTK_IS_PROGRESS_BAR(gtkobj) klass = gProgressBar;
    else if GTK_IS_PROGRESS(gtkobj) klass = gProgress;
    else if GTK_IS_PREVIEW(gtkobj) klass = gPreview;
    else if GTK_IS_PIXMAP(gtkobj) klass = gPixmap;
    else if GTK_IS_TIPS_QUERY(gtkobj) klass = gTipsQuery;
    else if GTK_IS_ACCEL_LABEL(gtkobj) klass = gAccelLabel;
    else if GTK_IS_LABEL(gtkobj) klass = gLabel;
    else if GTK_IS_LAYOUT(gtkobj) klass = gLayout;
    else if GTK_IS_IMAGE(gtkobj) klass = gImage;
    else if GTK_IS_ARROW(gtkobj) klass = gArrow;
    else if GTK_IS_MISC(gtkobj) klass = gMisc;
    else if GTK_IS_TEXT(gtkobj) klass = gText;
    else if GTK_IS_SPIN_BUTTON(gtkobj) klass = gSButton;
    else if GTK_IS_ENTRY(gtkobj) klass = gEntry;
    else if GTK_IS_EDITABLE(gtkobj) klass = gEditable;
    else if GTK_IS_CURVE(gtkobj) klass = gCurve;
    else if GTK_IS_DRAWING_AREA(gtkobj) klass = gDrawArea;
    else if GTK_IS_TREE(gtkobj) klass = gTree;
    else if GTK_IS_TOOLBAR(gtkobj) klass = gToolbar;
    else if GTK_IS_TABLE(gtkobj) klass = gTable;
    else if GTK_IS_SCROLLED_WINDOW(gtkobj) klass = gScrolledWin;
    else if GTK_IS_VPANED(gtkobj) klass = gVPaned;
    else if GTK_IS_HPANED(gtkobj) klass = gHPaned;
    else if GTK_IS_PANED(gtkobj) klass = gPaned;
    else if GTK_IS_NOTEBOOK(gtkobj) klass = gNotebook;
    else if GTK_IS_MENU_BAR(gtkobj) klass = gMenuBar;
    else if GTK_IS_MENU(gtkobj) klass = gMenu;
    else if GTK_IS_MENU_SHELL(gtkobj) klass = gMenuShell;
    else if GTK_IS_LIST(gtkobj) klass = gList;
    else if GTK_IS_FIXED(gtkobj) klass = gFixed;
    else if GTK_IS_CTREE(gtkobj) klass = gCTree;
    else if GTK_IS_CLIST(gtkobj) klass = gCList;
    else if GTK_IS_RADIO_BUTTON(gtkobj) klass = gRButton;
    else if GTK_IS_CHECK_BUTTON(gtkobj) klass = gCButton;
    else if GTK_IS_TOGGLE_BUTTON(gtkobj) klass = gTButton;
    else if GTK_IS_OPTION_MENU(gtkobj) klass = gOptionMenu;
    else if GTK_IS_BUTTON(gtkobj) klass = gButton;
    else if GTK_IS_GAMMA_CURVE(gtkobj) klass = gGamma;
    else if GTK_IS_COLOR_SELECTION(gtkobj) klass = gColorSel;
    else if GTK_IS_VBOX(gtkobj) klass = gVBox;
    else if GTK_IS_STATUSBAR(gtkobj) klass = gStatusBar;
    else if GTK_IS_COMBO(gtkobj) klass = gCombo;
    else if GTK_IS_HBOX(gtkobj) klass = gHBox;
    else if GTK_IS_HBUTTON_BOX(gtkobj) klass = gHBBox;
    else if GTK_IS_VBUTTON_BOX(gtkobj) klass = gVBBox;
    else if GTK_IS_BUTTON_BOX(gtkobj) klass = gBBox;
    else if GTK_IS_BOX(gtkobj) klass = gBox;
    else if GTK_IS_FILE_SELECTION(gtkobj) klass = gFileSel;
    else if GTK_IS_FONT_SELECTION(gtkobj) klass = gFontSelection;
    else if GTK_IS_FONT_SELECTION_DIALOG(gtkobj) klass = gFontSelectionDialog;
    else if GTK_IS_INPUT_DIALOG(gtkobj) klass = gInputDialog;
    else if GTK_IS_DIALOG(gtkobj) klass = gDialog;
    else if GTK_IS_COLOR_SELECTION_DIALOG(gtkobj) klass = gColorSelDialog;
    else if GTK_IS_WINDOW(gtkobj) klass = gWindow;
    else if GTK_IS_VIEWPORT(gtkobj) klass = gViewport;
    else if GTK_IS_TREE_ITEM(gtkobj) klass = gTreeItem;
    else if GTK_IS_TEAROFF_MENU_ITEM(gtkobj) klass = gTMenuItem;
    else if GTK_IS_RADIO_MENU_ITEM(gtkobj) klass = gRMenuItem;
    else if GTK_IS_CHECK_MENU_ITEM(gtkobj) klass = gCMenuItem;
    else if GTK_IS_MENU_ITEM(gtkobj) klass = gMenuItem;
    else if GTK_IS_LIST_ITEM(gtkobj) klass = gListItem;
    else if GTK_IS_ITEM(gtkobj) klass = gItem;
    else if GTK_IS_ASPECT_FRAME(gtkobj) klass = gAspectFrame;
    else if GTK_IS_EVENT_BOX(gtkobj) klass = gEventBox;
    else if GTK_IS_ALIGNMENT(gtkobj) klass = gAlignment;
    else if GTK_IS_HANDLE_BOX(gtkobj) klass = gHandleBox;
    else if GTK_IS_CONTAINER(gtkobj) klass = gContainer;
    else if GTK_IS_CALENDAR(gtkobj) klass = gCalendar;
    else if GTK_IS_WIDGET(gtkobj) klass = gWidget;
    else if GTK_IS_ADJUSTMENT(gtkobj) klass = gAdjustment;
    else if GTK_IS_TOOLTIPS(gtkobj) klass = gTooltips;
#if GTK_MAJOR_VERSION < 2
    else if GTK_IS_DATA(gtkobj) klass = gData;
#endif
    else if GTK_IS_OBJECT(gtkobj) klass = gObject;
    else {
	rb_raise(rb_eTypeError, "not a Gtk object");
    }
    return klass;
}

VALUE
make_gobject_auto_type(gtkobj)
    GtkObject *gtkobj;
{
    return make_gobject(get_gtk_type(gtkobj), gtkobj);
}

VALUE
make_gstyle(style)
    GtkStyle *style;
{
    gtk_style_ref(style);
    return Data_Wrap_Struct(gStyle, 0, gtk_style_unref, style);
}

GtkStyle*
get_gstyle(style)
    VALUE style;
{
    GtkStyle *gstyle;

    if (NIL_P(style)) return NULL;
    if (!rb_obj_is_instance_of(style, gStyle)) {
	rb_raise(rb_eTypeError, "not a GtkStyle");
    }
    Data_Get_Struct(style, GtkStyle, gstyle);

    return gstyle;
}

#if !defined _WIN32
VALUE
make_grcstyle(style)
    GtkRcStyle *style;
{
    gtk_rc_style_ref(style);
    return Data_Wrap_Struct(gRcStyle, 0, gtk_rc_style_unref, style);
}
#endif

GtkRcStyle*
get_grcstyle(style)
    VALUE style;
{
    GtkRcStyle *gstyle;

    if (NIL_P(style)) return NULL;
    if (!rb_obj_is_instance_of(style, gRcStyle)) {
	rb_raise(rb_eTypeError, "not a GtkRcStyle");
    }
    Data_Get_Struct(style, GtkRcStyle, gstyle);

    return gstyle;
}

VALUE
make_gtkaccelgrp(accel)
    GtkAccelGroup *accel;
{
    gtk_accel_group_ref(accel);
    return Data_Wrap_Struct(gAccelGroup,
                            0,
                            gtk_accel_group_unref,
			    accel);
}

GtkAccelGroup*
get_gtkaccelgrp(value)
    VALUE value;
{
    GtkAccelGroup *accel;

    if (NIL_P(value)) return NULL;
    if (!rb_obj_is_instance_of(value, gAccelGroup)) {
        rb_raise(rb_eTypeError, "not a GtkAccelGroup");
    }
    Data_Get_Struct(value, GtkAccelGroup, accel);

    return accel;
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

void
exec_callback(widget, proc)
    GtkWidget *widget;
    gpointer proc;
{
    rb_funcall((VALUE)proc, id_call, 1,
	       get_value_from_gobject(GTK_OBJECT(widget)));
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

static
void remove_idle(VALUE arg)
{
    gtk_idle_remove(NUM2UINT(arg));
}

#endif /* !USE_POLL_FUNC */
 
/*
 * Init
 */
void Init_gtk_gtk()
{
    gtk_object_list_v = Qnil;
    rb_global_variable(&gtk_object_list_v);
    gtk_object_list = st_init_numtable();
    gtk_object_list_v = Data_Wrap_Struct(rb_cObject,
                                         rb_mark_tbl, st_free_table,
                                         gtk_object_list);
    rb_global_variable(&gtk_type_hash);
    gtk_type_hash = rb_hash_new();

    /* IDs */
    id_gtkdata = rb_intern("gtkdata");
    id_relatives = rb_intern("relatives");
    id_relative_callbacks = rb_intern("relative_callbacks");
    
    id_call = rb_intern("call");
    id_class_info = rb_intern("gtk_class_info");

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

    Init_gtk_object();

    Init_gtk_accel_group();
    gRcStyle = rb_define_class_under(mGtk, "RcStyle", rb_cData);
    gPreviewInfo = rb_define_class_under(mGtk, "PreviewInfo", rb_cData);
    Init_gtk_requisiton();
    Init_gtk_allocation();

#ifdef USE_POLL_FUNC
    g_main_set_poll_func(rbgtk_poll);
#else
    rb_set_end_proc(&idle_remove, UINT2NUM(gtk_idle_add((GtkFunction)idle, 0)));
#endif
}
