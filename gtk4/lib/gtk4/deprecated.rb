# Copyright (C) 2011-2022  Ruby-GNOME Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

module Gtk
  extend GLib::Deprecatable
  define_deprecated_const :AccelGroup, raise: "Don't use this class."
  define_deprecated_const :Action, raise: "Use 'Gio::SimpleAction' instead."
  define_deprecated_const :ActionGroup, raise: "Use 'Gio::SimpleActionGroup' instead."
  define_deprecated_const :Allocation, "Gdk::Rectangle"
  define_deprecated_const :Arrow, raise: "Don't use this class."
  define_deprecated_const :ButtonBox, raise: "Don't use this class."
  define_deprecated_const :Clipboard, raise: "Use 'Gdk::Clipboard' instead."
  define_deprecated_const :Combo,                  :raise => "Use 'Gtk::ComboBoxText' instead."
  define_deprecated_const :ComboBoxEntry,          :raise => "Use 'Gtk::ComboBox' instead."
  define_deprecated_const :Curve,                  :raise => "Don't use this widget anymore."
  define_deprecated_const :FileSelection,          :raise => "Use 'Gtk::FileChooserDialog' instead."
  define_deprecated_const :FontSelection,          :raise => "Use 'Gtk::FontChooserWidget' instead."
  define_deprecated_const :FontSelectionDialog,    :raise => "Use 'Gtk::FontChooserDialog' instead."
  define_deprecated_const :ColorSelection,         :raise => "Use 'Gtk::ColorChooserWidget' instead."
  define_deprecated_const :ColorSelectionDialog,   :raise => "Use 'Gtk::ColorChooserDialog' instead."
  define_deprecated_const :GammaCurve,             :raise => "Don't use this widget anymore."
  define_deprecated_const :HRuler,                 :raise => "Don't use this widget anymore."
  define_deprecated_const :InputDialog,            :raise => "Don't use this widget anymore."
  define_deprecated_const :ItemFactory,            :raise => "Use 'Gtk::UIManager' instead."
  define_deprecated_const :Menu, raise: "Use 'Gtk::PopoverMenu' instead."
  define_deprecated_const :MenuBar, raise: "Use 'Gtk::PopoverMenuBar' instead."
  define_deprecated_const :MenuItem, raise: "Don't use this class."
  define_deprecated_const :MenuShell, raise: "Don't use this class."
  define_deprecated_const :MenuToolButton, raise: "Don't use this class."
  define_deprecated_const :OptionMenu,             :raise => "Use 'Gtk::ComboBox' instead."
  define_deprecated_const :PageSetupUnixDialog,    :raise => "Use 'Gtk::PrintOperation' instead."
  define_deprecated_const :Printer,                :raise => "Use 'Gtk::PrintOperation' instead."
  define_deprecated_const :PrintCapabilities,      :raise => "Use 'Gtk::PrintOperation' instead."
  define_deprecated_const :PrintJob,               :raise => "Use 'Gtk::PrintOperation' instead."
  define_deprecated_const :PrintUnixDialog,        :raise => "Use 'Gtk::PrintOperation' instead."
  define_deprecated_const :RC,                     :raise => "Use 'Gtk::StyleContext' instead."
  define_deprecated_const :RadioAction, raise: "Don't use this class."
  define_deprecated_const :RadioButton, raise: "Don't use this class."
  define_deprecated_const :RcStyle,                :raise => "Use 'Gtk::CssProvider' instead."
  define_deprecated_const :RecentChooser, raise: "Don't use this class."
  define_deprecated_const :RecentChooserDialog, raise: "Don't use this class."
  define_deprecated_const :RecentFilter, raise: "Don't use this class."
  define_deprecated_const :RecentFilterInfo, raise: "Don't use this class."
  define_deprecated_const :Ruler,                  :raise => "Don't use this widget anymore."
  define_deprecated_const :SelectionData, raise: "Use 'Gtk::DragSource' instead."
  define_deprecated_const :Stock, raise: "Use 'Gtk::Image.new(icon_name)' instead."
  define_deprecated_const :StockItem, raise: "Use 'Gtk::Image.new(icon_name)' instead."
  define_deprecated_const :Style,                  :raise => "Use 'Gtk::StyleContext' instead."
  define_deprecated_const :StyleProperties, raise: "Don't use this class."
  define_deprecated_const :Table, raise: "Use 'Gtk::Grid' instead."
  define_deprecated_const :TargetEntry, raise: "Use 'Gtk::DragSource' instead."
  define_deprecated_const :TargetList, raise: "Use 'Gtk::DragSource' instead."
  define_deprecated_const :ToggleAction, raise: "Don't use this class."
  define_deprecated_const :ToolButton, raise: "Don't use this class."
  define_deprecated_const :Tooltips,               :raise => "Use 'Gtk::Tooltip' API."
  define_deprecated_const :UIManager, raise: "Use 'Gtk::Builder' instead."
  define_deprecated_const :VRuler,                 :raise => "Don't use this widget anymore."
  define_deprecated_const :AccelFlags, raise: "Don't use this class."
  define_deprecated_const :AttachOptions, raise: "Don't use this class."
  define_deprecated_enums :CornerType, 'CORNER'
  define_deprecated_enums :DeleteType, 'DELETE'
  define_deprecated_enums :DirectionType, 'DIR'
  define_deprecated_const :ExpanderStyle, raise: "Don't use this class."
  define_deprecated_enums :Justification, 'JUSTIFY'
  define_deprecated_enums :MovementStep, 'MOVEMENT'
  define_deprecated_enums :Orientation, 'ORIENTATION'
  define_deprecated_enums :PackType, 'PACK'
  define_deprecated_const :PathPriorityType, raise: "Don't use this class."
  define_deprecated_const :PathType, raise: "Don't use this class."
  define_deprecated_enums :PolicyType, 'POLICY'
  define_deprecated_enums :PositionType, 'POS'
  define_deprecated_const :ReliefStyle, raise: "Don't use this class."
  define_deprecated_const :ResizeMode, raise: "Don't use this class."
  define_deprecated_enums :ScrollStep, 'SCROLL'
  define_deprecated_enums :ScrollType, 'SCROLL'
  define_deprecated_enums :SelectionMode, 'SELECTION'
  define_deprecated_const :ShadowType, raise: "Don't use this class."
  define_deprecated_const :StateType, raise: "Don't use this class."
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

  class Alignment
    extend GLib::Deprecatable
    define_deprecated_const :Align, "Gtk::Align"
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
    define_deprecated_method :add, :append
    define_deprecated_method :pack_start_defaults, :prepend
    define_deprecated_method :pack_end_defaults, :append
    set_pack_properties = lambda do |_self, child, args|
      if args.size == 1 and args[0].is_a?(Hash)
        options = args[0]
        expand = options[:expand]
        fill = options[:fill]
        padding = options[:padding]
        pack_type = options[:pack_type]
      else
        expand, fill, padding, pack_type, = *args
      end
      unless expand.nil?
        if _self.orientation == Orientation::HORIZONTAL
          child.hexpand = expand
        else
          child.vexpand = expand
        end
      end
      if fill
        if _self.orientation == Orientation::HORIZONTAL
          child.halign = :fill
        else
          child.valign = :fill
        end
      end
      unless padding.nil?
        child.margin_top = padding
        child.margin_bottom = padding
        child.margin_start = padding
        child.margin_end = padding
      end
      if pack_type
        if _self.orientation == Orientation::HORIZONTAL
          child.halign = pack_type
        else
          child.valign = pack_type
        end
      end
    end
    define_deprecated_method :pack_start, :prepend do |_self, child, *args|
      _self.prepend(child)
      set_pack_properties.call(_self, child, args)
    end
    define_deprecated_method :pack_end, :append do |_self, child, *args|
      _self.append(child)
      set_pack_properties.call(_self, child, args)
    end
    message = "Set child's " +
              "Gtk::Widget::{hexpand,vexpand,halign,valign,margin-*} " +
              "properties instead."
    define_deprecated_method :set_child_packing,
                             warn: message do |_self, child, *args|
      set_pack_properties.call(_self, child, args)
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

  class Calendar
    extend GLib::Deprecatable
    define_deprecated_const :CalendarDisplayOptions, raise: "Don't use this class."
    define_deprecated_method :freeze, :warn => "Don't use this method."
    define_deprecated_method :thaw, :warn => "Don't use this method."
    define_deprecated_method :select_month, :select_day do |_self, month, year|
      date = _self.date
      _self.select_day(GLib::DateTime.new(year: year,
                                          month: month,
                                          day: date.day_of_month,
                                          hour: date.hour,
                                          minute: date.minute,
                                          second: date.second))
    end
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
    message = "Use Gtk::Window#set_modal and 'response' signal instead."
    define_deprecated_method :run, raise: message
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
    define_deprecated_const :FileChooserConfirmation, raise: "Don't use this class."
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
    define_deprecated_const :Flags, raise: "Don't use this class."
    define_deprecated_const :FileFilterFlags, raise: "Don't use this class."
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

  class HeaderBar
    extend GLib::Deprecatable

    define_deprecated_method :title,
                             raise: "Use '#{self}#title_widget' instead."
    define_deprecated_method :set_title,
                             raise: "Use '#{self}#set_title_widget' instead."
    define_deprecated_method :title=,
                             raise: "Use '#{self}#title_widget=' instead."

    define_deprecated_method :custom_title,
                             raise: "Use '#{self}#title_widget' instead."
    define_deprecated_method :set_custom_title,
                             raise: "Use '#{self}#set_title_widget' instead."
    define_deprecated_method :custom_title=,
                             raise: "Use '#{self}#title_widget=' instead."

    define_deprecated_method :subtitle, raise: "Don't use this method."
    define_deprecated_method :set_subtitle, raise: "Don't use this method."
    define_deprecated_method :subtitle=, raise: "Don't use this method."

    define_deprecated_method :show_close_button?, :show_title_buttons?
    define_deprecated_method :set_show_close_button, :set_show_title_buttons
    define_deprecated_method :show_close_button=, :show_title_buttons=

    define_deprecated_method :spacing,
                             raise: "Use 'Gtk::Widget#margin_*' instead."
    define_deprecated_method :set_spacing,
                             raise: "Use 'Gtk::Widget#set_margin_*' instead."
    define_deprecated_method :spacing=,
                             raise: "Use 'Gtk::Widget#margin_*=' instead."
  end

  class IconSet
    extend GLib::Deprecatable
    define_deprecated_method :render_icon, :raise => "Use '#{self}#render_icon_pixbuf'."
  end

  class IconSize
   extend GLib::Deprecatable
   define_deprecated_const :IconSize, "Gtk::IconSize"
   define_deprecated_singleton_method :lookup, raise: "Don't use this method."
  end

  class IconTheme
    extend GLib::Deprecatable
    define_deprecated_const :LookupFlags, "Gtk::IconLookupFlags"
    define_deprecated_flags :IconLookupFlags, 'LOOKUP'
    define_deprecated_method :choose_icon, :lookup_icon
    define_deprecated_method :contexts, raise: "Don't use this method."
    define_deprecated_method :icons, :icon_names
  end

  class IconView
    extend GLib::Deprecatable
    define_deprecated_enums :IconViewDropPosition
  end

  class Image
    extend GLib::Deprecatable
    define_deprecated_const :Type, "Gtk::ImageType"
    define_deprecated_enums :ImageType
    message = "Use '#{self}#set_icon_name', " +
              "'#{self}#set_file' or '#{self}#set_pixbuf'."
    define_deprecated_method :set, warn: message
    define_deprecated_method_by_hash_args :initialize,
        'image',
        'icon_name: nil, icon: nil, file: nil, pixbuf: nil' do
        |_self, image|
      case image
      when String
        if File.extname(image).empty?
          [{icon_name: image}]
        else
          [{file: image}]
        end
      when Symbol
        [{icon_name: iamge}]
      when GdkPixbuf::Pixbuf
        [{:pixbuf => image}]
      when Gio::Icon
        [{icon: image}]
      else
        message =
          "Image must be String, Symbol, GdkPixbuf::Pixbuf or " +
          "Gio::Icon: #{image.inspect}"
        raise ArgumentError, message
      end
    end
    define_deprecated_method :pixbuf, :paintable
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

  class LevelBar
    extend GLib::Deprecatable
    define_deprecated_const :Mode, "Gtk::LevelBarMode"
  end

  class LinkButton
    extend GLib::Deprecatable
    define_deprecated_singleton_method :set_uri_hook, :warn => "Use 'clicked' signal."
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

  class Paned
    extend GLib::Deprecatable
    define_deprecated_method :pack1,
                             :set_start_child do |_self, child, options=nil|
      options = {resize: false, shrink: true}.merge(options || {})
      _self.set_start_child(child, options)
    end
    define_deprecated_method :pack2,
                             :set_end_child do |_self, child, options=nil|
      options = {resize: true, shrink: true}.merge(options || {})
      _self.set_end_child(child, options)
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

  class RecentManager
    extend GLib::Deprecatable
    define_deprecated_singleton_method :get_for_screen, :warn => "Use '#{self}.default'." do |_self, screen|
      _self.default
    end
    define_deprecated_method :set_screen, :warn => "Don't use this method."
    alias :screen= :set_screen
  end

  class Revealer
    extend GLib::Deprecatable
    define_deprecated_const :TransitionType, "Gtk::RevealerTransitionType"
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

  class Stack
    extend GLib::Deprecatable
    define_deprecated_const :TransitionType, "Gtk::StackTransitionType"
  end

  class StypeContext
    extend GLib::Deprecatable
    define_deprecated_singleton_method :add_provider_for_screen,
                                       :add_provider_for_display do |_self, display, provider, priority=nil|
      _self.add_privder_for_display(display, provider, priority)
    end
    define_deprecated_singleton_method :reset_widgets,
                                       warn: "Don't use this method."
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
    define_deprecated_method :insert_child_anchor, :insert
    define_deprecated_method :insert_paintable, :insert
    define_deprecated_method :insert_pixbuf, :insert
    define_deprecated_method :insert_with_tags, :insert
    define_deprecated_method :deserialize, raise: "Don't use this method."
    define_deprecated_method :serialize, raise: "Don't use this method."
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
    define_deprecated_const :WindowType, raise: "Don't use this class."
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
    define_deprecated_const :WidgetHelpType, raise: "Don't use this class."
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
    define_deprecated_method :set_allocation, raise: "Don't use this method."
    define_deprecated_signal :expose_event, :warn => "Use '#{self}::draw' signal."
    define_deprecated_signal :state_changed, :warn => "Use '#{self}::state-flags-changed' signal."
    define_deprecated_signal :style_set, :warn => "Use '#{self}::style-updated' signal."

    define_deprecated_method :events, raise: "Use 'Gtk::EventControllers' instead."
    define_deprecated_method :add_events, raise: "Use 'Gtk::EventControllers' instead."
    define_deprecated_method :set_events, raise: "Use 'Gtk::EventControllers' instead."
    define_deprecated_method :events=, raise: "Use 'Gtk::EventControllers' instead."
    define_deprecated_method :drag_source_set, raise: "Use 'Gtk::DragSource' instead."
    define_deprecated_method :drag_dest_set, raise: "Use 'Gtk::DropTarget' instead."
    define_deprecated_method :style_attach, raise: "Don't use this method."
    define_deprecated_method :style_get, raise: "Don't use this method."
    define_deprecated_method :style_get_property, raise: "Don't use this method."
    define_deprecated_method :render_icon_pixbuf,
                             raise: "Don't use this method."
    define_deprecated_method :set_icon, raise: "Use '#{self}#set_icon_name' instead."
    define_deprecated_method :icon=, raise: "Use '#{self}#icon_name=' instead."
  end

  class Window
    extend GLib::Deprecatable
    define_deprecated_const :Position, raise: "Don't use this class."
    define_deprecated_const :Type, raise: "Don't use this class."
    define_deprecated_method :active_focus, :activate_focus
    define_deprecated_method :active_default, :activate_default
    define_deprecated_method :add, :set_child
  end
end

module Gdk
  class Event
    define_deprecated_method :event_widget, raise: "Use 'Gtk::EventController#widget' instead."
    define_deprecated_method :widget, raise: "Use 'Gtk::EventController#widget' instead."
  end
end
