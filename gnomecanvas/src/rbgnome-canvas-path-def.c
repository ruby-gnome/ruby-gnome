/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
#include "rbgnomecanvas.h"
#include <libart_lgpl/art_bpath.h>

static VALUE gnoCanvasPathDef;

#define _SELF(self) (GnomeCanvasPathDef *)RVAL2BOXED(self, rbgno_canvas_path_def_get_type())
#define _WRAP(self) BOXED2RVAL(self, rbgno_canvas_path_def_get_type())

GType
rbgno_canvas_path_def_get_type()
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static ("GnomeCanvasPathDef",
                                                 (GBoxedCopyFunc)gnome_canvas_path_def_ref,
                                                 (GBoxedFreeFunc)gnome_canvas_path_def_unref);
    return our_type;
}

static void
cpathdef_r2g_func(from, to)
    VALUE from;
    GValue* to;
{
    g_value_set_pointer(to, _SELF(from));
}

static VALUE
cpathdef_g2r_func(from)
    const GValue *from;
{
    return _WRAP(g_value_get_pointer(from));
}

static VALUE
cpathdef_s_new(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE length;
    GnomeCanvasPathDef *path;
    rb_scan_args(argc, argv, "01", &length);
    if (NIL_P(length))
        path = gnome_canvas_path_def_new();
    else
        path = gnome_canvas_path_def_new_sized(NUM2INT(length));
    return _WRAP(path);
}

/*
 * Misc constructors
 * All these return NEW path, not unrefing old
 * Also copy and duplicate force bpath to be private (otherwise you
 * would use ref :)
 */
#if 0 /* who needs this method in ruby?. */
void gnome_canvas_path_def_copy (GnomeCanvasPathDef * dst, const GnomeCanvasPathDef * src);
#endif

static VALUE
cpathdef_duplicate(self)
    VALUE self;
{
    return _WRAP(gnome_canvas_path_def_duplicate(_SELF(self)));
}

static VALUE
cpathdef_concat(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    GSList *l = NULL;
	GnomeCanvasPathDef * new;
    int i;

    for (i = 0; i < argc; i++) {
        if (!RTEST(rb_obj_is_instance_of(argv[i], gnoCanvasPathDef)))
            rb_raise(rb_eArgError,
                     "invalid argument type %s (expect Gnome::CanvasPathDef)",
                     rb_class2name(CLASS_OF(argv[i])));
    }
    l = g_slist_append(l, _SELF(self));
    for (i = 0; i < argc; i++) {
        l = g_slist_append(l, _SELF(argv[i]));
    }
    new = gnome_canvas_path_def_concat(l);
    g_slist_free(l);
    return _WRAP(new);
}

static VALUE
cpathdef_split(self)
    VALUE self;
{
    GSList *list = gnome_canvas_path_def_split(_SELF(self));
    GSList *l;
    VALUE ary = rb_ary_new();

    for (l = list;l != NULL;l = g_slist_next(l)) {
        rb_ary_push(ary, _WRAP(l->data));
    }
    g_slist_free(list);
    return ary;
}

static VALUE
cpathdef_open_parts(self)
    VALUE self;
{
    return _WRAP(gnome_canvas_path_def_open_parts(_SELF(self)));
}

static VALUE
cpathdef_closed_parts(self)
    VALUE self;
{
    return _WRAP(gnome_canvas_path_def_closed_parts(_SELF(self)));
}

static VALUE
cpathdef_close_all(self)
    VALUE self;
{
    return _WRAP(gnome_canvas_path_def_close_all(_SELF(self)));
}

/* Methods */

/* Sets GnomeCanvasPathDef to zero length */
static VALUE
cpathdef_reset(self)
    VALUE self;
{
    gnome_canvas_path_def_reset(_SELF(self));
    return self;
}

/* Drawing methods */
static VALUE
cpathdef_moveto(self, x, y)
    VALUE self, x, y;
{
    gnome_canvas_path_def_moveto(_SELF(self), NUM2DBL(x), NUM2DBL(y));
    return self;
}

static VALUE
cpathdef_lineto(self, x, y)
    VALUE self, x, y;
{
    gnome_canvas_path_def_lineto(_SELF(self), NUM2DBL(x), NUM2DBL(y));
    return self;
}

/* Does not create new ArtBpath, but simply changes last lineto position */

static VALUE
cpathdef_lineto_moving(self, x, y)
    VALUE self, x, y;
{
    gnome_canvas_path_def_lineto_moving(_SELF(self), NUM2DBL(x), NUM2DBL(y));
    return self;
}

static VALUE
cpathdef_curveto(self, x0, y0, x1, y1, x2, y2)
    VALUE self, x0, y0, x1, y1, x2, y2;
{
    gnome_canvas_path_def_curveto(_SELF(self),
                                  NUM2DBL(x0), NUM2DBL(y0),
                                  NUM2DBL(x1), NUM2DBL(y1),
                                  NUM2DBL(x2), NUM2DBL(y2));
    return self;
}

static VALUE
cpathdef_closepath(self)
    VALUE self;
{
    gnome_canvas_path_def_closepath(_SELF(self));
    return self;
}

/* Does not draw new line to startpoint, but moves last lineto */

