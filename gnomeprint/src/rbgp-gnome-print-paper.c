/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgp-gnome-print-paper.c -

  $Author: ktou $
  $Date: 2005/10/10 02:07:41 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgp.h"
#include <libgnomeprint/gnome-print-paper.h>

static GnomePrintPaper*
paper_copy(const GnomePrintPaper *paper) {
    GnomePrintPaper* new_paper;
    g_return_val_if_fail (paper != NULL, NULL);
    new_paper = g_new(GnomePrintPaper, sizeof(GnomePrintPaper));
    *new_paper = *paper;
    return new_paper;
}

GType
gnome_print_paper_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0) {
        our_type = g_boxed_type_register_static("GnomePrintPaper",
                                                (GBoxedCopyFunc)paper_copy,
                                                (GBoxedFreeFunc)g_free);
    }
    return our_type;
}


#define _SELF(o) ((GnomePrintPaper*)RVAL2BOXED(o, GNOME_TYPE_PRINT_PRINT_PAPER))
#define GPP2RVAL(o) (BOXED2RVAL(o, GNOME_TYPE_PRINT_PRINT_PAPER))
#define CONST_GPP2RVAL(o) (BOXED2RVAL((GnomePrintPaper*)o, GNOME_TYPE_PRINT_PRINT_PAPER))


#define ATTR_INT(name)                          \
static VALUE                                    \
gp_paper_int_ ## name(VALUE self)               \
{                                               \
    return INT2NUM(_SELF(self)->name);          \
}

#define ATTR_STRING(name)                               \
static VALUE                                            \
gp_paper_string_ ## name(VALUE self)                    \
{                                                       \
    return CSTR2RVAL((const char*)_SELF(self)->name);   \
}

#define ATTR_DOUBLE(name)                       \
static VALUE                                    \
gp_paper_double_ ## name(VALUE self)            \
{                                               \
    return rb_float_new(_SELF(self)->name);     \
}

#define DEFINE_READER(gpt, type, name)                                        \
rb_define_method(gpt, G_STRINGIFY(name), gp_paper_ ## type ## _ ## name, 0);


ATTR_INT(version);
ATTR_STRING(name);
ATTR_DOUBLE(width);
ATTR_DOUBLE(height);


static VALUE
gp_paper_get_default(VALUE self)
{
    return CONST_GPP2RVAL(gnome_print_paper_get_default());
}

#define RETURN_GPP_OR_NIL(paper)                \
if (paper) {                                    \
    return CONST_GPP2RVAL(paper);               \
} else {                                        \
    return Qnil;                                \
}

static VALUE
gp_paper_get_by_name(VALUE self, VALUE name)
{
    const GnomePrintPaper *paper;
    paper = gnome_print_paper_get_by_name((const guchar*)RVAL2CSTR(name));
    RETURN_GPP_OR_NIL(paper);
}

static VALUE
gp_paper_get_by_size(VALUE self, VALUE width, VALUE height)
{
    const GnomePrintPaper *paper;
    paper = gnome_print_paper_get_by_size(NUM2DBL(width),
                                          NUM2DBL(height));
    RETURN_GPP_OR_NIL(paper);
}

static VALUE
gp_paper_get_closest_by_size(VALUE self, VALUE width, VALUE height, VALUE must_fit)
{
    const GnomePrintPaper *paper;
    paper = gnome_print_paper_get_closest_by_size(NUM2DBL(width),
                                                  NUM2DBL(height),
                                                  RVAL2CBOOL(must_fit));
    RETURN_GPP_OR_NIL(paper);
}

#define GET_ARG_RAISE                                                 \
rb_raise(rb_eArgError,                                                \
         "(name), (width, height) or (width, height, must_fit) ")

static VALUE
gp_paper_get_generic(int argc, VALUE *argv, VALUE self)
{
    if (argc == 0) {
        return gp_paper_get_default(self);
    } else {
        VALUE name, width, height, must_fit;
        gboolean use_closest = TRUE;
    
        if (rb_scan_args(argc, argv, "12", &name, &height, &must_fit) == 2) {
            use_closest = FALSE;
        }
    
        if (TYPE(name) == T_STRING) {
            if (argc > 1)
                GET_ARG_RAISE;
            return gp_paper_get_by_name(self, name);
        } else {
            width = name;
            if (use_closest) {
                return gp_paper_get_closest_by_size(self, width, height,
                                                    must_fit);
            } else {
                return gp_paper_get_by_size(self, width, height);
            }
        }
    }
}



static VALUE
gp_paper_get_list(VALUE self)
{
    VALUE array = rb_ary_new();
    GList *list = gnome_print_paper_get_list();
    GList *elem;

    elem = list;
    while (elem) {
        GnomePrintPaper *paper = (GnomePrintPaper *)elem->data;
        rb_ary_push(array, GPP2RVAL(paper));
        elem = elem->next;
    }
    gnome_print_paper_free_list(list);
  
    return array;
}



void
Init_gnome_print_paper(VALUE mGnome)
{
    VALUE cPaper = G_DEF_CLASS(GNOME_TYPE_PRINT_PRINT_PAPER,
                               "PrintPaper", mGnome);

    DEFINE_READER(cPaper, int, version);
    DEFINE_READER(cPaper, string, name);
    DEFINE_READER(cPaper, double, width);
    DEFINE_READER(cPaper, double, height);
  
    rb_define_module_function(cPaper, "default", gp_paper_get_default, 0);
    rb_define_module_function(cPaper, "get", gp_paper_get_generic, -1);
    rb_define_module_function(cPaper, "list", gp_paper_get_list, 0);
}
