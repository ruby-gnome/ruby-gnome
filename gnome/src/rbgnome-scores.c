/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-scores.c,v 1.3 2002/09/25 17:17:24 tkubo Exp $ */

/* Gnome::Scores widget for Ruby/Gnome
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "rbgnome.h"

#define _SELF(self) GNOME_SCORES(RVAL2GOBJ(self))

/* This function has some important differences from the C function
 * gnome_scores_new().
 *
 * 1) There is no n_scores parameter. This is determined by Ruby
 *    dynamically, based on the size of the input arrays. Obviously,
 *    this means that the input arrays need to all be the same length.
 *
 * 2) The times parameter is an array of Time objects.
 */
static VALUE
scores_initialize(self, names, scores, times, clear)
    VALUE self, names, scores, times, clear;
{
    GtkWidget* result;
    gchar** c_names;
    gfloat* c_scores;
    time_t* c_times;
    int i, num_scores;

    Check_Type(names, T_ARRAY);
    c_names = ALLOCA_N(gchar*, RARRAY(names)->len+1);
    for (i = 0; i < RARRAY(names)->len; ++i) {
        c_names[i] = RVAL2CSTR(RARRAY(names)->ptr[i]);
    }

    Check_Type(scores, T_ARRAY);
    c_scores = ALLOCA_N(gfloat, RARRAY(scores)->len+1);
    for (i = 0; i < RARRAY(scores)->len; ++i) {
        c_scores[i] = NUM2LONG(RARRAY(scores)->ptr[i]);
    }

    Check_Type(times, T_ARRAY);
    c_times = ALLOCA_N(time_t, RARRAY(times)->len+1);
    for (i = 0; i < RARRAY(times)->len; ++i) {
        c_times[i] = NUM2INT(rb_funcall(RARRAY(times)->ptr[i], rb_intern("to_i"), 0));
    }

    if ((RARRAY(names)->len == RARRAY(scores)->len) &&
        (RARRAY(names)->len == RARRAY(times)->len)) {
        num_scores = RARRAY(names)->len;
    } else {
        rb_raise(rb_eArgError, "All arrays must be the same length.");
    }

    result = gnome_scores_new(num_scores,
                              c_names,
                              c_scores,
                              c_times,
                              NUM2INT(clear));
    RBGTK_INITIALIZE(self, result);
    return Qnil;
}

static VALUE
scores_set_logo_label(self, txt, font, color)
    VALUE self, txt, font, color;
{
    gnome_scores_set_logo_label(_SELF(self),
                                RVAL2CSTR(txt),
                                RVAL2CSTR(font),
                                (GdkColor*)RVAL2BOXED(color));
    return self;
}

static VALUE
scores_set_logo_pixmap(self, logo)
    VALUE self, logo;
{
    gnome_scores_set_logo_pixmap(_SELF(self),
                                 RVAL2CSTR(logo));
    return self;
}

static VALUE
scores_set_logo_widget(self, w)
    VALUE self, w;
{
    gnome_scores_set_logo_widget(_SELF(self),
                                 GTK_WIDGET(RVAL2GOBJ(w)));
    return self;
}

static VALUE
scores_set_color(self, pos, color)
    VALUE self, pos, color;
{
    gnome_scores_set_color(_SELF(self),
                           NUM2INT(pos),
                           (GdkColor*)RVAL2BOXED(color));
    return self;
}

static VALUE
scores_set_def_color(self, color)
    VALUE self, color;
{
    gnome_scores_set_def_color(_SELF(self), (GdkColor*)RVAL2BOXED(color));
    return self;
}

static VALUE
scores_set_colors(self, color)
    VALUE self, color;
{
    gnome_scores_set_colors(_SELF(self), (GdkColor*)RVAL2BOXED(color));
    return self;
}

static VALUE
scores_set_logo_label_title(self, txt)
    VALUE self, txt;
{
    gnome_scores_set_logo_label_title(_SELF(self), RVAL2CSTR(txt));
    return self;
}

static VALUE
scores_set_current_player(self, i)
    VALUE self, i;
{
    gnome_scores_set_current_player(_SELF(self), NUM2INT(i));
    return self;
}

void
Init_gnome_scores(mGnome)
    VALUE mGnome;
{
    VALUE gnoScores = G_DEF_CLASS(GNOME_TYPE_SCORES, "Scores", mGnome);
    rb_define_method(gnoScores, "initialize", scores_initialize, 4);
    rb_define_method(gnoScores, "set_logo_label", scores_set_logo_label, 3);
    rb_define_method(gnoScores, "set_logo_pixmap", scores_set_logo_pixmap, 1);
    rb_define_method(gnoScores, "set_logo_widget", scores_set_logo_widget, 1);
    rb_define_method(gnoScores, "set_color", scores_set_color, 2);
    rb_define_method(gnoScores, "set_def_color", scores_set_def_color, 1);
    rb_define_method(gnoScores, "set_colors", scores_set_colors, 1);
    rb_define_method(gnoScores, "set_logo_label_title", scores_set_logo_label_title, 1);
    rb_define_method(gnoScores, "set_current_player", scores_set_current_player, 1);
}
