#!/usr/bin/env ruby

=begin

  rbbr.rb - Ruby Meta-Level Information Browser

  $Author: mutoh $
  $Date: 2002/06/22 05:26:49 $

  Copyright (C) 2000-2001 Hiroshi Igarashi

  This program is free software.
  You can distribute/modify this program under
  the terms of the Ruby Distribute License.

=end


=begin

  meta/metainfo.rb - API for Meta-level Information

  $Author: mutoh $
  $Date: 2002/06/22 05:26:49 $

  Copyright (C) 2000 Hiroshi Igarashi

  This program is free software.
  You can distribute/modify this program under
  the terms of the Ruby Distribute License.

=end

=begin
Object#ivar_get, ivar_set
=end
module Kernel
  def ivar_get(name)
    eval("@#{name.to_s}")
  end
  def ivar_set(name, val)
    eval("@#{name.to_s} = val")
  end
end

=begin
Module#inner_name, outer_name, outer
=end
class Module
  def inner_name
    name.split("::")[-1]
  end
  def outer_name
    #name.split("::")[0..-2].join("::")
    name.split("::")[0..-2]
  end
  def outer
    eval(name.split("::")[0..-2].join("::"))
  end
end

=begin
Module#constants_at, included_modules_at
=end
class Module

  unless Module.method_defined?(:constants_at)
    def constants_at
      cs = constants
      included_modules_at.each do |im|
	cs -= im.constants
      end
      cs
    end
  end
  unless Module.method_defined?(:included_modules_at)
    def included_modules_at
      ams = ancestors
      case ams.size # > 0
      when 1 # root modules
	[]
      when 2 # 2nd gen. modules and class Object
	[ams[1]]
      else   # ams.size>2  other modules and classes
	ims = ams
	i = 1
	while i < ams.size
	  m = ams[i]
	  if m.type == Class
	    ims -= m.ancestors
	    break
	  end
	  ims -= m.included_modules_at
	  i += 1
	end
	ims - [self]
      end
    end
  end

end

=begin
Class#constants_at
=end
class Class
  unless Module.method_defined?(:constants_at)
    def constants_at
      if self == Object
	constants
      else
	super - superclass.constants
      end
    end
  end
end

