/*
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 *
 * This file is part of Ruby/GStreamer.
 *
 * Ruby/GStreamer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/GStreamer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/GStreamer; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgst.h"

/* Class: Gst::Structure
 * Generic structure containing fields of names and values. 
 */

/*
 * Class method: from_string(string)
 * string: a string representation of a Gst::Structure.
 *
 * Creates a Gst::Structure from a string representation.
 *
 * Returns: a newly allocated Gst::Structure.
 */

static VALUE
rb_gst_struct_from_string (VALUE self, VALUE string)
{
    GstStructure *structure =
        gst_structure_from_string (RVAL2CSTR (string), NULL);
    return structure != NULL ? RGST_STRUCTURE_NEW (structure)
        : Qnil;
}

/*
 * Method: set(*fields)
 * fields: a set of name/value pairs.
 *
 * Parses the variable arguments and sets fields accordingly.  Variable arguments should be in
 * the form field name and value.
 *
 * Returns: self.
 */
static VALUE
rb_gst_struct_set (VALUE self, VALUE fields)
{
    int i;

    if (RARRAY (fields)->len == 0 || RARRAY (fields)->len % 2 != 0)
        rb_raise (rb_eArgError,
                  "Invalid number of arguments (must be a set of name/value pairs)");

    for (i = 0; i < RARRAY (fields)->len; i += 2) {
        VALUE name = rb_ary_entry (fields, i);
        VALUE value = rb_ary_entry (fields, i + 1);

        switch (TYPE (value)) {
        case T_TRUE:
        case T_FALSE:
            gst_structure_set (RGST_STRUCTURE (self), RVAL2CSTR (name),
                               G_TYPE_BOOLEAN, RVAL2CBOOL (value), NULL);
            break;

        case T_STRING:
            gst_structure_set (RGST_STRUCTURE (self), RVAL2CSTR (name),
                               G_TYPE_STRING, RVAL2CSTR (value), NULL);
            break;

        case T_FIXNUM:
            gst_structure_set (RGST_STRUCTURE (self), RVAL2CSTR (name),
                               G_TYPE_INT, FIX2INT (value), NULL);
            break;

        case T_FLOAT:
            gst_structure_set (RGST_STRUCTURE (self), RVAL2CSTR (name),
                               G_TYPE_DOUBLE, NUM2DBL (value), NULL);
            break;

        default:
            rb_raise (rb_eArgError, "Unsupported value type (%d)",
                      TYPE (value));
        }
    }
    return self;
}

/*
 * Class method: new(name, *fields)
 * name: the name for the new structure.
 * fields: a set of name/value pairs.
 *
 * Creates a new Gst::Structure with the given name.  If no fields are provided, 
 * the structure will be empty.  Otherwise, parses the variable list and sets fields
 * to the values listed.  Variables arguments should be passed as field name and value. 
 *
 * Returns: a newly created Gst::Structure object.
 */
static VALUE
rb_gst_struct_new (int argc, VALUE * argv, VALUE self)
{
    GstStructure *structure;
    VALUE name, fields;

    rb_scan_args (argc, argv, "1*", &name, &fields);

    structure = gst_structure_empty_new (RVAL2CSTR (name));
    if (structure != NULL) {
        G_INITIALIZE (self, structure);
        if (RARRAY (fields)->len > 0)
            rb_gst_struct_set (self, fields);
    }
    return Qnil;
}

/*
 * Method: name
 * Returns: the string representing the name of the structure.
 */
static VALUE
rb_gst_struct_name (VALUE self)
{
    return CSTR2RVAL (gst_structure_get_name (RGST_STRUCTURE (self)));
}

/*
 * Method: set_name(name)
 * name: the new name of the structure.
 *
 * Sets the name of the structure to the given name.
 *
 * Returns: self.
 */
static VALUE
rb_gst_struct_set_name (VALUE self, VALUE name)
{
    gst_structure_set_name (RGST_STRUCTURE (self), RVAL2CSTR (name));
    return self;
}

/*
 * Method: set_value(name, value)
 * name: the name of the field to set.
 * value: the new value of the field.
 * 
 * Sets the field with the given name to the provided value. 
 * If the field does not exist, it is created.  If the field exists, the
 * previous value is freed and replaced.
 *
 * Returns: self.
 */
static VALUE
rb_gst_struct_set_value (VALUE self, VALUE name, VALUE value)
{
    GValue gvalue = { 0, };
    rbgobj_rvalue_to_gvalue (value, &gvalue);
    gst_structure_set_value (RGST_STRUCTURE (self), RVAL2CSTR (name), &gvalue);
    return self;
}

/*
 * Method: [name]=(value)
 *
 * name: the name of the field to set.
 * value: the new value of the field.
 * 
 * Sets the field with the given name to the provided value. 
 * If the field does not exist, it is created.  If the field exists, the
 * previous value is freed and replaced.
 *
 * Returns: the given value.
 */
static VALUE
rb_gst_struct_set_value_eql (VALUE self, VALUE name, VALUE value)
{
    return rb_gst_struct_set_value (self, name, value);
}

/*
 * Method: get_value(name)
 * name: the name of the field to get.
 *
 * Returns: the value corresponding to the field with the given name.
 */
