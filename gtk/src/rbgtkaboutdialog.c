/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkaboutdialog.c -

  $Author: mutoh $
  $Date: 2005/01/29 11:44:14 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "global.h"
                                                                                
#if GTK_CHECK_VERSION(2,6,0)

#define _SELF(self) (GTK_ABOUT_DIALOG(RVAL2GOBJ(self)))

static VALUE
aboutdialog_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_about_dialog_new());
    return Qnil;
}

/* Defined as Properties
const gchar* gtk_about_dialog_get_name      (GtkAboutDialog *about);
void        gtk_about_dialog_set_name       (GtkAboutDialog *about,
                                             const gchar *name);
const gchar* gtk_about_dialog_get_version   (GtkAboutDialog *about);
void        gtk_about_dialog_set_version    (GtkAboutDialog *about,
                                             const gchar *version);
const gchar* gtk_about_dialog_get_copyright (GtkAboutDialog *about);
void        gtk_about_dialog_set_copyright  (GtkAboutDialog *about,
                                             const gchar *copyright);
const gchar* gtk_about_dialog_get_comments  (GtkAboutDialog *about);
void        gtk_about_dialog_set_comments   (GtkAboutDialog *about,
                                             const gchar *comments);
const gchar* gtk_about_dialog_get_license   (GtkAboutDialog *about);
void        gtk_about_dialog_set_license    (GtkAboutDialog *about,
                                             const gchar *license);
const gchar* gtk_about_dialog_get_website   (GtkAboutDialog *about);
void        gtk_about_dialog_set_website    (GtkAboutDialog *about,
                                             const gchar *website);
const gchar* gtk_about_dialog_get_website_label
                                            (GtkAboutDialog *about);
void        gtk_about_dialog_set_website_label
                                            (GtkAboutDialog *about,
                                             const gchar *website_label);
const gchar* const * gtk_about_dialog_get_authors
                                            (GtkAboutDialog *about);
void        gtk_about_dialog_set_authors    (GtkAboutDialog *about,
                                             const gchar **authors);
const gchar* const * gtk_about_dialog_get_artists
                                            (GtkAboutDialog *about);
void        gtk_about_dialog_set_artists    (GtkAboutDialog *about,
                                             const gchar **artists);
const gchar* const * gtk_about_dialog_get_documenters
                                            (GtkAboutDialog *about);
void        gtk_about_dialog_set_documenters
                                            (GtkAboutDialog *about,
                                             const gchar **documenters);
const gchar* gtk_about_dialog_get_translator_credits
                                            (GtkAboutDialog *about);
void        gtk_about_dialog_set_translator_credits
                                            (GtkAboutDialog *about,
                                             const gchar *translator_credits);
GdkPixbuf*  gtk_about_dialog_get_logo       (GtkAboutDialog *about);
void        gtk_about_dialog_set_logo       (GtkAboutDialog *about,
                                             GdkPixbuf *logo);
const gchar* gtk_about_dialog_get_logo_icon_name
                                            (GtkAboutDialog *about);
void        gtk_about_dialog_set_logo_icon_name
                                            (GtkAboutDialog *about,
                                             const gchar *icon_name);
*/

static void
activate_link_func(about, link, func)
    GtkAboutDialog* about;
    const gchar* link;
    gpointer func;
{
    rb_funcall((VALUE)func, id_call, 2, GOBJ2RVAL(about), CSTR2RVAL(link));
}

static VALUE
aboutdialog_s_set_email_hook(self)
    VALUE self;
{
    VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);
    gtk_about_dialog_set_email_hook((GtkAboutDialogActivateLinkFunc)activate_link_func, (gpointer)func, (GDestroyNotify)NULL);
    return self;
}

static VALUE
aboutdialog_s_set_url_hook(self)
    VALUE self;
{
    VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);
    gtk_about_dialog_set_url_hook((GtkAboutDialogActivateLinkFunc)activate_link_func, (gpointer)func, (GDestroyNotify)NULL);
    return self;
}


typedef struct {
    const char *name;
    gpointer value;
} prop_data;

#define ABOUT_PROP_NUM (15)

static VALUE
aboutdialog_s_show_about_dialog(self, parent, props)
    VALUE self, parent, props;
{
    int i;
    VALUE ary;
    prop_data pd[ABOUT_PROP_NUM];

    Check_Type(props, T_HASH);

    ary = rb_funcall(props, rb_intern("to_a"), 0);
    
    if (RARRAY(ary)->len > ABOUT_PROP_NUM)
        rb_raise(rb_eArgError, "Too many args.");

    for (i = 0; i < ABOUT_PROP_NUM; i++){
        pd[i].name = (const char*)NULL;
        pd[i].value = (gpointer)NULL;
    }
    for (i = 0; i < RARRAY(ary)->len; i++){
        if (TYPE(RARRAY(RARRAY(ary)->ptr[i])->ptr[0]) == T_SYMBOL){
            pd[i].name = rb_id2name(SYM2ID(RARRAY(RARRAY(ary)->ptr[i])->ptr[0]));
        } else {
            pd[i].name = RVAL2CSTR(RARRAY(RARRAY(ary)->ptr[i])->ptr[0]);
        }
        if (strncmp(pd[i].name, "artists", strlen("artists")) == 0||
            strncmp(pd[i].name, "authors", strlen("authors")) == 0||
            strncmp(pd[i].name, "documenters", strlen("documenters")) == 0){
            GValue gval = {0,};
            g_value_init(&gval, G_TYPE_STRV);
            rbgobj_rvalue_to_gvalue(RARRAY(RARRAY(ary)->ptr[i])->ptr[1], &gval);

            pd[i].value = g_boxed_copy(G_TYPE_STRV, g_value_get_boxed(&gval));
        } else if (strncmp(pd[i].name, "logo", strlen("logo")) == 0 && 
                   strlen(pd[i].name) == strlen("logo")){
            pd[i].value = g_object_ref(RVAL2GOBJ(RARRAY(RARRAY(ary)->ptr[i])->ptr[1]));
        } else {
            pd[i].value = g_strdup(RVAL2CSTR(RARRAY(RARRAY(ary)->ptr[i])->ptr[1]));
        }
    }

    gtk_show_about_dialog(GTK_WINDOW(RVAL2GOBJ(parent)), 
                          pd[0].name, pd[0].value,
                          pd[1].name, pd[1].value,
                          pd[2].name, pd[2].value,
                          pd[3].name, pd[3].value,
                          pd[4].name, pd[4].value,
                          pd[5].name, pd[5].value,
                          pd[6].name, pd[6].value,
                          pd[7].name, pd[7].value,
                          pd[8].name, pd[8].value,
                          pd[9].name, pd[9].value,
                          pd[10].name, pd[10].value,
                          pd[11].name, pd[11].value,
                          pd[12].name, pd[12].value,
                          pd[13].name, pd[13].value,
                          pd[14].name, pd[14].value,
                          NULL);
    return self;
}
#endif

void 
Init_gtk_aboutdialog()
{
#if GTK_CHECK_VERSION(2,6,0)
    VALUE gAboutDialog = G_DEF_CLASS(GTK_TYPE_ABOUT_DIALOG, "AboutDialog", mGtk);
    rb_define_method(gAboutDialog, "initialize", aboutdialog_initialize, 0);

    rb_define_singleton_method(gAboutDialog, "set_email_hook", aboutdialog_s_set_email_hook, 0);
    rb_define_singleton_method(gAboutDialog, "set_url_hook", aboutdialog_s_set_url_hook, 0);
    rb_define_singleton_method(gAboutDialog, "show", aboutdialog_s_show_about_dialog, 2);
#endif
}
