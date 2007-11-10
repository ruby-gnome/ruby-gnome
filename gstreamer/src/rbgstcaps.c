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

/* Class: Gst::Caps
 * Capabilities of pads. 
 */

/*
 * Method: new(*structures)
 * structures: a list of Hash objects.
 *
 * Creates a new Gst::Caps object and adds all given structures to it.  If no
 * structures are given, the caps will be empty.  If you want a caps that is 
 * compatible with any media format, just create an empty caps, then call
 * Gst::Caps#set_any. 
 *
 * Returns: a newly allocated Gst::Caps object.
 */
static VALUE
rb_gst_caps_new (int argc, VALUE * argv, VALUE self)
{
    GstCaps *caps;
    int i;

    caps = gst_caps_new_any ();
    if (caps != NULL) {
        for (i = 0; i < argc; i++)
            gst_caps_append_structure (caps, RVAL2GST_STRUCT(argv[i]));
        G_INITIALIZE (self, caps);
    }
    return Qnil;
}

/*
 * Method: set_any(state)
 * state: true or false.
 *
 * Sets whether the caps should be compatible with any media format.
 *
 * Returns: self.
 */
static VALUE
rb_gst_caps_set_any (VALUE self, VALUE state)
{
    RGST_CAPS (self)->flags = GST_CAPS_FLAGS_ANY;
    return self;
}

/*
 * Method: append(caps)
 * caps: a Gst::Caps to append.
 *
 * Appends the structures contained in caps to self.
 *
 * Returns: self.
 */
static VALUE
rb_gst_caps_append (VALUE self, VALUE caps)
{
    gst_caps_append (RGST_CAPS (self), RGST_CAPS (caps));
    return self;
}

/*
 * Method: append_structure(structure)
 * structure: the Hash object to append.
 *
 * Append the given structure to self.
 *
 * Returns: self.
 */
static VALUE
rb_gst_caps_append_structure (VALUE self, VALUE structure)
{
    gst_caps_append_structure (RGST_CAPS (self), RVAL2GST_STRUCT(structure));
    return self;
}

/* Method: size
 * Returns: the number of structures contained in the caps.
 */
static VALUE
rb_gst_caps_get_size (VALUE self)
{
    return INT2FIX (gst_caps_get_size (RGST_CAPS (self)));
}

/*
 * Method: get_structure(index)
 * index: the index of a structure.
 *
 * Finds the structure in the caps that has the given index, and 
returns it.
 *
 * Returns: a Hash object corresponding to index, or nil if not found.
 */
static VALUE
rb_gst_caps_get_structure (VALUE self, VALUE index)
{
    GstStructure *structure = gst_caps_get_structure (RGST_CAPS (self),
                                                      FIX2INT (index));

    return GST_STRUCT2RVAL(structure);
}

static VALUE
rb_gst_caps_set_simple (VALUE self, VALUE field, VALUE value)
{
    /*
     * TODO 
     */
    return self;
}

/* Method: any?
 * Returns: whether the caps represents any media format.
 */
static VALUE
rb_gst_caps_is_any (VALUE self)
{
    return CBOOL2RVAL (gst_caps_is_any (RGST_CAPS (self)));
}

/* Method: empty?
 * Returns: whether the caps represents no media formats.
 */
static VALUE
rb_gst_caps_is_empty (VALUE self)
{
    return CBOOL2RVAL (gst_caps_is_empty (RGST_CAPS (self)));
}

/* Method: simple?
 * Returns: whether the caps contains multiple structure.
 */
static VALUE
rb_gst_caps_is_simple (VALUE self)
{
    return CBOOL2RVAL(GST_CAPS_IS_SIMPLE(RGST_CAPS (self)));
}

/*
 * Method: fixed?
 *
 * Fixed caps describes exactly one format, that is, they have exactly one
 * structure, and each field in the structure describes a fixed type.
 * Examples of non-fixed types are ranges and lists.
 *
 * Returns: whether the caps is fixed.
 */
