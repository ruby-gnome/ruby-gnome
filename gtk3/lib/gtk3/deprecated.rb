module Gtk
  extend GLib::Deprecatable
  define_deprecated_const :Combo,         :raise => "Use 'Gtk::ComboBoxText' instead."
  define_deprecated_const :ComboBoxEntry, :raise => "Use 'Gtk::ComboBox' instead."
  define_deprecated_const :Curve,         :raise => "Don't use this widget anymore."
  define_deprecated_const :FileSelection, :raise => "Use 'Gtk::FileChooserDialog' instead."
  define_deprecated_const :GammaCurve,    :raise => "Don't use this widget anymore."
  define_deprecated_const :HRuler,        :raise => "Don't use this widget anymore."
  define_deprecated_const :InputDialog,   :raise => "Don't use this widget anymore."
  define_deprecated_const :ItemFactory,   :raise => "Use 'Gtk::UIManager' instead."
  define_deprecated_const :OptionMenu,    :raise => "Use 'Gtk::ComboBox' instead."
  define_deprecated_const :RC,            :raise => "Use 'Gtk::StyleContext' instead."
  define_deprecated_const :RcStyle,       :raise => "Use 'Gtk::CssProvider' instead."
  define_deprecated_const :Ruler,         :raise => "Don't use this widget anymore."
  define_deprecated_const :Style,         :raise => "Use 'Gtk::StyleContext' instead."
  define_deprecated_const :Tooltips,      :raise => "Use 'Gtk::Tooltip' API."
  define_deprecated_const :VRuler,        :raise => "Don't use this widget anymore."
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
  end

  class Arrow
    extend GLib::Deprecatable
    define_deprecated_enums :Type
  end

  class Assistant
    extend GLib::Deprecatable
    define_deprecated_enums :PageType, 'PAGE'
  end

  class BindingSet
    extend GLib::Deprecatable
    define_deprecated_method :entry_clear, :entry_remove
  end

  class Box
    extend GLib::Deprecatable
    define_deprecated_method :pack_start_defaults, :pack_start
    define_deprecated_method :pack_end_defaults, :pack_end
  end

  class ButtonBox
    extend GLib::Deprecatable
    define_deprecated_enums :Style
  end

  class Calendar
    extend GLib::Deprecatable
    define_deprecated_flags :DisplayOptions
    define_deprecated_method :freeze, :warn => "Don't use this method."
    define_deprecated_method :thaw, :warn => "Don't use this method."
  end

  class CellRenderer
    extend GLib::Deprecatable
    define_deprecated_enums :Mode, 'MODE'
    define_deprecated_flags :State
    define_deprecated_method :editing_canceled, :warn => "Use '#{self}#stop_editing'." do |_self|
      _self.stop_editing(true)
    end
  end

  class CellRendererAccel
    extend GLib::Deprecatable
    define_deprecated_enums :Mode, 'MODE'
  end

  class CellView
    extend GLib::Deprecatable
    define_deprecated_method :cell_renderers, :warn => "Use 'Gtk::CellLayout#cells'." do |_self|
      _self.cells
    end
  end

  class ColorSelectionDialog
    extend GLib::Deprecatable
    define_deprecated_method :colorsel, :color_selection
    define_deprecated_method_by_hash_args :initialize,
        'title', ':title => nil' do |_self, title|
      {:title => title}
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
        {:model => model}
      when true, false
        raise GLib::DeprecatedError.new("#{caller[0]}: '#{self}#initialize(is_text_only)' style has been deprecated. Use 'Gtk::ComboBoxText'.")
      end
    end
  end

  class Dialog
    extend GLib::Deprecatable
    define_deprecated_flags :Flags
    define_deprecated_enums :ResponseType, 'RESPONSE'
    define_deprecated_method :vbox, :child
    define_deprecated_method_by_hash_args :initialize,
        'title, parent, flags, *buttons',
        ':title => nil, :parent => nil, :flags => 0, :buttons => nil' do
        |_self, title, parent, flags, *buttons|
      {:title => title, :parent => parent, :flags => flags, :buttons => buttons}
    end
  end

  module Drag
    extend GLib::Deprecatable
    define_deprecated_flags :DestDefaults, 'DEST_DEFAULT'
    define_deprecated_flags :TargetFlags, 'TARGET'
  end

  module FileChooser
    extend GLib::Deprecatable
    define_deprecated_enums :Action, 'ACTION'
    define_deprecated_enums :Confirmation, 'CONFIRMATION'
  end

  class FileChooserDialog
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize,
        'title, parent, action, back, *buttons',
        ':title => nil, :parent => nil, :action => :open, :buttons => nil' do
        |_self, title, parent, action, back, *buttons|
      {:title => title, :parent => parent, :action => action, :buttons => buttons}
    end
  end

  class FileFilter
    extend GLib::Deprecatable
    define_deprecated_flags :Flags
  end

  class FontSelectionDialog
    extend GLib::Deprecatable
    define_deprecated_method :apply_button, :raise => "Don't use this method."
    define_deprecated_method_by_hash_args :initialize,
        'title', ':title => nil' do |_self, title|
      {:title => title}
    end
  end

  module IconSize
    extend GLib::Deprecatable
    define_deprecated_enums :IconSize
  end

  class IconTheme
    extend GLib::Deprecatable
    define_deprecated_flags :LookupFlags, 'LOOKUP'
  end

  class IconView
    extend GLib::Deprecatable
    define_deprecated_enums :Type
  end

  class Image
    extend GLib::Deprecatable
    define_deprecated_enums :Type
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
    define_deprecated_method :remove_submenu, :warn => "Use '#{self}#set_submenu'." do |_self|
      _self.set_submenu(nil)
    end
  end

  class MenuShell
    extend GLib::Deprecatable
    define_deprecated_enums :DirectionType, 'DIR'
  end

  class MessageDialog
    extend GLib::Deprecatable
    define_deprecated_enums :ButtonsType, 'BUTTONS'
    define_deprecated_enums :Type
    define_deprecated_method_by_hash_args :initialize,
        'parent, flags, type, buttons_type, message',
        ':parent => nil, :flags => 0, :type => :info, :buttons_type => :ok, :message => ""' do
        |_self, parent, flags, type, buttons_type, message|
      {:parent => parent, :flags => flags, :type => type, :buttons_type => buttons_type, :message => message}
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

  class PageSetupUnixDialog
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize,
        'title, parent', ':title => nil, :parent => nil' do |_self, title, parent|
      {:title => title, :parent => parent}
    end
  end

  class PaperSize
    extend GLib::Deprecatable
    define_deprecated_enums :Unit, 'UNIT'
  end

  class PrintOperation
    extend GLib::Deprecatable
    define_deprecated_enums :Action, 'ACTION'
    define_deprecated_enums :Result, 'RESULT'
    define_deprecated_enums :Status, 'STATUS'
  end

  class PrintSettings
    extend GLib::Deprecatable
    define_deprecated_enums :PageOrientation, 'ORIENTATION'
    define_deprecated_enums :PageSet, 'PAGE_SET'
    define_deprecated_enums :PrintDuplex, 'DUPLEX'
    define_deprecated_enums :PrintPages, 'PAGES'
    define_deprecated_enums :PrintQuality, 'QUALITY'
  end

  class PrintUnixDialog
    extend GLib::Deprecatable
    define_deprecated_flags :Capabilities, 'CAPABILITY'
    define_deprecated_method_by_hash_args :initialize,
        'title, parent', ':title => nil, :parent => nil' do |_self, title, parent|
      {:title => title, :parent => parent}
    end
  end

  class Paned
    extend GLib::Deprecatable
    %w(child1 child2).product(%w(resize shrink)).each do |child, prop|
      define_deprecated_method "#{child}_#{prop}?", :warn => "Use '#{prop}' child property." do |_self|
        _self.child_get_property(_self.send(child), prop)
      end
    end
  end

  class Range
    extend GLib::Deprecatable
    define_deprecated_enums :SensitivityType, 'SENSITIVITY'
    define_deprecated_method :update_policy, :raise => "Don't use this method."
    define_deprecated_method :set_update_policy, :warn => "Don't use this method."
    alias :update_policy= :set_update_policy
  end

  module RC
    extend GLib::Deprecatable
    define_deprecated_flags :Flags
    define_deprecated_enums :TokenType, 'TOKEN'
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
        ':title => nil, :parent => nil, :manager => nil, :buttons => nil' do
        |_self, title, parent, *buttons|
      manager = buttons.first.is_a?(RecentManager) ? buttons.shift : nil
      {:title => title, :parent => parent, :manager => manager, :buttons => buttons}
    end
  end

  class RecentFilter
    extend GLib::Deprecatable
    define_deprecated_flags :Flags
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

  class SizeGroup
    extend GLib::Deprecatable
    define_deprecated_enums :Mode
  end

  class SpinButton
    extend GLib::Deprecatable
    define_deprecated_enums :Type
    define_deprecated_enums :UpdatePolicy, 'UPDATE'
  end

  class TextAttributes
    extend GLib::Deprecatable
    define_deprecated_method :realized?, :warn => "Don't use this method." do |_self|
      false
    end
    define_deprecated_method :set_realized, :warn => "Don't use this method."
    alias :realized= :set_realized
  end

  class TextIter
    extend GLib::Deprecatable
    define_deprecated_flags :SearchFlags, 'SEARCH'
    define_deprecated_method :backword_visible_word_start, :backward_visible_word_start
  end

  class TextTag
    extend GLib::Deprecatable
    define_deprecated_enums :WrapMode, 'WRAP'
  end

  class TextView
    extend GLib::Deprecatable
    define_deprecated_enums :WindowType, 'WINDOW'
  end

  module TreeModel
    extend GLib::Deprecatable
    define_deprecated_flags :Flags
  end

  class TreeView
    extend GLib::Deprecatable
    define_deprecated_enums :DropPosition, 'DROP'
    define_deprecated_enums :GridLines, 'GRID_LINES'
    define_deprecated_method :widget_to_tree_coords, :convert_widget_to_bin_window_coords
    define_deprecated_method :tree_to_widget_coords, :convert_bin_window_to_widget_coords
  end

  class TreeViewColumn
    extend GLib::Deprecatable
    define_deprecated_enums :Sizing
    define_deprecated_method :cell_renderers, :warn => "Use 'Gtk::CellLayout#cells'." do |_self|
      _self.cells
    end
  end

  class UIManager
    extend GLib::Deprecatable
    define_deprecated_flags :ItemType
  end

  class Widget
    extend GLib::Deprecatable
    define_deprecated_enums :HelpType, 'HELP'
    define_deprecated_enums :TextDirection, 'TEXT_DIR'
    define_deprecated_singleton_method :push_colormap, :warn => "Don't use this method."
    define_deprecated_singleton_method :pop_colormap, :warn => "Don't use this method."
    define_deprecated_singleton_method :set_default_colormap, :warn => "Don't use this method."
    class << self
      alias :default_colormap= :set_default_colormap
    end
    define_deprecated_singleton_method :default_colormap, :raise => "Don't use this method."
    define_deprecated_singleton_method :default_visual, :raise => "Don't use this method."
    define_deprecated_const :Flags, :raise => "Don't use this flags anymore."
    define_deprecated_method :flags, :raise => "Use the proper method."
    define_deprecated_method :set_flags, :warn => "Use the proper method."
    alias :flags= :set_flags
    define_deprecated_method :unset_flags, :warn => "Use the proper method."
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
    define_deprecated_method :shape_combine_mask, :warn => "Don't use this method."
    define_deprecated_method :input_shape_combine_mask, :warn => "Don't use this method."
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
    define_deprecated_method :size_request, :warn => "Use '#{self}#preferred_size'." do |_self|
      _self.preferred_size.last
    end
    define_deprecated_method :child_requisition, :warn => "Use '#{self}#preferred_size'." do |_self|
      _self.preferred_size.last
    end
    define_deprecated_method :set_state, :warn => "Use '#{self}#set_state_flags'."
    alias :state= :set_state

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
  end

  class Window
    extend GLib::Deprecatable
    define_deprecated_enums :Position, 'POS'
    define_deprecated_enums :Type
    define_deprecated_method :active_focus, :activate_focus
    define_deprecated_method :active_default, :activate_default
  end
end

