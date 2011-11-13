#! /usr/bin/env ruby
=begin
  main.rb - Main class of gtk-demo.

  Copyright (c) 2003-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: main.rb,v 1.20 2007/07/09 13:21:54 ggc Exp $
=end

require 'gtk2'

$:.unshift(File.dirname(__FILE__))

module Demo

  class Main < Gtk::Window
    TITLE_COLUMN, FILENAME_COLUMN, CLASS_COLUMN, ITALIC_COLUMN = 0, 1, 2, 3

    def initialize
      super()

      @current_file = nil

      @info_buffer = Gtk::TextBuffer.new
      @source_buffer = Gtk::TextBuffer.new

      set_title('Ruby/GTK+ Code Demos')
      signal_connect('destroy') do
	Gtk.main_quit
      end

      signal_connect("key_press_event") do |widget, event|
        if event.state.control_mask? and event.keyval == Gdk::Keyval::GDK_q
          destroy
          true
        else
          false
        end
      end

      hbox = Gtk::HBox.new
      add(hbox)

      tree = create_tree
      scrolled_window = Gtk::ScrolledWindow.new
      scrolled_window.add(tree)
      hbox.pack_start(scrolled_window, false, false, 0)

      notebook = Gtk::Notebook.new
      hbox.pack_start(notebook, true, true, 0)

      notebook.append_page(create_text(@info_buffer, false),
			   Gtk::Label.new('_Info', true))

      notebook.append_page(create_text(@source_buffer, true),
			   Gtk::Label.new('_Source', true))

      @info_buffer.create_tag('title',
                             {'font' => 'Sans 18'})

      @source_buffer.create_tag('comment',
                               {'foreground' => 'red'})
      @source_buffer.create_tag('const',
                               {'foreground' => 'ForestGreen'})
      @source_buffer.create_tag('string',
                               {
				  'foreground' => 'RosyBrown',
                                  'weight' => Pango::FontDescription::WEIGHT_BOLD
				})
      @source_buffer.create_tag('reserved',
                               {'foreground' => 'purple'})
    end

    def script_info(path)
      title = nil
      klass = nil
      depend = nil

      file = File.open(path)
      file.each do |ln|
        if not title and ln =~ /^=\s+(.*)$/
          title = $1
          if title =~ /^(.*)\((.+?)\)$/
            title = $1
            depend = $2
          end
        elsif not klass and ln =~ /\s*class\s+([A-Z][A-Za-z0-9_]*)/
          klass = $1
        end

        if title and klass
          break
        end
      end

      if not klass
          raise "File not found: #{path}. This script should be run from its directory."
      end

      return title, klass.intern, depend
    end

    def generate_index
      # Target scripts
      scripts = Dir.glob('*.rb') - %w(common.rb main.rb)

      # Generate index tree
      children = {}
      index = []

      scripts.each do |fn|
        title, klass, depend = script_info(fn)

        if depend and not Gtk.const_defined?(depend)
          next
        end

        if title =~ %r{^(.+?)/(.+)$}
          parent = $1
          child = $2

          unless children[parent]
            children[parent] = []
            index += [[parent, nil, nil, []]]
          end

          children[parent] += [[child, fn, klass]]
        else
          index += [[title, fn, klass]]
        end
      end

      # Sort children
      children.each_key do |parent|
        children[parent].sort! do |a, b|
          a[0] <=> b[0]
        end
      end

      # Expand children
      index.collect! do |row|
        if row[3]
          row[3] = children[row[0]]
        end

        row
      end

      index.sort! do |a, b|
        a[0] <=> b[0]
      end

      index
    end

    def create_tree
      model = Gtk::TreeStore.new(String, String, String, TrueClass)

      tree_view = Gtk::TreeView.new
      tree_view.set_model(model)
      selection = tree_view.selection

      selection.set_mode(Gtk::SELECTION_BROWSE)
      tree_view.set_size_request(200, -1)

      append_children(model, generate_index)

      cell = Gtk::CellRendererText.new
      cell.style = Pango::FontDescription::STYLE_ITALIC
      column = Gtk::TreeViewColumn.new("Widget (double click for demo)", cell,
                                      {
                                         'text' => TITLE_COLUMN,
                                         'style_set' => ITALIC_COLUMN,
                                       })

      tree_view.append_column(column)

      selection.signal_connect('changed') do |selection|
	iter = selection.selected
	load_file(iter.get_value(FILENAME_COLUMN)) if iter
      end
      tree_view.signal_connect('row_activated') do |tree_view, path, column|
        row_activated_cb(tree_view.model, path)
      end

      tree_view.expand_all
      return tree_view
    end

    def append_children(model, source, parent = nil)
      source.each do |title, filename, klass, children|
	iter = model.append(parent)

	[title, filename, klass].each_with_index do |value, i|
	  if value
	    iter.set_value(i, value)
	  end
	end
        iter.set_value(ITALIC_COLUMN, false)

        if children
          append_children(model, children, iter)
        end
      end
    end

    def row_activated_cb(model, path)
      iter = model.get_iter(path)
      iter.set_value(ITALIC_COLUMN, true)

      lib = iter.get_value(FILENAME_COLUMN)
      if lib
        require(lib)
 
        klass = Demo.const_get(iter.get_value(CLASS_COLUMN))
        window = klass.new
        window.signal_connect('destroy') do
          iter.set_value(ITALIC_COLUMN, false)
        end
        window.show_all
	@window = window
      end
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
	font_desc = Pango::FontDescription.new('Monospace 12')
	text_view.modify_font(font_desc)

	text_view.set_wrap_mode(Gtk::TextTag::WRAP_NONE)
      else
	text_view.set_wrap_mode(Gtk::TextTag::WRAP_WORD)
	text_view.set_pixels_above_lines(2)
	text_view.set_pixels_below_lines(2)
      end

      return scrolled_window
    end

    def fontify(start_iter = @source_buffer.start_iter,
		end_iter = @source_buffer.end_iter)
      str = @source_buffer.get_text(start_iter, end_iter, true)

      tokenizer = RubyTokonizer.new
      tokenizer.tokenize(str, start_iter.offset) do |tag, start, last|
	@source_buffer.apply_tag(tag.to_s,
				 @source_buffer.get_iter_at_offset(start),
				 @source_buffer.get_iter_at_offset(last))
      end
    end


    def load_file(filename)
      if filename == @current_file
	return
      end

      @info_buffer.delete(*@info_buffer.bounds)

      @source_buffer.delete(*@source_buffer.bounds)

      file = begin
	       File.open(filename)
	     rescue
	       $stderr.puts "Cannot open: #{$!}" if $DEBUG
	       return
	     end
      start = @info_buffer.get_iter_at_offset(0)
      state = :before_header

      file.each do |line|
	case state
	when :before_header
	  if line =~ /^=begin$/
	    state = :in_header
	  end
	when :in_header
	  if line =~ /^=end$/
	    state = :body
	    start = @source_buffer.get_iter_at_offset(0)
	  elsif line =~ /^=\s+(.*)$/
	    title = $1
            title.gsub!(/\s*\(.*\)$/, '') # Delete depend field

	    last = start

	    @info_buffer.insert(last, title)
	    start = last.clone

	    start.backward_chars(title.length)
	    @info_buffer.apply_tag('title', start, last)

	    start = last
	  else
	    @info_buffer.insert(start, line)
	  end
	when :body  # Reading program body
	  @source_buffer.insert(start, line)
	end
      end

      fontify

      @current_file = filename
    end
  end

  class RubyTokonizer
    RESERVED_WORDS = %w(begin end module class def if then else while unless do case when require yield)
    RESERVED_WORDS_PATTERN = Regexp.compile(/(^|\s+)(#{RESERVED_WORDS.collect do |pat| Regexp.quote(pat) end.join('|')})(\s+|$)/)

    def tokenize(str, index = 0)
      until str.empty?
	tag = nil

	case str
	when /".+?"/, /'.+?'/
	  tag = :string
	when /#.*$/
	  tag = :comment
	when RESERVED_WORDS_PATTERN
	  tag = :reserved
	when /[A-Z][A-Za-z0-9_]+/
	  tag = :const
	end

	if tag
	  tokenize($~.pre_match, index) do |*args|
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
  end
end


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