static VALUE
rb_gst_caps_is_fixed (VALUE self)
{
    return CBOOL2RVAL (gst_caps_is_fixed (RGST_CAPS (self)));
}

/*
  * Method: equal?(caps)
  * caps: another Gst::Caps.
  *
  * Returns: whether the given caps represent the same set of caps than 
self.
  */
static VALUE
rb_gst_caps_is_equal (VALUE self, VALUE caps)
{
    return CBOOL2RVAL (gst_caps_is_equal (RGST_CAPS (self), RGST_CAPS (caps)));
}

/*
 * Method: equal_fixed?(caps)
 * caps: another Gst::Caps.
 *
 * Returns: whether the given fixed caps represent the same set of caps 
than self
 * (self must be fixed as well).
 */
static VALUE
rb_gst_caps_is_equal_fixed (VALUE self, VALUE caps)
{
    return CBOOL2RVAL (gst_caps_is_equal_fixed (RGST_CAPS (self), RGST_CAPS
                                                (caps)));
}

/*
 * Method: always_compatible?(caps)
 * caps: another Gst::Caps.
 *
 * A given caps structure is always compatible with another if every 
media format that is
 * in the first is also contained in the second.
 *
 * Returns: whether self is compatible with the given caps.
 */
static VALUE
rb_gst_caps_is_always_compatible (VALUE self, VALUE caps)
{
    return CBOOL2RVAL (gst_caps_is_always_compatible (RGST_CAPS (self),
                                                      RGST_CAPS (caps)));
}

/*
 * Method: subset?(caps)
 * caps: another Gst::Caps.
 *
 * Checks if all caps represented by self are also represented by the 
given caps.
 * This method does not work reliably if optional properties for caps 
are included
 * on one caps and omitted on the other.
 *
 * Returns: whether self is a subset of the given caps.
 */
static VALUE
rb_gst_caps_is_subset (VALUE self, VALUE caps)
{
    return CBOOL2RVAL (gst_caps_is_subset (RGST_CAPS (self), RGST_CAPS (caps)));
}

/*
 * Method: intersect(caps)
 * caps: another Gst::Caps.
 *
 * Creates a new Gst::Caps that contains all the formats that are 
common to both
 * self and the given caps.
 *
 * Returns: a new Gst::Caps object.
 */
static VALUE
rb_gst_caps_intersect (VALUE self, VALUE caps)
{
    return RGST_CAPS_NEW (gst_caps_intersect (RGST_CAPS (self), RGST_CAPS
                                              (caps)));
}

/*
 * Method: union(caps)
 * caps: another Gst::Caps.
 *
 * Creates a new Gst::Caps that contains all the formats that are in 
either
 * self and the given caps.
 *
 * Returns: a new Gst::Caps object.
 */
static VALUE
rb_gst_caps_union (VALUE self, VALUE caps)
{
    return RGST_CAPS_NEW (gst_caps_union (RGST_CAPS (self), RGST_CAPS (caps)));
}

/*
 * Method: normalize
 *
 * Creates a new Gst::Caps that represents the same set of formats as self,
 * but contains no lists.  Each list is expanded into separate structures
 * (as Hash objects).
 *
 * Returns: a new Gst::Caps object.
 */
static VALUE
rb_gst_caps_normalize (VALUE self)
{
    return RGST_CAPS_NEW (gst_caps_normalize (RGST_CAPS (self)));
}

/*
 * Method: simplify!
 *
 * Modifies the current caps inplace into a representation that represents
 * the same set of formats as self, but in a simpler form.
 * Component structures that are indentical are merged.  Component 
structures
 * that have ranges or lists that can be merged are also merged.
 *
 * Returns: whether the caps could be simplified.
 */
static VALUE
rb_gst_caps_do_simplify (VALUE self)
{
    return CBOOL2RVAL (gst_caps_do_simplify (RGST_CAPS (self)));
}

