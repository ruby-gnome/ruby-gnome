/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkstock.c -

  $Author: sakai $
  $Date: 2007/07/08 03:00:49 $

  Copyright (C) 2002,2003 KUBO Takehiro
************************************************/

#include "global.h"

#define Check_Symbol(sym) do { \
    if (!SYMBOL_P(sym)) \
        rb_raise(rb_eArgError, "invalid argument %s (expect Symbol)", \
                 rb_class2name(CLASS_OF(sym))); \
} while (0)

/* check whether sym is a Symbol or not in advance. */
#define SYM2CSTR(sym) rb_id2name(SYM2ID(sym))
#define CSTR2SYM(str) ID2SYM(rb_intern(str))

static VALUE
stock_m_add(argc, argv, klass)
    int argc;
    VALUE *argv, klass;
{
    VALUE stock_id, label, modifier, keyval, translation_domain;
    GtkStockItem item;

    rb_scan_args(argc, argv, "23", &stock_id, &label, &modifier, &keyval, &translation_domain);
    Check_Symbol(stock_id);
    item.stock_id = (gchar *)SYM2CSTR(stock_id);
    item.label = RVAL2CSTR(label);
    item.modifier = NIL_P(modifier) ? 0 : NUM2UINT(modifier);
    item.keyval = NIL_P(keyval) ? 0 : NUM2UINT(keyval);
    item.translation_domain = NIL_P(translation_domain) ? NULL : RVAL2CSTR(translation_domain);
    gtk_stock_add(&item, 1);
    return Qnil;
}

static VALUE
stock_m_lookup(klass, stock_id)
    VALUE klass, stock_id;
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
stock_m_list_ids(klass)
    VALUE klass;
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
static gchar*
translate_func(path, func)
    const gchar* path;
    gpointer func;
{
    VALUE ret = rb_funcall((VALUE)func, id_call, 1, CSTR2RVAL(path));
    return RVAL2CSTR(ret);
}

