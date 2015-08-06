#! /usr/bin/env ruby
=begin
  main.rb - Main class of gtk-demo.

  Copyright (c) 2003-2013 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: main.rb,v 1.20 2007/07/09 13:21:54 ggc Exp $
=end

require "gtk3"

$LOAD_PATH.unshift(File.dirname(__FILE__))

module Demo
  class Main < Gtk::Window
    TITLE_COLUMN, FILENAME_COLUMN, CLASS_COLUMN, ITALIC_COLUMN = 0, 1, 2, 3

    def initialize
      super()

      @current_file = nil

      @info_buffer = Gtk::TextBuffer.new
      @source_buffer = Gtk::TextBuffer.new

      set_title("Ruby/GTK+ Code Demos")
      signal_connect("destroy") { Gtk.main_quit }

      signal_connect("key_press_event") do |_widget, event|
        if event.state.control_mask? && event.keyval == Gdk::Keyval::KEY_q
          destroy
          true
        else
          false
        end
      end

      hbox = Gtk::Box.new(:horizontal)
      add(hbox)

      tree = create_tree
      scrolled_window = Gtk::ScrolledWindow.new
      scrolled_window.set_policy :never, :automatic
      scrolled_window.add(tree)
      hbox.pack_start(scrolled_window, :expand => false, :fill => false, :padding => 0)

      notebook = Gtk::Notebook.new
      hbox.pack_start(notebook, :expand => true, :fill => true, :padding => 0)

      notebook.append_page(create_text(@info_buffer, false),
                           Gtk::Label.new("_Info", :use_underline => true))

      notebook.append_page(create_text(@source_buffer, true),
                           Gtk::Label.new("_Source", :use_underline => true))

      @info_buffer.create_tag("title",
                              "font" => "Sans 18")

      @source_buffer.create_tag("comment",
                                "foreground" => "red")
      @source_buffer.create_tag("const",
                                "foreground" => "ForestGreen")
      @source_buffer.create_tag("string",
                                "foreground" => "RosyBrown",
                                "weight" => Pango::FontDescription::WEIGHT_BOLD
                                )
      @source_buffer.create_tag("reserved",
                                "foreground" => "purple")
    end

    def script_info(path)
      title = nil
      klass = nil
      depend = nil

      file = File.open(path)
      file.each do |ln|
        if !title && ln =~ /^=\s+(.*)$/
          title = Regexp.last_match(1)
          if title =~ /^(.*)\((.+?)\)$/
            title = Regexp.last_match(1)
            depend = Regexp.last_match(2)
          end
        elsif !klass && ln =~ /\s*class\s+([A-Z][A-Za-z0-9_]*)/
          klass = Regexp.last_match(1)
        end

        break if title && klass
      end

      raise "File not found: #{path}." unless klass

      return title, klass.intern, depend
    end

    def generate_index
      # Target scripts
      scripts = Dir.glob(File.join(File.dirname(__FILE__), "*.rb"))

      # Generate index tree
      children = {}
      index = []

      scripts.each do |script|
        next if ["common.rb", "main.rb"].include?(File.basename(script))
        title, klass, depend = script_info(script)

        next if depend && !Gtk.const_defined?(depend)

        if title =~ %r{^(.+?)/(.+)$}
          parent = Regexp.last_match(1)
          child = Regexp.last_match(2)

          unless children[parent]
            children[parent] = []
            index += [[parent, nil, nil, []]]
          end

          children[parent] += [[child, script, klass]]
        else
          index += [[title, script, klass]]
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
        row[3] = children[row[0]] if row[3]
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

      selection.set_mode(:browse)
      tree_view.set_size_request(200, -1)

      append_children(model, generate_index)

      cell = Gtk::CellRendererText.new
      cell.style = Pango::FontDescription::STYLE_ITALIC
      column = Gtk::TreeViewColumn.new("Widget (double click for demo)", cell,
                                       "text" => TITLE_COLUMN,
                                       "style_set" => ITALIC_COLUMN
                                       )

      tree_view.append_column(column)

      selection.signal_connect("changed") do |changed_selection|
        iter = changed_selection.selected
        load_file(iter.get_value(FILENAME_COLUMN)) if iter
      end
      tree_view.signal_connect("row_activated") do |_tree_view, path, _column|
        row_activated_cb(_tree_view.model, path)
      end

      tree_view.expand_all
      tree_view
    end

    def append_children(model, source, parent = nil)
      source.each do |title, filename, klass, children|
        iter = model.append(parent)

        [title, filename, klass].each_with_index do |value, i|
          iter.set_value(i, value) if value
        end
        iter.set_value(ITALIC_COLUMN, false)

        append_children(model, children, iter) if children
      end
    end

    def row_activated_cb(model, path)
      iter = model.get_iter(path)
      iter.set_value(ITALIC_COLUMN, true)

      lib = iter.get_value(FILENAME_COLUMN)
      if lib
        require_relative(File.basename(lib))

        klass = Demo.const_get(iter.get_value(CLASS_COLUMN))
        window = klass.new
        window.signal_connect("destroy") do
          iter.set_value(ITALIC_COLUMN, false)
        end
        window.show_all
        @window = window
      end
    end

    def create_text(buffer, is_source)
      scrolled_window = Gtk::ScrolledWindow.new
      scrolled_window.set_policy(:automatic,
                                 :automatic)
      scrolled_window.set_shadow_type(:in)

      text_view = Gtk::TextView.new

      text_view.set_buffer(buffer)
      text_view.set_editable(false)
      text_view.set_cursor_visible(false)

      scrolled_window.add(text_view)

      if is_source
        font_desc = Pango::FontDescription.new("Monospace 12")
        text_view.override_font(font_desc)

        text_view.set_wrap_mode(:none)
      else
        text_view.set_wrap_mode(:word)
        text_view.set_pixels_above_lines(2)
        text_view.set_pixels_below_lines(2)
      end

      scrolled_window
    end

    def fontify(start_iter = @source_buffer.start_iter,
                end_iter = @source_buffer.end_iter)
      str = @source_buffer.get_text(start_iter, end_iter, true)

      tokenizer = RubyTokonizer.new
      tokenizer.tokenize(str, start_iter.offset) do |tag, start, last|
        @source_buffer.apply_tag(tag.to_s,
                                 @source_buffer.get_iter_at(:offset => start),
                                 @source_buffer.get_iter_at(:offset => last))
      end
    end

    def load_file(filename)
      return if filename == @current_file

      @info_buffer.delete(*@info_buffer.bounds)

      @source_buffer.delete(*@source_buffer.bounds)

      file = begin
               File.open(filename)
             rescue
               $stderr.puts "Cannot open: #{$ERROR_INFO}" if $DEBUG
               return
             end
      start = @info_buffer.get_iter_at(:offset => 0)
      state = :before_header

      file.each do |line|
        case state
        when :before_header
          state = :in_header if line =~ /^=begin$/
        when :in_header
          if line =~ /^=end$/
            state = :body
            start = @source_buffer.get_iter_at(:offset => 0)
          elsif line =~ /^=\s+(.*)$/
            title = Regexp.last_match(1)
            title.gsub!(/\s*\(.*\)$/, "") # Delete depend field

            last = start

            @info_buffer.insert(last, title)
            start = last.clone

            start.backward_chars(title.length)
            @info_buffer.apply_tag("title", start, last)

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
    RESERVED_WORDS_PATTERN = Regexp.compile(/(^|\s+)(#{RESERVED_WORDS.collect { |pat| Regexp.quote(pat) }.join("|")})(\s+|$)/)

    def tokenize(str, index = 0)
      until str.empty?
        tag = nil

        case str
        when /".+?"/, /".+?"/
          tag = :string
        when /#.*$/
          tag = :comment
        when RESERVED_WORDS_PATTERN
          tag = :reserved
        when /[A-Z][A-Za-z0-9_]+/
          tag = :const
        end

        if tag
          tokenize($LAST_MATCH_INFO.pre_match, index) do |*args|
            yield(*args)
          end
          yield(tag, index + $LAST_MATCH_INFO.begin(0), index + $LAST_MATCH_INFO.end(0))
          index += (str.length - $LAST_MATCH_INFO.post_match.length)
          str = $LAST_MATCH_INFO.post_match
        else
          index += str.length
          str = ""
        end
      end
    end
  end
end

target = ARGV.shift
if target
  Demo::INDEX.each do |_title, filename, klass_symbol, _children|
    if target == filename || target == klass_symbol.id2name
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
