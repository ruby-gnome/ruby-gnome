require 'libglade2'

class Test
	def initialize
		@glade = GladeXML.new("test.glade") {|handler| method(handler)}
		@entry = @glade.widget("entrybox")
	end
	
	def say_hello(widget, data)
		@entry.set_text(data)
	end
	
	def quit
		Gtk.main_quit
	end
end

Test.new
Gtk.main
