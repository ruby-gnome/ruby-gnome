#!/usr/bin/env ruby
#
# Copyright (C) 2006-2016  Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

require "gtk3"
require "optparse"
require "fileutils"

current_path = File.expand_path(File.dirname(__FILE__))

gresource_bin = "#{current_path}/demo.gresource"
gresource_xml = "#{current_path}/demo.gresource.xml"

system("glib-compile-resources",
       "--target", gresource_bin,
       "--sourcedir", current_path,
       gresource_xml)

gschema_bin = "#{current_path}/gschemas.compiled"

system("glib-compile-schemas", current_path)

at_exit do
  FileUtils.rm_f([gresource_bin, gschema_bin])
end

resource = Gio::Resource.load(gresource_bin)
Gio::Resources.register(resource)

ENV["GSETTINGS_SCHEMA_DIR"] = current_path

TITLE_COLUMN = 0
FILENAME_COLUMN = 1
STYLE_COLUMN = 2

def script_info(path)
  title = depend = nil
  file = File.open(path)
  file.each do |ln|
    if !title && ln =~ /^=\s+(.*)$/
      title = Regexp.last_match(1)
      if title =~ /^(.*)\((.+?)\)$/
        title = Regexp.last_match(1)
        depend = Regexp.last_match(2)
      end
    end

    break if title
  end

  [title, depend]
end

def generate_index
  # Target scripts
  scripts = Dir.glob(File.join(File.dirname(__FILE__), "*.rb"))
  # Generate index tree
  children = {}
  index = []

  scripts.each do |script|
    next if "main.rb" == File.basename(script)
    title, depend = script_info(script)

    next if depend && !Gtk.const_defined?(depend)

    if title =~ %r{^(.+?)/(.+)$}
      parent = Regexp.last_match(1)
      child = Regexp.last_match(2)

      unless children[parent]
        children[parent] = []
        index += [[parent, nil, []]]
      end
      children[parent] += [[child, script]]
    else
      index += [[title, script]]
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
    row[2] = children[row[0]] if row[2]

    row
  end

  index.sort! do |a, b|
    a[0] <=> b[0]
  end

  index
end

def append_children(model, source, parent = nil)
  source.each do |title, filename, children|
    iter = model.append(parent)
    iter[TITLE_COLUMN] = title
    iter[FILENAME_COLUMN] = filename
    iter[STYLE_COLUMN] = Pango::Style::NORMAL

    append_children(model, children, iter) if children
  end
end

def get_demo_name_from_filename(filename)
  File.basename(filename, ".rb").tr("-", "_")
end

def get_class_name_from_filename(filename)
  pattern = get_demo_name_from_filename(filename)
  class_name = pattern.split("_").map(&:capitalize).join
  class_name << "Demo"
end

def list_demos(source, is_child = false)
  source.each do |title, filename, children|
    if is_child
      printf("%-30.30s", "\t" + title)
      printf("%-30.30s", get_demo_name_from_filename(filename))
      puts ""
    elsif filename
      printf("%-38.38s", title)
      printf("%-30.30s", get_demo_name_from_filename(filename))
      puts ""
    else
      puts "#{title} : "
    end

    list_demos(children, true) if children
  end
end

def find_demo_filename_from_name(source, name)
  demo_filename = nil
  source.each do |_title, filename, children|
    if filename && name == get_demo_name_from_filename(filename)
      demo_filename = filename
    end
    break if demo_filename
    (demo_filename = find_demo_filename_from_name(children, name)) if children
  end
  demo_filename
end

def get_demo_filename_from_name(name)
  index = generate_index
  filename = find_demo_filename_from_name(index, name)
  puts "Demo not found" unless filename

  filename
end

def run_demo_from_file(filename, window)
  class_name = get_class_name_from_filename(filename)

  require filename unless Object.const_defined?(class_name)

  klass = Object.const_get(class_name)
  demo = klass.new(window)
  demo_window = demo.run

  if demo_window.is_a?(Gtk::Window)
    demo_window.set_transient_for(window)
    demo_window.modal = true
  end
  demo_window
end

def select_treeview_item(treeview, filename)
  to_select = nil
  treeview.model.each do |model, path, iter|
    if iter[FILENAME_COLUMN] == filename
      to_select = path
      break
    end
  end
  treeview.expand_to_path(to_select)
  treeview.set_cursor(to_select, nil, false)
end

