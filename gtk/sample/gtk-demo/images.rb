# $Id: images.rb,v 1.2 2003/03/21 13:59:46 mutoh Exp $
=begin
= Images

Gtk::Image is used to display an image; the image can be in a number of formats.
Typically, you load an image into a Gdk::Pixbuf, then display the pixbuf.

This demo code shows some of the more obscure cases, in the simple
case a call to Gtk::Image.new is all you need.

If you want to put image data in your program as a String variable,
use the make-inline-pixbuf program that comes with GTK+.
This way you won't need to depend on loading external files, your
application binary can be self-contained.
=end
require 'common'

module Demo
  class Images < BasicWindow
    def initialize
      @pixbuf_loader = nil
      @load_timeout = 0
      @image_stream = nil

      super('Images')
      signal_connect('destroy') do
	cleanup_callback
      end
      
      self.border_width = 8

      vbox = Gtk::VBox.new(false, 8)
      vbox.border_width = 8
      add(vbox)

      label = Gtk::Label.new
      label.set_markup('<u>Image loaded from a file</u>')
      vbox.pack_start(label, false, false, 0)
      
      frame = Gtk::Frame.new
      frame.shadow_type = Gtk::SHADOW_IN
      
      # The alignment keeps the frame from growing when users resize
      # the window
      align = Gtk::Alignment.new(0.5, 0.5, 0, 0)
      align.add(frame)
      vbox.pack_start(align, false, false, 0)

      # demo_find_file looks in the the current directory first,
      # so you can run gtk-demo without installing GTK, then looks
      # in the location where the file is installed.
      pixbuf = nil
      begin
	filename = Demo.find_file('gtk-logo-rgb.gif')
	pixbuf = Gdk::Pixbuf.new(filename)
      rescue
	# This code shows off error handling. You can just use
	# Gtk::Image.new instead if you don't want to report
	# errors to the user. If the file doesn't load when using
	# Gtk::Image.new, a 'missing image' icon will
	# be displayed instead.
	dialog = Gtk::MessageDialog.new(self,
					Gtk::Dialog::DESTROY_WITH_PARENT,
					Gtk::MessageDialog::ERROR,
					Gtk::MessageDialog::BUTTONS_CLOSE,
					"Unable to open image file 'gtk-logo-rgb.gif': #{$1}")
	
	dialog.signal_connect('response') do |widget, data|
	  widget.destroy
	end
	dialog.show
      end
      
      image = Gtk::Image.new(pixbuf)
      frame.add(image)


      # Animation
      label = Gtk::Label.new
      label.set_markup('<u>Animation loaded from a file</u>')
      vbox.pack_start(label, false, false, 0)
      
      frame = Gtk::Frame.new
      frame.shadow_type = Gtk::SHADOW_IN
      
      # The alignment keeps the frame from growing when users resize
      # the window
      align = Gtk::Alignment.new(0.5, 0.5, 0, 0)
      align.add(frame)
      vbox.pack_start(align, false, false, 0)

      filename = Demo.find_file('floppybuddy.gif')
      image = Gtk::Image.new(filename)
      frame.add(image)
      

      # Progressive
      label = Gtk::Label.new
      label.set_markup('<u>Progressive image loading</u>')
      vbox.pack_start(label, false, false, 0)
      
      frame = Gtk::Frame.new(nil)
      frame.shadow_type = Gtk::SHADOW_IN
      
      # The alignment keeps the frame from growing when users resize
      # the window
      align = Gtk::Alignment.new(0.5, 0.5, 0, 0)
      align.add(frame)
      vbox.pack_start(align, false, false, 0)

      # Create an empty image for now; the progressive loader
      # will create the pixbuf and fill it in.
      image = Gtk::Image.new
      frame.add(image)

      start_progressive_loading(image)

      # Sensitivity control
      button = Gtk::ToggleButton.new('_Insensitive', true)
      vbox.pack_start(button, false, false, 0)

      button.signal_connect('toggled') do |widget|
	vbox.children.each do |widget|
	  if widget != button
	    widget.sensitive = ! button.active?
	  end
	end
      end
    end

    def start_progressive_loading(image)
      @load_timeout = Gtk.timeout_add(150) do
	progressive_timeout(image)
      end
    end

    def progressive_timeout(image)
      if @image_stream
	buf = @image_stream.read(1024)
	
	@pixbuf_loader.write(buf)
	
	if @image_stream.eof?
	  @image_stream.close
	  @image_stream = nil
	  
	  @pixbuf_loader.close
	  @pixbuf_loader = nil
	  
	  return false
	end
      else
	filename = Demo.find_file('alphatest.png')
	@image_stream = File.open(filename, 'r')

	if @pixbuf_loader != nil
	  @pixbuf_loader.close
	  @pixbuf_loader = nil
	end
	
	@pixbuf_loader = Gdk::PixbufLoader.new
	
	@pixbuf_loader.signal_connect('area_prepared') do |loader|
	  pixbuf = loader.pixbuf

	  # Avoid displaying random memory contents, since the pixbuf
	  # isn't filled in yet.
	  pixbuf.fill!(0xaaaaaaff)
	  
	  image.pixbuf = pixbuf
	end
	
	@pixbuf_loader.signal_connect('area_updated') do
	  # We know the pixbuf inside the Gtk::Image has changed, but the image
	  # itself doesn't know this; so queue a redraw.  If we wanted to be
	  # really efficient, we could use a drawing area or something
	  # instead of a Gtk::Image, so we could control the exact position of
	  # the pixbuf on the display, then we could queue a draw for only
	  # the updated area of the image.
	  image.queue_draw
	end
      end
      
      # leave timeout installed
      return true
    end

    def cleanup_callback
      if @load_timeout != 0
	Gtk.timeout_remove(@load_timeout)
      end
    end
  end
end
