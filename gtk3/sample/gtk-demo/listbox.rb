# Copyright (c) 2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  List Box

 GtkListBox allows lists with complicated layouts, using
 regular widgets supporting sorting and filtering.

=end
class ListboxDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "List Box"
    @window.set_default_size(400, 600)

    vbox = Gtk::Box.new(:vertical, 12)
    @window.add(vbox)
    label = Gtk::Label.new("Messages from Gtk+ and friends")
    vbox.pack_start(label, :expand => false, :fill => false, :padding => 0)
    scrolled = Gtk::ScrolledWindow.new
    scrolled.set_policy(:never, :automatic)
    vbox.pack_start(scrolled, :expand => true, :fill => true, :padding => 0)

    listbox = Gtk::ListBox.new
    scrolled.add(listbox)

    listbox.set_sort_func do |a, b|
      a.sort(b)
    end
    listbox.activate_on_single_click = false
    listbox.signal_connect("row-activated", &:row_activated)
    vbox.show_all

    data = Gio::Resources.lookup_data("/listbox/messages.txt", 0)
    data.to_s.each_line do |line|
      message = Message.new(line)
      row = GtkMessageRow.new(message)
      row.show
      listbox.add(row)
    end
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
    @window
  end
end

class Message
  attr_reader :id, :sender_name, :sender_nick, :message, :time, :reply_to,
              :resent_by
  attr_accessor :n_favorites, :n_reshares

  def initialize(str)
    strv = str.split("|")
    parse(strv)
  end

  private

  def parse(array_of_strings)
    @id = array_of_strings[0].to_i
    @sender_name = array_of_strings[1]
    @sender_nick = array_of_strings[2]
    @message = array_of_strings[3]
    @time = array_of_strings[4].to_i
    @reply_to = array_of_strings[5] || nil
    @resent_by = array_of_strings[6] || nil
    @n_favorites = array_of_strings[7] ? array_of_strings[7].to_i : nil
    @n_reshares = array_of_strings[8] ? array_of_strings[8].to_i : nil
  end
end

class GtkMessageRow < Gtk::ListBoxRow

  type_register
  class << self
    def init
      set_template(:resource => "/listbox/listbox.ui")
      bind_template_child("content_label")
      bind_template_child("source_name")
      bind_template_child("source_nick")
      bind_template_child("short_time_label")
      bind_template_child("detailed_time_label")
      bind_template_child("extra_buttons_box")
      bind_template_child("details_revealer")
      bind_template_child("avatar_image")
      bind_template_child("resent_box")
      bind_template_child("resent_by_button")
      bind_template_child("n_reshares_label")
      bind_template_child("n_favorites_label")
      bind_template_child("expand_button")

      set_connect_func do |name|
        method(name)
      end
    end

    private

    def favorite_clicked(button)
      row = button.parent
      row = row.parent while row.class != GtkMessageRow
      row.message.n_favorites += 1
      row.message_row_update
    end

    def expand_clicked(button)
      row = button.parent
      row = row.parent while row.class != GtkMessageRow
      row.row_expand
    end

    def reshare_clicked(button)
      row = button.parent
      row = row.parent while row.class != GtkMessageRow
      row.message.n_reshares += 1
      row.message_row_update
    end
  end

  attr_reader :message

  def initialize(message)
    super()
    @message = message
    @avatar_pixbuf_other = GdkPixbuf::Pixbuf.new(:resource => "/listbox/apple-red.png",
                                                 :width => 32, :height => 32,
                                                 :preserve_aspect_ratio => false)
    message_row_update
    extra_buttons_box.hide
    extra_buttons_box.visible = false
    extra_buttons_box.unset_state_flags([:prelight, :selected])
    set_state_flags(Gtk::StateFlags::NORMAL, false)

    signal_connect "state-flags-changed" do |widget, _previous_flags|
      flags = widget.state_flags

      is_prelight_or_selected = flags & (Gtk::StateFlags::PRELIGHT |
                                         Gtk::StateFlags::SELECTED)
      if is_prelight_or_selected.nonzero?
        widget.extra_buttons_box.visible = true
      else
        widget.extra_buttons_box.visible = false
      end
    end
  end

  def message_row_update
    source_name.text = @message.sender_name
    source_nick.text = @message.sender_nick
    content_label.text = @message.message
    short_time_label.text = Time.at(@message.time).utc.strftime("%e %b %y")
    detailed_time_label.text = Time.at(@message.time).utc.strftime("%Y - %e %b %Y")
    n_favorites_label.visible = !@message.n_favorites.zero?
    n_favorites_label.markup = "<b>#{@message.n_favorites}</b>\nFavorites"
    n_reshares_label.visible = !@message.n_reshares.zero?
    n_reshares_label.markup = "<b>#{@message.n_reshares}</b>\nReshares"
    resent_box.visible = !@message.resent_by.nil?
    resent_by_button.label = @message.resent_by if @message.resent_by
    if @message.sender_nick == "@GTKtoolkit"
      avatar_image.set_from_icon_name("gtk3-demo", Gtk::IconSize::DND)
    else
      avatar_image.from_pixbuf = @avatar_pixbuf_other
    end
  end

  def sort(b)
    @message.time - b.message.time
  end

  def row_expand
    expand = !details_revealer.reveal_child?
    details_revealer.reveal_child = expand
    expand ? expand_button.label = "Hide" : expand_button.label = "Expand"
  end
end
