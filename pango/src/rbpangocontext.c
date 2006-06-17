/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangocontext.c -

  $Author: mutoh $
  $Date: 2006/06/17 07:50:46 $

  Copyright (C) 2002-2005 Masao Mutoh
************************************************/

#include "rbpango.h"

#define _SELF(self) (PANGO_CONTEXT(RVAL2GOBJ(self)))
#define RVAL2DESC(v) ((PangoFontDescription*)RVAL2BOXED(v, PANGO_TYPE_FONT_DESCRIPTION))
#define RVAL2LANG(v) ((PangoLanguage*)RVAL2BOXED(v, PANGO_TYPE_LANGUAGE))

static VALUE
rcontext_itemize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1, arg2, arg3, arg4, arg5, arg6;
    GList* list;
    
    rb_scan_args(argc, argv, "42", &arg1, &arg2, &arg3, &arg4, &arg5, &arg6);

    if (TYPE(arg1) == T_STRING) {
        list = pango_itemize(_SELF(self), 
                             RVAL2CSTR(arg1),      /* text */ 
                             NUM2INT(arg2),        /* start_index */ 
                             NUM2INT(arg3),        /* length */
                             (PangoAttrList*)RVAL2BOXED(arg4, PANGO_TYPE_ATTR_LIST), /* attrs */
                             NIL_P(arg5) ? NULL : (PangoAttrIterator*)RVAL2BOXED(arg5, PANGO_TYPE_ATTR_ITERATOR)); /* cached_iter */
    } else {
#ifdef HAVE_PANGO_ITEMIZE_WITH_BASE_DIR
        list = pango_itemize_with_base_dir(_SELF(self), 
                                           RVAL2GENUM(arg1, PANGO_TYPE_DIRECTION), /* base_dir */
                                           RVAL2CSTR(arg2),      /* text */ 
                                           NUM2INT(arg3),        /* start_index */ 
                                           NUM2INT(arg4),        /* length */
                                           (PangoAttrList*)RVAL2BOXED(arg5, PANGO_TYPE_ATTR_LIST), /* attrs */
                                           NIL_P(arg6) ? NULL : (PangoAttrIterator*)RVAL2BOXED(arg6, PANGO_TYPE_ATTR_ITERATOR)); /* cached_iter */
#else
        rb_warn("Pango::Context#itemize(base_dir, text, start_index, length, attrs, cached_iter) isn't supported on this environment.");
        list = pango_itemize(_SELF(self), 
                             RVAL2CSTR(arg1),      /* text */ 
                             NUM2INT(arg2),        /* start_index */ 
                             NUM2INT(arg3),        /* length */
                             (PangoAttrList*)RVAL2BOXED(arg4, PANGO_TYPE_ATTR_LIST), /* attrs */
                             NIL_P(arg5) ? NULL : (PangoAttrIterator*)RVAL2BOXED(arg5, PANGO_TYPE_ATTR_ITERATOR)); /* cached_iter */
#endif
    }
    return GLIST2ARY2F(list, PANGO_TYPE_ITEM);
}

/* Move to Pango module (rbpangomain.c)
GList*      pango_reorder_items             (GList *logical_items);
*/

#ifdef PANGO_ENABLE_BACKEND
static VALUE
rcontext_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, pango_context_new());
    return Qnil;
}

static VALUE
rcontext_set_font_map(self, font_map)
    VALUE self, font_map;
{
    pango_context_set_font_map(_SELF(self), PANGO_FONT_MAP(RVAL2GOBJ(font_map)));
    return self;
}

#if PANGO_CHECK_VERSION(1,6,0)
static VALUE
rcontext_get_font_map(self)
    VALUE self;
{
    return GOBJ2RVAL(pango_context_get_font_map(_SELF(self)));
}
#endif
#endif /* PANGO_ENABLE_BACKEND */

static VALUE
rcontext_get_font_description(self)
    VALUE self;
{
    PangoFontDescription* ret = pango_context_get_font_description(_SELF(self));
    return ret ? BOXED2RVAL(ret, PANGO_TYPE_FONT_DESCRIPTION) : Qnil;
}

static VALUE
rcontext_set_font_description(self, desc)
    VALUE self, desc;
{
    pango_context_set_font_description(_SELF(self), RVAL2DESC(desc));
    return self;
}

static VALUE
rcontext_get_language(self)
    VALUE self;
{
    PangoLanguage* ret = pango_context_get_language(_SELF(self));
    return BOXED2RVAL(ret, PANGO_TYPE_LANGUAGE);
}

static VALUE
rcontext_set_language(self, lang)
    VALUE self, lang;
{
    pango_context_set_language(_SELF(self), 
                               RVAL2LANG(lang));
    return self;
}

static VALUE
rcontext_get_base_dir(self)
    VALUE self;
{
    return GENUM2RVAL(pango_context_get_base_dir(_SELF(self)), PANGO_TYPE_DIRECTION);
}

static VALUE
rcontext_set_base_dir(self, direction)
    VALUE self, direction;
{
    pango_context_set_base_dir(_SELF(self), RVAL2GENUM(direction, PANGO_TYPE_DIRECTION));
    return self;
}

#if PANGO_CHECK_VERSION(1,6,0)
static VALUE
rcontext_get_matrix(self)
    VALUE self;
{
    const PangoMatrix* matrix = pango_context_get_matrix(_SELF(self));
    return matrix ? BOXED2RVAL((PangoMatrix*)matrix, PANGO_TYPE_MATRIX) : Qnil;
}

