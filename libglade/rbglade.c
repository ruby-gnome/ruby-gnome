#include "ruby.h"

#include <glade/glade.h>
#include <gtk/gtk.h>

/*
 * Here are some function declarations from Ruby-Gtk; it is convenient
 * to be able to use these within Ruby-libglade. However, it could be
 * a hassle to require to Ruby-libglade include the rbgtk.h during
 * compilation, so we just copy-n-paste these few lines over.
 */

/*
 * If Gnome support is enabled, we need to use the appropriate function
 * to convert a GtkObject* to a Ruby object (a VALUE).
 */
#ifdef ENABLE_GNOME

extern VALUE get_value_from_gno_obj(GtkObject* obj);
#define get_glade_auto_type(obj) (get_value_from_gno_obj((obj)))

#else

extern VALUE get_value_from_gobject(GtkObject* obj);
#define get_glade_auto_type(obj) (get_value_from_gobject((obj)))

#endif

static VALUE cGladeXML;

static GladeXML *
get_glade(VALUE self)
{
    GladeXML *xml;
    Data_Get_Struct(self, GladeXML, xml);
    return xml;
}

static void
xml_connect(const gchar *handler_name, GtkObject *_source,
            const gchar *signal_name, const gchar *signal_data,
            GtkObject *_target, gboolean after, gpointer user_data)
{
    VALUE self = (VALUE)user_data;
    VALUE source, target, signal, handler, data;
    
    source = _source? get_glade_auto_type(_source) : Qnil;
    target = _target? get_glade_auto_type(_target) : Qnil;
    
    signal = signal_name ? rb_str_new2(signal_name) : Qnil;
    handler = handler_name ? rb_str_new2(handler_name) : Qnil;
    data = signal_data ? rb_str_new2(signal_data) : Qnil;
    
    rb_funcall(self, rb_intern("doConnect"), 5, source, target, signal, handler, data); 
}

static VALUE
rb_gladexml_get_widget(VALUE self, VALUE nameString)
{
    GtkWidget *widget;
    widget = glade_xml_get_widget(get_glade(self), STR2CSTR(nameString));
    return widget ? get_glade_auto_type(GTK_OBJECT(widget)) : Qnil;
}

static VALUE
rb_gladexml_get_widget_by_long_name(VALUE self, VALUE nameString)
{
    GtkWidget *widget;
    widget = glade_xml_get_widget_by_long_name(get_glade(self),
                                               STR2CSTR(nameString));
    return widget ? get_glade_auto_type(GTK_OBJECT(widget)) : Qnil;
}
    
static VALUE
rb_gladexml_new(int argc, VALUE *argv, VALUE self)
{
    VALUE fileString, rootString, handlerProc;
    GladeXML *xml;
    char *fileName;
    char *root;

    rb_scan_args(argc, argv, "11&", &fileString, &rootString, &handlerProc);

    fileName = NIL_P(fileString) ? 0 : STR2CSTR(fileString);
    root = NIL_P(rootString) ? 0 : STR2CSTR(rootString);
    
#ifndef ENABLE_GNOME
    glade_init();
#else
    glade_gnome_init();
#endif
    xml = glade_xml_new(fileName, root);

    if(xml)
    {
        /* Once constructed, this means a GladeXML object can never be freed. */
        self = Data_Wrap_Struct(cGladeXML, 0, 0, xml);
        rb_iv_set(self, "@handlerProc", handlerProc);
        glade_xml_signal_autoconnect_full(xml, xml_connect, (gpointer)self);
    }
    else
    {
        /* why does that raise not work properly?? */
        rb_raise(rb_eIOError, "could not load glade file %s", fileName);
    }

    return self;
}

void Init_lglade()
{
    /*
     * It is important that the first thing we do is load the Ruby-Gtk
     * extension module. This prevents some confusing errors if the user
     * doesn't do it themselves.
     */
    rb_require("gtk");
    cGladeXML = rb_define_class("GladeXML", rb_cObject);
    rb_define_singleton_method(cGladeXML, "new", rb_gladexml_new, -1);
    rb_define_method(cGladeXML, "getWidget", rb_gladexml_get_widget, 1);
    rb_define_method(cGladeXML, "getWidgetByLongName", rb_gladexml_get_widget_by_long_name, 1);    

    rb_eval_string(
        "class GladeXML														\n"
        "   def doConnect(source, target, signal, handler, data)            \n"
        "       if target                                                   \n"
        "           signalProc = target.method(handler)                     \n"
        "       else                                                        \n"
        "           signalProc = @handlerProc.call(handler)                 \n"
        "       end                                                         \n"
        "       case signalProc.arity                                       \n"
        "           when 0                                                  \n"
        "               source.signal_connect(signal) {signalProc.call}     \n"
        "           when 1                                                  \n"
        "               source.signal_connect(signal, &signalProc)          \n"
        "           else                                                    \n"
        "               source.signal_connect(signal, data, &signalProc)    \n"
        "       end                                                         \n"
        "   end                                                             \n"
        "end                                                                \n"
    );
}
