#include "ruby.h"

#include <glade/glade.h>
#include <gtk/gtk.h>
#include "../gtk/src/rbgtk.h"

VALUE cGladeXML;

static void xml_connect(const gchar *handler_name, GtkObject *_source,
						const gchar *signal_name, const gchar *signal_data,
						GtkObject *_target, gboolean after, gpointer self)
{
	VALUE source, target, signal, handler, data;
	
	source = _source? get_value_from_gobject(_source) : Qnil;
	target = _target? get_value_from_gobject(_target) : Qnil;
	
	signal = signal_name ? rb_str_new2(signal_name) : Qnil;
	handler = handler_name ? rb_str_new2(handler_name) : Qnil;
	data = signal_data ? rb_str_new2(signal_data) : Qnil;
	
	rb_funcall((VALUE) self, rb_intern("doConnect"), 5, source, target, signal, handler, data); 
}

static VALUE
rb_gladexml_get_widget(VALUE self, VALUE name)
{
        GladeXML *xml = GLADE_XML(get_gobject(self));
	
	GtkWidget *widget = glade_xml_get_widget(xml, STR2CSTR(name));
	return widget ? get_value_from_gobject(GTK_OBJECT(widget)) : Qnil;
}

static VALUE
rb_gladexml_get_widget_by_long_name(VALUE self, VALUE name)
{
        GladeXML *xml = GLADE_XML(get_gobject(self));
	
	GtkWidget *widget = glade_xml_get_widget_by_long_name(xml, STR2CSTR(name));
	return widget ? get_value_from_gobject(GTK_OBJECT(widget)) : Qnil;
}
	
static VALUE
rb_gladexml_init(int argc, VALUE *argv, VALUE self)
{
	VALUE fileString, rootString, handlerProc;
	GladeXML *xml;
	char *fileName;
	char *root;
	
	rb_scan_args(argc, argv, "11&", &fileString, &rootString, &handlerProc);

	rb_iv_set(self, "@handlerProc", handlerProc);
	
	fileName = STR2CSTR(fileString);
	root = NIL_P(rootString)? 0 : STR2CSTR(rootString);
	
	glade_init();
	xml = glade_xml_new(fileName, root);
	if(xml)
		glade_xml_signal_autoconnect_full(xml, xml_connect, (gpointer) self);
	else
		rb_raise(rb_eIOError, "could not load glade file %s", fileName);

	/* why does that raise not work properly?? */
	
        set_gobject(self, GTK_OBJECT(xml));

	return Qnil;
}
		
void Init_lglade()
{
	rb_require("gtk");
	cGladeXML = rb_define_class("GladeXML", gObject);
	rb_define_method(cGladeXML, "initialize", rb_gladexml_init, -1);
	rb_define_method(cGladeXML, "getWidget", rb_gladexml_get_widget, 1);
	rb_define_method(cGladeXML, "getWidgetByLongName", rb_gladexml_get_widget_by_long_name, 1);	

/***
class GladeXML
	def doConnect(source, target, signal, handler, data)
		if target
			signalProc = target.method(handler)
		else
			signalProc = @handlerProc.call(handler)
		end
		case signalProc.arity
			when 0
				source.signal_connect(signal) {signalProc.call}
			when 1
				source.signal_connect(signal, &signalProc)
			else
				source.signal_connect(signal, data, &signalProc)
		end
	end
end
***/

	rb_eval_string("class GladeXML\n def doConnect(source, target, signal, handler, data)\nif target\n signalProc = target.method(handler)\n else\n signalProc = @handlerProc.call(handler)\n end\ncase signalProc.arity\n when 0\n source.signal_connect(signal) {signalProc.call}\nwhen 1\n source.signal_connect(signal, &signalProc)\n else\n source.signal_connect(signal, data, &signalProc)\nend\n end\n end\n");
}
