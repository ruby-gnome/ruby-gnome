require 'gtk'
require 'lglade'

class Test
	def initialize
		@glade = GladeXML.new("test.glade") {|handler| method(handler)}
		@entry = @glade.getWidget("entrybox")
	end
	
	def sayHello(widget, data)
		@entry.set_text(data)
	end
	
	def quit
		Gtk.main_quit
	end
end

Test.new
Gtk.main