# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Theming/Shadows

 This demo shows how to use CSS shadows.
=end
class CssShadowsDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.title = "Shadows"
    @window.transient_for = main_window
    @window.set_default_size(400, 300)

    paned = Gtk::Paned.new(:vertical)
    @window.add(paned)

    child = create_toolbar
    paned.add(child)

    @default_css = Gio::Resources.lookup_data("/css_shadows/gtk.css")
    initialize_text_buffer
    initialize_provider

    container = Gtk::ScrolledWindow.new
    paned.add(container)

    child = Gtk::TextView.new(@text)
    container.add(child)

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

  def create_toolbar
    toolbar = Gtk::Toolbar.new
    toolbar.set_valign(:center)

    item = Gtk::ToolButton.new
    item.set_icon_name("go-next")
    toolbar.insert(item, -1)

    item = Gtk::ToolButton.new
    item.set_icon_name("go-previous")
    toolbar.insert(item, -1)

    item = Gtk::ToolButton.new(:label => "Hello World")
    item.set_is_important(true)
    toolbar.insert(item, -1)

    toolbar
  end

  def apply_style(widget, provider)
    style_context = widget.style_context
    style_context.add_provider(provider, Gtk::StyleProvider::PRIORITY_USER)
    return unless widget.respond_to?(:children)
    widget.children.each do |child|
      apply_style(child, provider)
    end
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