static VALUE
cpathdef_closepath_current(self)
    VALUE self;
{
    gnome_canvas_path_def_closepath_current(_SELF(self));
    return self;
}

/* Various methods */

ArtBpath * gnome_canvas_path_def_bpath (const GnomeCanvasPathDef * path);

static VALUE
cpathdef_length(self)
    VALUE self;
{
    gint ret;
    ret = gnome_canvas_path_def_length(_SELF(self));
    return NUM2INT(ret);
}

static VALUE
cpathdef_is_empty(self)
    VALUE self;
{
    gboolean ret;
    ret = gnome_canvas_path_def_is_empty(_SELF(self));
    return ret ? Qtrue : Qfalse;
}

static VALUE
cpathdef_has_current_point(self)
    VALUE self;
{
    gboolean ret;
    ret = gnome_canvas_path_def_has_currentpoint(_SELF(self));
    return ret ? Qtrue : Qfalse;
}

static VALUE
cpathdef_current_point(self)
    VALUE self;
{
    GnomeCanvasPathDef *path = _SELF(self);
    ArtPoint p;

    if (!gnome_canvas_path_def_has_currentpoint(path))
        return Qnil;
    gnome_canvas_path_def_currentpoint(path, &p);
    return rb_ary_new3(2, rb_float_new(p.x), rb_float_new(p.y));
}

#if 0 /* implement after Art::Bpath has been supported. */
ArtBpath * gnome_canvas_path_def_last_bpath (const GnomeCanvasPathDef * path);
ArtBpath * gnome_canvas_path_def_first_bpath (const GnomeCanvasPathDef * path);
#endif

static VALUE
cpathdef_any_open(self)
    VALUE self;
{
    gboolean ret;
    ret = gnome_canvas_path_def_any_open(_SELF(self));
    return ret ? Qtrue : Qfalse;
}

static VALUE
cpathdef_all_open(self)
    VALUE self;
{
    gboolean ret;
    ret = gnome_canvas_path_def_all_open(_SELF(self));
    return ret ? Qtrue : Qfalse;
}

static VALUE
cpathdef_any_closed(self)
    VALUE self;
{
    gboolean ret;
    ret = gnome_canvas_path_def_any_closed(_SELF(self));
    return ret ? Qtrue : Qfalse;
}

static VALUE
cpathdef_all_closed(self)
    VALUE self;
{
    gboolean ret;
    ret = gnome_canvas_path_def_all_closed(_SELF(self));
    return ret ? Qtrue : Qfalse;
}

void
Init_gnome_canvas_path_def(mGnome)
    VALUE mGnome;
{
    gnoCanvasPathDef = G_DEF_CLASS(rbgno_canvas_path_def_get_type(), "CanvasPathDef", mGnome);
    rbgobj_register_property_setter(GNOME_TYPE_CANVAS_BPATH, "bpath", cpathdef_r2g_func);
    rbgobj_register_property_getter(GNOME_TYPE_CANVAS_BPATH, "bpath", cpathdef_g2r_func);

    rb_define_singleton_method(gnoCanvasPathDef, "new", cpathdef_s_new, -1);
    rb_define_method(gnoCanvasPathDef, "duplicate", cpathdef_duplicate, 0);
    rb_define_method(gnoCanvasPathDef, "concat", cpathdef_concat, -1);
    rb_define_method(gnoCanvasPathDef, "split", cpathdef_split, 0);
    rb_define_method(gnoCanvasPathDef, "open_parts", cpathdef_open_parts, 0);
    rb_define_method(gnoCanvasPathDef, "closed_parts", cpathdef_closed_parts, 0);
    rb_define_method(gnoCanvasPathDef, "close_all", cpathdef_close_all, 0);
    rb_define_method(gnoCanvasPathDef, "reset", cpathdef_reset, 0);
    rb_define_method(gnoCanvasPathDef, "moveto", cpathdef_moveto, 2);
    rb_define_method(gnoCanvasPathDef, "lineto", cpathdef_lineto, 2);
    rb_define_method(gnoCanvasPathDef, "lineto_moving", cpathdef_lineto_moving, 2);
    rb_define_method(gnoCanvasPathDef, "curveto", cpathdef_curveto, 6);
    rb_define_method(gnoCanvasPathDef, "closepath", cpathdef_closepath, 0);
    rb_define_method(gnoCanvasPathDef, "closepath_current", cpathdef_closepath_current, 0);
    rb_define_method(gnoCanvasPathDef, "length", cpathdef_length, 0);
    rb_define_method(gnoCanvasPathDef, "empty?", cpathdef_is_empty, 0);
    rb_define_method(gnoCanvasPathDef, "current_point?", cpathdef_has_current_point, 0);
    rb_define_method(gnoCanvasPathDef, "current_point", cpathdef_current_point, 0);
    rb_define_method(gnoCanvasPathDef, "any_open?", cpathdef_any_open, 0);
    rb_define_method(gnoCanvasPathDef, "all_open?", cpathdef_all_open, 0);
    rb_define_method(gnoCanvasPathDef, "any_closed?", cpathdef_any_closed, 0);
    rb_define_method(gnoCanvasPathDef, "all_closed?", cpathdef_all_closed, 0);
}