=begin
Module.update_metainfo, root_modules
Module#true_constants, sub_modules
=end
class Module

  class << self

    public
    def update_metainfo
      @modules = []      # [Module]
      @root_modules = [] # [Module]
      @dil = {}          # Module -> Library
      @dil_rev = {}      # Library -> [Module]
      @dim = {}          # Module -> Module  (not needed?)
      @dim_rev = {}      # Module -> [Module]
      @constants = {}    # Module -> [String](constant name)
      # @include = {}     # Module -> [Module] (eq. to Module#included_modules)
      @include_rev = {}  # Module -> [Module]
      # @inherit = {}     # Class -> Class (eq. to Class#superclass)
      @inherit_rev = {}  # Class -> [Class]

      ObjectSpace.each_object(Module) do |m|
	@modules << m
	@include_rev[m] = [] if @include_rev[m].nil?
	ims = m.included_modules_at
	ims.each do |im|
	  @include_rev[im] = [] if @include_rev[im].nil?
	  @include_rev[im] << m
	end
	  
	if Class === m
	  if m == Object
	  else
	    c, sc = m, m.superclass
	    @inherit_rev[c] = [] if @inherit_rev[c].nil?
	    @inherit_rev[sc] = [] if @inherit_rev[sc].nil?
	    @inherit_rev[sc] << c
	  end
	else # m is Module but Class
	  if ims.empty?
	    @root_modules << m
	  end
	end
	#__update_metainfo_constants(m)
      end

      nil
    end

    private
    def __update_metainfo_constants(m)
      @constants[m] = []
      @dim_rev[m] = [] if @dim_rev[m].nil?
      m.constants_at.each do |const_name|
	const = m.const_get(const_name)
	if Module === const
	  @dim[const] = m
	  @dim_rev[m] << const
	else
	  @constants[m] << const_name
	end
      end
      # Object is an exception
      if m == Object
	@dim_rev[m] -= [m]
      end
      nil
    end

    def __eusure_metainfo
      unless @metainfo_initialized
	update_metainfo
	@metainfo_initialized = true
      end
    end

    public
    def root_modules
      __eusure_metainfo
      @root_modules.freeze
      @root_modules
    end

    def __true_constants(m)
      __eusure_metainfo
      __update_metainfo_constants(m) if @constants[m].nil?
      ns = @constants[m]
      ns.freeze
      ns
    end

    def __sub_modules(m)
      __eusure_metainfo
      ms = @include_rev[m]
      ms.freeze
      ms
    end

    def __sub_classes(c)
      __eusure_metainfo
      cs = @inherit_rev[c]
      cs.freeze
      cs
    end

    def __dump_metainfo(port=STDOUT)
      __eusure_metainfo
      port.puts("*** modules ***")
      p(@modules.sort{|x, y| x.name <=> y.name})
      port.puts("*** dim ***")
      @dim_rev.keys.sort{|x, y| x.name <=> y.name}.each do |outer_module|
	inner_modules = @dim_rev[outer_module]
	inner_modules_rep = inner_modules.sort{|x, y| x.name <=> y.name}.inspect
	port.puts("#{outer_module}:: #{inner_modules_rep}")
      end
      port.puts("*** constants ***")
      @constants.keys.sort{|x, y| x.name <=> y.name}.each do |outer_module|
	constants = @constants[outer_module]
	constants_rep = constants.sort.inspect
	port.puts("#{outer_module}:: #{constants_rep}")
      end
      port.puts("*** inclusion ***")
      @include_rev.keys.sort{|x, y| x.name <=> y.name}.each do |included_module|
	including_modules = @include_rev[included_module]
	including_modules_rep =
	  including_modules.sort{|x, y| x.name <=> y.name}.inspect
	port.puts("#{included_module} > #{including_modules_rep}")
      end
      port.puts("*** inheritance ***")
      @inherit_rev.keys.sort{|x, y| x.name <=> y.name}.each do |super_class|
	sub_classes = @inherit_rev[super_class]
	sub_classes_rep = sub_classes.sort{|x, y| x.name <=> y.name}.inspect
	port.puts("#{super_class} > #{sub_classes_rep}")
      end
    end

    def __dump_metainfo2(root, port=STDOUT)
      m = root
      port.print("  " * m.outer_name.size)
      port.puts(m.inner_name)
      @dim_rev[m].sort{|x, y| x.name <=> y.name}.each do |im|
	dump2(im, port)
      end
    end

    private
    def __update_metainfo2
      print("  " * level)
      puts("enter: #{root_module}")
      root_module._constants.sort.collect { |constant_name|
	root_module.const_get(constant_name)
      }.each { |constant|
	if constant.is_a?(Module)
	  traverse(constant, level + 1)
	end
      }
      print("  " * level)
      puts("leave: #{root_module}")
    end

  end

  public
#  def modules_defined_in
#    metainfo[0]
#  end

  def true_constants
    Module.__true_constants(self)
  end

  def sub_modules
    Module.__sub_modules(self)
  end
end

=begin
Class#sub_classes
=end
class Class
  def sub_classes
    Module.__sub_classes(self)
  end
end

require 'observer'
require 'gtk2'

=begin

  gtkutil.rb - Gtk Utility

  $Author: mutoh $
  $Date: 2002/06/22 05:26:49 $

  Copyright (C) 2000 Hiroshi Igarashi

  This program is free software.
  You can distribute/modify this program under
  the terms of the Ruby Distribute License.

=end

