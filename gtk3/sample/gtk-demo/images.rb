# Copyright (c) 2017 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Images

 GtkImage is used to display an image; the image can be in a number of formats.
 Typically, you load an image into a GdkPixbuf, then display the pixbuf.

 This demo code shows some of the more obscure cases, in the simple
 case a call to gtk_image_new_from_file() is all you need.

 If you want to put image data in your program as a C variable,
 use the make-inline-pixbuf program that comes with GTK+.
 This way you won't need to depend on loading external files, your
 application binary can be self-contained.
=end

require "thread"

class ImagesDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Images"
    @window.signal_connect "destroy" do
      GLib::Source.remove(@load_timeout) if @load_timeout
      @pixbuf_loader.close if @pixbuf_loader
    end

    @vbox = Gtk::Box.new(:vertical, 8)
    @vbox.margin = 16
    @window.add(@vbox)

    frame = generate_frame("<u>Image loaded from a file </u>")
    image = Gtk::Image.new(:icon_name => "gtk3-demo", :size => :dialog)
    frame.add(image)

    # Animation
    frame = generate_frame("<u>Animation loaded from a file</u>")
    image = Gtk::Image.new(:resource => "/images/floppybuddy.gif")
    frame.add(image)

    # Symbolic icon
    frame = generate_frame("<u>Symbolic themed icon</u>")
    gicon = Gio::ThemedIcon.new("battery-caution-charging-symbolic")
    image = Gtk::Image.new(:icon => gicon, :size => :dialog)
    frame.add(image)

    # Progressive
    frame = generate_frame("<u>Progressive image loading</u>")

    # Create an empty image for now; the progressive loader
    # will create the pixbuf and fill it in.
    image = Gtk::Image.new
    frame.add(image)
    start_progressive_loading(image)

    # Sensitivity control
    button = Gtk::ToggleButton.new(:label => "_Insensitive",
                                   :use_underline => true)
    @vbox.pack_start(button)
    button.signal_connect "toggled" do |toggle|
      @vbox.children.each do |child|
        child.sensitive = !toggle.active? if child.class != Gtk::ToggleButton
      end
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

  private

  def start_progressive_loading(image)
    # This is obviously totally contrived (we slow down loading
    # on purpose to show how incremental loading works).
    # The real purpose of incremental loading is the case where
    # you are reading data from a slow source such as the network.
    # The timeout simply simulates a slow data source by inserting
    # pauses in the reading process.
    image_stream = nil
    @pixbuf_loader = nil
    @load_timeout = GLib::Timeout.add(150) do
      if image_stream
        begin
          buf = image_stream.read(256)
        rescue => error
          buf = ""
          show_dialog_on_error("Failure reading image file 'alphatest.png': \
                               #{error.message}")
        end

        if buf.empty?
          # Errors can happen on close, e.g. if the image
          # file was truncated we'll know on close that
          # it was incomplete.
          begin
            image_stream.close
          rescue => error
            show_dialog_on_error("Failed to load image: #{error.message}")
          end

          begin
            @pixbuf_loader.close
          rescue => error
            show_dialog_on_error("Failed to load image: #{error.message}")
          end
          @pixbuf_loader = nil
          GLib::Source::REMOVE
        else
          begin
            @pixbuf_loader.write(buf)
          rescue => error
            show_dialog_on_error("Failed to load image: #{error.message}")
          end
          GLib::Source::CONTINUE
        end
      else
        begin
          image_stream = Gio::Resources.open_stream("/images/alphatest.png")
        rescue => error
          show_dialog_on_error(error.message)
        end
        @pixbuf_loader.close if @pixbuf_loader
        @pixbuf_loader = GdkPixbuf::PixbufLoader.new
        @pixbuf_loader.signal_connect "area-prepared" do |loader|
          pixbuf = loader.pixbuf
          pixbuf.fill(0xaaaaaaff)
          image.from_pixbuf = pixbuf
        end

        @pixbuf_loader.signal_connect "area-updated" do
          # progressive_updated_callback
          # We know the pixbuf inside the GtkImage has changed, but the image
          # itself doesn't know this; so give it a hint by setting the pixbuf
          # again. Queuing a redraw used to be sufficient, but nowadays
          # GtkImage uses GtkIconHelper which caches the pixbuf state and will
          # just redraw from the cache.
          pixbuf = image.pixbuf
          image.from_pixbuf = pixbuf
        end
        GLib::Source::CONTINUE
      end
    end
    source = GLib::MainContext.default.find_source(@load_timeout)
    source.name = "[gtk+] progressive_timeout"
  end

  def show_dialog_on_error(message)
    dialog = Gtk::MessageDialog.new(:parent => @window,
                                    :flags => :destroy_with_parent,
                                    :type => :error, :buttons => :close,
                                    :message => message)
    dialog.modal = true
    dialog.present
    dialog.signal_connect("response") { |w| w.destroy }
    GLib::Source.remove(@load_timeout)
    @load_timeout = nil
    false
  end

  def generate_frame(title)
    label = Gtk::Label.new
    label.markup = title
    @vbox.pack_start(label)

    frame = Gtk::Frame.new
    frame.shadow_type = :in
    frame.halign = :center
    frame.valign = :center
    @vbox.pack_start(frame)
    frame
  end
end
