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

/* Class: Gst::Index
 * Generates indexes on objects. 
 */

/* Class method: new
 * Returns: a newly allocated Gst::Index object.
 */
static VALUE
rb_gst_index_new (VALUE self)
{
    GstIndex *index = gst_index_new ();

    if (index != NULL)
        RBGST_INITIALIZE (self, index);
    return Qnil;
}

/* Method: readable?
 * Returns: whether the index can be read.
 */
static VALUE
rb_gst_index_is_readable (VALUE self)
{
    return CBOOL2RVAL (GST_INDEX_IS_READABLE (RGST_INDEX (self)));
}

/* Method: writable?
 * Returns: whether the index can be written.
 */
static VALUE
rb_gst_index_is_writable (VALUE self)
{
    return CBOOL2RVAL (GST_INDEX_IS_WRITABLE (RGST_INDEX (self)));
}

/*
 * Method: commit(id)
 * id: the writer that committed the index.
 *
 * Tells the index that the writer with the given ID is done with 
 * this index and is not going to write any more entries to it.
 *
 * Returns: self.
 */
static VALUE
rb_gst_index_commit (VALUE self, VALUE id)
{
    gst_index_commit (RGST_INDEX (self), FIX2INT (id));
    return self;
}

/* Method: group
 * Returns: the ID of the current group.
 */
static VALUE
rb_gst_index_get_group (VALUE self)
{
    return INT2FIX (gst_index_get_group (RGST_INDEX (self)));
}

/*
 * Method: new_group
 *
 * Creates a new group for the given index.  It will be set as the current
 * group.
 *
 * Returns: the ID of the newly created group.
 */
static VALUE
rb_gst_index_new_group (VALUE self)
{
    return INT2FIX (gst_index_new_group (RGST_INDEX (self)));
}

/*
 * Method: set_group(group)
 * group: the number of the group to set.
 *
 * Sets the current group number to the given argument.
 *
 * Returns: true if the operation succeeded, false if the group did not exist.
 */
static VALUE
rb_gst_index_set_group (VALUE self, VALUE group)
{
    return
        CBOOL2RVAL (gst_index_set_group (RGST_INDEX (self), FIX2INT (group)));
}

/* Method: certainty
 * Returns: the certainty of the index (see Gst::Index::Certainty).
 */
static VALUE
rb_gst_index_get_certainty (VALUE self)
{
    return GENUM2RVAL (gst_index_get_certainty (RGST_INDEX (self)),
                       GST_TYPE_INDEX_CERTAINTY);
}

/* 
 * Method: set_certainty(certainty)
 * certainty: the certainty to set (see Gst::Index::Certainty).
 *
 * Sets the certainty of the index.
 *
 * Returns: self.
 */
static VALUE
rb_gst_index_set_certainty (VALUE self, VALUE certainty)
{
    gst_index_set_certainty (RGST_INDEX (self),
                             RVAL2GENUM (certainty, GST_TYPE_INDEX_CERTAINTY));
    return self;
}

#if 0
static gboolean
__filter (GstIndex * index, GstIndexEntry * entry)
{

}

static VALUE
rb_gst_index_set_filter (VALUE self)
{
    /*
     * TODO 
     */
    return self;
}

static gboolean
__resolver (GstIndex * index, GstObject * writer, gchar ** writer_string,
            gpointer user_data)
{
    VALUE block = (VALUE) user_data;

    return
        RVAL2CBOOL (rb_funcall
                    (block, 2, RGST_INDEX_NEW (index),
                     RGST_OBJECT_NEW (writer)));
}

static VALUE
rb_gst_index_set_resolver (VALUE self)
{
    gst_index_set_resolver (RGST_INDEX (self), __resolver,
                            (gpointer) G_BLOCK_PROC ());
    return self;
}
#endif

/*
 * Method: add(id, *args)
 * id: the ID of the index writer.
 * args: additional parameters, see below.
 *
 * Adds an entry into the index.  The type of the entry depends of
 * the number and kind of additional parameters.
 *
 *	* For an ID type, args must be a String.
 *	* For a FORMAT type, args must be a Gst::Format.
 *	* For an ASSOCIATION type, args must contains an association flag (see Gst::Index::AssocFlags), a Gst::Format and a value for the format.
 *	* For an OBJECT type, well you must wait, because it is not yet implemented.
 *
 * Returns: a reference to the newly allocated entry in the index, as a Gst::EntryIndex object.
 */
static VALUE
rb_gst_index_add (int argc, VALUE * argv, VALUE self)
{
    GstIndexEntry *index_entry;
    VALUE id;

    if (argc == 2) {
        VALUE var;

        rb_scan_args (argc, argv, "2", &id, &var);

        index_entry = CLASS2GTYPE (CLASS_OF (var)) == GST_TYPE_FORMAT2
            ? gst_index_add_format (RGST_INDEX (self), FIX2INT (id),
                                    *RGST_FORMAT (var))
            : gst_index_add_id (RGST_INDEX (self), FIX2INT (id),
                                RVAL2CSTR (var));

    } else {
        VALUE flags, format, value;

        rb_scan_args (argc, argv, "4", &id, &flags, &format, &value);

        index_entry =
            gst_index_add_association (RGST_INDEX (self), FIX2INT (id),
                                       RVAL2GFLAGS (flags,
                                                    GST_TYPE_ASSOC_FLAGS),
                                       *RGST_FORMAT (format), NUM2ULL (value));
    }

    return index_entry != NULL ? RGST_INDEX_ENTRY_NEW (index_entry)
        : Qnil;
}

