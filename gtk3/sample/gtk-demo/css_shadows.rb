# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  CSS Theming/Shadows

 This demo shows how to use CSS shadows.
=end
module CssShadowsDemo
  def self.run_demo(main_window)
    window = Gtk::Window.new(:toplevel)
    window.set_title("Shadows")
    window.set_transient_for(main_window)
    window.set_default_size(400, 300)

    paned = Gtk::Paned.new(:vertical)
    window.add(paned)

    child = create_toolbar
    paned.add(child)

    text = Gtk::TextBuffer.new
    text.create_tag("warning", "underline" => :single)
    text.create_tag("error", "underline" => :error)
    default_css = Gio::Resources.lookup_data("/css_shadows/gtk.css")
    text.text = default_css

    provider = Gtk::CssProvider.new
    provider.load_from_data(default_css)

    container = Gtk::ScrolledWindow.new
    paned.add(container)

    child = Gtk::TextView.new(text)
    container.add(child)

    text.signal_connect "changed" do |buffer|
      buffer.remove_all_tags(buffer.start_iter, buffer.end_iter)
      modified_text = buffer.get_text(buffer.start_iter,
                                      buffer.end_iter,
                                      false)
      begin
        provider.load_from_data(modified_text)
      rescue
        provider.load_from_data(default_css)
      end

      Gtk::StyleContext.reset_widgets
    end

    provider.signal_connect "parsing-error" do |_css_provider, section, error|
      start_i = text.get_iter_at(:line => section.start_line,
                                 :index => section.start_position)
      end_i =  text.get_iter_at(:line => section.end_line,
                                :index => section.end_position)
      tag_name = nil
      if error == Gtk::CssProviderError::DEPRECATED
        tag_name = "warning"
      else
        tag_name = "error"
      end
      text.apply_tag_by_name(tag_name, start_i, end_i)
    end
    apply_style(window, provider)

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end

  def self.create_toolbar
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

  def self.apply_style(widget, provider)
    style_context = widget.style_context
    style_context.add_provider(provider, Gtk::StyleProvider::PRIORITY_USER)
    return unless widget.respond_to?(:children)
    widget.children.each do |child|
      apply_style(child, provider)
    end
  end
end
