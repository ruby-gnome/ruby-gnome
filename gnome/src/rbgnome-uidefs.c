/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-uidefs.c,v 1.5 2003/02/02 12:51:06 tkubo Exp $ */

/* Miscellaneous definitions for Ruby/Gnome
 * Copyright (C) 2002-2003 Ruby-GNOME2 Project Team
 * Copyright (C) 2001      Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "rbgnome.h"

void
Init_gnome_uidefs(mGnome)
    VALUE mGnome;
{
    /* All-purpose padding. If you always use these instead of making up 
       some arbitrary padding number that looks good on your screen, 
       people can change the "spaciousness" of the GUI globally. */
    rb_define_const(mGnome, "PAD", INT2FIX(GNOME_PAD));
    rb_define_const(mGnome, "PAD_SMALL", INT2FIX(GNOME_PAD_SMALL));
    rb_define_const(mGnome, "PAD_BIG", INT2FIX(GNOME_PAD_BIG));

    /* These are keybindings, in GnomeUIInfo format. USE THEM OR DIE! 
       Add to the list as well..
    */
    rb_define_const(mGnome, "KEY_NAME_QUIT", INT2FIX(GNOME_KEY_NAME_QUIT));
    rb_define_const(mGnome, "KEY_MOD_QUIT", INT2FIX(GNOME_KEY_MOD_QUIT));
    rb_define_const(mGnome, "KEY_NAME_CLOSE", INT2FIX(GNOME_KEY_NAME_CLOSE));
    rb_define_const(mGnome, "KEY_MOD_CLOSE", INT2FIX(GNOME_KEY_MOD_CLOSE));

    rb_define_const(mGnome, "KEY_NAME_CUT", INT2FIX(GNOME_KEY_NAME_CUT));
    rb_define_const(mGnome, "KEY_MOD_CUT", INT2FIX(GNOME_KEY_MOD_CUT));
    rb_define_const(mGnome, "KEY_NAME_COPY", INT2FIX(GNOME_KEY_NAME_COPY));
    rb_define_const(mGnome, "KEY_MOD_COPY", INT2FIX(GNOME_KEY_MOD_COPY));
    rb_define_const(mGnome, "KEY_NAME_PASTE", INT2FIX(GNOME_KEY_NAME_PASTE));
    rb_define_const(mGnome, "KEY_MOD_PASTE", INT2FIX(GNOME_KEY_MOD_PASTE));
    rb_define_const(mGnome, "KEY_NAME_SELECT_ALL", INT2FIX(GNOME_KEY_NAME_SELECT_ALL));
    rb_define_const(mGnome, "KEY_MOD_SELECT_ALL", INT2FIX(GNOME_KEY_MOD_SELECT_ALL));
    rb_define_const(mGnome, "KEY_NAME_CLEAR", INT2FIX(GNOME_KEY_NAME_CLEAR));
    rb_define_const(mGnome, "KEY_MOD_CLEAR", INT2FIX(GNOME_KEY_MOD_CLEAR));

    rb_define_const(mGnome, "KEY_NAME_UNDO", INT2FIX(GNOME_KEY_NAME_UNDO));
    rb_define_const(mGnome, "KEY_MOD_UNDO", INT2FIX(GNOME_KEY_MOD_UNDO));
    rb_define_const(mGnome, "KEY_NAME_REDO", INT2FIX(GNOME_KEY_NAME_REDO));
    rb_define_const(mGnome, "KEY_MOD_REDO", INT2FIX(GNOME_KEY_MOD_REDO));

    rb_define_const(mGnome, "KEY_NAME_SAVE", INT2FIX(GNOME_KEY_NAME_SAVE));
    rb_define_const(mGnome, "KEY_MOD_SAVE", INT2FIX(GNOME_KEY_MOD_SAVE));
    rb_define_const(mGnome, "KEY_NAME_OPEN", INT2FIX(GNOME_KEY_NAME_OPEN));
    rb_define_const(mGnome, "KEY_MOD_OPEN", INT2FIX(GNOME_KEY_MOD_OPEN));
    rb_define_const(mGnome, "KEY_NAME_SAVE_AS", INT2FIX(GNOME_KEY_NAME_SAVE_AS));
    rb_define_const(mGnome, "KEY_MOD_SAVE_AS", INT2FIX(GNOME_KEY_MOD_SAVE_AS));
    rb_define_const(mGnome, "KEY_NAME_NEW", INT2FIX(GNOME_KEY_NAME_NEW));
    rb_define_const(mGnome, "KEY_MOD_NEW", INT2FIX(GNOME_KEY_MOD_NEW));

    rb_define_const(mGnome, "KEY_NAME_PRINT", INT2FIX(GNOME_KEY_NAME_PRINT));
    rb_define_const(mGnome, "KEY_MOD_PRINT", INT2FIX(GNOME_KEY_MOD_PRINT));

    rb_define_const(mGnome, "KEY_NAME_PRINT_SETUP", INT2FIX(GNOME_KEY_NAME_PRINT_SETUP));
    rb_define_const(mGnome, "KEY_MOD_PRINT_SETUP", INT2FIX(GNOME_KEY_MOD_PRINT_SETUP));

    rb_define_const(mGnome, "KEY_NAME_FIND", INT2FIX(GNOME_KEY_NAME_FIND));
    rb_define_const(mGnome, "KEY_MOD_FIND", INT2FIX(GNOME_KEY_MOD_FIND));
    rb_define_const(mGnome, "KEY_NAME_FIND_AGAIN", INT2FIX(GNOME_KEY_NAME_FIND_AGAIN));
    rb_define_const(mGnome, "KEY_MOD_FIND_AGAIN", INT2FIX(GNOME_KEY_MOD_FIND_AGAIN));
    rb_define_const(mGnome, "KEY_NAME_REPLACE", INT2FIX(GNOME_KEY_NAME_REPLACE));
    rb_define_const(mGnome, "KEY_MOD_REPLACE", INT2FIX(GNOME_KEY_MOD_REPLACE));

    rb_define_const(mGnome, "KEY_NAME_NEW_WINDOW", INT2FIX(GNOME_KEY_NAME_NEW_WINDOW));
    rb_define_const(mGnome, "KEY_MOD_NEW_WINDOW", INT2FIX(GNOME_KEY_MOD_NEW_WINDOW));
    rb_define_const(mGnome, "KEY_NAME_CLOSE_WINDOW", INT2FIX(GNOME_KEY_NAME_CLOSE_WINDOW));
    rb_define_const(mGnome, "KEY_MOD_CLOSE_WINDOW", INT2FIX(GNOME_KEY_MOD_CLOSE_WINDOW));

    rb_define_const(mGnome, "KEY_NAME_REDO_MOVE", INT2FIX(GNOME_KEY_NAME_REDO_MOVE));
    rb_define_const(mGnome, "KEY_MOD_REDO_MOVE", INT2FIX(GNOME_KEY_MOD_REDO_MOVE));
    rb_define_const(mGnome, "KEY_NAME_UNDO_MOVE", INT2FIX(GNOME_KEY_NAME_UNDO_MOVE));
    rb_define_const(mGnome, "KEY_MOD_UNDO_MOVE", INT2FIX(GNOME_KEY_MOD_UNDO_MOVE));

    rb_define_const(mGnome, "KEY_NAME_PAUSE_GAME", INT2FIX(GNOME_KEY_NAME_PAUSE_GAME));
    rb_define_const(mGnome, "KEY_MOD_PAUSE_GAME", INT2FIX(GNOME_KEY_MOD_PAUSE_GAME));
    rb_define_const(mGnome, "KEY_NAME_NEW_GAME", INT2FIX(GNOME_KEY_NAME_NEW_GAME));
    rb_define_const(mGnome, "KEY_MOD_NEW_GAME", INT2FIX(GNOME_KEY_MOD_NEW_GAME));
}
