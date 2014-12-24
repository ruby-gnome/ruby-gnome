# Copyright (c) 2013 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Links

GtkLabel can show hyperlinks. The default action is to call gtk_show_uri() on their URI, but it is possible to override this with a custom handler.
=end
require 'common'

module Demo
  class Links < BasicWindow
    def initialize
      super('Links')

      title = 'Links'
      border_width = 12
      signal_connect(:destroy) {self.destroy}

      label = Gtk::Label.new "Some <a href=\"http://en.wikipedia.org/wiki/Text\"" +
                             "title=\"plain text\">text</a> may be marked up\n" +
                             "as hyperlinks, which can be clicked\n" +
                             "or activated via <a href=\"keynav\">keynav</a>\n" +
                             "and they work fine with other markup, like when\n" +
                             "searching on <a href=\"http://www.google.com/\">" +
                             "<span color=\"#0266C8\">G</span><span color=\"#F90101\">o</span>" +
                             "<span color=\"#F2B50F\">o</span><span color=\"#0266C8\">g</span>" +
                             "<span color=\"#00933B\">l</span><span color=\"#F90101\">e</span>" +
                             "</a>."
      label.use_markup = true
      label.signal_connect(:activate_link) {|widget, uri| activate_link uri}
      add label
      label.show
    end

    def activate_link uri
      if uri == 'keynav'
        dialog = Gtk::MessageDialog.new :parent => self,
                   :flags => :destroy_with_parent,
                   :type => :info,
                   :buttons_type => :ok

        dialog.markup = "The term <i>keynav</i> is a shorthand for " +
                        "keyboard navigation and refers to the process of using " +
                        "a program (exclusively) via keyboard input."
        dialog.present
        dialog.signal_connect(:response) {dialog.destroy}
        return true
      end
      false
    end
  end
end