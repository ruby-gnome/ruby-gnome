/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksourcetagtable.c -

  $Author: lrz $
  $Date: 2004/08/05 18:13:49 $

  Copyright (C) 2004 Ruby-GNOME2 Project Team
  Copyright (C) 2003 Geoff Youngs, based on gtktextview.c by Masao Mutoh
************************************************/
#include "rbgtksourcemain.h"

/* Class: Gtk::SourceTagTable
 * A source tag table.
 */
#define _SELF(self) (GTK_SOURCE_TAG_TABLE(RVAL2GOBJ(self)))

/* Class method: new
 * Creates a new GtkSourceTagTable. The table contains no tags by default.
 * Returns: a newly created Gtk::SourceTagTable object.
 */
static VALUE
sourcetagtable_new (self)
	VALUE self;
{
	G_INITIALIZE (self, gtk_source_tag_table_new ());
	return Qnil;
}

/* Method: remove_source_tags
 * Removes all the source tags from the table. 
 * Returns: self.
 */
static VALUE
sourcetagtable_remove_source_tags (self)
	VALUE self;
{
	gtk_source_tag_table_remove_source_tags (_SELF (self));
	return self;
}

/*
 * Method: add_tags(*tags)
 * tags: a list of Gtk::TextTag objects.
 *
 * Adds a list of tag to the table. The added tags are assigned the highest
 * priority in the table.
 *
 * If a tag is already present in table or has the same name as an already-added
 * tag, then it is not added to the table. 
 *
 * Returns: self.
 */
static VALUE
sourcetagtable_add_tags (self, list)
	VALUE self, list;
{
	GSList *tags = NULL;
	int i;

	Check_Type (list, T_ARRAY);

	for (i = 0; i < RARRAY_LEN(list); i++)
		tags =
		    g_slist_append (tags, RVAL2GOBJ(RARRAY_PTR(list)[i]));

	gtk_source_tag_table_add_tags (_SELF (self), tags);

	g_slist_free (tags);

	return self;
}

void
Init_gtk_sourcetagtable ()
{
	VALUE cSourceTagTable =
	    G_DEF_CLASS (GTK_TYPE_SOURCE_TAG_TABLE, "SourceTagTable", mGtk);

	rb_define_method (cSourceTagTable, "remove_source_tags",
			  sourcetagtable_remove_source_tags, 0);
	rb_define_method (cSourceTagTable, "add_tags", sourcetagtable_add_tags,
			  -2);
	rb_define_alias (cSourceTagTable, "<<", "add_tags");
	rb_define_method (cSourceTagTable, "initialize", sourcetagtable_new,
			  0);

	G_DEF_SETTERS (cSourceTagTable);
}
