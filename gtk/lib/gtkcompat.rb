=begin
Definitions for name compatibility with GTK+ original functions
=end

require 'gtk'

module Gdk
end

module Gtk
end

module GdkImlib
  class << Image
    alias_method(:load_image, :new)
    alias_method(:create_image_from_data, :create_from_data)
  end
  class Image
    alias_method(:add_image_to_eim, :add_to_eim)
    alias_method(:apply_image, :apply)
    alias_method(:changed_image, :changed)
    alias_method(:clone_image, :clone)
    alias_method(:clone_scaled_image, :clone_scaled)
    alias_method(:crop_and_clone_image, :crop_and_clone)
    alias_method(:crop_image, :crop)
    alias_method(:flip_image_horizontal, :flip_horizontal)
    alias_method(:flip_image_vertical, :flip_vertical)
    alias_method(:image_blue_curve, :blue_curve)
    alias_method(:image_blue_curve=, :blue_curve=)
    alias_method(:image_blue_modifier, :blue_modifier)
    alias_method(:image_blue_modifier=, :blue_modifier=)
    alias_method(:image_border, :border)
    alias_method(:image_border=, :border=)
    alias_method(:image_green_curve, :green_curve)
    alias_method(:image_green_curve=, :green_curve=)
    alias_method(:image_green_modifier, :green_modifier)
    alias_method(:image_green_modifier=, :green_modifier=)
    alias_method(:image_modifier, :modifier)
    alias_method(:image_modifier=, :modifier=)
    alias_method(:image_red_curve, :red_curve)
    alias_method(:image_red_curve=, :red_curve=)
    alias_method(:image_red_modifier, :red_modifier)
    alias_method(:image_red_modifier=, :red_modifier=)
    alias_method(:image_shape, :shape)
    alias_method(:image_shape=, :shape=)
    #alias_method(:kill_image, :image)
    alias_method(:paste_image, :paste)
    alias_method(:paste_image_border, :paste_border)
    alias_method(:rotate_image, :rotate)
    alias_method(:save_image, :save)
    alias_method(:save_image_to_eim, :save_to_eim)
    alias_method(:save_image_to_ppm, :save_to_ppm)
  end
end
