require 'gnome2'

Gnome::Program.new('test-app-helper', '0.1', Gnome::ModuleInfo::LIBGNOMEUI)

class SampleApp < Gnome::App

  APP_NAME = 'test-app-helper'
  TITLE = 'App Helper Sample'

  include Gnome::I18n

  def initialize
    super(APP_NAME, TITLE)

    file_menu = [
      Gnome::UIInfo::menu_new_item(N_('_New'), 'tooltip', proc{ puts 'Open' }, nil),
      Gnome::UIInfo::menu_new_subtree([]),
      Gnome::UIInfo::menu_open_item(proc{ puts 'Open' }, nil),
      Gnome::UIInfo::menu_save_item(proc{ puts 'Save' }, nil),
      Gnome::UIInfo::menu_save_as_item(proc{ puts 'Save As' }, nil),
      Gnome::UIInfo::menu_revert_item(proc{ puts 'Revert' }, nil),
      Gnome::UIInfo::menu_print_item(proc{ puts 'Print' }, nil),
      Gnome::UIInfo::menu_print_setup_item(proc{ puts 'Print Setup' }, nil),
      Gnome::UIInfo::menu_close_item(proc{ puts 'Close' }, nil),
      Gnome::UIInfo::menu_quit_item(proc{ puts 'Quit' }, nil),
    ]

    edit_menu = [
      Gnome::UIInfo::menu_cut_item(proc{ puts 'Cut' }, nil),
      Gnome::UIInfo::menu_copy_item(proc{ puts 'Copy' }, nil),
      Gnome::UIInfo::menu_paste_item(proc{ puts 'Paste' }, nil),
      Gnome::UIInfo::menu_select_all_item(proc{ puts 'Select All' }, nil),
      Gnome::UIInfo::menu_clear_item(proc{ puts 'Clear' }, nil),
      Gnome::UIInfo::menu_undo_item(proc{ puts 'Undo' }, nil),
      Gnome::UIInfo::menu_redo_item(proc{ puts 'Redo' }, nil),
      Gnome::UIInfo::menu_find_item(proc{ puts 'Fing' }, nil),
      Gnome::UIInfo::menu_find_again_item(proc{ puts 'Find Again' }, nil),
      Gnome::UIInfo::menu_replace_item(proc{ puts 'Replace' }, nil),
      Gnome::UIInfo::menu_properties_item(proc{ puts 'Properties' }, nil),
    ]

    view_menu = [
    ]

    settings_menu = [
      Gnome::UIInfo::menu_preferences_item(proc{ puts 'Preferences' }, nil),
    ]

    files_menu = [
    ]

    windows_menu = [
      Gnome::UIInfo::menu_new_window_item(proc{ puts 'New Window' }, nil),
      Gnome::UIInfo::menu_close_window_item(proc{ puts 'Close Window' }, nil),
    ]

    help_menu = [
      Gnome::UIInfo::menu_about_item(proc{ puts 'About' }, nil),
    ]

    game_menu = [
      Gnome::UIInfo::menu_pause_game_item(proc{ puts 'Pause Game' }, nil),
      Gnome::UIInfo::menu_restart_game_item(proc{ puts 'Restart Game' }, nil),
      Gnome::UIInfo::menu_undo_move_item(proc{ puts 'Undo Move' }, nil),
      Gnome::UIInfo::menu_redo_move_item(proc{ puts 'Redo Move' }, nil),
      Gnome::UIInfo::menu_hint_item(proc{ puts 'Hint' }, nil),
      Gnome::UIInfo::menu_scores_item(proc{ puts 'Scores' }, nil),
      Gnome::UIInfo::menu_end_game_item(proc{ puts 'End Game' }, nil),
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

    self.create_menus(main_menu)

    self.set_default_size(200, 200)
    self.set_contents(Gtk::Label.new(TITLE))
    self.signal_connect("delete_event") do 
      Gtk::main_quit()
    end
  end
end

app = SampleApp.new()
app.show_all()

Gtk::main
