/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 KUBO Takehiro
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

#include "global.h"

#define RG_TARGET_NAMESPACE mStock

#define Check_Symbol(sym) do { \
    if (!SYMBOL_P(sym)) \
        rb_raise(rb_eArgError, "invalid argument %s (expect Symbol)", \
                 rb_class2name(CLASS_OF(sym))); \
} while (0)

/* check whether sym is a Symbol or not in advance. */
#define SYM2CSTR(sym) rb_id2name(SYM2ID(sym))
#define CSTR2SYM(str) ID2SYM(rb_intern(str))

static VALUE
rg_s_add(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE stock_id, label, modifier, keyval, translation_domain;
    GtkStockItem item;

    rb_scan_args(argc, argv, "23", &stock_id, &label, &modifier, &keyval, &translation_domain);
    Check_Symbol(stock_id);
    item.stock_id = (gchar *)SYM2CSTR(stock_id);
    item.label = (gchar *)RVAL2CSTR(label);
    item.modifier = NIL_P(modifier) ? 0 : NUM2UINT(modifier);
    item.keyval = NIL_P(keyval) ? 0 : NUM2UINT(keyval);
    item.translation_domain = NIL_P(translation_domain) ? NULL : (gchar *)RVAL2CSTR(translation_domain);
    gtk_stock_add(&item, 1);
    return Qnil;
}

static VALUE
rg_s_lookup(G_GNUC_UNUSED VALUE self, VALUE stock_id)
{
    GtkStockItem item;

    Check_Symbol(stock_id);
    if (gtk_stock_lookup(SYM2CSTR(stock_id), &item)) {
        return rb_ary_new3(5,
                           CSTR2SYM(item.stock_id),
                           CSTR2RVAL(item.label),
                           UINT2NUM(item.modifier),
                           UINT2NUM(item.keyval),
                           CSTR2RVAL(item.translation_domain));
    }
    rb_raise(rb_eArgError, "no such stock-id: %s", SYM2CSTR(stock_id));
}

static VALUE
rg_s_ids(G_GNUC_UNUSED VALUE self)
{
    GSList *ids = gtk_stock_list_ids();
    GSList *l;
    VALUE ary = rb_ary_new();
    for (l = ids; l != NULL; l = g_slist_next(l)) {
        rb_ary_push(ary, CSTR2SYM(l->data));
        g_free(l->data);
    }
    g_slist_free(ids);
    return ary;
}

#if GTK_CHECK_VERSION(2,8,0)
static gchar *
translate_func(const gchar *path, gpointer func)
{
    VALUE ret = rb_funcall((VALUE)func, id_call, 1, CSTR2RVAL(path));
    return (gchar *)RVAL2CSTR(ret);
}

static VALUE
rg_s_set_translate_func(VALUE klass, VALUE domain)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(klass, func);
    gtk_stock_set_translate_func(RVAL2CSTR(domain),
                                 (GtkTranslateFunc)translate_func, 
                                 (gpointer)func,
                                 NULL);
    return Qnil;
}
#endif

void
Init_gtk_stock(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGtk, "Stock");

    rb_undef_method(RG_TARGET_NAMESPACE, "new");
    RG_DEF_SMETHOD(add, -1);
    RG_DEF_SMETHOD(lookup, 1);
    RG_DEF_SMETHOD(ids, 0);
#if GTK_CHECK_VERSION(2,8,0)
    RG_DEF_SMETHOD(set_translate_func, 1);
#endif

    /* Stock IDs (not all are stock items; some are images only) */
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_const(RG_TARGET_NAMESPACE, "ABOUT", CSTR2SYM(GTK_STOCK_ABOUT));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "ADD", CSTR2SYM(GTK_STOCK_ADD));
    rb_define_const(RG_TARGET_NAMESPACE, "APPLY", CSTR2SYM(GTK_STOCK_APPLY));
    rb_define_const(RG_TARGET_NAMESPACE, "BOLD", CSTR2SYM(GTK_STOCK_BOLD));
    rb_define_const(RG_TARGET_NAMESPACE, "CANCEL", CSTR2SYM(GTK_STOCK_CANCEL));
