/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-stock-icons.c,v 1.1 2002/10/16 13:49:54 tkubo Exp $ */
/* based on libgnomeui/gnome-stock-icons.h */

/* Gnome::Scores widget for Ruby/GNOME2
 * Copyright (C) 2002 KUBO Takehiro <kubo@jiubao.org>
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

void
Init_gnome_stock_icons(mGnome)
    VALUE mGnome;
{
    VALUE mStock = rb_define_module_under(mGnome, "Stock");

    rb_define_const(mStock, "TIMER", rb_str_new2(GNOME_STOCK_TIMER));
    rb_define_const(mStock, "TIMER_STOP", rb_str_new2(GNOME_STOCK_TIMER_STOP));
    rb_define_const(mStock, "TRASH", rb_str_new2(GNOME_STOCK_TRASH));
    rb_define_const(mStock, "TRASH_FULL", rb_str_new2(GNOME_STOCK_TRASH_FULL));

    rb_define_const(mStock, "SCORES", rb_str_new2(GNOME_STOCK_SCORES));
    rb_define_const(mStock, "ABOUT", rb_str_new2(GNOME_STOCK_ABOUT));
    rb_define_const(mStock, "BLANK", rb_str_new2(GNOME_STOCK_BLANK));

    rb_define_const(mStock, "VOLUME", rb_str_new2(GNOME_STOCK_VOLUME));
    rb_define_const(mStock, "MIDI", rb_str_new2(GNOME_STOCK_MIDI));
    rb_define_const(mStock, "MIC", rb_str_new2(GNOME_STOCK_MIC));
    rb_define_const(mStock, "LINE_IN", rb_str_new2(GNOME_STOCK_LINE_IN));

    rb_define_const(mStock, "MAIL", rb_str_new2(GNOME_STOCK_MAIL));
    rb_define_const(mStock, "MAIL_RCV", rb_str_new2(GNOME_STOCK_MAIL_RCV));
    rb_define_const(mStock, "MAIL_SND", rb_str_new2(GNOME_STOCK_MAIL_SND));
    rb_define_const(mStock, "MAIL_RPL", rb_str_new2(GNOME_STOCK_MAIL_RPL));
    rb_define_const(mStock, "MAIL_FWD", rb_str_new2(GNOME_STOCK_MAIL_FWD));
    rb_define_const(mStock, "MAIL_NEW", rb_str_new2(GNOME_STOCK_MAIL_NEW));
    rb_define_const(mStock, "ATTACH", rb_str_new2(GNOME_STOCK_ATTACH));

    rb_define_const(mStock, "BOOK_RED", rb_str_new2(GNOME_STOCK_BOOK_RED));
    rb_define_const(mStock, "BOOK_GREEN", rb_str_new2(GNOME_STOCK_BOOK_GREEN));
    rb_define_const(mStock, "BOOK_BLUE", rb_str_new2(GNOME_STOCK_BOOK_BLUE));
    rb_define_const(mStock, "BOOK_YELLOW", rb_str_new2(GNOME_STOCK_BOOK_YELLOW));
    rb_define_const(mStock, "BOOK_OPEN", rb_str_new2(GNOME_STOCK_BOOK_OPEN));

    rb_define_const(mStock, "MULTIPLE_FILE", rb_str_new2(GNOME_STOCK_MULTIPLE_FILE));
    rb_define_const(mStock, "NOT", rb_str_new2(GNOME_STOCK_NOT));

    rb_define_const(mStock, "TABLE_BORDERS", rb_str_new2(GNOME_STOCK_TABLE_BORDERS));
    rb_define_const(mStock, "TABLE_FILL", rb_str_new2(GNOME_STOCK_TABLE_FILL));

    rb_define_const(mStock, "TEXT_INDENT", rb_str_new2(GNOME_STOCK_TEXT_INDENT));
    rb_define_const(mStock, "TEXT_UNINDENT", rb_str_new2(GNOME_STOCK_TEXT_UNINDENT));
    rb_define_const(mStock, "TEXT_BULLETED_LIST", rb_str_new2(GNOME_STOCK_TEXT_BULLETED_LIST));
    rb_define_const(mStock, "TEXT_NUMBERED_LIST", rb_str_new2(GNOME_STOCK_TEXT_NUMBERED_LIST));
}
