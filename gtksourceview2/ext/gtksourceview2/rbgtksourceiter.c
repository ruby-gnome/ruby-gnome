/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Ruby-GNOME2 Project Team
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

#include "rbgtksourcemain.h"

#define RG_TARGET_NAMESPACE cTextIter
#define _SELF(s) ((GtkTextIter*)RVAL2BOXED(s, GTK_TYPE_TEXT_ITER))
#define ITR2RVAL(i) (BOXED2RVAL(i, GTK_TYPE_TEXT_ITER))

/* Method: forward_search(str, flags, limit)
 * str: the search string (string)
 * flags: flags affecting the search (Gtk::SourceSearchFlags)
 * limit: location of last possible match start, or NULL for start of buffer
 * Returns: locations of start and end of match.
 */
static VALUE
rg_forward_search(int argc, VALUE *argv, VALUE self)
{
    GtkTextIter m_start, m_end;
    VALUE str, flags, limit;
    VALUE ret = Qnil;

    rb_scan_args (argc, argv, "21", &str, &flags, &limit);

    if (rb_obj_is_kind_of
        (flags, GTYPE2CLASS (GTK_TYPE_SOURCE_SEARCH_FLAGS))) {
        if (gtk_source_iter_forward_search
            (_SELF (self), RVAL2CSTR (str),
             RVAL2GFLAGS (flags, GTK_TYPE_SOURCE_SEARCH_FLAGS),
             &m_start, &m_end, NIL_P (limit) ? NULL : _SELF (limit)))
            ret =
                rb_ary_new3 (2, ITR2RVAL (&m_start),
                     ITR2RVAL (&m_end));
    } else
        if (rb_obj_is_kind_of
        (flags, GTYPE2CLASS (GTK_TYPE_TEXT_SEARCH_FLAGS))) {
        if (gtk_text_iter_forward_search
            (_SELF (self), RVAL2CSTR (str),
             RVAL2GFLAGS (flags, GTK_TYPE_TEXT_SEARCH_FLAGS), &m_start,
             &m_end, NIL_P (limit) ? NULL : _SELF (limit)))
            ret =
                rb_ary_new3 (2, ITR2RVAL (&m_start),
                     ITR2RVAL (&m_end));
    }
    return ret;
}

/* Method: backward_search(str, flags, limit)
 * str: the search string (string)
 * flags: flags affecting the search (Gtk::SourceSearchFlags)
 * limit: location of last possible match end, or NULL for end of buffer
 * Returns: locations of start and end of match.
 */
static VALUE
rg_backward_search(int argc, VALUE *argv, VALUE self)
{
    GtkTextIter m_start, m_end;
    VALUE str, flags, limit;
    VALUE ret = Qnil;

    rb_scan_args (argc, argv, "21", &str, &flags, &limit);
    if (rb_obj_is_kind_of
        (flags, GTYPE2CLASS (GTK_TYPE_SOURCE_SEARCH_FLAGS))) {
        if (gtk_source_iter_backward_search
            (_SELF (self), RVAL2CSTR (str),
             RVAL2GFLAGS (flags, GTK_TYPE_SOURCE_SEARCH_FLAGS),
             &m_start, &m_end, NIL_P (limit) ? NULL : _SELF (limit)))
            ret =
                rb_ary_new3 (2, ITR2RVAL (&m_start),
                     ITR2RVAL (&m_end));
    } else
        if (rb_obj_is_kind_of
        (flags, GTYPE2CLASS (GTK_TYPE_TEXT_SEARCH_FLAGS))) {
        if (gtk_text_iter_backward_search
            (_SELF (self), RVAL2CSTR (str),
             RVAL2GFLAGS (flags, GTK_TYPE_TEXT_SEARCH_FLAGS), &m_start,
             &m_end, NIL_P (limit) ? NULL : _SELF (limit)))
            ret =
                rb_ary_new3 (2, ITR2RVAL (&m_start),
                     ITR2RVAL (&m_end));
    }
    return ret;
}

void
Init_gtk_sourceiter (VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = GTYPE2CLASS (GTK_TYPE_TEXT_ITER);

    /*
     * They are override original Gtk::TextIter#[for|back]ward_search
     */
    RG_REPLACE_ACTION(forward_search, -1);
    RG_REPLACE_ACTION(backward_search, -1);

    G_DEF_CLASS(GTK_TYPE_SOURCE_SEARCH_FLAGS, "SourceSearchFlags", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_SOURCE_SEARCH_FLAGS, "GTK_");
}
