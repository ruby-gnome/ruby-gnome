=begin header

  menu.rb - a part of testgtk.c rewritten in ruby-gtk

=end

require 'sample'

class MenuSample < SampleWindow
  def initialize
    super("menus")
    signal_connect("delete_event") do true end

    box1 = Gtk::VBox::new(false, 0)
    add(box1)
    box1.show

    menubar = Gtk::MenuBar::new()
    box1.pack_start(menubar, false, true, 0)
    menubar.show

    menu = create_menu(2, true)

    menuitem = Gtk::MenuItem::new("test\nline2")
    menuitem.set_submenu(menu)
    menubar.append(menuitem)
    menuitem.show
    
    menuitem = Gtk::MenuItem::new("foo")
    menuitem.set_submenu(create_menu(3, true))
    menubar.append(menuitem)
    menuitem.show

    menuitem = Gtk::MenuItem::new("bar")
    menuitem.set_submenu(create_menu(4, true))
    menuitem.right_justify
    menubar.append(menuitem)
    menuitem.show

    box2 = Gtk::VBox::new(false, 10)
    box2.border_width(10)
    box1.pack_start(box2, true, true, 0)
    box2.show

    optionmenu = Gtk::OptionMenu::new()
    optionmenu.set_menu(create_menu(1, false))
    optionmenu.set_history(4)
    box2.pack_start(optionmenu, true, true, 0)
    optionmenu.show

    separator = Gtk::HSeparator::new()
    box1.pack_start(separator, false, true, 0)
    separator.show

    box2 = Gtk::VBox::new(false, 10)
    box2.border_width(10)
    box1.pack_start(box2, false, true, 0)
    box2.show

    button = Gtk::Button::new("close")
    button.signal_connect("clicked") do destroy end
    box2.pack_start(button, true, true, 0)
    button.set_flags(Gtk::Widget::CAN_DEFAULT)
    button.grab_default
    button.show
  end
  private
  def create_menu(depth, tearoff)
    return nil if depth < 1

    menu = Gtk::Menu::new()
    group = nil

    if tearoff then
      menuitem = Gtk::TearoffMenuItem::new()
      menu.append(menuitem)
      menuitem.show
    end

    for i in 0..4
      j = i + 1
      buf = sprintf("item %2d - %d", depth, j)
      menuitem = Gtk::RadioMenuItem::new(group, buf)
      group = menuitem.group

      menuitem.set_show_toggle(true) if depth % 2 != 0
      menu.append(menuitem)
      menuitem.show
      menuitem.set_sensitive(false) if i == 3
      submenu = create_menu(depth - 1, true)
      menuitem.set_submenu(submenu) unless submenu.nil?
    end

    menu
  end
end
