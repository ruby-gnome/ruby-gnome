# Copyright (c) 2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Pango/Font Features

 This demonstrates support for OpenType font features with
 Pango attributes. The attributes can be used manually or
 via Pango markup.
=end
class FontFeaturesDemo
  def initialize(_main_window)
    initialize_builder
    initialize_widgets
    builder_add_callback_symbols

    update(@toggle, @entry, @font, @settings, @label)
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
    @window
  end

  private

  def initialize_builder
    @builder = Gtk::Builder.new(:resource => "/font_features/font-features.ui")
  end

  def initialize_widgets
    @window = @builder["window"]
    @label = @builder["label"]
    @settings = @builder["settings"]
    @resetbutton = @builder["reset"]
    @font = @builder["font"]
    @numcasedefault = @builder["numcasedefault"]
    @numspacedefault = @builder["numspacedefault"]
    @fractiondefault = @builder["fractiondefault"]
    @stack = @builder["stack"]
    @entry = @builder["entry"]
    @toggle = []

    %w(kern liga dlig hlig clig smcp c2sc lnum onum pnum tnum frac afrc zero
       nalt swsh calt hist salt ss01 ss02 ss03 ss04 ss05).each do |name|
      @toggle << @builder[name]
    end
    @text = nil
  end

  def builder_add_callback_symbols
    @builder.connect_signals do |name|
      case name
      when "reset"
        reset_callback
      when "update"
        update_callback
      when "switch_to_entry"
        switch_to_entry_callback
      when "switch_to_label"
        switch_to_label_callback
      when "entry_key_press"
        entry_key_press_callback
      end
    end
  end

  def reset_callback
    proc do
      @numcasedefault.active = true
      @numspacedefault.active = true
      @fractiondefault.active = true
      @toggle.each do |widget|
        if widget.class != Gtk::RadioButton
          widget.active = false
          widget.sensitive = false
        end
      end
    end
  end

  def update_callback
    proc do
      update(@toggle, @entry, @font, @settings, @label)
    end
  end

  def switch_to_entry_callback
    proc do
      @text = @entry.text
      @stack.visible_child_name = "entry"
    end
  end

  def switch_to_label_callback
    proc do
      @text = nil
      @stack.set_visible_child_name("label")
      update(@toggle, @entry, @font, @settings, @label)
    end
  end

  def entry_key_press_callback
    proc do |_entry, event|
      if event.keyval == Gdk::Keyval::KEY_Escape
        @entry.text = @text
        @text = nil
        @stack.set_visible_child_name("label")
        update(@toggle, @entry, @font, @settings, @label)
      end
      Gdk::Event::PROPAGATE
    end
  end

  def update(toggle, entry, font, settings, label)
    text = entry.text
    font_desc = font.font
    s = ""
    has_feature = false

    toggle.each do |widget|
      next unless widget.sensitive?

      if widget.class == Gtk::RadioButton
        if widget.active?
          s += ", " if has_feature
          s += widget.builder_name
          s += " 1"
          has_feature = true
        end
      else
        s += ", " if has_feature
        s += widget.builder_name
        s += widget.active? ? " 1" : " 0"
        has_feature = true
      end
    end
    font_settings = s
    settings.text = font_settings

    s = "<span font_desc='#{font_desc}' font_features='#{font_settings}'>#{text}</span>"
    label.markup = s
  end
end