/*
 * Method: replace(caps)
 * caps: another Gst::Caps.
 *
 * Replaces the current caps inplace with the given caps.
 *
 * Returns: self.
 */
static VALUE
rb_gst_caps_replace (VALUE self, VALUE caps)
{
    GstCaps *current = RGST_CAPS (self);

    gst_caps_replace (&current, RGST_CAPS (caps));
    return self;
}

/* Method: to_string
 * Returns: a string representation of the current caps.
 */
static VALUE
rb_gst_caps_to_string (VALUE self)
{
    return CSTR2RVAL (gst_caps_to_string (RGST_CAPS (self)));
}

/*
 * Class method: from_string(string)
 * string: a string to convert to a Gst::Caps.
 *
 * Converts a caps from a string representation.
 *
 * Returns: a newly allocated Gst::Caps.
 */
static VALUE
rb_gst_caps_from_string (VALUE self, VALUE string)
{
    return RGST_CAPS_NEW (gst_caps_from_string (RVAL2CSTR (string)));
}

/*
 * Method: subtract(caps)
 * caps: another Gst::Caps.
 *
 * Subtracts the given subtrahend caps from the minuend (self).
 * This method does not work reliably if optional properties for caps 
are included
 * on one caps and omitted on the other.
 *
 * Returns: the resulting caps, as a new Gst::Caps object.
 */
static VALUE
rb_gst_caps_subtract (VALUE self, VALUE caps)
{
    return RGST_CAPS_NEW (gst_caps_subtract (RGST_CAPS (self), RGST_CAPS
                                             (caps)));
}

void
Init_gst_caps (void)
{
    VALUE c = G_DEF_CLASS (GST_TYPE_CAPS, "Caps", mGst);

    rb_define_method (c, "initialize", rb_gst_caps_new, -1);
    rb_define_method (c, "set_any", rb_gst_caps_set_any, 1);
    rb_define_method (c, "append", rb_gst_caps_append, 1);
    rb_define_method (c, "append_structure", rb_gst_caps_append_structure, 1);
    rb_define_method (c, "size", rb_gst_caps_get_size, 0);
    rb_define_alias (c, "length", "size");
    rb_define_method (c, "get_structure", rb_gst_caps_get_structure, 1);
    rb_define_method (c, "set_simple", rb_gst_caps_set_simple, 2);
    rb_define_method (c, "any?", rb_gst_caps_is_any, 0);
    rb_define_method (c, "empty?", rb_gst_caps_is_empty, 0);
    rb_define_method (c, "simple?", rb_gst_caps_is_simple, 0);
    rb_define_alias (c, "chained?", "simple?");
    rb_define_method (c, "fixed?", rb_gst_caps_is_fixed, 0);
    rb_define_method (c, "equal?", rb_gst_caps_is_equal, 1);
    rb_define_alias (c, "==", "equal?");
    rb_define_method (c, "equal_fixed?", rb_gst_caps_is_equal_fixed, 1);
    rb_define_method (c, "always_compatible?",
                      rb_gst_caps_is_always_compatible, 1);
    rb_define_method (c, "subset?", rb_gst_caps_is_subset, 1);
    rb_define_method (c, "intersect", rb_gst_caps_intersect, 1);
    rb_define_method (c, "union", rb_gst_caps_union, 1);
    rb_define_method (c, "normalize", rb_gst_caps_normalize, 0);
    rb_define_method (c, "simplify!", rb_gst_caps_do_simplify, 0);
    rb_define_method (c, "replace!", rb_gst_caps_replace, 1);
    rb_define_method (c, "to_string", rb_gst_caps_to_string, 0);
    rb_define_alias (c, "to_s", "to_string");
    rb_define_singleton_method (c, "from_string", rb_gst_caps_from_string, 1);
    rb_define_method (c, "subtract", rb_gst_caps_subtract, 1);

    G_DEF_SETTERS(c);

/* TODO:
    gst_caps_structure_fixate_field_nearest_int ()
    gst_caps_structure_fixate_field_nearest_double () */

}
