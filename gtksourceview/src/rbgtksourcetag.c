/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksourcetag.c -

  $Author $
  $Date: 2004/08/05 18:13:49 $

  Copyright (C) 2004 Ruby-GNOME2 Project Team
  Copyright (C) 2003 Geoff Youngs, based on gtktextview.c by Masao Mutoh
************************************************/
#include "rbgtksourcemain.h"

/* TODO: Write documentation */

#define _SELF(self) (GTK_SOURCE_TAG(RVAL2GOBJ(self)))

static VALUE
syntaxtag_new (self, id, name, pattern_start, pattern_end)
	VALUE self, id, name, pattern_start, pattern_end;
{
	G_INITIALIZE (self,
		      gtk_syntax_tag_new (RVAL2CSTR (id), RVAL2CSTR (name),
					  RVAL2CSTR (pattern_start),
					  RVAL2CSTR (pattern_end)));

	return Qnil;
}

static VALUE
patterntag_new (self, id, name, pattern)
	VALUE self, id, name, pattern;
{
	G_INITIALIZE (self,
		      gtk_pattern_tag_new (RVAL2CSTR (id), RVAL2CSTR (name),
					   RVAL2CSTR (pattern)));
	return Qnil;
}

/* FIXME */
static VALUE
keywordlisttag_new (self, id, rname, rkeywords, rcase_sensitive,
		    rmatch_empty_string_at_beginning,
		    rmatch_empty_string_at_end, rbeginning_regex, rend_regex)
	VALUE self, id, rname, rkeywords, rcase_sensitive,
	    rmatch_empty_string_at_beginning, rmatch_empty_string_at_end,
	    rbeginning_regex, rend_regex;
{
	rb_notimplement ();
}

static VALUE
linecommenttag_new (self, id, name, pattern_start)
	VALUE self, id, name, pattern_start;
{
	return
	    GOBJ2RVAL (gtk_line_comment_tag_new
		       (RVAL2CSTR (id), RVAL2CSTR (name),
			RVAL2CSTR (pattern_start)));
}

static VALUE
stringtag_new (self, id, name, pattern_start, pattern_end, end_at_line_end)
	VALUE self, id, name, pattern_start, pattern_end, end_at_line_end;
{
	return GOBJ2RVAL (gtk_string_tag_new (RVAL2CSTR (id), RVAL2CSTR (name),
					      RVAL2CSTR (pattern_start),
					      RVAL2CSTR (pattern_end),
					      RTEST (end_at_line_end)));

	return self;
}

void
Init_gtk_sourcetag ()
{
	VALUE cSourceTag =
	    G_DEF_CLASS (GTK_TYPE_SOURCE_TAG, "SourceTag", mGtk);
	VALUE cSyntaxTag =
	    G_DEF_CLASS (GTK_TYPE_SYNTAX_TAG, "SyntaxTag", mGtk);
	VALUE cPatternTag =
	    G_DEF_CLASS (GTK_TYPE_PATTERN_TAG, "PatternTag", mGtk);

	rb_define_method (cPatternTag, "initialize", patterntag_new, 2);
	rb_define_method (cSyntaxTag, "initialize", syntaxtag_new, 3);

	rb_define_singleton_method (cSyntaxTag, "string_tag_new",
				    stringtag_new, 5);
	rb_define_singleton_method (cSyntaxTag, "line_comment_tag_new",
				    linecommenttag_new, 3);
	rb_define_singleton_method (cPatternTag, "keyword_list_tag_new",
				    keywordlisttag_new, 9);

	G_DEF_SETTERS (cSourceTag);
	G_DEF_SETTERS (cSyntaxTag);
	G_DEF_SETTERS (cPatternTag);
}
