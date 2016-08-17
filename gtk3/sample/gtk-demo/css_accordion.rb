# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Theming/CSS Accordion

A simple accordion demo written using CSS transitions and multiple backgrounds
=end
class CssAccordionDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "CSS Accordion"
    @window.set_default_size(600, 300)

    container = Gtk::Box.new(:horizontal, 0)
    container.halign = :center
    container.valign = :center

    @window.add(container)

    %w(This Is A CSS Accordion :-).each do |label|
      child = Gtk::Button.new(:label => label)
      container.add(child)
    end

    provider = Gtk::CssProvider.new
    provider.load_from_resource("/css_accordion/css_accordion.css")

    style_context = @window.style_context
    style_context.add_provider(provider, Gtk::StyleProvider::PRIORITY_USER)

    apply_style(@window, provider)
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end

    @window
  end

  def apply_style(widget, provider)
    style_context = widget.style_context
    style_context.add_provider(provider, Gtk::StyleProvider::PRIORITY_USER)
    return unless widget.respond_to?(:children)
    widget.children.each do |child|
      apply_style(child, provider)
    end
  end
end
