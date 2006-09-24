require 'gtk2'
$:.push(File::join(File.dirname(__FILE__),'/src'))

require 'gtksourceview'
def sw(a) 
	w = Gtk::ScrolledWindow.new
	w.add(a)
	w
end

w = Gtk::Window.new
w.signal_connect("delete-event"){Gtk::main_quit}
w.add(sw(b = Gtk::SourceView.new))
b.show_line_numbers = 1
#p b.buffer.methods
lang = Gtk::SourceLanguagesManager.new.get_language('text/x-ruby')
b.buffer.language = lang
b.buffer.highlight = true
w.set_default_size(400,300)
# customize style
lang.tags.each do |tag|
  if tag.id == "Keywords"
    style = tag.style
    style.mask |= Gtk::SourceTagStyle::USE_FOREGROUND
    style.foreground = Gdk::Color.parse("blue")
    lang.set_tag_style(tag.id, style)
  end
end
w.show_all

Gtk.main
