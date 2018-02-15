module Gtk
  extend GLib::Deprecatable
  define_deprecated_const :Allocation, "Gdk::Rectangle"

  define_deprecated_const :Combo,                  :raise => "Use 'Gtk::ComboBoxText' instead."
  define_deprecated_const :ComboBoxEntry,          :raise => "Use 'Gtk::ComboBox' instead."
  define_deprecated_const :Curve,                  :raise => "Don't use this widget anymore."
  define_deprecated_const :FileSelection,          :raise => "Use 'Gtk::FileChooserDialog' instead."
  define_deprecated_const :FontSelection,          :raise => "Use 'Gtk::FontChooserWidget' instead."
  define_deprecated_const :FontSelectionDialog,    :raise => "Use 'Gtk::FontChooserDialog' instead."
  remove_const(:ColorSelection)
  define_deprecated_const :ColorSelection,         :raise => "Use 'Gtk::ColorChooserWidget' instead."
  remove_const(:ColorSelectionDialog)
  define_deprecated_const :ColorSelectionDialog,   :raise => "Use 'Gtk::ColorChooserDialog' instead."
  define_deprecated_const :GammaCurve,             :raise => "Don't use this widget anymore."
  define_deprecated_const :HRuler,                 :raise => "Don't use this widget anymore."
  define_deprecated_const :InputDialog,            :raise => "Don't use this widget anymore."
  define_deprecated_const :ItemFactory,            :raise => "Use 'Gtk::UIManager' instead."
  define_deprecated_const :OptionMenu,             :raise => "Use 'Gtk::ComboBox' instead."
  define_deprecated_const :PageSetupUnixDialog,    :raise => "Use 'Gtk::PrintOperation' instead."
  define_deprecated_const :Printer,                :raise => "Use 'Gtk::PrintOperation' instead."
  define_deprecated_const :PrintCapabilities,      :raise => "Use 'Gtk::PrintOperation' instead."
  define_deprecated_const :PrintJob,               :raise => "Use 'Gtk::PrintOperation' instead."
  define_deprecated_const :PrintUnixDialog,        :raise => "Use 'Gtk::PrintOperation' instead."
  define_deprecated_const :RC,                     :raise => "Use 'Gtk::StyleContext' instead."
  define_deprecated_const :RcStyle,                :raise => "Use 'Gtk::CssProvider' instead."
  define_deprecated_const :Ruler,                  :raise => "Don't use this widget anymore."
  define_deprecated_const :Style,                  :raise => "Use 'Gtk::StyleContext' instead."
  define_deprecated_const :Tooltips,               :raise => "Use 'Gtk::Tooltip' API."
  define_deprecated_const :VRuler,                 :raise => "Don't use this widget anymore."
  define_deprecated_flags :AccelFlags, 'ACCEL'
  define_deprecated_flags :AttachOptions
  define_deprecated_enums :CornerType, 'CORNER'
  define_deprecated_enums :DeleteType, 'DELETE'
  define_deprecated_enums :DirectionType, 'DIR'
  define_deprecated_enums :ExpanderStyle, 'EXPANDER'
  define_deprecated_enums :Justification, 'JUSTIFY'
  define_deprecated_enums :MovementStep, 'MOVEMENT'
  define_deprecated_enums :Orientation, 'ORIENTATION'
  define_deprecated_enums :PackType, 'PACK'
  define_deprecated_enums :PathPriorityType, 'PATH_PRIO'
  define_deprecated_enums :PathType, 'PATH'
  define_deprecated_enums :PolicyType, 'POLICY'
  define_deprecated_enums :PositionType, 'POS'
  define_deprecated_enums :ReliefStyle, 'RELIEF'
  define_deprecated_enums :ResizeMode, 'RESIZE'
  define_deprecated_enums :ScrollStep, 'SCROLL'
  define_deprecated_enums :ScrollType, 'SCROLL'
  define_deprecated_enums :SelectionMode, 'SELECTION'
  define_deprecated_enums :ShadowType, 'SHADOW'
  define_deprecated_enums :StateType, 'STATE'
  define_deprecated_enums :SortType, 'SORT'
  define_deprecated_const :AnchorType
  define_deprecated_const :ANCHOR_CENTER
  define_deprecated_const :ANCHOR_NORTH
  define_deprecated_const :ANCHOR_NORTH_WEST
  define_deprecated_const :ANCHOR_NORTH_EAST
  define_deprecated_const :ANCHOR_SOUTH
  define_deprecated_const :ANCHOR_SOUTH_WEST
  define_deprecated_const :ANCHOR_SOUTH_EAST
  define_deprecated_const :ANCHOR_WEST
  define_deprecated_const :ANCHOR_EAST
  define_deprecated_const :ANCHOR_N
  define_deprecated_const :ANCHOR_NW
  define_deprecated_const :ANCHOR_NE
  define_deprecated_const :ANCHOR_S
  define_deprecated_const :ANCHOR_SW
  define_deprecated_const :ANCHOR_SE
  define_deprecated_const :ANCHOR_W
  define_deprecated_const :ANCHOR_E
  define_deprecated_const :UpdateType
  define_deprecated_const :UPDATE_CONTINUOUS
  define_deprecated_const :UPDATE_DISCONTINUOUS
  define_deprecated_const :UPDATE_DELAYED
  define_deprecated_singleton_method :init_add, :warn => "Don't use this method."
  define_deprecated_singleton_method :quit_add, :warn => "Don't use this method."
  define_deprecated_singleton_method :quit_remove, :warn => "Don't use this method."
  define_deprecated_singleton_method :get_event_widget, :warn => "Use 'Gdk::Event#widget'." do |_self, event|
    event && event.widget
  end
  define_deprecated_singleton_method :timeout_add, :warn => "Use 'GLib::Timeout.add'." do |_self, interval, &block|
    GLib::Timeout.add(interval, &block)
  end
  define_deprecated_singleton_method :timeout_remove, :warn => "Use 'GLib::Source.remove'." do |_self, id|
    GLib::Source.remove(id)
  end
  define_deprecated_singleton_method :idle_add, :warn => "Use 'GLib::Idle.add'." do |_self, &block|
    GLib::Idle.add(&block)
  end
  define_deprecated_singleton_method :idle_add_priority, :warn => "Use 'GLib::Idle.add'." do |_self, priority, &block|
    GLib::Idle.add(priority, &block)
  end
  define_deprecated_singleton_method :idle_remove, :warn => "Use 'GLib::Source.remove'." do |_self, id|
    GLib::Source.remove(id)
  end
  define_deprecated_singleton_method :grab_add, :warn => "Use 'Gtk::Widget#grab_add'." do |_self, widget|
    widget.grab_add
  end
  define_deprecated_singleton_method :grab_remove, :warn => "Use 'Gtk::Widget#grab_remove'." do |_self, widget|
    widget.grab_remove
  end

  class AboutDialog
    extend GLib::Deprecatable
    define_deprecated_singleton_method :set_email_hook, :warn => "Use 'activate-link' signal."
    define_deprecated_singleton_method :set_url_hook, :warn => "Use 'activate-link' signal."
  end

  class Action
    extend GLib::Deprecatable
    define_deprecated_method :connect_proxy, :warn => "Use 'Gtk::Activatable#set_related_action'."
    define_deprecated_method :disconnect_proxy, :warn => "Use 'Gtk::Activatable#set_related_action'."
    define_deprecated_method :block_activate_from, :warn => "Don't use this method."
    define_deprecated_method :unblock_activate_from, :warn => "Don't use this method."
    define_deprecated_method_by_hash_args :initialize,
        'name, label, tooltip = nil, stock_id = nil',
        'name, :label => nil, :tooltip => nil, :stock_id => nil', 1 do
        |_self, name, label, tooltip, stock_id|
      [name, {:label => label, :tooltip => tooltip, :stock_id => stock_id}]
    end
  end

  class Alignment
    extend GLib::Deprecatable
    define_deprecated_const :Align, "Gtk::Align"
  end

  class Arrow
    extend GLib::Deprecatable
    define_deprecated_enums :Type
  end

  class Assistant
    extend GLib::Deprecatable
    define_deprecated_const :PageType, "Gtk::AssistantPageType"
    define_deprecated_enums :AssistantPageType, 'PAGE'
    define_deprecated_method :set_page_header_image, :warn => "Don't use this method."
    define_deprecated_method :get_page_header_image, :warn => "Don't use this method."
    define_deprecated_method :set_page_side_image, :warn => "Don't use this method."
    define_deprecated_method :get_page_side_image, :warn => "Don't use this method."
  end

  class BindingSet
    extend GLib::Deprecatable
    define_deprecated_method :entry_clear, :remove
    define_deprecated_method :entry_skip, :skip
    define_deprecated_method :entry_add_signal, :add_signal
    define_deprecated_method :add_path, :raise => "Don't use this method."
  end

  class Box
    extend GLib::Deprecatable
    define_deprecated_method :pack_start_defaults, :pack_start
    define_deprecated_method :pack_end_defaults, :pack_end
    define_deprecated_method_by_hash_args :pack_start,
        'child, expand = true, fill = true, padding = 0',
        'child, :expand => true, :fill => true, :padding => 0', 1 do
        |_self, child, expand, fill, padding|
      [child, {:expand => expand, :fill => fill, :padding => padding}]
    end
    define_deprecated_method_by_hash_args :pack_end,
        'child, expand = true, fill = true, padding = 0',
        'child, :expand => true, :fill => true, :padding => 0', 1 do
        |_self, child, expand, fill, padding|
      [child, {:expand => expand, :fill => fill, :padding => padding}]
    end
    define_deprecated_method_by_hash_args :set_child_packing,
        'child, expand, fill, padding, pack_type',
        'child, :expand => nil, :fill => nil, :padding => nil, :pack_type => nil', 1 do
        |_self, child, expand, fill, padding, pack_type|
      [child, {:expand => expand, :fill => fill, :padding => padding, :pack_type => pack_type}]
    end
  end

  class Button
    extend GLib::Deprecatable
    define_deprecated_method :enter, :warn => "Don't use this method."
    define_deprecated_method :leave, :warn => "Don't use this method."
    define_deprecated_method :pressed, :warn => "Don't use this method."
    define_deprecated_method :released, :warn => "Don't use this method."
    define_deprecated_signal :enter, :warn => "Use 'Gtk::Widget::enter-notify-event' signal."
    define_deprecated_signal :leave, :warn => "Use 'Gtk::Widget::leave-notify-event' signal."
    define_deprecated_signal :pressed, :warn => "Use 'Gtk::Widget::button-press-event' signal."
    define_deprecated_signal :released, :warn => "Use 'Gtk::Widget::button-release-event' signal."
    define_deprecated_method_by_hash_args :initialize,
        'label_or_stock_id, use_underline = nil',
        ':label => nil, :use_underline => nil, :stock_id => nil' do
        |_self, label_or_stock_id, use_underline|
      case label_or_stock_id
      when String
        [{:label => label_or_stock_id, :use_underline => use_underline}]
      when Symbol
        [{:stock_id => label_or_stock_id}]
      else
        [label_or_stock_id]
      end
    end
  end

  class ButtonBox
    extend GLib::Deprecatable
    define_deprecated_const :Style, "Gtk::ButtonBoxStyle"
    define_deprecated_enums "Gtk::ButtonBoxStyle", "STYLE"
  end

  class Calendar
    extend GLib::Deprecatable
    define_deprecated_flags "Gtk::CalendarDisplayOptions"
    define_deprecated_method :freeze, :warn => "Don't use this method."
    define_deprecated_method :thaw, :warn => "Don't use this method."
  end

  class CellRenderer
    extend GLib::Deprecatable
    define_deprecated_const :Mode, "Gtk::CellRendererMode"
    define_deprecated_enums :CellRendererMode, 'MODE'
    define_deprecated_const :State, "Gtk::CellRendererState"
    define_deprecated_flags :CellRendererState
    define_deprecated_method :editing_canceled, :warn => "Use '#{self}#stop_editing'." do |_self|
      _self.stop_editing(true)
    end
    define_deprecated_method :get_size, :raise => "Use Gtk::CellRenderer#get_preferred_size."
  end

  class CellRendererAccel
    extend GLib::Deprecatable
    define_deprecated_const :Mode, "Gtk::CellRendererAccelMode"
    define_deprecated_enums :CellRendererAccelMode, 'MODE'
  end

  class CellView
    extend GLib::Deprecatable
    define_deprecated_method :cell_renderers, :warn => "Use 'Gtk::CellLayout#cells'." do |_self|
      _self.cells
    end
    define_deprecated_method :get_size_of_row, :raise => "Use Gtk::Widget#get_preferred_size."
  end

  class CheckMenuItem
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize,
        "label, use_underline=false",
        ":label => label, :use_underline => use_underline" do |_self, label, use_underline|
      [{:label => label, :use_underline => use_underline}]
    end
  end

  class ComboBox
    extend GLib::Deprecatable
    define_deprecated_method :append_text, :raise => "Use 'Gtk::ComboBoxText#append_text'."
    define_deprecated_method :insert_text, :raise => "Use 'Gtk::ComboBoxText#insert_text'."
    define_deprecated_method :prepend_text, :raise => "Use 'Gtk::ComboBoxText#prepend_text'."
    define_deprecated_method :remove_text, :raise => "Use 'Gtk::ComboBoxText#remove'."
    define_deprecated_method :active_text, :raise => "Use 'Gtk::ComboBoxText#active_text'."
    define_deprecated_method_by_hash_args :initialize,
        'model', ':entry => false, :model => nil, :area => nil' do |_self, model|
      case model
      when TreeModel
        [{:model => model}]
      when true, false
        raise GLib::DeprecatedError.new("#{caller[0]}: '#{self}#initialize(is_text_only)' style has been deprecated. Use 'Gtk::ComboBoxText'.")
      end
    end
  end

  class Container
    extend GLib::Deprecatable
    define_deprecated_singleton_method :child_property, :find_child_property
    define_deprecated_method :each_forall, :each_all
  end

  class Dialog
    extend GLib::Deprecatable
    define_deprecated_const :Flags, "Gtk::DialogFlags"
    define_deprecated_flags :DialogFlags
    define_deprecated_const :ResponseType, 'Gtk::ResponseType'
    define_deprecated_enums 'Gtk::ResponseType', 'RESPONSE'
    define_deprecated_method :vbox, :child
    define_deprecated_method_by_hash_args :initialize,
        'title, parent, flags, *buttons',
        ':title => nil, :parent => nil, :flags => 0, :buttons => nil' do
        |_self, title, parent, flags, *buttons|
      [{:title => title, :parent => parent, :flags => flags, :buttons => buttons}]
    end
  end

  module Drag
    extend GLib::Deprecatable
    define_deprecated_const :DestDefaults, "Gtk::DestDefaults"
    define_deprecated_const :TargetFlags, "Gtk::TargetFlags"
    define_deprecated_singleton_method :finish, :warn => "Use 'Gdk::DragContext#finish'." do |_self, context, success, del, time|
      context.finish(:success => success,
                     :delete => delete,
                     :time => time)
    end
    define_deprecated_singleton_method :set_icon_default, :warn => "Use 'Gdk::DragContext#set_icon_default'." do |_self, context|
      context.set_icon_default
    end
    define_deprecated_singleton_method :set_icon_name, :warn => "Use 'Gdk::DragContext#set_icon'." do |_self, context, name, hot_x, hot_y|
      context.set_icon(:name => name, :hot_x => hot_x, :hot_y => hot_y)
    end
    define_deprecated_singleton_method :set_icon, :warn => "Use 'Gdk::DragContext#set_icon'." do |_self, context, icon, hot_x, hot_y|
      case icon
      when Symbol
        context.set_icon(:stock_id => icon, :hot_x => hot_x, :hot_y => hot_y)
      when GdkPixbuf::Pixbuf
        context.set_icon(:pixbuf => icon, :hot_x => hot_x, :hot_y => hot_y)
      when Gtk::Widget
        context.set_icon(:widget => icon, :hot_x => hot_x, :hot_y => hot_y)
      end
    end
    define_deprecated_singleton_method :dest_set,
        :warn => "Use 'Gtk::Widget#drag_dest_set'." do |_self, widget, flags, targets, actions|
      widget.drag_dest_set(flags, targets, actions)
    end
    define_deprecated_singleton_method :dest_set_proxy,
        :warn => "Use 'Gtk::Widget#drag_dest_set_proxy'." do |_self, widget, proxy_window, protocol, use_coordinates|
      widget.drag_dest_set_proxy(proxy_window, protocol, use_coordinates)
    end
    define_deprecated_singleton_method :dest_unset,
        :warn => "Use 'Gtk::Widget#drag_dest_unset'." do |_self, widget|
      widget.drag_dest_unset
    end
    define_deprecated_singleton_method :dest_find_target,
        :warn => "Use 'Gtk::Widget#drag_dest_find_target'." do |_self, widget, context, target_list|
      widget.drag_dest_find_target(context, target_list)
    end
    define_deprecated_singleton_method :dest_get_target_list,
        :warn => "Use 'Gtk::Widget#drag_dest_get_target_list'." do |_self, widget|
      widget.drag_dest_get_target_list
    end
    define_deprecated_singleton_method :dest_set_target_list,
        :warn => "Use 'Gtk::Widget#drag_dest_set_target_list'." do |_self, widget, target_list|
      widget.drag_dest_set_target_list(target_list)
    end
    define_deprecated_singleton_method :dest_add_text_targets,
        :warn => "Use 'Gtk::Widget#drag_dest_add_text_targets'." do |_self, widget|
      widget.drag_dest_add_text_targets
    end
    define_deprecated_singleton_method :dest_add_image_targets,
        :warn => "Use 'Gtk::Widget#drag_dest_add_image_targets'." do |_self, widget|
      widget.drag_dest_add_image_targets
    end
    define_deprecated_singleton_method :dest_add_uri_targets,
        :warn => "Use 'Gtk::Widget#drag_dest_add_uri_targets'." do |_self, widget|
      widget.drag_dest_add_uri_targets
    end
    define_deprecated_singleton_method :dest_set_track_motion,
        :warn => "Use 'Gtk::Widget#drag_dest_set_track_motion'." do |_self, widget, track_motion|
      widget.drag_dest_set_track_motion(track_motion)
    end
    define_deprecated_singleton_method :dest_get_track_motion,
        :warn => "Use 'Gtk::Widget#drag_dest_get_track_motion'." do |_self, widget|
      widget.drag_dest_get_track_motion
    end
    define_deprecated_singleton_method :get_data,
        :warn => "Use 'Gtk::Widget#drag_get_data'." do |_self, widget, context, target, time|
      widget.drag_get_data(context, target, time)
    end
    define_deprecated_singleton_method :highlight,
        :warn => "Use 'Gtk::Widget#drag_highlight'." do |_self, widget|
      widget.drag_highlight
    end
    define_deprecated_singleton_method :unhighlight,
        :warn => "Use 'Gtk::Widget#drag_unhighlight'." do |_self, widget|
      widget.drag_unhighlight
    end
    define_deprecated_singleton_method :begin,
        :warn => "Use 'Gtk::Widget#drag_begin'." do |_self, widget, target_list, actions, button, event|
      widget.drag_begin(target_list, actions, button, event)
    end
    define_deprecated_singleton_method :threshold?,
        :warn => "Use 'Gtk::Widget#drag_threshold?'." do |_self, widget, start_x, start_y, current_x, current_y|
      widget.drag_threshold?(start_x, start_y, current_x, current_y)
    end
    define_deprecated_singleton_method :source_set,
        :warn => "Use 'Gtk::Widget#drag_source_set'." do |_self, widget, start_button_mask, targets, actions|
      widget.drag_source_set(start_button_mask, targets, actions)
    end
    define_deprecated_singleton_method :source_set_icon,
        :warn => "Use 'Gtk::Widget#drag_source_set_icon(:stock_id => nil, :pixbuf => nil)'." do |_self, widget, icon|
      case icon
      when Symbol
        widget.drag_source_set_icon(:stock_id => icon)
      else
        widget.drag_source_set_icon(:pixbuf => icon)
      end
    end
    define_deprecated_singleton_method :source_set_icon_name,
        :warn => "Use 'Gtk::Widget#drag_source_set_icon(:icon_name => nil)'." do |_self, widget, icon|
      widget.drag_source_set_icon(:icon_name => icon)
    end
    define_deprecated_singleton_method :source_unset,
        :warn => "Use 'Gtk::Widget#drag_source_unset'." do |_self, widget|
      widget.drag_source_unset
    end
    define_deprecated_singleton_method :source_set_target_list,
        :warn => "Use 'Gtk::Widget#drag_source_set_target_list'." do |_self, widget, target_list|
      widget.drag_source_set_target_list(target_list)
    end
    define_deprecated_singleton_method :source_get_target_list,
        :warn => "Use 'Gtk::Widget#drag_source_get_target_list'." do |_self, widget|
      widget.drag_source_get_target_list
    end
    define_deprecated_singleton_method :source_add_text_targets,
        :warn => "Use 'Gtk::Widget#drag_source_add_text_targets'." do |_self, widget|
      widget.drag_source_add_text_targets
    end
    define_deprecated_singleton_method :source_add_image_targets,
        :warn => "Use 'Gtk::Widget#drag_source_add_image_targets'." do |_self, widget|
      widget.drag_source_add_image_targets
    end
    define_deprecated_singleton_method :source_add_uri_targets,
        :warn => "Use 'Gtk::Widget#drag_source_add_uri_targets'." do |_self, widget|
      widget.drag_source_add_uri_targets
    end
  end

  module FileChooser
    extend GLib::Deprecatable
    define_deprecated_const :Action, "Gtk::FileChooserAction"
    define_deprecated_enums :FileChooserAction, 'ACTION'
    define_deprecated_const :Confirmation, "Gtk::FileChooserConfirmation"
    define_deprecated_enums :FileChooserConfirmation, 'CONFIRMATION'
  end

  class FileChooserDialog
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize,
        'title, parent, action, back, *buttons',
        ':title => nil, :parent => nil, :action => :open, :buttons => nil' do
        |_self, title, parent, action, back, *buttons|
      options = {
          :title => title,
          :parent => parent,
          :action => action,
          :buttons => buttons,
      }
      [options]
    end
  end

  class FileFilter
    extend GLib::Deprecatable
    define_deprecated_const :Flags, "Gtk::FileFilterFlags"
    define_deprecated_flags :FileFilterFlags
  end

  class HBox
    extend GLib::Deprecatable
    define_deprecated_singleton_method :new, :warn => "Use 'Gtk::Box.new(:horizontal, spacing)'." do |_self, homogeneous, spacing|
      box = Gtk::Box.new(:horizontal, spacing)
      box.set_homogeneous(homogeneous)
      box
    end
  end

  class HButtonBox
    extend GLib::Deprecatable
    define_deprecated_singleton_method :new, :warn => "Use 'Gtk::ButtonBox.new(:horizontal)'." do |_self|
      Gtk::ButtonBox.new(:horizontal)
    end
  end

  class HPaned
    extend GLib::Deprecatable
    define_deprecated_singleton_method :new, :warn => "Use 'Gtk::Paned.new(:horizontal)'." do |_self|
      Gtk::Paned.new(:horizontal)
    end
  end

  class HScale
    extend GLib::Deprecatable
    define_deprecated_singleton_method :new, :warn => "Use 'Gtk::Scale.new(:horizontal, *args)'." do |_self, *args|
      Gtk::Scale.new(:horizontal, *args)
    end
  end

  class HScrollbar
    extend GLib::Deprecatable
    define_deprecated_singleton_method :new, :warn => "Use 'Gtk::Scrollbar.new(:horizontal, adjustment)'." do |_self, adjustment|
      Gtk::Scrollbar.new(:horizontal, adjustment)
    end
  end

  class HSeparator
    extend GLib::Deprecatable
    define_deprecated_singleton_method :new, :warn => "Use 'Gtk::Separator.new(:horizontal)'." do |_self|
      Gtk::Separator.new(:horizontal)
    end
  end

  class IconSet
    extend GLib::Deprecatable
    define_deprecated_method :render_icon, :raise => "Use '#{self}#render_icon_pixbuf'."
  end

  class IconSize
   extend GLib::Deprecatable
   define_deprecated_const :IconSize, "Gtk::IconSize"
  end

  class IconTheme
    extend GLib::Deprecatable
    define_deprecated_const :LookupFlags, "Gtk::IconLookupFlags"
    define_deprecated_flags :IconLookupFlags, 'LOOKUP'
  end

  class IconView
    extend GLib::Deprecatable
    define_deprecated_enums :IconViewDropPosition
  end

  class Image
    extend GLib::Deprecatable
    define_deprecated_const :Type, "Gtk::ImageType"
    define_deprecated_enums :ImageType
    define_deprecated_method :set, :warn => "Use '#{self}#set_stock', '#{self}#set_icon_name', '#{self}#set_icon_set', '#{self}#set_file', '#{self}#set_pixbuf' or '#{self}#set_pixbuf_animation'."
    define_deprecated_method_by_hash_args :initialize,
        'image, size = nil',
        ':stock => nil, :icon_name => nil, :icon_set => nil, :icon => nil, :file => nil, :pixbuf => nil, :animation => nil, :surface => nil, :size => nil' do
        |_self, image, size|
      case image
      when String
        if size
          [{:icon_name => image, :size => size}]
        else
          [{:file => image}]
        end
      when Symbol
        [{:stock => image, :size => size}]
      when GdkPixbuf::Pixbuf
        [{:pixbuf => image}]
      when Gtk::IconSet
        [{:icon_set => image, :size => size}]
      when Gio::Icon
        [{:icon => image, :size => size}]
      else
        message =
          "Image must be String, Symbol, GdkPixbuf::Pixbuf, Gtk::IconSet or " +
          "Gio::Icon: #{image.inspect}"
        raise ArgumentError, message
      end
    end
  end

  class ImageMenuItem
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize,
        'label_or_stock_id = nil, use_underline_or_accel_group = nil',
        ':label => nil, :mnemonic => nil, :stock_id => nil, :accel_group => nil' do
        |_self, label_or_stock_id, use_underline_or_accel_group|
      case label_or_stock_id
      when String
        if use_underline_or_accel_group
          [{:mnemonic => label_or_stock_id}]
        else
          [{:label => label_or_stock_id}]
        end
      when Symbol
        [{:stock_id => label_or_stock_id, :accel_group => use_underline_or_accel_group}]
      end
    end
  end

  class LinkButton
    extend GLib::Deprecatable
    define_deprecated_singleton_method :set_uri_hook, :warn => "Use 'clicked' signal."
  end

  class MenuBar
    extend GLib::Deprecatable
    define_deprecated_enums :PackDirection, 'PACK_DIRECTION'
  end

  class MenuItem
    extend GLib::Deprecatable

    define_deprecated_method_by_hash_args :initialize,
        "label, use_underline=false",
        ":label => label, :use_underline => use_underline" do |_self, label, use_underline|
      [{:label => label, :use_underline => use_underline}]
    end

    define_deprecated_method :remove_submenu, :warn => "Use '#{self}#set_submenu'." do |_self|
      _self.set_submenu(nil)
    end
  end

  class MenuShell
    extend GLib::Deprecatable
    define_deprecated_enums :DirectionType, 'DIR'
  end

  class MenuToolButton
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize,
        'icon_widget_or_stock_id = nil, label = nil',
        ':icon_widget => nil, :label => nil, :stock_id => nil' do
        |_self, icon_widget_or_stock_id, label|
      case icon_widget_or_stock_id
      when String, Symbol
        [{:stock_id => icon_widget_or_stock_id}]
      when Gtk::Widget
        [{:icon_widget => icon_widget_or_stock_id, :label => label}]
      end
    end
  end

  class MessageDialog
    extend GLib::Deprecatable
    define_deprecated_const :ButtonsType, "Gtk::ButtonsType"
    define_deprecated_enums "Gtk::ButtonsType", "BUTTONS"
    define_deprecated_const :Type, 'Gtk::MessageType'
    define_deprecated_enums 'Gtk::MessageType'
    define_deprecated_method_by_hash_args :initialize,
        'parent, flags, type, buttons_type, message',
        ':parent => nil, :flags => 0, :type => :info, :buttons => :ok, :message => ""' do
        |_self, parent, flags, type, buttons, message|
      [{:parent => parent, :flags => flags, :type => type, :buttons => buttons, :message => message}]
    end
  end

  class Notebook
    extend GLib::Deprecatable
    define_deprecated_singleton_method :set_window_creation_hook, :warn => "Use 'create-window' signal."
    define_deprecated_method :query_tab_label_packing, :warn => "Use 'tab-expand' and 'tab-fill' child property." do |_self, child|
      [_self.child_get_property(child, 'tab-expand'), _self.child_get_property(child, 'tab-fill')]
    end
    define_deprecated_method :set_tab_label_packing, :warn => "Use 'tab-expand' and 'tab-fill' child property." do |_self, child, expand, fill, pack_type|
      _self.child_set_property(child, 'tab-expand', expand)
      _self.child_set_property(child, 'tab-fill', fill)
    end
  end

  class PaperSize
    extend GLib::Deprecatable
    define_deprecated_enums :Unit, 'UNIT'
  end

  class PrintOperation
    extend GLib::Deprecatable
    define_deprecated_const :Action, "Gtk::PrintOperationAction"
    define_deprecated_enums :PrintOperationAction, 'ACTION'
    define_deprecated_const :Result, "Gtk::PrintOperationResult"
    define_deprecated_enums :PrintOperationResult, 'RESULT'
    define_deprecated_const :Status, "Gtk::PrintStatus"
    define_deprecated_enums :PrintStatus, 'STATUS'
  end

  class PrintSettings
    extend GLib::Deprecatable
    define_deprecated_enums :PageOrientation, 'ORIENTATION'
    define_deprecated_enums :PageSet, 'PAGE_SET'
    define_deprecated_enums :PrintDuplex, 'DUPLEX'
    define_deprecated_enums :PrintPages, 'PAGES'
    define_deprecated_enums :PrintQuality, 'QUALITY'
  end

  class Paned
    extend GLib::Deprecatable
    %w(child1 child2).product(%w(resize shrink)).each do |child, prop|
      define_deprecated_method "#{child}_#{prop}?", :warn => "Use '#{prop}' child property." do |_self|
        _self.child_get_property(_self.send(child), prop)
      end
    end
    define_deprecated_method_by_hash_args :pack1,
        'child, resize, shrink',
        'child, :resize => false, :shrink => true', 1 do
        |_self, child, resize, shrink|
      [child, {:resize => resize, :shrink => shrink}]
    end
    define_deprecated_method_by_hash_args :pack2,
        'child, resize, shrink',
        'child, :resize => true, :shrink => true', 1 do
        |_self, child, resize, shrink|
      [child, {:resize => resize, :shrink => shrink}]
    end
  end

  class RadioAction
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize,
        'name, label, tooltip, stock_id, value',
        'name, value, :label => nil, :tooltip => nil, :stock_id => nil', 2 do
        |_self, name, label, tooltip, stock_id, value|
      [name, value, {:label => label, :tooltip => tooltip, :stock_id => stock_id}]
    end
  end

  class RadioButton
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize,
        'member_or_label, label_or_use_underline, use_underline',
        ':label => label, :member => member, :use_underline => use_underline',
        0 do |_self, member_or_label, label_or_use_underline, use_underline|
      options = {}
      if member_or_label.is_a?(Gtk::RadioButton)
        options[:member] = member_or_label
        if label_or_use_underline.is_a?(String)
          options[:label] = label_or_use_underline
          options[:use_underline] = use_underline
        end
      else
        options[:label] = member_or_label
        options[:use_underline] = label_or_use_underline
      end
      [options]
    end
  end

  class Range
    extend GLib::Deprecatable
    define_deprecated_enums :SensitivityType, 'SENSITIVITY'
    define_deprecated_method :update_policy, :raise => "Don't use this method."
    define_deprecated_method :set_update_policy, :warn => "Don't use this method."
    alias :update_policy= :set_update_policy
  end

  class RecentAction
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize,
        'name, label, tooltip = nil, stock_id = nil, manager = nil',
        'name, :label => nil, :tooltip => nil, :stock_id => nil, :manager => nil', 1 do
        |_self, name, label, tooltip, stock_id, manager|
      [name, {:label => label, :tooltip => tooltip, :stock_id => stock_id, :manager => manager}]
    end
  end

  module RecentChooser
    extend GLib::Deprecatable
    define_deprecated_enums :SortType, 'SORT'
    define_deprecated_method :show_numbers, :warn => "Use 'Gtk::RecentChooserMenu#show_numbers?'." do |_self|
      false
    end
    define_deprecated_method :set_show_numbers, :warn => "Use 'Gtk::RecentChooserMenu#set_show_numbers'."
    alias :show_numbers= :set_show_numbers
  end

  class RecentChooserDialog
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize,
        'title, parent, manager, *buttons',
        ':title => nil, :parent => nil, ' +
        ':recent_manager => nil, :buttons => nil' do |_self, title, parent, *buttons|
      if buttons.first.is_a?(RecentManager)
        recent_manager = buttons.shift
      else
        recent_manager = nil
      end
      [
        {
          :title          => title,
          :parent         => parent,
          :recent_manager => recent_manager,
          :buttons        => buttons,
        },
      ]
    end
  end

  class RecentFilter
    extend GLib::Deprecatable
    define_deprecated_const :Flags, "Gtk::RecentFilterFlags"
    define_deprecated_flags :RecentFilterFlags
  end

  class RecentManager
    extend GLib::Deprecatable
    define_deprecated_singleton_method :get_for_screen, :warn => "Use '#{self}.default'." do |_self, screen|
      _self.default
    end
    define_deprecated_method :set_screen, :warn => "Don't use this method."
    alias :screen= :set_screen
  end

  class ProgressBar
    extend GLib::Deprecatable
    define_deprecated_method :set_activity_mode, :warn => "Don't use this method."
    alias :activity_mode= :set_activity_mode
    define_deprecated_method :activity_mode?, :warn => "Don't use this method." do |_self|
      false
    end
    define_deprecated_method :set_text_xalign, :warn => "Don't use this method."
    alias :text_xalign= :set_text_xalign
    define_deprecated_method :text_xalign, :warn => "Don't use this method." do |_self|
      0.0
    end
    define_deprecated_method :set_text_yalign, :warn => "Don't use this method."
    alias :text_yalign= :set_text_yalign
    define_deprecated_method :text_yalign, :warn => "Don't use this method." do |_self|
      0.0
    end
  end

  class SelectionData
    extend GLib::Deprecatable
    define_deprecated_method :type, :data_type
  end

  class SizeGroup
    extend GLib::Deprecatable
    define_deprecated_const :Mode, "Gtk::SizeGroupMode"
    define_deprecated_enums :SizeGroupMode
  end

  class SpinButton
    extend GLib::Deprecatable
    define_deprecated_const :Type, "Gtk::SpinType"
    define_deprecated_enums :SpinType
    define_deprecated_const :UpdatePolicy, "Gtk::SpinButtonUpdatePolicy"
    define_deprecated_enums :SpinButtonUpdatePolicy, 'UPDATE'
  end

  class TextAttributes
    extend GLib::Deprecatable
    define_deprecated_method :realized?, :warn => "Don't use this method." do |_self|
      false
    end
    define_deprecated_method :set_realized, :warn => "Don't use this method."
    alias :realized= :set_realized
  end

  class TextBuffer
    extend GLib::Deprecatable
    define_deprecated_method :get_iter_at_line,
        :warn => "Use '#{self}#get_iter_at(:line => nil)'." do |_self, line|
      _self.get_iter_at(:line => line)
    end
    define_deprecated_method :get_iter_at_line_offset,
        :warn => "Use '#{self}#get_iter_at(:line => nil, :offset => nil)'." do |_self, line, offset|
      _self.get_iter_at(:line => line, :offset => offset)
    end
    define_deprecated_method :get_iter_at_line_index,
        :warn => "Use '#{self}#get_iter_at(:line => nil, :index => nil)'." do |_self, line, index|
      _self.get_iter_at(:line => line, :index => index)
    end
    define_deprecated_method :get_iter_at_offset,
        :warn => "Use '#{self}#get_iter_at(:offset => nil)'." do |_self, offset|
      _self.get_iter_at(:offset => offset)
    end
    define_deprecated_method :get_iter_at_mark,
        :warn => "Use '#{self}#get_iter_at(:mark => nil)'." do |_self, mark|
      _self.get_iter_at(:mark => mark)
    end
    define_deprecated_method :get_iter_at_child_anchor,
        :warn => "Use '#{self}#get_iter_at(:anchor => nil)'." do |_self, anchor|
      _self.get_iter_at(:anchor => anchor)
    end
    define_deprecated_method :insert_pixbuf, :insert
    define_deprecated_method :insert_child_anchor, :insert
    define_deprecated_method :insert_with_tags, :insert
  end

  class TextIter
    extend GLib::Deprecatable
    define_deprecated_const :SearchFlags, "Gtk::TextSearchFlags"
    define_deprecated_flags :TextSearchFlags, 'SEARCH'
    define_deprecated_method :backword_visible_word_start, :backward_visible_word_start
  end

  class TextTag
    extend GLib::Deprecatable
    define_deprecated_const :WrapMode, "Gtk::WrapMode"
    define_deprecated_enums "Gtk::WrapMode", "WRAP"
  end

  class TextView
    extend GLib::Deprecatable
    define_deprecated_enums :WindowType, 'WINDOW'
  end

  class ToggleAction
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize,
        'name, label, tooltip = nil, stock_id = nil',
        'name, :label => nil, :tooltip => nil, :stock_id => nil', 1 do
        |_self, name, label, tooltip, stock_id|
      [name, {:label => label, :tooltip => tooltip, :stock_id => stock_id}]
    end
  end

  class ToggleButton
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize,
        'label=nil, use_underline=nil',
        ':label => label, :use_underline => use_underline', 0 do
        |_self, label, use_underline|
      [{:label => label, :use_underline => use_underline}]
    end
  end

  class Toolbar
    extend GLib::Deprecatable
    define_deprecated_const :Style, "Gtk::ToolbarStyle"
    define_deprecated_method :append, :warn => "Don't use this method."
    define_deprecated_method :prepend, :warn => "Don't use this method."
    define_deprecated_method :item_index, :get_item_index
    define_deprecated_method :nth_item, :get_nth_item
    define_deprecated_method :drop_index, :get_drop_index
    define_deprecated_method :append_space, :warn => "Don't use this method."
    define_deprecated_method :prepend_space, :warn => "Don't use this method."
    define_deprecated_method :insert_space, :warn => "Don't use this method."
    define_deprecated_method :remove_space, :warn => "Don't use this method."
  end

  class ToolButton
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize,
        'icon_widget_or_stock_id = nil, label = nil',
        ':icon_widget => nil, :label => nil, :stock_id => nil' do
        |_self, icon_widget_or_stock_id, label|
      case icon_widget_or_stock_id
      when String, Symbol
        [{:stock_id => icon_widget_or_stock_id}]
      when Gtk::Widget
        [{:icon_widget => icon_widget_or_stock_id, :label => label}]
      end
    end
  end

  class Tooltip
    extend GLib::Deprecatable
    define_deprecated_method :set_icon_from_stock, :warn => "Use '#{self}#set_icon'." do |_self, stock_id, size|
      _self.set_icon(:stock_id => stock_id, :size => size)
    end
  end

  module TreeModel
    extend GLib::Deprecatable
    define_deprecated_const :Flags, "Gtk::TreeModelFlags"
    define_deprecated_flags :TreeModelFlags
  end

  class TreeSelection
    extend GLib::Deprecatable
    define_deprecated_method :selected_each, :each
  end

  class TreeView
    extend GLib::Deprecatable
    define_deprecated_const :DropPosition, "Gtk::TreeViewDropPosition"
    define_deprecated_enums :TreeViewDropPosition, 'DROP'
    define_deprecated_const :GridLines, "Gtk::TreeViewGridLines"
    define_deprecated_enums :TreeViewGridLines, 'GRID_LINES'
    define_deprecated_method :widget_to_tree_coords, :convert_widget_to_bin_window_coords
    define_deprecated_method :tree_to_widget_coords, :convert_bin_window_to_widget_coords
  end

  class TreeViewColumn
    extend GLib::Deprecatable
    define_deprecated_const :Sizing, "Gtk::TreeViewColumnSizing"
    define_deprecated_enums :TreeViewColumnSizing
    define_deprecated_method :cell_renderers, :warn => "Use 'Gtk::CellLayout#cells'." do |_self|
      _self.cells
    end
  end

  class UIManager
    extend GLib::Deprecatable
    define_deprecated_const :ItemType, "Gtk::UIManagerItemType"
    define_deprecated_flags :UIManagerItemType
  end

  class VBox
    extend GLib::Deprecatable
    define_deprecated_singleton_method :new, :warn => "Use 'Gtk::Box.new(:vertical, spacing)'." do |_self, homogeneous, spacing|
      box = Gtk::Box.new(:vertical, spacing)
      box.set_homogeneous(homogeneous)
      box
    end
  end

  class VButtonBox
    extend GLib::Deprecatable
    define_deprecated_singleton_method :new, :warn => "Use 'Gtk::ButtonBox.new(:vertical)'." do |_self|
      Gtk::ButtonBox.new(:vertical)
    end
  end

  class VPaned
    extend GLib::Deprecatable
    define_deprecated_singleton_method :new, :warn => "Use 'Gtk::Paned.new(:vertical)'." do |_self|
      Gtk::Paned.new(:vertical)
    end
  end

  class VScale
    extend GLib::Deprecatable
    define_deprecated_singleton_method :new, :warn => "Use 'Gtk::Scale.new(:vertical, *args)'." do |_self, *args|
      Gtk::Scale.new(:vertical, *args)
    end
  end

  class VScrollbar
    extend GLib::Deprecatable
    define_deprecated_singleton_method :new, :warn => "Use 'Gtk::Scrollbar.new(:vertical, adjustment)'." do |_self, adjustment|
      Gtk::Scrollbar.new(:vertical, adjustment)
    end
  end

  class VSeparator
    extend GLib::Deprecatable
    define_deprecated_singleton_method :new, :warn => "Use 'Gtk::Separator.new(:vertical)'." do |_self|
      Gtk::Separator.new(:vertical)
    end
  end

  class Widget
    extend GLib::Deprecatable
    define_deprecated_const :Align, "Gtk::Align"
    define_deprecated_const :HelpType, "Gtk::WidgetHelpType"
    define_deprecated_enums :WidgetHelpType, 'HELP'
    define_deprecated_const :TextDirection, "Gtk::TextDIrection"
    define_deprecated_enums "Gtk::TextDirection", 'TEXT_DIR'
    define_deprecated_singleton_method :push_colormap, :warn => "Don't use this method."
    define_deprecated_singleton_method :pop_colormap, :warn => "Don't use this method."
    define_deprecated_singleton_method :set_default_colormap, :warn => "Don't use this method."
    class << self
      alias :default_colormap= :set_default_colormap
    end
    define_deprecated_singleton_method :default_colormap, :raise => "Don't use this method."
    define_deprecated_singleton_method :default_visual, :raise => "Don't use this method."
    define_deprecated_singleton_method :default_style, :raise => "Use 'Gtk::StyleContext' and 'Gtk::CssProvider'."
    define_deprecated_const :Flags, :raise => "Don't use this flags anymore."
    define_deprecated_method :flags, :raise => "Use the proper method."
    define_deprecated_method :set_flags, :warn => "Use the proper method."
    alias :flags= :set_flags
    define_deprecated_method :unset_flags, :warn => "Use the proper method."
    define_deprecated_method :get_size_request, :size_request
    define_deprecated_method :no_window?, :warn => "Use '#{self}#has_window?'." do |_self|
      !_self.has_window?
    end
    define_deprecated_method :rc_style?, :has_rc_style?
    define_deprecated_method :parent_sensitive?, :warn => "Use '#{self}#sensitive?' on the parent widget." do |_self|
      _self.parent.sensitive?
    end
    define_deprecated_method :hide_all, :hide
    define_deprecated_method :colormap, :raise => "Use '#{self}#visual'."
    define_deprecated_method :set_colormap, :warn => "Use '#{self}#set_visual'."
    alias :colormap= :set_colormap
    define_deprecated_method :shape_combine_mask, :warn => "Use '#{self}#shape_combine_region' instead."
    define_deprecated_method :input_shape_combine_mask, :warn => "Use '#{self}#input_shape_combine_region' instead."
    define_deprecated_method :reset_shapes, :warn => "Don't use this method."
    define_deprecated_method :set_scroll_adjustments, :warn => "Use 'Gtk::Scrollable#set_hadjustment' and 'Gtk::Scrollable#set_vadjustment'." do |_self, hadj, vadj|
      _self.set_hadjustment(hadj)
      _self.set_vadjustment(vadj)
      true
    end
    alias :set_scroll_adjustment :set_scroll_adjustments
    define_deprecated_method :action, :warn => "Use 'Gtk::Activatable#related_action'." do |_self|
      _self.related_action
    end
    define_deprecated_method :child_requisition, :warn => "Use '#{self}#preferred_size'." do |_self|
      _self.preferred_size.last
    end
    define_deprecated_method :set_state, :warn => "Use '#{self}#set_state_flags'."
    alias :state= :set_state
    define_deprecated_method :ensure_style, :warn => "Use 'Gtk::StyleContext'."
    define_deprecated_method :reset_rc_styles, :warn => "Use '#{self}#reset_style'."
    define_deprecated_method :class_path, :raise => "Use '#{self}#widget_path'."
    define_deprecated_method :modify_style, :warn => "Use 'Gtk::StyleContext'."
    define_deprecated_method :modifier_style, :raise => "Use 'Gtk::StyleContext'."
    define_deprecated_method :modify_fg, :warn => "Use '#{self}#override_color'."
    define_deprecated_method :modify_bg, :warn => "Use '#{self}#override_background_color'."
    define_deprecated_method :modify_text, :warn => "Use '#{self}#override_color'."
    define_deprecated_method :modify_base, :warn => "Use '#{self}#override_background_color'."
    define_deprecated_method :modify_font, :warn => "Use '#{self}#override_font'."
    define_deprecated_method :modify_cursor, :warn => "Use '#{self}#override_cursor'."
    define_deprecated_method :render_icon, :warn => "Use '#{self}#render_icon_pixbuf'." do |_self, stock_id, size, detail|
      _self.render_icon_pixbuf(stock_id, size)
    end
    define_deprecated_method :state, :raise => "Use '#{self}#state_flags'."
    define_deprecated_method :has_rc_style?, :warn => "Use 'Gtk::StyleContext'." do |_self|
      false
    end
    define_deprecated_method :requisition, :raise => "Don't use this method."
    define_deprecated_method :set_requisition, :warn => "Don't use this method."
    define_deprecated_method :saved_state, :raise => "Don't use this method."
    define_deprecated_method :pointer, :raise => "Use 'Gdk::Window#get_device_position'."

    alias :__set_allocation__ :set_allocation
    private :__set_allocation__
    def set_allocation(*args)
      case args.size
      when 1
        __set_allocation__(args.first)
      when 4
        warn "#{caller[0]}: '#{self.class}#set_allocation(x, y, width, height)' style has been deprecated. Use '#{self.class}#set_allocation(alloc)' style."
        __set_allocation__(Gtk::Allocation.new(*args))
      else
        raise ArgumentError.new("need 1 or 4 arguments.")
      end
    end

    define_deprecated_signal :expose_event, :warn => "Use '#{self}::draw' signal."
    define_deprecated_signal :state_changed, :warn => "Use '#{self}::state-flags-changed' signal."
    define_deprecated_signal :style_set, :warn => "Use '#{self}::style-updated' signal."
  end

  class Window
    extend GLib::Deprecatable
    define_deprecated_const :Position, "Gtk::WindowPosition"
    define_deprecated_enums "Gtk::WindowPosition", 'POS'
    define_deprecated_const :Type, "Gtk::WindowType"
    define_deprecated_enums "Gtk::WindowType"
    define_deprecated_method :active_focus, :activate_focus
    define_deprecated_method :active_default, :activate_default
  end

  class RecentFilter
    extend GLib::Deprecatable
    define_deprecated_const :Flags, 'Gtk::RecentFilterFlags'
  end
end

module Gdk
  class DragContext
    define_deprecated_method_by_hash_args :finish,
        'success, delete, time',
        ':success => true, :delete => false, :time => Gdk::CURRENT_TIME' do |_self, success, delete, time|
      [{:success => success, :delete => delete, :time => time}]
    end
  end

  class Event
    define_deprecated_method :event_widget, :warn => "Use 'Gdk::Event#widget'." do |_self|
      _self.widget
    end
  end
end
