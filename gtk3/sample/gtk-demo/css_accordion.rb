# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= CSS Theming/CSS Accordion

A simple accordion demo written using CSS transitions and multiple backgrounds
=end
def apply_style(widget, provider)
  widget.children.each do |child|
    style_context = child.style_context
    style_context.add_provider(provider, Gtk::StyleProvider::PRIORITY_USER)
    apply_style(child, provider) if child.respond_to?(:children)
  end
end

module CssAccordionDemo
  def self.run_demo(main_window)
    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen

    window.set_title("CSS Accordion")
    window.set_default_size(600, 300)

    container = Gtk::Box.new(:horizontal, 0)
    container.set_halign(:center)
    container.set_valign(:center)

    window.add(container)

    %w(This Is A CSS Accordion :-).each do |label|
      child = Gtk::Button.new(:label => label)
      container.add(child)
    end

    provider = Gtk::CssProvider.new
    provider.load_from_resource("/css_accordion/css_accordion.css")

    style_context = window.style_context
    style_context.add_provider(provider, Gtk::StyleProvider::PRIORITY_USER)

    apply_style(window, provider)

    if !window.visible?
      window.show_all
    else
      window.destroy
    end

    window
  end
end
