#!/usr/bin/env ruby
=begin
  test-assistant.rb - Ruby/GTK version of testassistant.c from gtk+ 2.10 sources.

  Guillaume Cottenceau for the ruby-gnome2 project.

  Copyright (c) 2005,2006  Ruby-GNOME2 Project Team 
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: assistant.rb,v 1.1 2006/11/23 08:39:12 mutoh Exp $
=end

require 'gtk2'

if str = Gtk.check_version(2, 10, 0)
    puts "This sample requires GTK+ 2.10.0 or later"
    puts str
    exit
end

def get_test_page(text)
    return Gtk::Label.new(text)
end

def add_completion_test_page(assistant, text, visible, complete)
    page = Gtk::VBox.new(0, 0)
    check = Gtk::CheckButton.new("Complete")
    page.add(Gtk::Label.new(text))
    page.add(check)
    check.active = complete
    check.signal_connect('toggled') {
        complete = check.active?
        assistant.set_page_complete(page, complete)
    }
    if visible
        page.show_all
    end
    assistant.append_page(page)
    assistant.set_page_title(page, text)
    assistant.set_page_complete(page, complete)
    return page
end

def prepare_cb(assistant, page)
    if page.is_a?(Gtk::Label)
        puts "prepare: #{page.text}"
    elsif assistant.get_page_type(page) == Gtk::Assistant::PAGE_PROGRESS
        progress = page.child
        assistant.set_page_complete(page, false)
        progress.fraction = 0.0
        Gtk.timeout_add(300) {
            retval = true
            page = assistant.get_nth_page(assistant.current_page)
            progress = page.child
            value = progress.fraction = progress.fraction + 0.1
            if value >= 1.0
                assistant.set_page_complete(page, true)
                retval = false
            end
            retval
        }
    else
        puts "prepare: #{assistant.current_page}"
    end
end

$simple_assistant = nil
def create_simple_assistant()
    if $simple_assistant.nil?
        $simple_assistant = Gtk::Assistant.new
        $simple_assistant.set_default_size(400, 300)
        $simple_assistant.signal_connect('cancel') {
            puts "cancel"
            $simple_assistant.hide
        }
        $simple_assistant.signal_connect('close') {
            puts "close"
            $simple_assistant.hide
        }
        $simple_assistant.signal_connect('apply') {
            puts "apply"
        }
        $simple_assistant.signal_connect('prepare') { |assistant, page|
            prepare_cb(assistant, page)
        }

        page = get_test_page("Page 1")
        page.show
        $simple_assistant.append_page(page)
        $simple_assistant.set_page_title(page, "Page 1")
        $simple_assistant.set_page_complete(page, true)

        page = get_test_page("Page 2")
        page.show
        $simple_assistant.append_page(page)
        $simple_assistant.set_page_title(page, "Page 2")
        $simple_assistant.set_page_type(page, Gtk::Assistant::PAGE_CONFIRM)
        $simple_assistant.set_page_complete(page, true)
    end

    if ! $simple_assistant.visible?
        $simple_assistant.show
    else
        $simple_assistant.destroy
        $simple_assistant = nil
    end
end

$generous_assistant = nil
def create_generous_assistant()
    if $generous_assistant.nil?
        $generous_assistant = Gtk::Assistant.new
        $generous_assistant.set_default_size(400, 300)
        $generous_assistant.signal_connect('cancel') {
            puts "cancel"
            $generous_assistant.hide
        }
        $generous_assistant.signal_connect('close') {
            puts "close"
            $generous_assistant.hide
        }
        $generous_assistant.signal_connect('apply') {
            puts "apply"
        }
        $generous_assistant.signal_connect('prepare') { |assistant, page|
            prepare_cb(assistant, page)
        }

        page = get_test_page("Introduction")
        page.show
        $generous_assistant.append_page(page)
        $generous_assistant.set_page_title(page, "Introduction")
        $generous_assistant.set_page_type(page, Gtk::Assistant::PAGE_INTRO)
        $generous_assistant.set_page_complete(page, true)

        page = add_completion_test_page($generous_assistant, "Content", true, false)
        next_ = add_completion_test_page($generous_assistant, "More Content", true, true)

        check = Gtk::CheckButton.new("Next page visible");
        check.active = true
        check.signal_connect('toggled') {
            puts "beuh"
            next_.visible = check.active?
        }
        check.show
        page.add(check)

        add_completion_test_page($generous_assistant, "Even More Content", true, true)

        page = get_test_page("Confirmation")
        page.show
        $generous_assistant.append_page(page)
        $generous_assistant.set_page_title(page, "Confirmation")
        $generous_assistant.set_page_type(page, Gtk::Assistant::PAGE_CONFIRM)
        $generous_assistant.set_page_complete(page, true)

        page = Gtk::Alignment.new(0.5, 0.5, 0.9, 0.0)
        page.add(Gtk::ProgressBar.new)
        page.show_all
        $generous_assistant.append_page(page)
        $generous_assistant.set_page_title(page, "Progress")
        $generous_assistant.set_page_type(page, Gtk::Assistant::PAGE_PROGRESS)
        $generous_assistant.set_page_complete(page, true)

        page = get_test_page("Summary")
        page.show
        $generous_assistant.append_page(page)
        $generous_assistant.set_page_title(page, "Summary")
        $generous_assistant.set_page_type(page, Gtk::Assistant::PAGE_SUMMARY)
        $generous_assistant.set_page_complete(page, true)
    end

    if ! $generous_assistant.visible?
        $generous_assistant.show
    else
        $generous_assistant.destroy
        $generous_assistant = nil
    end
end