class Demo < Gtk::Application
  def initialize
    super("org.gtk.Demo", [:non_unique, :handles_command_line])

    @current_file = nil

    action = Gio::SimpleAction.new("quit")
    action.signal_connect "activate" do |_action, _parameter|
      quit
    end
    add_action(action)

    action = Gio::SimpleAction.new("about")
    action.signal_connect "activate" do |_action, _parameter|
      Gtk::AboutDialog.show(active_window,
                            "program_name" => "GTK+ Demo",
                            "version" => Gtk::Version::STRING,
                            "copyright" => "(C) 1997-2013 The GTK+ Team",
                            "license_type" => Gtk::License::LGPL_2_1,
                            "website" => "http://www.gtk.org",
                            "comments" => "Program to demonstrate GTK+ widgets",
                            "authors" => ["The GTK+ Team"],
                            "logo_icon_name" => "gtk3-demo",
                            "title" => "About GTK+ Demo")
    end

    add_action(action)
    @options = {}
    @exit_status = 0

    signal_connect "startup" do |application|
      @builder = Gtk::Builder.new(:resource => "/ui/main.ui")
      appmenu = @builder["appmenu"]
      application.set_app_menu(appmenu)

      @info_buffer = Gtk::TextBuffer.new
      @source_buffer = Gtk::TextBuffer.new

      @info_buffer.create_tag("title",
                              "font" => "Sans 18")

      @source_buffer.create_tag("comment",
                                "foreground" => "red")
      @source_buffer.create_tag("const",
                                "foreground" => "ForestGreen")
      @source_buffer.create_tag("string",
                                "foreground" => "RosyBrown",
                                "weight" => Pango::Weight::BOLD)
      @source_buffer.create_tag("reserved",
                                "foreground" => "purple")
    end

    signal_connect "activate" do |_application|
      begin
        run_application
      rescue => error
        report_error(error)
        @exit_status = 1
      end
    end

    signal_connect "command-line" do |_application, command_line|
      begin
        parse_command_line(command_line.arguments)
      rescue SystemExit => error
        error.status
      rescue OptionParser::InvalidOption => error
        puts error.message
        1
      rescue => error
        report_error(error)
        1
      else
        activate
        @exit_status
      end
    end
  end

  private

  def parse_command_line(arguments)
    parser = OptionParser.new
    parser.on("-r", "--run EXAMPLE", "Run an example") do |example|
      @options[:name] = example
    end
    parser.on("-a", "--autoquit", "Quit after a delay") do
      @options[:autoquit] = true
    end
    parser.on("-l", "--list", "List examples") do
      @options[:list] = true
    end
    parser.parse(arguments)
  end

  def report_error(error)
    puts "#{error.class}: #{error.message}"
    puts error.backtrace
  end

  def run_application
    if @options[:list]
      list_demos(generate_index)
      quit
    end

    window = @builder["window"]
    add_window(window)
    action = Gio::SimpleAction.new("run")

    action.signal_connect "activate" do |_action, _parameter|
      selection = @treeview.selection
      iter = selection.selected
      filename = iter[FILENAME_COLUMN]
      if filename
        begin
          run_demo_from_file(filename, windows.first)
        rescue => error
          puts("failed to run demo: #{filename}")
          report_error(error)
        end
      end
    end
    window.add_action(action)

    @notebook = @builder["notebook"]
    @info_view = @builder["info-textview"]
    @source_view = @builder["source-textview"]
    headerbar = @builder["headerbar"]
    @treeview = @builder["treeview"]
    model = @treeview.model
    append_children(model, generate_index)

    @source_sw = @builder["source-scrolledwindow"]
    scrollbar = @source_sw.vscrollbar

    @menu = Gtk::Menu.new

    item = Gtk::MenuItem.new(:label => "Start")
    @menu.append(item)
    item.signal_connect "activate" do
      adj = scrollbar.adjustment
      adj.value = adj.lower
    end

    item = Gtk::MenuItem.new(:label => "End")
    @menu.append(item)
    item.signal_connect "activate" do
      adj = scrollbar.adjustment
      adj.value = adj.upper - adj.page_size
    end

    @info_sw = @builder["info-scrolledwindow"]

    @menu.show_all

    scrollbar.signal_connect "popup-menu" do |_widget, _button, _activate_time|
      @menu.popup(nil, nil, 0, Gtk.current_event_time)
    end

    @treeview.signal_connect "row-activated" do |_tree_view, path, _column|
      iter = model.get_iter(path)
      filename = iter[FILENAME_COLUMN]
      begin
        demo = run_demo_from_file(filename, windows.first)
      rescue => error
        puts("failed to run demo: #{filename}")
        report_error(error)
      else
        if demo
          iter[STYLE_COLUMN] = Pango::Style::ITALIC
          demo.signal_connect "destroy" do
            iter[STYLE_COLUMN] = Pango::Style::NORMAL
          end
        end
      end
    end

    treeview_selection = @builder["treeview-selection"]
    treeview_selection.signal_connect "changed" do |selection, _model|
      iter = selection.selected
      if iter
        filename = iter[FILENAME_COLUMN]
        title = iter[TITLE_COLUMN]
        load_file(filename) if filename
        headerbar.set_title(title)
      end
    end

    window.show_all

    if @options[:name]
      filename = get_demo_filename_from_name(@options[:name])
      if filename
        select_treeview_item(@treeview, filename)
        run_demo_from_file(filename, windows.first)
      end
    end

    if @options[:autoquit]
      GLib::Timeout.add_seconds(1) do
        quit
      end
    end
  end

  def fill_buffers_from(file)
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
      when :body # Reading program body
        @source_buffer.insert(start, line)
      end
    end
  end

  def load_file(filename)
    return if filename == @current_file

    # implement add_data_tab

    @info_buffer.delete(*@info_buffer.bounds)

    @source_buffer.delete(*@source_buffer.bounds)

    file = begin
             File.open(filename)
           rescue
             $stderr.puts "Cannot open: #{$ERROR_INFO}" if $DEBUG
             return
           end

    fill_buffers_from(file)

    fontify

    @source_view.buffer = @source_buffer
    @info_view.buffer = @info_buffer

    @current_file = filename
  end

  def fontify(start_iter = @source_buffer.start_iter,
              end_iter = @source_buffer.end_iter)
    str = @source_buffer.get_text(start_iter, end_iter, true)
    tokenizer = RubyTokenizer.new
    tokenizer.tokenize(str, start_iter.offset) do |tag, start, last|
      @source_buffer.apply_tag(tag.to_s,
                               @source_buffer.get_iter_at(:offset => start),
                               @source_buffer.get_iter_at(:offset => last))
    end
  end

  class RubyTokenizer
    RESERVED_WORDS = %w(begin end module class def if then else
                        while unless do case when require yield)
    RESERVED_WORDS_PATTERN = Regexp.compile(/(^|\s+)(#{RESERVED_WORDS.collect do |pat| Regexp.quote(pat) end.join("|")})(\s+|$)/)

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

demo = Demo.new

exit(demo.run([$PROGRAM_NAME] + ARGV))