static VALUE
stock_m_set_translate_func(klass, domain)
    VALUE klass;
    VALUE domain;
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
Init_gtk_stock()
{
    VALUE mGtkStock = rb_define_module_under(mGtk, "Stock");

    rb_undef_method(mGtkStock, "new");
    rb_define_singleton_method(mGtkStock, "add", stock_m_add, -1);
    rb_define_singleton_method(mGtkStock, "lookup", stock_m_lookup, 1);
    rb_define_singleton_method(mGtkStock, "ids", stock_m_list_ids, 0);
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_singleton_method(mGtkStock, "set_translate_func", stock_m_set_translate_func, 1);
#endif

    /* Stock IDs (not all are stock items; some are images only) */
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_const(mGtkStock, "ABOUT", CSTR2SYM(GTK_STOCK_ABOUT));
#endif
    rb_define_const(mGtkStock, "ADD", CSTR2SYM(GTK_STOCK_ADD));
    rb_define_const(mGtkStock, "APPLY", CSTR2SYM(GTK_STOCK_APPLY));
    rb_define_const(mGtkStock, "BOLD", CSTR2SYM(GTK_STOCK_BOLD));
    rb_define_const(mGtkStock, "CANCEL", CSTR2SYM(GTK_STOCK_CANCEL));
    rb_define_const(mGtkStock, "CDROM", CSTR2SYM(GTK_STOCK_CDROM));
    rb_define_const(mGtkStock, "CLEAR", CSTR2SYM(GTK_STOCK_CLEAR));
    rb_define_const(mGtkStock, "CLOSE", CSTR2SYM(GTK_STOCK_CLOSE));
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_const(mGtkStock, "COLOR_PICKER", CSTR2SYM(GTK_STOCK_COLOR_PICKER));
#endif
    rb_define_const(mGtkStock, "CONVERT", CSTR2SYM(GTK_STOCK_CONVERT));
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_const(mGtkStock, "CONNECT", CSTR2SYM(GTK_STOCK_CONNECT));
#endif
    rb_define_const(mGtkStock, "COPY", CSTR2SYM(GTK_STOCK_COPY));
    rb_define_const(mGtkStock, "CUT", CSTR2SYM(GTK_STOCK_CUT));
    rb_define_const(mGtkStock, "DELETE", CSTR2SYM(GTK_STOCK_DELETE));
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_const(mGtkStock, "DIALOG_AUTHENTICATION", CSTR2SYM(GTK_STOCK_DIALOG_AUTHENTICATION));
#endif
    rb_define_const(mGtkStock, "DIALOG_ERROR", CSTR2SYM(GTK_STOCK_DIALOG_ERROR));
    rb_define_const(mGtkStock, "DIALOG_INFO", CSTR2SYM(GTK_STOCK_DIALOG_INFO));
    rb_define_const(mGtkStock, "DIALOG_QUESTION", CSTR2SYM(GTK_STOCK_DIALOG_QUESTION));
    rb_define_const(mGtkStock, "DIALOG_WARNING", CSTR2SYM(GTK_STOCK_DIALOG_WARNING));
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_const(mGtkStock, "DIRECTORY", CSTR2SYM(GTK_STOCK_DIRECTORY));
#endif
#if GTK_CHECK_VERSION(2,12,0)
    rb_define_const(mGtkStock, "DISCARD", CSTR2SYM(GTK_STOCK_DISCARD));
#endif
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_const(mGtkStock, "DISCONNECT", CSTR2SYM(GTK_STOCK_DISCONNECT));
#endif
    rb_define_const(mGtkStock, "DND", CSTR2SYM(GTK_STOCK_DND));
    rb_define_const(mGtkStock, "DND_MULTIPLE", CSTR2SYM(GTK_STOCK_DND_MULTIPLE));
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_const(mGtkStock, "EDIT", CSTR2SYM(GTK_STOCK_EDIT));
    rb_define_const(mGtkStock, "FILE", CSTR2SYM(GTK_STOCK_FILE));
#endif
    rb_define_const(mGtkStock, "EXECUTE", CSTR2SYM(GTK_STOCK_EXECUTE));
    rb_define_const(mGtkStock, "FIND", CSTR2SYM(GTK_STOCK_FIND));
    rb_define_const(mGtkStock, "FIND_AND_REPLACE", CSTR2SYM(GTK_STOCK_FIND_AND_REPLACE));
    rb_define_const(mGtkStock, "FLOPPY", CSTR2SYM(GTK_STOCK_FLOPPY));
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_const(mGtkStock, "FULLSCREEN", CSTR2SYM(GTK_STOCK_FULLSCREEN));
#endif
    rb_define_const(mGtkStock, "GOTO_BOTTOM", CSTR2SYM(GTK_STOCK_GOTO_BOTTOM));
    rb_define_const(mGtkStock, "GOTO_FIRST", CSTR2SYM(GTK_STOCK_GOTO_FIRST));
    rb_define_const(mGtkStock, "GOTO_LAST", CSTR2SYM(GTK_STOCK_GOTO_LAST));
    rb_define_const(mGtkStock, "GOTO_TOP", CSTR2SYM(GTK_STOCK_GOTO_TOP));
    rb_define_const(mGtkStock, "GO_BACK", CSTR2SYM(GTK_STOCK_GO_BACK));
    rb_define_const(mGtkStock, "GO_DOWN", CSTR2SYM(GTK_STOCK_GO_DOWN));
    rb_define_const(mGtkStock, "GO_FORWARD", CSTR2SYM(GTK_STOCK_GO_FORWARD));
    rb_define_const(mGtkStock, "GO_UP", CSTR2SYM(GTK_STOCK_GO_UP));
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_const(mGtkStock, "HARDDISK", CSTR2SYM(GTK_STOCK_HARDDISK));
#endif
    rb_define_const(mGtkStock, "HELP", CSTR2SYM(GTK_STOCK_HELP));
    rb_define_const(mGtkStock, "HOME", CSTR2SYM(GTK_STOCK_HOME));
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_const(mGtkStock, "INDENT", CSTR2SYM(GTK_STOCK_INDENT));
#endif
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_const(mGtkStock, "INFO", CSTR2SYM(GTK_STOCK_INFO));
#endif
    rb_define_const(mGtkStock, "INDEX", CSTR2SYM(GTK_STOCK_INDEX));
    rb_define_const(mGtkStock, "ITALIC", CSTR2SYM(GTK_STOCK_ITALIC));
    rb_define_const(mGtkStock, "JUMP_TO", CSTR2SYM(GTK_STOCK_JUMP_TO));
    rb_define_const(mGtkStock, "JUSTIFY_CENTER", CSTR2SYM(GTK_STOCK_JUSTIFY_CENTER));
    rb_define_const(mGtkStock, "JUSTIFY_FILL", CSTR2SYM(GTK_STOCK_JUSTIFY_FILL));
    rb_define_const(mGtkStock, "JUSTIFY_LEFT", CSTR2SYM(GTK_STOCK_JUSTIFY_LEFT));
    rb_define_const(mGtkStock, "JUSTIFY_RIGHT", CSTR2SYM(GTK_STOCK_JUSTIFY_RIGHT));
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_const(mGtkStock, "LEAVE_FULLSCREEN", CSTR2SYM(GTK_STOCK_LEAVE_FULLSCREEN));
#endif
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_const(mGtkStock, "MEDIA_FORWARD", CSTR2SYM(GTK_STOCK_MEDIA_FORWARD));
    rb_define_const(mGtkStock, "MEDIA_NEXT", CSTR2SYM(GTK_STOCK_MEDIA_NEXT));
    rb_define_const(mGtkStock, "MEDIA_PAUSE", CSTR2SYM(GTK_STOCK_MEDIA_PAUSE));
    rb_define_const(mGtkStock, "MEDIA_PLAY", CSTR2SYM(GTK_STOCK_MEDIA_PLAY));
    rb_define_const(mGtkStock, "MEDIA_PREVIOUS", CSTR2SYM(GTK_STOCK_MEDIA_PREVIOUS));
    rb_define_const(mGtkStock, "MEDIA_RECORD", CSTR2SYM(GTK_STOCK_MEDIA_RECORD));
    rb_define_const(mGtkStock, "MEDIA_REWIND", CSTR2SYM(GTK_STOCK_MEDIA_REWIND));
    rb_define_const(mGtkStock, "MEDIA_STOP", CSTR2SYM(GTK_STOCK_MEDIA_STOP));
#endif
    rb_define_const(mGtkStock, "MISSING_IMAGE", CSTR2SYM(GTK_STOCK_MISSING_IMAGE));
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_const(mGtkStock, "NETWORK", CSTR2SYM(GTK_STOCK_NETWORK));
#endif
    rb_define_const(mGtkStock, "NEW", CSTR2SYM(GTK_STOCK_NEW));
    rb_define_const(mGtkStock, "NO", CSTR2SYM(GTK_STOCK_NO));
    rb_define_const(mGtkStock, "OK", CSTR2SYM(GTK_STOCK_OK));
    rb_define_const(mGtkStock, "OPEN", CSTR2SYM(GTK_STOCK_OPEN));
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_const(mGtkStock, "ORIENTATION_LANDSCAPE", CSTR2SYM(GTK_STOCK_ORIENTATION_LANDSCAPE));
    rb_define_const(mGtkStock, "ORIENTATION_PORTRAIT", CSTR2SYM(GTK_STOCK_ORIENTATION_PORTRAIT));
    rb_define_const(mGtkStock, "ORIENTATION_REVERSE_LANDSCAPE", CSTR2SYM(GTK_STOCK_ORIENTATION_REVERSE_LANDSCAPE));
    rb_define_const(mGtkStock, "ORIENTATION_REVERSE_PORTRAIT", CSTR2SYM(GTK_STOCK_ORIENTATION_REVERSE_PORTRAIT));
#endif
    rb_define_const(mGtkStock, "PASTE", CSTR2SYM(GTK_STOCK_PASTE));
    rb_define_const(mGtkStock, "PREFERENCES", CSTR2SYM(GTK_STOCK_PREFERENCES));
    rb_define_const(mGtkStock, "PRINT", CSTR2SYM(GTK_STOCK_PRINT));
    rb_define_const(mGtkStock, "PRINT_PREVIEW", CSTR2SYM(GTK_STOCK_PRINT_PREVIEW));
    rb_define_const(mGtkStock, "PROPERTIES", CSTR2SYM(GTK_STOCK_PROPERTIES));
    rb_define_const(mGtkStock, "QUIT", CSTR2SYM(GTK_STOCK_QUIT));
    rb_define_const(mGtkStock, "REDO", CSTR2SYM(GTK_STOCK_REDO));
    rb_define_const(mGtkStock, "REFRESH", CSTR2SYM(GTK_STOCK_REFRESH));
    rb_define_const(mGtkStock, "REMOVE", CSTR2SYM(GTK_STOCK_REMOVE));
    rb_define_const(mGtkStock, "REVERT_TO_SAVED", CSTR2SYM(GTK_STOCK_REVERT_TO_SAVED));
    rb_define_const(mGtkStock, "SAVE", CSTR2SYM(GTK_STOCK_SAVE));
    rb_define_const(mGtkStock, "SAVE_AS", CSTR2SYM(GTK_STOCK_SAVE_AS));
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_const(mGtkStock, "SELECT_ALL", CSTR2SYM(GTK_STOCK_SELECT_ALL));
#endif
    rb_define_const(mGtkStock, "SELECT_COLOR", CSTR2SYM(GTK_STOCK_SELECT_COLOR));
    rb_define_const(mGtkStock, "SELECT_FONT", CSTR2SYM(GTK_STOCK_SELECT_FONT));
    rb_define_const(mGtkStock, "SORT_ASCENDING", CSTR2SYM(GTK_STOCK_SORT_ASCENDING));
    rb_define_const(mGtkStock, "SORT_DESCENDING", CSTR2SYM(GTK_STOCK_SORT_DESCENDING));
    rb_define_const(mGtkStock, "SPELL_CHECK", CSTR2SYM(GTK_STOCK_SPELL_CHECK));
    rb_define_const(mGtkStock, "STOP", CSTR2SYM(GTK_STOCK_STOP));
    rb_define_const(mGtkStock, "STRIKETHROUGH", CSTR2SYM(GTK_STOCK_STRIKETHROUGH));
    rb_define_const(mGtkStock, "UNDELETE", CSTR2SYM(GTK_STOCK_UNDELETE));
    rb_define_const(mGtkStock, "UNDERLINE", CSTR2SYM(GTK_STOCK_UNDERLINE));
    rb_define_const(mGtkStock, "UNDO", CSTR2SYM(GTK_STOCK_UNDO));
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_const(mGtkStock, "UNINDENT", CSTR2SYM(GTK_STOCK_UNINDENT));
#endif
    rb_define_const(mGtkStock, "YES", CSTR2SYM(GTK_STOCK_YES));
    rb_define_const(mGtkStock, "ZOOM_100", CSTR2SYM(GTK_STOCK_ZOOM_100));
    rb_define_const(mGtkStock, "ZOOM_FIT", CSTR2SYM(GTK_STOCK_ZOOM_FIT));
    rb_define_const(mGtkStock, "ZOOM_IN", CSTR2SYM(GTK_STOCK_ZOOM_IN));
    rb_define_const(mGtkStock, "ZOOM_OUT", CSTR2SYM(GTK_STOCK_ZOOM_OUT));
}
