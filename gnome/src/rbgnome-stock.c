/* $Id: rbgnome-stock.c,v 1.2 2002/05/19 15:48:28 mutoh Exp $ */

/* Gnome::Stock class for Ruby/Gnome
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

VALUE gnoStock;

static VALUE
stock_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    GtkWidget* stock;
    VALUE icon;

    if (rb_scan_args(argc, argv, "01", &icon) == 0) {
        stock = gnome_stock_new();
    } else {
        stock = gnome_stock_new_with_icon(STR2CSTR(icon));
    }
    set_widget(self, stock);
    return Qnil;
}

static VALUE
stock_set_icon(self, icon)
    VALUE self, icon;
{
    gboolean result = gnome_stock_set_icon(GNOME_STOCK(get_widget(self)),
                                            STR2CSTR(icon));
    return result?Qtrue:Qfalse;
}

static VALUE
stock_pixmap_widget(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE window, icon, width, height;
    GtkWidget *stock;

    if (rb_scan_args(argc, argv, "22", &window, &icon, &width, &height) == 2) {
        stock = gnome_stock_pixmap_widget(get_widget(window),
                                          STR2CSTR(icon));
    } else {
        stock = gnome_stock_pixmap_widget_at_size(get_widget(window),
                                                  STR2CSTR(icon),
                                                  NUM2INT(width),
                                                  NUM2INT(height));
    }
    return make_widget(gnoStock, stock);
}

void
Init_gnome_stock()
{
    gnoStock = rb_define_class_under(mGnome, "Stock", gnoPixmap);

    rb_define_const(gnoStock, "PIXMAP_NEW",
            rb_str_new2(GNOME_STOCK_PIXMAP_NEW));
    rb_define_const(gnoStock, "PIXMAP_OPEN",
            rb_str_new2(GNOME_STOCK_PIXMAP_OPEN));
    rb_define_const(gnoStock, "PIXMAP_CLOSE",
            rb_str_new2(GNOME_STOCK_PIXMAP_CLOSE));
    rb_define_const(gnoStock, "PIXMAP_REVERT",
            rb_str_new2(GNOME_STOCK_PIXMAP_REVERT));
    rb_define_const(gnoStock, "PIXMAP_SAVE",
            rb_str_new2(GNOME_STOCK_PIXMAP_SAVE));
    rb_define_const(gnoStock, "PIXMAP_SAVE_AS",
            rb_str_new2(GNOME_STOCK_PIXMAP_SAVE_AS));
    rb_define_const(gnoStock, "PIXMAP_CUT",
            rb_str_new2(GNOME_STOCK_PIXMAP_CUT));
    rb_define_const(gnoStock, "PIXMAP_COPY",
            rb_str_new2(GNOME_STOCK_PIXMAP_COPY));
    rb_define_const(gnoStock, "PIXMAP_PASTE",
            rb_str_new2(GNOME_STOCK_PIXMAP_PASTE));
    rb_define_const(gnoStock, "PIXMAP_CLEAR",
            rb_str_new2(GNOME_STOCK_PIXMAP_CLEAR));
    rb_define_const(gnoStock, "PIXMAP_PROPERTIES",
            rb_str_new2(GNOME_STOCK_PIXMAP_PROPERTIES));
    rb_define_const(gnoStock, "PIXMAP_PREFERENCES",
            rb_str_new2(GNOME_STOCK_PIXMAP_PREFERENCES));
    rb_define_const(gnoStock, "PIXMAP_HELP",
            rb_str_new2(GNOME_STOCK_PIXMAP_HELP));
    rb_define_const(gnoStock, "PIXMAP_SCORES",
            rb_str_new2(GNOME_STOCK_PIXMAP_SCORES));
    rb_define_const(gnoStock, "PIXMAP_PRINT",
            rb_str_new2(GNOME_STOCK_PIXMAP_PRINT));
    rb_define_const(gnoStock, "PIXMAP_SEARCH",
            rb_str_new2(GNOME_STOCK_PIXMAP_SEARCH));
    rb_define_const(gnoStock, "PIXMAP_SRCHRPL",
            rb_str_new2(GNOME_STOCK_PIXMAP_SRCHRPL));
    rb_define_const(gnoStock, "PIXMAP_BACK",
            rb_str_new2(GNOME_STOCK_PIXMAP_BACK));
    rb_define_const(gnoStock, "PIXMAP_FORWARD",
            rb_str_new2(GNOME_STOCK_PIXMAP_FORWARD));
    rb_define_const(gnoStock, "PIXMAP_FIRST",
            rb_str_new2(GNOME_STOCK_PIXMAP_FIRST));
    rb_define_const(gnoStock, "PIXMAP_LAST",
            rb_str_new2(GNOME_STOCK_PIXMAP_LAST));
    rb_define_const(gnoStock, "PIXMAP_HOME",
            rb_str_new2(GNOME_STOCK_PIXMAP_HOME));
    rb_define_const(gnoStock, "PIXMAP_STOP",
            rb_str_new2(GNOME_STOCK_PIXMAP_STOP));
    rb_define_const(gnoStock, "PIXMAP_REFRESH",
            rb_str_new2(GNOME_STOCK_PIXMAP_REFRESH));
    rb_define_const(gnoStock, "PIXMAP_UNDO",
            rb_str_new2(GNOME_STOCK_PIXMAP_UNDO));
    rb_define_const(gnoStock, "PIXMAP_REDO",
            rb_str_new2(GNOME_STOCK_PIXMAP_REDO));
    rb_define_const(gnoStock, "PIXMAP_TIMER",
            rb_str_new2(GNOME_STOCK_PIXMAP_TIMER));
    rb_define_const(gnoStock, "PIXMAP_TIMER_STOP",
            rb_str_new2(GNOME_STOCK_PIXMAP_TIMER_STOP));
    rb_define_const(gnoStock, "PIXMAP_MAIL",
            rb_str_new2(GNOME_STOCK_PIXMAP_MAIL));
    rb_define_const(gnoStock, "PIXMAP_MAIL_RCV",
            rb_str_new2(GNOME_STOCK_PIXMAP_MAIL_RCV));
    rb_define_const(gnoStock, "PIXMAP_MAIL_SND",
            rb_str_new2(GNOME_STOCK_PIXMAP_MAIL_SND));
    rb_define_const(gnoStock, "PIXMAP_MAIL_RPL",
            rb_str_new2(GNOME_STOCK_PIXMAP_MAIL_RPL));
    rb_define_const(gnoStock, "PIXMAP_MAIL_FWD",
            rb_str_new2(GNOME_STOCK_PIXMAP_MAIL_FWD));
    rb_define_const(gnoStock, "PIXMAP_MAIL_NEW",
            rb_str_new2(GNOME_STOCK_PIXMAP_MAIL_NEW));
    rb_define_const(gnoStock, "PIXMAP_TRASH",
            rb_str_new2(GNOME_STOCK_PIXMAP_TRASH));
    rb_define_const(gnoStock, "PIXMAP_TRASH_FULL",
            rb_str_new2(GNOME_STOCK_PIXMAP_TRASH_FULL));
    rb_define_const(gnoStock, "PIXMAP_UNDELETE",
            rb_str_new2(GNOME_STOCK_PIXMAP_UNDELETE));
    rb_define_const(gnoStock, "PIXMAP_SPELLCHECK",
            rb_str_new2(GNOME_STOCK_PIXMAP_SPELLCHECK));
    rb_define_const(gnoStock, "PIXMAP_MIC",
            rb_str_new2(GNOME_STOCK_PIXMAP_MIC));
    rb_define_const(gnoStock, "PIXMAP_LINE_IN",
            rb_str_new2(GNOME_STOCK_PIXMAP_LINE_IN));
    rb_define_const(gnoStock, "PIXMAP_CDROM",
            rb_str_new2(GNOME_STOCK_PIXMAP_CDROM));
    rb_define_const(gnoStock, "PIXMAP_VOLUME",
            rb_str_new2(GNOME_STOCK_PIXMAP_VOLUME));
    rb_define_const(gnoStock, "PIXMAP_MIDI",
            rb_str_new2(GNOME_STOCK_PIXMAP_MIDI));
    rb_define_const(gnoStock, "PIXMAP_BOOK_RED",
            rb_str_new2(GNOME_STOCK_PIXMAP_BOOK_RED));
    rb_define_const(gnoStock, "PIXMAP_BOOK_GREEN",
            rb_str_new2(GNOME_STOCK_PIXMAP_BOOK_GREEN));
    rb_define_const(gnoStock, "PIXMAP_BOOK_BLUE",
            rb_str_new2(GNOME_STOCK_PIXMAP_BOOK_BLUE));
    rb_define_const(gnoStock, "PIXMAP_BOOK_YELLOW",
            rb_str_new2(GNOME_STOCK_PIXMAP_BOOK_YELLOW));
    rb_define_const(gnoStock, "PIXMAP_BOOK_OPEN",
            rb_str_new2(GNOME_STOCK_PIXMAP_BOOK_OPEN));
    rb_define_const(gnoStock, "PIXMAP_ABOUT",
            rb_str_new2(GNOME_STOCK_PIXMAP_ABOUT));
    rb_define_const(gnoStock, "PIXMAP_QUIT",
            rb_str_new2(GNOME_STOCK_PIXMAP_QUIT));
    rb_define_const(gnoStock, "PIXMAP_MULTIPLE",
            rb_str_new2(GNOME_STOCK_PIXMAP_MULTIPLE));
    rb_define_const(gnoStock, "PIXMAP_NOT",
            rb_str_new2(GNOME_STOCK_PIXMAP_NOT));
    rb_define_const(gnoStock, "PIXMAP_CONVERT",
            rb_str_new2(GNOME_STOCK_PIXMAP_CONVERT));
    rb_define_const(gnoStock, "PIXMAP_JUMP_TO",
            rb_str_new2(GNOME_STOCK_PIXMAP_JUMP_TO));
    rb_define_const(gnoStock, "PIXMAP_UP",
            rb_str_new2(GNOME_STOCK_PIXMAP_UP));
    rb_define_const(gnoStock, "PIXMAP_DOWN",
            rb_str_new2(GNOME_STOCK_PIXMAP_DOWN));
    rb_define_const(gnoStock, "PIXMAP_TOP",
            rb_str_new2(GNOME_STOCK_PIXMAP_TOP));
    rb_define_const(gnoStock, "PIXMAP_BOTTOM",
            rb_str_new2(GNOME_STOCK_PIXMAP_BOTTOM));
    rb_define_const(gnoStock, "PIXMAP_ATTACH",
            rb_str_new2(GNOME_STOCK_PIXMAP_ATTACH));
    rb_define_const(gnoStock, "PIXMAP_INDEX",
            rb_str_new2(GNOME_STOCK_PIXMAP_INDEX));
    rb_define_const(gnoStock, "PIXMAP_FONT",
            rb_str_new2(GNOME_STOCK_PIXMAP_FONT));
    rb_define_const(gnoStock, "PIXMAP_EXEC",
            rb_str_new2(GNOME_STOCK_PIXMAP_EXEC));

    rb_define_const(gnoStock, "PIXMAP_ALIGN_LEFT",
            rb_str_new2(GNOME_STOCK_PIXMAP_ALIGN_LEFT));
    rb_define_const(gnoStock, "PIXMAP_ALIGN_RIGHT",
            rb_str_new2(GNOME_STOCK_PIXMAP_ALIGN_RIGHT));
    rb_define_const(gnoStock, "PIXMAP_ALIGN_CENTER",
            rb_str_new2(GNOME_STOCK_PIXMAP_ALIGN_CENTER));
    rb_define_const(gnoStock, "PIXMAP_ALIGN_JUSTIFY",
            rb_str_new2(GNOME_STOCK_PIXMAP_ALIGN_JUSTIFY));

    rb_define_const(gnoStock, "PIXMAP_TEXT_BOLD",
            rb_str_new2(GNOME_STOCK_PIXMAP_TEXT_BOLD));
    rb_define_const(gnoStock, "PIXMAP_TEXT_ITALIC",
            rb_str_new2(GNOME_STOCK_PIXMAP_TEXT_ITALIC));
    rb_define_const(gnoStock, "PIXMAP_TEXT_UNDERLINE",
            rb_str_new2(GNOME_STOCK_PIXMAP_TEXT_UNDERLINE));
    rb_define_const(gnoStock, "PIXMAP_TEXT_STRIKEOUT",
            rb_str_new2(GNOME_STOCK_PIXMAP_TEXT_STRIKEOUT));

    rb_define_const(gnoStock, "PIXMAP_TEXT_INDENT",
            rb_str_new2(GNOME_STOCK_PIXMAP_TEXT_INDENT));
    rb_define_const(gnoStock, "PIXMAP_TEXT_UNINDENT",
            rb_str_new2(GNOME_STOCK_PIXMAP_TEXT_UNINDENT));

    rb_define_const(gnoStock, "PIXMAP_EXIT",
            rb_str_new2(GNOME_STOCK_PIXMAP_QUIT));

    rb_define_const(gnoStock, "PIXMAP_COLORSELECTOR",
            rb_str_new2(GNOME_STOCK_PIXMAP_COLORSELECTOR));

    rb_define_const(gnoStock, "PIXMAP_ADD",
            rb_str_new2(GNOME_STOCK_PIXMAP_ADD));
    rb_define_const(gnoStock, "PIXMAP_REMOVE",
            rb_str_new2(GNOME_STOCK_PIXMAP_REMOVE));

    rb_define_const(gnoStock, "PIXMAP_TABLE_BORDERS",
            rb_str_new2(GNOME_STOCK_PIXMAP_TABLE_BORDERS));
    rb_define_const(gnoStock, "PIXMAP_TABLE_FILL",
            rb_str_new2(GNOME_STOCK_PIXMAP_TABLE_FILL));

    rb_define_const(gnoStock, "PIXMAP_TEXT_BULLETED_LIST",
            rb_str_new2(GNOME_STOCK_PIXMAP_TEXT_BULLETED_LIST));
    rb_define_const(gnoStock, "PIXMAP_TEXT_NUMBERED_LIST",
            rb_str_new2(GNOME_STOCK_PIXMAP_TEXT_NUMBERED_LIST));

    rb_define_const(gnoStock, "PIXMAP_REGULAR",
            rb_str_new2(GNOME_STOCK_PIXMAP_REGULAR));
    rb_define_const(gnoStock, "PIXMAP_DISABLED",
            rb_str_new2(GNOME_STOCK_PIXMAP_DISABLED));
    rb_define_const(gnoStock, "PIXMAP_FOCUSED",
            rb_str_new2(GNOME_STOCK_PIXMAP_FOCUSED));

    /* GnomeStockPixmapType */
    rb_define_const(gnoStock, "PIXMAP_TYPE_NONE",
            INT2FIX(GNOME_STOCK_PIXMAP_TYPE_NONE));
    rb_define_const(gnoStock, "PIXMAP_TYPE_DATA",
            INT2FIX(GNOME_STOCK_PIXMAP_TYPE_DATA));
    rb_define_const(gnoStock, "PIXMAP_TYPE_FILE",
            INT2FIX(GNOME_STOCK_PIXMAP_TYPE_FILE));
    rb_define_const(gnoStock, "PIXMAP_TYPE_PATH",
            INT2FIX(GNOME_STOCK_PIXMAP_TYPE_PATH));
    rb_define_const(gnoStock, "PIXMAP_TYPE_WIDGET",
            INT2FIX(GNOME_STOCK_PIXMAP_TYPE_WIDGET));
    rb_define_const(gnoStock, "PIXMAP_TYPE_IMLIB",
            INT2FIX(GNOME_STOCK_PIXMAP_TYPE_IMLIB));
    rb_define_const(gnoStock, "PIXMAP_TYPE_IMLIB_SCALED",
            INT2FIX(GNOME_STOCK_PIXMAP_TYPE_IMLIB_SCALED));
    rb_define_const(gnoStock, "PIXMAP_TYPE_GPIXMAP",
            INT2FIX(GNOME_STOCK_PIXMAP_TYPE_GPIXMAP));

    /* menus */
    rb_define_const(gnoStock, "MENU_BLANK",
            rb_str_new2(GNOME_STOCK_MENU_BLANK));
    rb_define_const(gnoStock, "MENU_NEW",
            rb_str_new2(GNOME_STOCK_MENU_NEW));
    rb_define_const(gnoStock, "MENU_SAVE",
            rb_str_new2(GNOME_STOCK_MENU_SAVE));
    rb_define_const(gnoStock, "MENU_SAVE_AS",
            rb_str_new2(GNOME_STOCK_MENU_SAVE_AS));
    rb_define_const(gnoStock, "MENU_REVERT",
            rb_str_new2(GNOME_STOCK_MENU_REVERT));
    rb_define_const(gnoStock, "MENU_OPEN",
            rb_str_new2(GNOME_STOCK_MENU_OPEN));
    rb_define_const(gnoStock, "MENU_CLOSE",
            rb_str_new2(GNOME_STOCK_MENU_CLOSE));
    rb_define_const(gnoStock, "MENU_QUIT",
            rb_str_new2(GNOME_STOCK_MENU_QUIT));
    rb_define_const(gnoStock, "MENU_CUT",
            rb_str_new2(GNOME_STOCK_MENU_CUT));
    rb_define_const(gnoStock, "MENU_COPY",
            rb_str_new2(GNOME_STOCK_MENU_COPY));
    rb_define_const(gnoStock, "MENU_PASTE",
            rb_str_new2(GNOME_STOCK_MENU_PASTE));
    rb_define_const(gnoStock, "MENU_PROP",
            rb_str_new2(GNOME_STOCK_MENU_PROP));
    rb_define_const(gnoStock, "MENU_PREF",
            rb_str_new2(GNOME_STOCK_MENU_PREF));
    rb_define_const(gnoStock, "MENU_ABOUT",
            rb_str_new2(GNOME_STOCK_MENU_ABOUT));
    rb_define_const(gnoStock, "MENU_SCORES",
            rb_str_new2(GNOME_STOCK_MENU_SCORES));
    rb_define_const(gnoStock, "MENU_UNDO",
            rb_str_new2(GNOME_STOCK_MENU_UNDO));
    rb_define_const(gnoStock, "MENU_REDO",
            rb_str_new2(GNOME_STOCK_MENU_REDO));
    rb_define_const(gnoStock, "MENU_PRINT",
            rb_str_new2(GNOME_STOCK_MENU_PRINT));
    rb_define_const(gnoStock, "MENU_SEARCH",
            rb_str_new2(GNOME_STOCK_MENU_SEARCH));
    rb_define_const(gnoStock, "MENU_SRCHRPL",
            rb_str_new2(GNOME_STOCK_MENU_SRCHRPL));
    rb_define_const(gnoStock, "MENU_BACK",
            rb_str_new2(GNOME_STOCK_MENU_BACK));
    rb_define_const(gnoStock, "MENU_FORWARD",
            rb_str_new2(GNOME_STOCK_MENU_FORWARD));
    rb_define_const(gnoStock, "MENU_FIRST",
            rb_str_new2(GNOME_STOCK_MENU_FIRST));
    rb_define_const(gnoStock, "MENU_LAST",
            rb_str_new2(GNOME_STOCK_MENU_LAST));
    rb_define_const(gnoStock, "MENU_HOME",
            rb_str_new2(GNOME_STOCK_MENU_HOME));
    rb_define_const(gnoStock, "MENU_STOP",
            rb_str_new2(GNOME_STOCK_MENU_STOP));
    rb_define_const(gnoStock, "MENU_REFRESH",
            rb_str_new2(GNOME_STOCK_MENU_REFRESH));
    rb_define_const(gnoStock, "MENU_MAIL",
            rb_str_new2(GNOME_STOCK_MENU_MAIL));
    rb_define_const(gnoStock, "MENU_MAIL_RCV",
            rb_str_new2(GNOME_STOCK_MENU_MAIL_RCV));
    rb_define_const(gnoStock, "MENU_MAIL_SND",
            rb_str_new2(GNOME_STOCK_MENU_MAIL_SND));
    rb_define_const(gnoStock, "MENU_MAIL_RPL",
            rb_str_new2(GNOME_STOCK_MENU_MAIL_RPL));
    rb_define_const(gnoStock, "MENU_MAIL_FWD",
            rb_str_new2(GNOME_STOCK_MENU_MAIL_FWD));
    rb_define_const(gnoStock, "MENU_MAIL_NEW",
            rb_str_new2(GNOME_STOCK_MENU_MAIL_NEW));
    rb_define_const(gnoStock, "MENU_TRASH",
            rb_str_new2(GNOME_STOCK_MENU_TRASH));
    rb_define_const(gnoStock, "MENU_TRASH_FULL",
            rb_str_new2(GNOME_STOCK_MENU_TRASH_FULL));
    rb_define_const(gnoStock, "MENU_UNDELETE",
            rb_str_new2(GNOME_STOCK_MENU_UNDELETE));
    rb_define_const(gnoStock, "MENU_TIMER",
            rb_str_new2(GNOME_STOCK_MENU_TIMER));
    rb_define_const(gnoStock, "MENU_TIMER_STOP",
            rb_str_new2(GNOME_STOCK_MENU_TIMER_STOP));
    rb_define_const(gnoStock, "MENU_SPELLCHECK",
            rb_str_new2(GNOME_STOCK_MENU_SPELLCHECK));
    rb_define_const(gnoStock, "MENU_MIC",
            rb_str_new2(GNOME_STOCK_MENU_MIC));
    rb_define_const(gnoStock, "MENU_LINE_IN",
            rb_str_new2(GNOME_STOCK_MENU_LINE_IN));
    rb_define_const(gnoStock, "MENU_CDROM",
            rb_str_new2(GNOME_STOCK_MENU_CDROM));
    rb_define_const(gnoStock, "MENU_VOLUME",
            rb_str_new2(GNOME_STOCK_MENU_VOLUME));
    rb_define_const(gnoStock, "MENU_MIDI",
            rb_str_new2(GNOME_STOCK_MENU_MIDI));
    rb_define_const(gnoStock, "MENU_BOOK_RED",
            rb_str_new2(GNOME_STOCK_MENU_BOOK_RED));
    rb_define_const(gnoStock, "MENU_BOOK_GREEN",
            rb_str_new2(GNOME_STOCK_MENU_BOOK_GREEN));
    rb_define_const(gnoStock, "MENU_BOOK_BLUE",
            rb_str_new2(GNOME_STOCK_MENU_BOOK_BLUE));
    rb_define_const(gnoStock, "MENU_BOOK_YELLOW",
            rb_str_new2(GNOME_STOCK_MENU_BOOK_YELLOW));
    rb_define_const(gnoStock, "MENU_BOOK_OPEN",
            rb_str_new2(GNOME_STOCK_MENU_BOOK_OPEN));
    rb_define_const(gnoStock, "MENU_CONVERT",
            rb_str_new2(GNOME_STOCK_MENU_CONVERT));
    rb_define_const(gnoStock, "MENU_JUMP_TO",
            rb_str_new2(GNOME_STOCK_MENU_JUMP_TO));
    rb_define_const(gnoStock, "MENU_UP",
            rb_str_new2(GNOME_STOCK_MENU_UP));
    rb_define_const(gnoStock, "MENU_DOWN",
            rb_str_new2(GNOME_STOCK_MENU_DOWN));
    rb_define_const(gnoStock, "MENU_TOP",
            rb_str_new2(GNOME_STOCK_MENU_TOP));
    rb_define_const(gnoStock, "MENU_BOTTOM",
            rb_str_new2(GNOME_STOCK_MENU_BOTTOM));
    rb_define_const(gnoStock, "MENU_ATTACH",
            rb_str_new2(GNOME_STOCK_MENU_ATTACH));
    rb_define_const(gnoStock, "MENU_INDEX",
            rb_str_new2(GNOME_STOCK_MENU_INDEX));
    rb_define_const(gnoStock, "MENU_FONT",
            rb_str_new2(GNOME_STOCK_MENU_FONT));
    rb_define_const(gnoStock, "MENU_EXEC",
            rb_str_new2(GNOME_STOCK_MENU_EXEC));

    rb_define_const(gnoStock, "MENU_ALIGN_LEFT",
            rb_str_new2(GNOME_STOCK_MENU_ALIGN_LEFT));
    rb_define_const(gnoStock, "MENU_ALIGN_RIGHT",
            rb_str_new2(GNOME_STOCK_MENU_ALIGN_RIGHT));
    rb_define_const(gnoStock, "MENU_ALIGN_CENTER",
            rb_str_new2(GNOME_STOCK_MENU_ALIGN_CENTER));
    rb_define_const(gnoStock, "MENU_ALIGN_JUSTIFY",
            rb_str_new2(GNOME_STOCK_MENU_ALIGN_JUSTIFY));

    rb_define_const(gnoStock, "MENU_TEXT_BOLD",
            rb_str_new2(GNOME_STOCK_MENU_TEXT_BOLD));
    rb_define_const(gnoStock, "MENU_TEXT_ITALIC",
            rb_str_new2(GNOME_STOCK_MENU_TEXT_ITALIC));
    rb_define_const(gnoStock, "MENU_TEXT_UNDERLINE",
            rb_str_new2(GNOME_STOCK_MENU_TEXT_UNDERLINE));
    rb_define_const(gnoStock, "MENU_TEXT_STRIKEOUT",
            rb_str_new2(GNOME_STOCK_MENU_TEXT_STRIKEOUT));

    rb_define_const(gnoStock, "MENU_EXIT",
            rb_str_new2(GNOME_STOCK_MENU_QUIT));

    rb_define_const(gnoStock, "BUTTON_OK",
            rb_str_new2(GNOME_STOCK_BUTTON_OK));
    rb_define_const(gnoStock, "BUTTON_CANCEL",
            rb_str_new2(GNOME_STOCK_BUTTON_CANCEL));
    rb_define_const(gnoStock, "BUTTON_YES",
            rb_str_new2(GNOME_STOCK_BUTTON_YES));
    rb_define_const(gnoStock, "BUTTON_NO",
            rb_str_new2(GNOME_STOCK_BUTTON_NO));
    rb_define_const(gnoStock, "BUTTON_CLOSE",
            rb_str_new2(GNOME_STOCK_BUTTON_CLOSE));
    rb_define_const(gnoStock, "BUTTON_APPLY",
            rb_str_new2(GNOME_STOCK_BUTTON_APPLY));
    rb_define_const(gnoStock, "BUTTON_HELP",
            rb_str_new2(GNOME_STOCK_BUTTON_HELP));
    rb_define_const(gnoStock, "BUTTON_NEXT",
            rb_str_new2(GNOME_STOCK_BUTTON_NEXT));
    rb_define_const(gnoStock, "BUTTON_PREV",
            rb_str_new2(GNOME_STOCK_BUTTON_PREV));
    rb_define_const(gnoStock, "BUTTON_UP",
            rb_str_new2(GNOME_STOCK_BUTTON_UP));
    rb_define_const(gnoStock, "BUTTON_DOWN",
            rb_str_new2(GNOME_STOCK_BUTTON_DOWN));
    rb_define_const(gnoStock, "BUTTON_FONT",
            rb_str_new2(GNOME_STOCK_BUTTON_FONT));

    rb_define_singleton_method(gnoStock, "pixmap_widget",
                               stock_pixmap_widget, -1);

    rb_define_method(gnoStock, "initialize", stock_initialize, -1);
    rb_define_method(gnoStock, "set_icon", stock_set_icon, 1);
}
