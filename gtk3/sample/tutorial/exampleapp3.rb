require "gtk3"

require "fileutils"

current_path = File.expand_path(File.dirname(__FILE__))
gresource_bin = "#{current_path}/exampleapp.gresource"
gresource_xml = "#{current_path}/exampleapp.gresource.xml"

Dir.chdir(File.dirname(gresource_xml)) do
  system("glib-compile-resources",
         "--target", gresource_bin,
         File.basename(gresource_xml))
end

at_exit do
  FileUtils.rm_f(gresource_bin)
end

resource = Gio::Resource.load(gresource_bin)
Gio::Resources.register(resource)

class ExampleAppWindow < Gtk::ApplicationWindow
  # https://github.com/ruby-gnome2/ruby-gnome2/pull/445
  # https://github.com/ruby-gnome2/ruby-gnome2/issues/503
  type_register
  class << self
    def init
      set_template(:resource => "/org/gtk/exampleapp/window.ui")
      bind_template_child("stack")
    end
  end

  def initialize(application)
    super(:application => application)
  end

  def open(file)
    basename = file.basename
    scrolled = Gtk::ScrolledWindow.new
    scrolled.show
    scrolled.set_hexpand(true)
    scrolled.set_vexpand(true)
    view = Gtk::TextView.new
    view.set_editable(false)
    view.set_cursor_visible(false)
    view.show
    scrolled.add(view)
    stack.add_titled(scrolled, basename, basename)
    stream = file.read
    puts basename
    view.buffer.text = stream.read
  end
end

class ExampleApp < Gtk::Application
  def initialize
    super("org.gtk.exampleapp", :handles_open)

    signal_connect "activate" do |application|
      window = ExampleAppWindow.new(application)
      window.present
    end

    signal_connect "open" do |application, files, hint|
      windows = application.windows
      win = nil
      unless windows.empty?
        win = windows.first
      else
        win = ExampleAppWindow.new(application)
      end

      files.each { |file| win.open(file) }
        
      win.present
    end

  end
end

app = ExampleApp.new

puts app.run(ARGV)
