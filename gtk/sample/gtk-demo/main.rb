#! /usr/bin/env ruby
# $Id: main.rb,v 1.3 2003/08/17 09:30:44 kzys Exp $

require 'gtk2'

module Demo
  INDEX = [
    ["Application main window",
      "appwindow.rb",     :AppWindow,     []],
    ["Button Boxes",
      "button_box.rb",    :ButtonBox,     []],
    ["Change Display",
      "changedisplay.rb", :ChangeDisplay, []],
    ["Color Selector",
      "colorsel.rb",      :ColorSel,      []],
    ["Dialog and Message Boxes",
      "dialog.rb",        :Dialog,        []],
    ["Drawing Area",
      "drawingarea.rb",   :DrawingArea,   []],
    ["Images",
      "images.rb",        :Images,        []],
	["Item Factory",
      "item_factory.rb",  :ItemFactory,   []],
    ["Menus",
      "menus.rb",         :Menus,         []],
    ["Paned Widgets",
      "panes.rb",         :Panes,         []],
    ["Pixbufs",
      "pixbufs.rb",       :Pixbufs,       []],
    ["Size Groups",
      "sizegroup.rb",     :SizeGroup,     []],
    ["Stock Item and Icon Browser",
      "stock_browser.rb", :StockBrowser,  []],
    ["Text Widget",
      "textview.rb",      :TextView,      []],
    ["Tree View", nil, nil,
      [
	["Editable Cells", "editable_cells.rb", :EditableCells],
	["List Store",     "list_store.rb",     :ListStore],
	["Tree Store",     "tree_store.rb",     :TreeStore],
      ]
    ],
  ]

  class Main < Gtk::Window
    TITLE_COLUMN    = 0
    FILENAME_COLUMN = 1
    FUNC_COLUMN     = 2
    ITALIC_COLUMN   = 3

    def initialize
      super()
      @instances = {}
      @current_file = ''

      @info_buffer = Gtk::TextBuffer.new
      @source_buffer = Gtk::TextBuffer.new

      set_title('Ruby/GTK+ Code Demos')
      signal_connect('destroy') do
	Gtk.main_quit
      end

      hbox = Gtk::HBox.new
      add(hbox)

      tree = create_tree
      hbox.pack_start(tree, false, false, 0)

      notebook = Gtk::Notebook.new
      hbox.pack_start(notebook, true, true, 0)

      notebook.append_page(create_text(@info_buffer, false),
			   Gtk::Label.new('_Info', true))

      notebook.append_page(create_text(@source_buffer, true),
			   Gtk::Label.new('_Source', true))

      tag = @info_buffer.create_tag('title', {'font' => 'Sans 18'})

      @source_buffer.create_tag('comment',
				{'foreground' => 'red'})
      @source_buffer.create_tag('const',
				{'foreground' => 'ForestGreen'})
      @source_buffer.create_tag('string',
				{
				  'foreground' => 'RosyBrown',
				  'weight' => Pango::FontDescription::WEIGHT_BOLD
				})
      @source_buffer.create_tag('control',
				{'foreground' => 'purple'})
    end


    def create_tree
      model = Gtk::TreeStore.new(String, String, String)

      tree_view = Gtk::TreeView.new
      tree_view.set_model(model)
      selection = tree_view.selection

      selection.set_mode(Gtk::SELECTION_BROWSE)
      tree_view.set_size_request(200, -1)

      DEMOS.each do |*args| # title, filename, class, children
	iter = model.append(nil)

	args[0, 3].each_with_index do |value, column|
	  if value
	    iter.set_value(column, value)
	  end
	end
	#iter.set_value(ITALIC_COLUMN, false)

	children = args[3]
	children.each do |*args|
	  child_iter = model.append(iter)

	  args[0, 3].each_with_index do |value, column|
	    if value
	      child_iter.set_value(column, value)
	    end
	  end
	end
      end

      cell = Gtk::CellRendererText.new

      column = Gtk::TreeViewColumn.new("Widget (double click for demo)", cell,
				       {
					 'text' => TITLE_COLUMN,
					 #'style_set' => ITALIC_COLUMN,
				       })

      tree_view.append_column(column)

      selection.signal_connect('changed') do |selection|
	iter = selection.selected
	load_file(iter.get_value(FILENAME_COLUMN))
      end
      tree_view.signal_connect('row_activated') do |tree_view, path, column|
	model = tree_view.model

	iter = model.get_iter(path)

	## TODO: Using WeakRef?
	begin
          lib = iter.get_value(FILENAME_COLUMN)
          if lib
            #require(lib)
	    load(lib)
            
            klass = eval(iter.get_value(FUNC_COLUMN))
	    @instances[klass] = nil
            if @instances[klass]
              @instances[klass].show
            else
              @instances[klass] = klass.new
              @instances[klass].show_all
            end
          end
	rescue LoadError
	  ;
	end
      end

      tree_view.expand_all
      return tree_view
    end

    def create_text(buffer, is_source)
      scrolled_window = Gtk::ScrolledWindow.new
      scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC,
				 Gtk::POLICY_AUTOMATIC)
      scrolled_window.set_shadow_type(Gtk::SHADOW_IN)

      text_view = Gtk::TextView.new

      text_view.set_buffer(buffer)
      text_view.set_editable(false)
      text_view.set_cursor_visible(false)

      scrolled_window.add(text_view)

      if is_source
	font_desc = Pango::FontDescription.new('Courier 12')
	text_view.modify_font(font_desc)

	text_view.set_wrap_mode(Gtk::TextTag::WRAP_NONE)
      else
	text_view.set_wrap_mode(Gtk::TextTag::WRAP_WORD)
	text_view.set_pixels_above_lines(2)
	text_view.set_pixels_below_lines(2)
      end

      return scrolled_window
    end

    CONTROL = %w(begin end module class def if then else while unless do case when require yield)
    CONTROL_PATTERN = Regexp.compile(/(^|\s+)(#{CONTROL.collect do |pat| Regexp.quote(pat) end.join('|')})(\s+|$)/)
    def parse_chars(str, index = 0)
      until str.empty?
	tag = nil
	case str
	when /".+?"/, /'.+?'/
	  tag = 'string'
	when /#.*$/
	  tag = 'comment'
	when CONTROL_PATTERN
	  tag = 'control'
	when /[A-Z][A-Za-z0-9_]+/
	  tag = 'const'
	end

	if tag
	  parse_chars($~.pre_match, index) do |*args|
	     yield(*args)
	  end
	  yield(tag, index + $~.begin(0), index + $~.end(0))
	  index += (str.length - $~.post_match.length)
	  str = $~.post_match
	else
	  index += str.length
	  str = ''
	end
      end
    end

    def fontify(start_iter = @source_buffer.start_iter,
		end_iter = @source_buffer.end_iter)
      str = @source_buffer.get_text(start_iter, end_iter, true)

      parse_chars(str, start_iter.offset) do |tag, start, last|
	# puts "#{tag} #{start} #{last}"
	@source_buffer.apply_tag(tag,
				 @source_buffer.get_iter_at_offset(start),
				 @source_buffer.get_iter_at_offset(last))
      end
    end


    def load_file(filename)
      if @current_file == filename
	return
      end

      @info_buffer.delete(*@info_buffer.bounds)

      @source_buffer.delete(*@source_buffer.bounds)

      file = begin
	       File.open(filename)
	     rescue
	       $stderr.puts "Cannot open: #{$!}"
	       return
	     end
      start = @info_buffer.get_iter_at_offset(0)
      state = 0

      file.each do |line|
	case state
	when 0  # before the document
	  if line =~ /^=begin$/
	    state += 1
	  end
	when 1  # in the document
	  if line =~ /^=end$/
	    state += 1
	    start = @source_buffer.get_iter_at_offset(0)
	  elsif line =~ /^=\s+(.*)$/
	    title = $1

	    last = start

	    @info_buffer.insert(last, title)
	    start = last.clone

	    start.backward_chars(title.length)
	    @info_buffer.apply_tag('title', start, last)

	    start = last
	  else
	    @info_buffer.insert(start, line)
	  end
	when 2  # Reading program body
	  @source_buffer.insert(start, line)
	end
      end

      fontify
    end
  end
end

Gtk.init

target = ARGV.shift
if target
  Demo::INDEX.each do |title, filename, klass_symbol, children|
    if target == filename or target == klass_symbol.id2name
      require filename
      
      window = Demo.const_get(klass_symbol).new
      window.show_all
      
      class << window
	def quit
	  Gtk.main_quit
	end
      end
      
      break
    end
  end
else
  main = Demo::Main.new
  main.set_default_size(600, 400)
  main.show_all
end

Gtk.main
