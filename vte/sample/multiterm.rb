#!/usr/bin/env ruby

require "vte"

class MultiTerm
  def initialize
    @terminals = []
    init_window
    add_terminal
  end

  def run
    @window.show_all
    @notebook.get_nth_page(@notebook.page).grab_focus
  end

  private
  def init_window
    @window = Gtk::Window.new
    @window.signal_connect("destroy") do |widget|
      Gtk.main_quit
    end
    init_notebook
    @window.add(@notebook)
  end

  def init_notebook
    @notebook = Gtk::Notebook.new
    @last_page_index = nil
    @notebook.signal_connect_after("switch-page") do |widget, page, i|
      max_page_index = widget.n_pages - 1
      if max_page_index > 0 and i == max_page_index
        widget.page = @last_page_index
      end
    end
    @notebook.signal_connect("switch-page") do |widget, page, i|
      @last_page_index ||= i
      terminal = widget.get_nth_page(i)
      if terminal.respond_to?(:window_title)
        @last_page_index = i
        update_window_title(terminal.window_title)
      end
    end
    @notebook.tab_reorderable = true if @notebook.respond_to?(:tab_reorderable=)
    @notebook.scrollable = true
    init_new_tab_label
    dummy = Gtk::EventBox.new
    @notebook.append_page(dummy, @new_tab_label)
  end

  def init_new_tab_label
    @new_tab_label = Gtk::EventBox.new
    @new_tab_label.signal_connect("button-press-event") do |widget, event|
      add_terminal
      @notebook.page = @last_page_index
      true
    end
    # @new_tab_label.set_size_request(30, -1)
    image = Gtk::Image.new(Gtk::Stock::NEW, Gtk::IconSize::MENU)
    image.show
    @new_tab_label.add(image)
    @new_tab_label
  end

  def destroy
    @window.destroy
  end

  def add_terminal
    terminal = Vte::Terminal.new
    terminal.signal_connect("child-exited") do |widget|
      @notebook.remove_page(@notebook.page_num(widget))
      @terminals.delete(widget)
      destroy if @terminals.empty?
    end
    terminal.signal_connect("window-title-changed") do |widget|
      @notebook.set_tab_label_text(widget, widget.window_title)
      update_window_title(widget.window_title)
    end
    terminal.set_font("Monospace 14")
    terminal.fork_command
    terminal.show
    last = @notebook.n_pages
    @notebook.insert_page(last - 1, terminal)
    @terminals << terminal
  end

  def update_window_title(title)
    @window.title = title if title
  end
end

Gtk.init
multi_term = MultiTerm.new
multi_term.run
Gtk.main