static gint
__compare (gconstpointer a, gconstpointer b, gpointer user_data)
{
    VALUE block = (VALUE) user_data;

    return
        FIX2INT (rb_funcall
                 (block, rb_intern ("call"), 2, RGST_INDEX_ENTRY_NEW (a),
                  RGST_INDEX_ENTRY_NEW (b)));
}

/*
 * Method: get_assoc_entry(id, method, flags, format, value) { ... }
 * id: the ID of the index writer.
 * method: the lookup method to use (see Gst::Index::LookupMethod).
 * flags: flags for the entry (see Gst::Index::AssocFlags).
 * format: a Gst::Format object.
 * value: the value to find.
 * 
 * Finds the given format/value in the index.  If a block is given, it will be
 * called as a compare function, passing references to 2 Gst::IndexEntry objects,
 * and waiting for a boolean as the return value.
 *
 * Returns: the entry associated with the value (as a Gst::IndexEntry object), or nil 
 * if the value is not found.
 */
static VALUE
rb_gst_index_get_assoc_entry (VALUE self, VALUE id, VALUE method, VALUE flags,
                              VALUE format, VALUE value)
{
    GstIndexEntry *index_entry;

    if (rb_block_given_p () == Qfalse)
        index_entry =
            gst_index_get_assoc_entry (RGST_INDEX (self), FIX2INT (id),
                                       RVAL2GENUM (method,
                                                   GST_TYPE_INDEX_LOOKUP_METHOD),
                                       RVAL2GFLAGS (flags,
                                                    GST_TYPE_ASSOC_FLAGS),
                                       *RGST_FORMAT (format), NUM2ULL (value));
    else
        index_entry =
            gst_index_get_assoc_entry_full (RGST_INDEX (self), FIX2INT (id),
                                            RVAL2GENUM (method,
                                                        GST_TYPE_INDEX_LOOKUP_METHOD),
                                            RVAL2GFLAGS (flags,
                                                         GST_TYPE_ASSOC_FLAGS),
                                            *RGST_FORMAT (format),
                                            NUM2ULL (value), __compare,
                                            (gpointer) G_BLOCK_PROC ());

    return index_entry != NULL ? RGST_INDEX_ENTRY_NEW (index_entry)
        : Qnil;
}

void
Init_gst_index (void)
{
    VALUE c = G_DEF_CLASS (GST_TYPE_INDEX, "Index", mGst);

    rb_define_method (c, "initialize", rb_gst_index_new, 0);
    rb_define_method (c, "readable?", rb_gst_index_is_readable, 0);
    rb_define_method (c, "writable?", rb_gst_index_is_writable, 0);
    rb_define_method (c, "commit", rb_gst_index_commit, 1);
    rb_define_method (c, "group", rb_gst_index_get_group, 0);
    rb_define_method (c, "new_group", rb_gst_index_new_group, 0);
    rb_define_method (c, "set_group", rb_gst_index_set_group, 1);
    rb_define_method (c, "certainty", rb_gst_index_get_certainty, 0);
    rb_define_method (c, "set_certainty", rb_gst_index_set_certainty, 1);
#if 0
    rb_define_method (c, "set_filter", rb_gst_index_set_filter, 0);
    rb_define_method (c, "set_resolver", rb_gst_index_set_resolver, 0);
#endif
    rb_define_method (c, "add", rb_gst_index_add, -1);
    rb_define_method (c, "get_assoc_entry", rb_gst_index_get_assoc_entry, 5);

    G_DEF_CLASS (GST_TYPE_INDEX_CERTAINTY, "Certainty", c);
    G_DEF_CONSTANTS (c, GST_TYPE_INDEX_CERTAINTY, "GST_INDEX_");
    G_DEF_CLASS (GST_TYPE_INDEX_LOOKUP_METHOD, "LookupMethod", c);
    G_DEF_CONSTANTS (c, GST_TYPE_INDEX_LOOKUP_METHOD, "GST_INDEX_");
    G_DEF_CLASS (GST_TYPE_ASSOC_FLAGS, "AssocFlags", c);
    G_DEF_CONSTANTS (c, GST_TYPE_ASSOC_FLAGS, "GST_");
    G_DEF_CLASS (GST_TYPE_INDEX_RESOLVER_METHOD, "ResolverMethod", c);
    G_DEF_CONSTANTS (c, GST_TYPE_INDEX_RESOLVER_METHOD, "GST_INDEX_");
    G_DEF_CLASS (GST_TYPE_INDEX_FLAGS, "Flags", c);
    G_DEF_CONSTANTS (c, GST_TYPE_INDEX_FLAGS, "GST_INDEX_");

    G_DEF_SETTERS (c);

    rb_undef_method (c, "resolver");
    rb_undef_method (c, "resolver=");
}
