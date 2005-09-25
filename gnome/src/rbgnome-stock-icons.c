/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-stock-icons.c,v 1.4 2005/09/25 17:53:07 mutoh Exp $ */
/* based on libgnomeui/gnome-stock-icons.h */

/* Gnome::Scores widget for Ruby/GNOME2
 * Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
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

/* Deprecated.
#define     GNOME_STOCK_PIXMAP_NEW
#define     GNOME_STOCK_PIXMAP_OPEN
#define     GNOME_STOCK_PIXMAP_CLOSE
#define     GNOME_STOCK_PIXMAP_REVERT
#define     GNOME_STOCK_PIXMAP_SAVE
#define     GNOME_STOCK_PIXMAP_SAVE_AS
#define     GNOME_STOCK_PIXMAP_CUT
#define     GNOME_STOCK_PIXMAP_COPY
#define     GNOME_STOCK_PIXMAP_PASTE
#define     GNOME_STOCK_PIXMAP_CLEAR
#define     GNOME_STOCK_PIXMAP_PROPERTIES
#define     GNOME_STOCK_PIXMAP_PREFERENCES
#define     GNOME_STOCK_PIXMAP_HELP
#define     GNOME_STOCK_PIXMAP_SCORES
#define     GNOME_STOCK_PIXMAP_PRINT
#define     GNOME_STOCK_PIXMAP_SEARCH
#define     GNOME_STOCK_PIXMAP_SRCHRPL
#define     GNOME_STOCK_PIXMAP_BACK
#define     GNOME_STOCK_PIXMAP_FORWARD
#define     GNOME_STOCK_PIXMAP_FIRST
#define     GNOME_STOCK_PIXMAP_LAST
#define     GNOME_STOCK_PIXMAP_HOME
#define     GNOME_STOCK_PIXMAP_STOP
#define     GNOME_STOCK_PIXMAP_REFRESH
#define     GNOME_STOCK_PIXMAP_UNDO
#define     GNOME_STOCK_PIXMAP_REDO
#define     GNOME_STOCK_PIXMAP_TIMER
#define     GNOME_STOCK_PIXMAP_TIMER_STOP
#define     GNOME_STOCK_PIXMAP_MAIL
#define     GNOME_STOCK_PIXMAP_MAIL_RCV
#define     GNOME_STOCK_PIXMAP_MAIL_SND
#define     GNOME_STOCK_PIXMAP_MAIL_RPL
#define     GNOME_STOCK_PIXMAP_MAIL_FWD
#define     GNOME_STOCK_PIXMAP_MAIL_NEW
#define     GNOME_STOCK_PIXMAP_TRASH
#define     GNOME_STOCK_PIXMAP_TRASH_FULL
#define     GNOME_STOCK_PIXMAP_UNDELETE
#define     GNOME_STOCK_PIXMAP_SPELLCHECK
#define     GNOME_STOCK_PIXMAP_MIC
#define     GNOME_STOCK_PIXMAP_LINE_IN
#define     GNOME_STOCK_PIXMAP_CDROM
#define     GNOME_STOCK_PIXMAP_VOLUME
#define     GNOME_STOCK_PIXMAP_MIDI
#define     GNOME_STOCK_PIXMAP_BOOK_RED
#define     GNOME_STOCK_PIXMAP_BOOK_GREEN
#define     GNOME_STOCK_PIXMAP_BOOK_BLUE
#define     GNOME_STOCK_PIXMAP_BOOK_YELLOW
#define     GNOME_STOCK_PIXMAP_BOOK_OPEN
#define     GNOME_STOCK_PIXMAP_ABOUT
#define     GNOME_STOCK_PIXMAP_QUIT
#define     GNOME_STOCK_PIXMAP_MULTIPLE
#define     GNOME_STOCK_PIXMAP_NOT
#define     GNOME_STOCK_PIXMAP_CONVERT
#define     GNOME_STOCK_PIXMAP_JUMP_TO
#define     GNOME_STOCK_PIXMAP_UP
#define     GNOME_STOCK_PIXMAP_DOWN
#define     GNOME_STOCK_PIXMAP_TOP
#define     GNOME_STOCK_PIXMAP_BOTTOM
#define     GNOME_STOCK_PIXMAP_ATTACH
#define     GNOME_STOCK_PIXMAP_INDEX
#define     GNOME_STOCK_PIXMAP_FONT
#define     GNOME_STOCK_PIXMAP_EXEC
#define     GNOME_STOCK_PIXMAP_ALIGN_LEFT
#define     GNOME_STOCK_PIXMAP_ALIGN_RIGHT
#define     GNOME_STOCK_PIXMAP_ALIGN_CENTER
#define     GNOME_STOCK_PIXMAP_ALIGN_JUSTIFY
#define     GNOME_STOCK_PIXMAP_TEXT_BOLD
#define     GNOME_STOCK_PIXMAP_TEXT_ITALIC
#define     GNOME_STOCK_PIXMAP_TEXT_UNDERLINE
#define     GNOME_STOCK_PIXMAP_TEXT_STRIKEOUT
#define     GNOME_STOCK_PIXMAP_TEXT_INDENT
#define     GNOME_STOCK_PIXMAP_TEXT_UNINDENT
#define     GNOME_STOCK_PIXMAP_EXIT
#define     GNOME_STOCK_PIXMAP_COLORSELECTOR
#define     GNOME_STOCK_PIXMAP_ADD
#define     GNOME_STOCK_PIXMAP_REMOVE
#define     GNOME_STOCK_PIXMAP_TABLE_BORDERS
#define     GNOME_STOCK_PIXMAP_TABLE_FILL
#define     GNOME_STOCK_PIXMAP_TEXT_BULLETED_LIST
#define     GNOME_STOCK_PIXMAP_TEXT_NUMBERED_LIST
#define     GNOME_STOCK_MENU_BLANK
#define     GNOME_STOCK_MENU_NEW
#define     GNOME_STOCK_MENU_SAVE
#define     GNOME_STOCK_MENU_SAVE_AS
#define     GNOME_STOCK_MENU_REVERT
#define     GNOME_STOCK_MENU_OPEN
#define     GNOME_STOCK_MENU_CLOSE
#define     GNOME_STOCK_MENU_QUIT
#define     GNOME_STOCK_MENU_CUT
#define     GNOME_STOCK_MENU_COPY
#define     GNOME_STOCK_MENU_PASTE
#define     GNOME_STOCK_MENU_PROP
#define     GNOME_STOCK_MENU_PREF
#define     GNOME_STOCK_MENU_ABOUT
#define     GNOME_STOCK_MENU_SCORES
#define     GNOME_STOCK_MENU_UNDO
#define     GNOME_STOCK_MENU_REDO
#define     GNOME_STOCK_MENU_PRINT
#define     GNOME_STOCK_MENU_SEARCH
#define     GNOME_STOCK_MENU_SRCHRPL
#define     GNOME_STOCK_MENU_BACK
#define     GNOME_STOCK_MENU_FORWARD
#define     GNOME_STOCK_MENU_FIRST
#define     GNOME_STOCK_MENU_LAST
#define     GNOME_STOCK_MENU_HOME
#define     GNOME_STOCK_MENU_STOP
#define     GNOME_STOCK_MENU_REFRESH
#define     GNOME_STOCK_MENU_MAIL
#define     GNOME_STOCK_MENU_MAIL_RCV
#define     GNOME_STOCK_MENU_MAIL_SND
#define     GNOME_STOCK_MENU_MAIL_RPL
#define     GNOME_STOCK_MENU_MAIL_FWD
#define     GNOME_STOCK_MENU_MAIL_NEW
#define     GNOME_STOCK_MENU_TRASH
#define     GNOME_STOCK_MENU_TRASH_FULL
#define     GNOME_STOCK_MENU_UNDELETE
#define     GNOME_STOCK_MENU_TIMER
#define     GNOME_STOCK_MENU_TIMER_STOP
#define     GNOME_STOCK_MENU_SPELLCHECK
#define     GNOME_STOCK_MENU_MIC
#define     GNOME_STOCK_MENU_LINE_IN
#define     GNOME_STOCK_MENU_CDROM
#define     GNOME_STOCK_MENU_VOLUME
#define     GNOME_STOCK_MENU_MIDI
#define     GNOME_STOCK_MENU_BOOK_RED
#define     GNOME_STOCK_MENU_BOOK_GREEN
#define     GNOME_STOCK_MENU_BOOK_BLUE
#define     GNOME_STOCK_MENU_BOOK_YELLOW
#define     GNOME_STOCK_MENU_BOOK_OPEN
#define     GNOME_STOCK_MENU_CONVERT
#define     GNOME_STOCK_MENU_JUMP_TO
#define     GNOME_STOCK_MENU_UP
#define     GNOME_STOCK_MENU_DOWN
#define     GNOME_STOCK_MENU_TOP
#define     GNOME_STOCK_MENU_BOTTOM
#define     GNOME_STOCK_MENU_ATTACH
#define     GNOME_STOCK_MENU_INDEX
#define     GNOME_STOCK_MENU_FONT
#define     GNOME_STOCK_MENU_EXEC
#define     GNOME_STOCK_MENU_ALIGN_LEFT
#define     GNOME_STOCK_MENU_ALIGN_RIGHT
#define     GNOME_STOCK_MENU_ALIGN_CENTER
#define     GNOME_STOCK_MENU_ALIGN_JUSTIFY
#define     GNOME_STOCK_MENU_TEXT_BOLD
#define     GNOME_STOCK_MENU_TEXT_ITALIC
#define     GNOME_STOCK_MENU_TEXT_UNDERLINE
#define     GNOME_STOCK_MENU_TEXT_STRIKEOUT
#define     GNOME_STOCK_MENU_EXIT
*/
}
