# Copyright (c) 2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Clipboard

 GtkClipboard is used for clipboard handling. This demo shows how to
 copy and paste text to and from the clipboard.

 It also shows how to transfer images via the clipboard or via
 drag-and-drop, and how to make clipboard contents persist after
 the application exits. Clipboard persistence requires a clipboard
 manager to run.
=end
class ClipboardDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Clipboard"

    @vbox = Gtk::Box.new(:vertical, 0)
    @vbox.margin = 8
    @window.add(@vbox)

    text = "\"Copy\" will copy the text\nin the entry to the clipboard"
    generate_entry(text, "_Copy") do |entry|
      clipboard = entry.get_clipboard(Gdk::Selection::CLIPBOARD)
      clipboard.text = entry.text
    end

    text = "\"Paste\" will paste the text from the clipboard to the entry"
    generate_entry(text, "_Paste") do |entry|
      clipboard = entry.get_clipboard(Gdk::Selection::CLIPBOARD)
      clipboard.request_text { |_clip, entry_text| entry.text = entry_text || "" }
    end

    text = "Images can be transferred via the clipboard, too"
    label = Gtk::Label.new(text)
    @vbox.pack_start(label, :expand => false, :fill => false, :padding => 0)

    @hbox = Gtk::Box.new(:horizontal, 4)
    @vbox.pack_start(@hbox, :expand => false, :fill => false, :padding => 0)

    # Create the first image
    generate_image("dialog-warning")

    # Create the second image
    generate_image("process-stop")

    # Tell the clipboard manager to make the data persistent
    clipboard = Gtk::Clipboard.get(Gdk::Selection::CLIPBOARD)
    clipboard.set_can_store([])
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
    @window
  end

  private

  def get_image_pixbuf(image)
    return image.pixbuf if image.storage_type == :pixbuf

    if image.storage_type == :icon_name
      icon_name, size = image.icon_name
      icon_theme = Gtk::IconTheme.get_for_screen(image.screen)
      width, _height = Gtk::IconSize.lookup(size)
      return icon_theme.load_icon(icon_name, width, :generic_fallback)
    else
      puts "Image storage type #{image.storage_type.to_i} not handled"
    end
  end

  def generate_entry(text, action_name)
    label = Gtk::Label.new(text)
    @vbox.pack_start(label, :expand => false, :fill => false, :padding => 0)

    hbox = Gtk::Box.new(:horizontal, 4)
    hbox.margin = 8
    @vbox.pack_start(hbox, :expand => false, :fill => false, :padding => 0)

    # Create the first entry
    entry = Gtk::Entry.new
    hbox.pack_start(entry, :expand => true, :fill => true, :padding => 0)

    # Create the button
    button = Gtk::Button.new(:label => action_name, :use_underline => true)
    hbox.pack_start(button, :expand => false, :fill => false, :padding => 0)
    button.signal_connect "clicked" do
      yield(entry) if block_given?
    end
  end

  def generate_image(icon_name)
    # Create the first image
    image = Gtk::Image.new(:icon_name => icon_name, :size => :button)
    ebox = Gtk::EventBox.new
    ebox.add(image)
    @hbox.add(ebox)

    # Make ebox a drag source
    ebox.drag_source_set(Gdk::ModifierType::BUTTON1_MASK, [], :copy)
    ebox.drag_source_add_image_targets
    ebox.signal_connect "drag-begin" do |_widget, context|
      pixbuf = get_image_pixbuf(image)
      context.set_icon_pixbuf(pixbuf, -2, -2)
    end

    ebox.signal_connect "drag-data-get" do |_widget, _context, selection_data|
      pixbuf = get_image_pixbuf(image)
      selection_data.pixbuf = pixbuf
    end

    # Accept drops on ebox
    ebox.drag_dest_set(Gtk::DestDefaults::ALL, [], Gdk::DragAction::COPY)
    ebox.drag_dest_add_image_targets
    ebox.signal_connect "drag-data-received" do |_w, _c, _x, _y, selection_data|
      pixbuf = selection_data.pixbuf
      image.from_pixbuf = pixbuf
    end

    # Context menu on ebox
    ebox.signal_connect "button-press-event" do |_widget, button|
      manage_button_press_event(image, button)
      true
    end
  end

  def manage_button_press_event(image, button)
    return false unless button.button == Gdk::BUTTON_SECONDARY
    menu = Gtk::Menu.new
    generate_menu_item(menu, "_Copy") do
      clipboard = Gtk::Clipboard.get(Gdk::Selection::CLIPBOARD)
      pixbuf = get_image_pixbuf(image)
      clipboard.image = pixbuf
    end

    generate_menu_item(menu, "_Paste") do
      clipboard = Gtk::Clipboard.get(Gdk::Selection::CLIPBOARD)
      pixbuf = clipboard.wait_for_image
      image.from_pixbuf = pixbuf if pixbuf
    end
    menu.popup(nil, nil, 3, button.time)
  end

  def generate_menu_item(menu, label)
    item = Gtk::MenuItem.new(:label => label, :use_underline => true)
    item.signal_connect "activate" do
      yield if block_given?
    end
    item.show
    menu.append(item)
  end
end
