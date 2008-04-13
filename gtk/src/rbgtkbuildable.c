/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkbuildable.c -

  Copyright (C) 2008 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2, 12, 0)
#define _SELF(self) (GTK_BUILDABLE(RVAL2GOBJ(self)))
#define RVAL2BUILDER(obj) (GTK_BUILDER(RVAL2GOBJ(obj)))

static VALUE
buildable_get_name(VALUE self, VALUE name)
{
    return CSTR2RVAL(gtk_buildable_get_name(_SELF(self)));
}

static VALUE
buildable_set_name(VALUE self, VALUE name)
{
    gtk_buildable_set_name(_SELF(self), RVAL2CSTR(name));
    return self;
}

static VALUE
buildable_add_child(int argc, VALUE *argv, VALUE self)
{
    VALUE builder, child, type;

    rb_scan_args(argc, argv, "21", &builder, &child, &type);
    gtk_buildable_add_child(_SELF(self), RVAL2BUILDER(builder), RVAL2GOBJ(child),
                            RVAL2CSTR_ACCEPT_NIL(type));
    return self;
}

static VALUE
buildable_set_buildable_property(VALUE self, VALUE rb_builder,
                                 VALUE rb_name, VALUE rb_value)
{
    GtkBuilder *builder;
    const gchar *name;
    GValue value = {0};

    builder = RVAL2BUILDER(rb_builder);
    name = RVAL2CSTR(rb_name);
    rbgobj_initialize_gvalue(&value, rb_value);
    gtk_buildable_set_buildable_property(_SELF(self), builder, name, &value);

    return self;
}

static VALUE
buildable_construct_child(VALUE self, VALUE builder, VALUE name)
{
    return GOBJ2RVAL(gtk_buildable_construct_child(_SELF(self),
						   RVAL2BUILDER(builder),
						   RVAL2CSTR(name)));
}

/*
  customized buildable is not supported yet.

gboolean  gtk_buildable_custom_tag_start       (GtkBuildable        *buildable,
						GtkBuilder          *builder,
						GObject             *child,
						const gchar         *tagname,
						GMarkupParser       *parser,
						gpointer            *data);
void      gtk_buildable_custom_tag_end         (GtkBuildable        *buildable,
						GtkBuilder          *builder,
						GObject             *child,
						const gchar         *tagname,
						gpointer            *data);
void      gtk_buildable_custom_finished        (GtkBuildable        *buildable,
						GtkBuilder          *builder,
						GObject             *child,
						const gchar         *tagname,
						gpointer             data);
void      gtk_buildable_parser_finished        (GtkBuildable        *buildable,
						GtkBuilder          *builder);
*/

static VALUE
buildable_get_internal_child(VALUE self, VALUE builder, VALUE child_name)
{
    return GOBJ2RVAL(gtk_buildable_get_internal_child(_SELF(self),
						      RVAL2BUILDER(builder),
						      RVAL2CSTR(child_name)));
}
#endif

void
Init_gtk_buildable()
{
#if GTK_CHECK_VERSION(2, 12, 0)
    VALUE cGtkBuildable;

    cGtkBuildable = G_DEF_CLASS(GTK_TYPE_BUILDABLE, "Buildable", mGtk);

    rb_define_method(cGtkBuildable, "name", buildable_get_name, 0);
    rb_define_method(cGtkBuildable, "set_name", buildable_set_name, 1);
    rb_define_method(cGtkBuildable, "add_child", buildable_add_child, -1);
    rb_define_method(cGtkBuildable, "set_buildable_property",
                     buildable_set_buildable_property, 3);
    rb_define_method(cGtkBuildable, "construct_child",
                     buildable_construct_child, 2);
    rb_define_method(cGtkBuildable, "get_internal_child",
                     buildable_get_internal_child, 2);

    G_DEF_SETTERS(cGtkBuildable);
#endif
}
