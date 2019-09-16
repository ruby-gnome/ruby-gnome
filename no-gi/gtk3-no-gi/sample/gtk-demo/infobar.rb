# Copyright (c) 2013 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Info bar

Info bar widgets are used to report important messages to the user.
=end
require 'common'

module Demo
  class InfoBar < BasicWindow
    def initialize
      super('Info Bars')

      self.border_width = 8


      vbox = Gtk::Box.new :vertical
      self.add vbox

      bar = Gtk::InfoBar.new
      vbox.pack_start bar, :expand => false, :fill => false, :padding => 0
      bar.message_type = :info
      label = Gtk::Label.new 'This is an info bar with message type GTK_MESSAGE_INFO'
      bar.content_area.pack_start label, :expand => false, :fill => false, :padding => 0

      bar = Gtk::InfoBar.new
      vbox.pack_start bar, :expand => false, :fill => false, :padding => 0
      bar.message_type = :warning
      label = Gtk::Label.new 'This is an info bar with message type GTK_MESSAGE_WARNING'
      bar.content_area.pack_start label, :expand => false, :fill => false, :padding => 0

      bar = Gtk::InfoBar.new [:ok, :ok]
      bar.signal_connect(:response) {|widget, response_id| on_bar_response widget, response_id}
      vbox.pack_start bar, :expand => false, :fill => false, :padding => 0
      bar.message_type = :question
      label = Gtk::Label.new 'This is an info bar with message type GTK_MESSAGE_QUESTION'
      bar.content_area.pack_start label, :expand => false, :fill => false, :padding => 0

      bar = Gtk::InfoBar.new
      vbox.pack_start bar, :expand => false, :fill => false, :padding => 0
      bar.message_type = :error
      label = Gtk::Label.new 'This is an info bar with message type GTK_MESSAGE_ERROR'
      bar.content_area.pack_start label, :expand => false, :fill => false, :padding => 0

      bar = Gtk::InfoBar.new
      vbox.pack_start bar, :expand => false, :fill => false, :padding => 0
      bar.message_type = :other
      label = Gtk::Label.new 'This is an info bar with message type GTK_MESSAGE_OTHER'
      bar.content_area.pack_start label, :expand => false, :fill => false, :padding => 0


      frame = Gtk::Frame.new 'Info bars'
      vbox.pack_start frame, :expand => false, :fill => false, :padding => 8

      vbox2 = Gtk::Box.new :vertical, 8
      vbox2.border_width = 8
      frame.add vbox2

      # Standard message dialog
      label = Gtk::Label.new 'An example of different info bars'
      vbox2.pack_start label, :expand => false, :fill => false, :padding => 0
    end

    def on_bar_response info_bar, response_id
      dialog = Gtk::MessageDialog.new :parent => self,
                                      :flags => [:modal, :destroy_with_parent],
                                      :type => :info,
                                      :buttons_type => :ok,
                                      :message => 'You clicked a button on an info bar'

      dialog.set_secondary_text "Your response has id %d" % response_id
      dialog.signal_connect(:response) {dialog.destroy}
      dialog.show_all
    end
  end
end