#if GTK_CHECK_VERSION(2,16,0)
    rb_define_const(RG_TARGET_NAMESPACE, "CAPS_LOCK_WARNING", CSTR2SYM(GTK_STOCK_CAPS_LOCK_WARNING));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "CDROM", CSTR2SYM(GTK_STOCK_CDROM));
    rb_define_const(RG_TARGET_NAMESPACE, "CLEAR", CSTR2SYM(GTK_STOCK_CLEAR));
    rb_define_const(RG_TARGET_NAMESPACE, "CLOSE", CSTR2SYM(GTK_STOCK_CLOSE));
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_const(RG_TARGET_NAMESPACE, "COLOR_PICKER", CSTR2SYM(GTK_STOCK_COLOR_PICKER));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "CONVERT", CSTR2SYM(GTK_STOCK_CONVERT));
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_const(RG_TARGET_NAMESPACE, "CONNECT", CSTR2SYM(GTK_STOCK_CONNECT));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "COPY", CSTR2SYM(GTK_STOCK_COPY));
    rb_define_const(RG_TARGET_NAMESPACE, "CUT", CSTR2SYM(GTK_STOCK_CUT));
    rb_define_const(RG_TARGET_NAMESPACE, "DELETE", CSTR2SYM(GTK_STOCK_DELETE));
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_const(RG_TARGET_NAMESPACE, "DIALOG_AUTHENTICATION", CSTR2SYM(GTK_STOCK_DIALOG_AUTHENTICATION));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "DIALOG_ERROR", CSTR2SYM(GTK_STOCK_DIALOG_ERROR));
    rb_define_const(RG_TARGET_NAMESPACE, "DIALOG_INFO", CSTR2SYM(GTK_STOCK_DIALOG_INFO));
    rb_define_const(RG_TARGET_NAMESPACE, "DIALOG_QUESTION", CSTR2SYM(GTK_STOCK_DIALOG_QUESTION));
    rb_define_const(RG_TARGET_NAMESPACE, "DIALOG_WARNING", CSTR2SYM(GTK_STOCK_DIALOG_WARNING));
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_const(RG_TARGET_NAMESPACE, "DIRECTORY", CSTR2SYM(GTK_STOCK_DIRECTORY));
#endif
#if GTK_CHECK_VERSION(2,12,0)
    rb_define_const(RG_TARGET_NAMESPACE, "DISCARD", CSTR2SYM(GTK_STOCK_DISCARD));