static VALUE
rb_gst_struct_get_value (VALUE self, VALUE name)
{
    return
        GVAL2RVAL (gst_structure_get_value
                   (RGST_STRUCTURE (self), RVAL2CSTR (name)));
}

/*
 * Method: remove_field(field)
 * field: the name of the field to remove.
 *
 * Removes the field with the given name.  If the field with the given
 * name does not exist, the structure is unchanged.
 *
 * Returns: self.
 */
static VALUE
rb_gst_struct_remove_field (VALUE self, VALUE field)
{
    gst_structure_remove_field (RGST_STRUCTURE (self), RVAL2CSTR (field));
    return self;
}

/*
 * Method: remove_fields(*fields)
 * fields: the name of the fields to remove.
 *
 * Removes the fields with the given names.  If a field does not exist, 
 * it is ignored.
 *
 * Returns: self.
 */
static VALUE
rb_gst_struct_remove_fields (VALUE self, VALUE fields)
{
    int i;

    if (RARRAY (fields)->len == 0)
        rb_raise (rb_eArgError,
                  "Invalid number of arguments (at least one field is required)");

    for (i = 0; i < RARRAY (fields)->len; i++)
        rb_gst_struct_remove_field (self, rb_ary_entry (fields, i));
    return self;
}

/*
 * Method: remove_all_fields
 * 
 * Removes all fields in the structure.
 *
 * Returns: self.
 */
static VALUE
rb_gst_struct_remove_all_fields (VALUE self)
{
    gst_structure_remove_all_fields (RGST_STRUCTURE (self));
    return self;
}

/*
 * Method: get_field_type(field)
 * field: the name of the field.
 *
 * Returns: the type of the field with the given name (as a Class object), or nil if
 * the field is not found.
 */
static VALUE
rb_gst_struct_get_field_type (VALUE self, VALUE field)
{
    GType type = gst_structure_get_field_type (RGST_STRUCTURE (self),
                                               RVAL2CSTR (field));

    return type == G_TYPE_INVALID ? Qnil : GTYPE2CLASS (type);
}

/* Method: to_string
 * Returns: a human-readable representation of the structure.
 */
static VALUE
rb_gst_struct_to_string (VALUE self)
{
    return CSTR2RVAL (gst_structure_to_string (RGST_STRUCTURE (self)));
}

/* Method: n_fields
 * Returns: the number of fields in the structure.
 */
static VALUE
rb_gst_struct_n_fields (VALUE self)
{
    return INT2FIX (gst_structure_n_fields (RGST_STRUCTURE (self)));
}

/* Method: has_field?(field)
 * field: the name of a field.
 *
 * Returns: true if the structure contains a field with the given name.
 */
static VALUE
rb_gst_struct_has_field (VALUE self, VALUE field)
{
    return CBOOL2RVAL (gst_structure_has_field (RGST_STRUCTURE (self), RVAL2CSTR (field)));
}

/* Method: has_field_typed?(field, klass)
 * field: the name of a field.
 * klass: the class of a value. 
 *
 * Returns: true if the structure contains a field with the given name and klass.
 */
static VALUE
rb_gst_struct_has_field_typed (VALUE self, VALUE field, VALUE klass)
{
    return CBOOL2RVAL (gst_structure_has_field_typed (RGST_STRUCTURE (self), RVAL2CSTR (field), CLASS2GTYPE (klass)));
}

void
Init_gst_structure (void)
{
    VALUE c = G_DEF_CLASS (GST_TYPE_STRUCTURE, "Structure", mGst);

    rb_define_singleton_method (c, "from_string", rb_gst_struct_from_string, 1);
    rb_define_method (c, "initialize", rb_gst_struct_new, -1);
    rb_define_method (c, "name", rb_gst_struct_name, 0);
    rb_define_method (c, "set_name", rb_gst_struct_set_name, 1);
    rb_define_method (c, "set_value", rb_gst_struct_set_value, 2);
    rb_define_method (c, "[]=", rb_gst_struct_set_value_eql, 2);
    rb_define_method (c, "get_value", rb_gst_struct_get_value, 1);
    rb_define_alias (c, "[]", "get_value");
    rb_define_method (c, "set", rb_gst_struct_set, -2);
    rb_define_method (c, "remove_field", rb_gst_struct_remove_field, 1);
    rb_define_method (c, "remove_fields", rb_gst_struct_remove_fields, -2);
    rb_define_alias (c, "remove", "remove_fields");
    rb_define_method (c, "remove_all_fields", rb_gst_struct_remove_all_fields, 0);
    rb_define_alias (c, "clear", "remove_all_fields");
    rb_define_method (c, "get_field_type", rb_gst_struct_get_field_type, 1);
    rb_define_method (c, "n_fields", rb_gst_struct_n_fields, 0);
    rb_define_alias (c, "size", "n_fields");
    rb_define_alias (c, "length", "n_fields");
    rb_define_method (c, "has_field?", rb_gst_struct_has_field, 1);
    rb_define_method (c, "has_field_typed?", rb_gst_struct_has_field_typed, 2);
    rb_define_method (c, "to_string", rb_gst_struct_to_string, 0);
    rb_define_alias (c, "to_s", "to_string");

    /* TODO: wrap gst_structure_foreach */

    G_DEF_SETTERS (c);
}
