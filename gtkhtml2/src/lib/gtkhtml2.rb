require 'gtk2'
require 'gtkhtml2.so'

module Gtk
  module Html
    LOG_DOMAIN = "GtkHtml"
  end

  class HtmlDocument
    LOG_DOMAIN = "HtmlDocument"
  end

  class HtmlView
    LOG_DOMAIN = "HtmlView"
  end
    
end

GLib::Log.set_log_domain(Gtk::Html::LOG_DOMAIN)
GLib::Log.set_log_domain(Gtk::HtmlDocument::LOG_DOMAIN)
GLib::Log.set_log_domain(Gtk::HtmlView::LOG_DOMAIN)

GLib::Log.set_log_domain("HtmlA11y")
GLib::Log.set_log_domain("HtmlCss")
GLib::Log.set_log_domain("DomViews")
GLib::Log.set_log_domain("HtmlGraphics")
GLib::Log.set_log_domain("HtmlLayout")
GLib::Log.set_log_domain("HtmlUtil")