#endif
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_const(RG_TARGET_NAMESPACE, "DISCONNECT", CSTR2SYM(GTK_STOCK_DISCONNECT));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "DND", CSTR2SYM(GTK_STOCK_DND));
    rb_define_const(RG_TARGET_NAMESPACE, "DND_MULTIPLE", CSTR2SYM(GTK_STOCK_DND_MULTIPLE));
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_const(RG_TARGET_NAMESPACE, "EDIT", CSTR2SYM(GTK_STOCK_EDIT));
    rb_define_const(RG_TARGET_NAMESPACE, "FILE", CSTR2SYM(GTK_STOCK_FILE));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "EXECUTE", CSTR2SYM(GTK_STOCK_EXECUTE));
    rb_define_const(RG_TARGET_NAMESPACE, "FIND", CSTR2SYM(GTK_STOCK_FIND));
    rb_define_const(RG_TARGET_NAMESPACE, "FIND_AND_REPLACE", CSTR2SYM(GTK_STOCK_FIND_AND_REPLACE));
    rb_define_const(RG_TARGET_NAMESPACE, "FLOPPY", CSTR2SYM(GTK_STOCK_FLOPPY));
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_const(RG_TARGET_NAMESPACE, "FULLSCREEN", CSTR2SYM(GTK_STOCK_FULLSCREEN));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "GOTO_BOTTOM", CSTR2SYM(GTK_STOCK_GOTO_BOTTOM));
    rb_define_const(RG_TARGET_NAMESPACE, "GOTO_FIRST", CSTR2SYM(GTK_STOCK_GOTO_FIRST));
    rb_define_const(RG_TARGET_NAMESPACE, "GOTO_LAST", CSTR2SYM(GTK_STOCK_GOTO_LAST));
    rb_define_const(RG_TARGET_NAMESPACE, "GOTO_TOP", CSTR2SYM(GTK_STOCK_GOTO_TOP));
    rb_define_const(RG_TARGET_NAMESPACE, "GO_BACK", CSTR2SYM(GTK_STOCK_GO_BACK));
    rb_define_const(RG_TARGET_NAMESPACE, "GO_DOWN", CSTR2SYM(GTK_STOCK_GO_DOWN));
    rb_define_const(RG_TARGET_NAMESPACE, "GO_FORWARD", CSTR2SYM(GTK_STOCK_GO_FORWARD));
    rb_define_const(RG_TARGET_NAMESPACE, "GO_UP", CSTR2SYM(GTK_STOCK_GO_UP));
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_const(RG_TARGET_NAMESPACE, "HARDDISK", CSTR2SYM(GTK_STOCK_HARDDISK));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "HELP", CSTR2SYM(GTK_STOCK_HELP));
    rb_define_const(RG_TARGET_NAMESPACE, "HOME", CSTR2SYM(GTK_STOCK_HOME));
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_const(RG_TARGET_NAMESPACE, "INDENT", CSTR2SYM(GTK_STOCK_INDENT));
#endif
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_const(RG_TARGET_NAMESPACE, "INFO", CSTR2SYM(GTK_STOCK_INFO));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "INDEX", CSTR2SYM(GTK_STOCK_INDEX));
    rb_define_const(RG_TARGET_NAMESPACE, "ITALIC", CSTR2SYM(GTK_STOCK_ITALIC));
    rb_define_const(RG_TARGET_NAMESPACE, "JUMP_TO", CSTR2SYM(GTK_STOCK_JUMP_TO));
    rb_define_const(RG_TARGET_NAMESPACE, "JUSTIFY_CENTER", CSTR2SYM(GTK_STOCK_JUSTIFY_CENTER));
    rb_define_const(RG_TARGET_NAMESPACE, "JUSTIFY_FILL", CSTR2SYM(GTK_STOCK_JUSTIFY_FILL));
    rb_define_const(RG_TARGET_NAMESPACE, "JUSTIFY_LEFT", CSTR2SYM(GTK_STOCK_JUSTIFY_LEFT));
    rb_define_const(RG_TARGET_NAMESPACE, "JUSTIFY_RIGHT", CSTR2SYM(GTK_STOCK_JUSTIFY_RIGHT));
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_const(RG_TARGET_NAMESPACE, "LEAVE_FULLSCREEN", CSTR2SYM(GTK_STOCK_LEAVE_FULLSCREEN));
#endif
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_const(RG_TARGET_NAMESPACE, "MEDIA_FORWARD", CSTR2SYM(GTK_STOCK_MEDIA_FORWARD));
    rb_define_const(RG_TARGET_NAMESPACE, "MEDIA_NEXT", CSTR2SYM(GTK_STOCK_MEDIA_NEXT));
    rb_define_const(RG_TARGET_NAMESPACE, "MEDIA_PAUSE", CSTR2SYM(GTK_STOCK_MEDIA_PAUSE));
    rb_define_const(RG_TARGET_NAMESPACE, "MEDIA_PLAY", CSTR2SYM(GTK_STOCK_MEDIA_PLAY));
    rb_define_const(RG_TARGET_NAMESPACE, "MEDIA_PREVIOUS", CSTR2SYM(GTK_STOCK_MEDIA_PREVIOUS));
    rb_define_const(RG_TARGET_NAMESPACE, "MEDIA_RECORD", CSTR2SYM(GTK_STOCK_MEDIA_RECORD));
    rb_define_const(RG_TARGET_NAMESPACE, "MEDIA_REWIND", CSTR2SYM(GTK_STOCK_MEDIA_REWIND));
    rb_define_const(RG_TARGET_NAMESPACE, "MEDIA_STOP", CSTR2SYM(GTK_STOCK_MEDIA_STOP));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "MISSING_IMAGE", CSTR2SYM(GTK_STOCK_MISSING_IMAGE));
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_const(RG_TARGET_NAMESPACE, "NETWORK", CSTR2SYM(GTK_STOCK_NETWORK));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "NEW", CSTR2SYM(GTK_STOCK_NEW));
    rb_define_const(RG_TARGET_NAMESPACE, "NO", CSTR2SYM(GTK_STOCK_NO));
    rb_define_const(RG_TARGET_NAMESPACE, "OK", CSTR2SYM(GTK_STOCK_OK));
    rb_define_const(RG_TARGET_NAMESPACE, "OPEN", CSTR2SYM(GTK_STOCK_OPEN));
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_const(RG_TARGET_NAMESPACE, "ORIENTATION_LANDSCAPE", CSTR2SYM(GTK_STOCK_ORIENTATION_LANDSCAPE));
    rb_define_const(RG_TARGET_NAMESPACE, "ORIENTATION_PORTRAIT", CSTR2SYM(GTK_STOCK_ORIENTATION_PORTRAIT));
    rb_define_const(RG_TARGET_NAMESPACE, "ORIENTATION_REVERSE_LANDSCAPE", CSTR2SYM(GTK_STOCK_ORIENTATION_REVERSE_LANDSCAPE));
    rb_define_const(RG_TARGET_NAMESPACE, "ORIENTATION_REVERSE_PORTRAIT", CSTR2SYM(GTK_STOCK_ORIENTATION_REVERSE_PORTRAIT));
#endif
#if GTK_CHECK_VERSION(2,14,0)
    rb_define_const(RG_TARGET_NAMESPACE, "PAGE_SETUP", CSTR2SYM(GTK_STOCK_PAGE_SETUP));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "PASTE", CSTR2SYM(GTK_STOCK_PASTE));
    rb_define_const(RG_TARGET_NAMESPACE, "PREFERENCES", CSTR2SYM(GTK_STOCK_PREFERENCES));
    rb_define_const(RG_TARGET_NAMESPACE, "PRINT", CSTR2SYM(GTK_STOCK_PRINT));
