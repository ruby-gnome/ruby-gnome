# Copyright (c) 2017 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Theming/CSS Blend Modes

 You can blend multiple backgrounds using the CSS blend modes available.
=end

BlendMode = Struct.new(:name, :id)

class CssBlendmodesDemo
  def initialize(main_window)
    @builder = Gtk::Builder.new(:resource => "/css_blendmodes/blendmodes.ui")
    @window = @builder["window"]
    @window.transient_for = main_window

    # Setup the CSS provider for window
    @provider = Gtk::CssProvider.new
    Gtk::StyleContext.add_provider_for_screen(Gdk::Screen.default,
                                              @provider,
                                              Gtk::StyleProvider::PRIORITY_APPLICATION)

    initialize_blend_modes
    setup_listbox
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

  def initialize_blend_modes
    @blend_modes = []
    @blend_modes << BlendMode.new("Color", "color")
    @blend_modes << BlendMode.new("Color (burn)", "color-burn")
    @blend_modes << BlendMode.new("Color (dodge)", "color-dodge")
    @blend_modes << BlendMode.new("Darken", "darken")
    @blend_modes << BlendMode.new("Difference", "difference")
    @blend_modes << BlendMode.new("Exclusion", "exclusion")
    @blend_modes << BlendMode.new("Hard Light", "hard-light")
    @blend_modes << BlendMode.new("Hue", "hue")
    @blend_modes << BlendMode.new("Lighten", "lighten")
    @blend_modes << BlendMode.new("Luminosity", "luminosity")
    @blend_modes << BlendMode.new("Multiply", "multiply")
    @blend_modes << BlendMode.new("Normal", "normal")
    @blend_modes << BlendMode.new("Overlay", "overlay")
    @blend_modes << BlendMode.new("Saturate", "saturate")
    @blend_modes << BlendMode.new("Screen", "screen")
    @blend_modes << BlendMode.new("Soft Light", "soft-light")
  end

  def setup_listbox
    normal_row = nil
    listbox = Gtk::ListBox.new
    @builder["scrolledwindow"].add(listbox)

    listbox.signal_connect "row-activated" do |_widget, w_row, _provider|
      blend_mode = @blend_modes[w_row.index].id
      update_css_for_blend_mode(blend_mode)
    end

    @blend_modes.each do |blend_mode|
      row = Gtk::ListBoxRow.new
      label = Gtk::Label.new(blend_mode.name)
      label.xalign = 0
      row.add(label)
      listbox.add(row)
      normal_row = row if blend_mode.id == "normal"
    end

    # select the "normal" row
    listbox.select_row(normal_row)
    normal_row.signal_emit("activate")
    normal_row.grab_focus
  end

  def update_css_for_blend_mode(blend_mode)
    raw_css = Gio::Resources.lookup_data("/css_blendmodes/css_blendmodes.css", 0)
    css = format(raw_css, blend_mode, blend_mode, blend_mode)
    @provider.load_from_data(css)
  end
end