module GtkExt

  def select_file(title="select file")
    fs = Gtk::FileSelection.new(title)
    fs.set_modal(true)
    filename = nil
    fs.signal_connect("destroy") do
      Gtk.main_quit
    end
    fs.ok_button.signal_connect("clicked") do
      fs.hide
      filename = fs.get_filename
      fs.destroy
    end
    fs.cancel_button.signal_connect("clicked") do
      fs.hide
      fs.destroy
    end
    fs.show_all
    Gtk.main
    filename
  end
  module_function(:select_file)

  def show_message(message)
    dialog = Gtk::Dialog.new
    dialog.set_title("message")
    dialog.set_modal(true)

    label = Gtk::Label.new(message)
    label.jtype = Gtk::JUSTIFY_FILL
    label.set_line_wrap(true)
    label.set_padding(10, 10)
    dialog.vbox.pack_start(label)

    button = Gtk::Button::new("OK")
    button.flags |= Gtk::Widget::CAN_DEFAULT
    dialog.action_area.pack_start(button)
    button.grab_default

    dialog.signal_connect("destroy") do Gtk.main_quit end
    button.signal_connect("clicked") do dialog.destroy end

    dialog.show_all
    Gtk.main
    nil
  end
  module_function(:show_message)

  def show_about(about)
    dialog = Gtk::Dialog.new
    dialog.set_title("about")
    dialog.set_modal(true)

    label = Gtk::Label.new(about)
    label.set_padding(10, 10)
    dialog.vbox.pack_start(label)

    button = Gtk::Button::new("OK")
    button.flags |= Gtk::Widget::CAN_DEFAULT
    dialog.action_area.pack_start(button)
    button.grab_default

    dialog.signal_connect("destroy") do Gtk.main_quit end
    button.signal_connect("clicked") do dialog.destroy end

    dialog.show_all
    Gtk.main
    nil
  end
  module_function(:show_about)

  def input_line(title, orig="")
    result = orig

    dialog = Gtk::Dialog.new
    dialog.set_title(title)
    dialog.set_modal(true)

#    label = Label.new(message)
#    label.jtype = JUSTIFY_FILL
#    label.set_line_wrap(true)
#    label.set_padding(10, 10)
#    dialog.vbox.pack_start(label)
    entry = Gtk::Entry.new
    entry.set_text(orig)
    dialog.vbox.pack_start(entry)

    button = Gtk::Button::new("OK")
    button.flags |= Gtk::Widget::CAN_DEFAULT
    dialog.action_area.pack_start(button)
    button.grab_default

    dialog.signal_connect("destroy") do Gtk.main_quit end
    button.signal_connect("clicked") do
      result = entry.get_text
      dialog.destroy
    end

    dialog.show_all
    Gtk.main
    result
  end
  module_function(:input_line)

end

=begin

  meta/browser.rb - Meta-Level Information Browser

  $Author: mutoh $
  $Date: 2002/06/22 05:26:49 $

  Copyright (C) 2000-2001 Hiroshi Igarashi

  This program is free software.
  You can distribute/modify this program under
  the terms of the Ruby Distribute License.

=end

