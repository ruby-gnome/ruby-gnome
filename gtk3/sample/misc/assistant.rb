#!/usr/bin/env ruby
=begin
  test-assistant.rb - Ruby/GTK version of testassistant.c from gtk+ 2.10 sources.

  Guillaume Cottenceau for the ruby-gnome2 project.

  Copyright (c) 2005,2006  Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: assistant.rb,v 1.1 2006/11/23 08:39:12 mutoh Exp $
=end

require 'gtk3'

if str = Gtk.check_version(2, 10, 0)
  puts "This sample requires GTK+ 2.10.0 or later"
  puts str
  exit
end

class AssistantRunner
  def initialize
    @simple_assistant = nil
    @generous_assistant = nil
    @selected_branch = "A"
    @nonlinear_assistant = nil
    @full_featured_assistant = nil
  end

  def run_simple_assistant
    @simple_assistant = run_assistant(@simple_assistant,
                                      :create_simple_assistant)
  end

  def run_generous_assistant
    @generous_assistant = run_assistant(@generous_assistant,
                                        :create_generous_assistant)
  end

  def run_nonlinear_assistant
    @nonlinear_assistant = run_assistant(@nonlinear_assistant,
                                         :create_nonlinear_assistant)
  end

  def run_full_featured_assistant
    @full_featured_assistant = run_assistant(@full_featured_assistant,
                                             :create_full_featured_assistant)
  end

  private
  def run_assistant(assistant, assistant_create_method)
    assistant ||= send(assistant_create_method)
    if !assistant.visible?
      assistant.show
    else
      assistant.destroy
      assistant = nil
    end
    assistant
  end

  def add_completion_test_page(assistant, text, visible, complete)
    page = Gtk::VBox.new(0, 0)
    check = Gtk::CheckButton.new("Complete")
    page.add(Gtk::Label.new(text))
    page.add(check)
    check.active = complete
    check.signal_connect('toggled') do
      complete = check.active?
      assistant.set_page_complete(page, complete)
    end
    page.show_all if visible
    assistant.append_page(page)
    assistant.set_page_title(page, text)
    assistant.set_page_complete(page, complete)
    page
  end

  def create_test_page(text)
    Gtk::Label.new(text)
  end

  def prepare_cb(assistant, page)
    if page.is_a?(Gtk::Label)
      puts "prepare: #{page.text}"
    elsif assistant.get_page_type(page) == Gtk::Assistant::PAGE_PROGRESS
      progress = page.child
      assistant.set_page_complete(page, false)
      progress.fraction = 0.0
      Gtk.timeout_add(300) do
        page = assistant.get_nth_page(assistant.current_page)
        progress = page.child
        value = progress.fraction = progress.fraction + 0.1
        continue = value < 1.0
        assistant.set_page_complete(page, true) unless continue
        continue
      end
    else
      puts "prepare: #{assistant.current_page}"
    end
  end

  def create_simple_assistant
    assistant = Gtk::Assistant.new
    assistant.set_default_size(400, 300)
    assistant.signal_connect('cancel') do
      puts "cancel"
      assistant.hide
    end
    assistant.signal_connect('close') do
      puts "close"
      assistant.hide
    end
    assistant.signal_connect('apply') do
      puts "apply"
    end
    assistant.signal_connect('prepare') do |_assistant, page|
      prepare_cb(_assistant, page)
    end

    page = create_test_page("Page 1")
    page.show
    assistant.append_page(page)
    assistant.set_page_title(page, "Page 1")
    assistant.set_page_complete(page, true)

    page = create_test_page("Page 2")
    page.show
    assistant.append_page(page)
    assistant.set_page_title(page, "Page 2")
    assistant.set_page_type(page, :confirm)
    assistant.set_page_complete(page, true)
  end

  def create_generous_assistant
    assistant = Gtk::Assistant.new
    assistant.set_default_size(400, 300)
    assistant.signal_connect('cancel') do
      puts "cancel"
      assistant.hide
    end
    assistant.signal_connect('close') do
      puts "close"
      assistant.hide
    end
    assistant.signal_connect('apply') do
      puts "apply"
    end
    assistant.signal_connect('prepare') do|_assistant, page|
      prepare_cb(_assistant, page)
    end

    page = create_test_page("Introduction")
    page.show
    assistant.append_page(page)
    assistant.set_page_title(page, "Introduction")
    assistant.set_page_type(page, :intro)
    assistant.set_page_complete(page, true)

    page = add_completion_test_page(assistant, "Content", true, false)
    next_page = add_completion_test_page(assistant, "More Content", true, true)

    check = Gtk::CheckButton.new("Next page visible");
    check.active = true
    check.signal_connect('toggled') do
      puts "beuh"
      next_page.visible = check.active?
    end
    check.show
    page.add(check)

    add_completion_test_page(assistant, "Even More Content", true, true)

    page = create_test_page("Confirmation")
    page.show
    assistant.append_page(page)
    assistant.set_page_title(page, "Confirmation")
    assistant.set_page_type(page, :confirm)
    assistant.set_page_complete(page, true)

    page = Gtk::Alignment.new(0.5, 0.5, 0.9, 0.0)
    page.add(Gtk::ProgressBar.new)
    page.show_all
    assistant.append_page(page)
    assistant.set_page_title(page, "Progress")
    assistant.set_page_type(page, :progress)
    assistant.set_page_complete(page, true)

    page = create_test_page("Summary")
    page.show
    assistant.append_page(page)
    assistant.set_page_title(page, "Summary")
    assistant.set_page_type(page, :summary)
    assistant.set_page_complete(page, true)
  end

  def create_nonlinear_assistant
    assistant = Gtk::Assistant.new
    assistant.set_default_size(400, 300)
    assistant.signal_connect('cancel') do
      puts "cancel"
      assistant.hide
    end
    assistant.signal_connect('close') do
      puts "close"
      assistant.hide
    end
    assistant.signal_connect('apply') do
      puts "apply"
    end
    assistant.signal_connect('prepare') do |_assistant, page|
      prepare_cb(_assistant, page)
    end

    assistant.set_forward_page_func do |current_page|
      retval = -1
      if current_page == 0
        if @selected_branch == 'A'
          retval = 1
        else
          retval = 2
        end
      elsif current_page == 1 || current_page == 2
        retval = 3
      end
      retval
    end

    page = Gtk::VBox.new(false, 6)
    button = Gtk::RadioButton.new('branch A')
    page.pack_start(button, false, false, 0)
    button.signal_connect('toggled') do
      @selected_branch = 'A'
    end
    button.active = true
    button = Gtk::RadioButton.new(button, 'branch B')
    page.pack_start(button, false, false, 0)
    button.signal_connect('toggled') do
      @selected_branch = 'B'
    end
    page.show_all
    assistant.append_page(page)
    assistant.set_page_title(page, "Page 1")
    assistant.set_page_complete(page, true)

    page = create_test_page("Page 2A")
    page.show
    assistant.append_page(page)
    assistant.set_page_title(page, "Page 2A")
    assistant.set_page_complete(page, true)

    page = create_test_page("Page 2B")
    page.show
    assistant.append_page(page)
    assistant.set_page_title(page, "Page 2B")
    assistant.set_page_complete(page, true)

    page = create_test_page("Confirmation")
    page.show
    assistant.append_page(page)
    assistant.set_page_title(page, "Confirmation")
    assistant.set_page_type(page, :confirm)
    assistant.set_page_complete(page, true)
  end

  def create_full_featured_assistant
    assistant = Gtk::Assistant.new
    assistant.set_default_size(400, 300)
    assistant.signal_connect('cancel') do
      puts "cancel"
      assistant.hide
    end
    assistant.signal_connect('close') do
      puts "close"
      assistant.hide
    end
    assistant.signal_connect('apply') do
      puts "apply"
    end
    assistant.signal_connect('prepare') do |_assistant, page|
      prepare_cb(_assistant, page)
    end

    button = Gtk::Button.new(Gtk::Stock::STOP)
    button.show
    assistant.add_action_widget(button)

    page = create_test_page("Page 1")
    page.show
    assistant.append_page(page)
    assistant.set_page_title(page, "Page 1")
    assistant.set_page_complete(page, true)

    #- set a side image
    pixbuf = page.render_icon(Gtk::Stock::DIALOG_WARNING, :dialog)
    assistant.set_page_side_image(page, pixbuf)

    #- set a header image
    pixbuf = page.render_icon(Gtk::Stock::DIALOG_INFO, :dialog)
    assistant.set_page_header_image(page, pixbuf)

    page = create_test_page("Invisible page")
    assistant.append_page(page)

    page = create_test_page("Page 3")
    page.show
    assistant.append_page(page)
    assistant.set_page_title(page, "Page 3")
    assistant.set_page_type(page, :confirm)
    assistant.set_page_complete(page, true)

    #- set a header image
    pixbuf = page.render_icon(Gtk::Stock::DIALOG_INFO, :dialog)
    assistant.set_page_header_image(page, pixbuf)

    assistant
  end
end

runner = AssistantRunner.new
buttons = [
    [ 'simple assistant', proc { runner.run_simple_assistant } ],
    [ 'generous assistant', proc { runner.run_generous_assistant } ],
    [ 'nonlinear assistant', proc { runner.run_nonlinear_assistant } ],
    [ 'full featured assistant', proc { runner.run_full_featured_assistant } ],
]

if ENV['RTL']
  Gtk::Widget.default_direction = Gtk::Widget::TEXT_DIR_RTL
end

window = Gtk::Window.new(Gtk::Window::TOPLEVEL)
window.signal_connect('destroy') { Gtk.main_quit }
window.signal_connect('delete-event') { false }

box = Gtk::VBox.new(false, 6)
window.add(box)

buttons.each do |label, callback|
  button = Gtk::Button.new(label)
  button.signal_connect('clicked') do
    callback.call
  end
  box.pack_start(button, true, true, 0)
end
window.show_all
Gtk.main