static VALUE
rcontext_set_matrix(self, matrix)
    VALUE self, matrix;
{
    pango_context_set_matrix(_SELF(self), 
                             (PangoMatrix*)RVAL2BOXED(matrix, PANGO_TYPE_MATRIX));
    return self;
}
#endif

static VALUE
rcontext_load_font(self, desc)
    VALUE self, desc;
{
    return GOBJ2RVAL(pango_context_load_font(_SELF(self), RVAL2DESC(desc)));
}

static VALUE
rcontext_load_fontset(self, desc, lang)
    VALUE self, desc, lang;
{
    return GOBJ2RVAL(pango_context_load_fontset(_SELF(self),
                                                RVAL2DESC(desc), RVAL2LANG(lang)));
}

static VALUE
rcontext_get_metrics(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE desc, lang;

    rb_scan_args(argc, argv, "11", &desc, &lang);

    return BOXED2RVAL(pango_context_get_metrics(_SELF(self), 
                                                RVAL2DESC(desc), 
                                                NIL_P(lang) ? NULL : RVAL2LANG(lang)),
                                                PANGO_TYPE_FONT_METRICS);
}

static VALUE
rcontext_list_families(self)
    VALUE self;
{
    int n_families;
    PangoFontFamily** families;
    int i;
    VALUE result;

    pango_context_list_families(_SELF(self),
                                &families,
                                &n_families);

    result = rb_ary_new2(n_families);
    for (i = 0; i < n_families; i++)
      rb_ary_store(result, i, GOBJ2RVAL(families[i]));

    g_free(families);

    return result;
}

#if PANGO_CHECK_VERSION(1,10,0)
#  if HAVE_RB_CAIRO_H
static VALUE
rcontext_set_font_options(self, options)
    VALUE self, options;
{
    if (NIL_P(options))
        pango_cairo_context_set_font_options(_SELF(self), NULL);
    else
        pango_cairo_context_set_font_options(_SELF(self),
                                             RVAL2CRFONTOPTIONS(options));
    return self;
}

static VALUE
rcontext_get_font_options(self)
    VALUE self;
{
    const cairo_font_options_t *options;
    options = pango_cairo_context_get_font_options(_SELF(self));
    if (options)
        return CRFONTOPTIONS2RVAL(cairo_font_options_copy(options));
    else
        return Qnil;
}

static VALUE
rcontext_set_resolution(self, dpi)
    VALUE self, dpi;
{
    pango_cairo_context_set_resolution(_SELF(self), NUM2DBL(dpi));
    return self;
}

static VALUE
rcontext_get_resolution(self)
    VALUE self;
{
    return rb_float_new(pango_cairo_context_get_resolution(_SELF(self)));
}
#  endif
#endif


static VALUE
rcontext_list_families_old(self)
    VALUE self;
{
    rb_warn("Deprecated. Use Pango::Context#families instead.");
    return rcontext_list_families(self);
}

void
Init_pango_context()
{
    VALUE pContext = G_DEF_CLASS(PANGO_TYPE_CONTEXT, "Context", mPango);

    rb_define_method(pContext, "itemize", rcontext_itemize, -1);

#ifdef PANGO_ENABLE_BACKEND
    rb_define_method(pContext, "initialize", rcontext_initialize, 0);
    rb_define_method(pContext, "set_font_map", rcontext_set_font_map, 1);
#if PANGO_CHECK_VERSION(1,6,0)
    rb_define_method(pContext, "font_map", rcontext_get_font_map, 0);
#endif
#endif /* PANGO_ENABLE_BACKEND */
    rb_define_method(pContext, "font_description", rcontext_get_font_description, 0);
    rb_define_method(pContext, "set_font_description", rcontext_set_font_description, 1);
    rb_define_method(pContext, "language", rcontext_get_language, 0);
    rb_define_method(pContext, "set_language", rcontext_set_language, 1);
    rb_define_method(pContext, "base_dir", rcontext_get_base_dir, 0);
    rb_define_method(pContext, "set_base_dir", rcontext_set_base_dir, 1);
#if PANGO_CHECK_VERSION(1,6,0)
    rb_define_method(pContext, "matrix", rcontext_get_matrix, 0);
    rb_define_method(pContext, "set_matrix", rcontext_set_matrix, 1);
#endif
    rb_define_method(pContext, "load_font", rcontext_load_font, 1);
    rb_define_method(pContext, "load_fontset", rcontext_load_fontset, 2);
    rb_define_method(pContext, "get_metrics", rcontext_get_metrics, -1);
    rb_define_method(pContext, "families", rcontext_list_families, 0);

#if PANGO_CHECK_VERSION(1,10,0)
#  if HAVE_RB_CAIRO_H
    rb_define_method(pContext, "set_font_options", rcontext_set_font_options, 1);
    rb_define_method(pContext, "font_options", rcontext_get_font_options, 0);
    rb_define_method(pContext, "set_resolution", rcontext_set_resolution, 1);
    rb_define_method(pContext, "resolution", rcontext_get_resolution, 0);
#  endif
#endif
    
    /* This will remove 2 or 3 releases later since 0.14.0. */
    rb_define_method(pContext, "list_families", rcontext_list_families_old, 0);

    G_DEF_SETTERS(pContext);

    /* PangoDirection */
    G_DEF_CLASS(PANGO_TYPE_DIRECTION, "Direction", pContext);
    G_DEF_CONSTANTS(pContext, PANGO_TYPE_DIRECTION, "PANGO_");
}
