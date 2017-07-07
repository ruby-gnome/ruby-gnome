# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Theming/Multiple Backgrounds

 Gtk themes are written using CSS. Every widget is build of multiple items
 that you can style very similarly to a regular website.
=end
class CssMultiplebgsDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Mutiple Backgrounds"
    @window.transient_for = main_window
    @window.set_default_size(400, 300)

    container = Gtk::Overlay.new
    container.add_events([:enter_notify_mask, :leave_notify_mask,
                          :pointer_motion_mask])
    @window.add(container)

    da = initialize_drawing_area
    container.add(da)

    button = initialize_bricks_button
    container.add_overlay(button)

    paned = Gtk::Paned.new(:vertical)
    container.add_overlay(paned)
    css = "/css_multiplebgs/css_multiplebgs.css"
    @default_css = Gio::Resources.lookup_data(css)
    # Need a filler so we get a handle
    child = Gtk::Box.new(:vertical, 0)
    paned.add(child)

    initialize_text_buffer
    container = Gtk::ScrolledWindow.new
    paned.add(container)

    child = Gtk::TextView.new(@text)
    container.add(child)

    initialize_provider
    apply_style(@window, @provider)
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

  def apply_style(widget, provider)
    style_context = widget.style_context
    style_context.add_provider(provider, Gtk::StyleProvider::PRIORITY_USER)
    return unless widget.respond_to?(:children)
    widget.children.each do |child|
      apply_style(child, provider)
    end
  end

  def initialize_drawing_area
    da = Gtk::DrawingArea.new
    da.name = "canvas"
    da.signal_connect "draw" do |widget, cr|
      context = widget.style_context
      Gtk.render_background(context, cr, 0, 0, widget.allocated_width,
                            widget.allocated_height)
      Gtk.render_frame(context, cr, 0, 0, widget.allocated_width,
                       widget.allocated_height)
      false
    end
    da
  end

  def initialize_bricks_button
    button = Gtk::Button.new
    button.add_events([:enter_notify_mask, :leave_notify_mask,
                       :pointer_motion_mask])
    button.name = "bricks-button"
    button.halign = :center
    button.valign = :center
    button.set_size_request(250, 84)
    button
  end

  def initialize_text_buffer
    @text = Gtk::TextBuffer.new
    @text.create_tag("warning", "underline" => :single)
    @text.create_tag("error", "underline" => :error)
    @text.text = @default_css
    text_buffer_signal_connect_changed
  end

  def text_buffer_signal_connect_changed
    @text.signal_connect "changed" do |buffer|
      buffer.remove_all_tags(buffer.start_iter, buffer.end_iter)
      modified_text = buffer.get_text(buffer.start_iter,
                                      buffer.end_iter,
                                      false)
      begin
        @provider.load_from_data(modified_text)
      rescue
        @provider.load_from_data(@default_css)
      end

      Gtk::StyleContext.reset_widgets
    end
  end

  def initialize_provider
    @provider = Gtk::CssProvider.new
    @provider.load_from_data(@default_css)
    provider_signal_connect_parsing_error
  end

  def provider_signal_connect_parsing_error
    @provider.signal_connect "parsing-error" do |_css_provider, section, error|
      start_i = @text.get_iter_at(:line => section.start_line,
                                  :index => section.start_position)
      end_i = @text.get_iter_at(:line => section.end_line,
                                :index => section.end_position)
      tag = error == Gtk::CssProviderError::DEPRECATED ? "warning" : "error"
      @text.apply_tag_by_name(tag, start_i, end_i)
    end
  end
end