module RBBR

  class ModuleIndex < Gtk::Tree
    include Observable

    def initialize
      super()
      self.set_selection_mode(Gtk::SELECTION_SINGLE)
      update
    end

    def update
      clear_items(0, 1)
      append_class(self, Object)
      root_modules = Module.root_modules.sort{|x, y| x.name <=> y.name}

      module_treeitem = Gtk::TreeItem.new("<module tree>")
      module_treeitem.show
      self.append(module_treeitem)
      module_treeitem.set_subtree(module_tree = Gtk::Tree.new)
      root_modules.each do |root_module|
	append_module(module_tree, root_module)
      end
    end

    private
    def append_class(tree, klass)
      treeitem = Gtk::TreeItem.new(klass.name)
      treeitem.signal_connect('select', klass) do |widget, klass|
	changed
	notify_observers(klass)
      end
      treeitem.show
      tree.append(treeitem)
      subclasses = klass.sub_classes
      unless subclasses.empty?
	treeitem.set_subtree(subtree = Gtk::Tree.new)
	treeitem.signal_connect("expand") do
	  if subtree.children.empty?
	    subclasses.sort{|x, y| x.name <=> y.name}.each do |subclass|
	      append_class(subtree, subclass)
	    end
	  end
	end
      end
    end

    def append_module(tree, modul)
      treeitem = Gtk::TreeItem.new(modul.name)
      treeitem.signal_connect('select', modul) do |widget, modul|
	changed
	notify_observers(modul)
      end
      treeitem.show
      tree.append(treeitem)
      submodules = modul.sub_modules
      unless submodules.empty?
	treeitem.set_subtree(subtree = Gtk::Tree.new)
	treeitem.signal_connect("expand") do
	  if subtree.children.empty?
	    submodules.sort{|x, y| x.name <=> y.name}.each do |submodule|
	      append_module(subtree, submodule)
	    end
	  end
	end
      end
    end

    public
    def expand_all
      expand_recursive(self)
    end
    def collapse_all
      collapse_recursive(self)
    end

    private
    def expand_recursive(tree)
      tree.each do |treeitem|
	treeitem.expand
	expand_recursive(treeitem.subtree) unless treeitem.subtree.nil?
      end
    end
    def collapse_recursive(tree)
      tree.each do |treeitem|
	treeitem.collapse
	collapse_recursive(treeitem.subtree) unless treeitem.subtree.nil?
      end
    end
  end

  class ModuleLabel < Gtk::Entry
    def initialize
      #super("")
      #self.set_justify(Gtk::JUSTIFY_LEFT)
      super
      self.set_editable(false)
    end

    def update(modul)
      text = 
	if Class === modul
	  "class #{modul.name}"
	else # Module
	  "module #{modul.name}"
	end
      if Class === modul and Object != modul
	text << " < #{modul.superclass.name}"
      end
      included_modules_at = modul.included_modules_at
      unless included_modules_at.empty?
	text << "; include " << included_modules_at.join(", ")
      end
      self.set_text(text)
    end
  end

  class BrowseList < Gtk::CList
    include Observable
    def initialize(labels)
      @modul = nil
      super(labels)
      
      signal_connect('select_row'){|w, r, c, e|
	changed
	notify_observers(@modul, w.get_text(r, 0))
      }
      
      labels.each_index do |i|
	set_column_auto_resize(i, true)
      end
    end

    def update(modul)
      self.freeze
      self.clear
      update_list(modul)
      self.thaw
      @modul = modul
    end
      
    def update_list(modul)
      # do nothing
    end

    def module
      @modul
    end
  end

  class MethodList < BrowseList
    def initialize
      super(["Name", "Arity"])
    end
  end

  class PublicInstanceMethodList < MethodList
    def update_list(modul)
      modul.public_instance_methods.sort.each do |name|
	self.append([name, modul.instance_method(name).arity.to_s])
      end
    end
  end

  class ProtectedInstanceMethodList < MethodList
    def update_list(modul)
      modul.protected_instance_methods.sort.each do |name|
	self.append([name, modul.instance_method(name).arity.to_s])
      end
    end
  end

  class PrivateInstanceMethodList < MethodList
    def update_list(modul)
      modul.private_instance_methods.sort.each do |name|
	self.append([name, modul.instance_method(name).arity.to_s])
      end
    end
  end

  class SingletonMethodList < MethodList
    def update_list(modul)
      modul.singleton_methods.sort.each do |name|
	self.append([name, modul.method(name).arity.to_s])
      end
    end
  end

  class ConstantList < BrowseList
    def initialize
      super(["Name", "Value"])
    end

    def update_list(modul)
      modul.true_constants.sort.each do |name|
	self.append([name, modul.const_get(name).inspect])
      end
    end
  end

  class Browser < Gtk::Window
    def initialize
      super
      
      signal_connect("destroy") do
	quit
      end

      # create vbox
      vbox = Gtk::VBox.new(false, 0)
      add(vbox)

      # create menu
      menubar = Gtk::MenuBar.new
      vbox.pack_start(menubar, false, false, 0)
      # File
      menubar.append(file_item = Gtk::MenuItem.new("File"))
      file_item.set_submenu(file_menu = Gtk::Menu.new)
      file_menu.append(require_library_item = Gtk::MenuItem.new("Require Library..."))
      file_menu.append(load_script_item = Gtk::MenuItem.new("Load Script..."))
      file_menu.append(quit_item = Gtk::MenuItem.new("Quit"))
      # Edit
      menubar.append(edit_item = Gtk::MenuItem.new("Edit"))
      edit_item.set_submenu(edit_menu = Gtk::Menu.new)
      edit_menu.append(copy_item = Gtk::MenuItem.new("Copy"))
      copy_item.set_sensitive(false)
      # View
      menubar.append(view_item = Gtk::MenuItem.new("View"))
      view_item.set_submenu(view_menu = Gtk::Menu.new)
      view_menu.append(expand_all_item = Gtk::MenuItem.new("Expand All"))
      view_menu.append(fold_all_item = Gtk::MenuItem.new("Fold All"))
      view_menu.append(update_info_item = Gtk::MenuItem.new("Update Infomation"))
      view_menu.append(filter_setting_item = Gtk::MenuItem.new("Filter Setting..."))
      filter_setting_item.set_sensitive(false)
      # Help
      menubar.append(help_item = Gtk::MenuItem.new("Help"))
      help_item.right_justify
      help_item.set_submenu(help_menu = Gtk::Menu.new)
      help_menu.append(about_item = Gtk::MenuItem.new("About"))

      # create main box
      main_paned = Gtk::HPaned.new
      vbox.pack_start(main_paned, true, true, 0)

      # create module index
      module_index_scwin = Gtk::ScrolledWindow.new
      module_index_scwin.set_policy(Gtk::POLICY_AUTOMATIC,
				    Gtk::POLICY_AUTOMATIC)
      main_paned.add1(module_index_scwin)
      module_index = ModuleIndex.new
      module_index_scwin.add_with_viewport(module_index)

      # create display box
      display_box = Gtk::VBox.new(false, 0)
      main_paned.add2(display_box)

      # create module label
      module_label = ModuleLabel.new
      module_index.add_observer(module_label)
      display_box.pack_start(module_label, false, true, 0)

      # create display option menu
      display_option_menu = Gtk::OptionMenu.new
      display_box.pack_start(display_option_menu, false, false, 0)
      display_option_menu.set_menu(display_menu = Gtk::Menu.new)

      # create notebook
      notebook = Gtk::Notebook.new
      notebook.set_tab_pos(Gtk::POS_TOP)
      notebook.set_homogeneous_tabs(true)
      notebook.set_show_tabs(false)
      display_box.pack_start(notebook, true, true, 0)

      # create lists
      singleton_method_list = SingletonMethodList.new
      public_instance_method_list = PublicInstanceMethodList.new
      protected_instance_method_list = ProtectedInstanceMethodList.new
      private_instance_method_list = PrivateInstanceMethodList.new
      constant_list = ConstantList.new
      [ 
	[public_instance_method_list, "Public Instance Methods"],
	[protected_instance_method_list, "Protected Instance Methods"],
	[private_instance_method_list, "Private Instance Methods"],
	[singleton_method_list, "Singleton Methods"],
	[constant_list, "Constants"],
      ].each_with_index do |(list, label), index|
	scwin = Gtk::ScrolledWindow.new
	scwin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC)
	scwin.add(list)
	display_menu.append(menu_item = Gtk::MenuItem.new(label))
	menu_item.signal_connect("activate") do
	  notebook.set_page(index)
	end
	notebook.append_page(scwin, Gtk::Label.new(label))
	notebook.set_tab_label_packing(scwin, true, true, Gtk::PACK_START)
	module_index.add_observer(list)
      end
      display_option_menu.set_history(0)

      # setting size
      self.set_default_size(640, 480)
      module_index_scwin.set_usize(200, 480)

      # menu binding
      require_library_item.signal_connect("activate") do
	feature = GtkExt.input_line("load library").strip
	begin
	  unless feature.empty?
	    require(feature) 
	    Module.update_metainfo
	    module_index.update
	  end
	rescue LoadError
	  show_message($!)
	end
      end
      load_script_item.signal_connect("activate") do
	filename = GtkExt.select_file
	begin
	  load(filename) unless filename.nil?
	  Module.update_metainfo
	  module_index.update
	rescue LoadError
	  show_message($!)
	end
      end
      quit_item.signal_connect("activate") do
	quit
      end
      copy_item.signal_connect("activate") do
	not_implemented
      end
      expand_all_item.signal_connect("activate") do
	module_index.expand_all
      end
      fold_all_item.signal_connect("activate") do
	module_index.collapse_all
      end
      filter_setting_item.signal_connect("activate") do
	not_implemented
      end
      update_info_item.signal_connect("activate") do
	Module.update_metainfo
	module_index.update
      end
      about_item.signal_connect("activate") do
	show_about
      end
    end

    def quit
      Gtk.main_quit
    end

    def show_message(message)
      dialog = Gtk::Dialog.new
      dialog.set_title("Ruby Browser: message")
      dialog.set_modal(true)

      label = Gtk::Label.new(message)
      label.jtype = Gtk::JUSTIFY_FILL
      label.set_line_wrap(true)
      label.set_padding(10, 10)
      dialog.vbox.pack_start(label)

      button = Gtk::Button::new("OK")
      button.flags |= Gtk::Widget::CAN_DEFAULT
      dialog.action_area.pack_start(button)
      button.grab_default

      dialog.signal_connect("destroy") do Gtk.main_quit end
      button.signal_connect("clicked") do dialog.destroy end

      dialog.show_all
      Gtk.main
      nil
    end

    def not_implemented
      show_message("Sorry, not implemented.")
    end

    def show_about
      require 'rbconfig'
      about = "Ruby Browser\nCopyright (C) 2000-2001 Hiroshi Igarashi"

      dialog = Gtk::Dialog.new
      dialog.set_title("Ruby Browser: about")
      dialog.set_modal(true)
      dialog.set_usize(320, 320)

      label = Gtk::Label.new(about)
      label.set_padding(10, 10)
      dialog.vbox.pack_start(label, false, false, 0)

      scwin = Gtk::ScrolledWindow.new
      dialog.vbox.pack_start(scwin, true, true, 8)
      titles = ["item", "value"]
      list = Gtk::CList.new(titles)
      titles.each_with_index do |title, i|
	list.set_column_resizeable(i, true)
	list.set_column_auto_resize(i, true)
      end
      Config::CONFIG.keys.sort.each do |k|
	list.append([k, Config::CONFIG[k].inspect])
      end
      scwin.add(list)

      button = Gtk::Button::new("OK")
      button.flags |= Gtk::Widget::CAN_DEFAULT
      dialog.action_area.pack_start(button)
      button.grab_default

      dialog.signal_connect("destroy") do Gtk.main_quit end
      button.signal_connect("clicked") do dialog.destroy end

      dialog.show_all
      Gtk.main
      nil
    end

  end

  ## Document Viewer by ReFe
  class DocViewer < Gtk::Text
    def update(modul, meth)
    end
  end

  class RefeDocViewer < DocViewer
    def initialize
      require 'rbconfig'
      @database = ReFe::Database.load(File.join(Config::CONFIG['datadir'], ReFe::Database::INDEX_FILE))

      super
      @font = {}
      @font[:header] = Gdk::Font.fontset_load('-b&h-lucidatypewriter-bold-r-normal-*-18-*-*-*-m-*-iso8859-1,*')
      @font[:monospace] = Gdk::Font.fontset_load('-b&h-lucidatypewriter-medium-r-normal-*-12-*-*-*-m-*-iso8859-1,*')
    end


    def update(modul, meth)
      freeze
      delete_text(0, -1)
      begin
	result = @database.lookup(modul.to_s, meth, nil)

	insert(@font[:header], nil, nil, "#{result.name}\n")
	insert(nil, nil, nil, result.description)
      rescue ReFe::SearchError
	;
      ensure
	thaw
      end
    end
  end

  ## Document Viewer by ri
  class RIDocViewer < DocViewer
    def initialize
      @ri = RI.new
      @ri.setOutputFormatter(self)

      super
      set_word_wrap(true)

      @font = {}
      @font[:header] = Gdk::Font.fontset_load('-b&h-lucidatypewriter-bold-r-normal-*-18-*-*-*-m-*-iso8859-1,*')
      @font[:monospace] = Gdk::Font.fontset_load('-b&h-lucidatypewriter-medium-r-normal-*-12-*-*-*-m-*-iso8859-1,*')
    end
    
    def update(modul, meth)
      freeze
      delete_text(0, -1)
      catch(:exit) do
	@ri.describeMethod(modul.name, '.', meth)
      end
      thaw
    end

    def putMethodList(names)
      names.sort.join(",")
    end
    
    def putVerbatim(s)
      write("#{s.gsub(/^/, '  ')}\n", @font[:monospace])
    end

    def putParagraph(s)
      write(stripFormatting(s))
    end

    def putMessage(s)
      write(stripFormatting(s))
    end
    
    def newline
      write("\n")
    end

    def putMethodHeader(cname, type, mname, callseq)
      write("#{cname}#{type}#{mname}\n\n", @font[:header])
      write("#{stripFormatting(callseq).gsub(/^/, '  ')}\n\n",
	    @font[:monospace])
    end
    
    def putClassHeader(type, name, superClass, subclasses)
      sub = ""
      sub = subclasses.join(',') if subclasses
      write("#{type}|#{name}|#{superClass}|#{sub}|")
    end

    def putClassMethods(names)
      putMethodList(names)
    end
    
    # These are methods that wrap other calls    
    def putFragmentBlock
      yield
    end
    
    def putMethodDescription
      yield
    end
    
    def putClassDescription
      yield
    end
    
    def putListOfClassesMatchingName(name)
      yield
    end
    
    def putListOfMethodsMatchingName(name)
      yield
    end
    
    def error(message)
    end

    def write(s, font = nil)
      insert(font, nil, nil, s)
    end
  end

  ## Class Browser with Document Viewer
  class BrowserWithDocumentViewer < Gtk::Window
    def initialize      
      super
      
      signal_connect("destroy") do
	quit
      end
      
      # create vbox
      vbox = Gtk::VBox.new(false, 0)
      add(vbox)

      # create menu
      menubar = Gtk::MenuBar.new
      vbox.pack_start(menubar, false, false, 0)
      # File
      menubar.append(file_item = Gtk::MenuItem.new("File"))
      file_item.set_submenu(file_menu = Gtk::Menu.new)
      file_menu.append(require_library_item = Gtk::MenuItem.new("Require Library..."))
      file_menu.append(load_script_item = Gtk::MenuItem.new("Load Script..."))
      file_menu.append(quit_item = Gtk::MenuItem.new("Quit"))
      # Edit
      menubar.append(edit_item = Gtk::MenuItem.new("Edit"))
      edit_item.set_submenu(edit_menu = Gtk::Menu.new)
      edit_menu.append(copy_item = Gtk::MenuItem.new("Copy"))
      copy_item.set_sensitive(false)
      # View
      menubar.append(view_item = Gtk::MenuItem.new("View"))
      view_item.set_submenu(view_menu = Gtk::Menu.new)
      view_menu.append(expand_all_item = Gtk::MenuItem.new("Expand All"))
      view_menu.append(fold_all_item = Gtk::MenuItem.new("Fold All"))
      view_menu.append(update_info_item = Gtk::MenuItem.new("Update Infomation"))
      view_menu.append(filter_setting_item = Gtk::MenuItem.new("Filter Setting..."))
      filter_setting_item.set_sensitive(false)
      # Help
      menubar.append(help_item = Gtk::MenuItem.new("Help"))
      help_item.right_justify
      help_item.set_submenu(help_menu = Gtk::Menu.new)
      help_menu.append(about_item = Gtk::MenuItem.new("About"))

      # create main box
      main_paned = Gtk::HPaned.new
      vbox.pack_start(main_paned, true, true, 0)

      # create module index
      module_index_scwin = Gtk::ScrolledWindow.new
      module_index_scwin.set_policy(Gtk::POLICY_AUTOMATIC,
				    Gtk::POLICY_AUTOMATIC)
      main_paned.add1(module_index_scwin)
      module_index = ModuleIndex.new
      module_index_scwin.add_with_viewport(module_index)

      # create document viewer
      docviewer = nil
      [
	['librefe', RefeDocViewer],
	['ri/ri', RIDocViewer],
      ].each do |path, klass|
	begin
	  require path
	  docviewer = klass.new
	  break
	rescue LoadError
	end
      end
	
      # create display box
      display_box = Gtk::VBox.new(false, 0)
      if(docviewer)
	# create display paned
	display_paned = Gtk::VPaned.new
	main_paned.add2(display_paned)
	display_paned.add1(display_box)

	docviewer_scwin = Gtk::ScrolledWindow.new 
	docviewer_scwin.set_policy(Gtk::POLICY_AUTOMATIC,
				   Gtk::POLICY_AUTOMATIC)
	display_paned.add2(docviewer_scwin)
	docviewer_scwin.add(docviewer)
      else
	main_paned.add2(display_box)
      end

      # create module label
      module_label = ModuleLabel.new
      module_index.add_observer(module_label)
      display_box.pack_start(module_label, false, true, 0)

      # create display option menu
      display_option_menu = Gtk::OptionMenu.new
      display_box.pack_start(display_option_menu, false, false, 0)
      display_option_menu.set_menu(display_menu = Gtk::Menu.new)

      # create notebook
      notebook = Gtk::Notebook.new
      notebook.set_tab_pos(Gtk::POS_TOP)
      notebook.set_homogeneous_tabs(true)
      notebook.set_show_tabs(false)
      display_box.pack_start(notebook, true, true, 0)

      # create lists
      singleton_method_list = SingletonMethodList.new
      public_instance_method_list = PublicInstanceMethodList.new
      protected_instance_method_list = ProtectedInstanceMethodList.new
      private_instance_method_list = PrivateInstanceMethodList.new
      constant_list = ConstantList.new
      [ 
	[public_instance_method_list, "Public Instance Methods"],
	[protected_instance_method_list, "Protected Instance Methods"],
	[private_instance_method_list, "Private Instance Methods"],
	[singleton_method_list, "Singleton Methods"],
	[constant_list, "Constants"],
      ].each_with_index do |(list, label), index|
	scwin = Gtk::ScrolledWindow.new
	scwin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC)
	scwin.add(list)
	display_menu.append(menu_item = Gtk::MenuItem.new(label))
	menu_item.signal_connect("activate") do
	  notebook.set_page(index)
	end
	notebook.append_page(scwin, Gtk::Label.new(label))
	notebook.set_tab_label_packing(scwin, true, true, Gtk::PACK_START)
	module_index.add_observer(list)
	
	if(docviewer)
	  list.add_observer(docviewer)
	end
      end
      display_option_menu.set_history(0)

      # setting size
      self.set_default_size(640, 480)
      module_index_scwin.set_usize(200, 480)

      # menu binding
      require_library_item.signal_connect("activate") do
	feature = GtkExt.input_line("load library").strip
	begin
	  unless feature.empty?
	    require(feature) 
	    Module.update_metainfo
	    module_index.update
	  end
	rescue LoadError
	  show_message($!)
	end
      end
      load_script_item.signal_connect("activate") do
	filename = GtkExt.select_file
	begin
	  load(filename) unless filename.nil?
	  Module.update_metainfo
	  module_index.update
	rescue LoadError
	  show_message($!)
	end
      end
      quit_item.signal_connect("activate") do
	quit
      end
      copy_item.signal_connect("activate") do
	not_implemented
      end
      expand_all_item.signal_connect("activate") do
	module_index.expand_all
      end
      fold_all_item.signal_connect("activate") do
	module_index.collapse_all
      end
      filter_setting_item.signal_connect("activate") do
	not_implemented
      end
      update_info_item.signal_connect("activate") do
	Module.update_metainfo
	module_index.update
      end
      about_item.signal_connect("activate") do
	show_about
      end
    end

    def quit
      Gtk.main_quit
    end

    def show_message(message)
      dialog = Gtk::Dialog.new
      dialog.set_title("Ruby Browser: message")
      dialog.set_modal(true)

      label = Gtk::Label.new(message)
      label.jtype = Gtk::JUSTIFY_FILL
      label.set_line_wrap(true)
      label.set_padding(10, 10)
      dialog.vbox.pack_start(label)

      button = Gtk::Button::new("OK")
      button.flags |= Gtk::Widget::CAN_DEFAULT
      dialog.action_area.pack_start(button)
      button.grab_default

      dialog.signal_connect("destroy") do Gtk.main_quit end
      button.signal_connect("clicked") do dialog.destroy end

      dialog.show_all
      Gtk.main
      nil
    end

    def not_implemented
      show_message("Sorry, not implemented.")
    end

    def show_about
      require 'rbconfig'
      about = "Ruby Browser\nCopyright (C) 2000-2001 Hiroshi Igarashi"
      about += "\nri and ReFe support by kadu\n"

      dialog = Gtk::Dialog.new
      dialog.set_title("Ruby Browser: about")
      dialog.set_modal(true)
      dialog.set_usize(320, 320)

      label = Gtk::Label.new(about)
      label.set_padding(10, 10)
      dialog.vbox.pack_start(label, false, false, 0)

      scwin = Gtk::ScrolledWindow.new
      dialog.vbox.pack_start(scwin, true, true, 8)
      titles = ["item", "value"]
      list = Gtk::CList.new(titles)
      titles.each_with_index do |title, i|
	list.set_column_resizeable(i, true)
	list.set_column_auto_resize(i, true)
      end
      Config::CONFIG.keys.sort.each do |k|
	list.append([k, Config::CONFIG[k].inspect])
      end
      scwin.add(list)

      button = Gtk::Button::new("OK")
      button.flags |= Gtk::Widget::CAN_DEFAULT
      dialog.action_area.pack_start(button)
      button.grab_default

      dialog.signal_connect("destroy") do Gtk.main_quit end
      button.signal_connect("clicked") do dialog.destroy end

      dialog.show_all
      Gtk.main
      nil
    end

  end

  def self.main
    window = BrowserWithDocumentViewer.new
    window.set_title("Ruby Browser + Document Viewer")
    window.show_all
    Gtk.main
  end

end

if $0 == __FILE__
  RBBR.main
end
