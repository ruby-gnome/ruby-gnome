# -*- indent-tabs-mode: nil -*-
# $Id: gnome-app-helper.rb,v 1.3 2002/10/17 14:33:27 tkubo Exp $
#
# gnome-app-helper.rb - sample script of Gnome::App#create_menus()
#                       and Gnome::App#toolbar().
#
# Copyright (C) 2002 KUBO Takehiro <kubo@jiubao.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.
#
require 'gnome2'

program = Gnome::Program.new('test-app-helper', '0.1', Gnome::ModuleInfo::LIBGNOMEUI)

class SampleApp < Gnome::App

  include Gnome::I18n

  APP_NAME = 'test-app-helper'
  TITLE = 'App Helper Sample'

  def initialize
    super(APP_NAME, TITLE)

    self.set_default_size(200, 200)
    self.signal_connect("delete_event") do 
      Gtk::main_quit()
    end

    bar = Gnome::AppBar.new(false, true, Gnome::PREFERENCES_USER)
    self.statusbar = bar

    @label = Gtk::Label.new(TITLE)
    self.set_contents(@label)

    callback = proc { |item, arg| @label.text = arg }

    file_menu = [
      Gnome::UIInfo::menu_new_item(N_('_New'), 'Create a new file', callback, 'New'),
#      Gnome::UIInfo::menu_new_subtree(new_submenu),
      Gnome::UIInfo::menu_open_item(callback, 'Open'),
      Gnome::UIInfo::menu_save_item(callback, 'Save'),
      Gnome::UIInfo::menu_save_as_item(callback, 'Save As'),
      Gnome::UIInfo::menu_revert_item(callback, 'Revert'),
      Gnome::UIInfo::menu_print_item(callback, 'Print'),
      Gnome::UIInfo::menu_print_setup_item(callback, 'Print Setup'),
      Gnome::UIInfo::menu_close_item(callback, 'Close'),
      Gnome::UIInfo::menu_quit_item(callback, 'Quit'),
    ]

    edit_menu = [
      Gnome::UIInfo::menu_cut_item(callback, 'Cut'),
      Gnome::UIInfo::menu_copy_item(callback, 'Copy'),
      Gnome::UIInfo::menu_paste_item(callback, 'Paste'),
      Gnome::UIInfo::menu_select_all_item(callback, 'Select All'),
      Gnome::UIInfo::menu_clear_item(callback, 'Clear'),
      Gnome::UIInfo::menu_undo_item(callback, 'Undo'),
      Gnome::UIInfo::menu_redo_item(callback, 'Redo'),
      Gnome::UIInfo::menu_find_item(callback, 'Find'),
      Gnome::UIInfo::menu_find_again_item(callback, 'Find Again'),
      Gnome::UIInfo::menu_replace_item(callback, 'Replace'),
      Gnome::UIInfo::menu_properties_item(callback, 'Properties'),
    ]

    view_menu = [
    ]

    settings_menu = [
      Gnome::UIInfo::menu_preferences_item(callback, 'Preferences'),
    ]

    files_menu = [
    ]

    windows_menu = [
      Gnome::UIInfo::menu_new_window_item(callback, 'New Window'),
      Gnome::UIInfo::menu_close_window_item(callback, 'Close Window'),
    ]

    help_menu = [
      Gnome::UIInfo::menu_about_item(callback, 'About'),
    ]

    game_menu = [
      Gnome::UIInfo::menu_pause_game_item(callback, 'Pause Game'),
      Gnome::UIInfo::menu_restart_game_item(callback, 'Restart Game'),
      Gnome::UIInfo::menu_undo_move_item(callback, 'Undo Move'),
      Gnome::UIInfo::menu_redo_move_item(callback, 'Redo Move'),
      Gnome::UIInfo::menu_hint_item(callback, 'Hint'),
      Gnome::UIInfo::menu_scores_item(callback, 'Scores'),
      Gnome::UIInfo::menu_end_game_item(callback, 'End Game'),
    ]

    main_menu = [
      Gnome::UIInfo::menu_file_tree(file_menu),
      Gnome::UIInfo::menu_edit_tree(edit_menu),
      Gnome::UIInfo::menu_view_tree(view_menu),
      Gnome::UIInfo::menu_settings_tree(settings_menu),
      Gnome::UIInfo::menu_files_tree(files_menu),
      Gnome::UIInfo::menu_windows_tree(windows_menu),
      Gnome::UIInfo::menu_help_tree(help_menu),
      Gnome::UIInfo::menu_game_tree(game_menu),
    ]
    
    radio_toolbar = [
      [Gnome::App::UI_ITEM, "Red", "Red Book", callback, 'toolbar/red',
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::BOOK_RED, 0, 0, nil],
      [Gnome::App::UI_ITEM, "Green", "Green Book", callback, 'toolbar/green',
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::BOOK_GREEN, 0, 0, nil],
      [Gnome::App::UI_ITEM, "Blue", "Blue Book", callback, 'toolbar/blue',
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::BOOK_BLUE, 0, 0, nil],
      [Gnome::App::UI_ITEM, "Yellow", "Yellow Book", callback, 'toolbar/yellow',
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::BOOK_YELLOW, 0, 0, nil],
    ]

    main_toolbar = [
      [Gnome::App::UI_ITEM, "mail", "Mail", callback, 'toolbar/mail',
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::MAIL, 0, 0, nil],
      [Gnome::App::UI_ITEM, "receive", "Mail receive", callback, 'toolbar/mail_receive',
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::MAIL_RCV, 0, 0, nil],
      [Gnome::App::UI_ITEM, "send", "Mail Send", callback, 'toolbar/mail_send',
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::MAIL_SND, 0, 0, nil],
      [Gnome::App::UI_ITEM, "reply", "Mail Reply", callback, 'toolbar/mail_reply',
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::MAIL_RPL, 0, 0, nil],
      [Gnome::App::UI_ITEM, "foward", "Mail Forward", callback, 'toolbar/mail_foward',
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::MAIL_FWD, 0, 0, nil],
      [Gnome::App::UI_ITEM, "new", "Mail New", callback, 'toolbar/mail_new',
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::MAIL_NEW, 0, 0, nil],
      [Gnome::App::UI_ITEM, "attach", "Attach", callback, 'toolbar/attach',
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::ATTACH, 0, 0, nil],
      Gnome::UIInfo::separator,
      Gnome::UIInfo::radiolist(radio_toolbar)
    ]

    menu_info = self.create_menus(main_menu)
    bar.install_menu_hints(menu_info)
    self.create_toolbar(main_toolbar)
  end
end

app = SampleApp.new()
app.show_all()

Gtk::main