$selected_branch = 'A'
$nonlinear_assistant = nil
def create_nonlinear_assistant()
    if $nonlinear_assistant.nil?
        $nonlinear_assistant = Gtk::Assistant.new
        $nonlinear_assistant.set_default_size(400, 300)
        $nonlinear_assistant.signal_connect('cancel') {
            puts "cancel"
            $nonlinear_assistant.hide
        }
        $nonlinear_assistant.signal_connect('close') {
            puts "close"
            $nonlinear_assistant.hide
        }
        $nonlinear_assistant.signal_connect('apply') {
            puts "apply"
        }
        $nonlinear_assistant.signal_connect('prepare') { |assistant, page|
            prepare_cb(assistant, page)
        }

        $nonlinear_assistant.set_forward_page_func { |current_page|
            retval = -1
            if current_page == 0
                if $selected_branch == 'A'
                    retval = 1
                else
                    retval = 2
                end
            elsif current_page == 1 || current_page == 2
                retval = 3
            end
            retval
        }

        page = Gtk::VBox.new(false, 6)
        button = Gtk::RadioButton.new('branch A')
        page.pack_start(button, false, false, 0)
        button.signal_connect('toggled') {
            $selected_branch = 'A'
        }
        button.active = true
        button = Gtk::RadioButton.new(button, 'branch B')
        page.pack_start(button, false, false, 0)
        button.signal_connect('toggled') {
            $selected_branch = 'B'
        }
        page.show_all
        $nonlinear_assistant.append_page(page)
        $nonlinear_assistant.set_page_title(page, "Page 1")
        $nonlinear_assistant.set_page_complete(page, true)

        page = get_test_page("Page 2A")
        page.show
        $nonlinear_assistant.append_page(page)
        $nonlinear_assistant.set_page_title(page, "Page 2A")
        $nonlinear_assistant.set_page_complete(page, true)

        page = get_test_page("Page 2B")
        page.show
        $nonlinear_assistant.append_page(page)
        $nonlinear_assistant.set_page_title(page, "Page 2B")
        $nonlinear_assistant.set_page_complete(page, true)

        page = get_test_page("Confirmation")
        page.show
        $nonlinear_assistant.append_page(page)
        $nonlinear_assistant.set_page_title(page, "Confirmation")
        $nonlinear_assistant.set_page_type(page, Gtk::Assistant::PAGE_CONFIRM)
        $nonlinear_assistant.set_page_complete(page, true)
    end

    if ! $nonlinear_assistant.visible?
        $nonlinear_assistant.show
    else
        $nonlinear_assistant.destroy
        $nonlinear_assistant = nil
    end
end


$full_featured_assistant = nil
def create_full_featured_assistant()
    if $full_featured_assistant.nil?
        $full_featured_assistant = Gtk::Assistant.new
        $full_featured_assistant.set_default_size(400, 300)
        $full_featured_assistant.signal_connect('cancel') {
            puts "cancel"
            $full_featured_assistant.hide
        }
        $full_featured_assistant.signal_connect('close') {
            puts "close"
            $full_featured_assistant.hide
        }
        $full_featured_assistant.signal_connect('apply') {
            puts "apply"
        }
        $full_featured_assistant.signal_connect('prepare') { |assistant, page|
            prepare_cb(assistant, page)
        }

        button = Gtk::Button.new(Gtk::Stock::STOP)
        button.show
        $full_featured_assistant.add_action_widget(button)

        page = get_test_page("Page 1")
        page.show
        $full_featured_assistant.append_page(page)
        $full_featured_assistant.set_page_title(page, "Page 1")
        $full_featured_assistant.set_page_complete(page, true)

        #- set a side image
        pixbuf = page.render_icon(Gtk::Stock::DIALOG_WARNING, Gtk::IconSize::DIALOG)
        $full_featured_assistant.set_page_side_image(page, pixbuf)
        
        #- set a header image
        pixbuf = page.render_icon(Gtk::Stock::DIALOG_INFO, Gtk::IconSize::DIALOG)
        $full_featured_assistant.set_page_header_image(page, pixbuf)

        page = get_test_page("Invisible page")
        $full_featured_assistant.append_page(page)

        page = get_test_page("Page 3")
        page.show
        $full_featured_assistant.append_page(page)
        $full_featured_assistant.set_page_title(page, "Page 3")
        $full_featured_assistant.set_page_type(page, Gtk::Assistant::PAGE_CONFIRM)
        $full_featured_assistant.set_page_complete(page, true)

        #- set a header image
        pixbuf = page.render_icon(Gtk::Stock::DIALOG_INFO, Gtk::IconSize::DIALOG)
        $full_featured_assistant.set_page_header_image(page, pixbuf)
    end

    if ! $full_featured_assistant.visible?
        $full_featured_assistant.show
    else
        $full_featured_assistant.destroy
        $full_featured_assistant = nil
    end
end

buttons = [
    [ 'simple assistant', proc { create_simple_assistant } ],
    [ 'generous assistant', proc { create_generous_assistant } ],
    [ 'nonlinear assistant', proc { create_nonlinear_assistant } ],
    [ 'full featured assistant', proc { create_full_featured_assistant } ],
]

if ENV['RTL']
    Gtk::Widget.default_direction = Gtk::Widget::TEXT_DIR_RTL
end

window = Gtk::Window.new(Gtk::Window::TOPLEVEL)
window.signal_connect('destroy') { Gtk.main_quit }
window.signal_connect('delete-event') { false }

box = Gtk::VBox.new(false, 6)
window.add(box)

buttons.each { |elem|
    button = Gtk::Button.new(elem[0])
    button.signal_connect('clicked') {
        elem[1].call
    }
    box.pack_start(button, true, true, 0)
}
window.show_all
Gtk.main
