/* $Id: rbgnome-spell.c,v 1.2 2002/05/19 15:48:28 mutoh Exp $ */

/* Gnome::Spell widget for Ruby/Gnome
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

VALUE gnoSpell;

static VALUE
spell_initialize(self)
    VALUE self;
{
    GtkWidget *spell;
    spell = gnome_spell_new();
    set_widget(self, spell);
    return Qnil;
}

/* check str for mispelled words  returns 0 if words are ok */
static VALUE
spell_check(self, str)
    VALUE self, str;
{
    VALUE retval;
    gint result;
    result = gnome_spell_check(GNOME_SPELL(get_widget(self)),
			       STR2CSTR(str));
    if (result == -1) {
        rb_raise(rb_eRuntimeError, "Gnome::Spell#spell_check failed");
        retval = Qnil;
    } else if (result == 1) {
        retval = Qtrue;
    } else {
        retval = Qfalse;
    }
    return retval;
}

/* accept word for this session only */
static VALUE
spell_accept(self, word)
    VALUE self, word;
{
    gnome_spell_accept(GNOME_SPELL(get_widget(self)), STR2CSTR(word));
    return self;
}

/* insert word in personal dictionary */
static VALUE
spell_insert(self, word, lowercase)
    VALUE self, word, lowercase;
{
    gnome_spell_insert(GNOME_SPELL(get_widget(self)),
		       STR2CSTR(word), RTEST(lowercase));
    return self;
}

/* go for the next word */
static VALUE
spell_next(self)
    VALUE self;
{
    return (gnome_spell_next(GNOME_SPELL(get_widget(self))) ? Qtrue : Qfalse);
}

/* kill the ispell process */
static VALUE
spell_kill(self)
    VALUE self;
{
    gnome_spell_kill(GNOME_SPELL(get_widget(self)));
    return self;
}

void
Init_gnome_spell()
{
    gnoSpell = rb_define_class_under(mGnome, "Spell", gVBox);

    /* Instance methods */
    rb_define_method(gnoSpell, "initialize", spell_initialize, 0);
    rb_define_method(gnoSpell, "check", spell_check, 1);
    rb_define_method(gnoSpell, "accept", spell_accept, 1);
    rb_define_method(gnoSpell, "insert", spell_insert, 2);
    rb_define_method(gnoSpell, "next", spell_next, 0);
    rb_define_method(gnoSpell, "kill", spell_kill, 0);

    /* Signals */
    rb_define_const(gnoSpell, "SIGNAL_FOUND_WORD",
		    rb_str_new2("found_word"));
    rb_define_const(gnoSpell, "SIGNAL_HANDLED_WORD",
		    rb_str_new2("handled_word"));
}
