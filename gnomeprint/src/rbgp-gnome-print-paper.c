#include "rbgp.h"
#include <libgnomeprint/gnome-print-paper.h>

#define GPP2RVAL(paper)                         \
(rb_funcall(cPaper,                             \
            s_new,                              \
            4,                                  \
            INT2NUM(paper->version),            \
            CSTR2RVAL(paper->name),             \
            rb_float_new(paper->width),         \
            rb_float_new(paper->height)))

#define RETURN_GPP_AS_RVAL(paper) do {          \
  const GnomePrintPaper *_paper = paper;        \
  return (GPP2RVAL(_paper));                    \
} while (0)


static VALUE cPaper;
static ID s_new;

static VALUE
gp_paper_get_default(VALUE self)
{
  RETURN_GPP_AS_RVAL(gnome_print_paper_get_default());
}

static VALUE
gp_paper_get_by_name(VALUE self, VALUE name)
{
  RETURN_GPP_AS_RVAL(gnome_print_paper_get_by_name(RVAL2CSTR(name)));
}

static VALUE
gp_paper_get_by_size(VALUE self, VALUE width, VALUE height)
{
  RETURN_GPP_AS_RVAL(gnome_print_paper_get_by_size(NUM2DBL(width),
                                                   NUM2DBL(height)));
}

static VALUE
gp_paper_get_closest_by_size(VALUE self, VALUE width, VALUE height, VALUE mustfit)
{
  RETURN_GPP_AS_RVAL(gnome_print_paper_get_closest_by_size(NUM2DBL(width),
                                                           NUM2DBL(height),
                                                           RTEST(mustfit)));
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
Init_gnome_print_paper(VALUE mGnome, VALUE mGP)
{
  cPaper = rb_struct_define("Paper",
                            "version", "name", "width", "height",
                            NULL);
  rb_define_const(mGnome, "PrintPaper", cPaper);

  s_new = rb_intern("new");

  rb_define_module_function(cPaper, "default", gp_paper_get_default, 0);
  rb_define_module_function(cPaper, "get_by_name", gp_paper_get_by_name, 1);
  rb_define_module_function(cPaper, "get_by_size", gp_paper_get_by_size, 2);
  rb_define_module_function(cPaper, "get_closest_by_size",
                            gp_paper_get_closest_by_size, 3);
  rb_define_module_function(cPaper, "list", gp_paper_get_list, 0);
}
