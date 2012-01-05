/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2012  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbgst-private.h"

#define RG_TARGET_NAMESPACE cCaps
#define SELF(self) (RVAL2GST_CAPS(self))

/* Class: Gst::Caps
 * Capabilities of pads. 
 */

VALUE
rbgst_ruby_object_from_gst_caps_with_unref(GstCaps *caps)
{
    VALUE rb_caps;

    rb_caps = GST_CAPS2RVAL(caps);
    if (caps)
        gst_caps_unref(caps);
    return rb_caps;
}

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
rg_initialize (int argc, VALUE * argv, VALUE self)
{
    GstCaps *caps;
    int i;

    caps = gst_caps_new_any ();
    if (caps != NULL) {
        for (i = 0; i < argc; i++) {
            GstStructure *structure;
            structure = RVAL2GST_STRUCT(argv[i]);
            gst_caps_append_structure (caps, gst_structure_copy (structure));
        }
        G_INITIALIZE (self, caps);
        gst_caps_unref (caps);
    }
    return Qnil;
}

/*
 * Method: set_any(set)
 * set: true or false.
 *
 * Sets whether the caps should be compatible with any media format.
 *
 * Returns: self.
 */
static VALUE
rg_set_any (VALUE self, VALUE set)
{
    if (RVAL2CBOOL(set)) {
        RGST_CAPS(self)->flags |= GST_CAPS_FLAGS_ANY;
    } else {
        RGST_CAPS(self)->flags &= ~GST_CAPS_FLAGS_ANY;
    }
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
rg_append (VALUE self, VALUE caps)
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
rg_append_structure (VALUE self, VALUE structure)
{
    gst_caps_append_structure (RGST_CAPS (self), RVAL2GST_STRUCT(structure));
    return self;
}

/* Method: size
 * Returns: the number of structures contained in the caps.
 */
static VALUE
rg_size (VALUE self)
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
rg_get_structure (VALUE self, VALUE index)
{
    GstStructure *structure = gst_caps_get_structure (RGST_CAPS (self),
                                                      FIX2INT (index));

    return GST_STRUCT2RVAL(structure);
}

static VALUE
rg_set_simple (VALUE self, VALUE field, VALUE value)
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
rg_any_p (VALUE self)
{
    return CBOOL2RVAL (gst_caps_is_any (RGST_CAPS (self)));
}

/* Method: empty?
 * Returns: whether the caps represents no media formats.
 */
static VALUE
rg_empty_p (VALUE self)
{
    return CBOOL2RVAL (gst_caps_is_empty (RGST_CAPS (self)));
}

/* Method: simple?
 * Returns: whether the caps contains multiple structure.
 */
static VALUE
rg_simple_p (VALUE self)
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
rg_fixed_p (VALUE self)
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
rg_equal_p (VALUE self, VALUE caps)
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
rg_equal_fixed_p (VALUE self, VALUE caps)
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
rg_always_compatible_p (VALUE self, VALUE caps)
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
rg_subset_p (VALUE self, VALUE caps)
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
rg_intersect (VALUE self, VALUE caps)
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
rg_union (VALUE self, VALUE caps)
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
rg_normalize (VALUE self)
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
rg_simplify_bang (VALUE self)
{
    return CBOOL2RVAL (gst_caps_do_simplify (SELF (self)));
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
rg_replace_bang (VALUE self, VALUE caps)
{
    GstCaps *current = RGST_CAPS (self);

    gst_caps_replace (&current, RGST_CAPS (caps));
    return self;
}

/* Method: to_string
 * Returns: a string representation of the current caps.
 */
static VALUE
rg_to_s (VALUE self)
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
rg_s_parse (VALUE self, VALUE string)
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
rg_subtract (VALUE self, VALUE caps)
{
    return RGST_CAPS_NEW (gst_caps_subtract (RGST_CAPS (self), RGST_CAPS
                                             (caps)));
}

static VALUE
rg_each(VALUE self)
{
    int i, size;
    GstCaps *caps;

    caps = SELF(self);
    size = gst_caps_get_size(caps);

    for (i = 0; i < size; i++) {
        rb_yield(GST_STRUCT2RVAL(gst_caps_get_structure(caps, i)));
    }

    return Qnil;
}

void
Init_gst_caps (VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS (GST_TYPE_CAPS, "Caps", mGst);

    rb_include_module(RG_TARGET_NAMESPACE, rb_mEnumerable);

    RG_DEF_SMETHOD(parse, 1);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(set_any, 1);
    RG_DEF_METHOD(append, 1);
    RG_DEF_METHOD(append_structure, 1);
    RG_DEF_METHOD(size, 0);
    RG_DEF_ALIAS("length", "size");
    RG_DEF_METHOD(get_structure, 1);
    RG_DEF_ALIAS("[]", "get_structure");
    RG_DEF_METHOD(set_simple, 2);
    RG_DEF_METHOD_P(any, 0);
    RG_DEF_METHOD_P(empty, 0);
    RG_DEF_METHOD_P(simple, 0);
    RG_DEF_ALIAS("chained?", "simple?");
    RG_DEF_METHOD_P(fixed, 0);
    RG_DEF_METHOD_P(equal, 1);
    RG_DEF_ALIAS("==", "equal?");
    RG_DEF_METHOD_P(equal_fixed, 1);
    RG_DEF_METHOD_P(always_compatible, 1);
    RG_DEF_METHOD_P(subset, 1);
    RG_DEF_METHOD(intersect, 1);
    RG_DEF_METHOD(union, 1);
    RG_DEF_METHOD(normalize, 0);
    RG_DEF_METHOD_BANG(simplify, 0);
    RG_DEF_METHOD_BANG(replace, 1);
    RG_DEF_METHOD(to_s, 0);
    RG_DEF_METHOD(subtract, 1);

    RG_DEF_METHOD(each, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

/* TODO:
    gst_caps_structure_fixate_field_nearest_int ()
    gst_caps_structure_fixate_field_nearest_double () */

}