#if GTK_CHECK_VERSION(2,14,0)
    rb_define_const(RG_TARGET_NAMESPACE, "PRINT_ERROR", CSTR2SYM(GTK_STOCK_PRINT_ERROR));
    rb_define_const(RG_TARGET_NAMESPACE, "PRINT_PAUSED", CSTR2SYM(GTK_STOCK_PRINT_PAUSED));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "PRINT_PREVIEW", CSTR2SYM(GTK_STOCK_PRINT_PREVIEW));
#if GTK_CHECK_VERSION(2,14,0)
    rb_define_const(RG_TARGET_NAMESPACE, "PRINT_REPORT", CSTR2SYM(GTK_STOCK_PRINT_REPORT));
    rb_define_const(RG_TARGET_NAMESPACE, "PRINT_WARNING", CSTR2SYM(GTK_STOCK_PRINT_WARNING));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "PROPERTIES", CSTR2SYM(GTK_STOCK_PROPERTIES));
    rb_define_const(RG_TARGET_NAMESPACE, "QUIT", CSTR2SYM(GTK_STOCK_QUIT));
    rb_define_const(RG_TARGET_NAMESPACE, "REDO", CSTR2SYM(GTK_STOCK_REDO));
    rb_define_const(RG_TARGET_NAMESPACE, "REFRESH", CSTR2SYM(GTK_STOCK_REFRESH));
    rb_define_const(RG_TARGET_NAMESPACE, "REMOVE", CSTR2SYM(GTK_STOCK_REMOVE));
    rb_define_const(RG_TARGET_NAMESPACE, "REVERT_TO_SAVED", CSTR2SYM(GTK_STOCK_REVERT_TO_SAVED));
    rb_define_const(RG_TARGET_NAMESPACE, "SAVE", CSTR2SYM(GTK_STOCK_SAVE));
    rb_define_const(RG_TARGET_NAMESPACE, "SAVE_AS", CSTR2SYM(GTK_STOCK_SAVE_AS));
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_const(RG_TARGET_NAMESPACE, "SELECT_ALL", CSTR2SYM(GTK_STOCK_SELECT_ALL));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "SELECT_COLOR", CSTR2SYM(GTK_STOCK_SELECT_COLOR));
    rb_define_const(RG_TARGET_NAMESPACE, "SELECT_FONT", CSTR2SYM(GTK_STOCK_SELECT_FONT));
    rb_define_const(RG_TARGET_NAMESPACE, "SORT_ASCENDING", CSTR2SYM(GTK_STOCK_SORT_ASCENDING));
    rb_define_const(RG_TARGET_NAMESPACE, "SORT_DESCENDING", CSTR2SYM(GTK_STOCK_SORT_DESCENDING));
    rb_define_const(RG_TARGET_NAMESPACE, "SPELL_CHECK", CSTR2SYM(GTK_STOCK_SPELL_CHECK));
    rb_define_const(RG_TARGET_NAMESPACE, "STOP", CSTR2SYM(GTK_STOCK_STOP));
    rb_define_const(RG_TARGET_NAMESPACE, "STRIKETHROUGH", CSTR2SYM(GTK_STOCK_STRIKETHROUGH));
    rb_define_const(RG_TARGET_NAMESPACE, "UNDELETE", CSTR2SYM(GTK_STOCK_UNDELETE));
    rb_define_const(RG_TARGET_NAMESPACE, "UNDERLINE", CSTR2SYM(GTK_STOCK_UNDERLINE));
    rb_define_const(RG_TARGET_NAMESPACE, "UNDO", CSTR2SYM(GTK_STOCK_UNDO));
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_const(RG_TARGET_NAMESPACE, "UNINDENT", CSTR2SYM(GTK_STOCK_UNINDENT));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "YES", CSTR2SYM(GTK_STOCK_YES));
    rb_define_const(RG_TARGET_NAMESPACE, "ZOOM_100", CSTR2SYM(GTK_STOCK_ZOOM_100));
    rb_define_const(RG_TARGET_NAMESPACE, "ZOOM_FIT", CSTR2SYM(GTK_STOCK_ZOOM_FIT));
    rb_define_const(RG_TARGET_NAMESPACE, "ZOOM_IN", CSTR2SYM(GTK_STOCK_ZOOM_IN));
    rb_define_const(RG_TARGET_NAMESPACE, "ZOOM_OUT", CSTR2SYM(GTK_STOCK_ZOOM_OUT));
}
