/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-stock-icons.c,v 1.3 2003/02/02 12:51:06 tkubo Exp $ */
/* based on libgnomeui/gnome-stock-icons.h */

/* Gnome::Scores widget for Ruby/GNOME2
 * Copyright (C) 2002-2003 Ruby-GNOME2 Project Team
 * Copyright (C) 2002      KUBO Takehiro <kubo@jiubao.org>
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

#define CSTR2SYM(str) ID2SYM(rb_intern(str))

void
Init_gnome_stock_icons(mGnome)
    VALUE mGnome;
{
    VALUE mStock = rb_define_module_under(mGnome, "Stock");

    rb_define_const(mStock, "TIMER", CSTR2SYM(GNOME_STOCK_TIMER));
    rb_define_const(mStock, "TIMER_STOP", CSTR2SYM(GNOME_STOCK_TIMER_STOP));
    rb_define_const(mStock, "TRASH", CSTR2SYM(GNOME_STOCK_TRASH));
    rb_define_const(mStock, "TRASH_FULL", CSTR2SYM(GNOME_STOCK_TRASH_FULL));

    rb_define_const(mStock, "SCORES", CSTR2SYM(GNOME_STOCK_SCORES));
    rb_define_const(mStock, "ABOUT", CSTR2SYM(GNOME_STOCK_ABOUT));
    rb_define_const(mStock, "BLANK", CSTR2SYM(GNOME_STOCK_BLANK));

    rb_define_const(mStock, "VOLUME", CSTR2SYM(GNOME_STOCK_VOLUME));
    rb_define_const(mStock, "MIDI", CSTR2SYM(GNOME_STOCK_MIDI));
    rb_define_const(mStock, "MIC", CSTR2SYM(GNOME_STOCK_MIC));
    rb_define_const(mStock, "LINE_IN", CSTR2SYM(GNOME_STOCK_LINE_IN));

    rb_define_const(mStock, "MAIL", CSTR2SYM(GNOME_STOCK_MAIL));
    rb_define_const(mStock, "MAIL_RCV", CSTR2SYM(GNOME_STOCK_MAIL_RCV));
    rb_define_const(mStock, "MAIL_SND", CSTR2SYM(GNOME_STOCK_MAIL_SND));
    rb_define_const(mStock, "MAIL_RPL", CSTR2SYM(GNOME_STOCK_MAIL_RPL));
    rb_define_const(mStock, "MAIL_FWD", CSTR2SYM(GNOME_STOCK_MAIL_FWD));
    rb_define_const(mStock, "MAIL_NEW", CSTR2SYM(GNOME_STOCK_MAIL_NEW));
    rb_define_const(mStock, "ATTACH", CSTR2SYM(GNOME_STOCK_ATTACH));

    rb_define_const(mStock, "BOOK_RED", CSTR2SYM(GNOME_STOCK_BOOK_RED));
    rb_define_const(mStock, "BOOK_GREEN", CSTR2SYM(GNOME_STOCK_BOOK_GREEN));
    rb_define_const(mStock, "BOOK_BLUE", CSTR2SYM(GNOME_STOCK_BOOK_BLUE));
    rb_define_const(mStock, "BOOK_YELLOW", CSTR2SYM(GNOME_STOCK_BOOK_YELLOW));
    rb_define_const(mStock, "BOOK_OPEN", CSTR2SYM(GNOME_STOCK_BOOK_OPEN));

    rb_define_const(mStock, "MULTIPLE_FILE", CSTR2SYM(GNOME_STOCK_MULTIPLE_FILE));
    rb_define_const(mStock, "NOT", CSTR2SYM(GNOME_STOCK_NOT));

    rb_define_const(mStock, "TABLE_BORDERS", CSTR2SYM(GNOME_STOCK_TABLE_BORDERS));
    rb_define_const(mStock, "TABLE_FILL", CSTR2SYM(GNOME_STOCK_TABLE_FILL));

    rb_define_const(mStock, "TEXT_INDENT", CSTR2SYM(GNOME_STOCK_TEXT_INDENT));
    rb_define_const(mStock, "TEXT_UNINDENT", CSTR2SYM(GNOME_STOCK_TEXT_UNINDENT));
    rb_define_const(mStock, "TEXT_BULLETED_LIST", CSTR2SYM(GNOME_STOCK_TEXT_BULLETED_LIST));
    rb_define_const(mStock, "TEXT_NUMBERED_LIST", CSTR2SYM(GNOME_STOCK_TEXT_NUMBERED_LIST));
}
