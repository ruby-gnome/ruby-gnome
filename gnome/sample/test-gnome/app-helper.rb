require 'sample'

class AppHelperSample < Gnome::App

  # Menu definitions for the GnomeAppHelper test
  FILE_MENU = [
    [ Gnome::App::UI_ITEM, "_New", "Create a new file",
      proc{ item_activated }, "file/new",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::STOCK_MENU_NEW,
      ?n, Gdk::CONTROL_MASK, nil ],
    [ Gnome::App::UI_ITEM, "_Open...", "Open an existing file",
      proc{ item_activated }, "file/open",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::MENU_OPEN,
      ?o, Gdk::CONTROL_MASK, nil ],
    [ Gnome::App::UI_ITEM, "_Save", "Save the current file",
      proc{ item_activated }, "file/save",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::MENU_SAVE,
      ?s, Gdk::CONTROL_MASK, nil ],
    [ Gnome::App::UI_ITEM, "Save _as...",
      "Save the current file with a new name",
      proc { item_activated }, "file/save as",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::MENU_SAVE_AS, 0, 0, nil ],
    Gnome::UIInfo::separator,
    [ Gnome::App::UI_ITEM, "_Print...", "Print the current file",
      proc { item_activated }, "file/print",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::MENU_PRINT,
      ?p, Gdk::CONTROL_MASK, nil ],
    Gnome::UIInfo::separator,
    [ Gnome::App::UI_ITEM, "_Close", "Close the current file",
      proc { item_activated }, "file/close",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::MENU_CLOSE, 0, 0, nil ],
    [ Gnome::App::UI_ITEM, "E_xit", "Exit the program",
      proc { item_activated }, "file/exit",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::MENU_EXIT,
      ?q, Gdk::CONTROL_MASK, nil ]
  ]

  EDIT_MENU = [
    [ Gnome::App::UI_ITEM, "_Undo", "Undo the last operation",
      proc { item_activated }, "edit/undo",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::MENU_UNDO,
      ?z, Gdk::CONTROL_MASK, nil ],
    [ Gnome::App::UI_ITEM, "_Redo", "Redo the last undo-ed operation",
      proc { item_activated }, "edit/redo",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::MENU_REDO, 0, 0, nil ],
    Gnome::UIInfo::separator,
    [ Gnome::App::UI_ITEM, "Cu_t", "Cut the selection to the clipboard",
      proc { item_activated }, "edit/cut",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::MENU_CUT,
      ?x, Gdk::CONTROL_MASK, nil ],
    [ Gnome::App::UI_ITEM, "_Copy", "Copy the selection to the clipboard",
      proc { item_activated }, "edit/copy",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::MENU_COPY,
      ?c, Gdk::CONTROL_MASK, nil ],
    [ Gnome::App::UI_ITEM, "_Paste", "Paste the contents of the clipboard",
      proc { item_activated }, "edit/paste",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::MENU_PASTE,
      ?v, Gdk::CONTROL_MASK, NULL ]
  ]

  STYLE_RADIO_ITEMS = [
    [ Gnome::App::UI_ITEM, "_10 points", nil,
      proc { item_activated }, "style/10 points",
      Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
    [ Gnome::App::UI_ITEM, "_20 points", nil,
      proc { item_activated }, "style/20 points",
      Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
    [ Gnome::App::UI_ITEM, "_30 points", nil,
      proc { item_activated }, "style/30 points",
      Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
    [ Gnome::App::UI_ITEM, "_40 points", nil,
      proc { item_activated }, "style/40 points",
      Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ]
  ]

  STYLE_MENU = {
    [ Gnome::App::UI_TOGGLEITEM, "_Bold", "Make the selection bold",
      proc { item_activated }, "style/bold",
      Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
    [ Gnome::App::UI_TOGGLEITEM, "_Italic", "Make the selection italic",
      proc { item_activated }, "style/bold", nil,
      Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
    Gnome::UIInfo::separator,
    Gnome::UIInfo::radiolist(STYLE_RATIO_ITEMS)
  ]

  HELP_MENU = [
    [ Gnome::App::UI_ITEM, "_About...",
      "Displays information about the program",
      proc { item_activated }, "help/about",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::MENU_ABOUT, 0, 0, nil ]
  ]

  MAIN_MENU = [
    [ Gnome::App::UI_SUBTREE, "_File", "File operations",
      FILE_MENU, nil,
      Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
    [ Gnome::App::UI_SUBTREE, "_Edit", "Editing commands",
      EDIT_MENU, nil,
      Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
    [ Gnome::App::UI_SUBTREE, "_Style", "Style settings",
      STYLE_MENU, nil,
      Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
    [ Gnome::App::UI_SUBTREE, "_Help", "Help on the program",
      HELP_MENU, nil,
      Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ]
  ]

  # Toolbar definition for the GnomeAppHelper test

  TOOLBAR_RADIO_ITEMS = [
    [ Gnome::App::UI_ITEM, "Red", "Set red color",
      proc { item_activated }, "toolbar/red",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::PIXMAP_BOOK_RED, 0, 0, ni; ],
    [ Gnome::App::UI_ITEM, "Green", "Set green color",
      proc { item_activated }, "toolbar/green",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::PIXMAP_BOOK_GREEN, 0, 0, nil ],
    [ Gnome::App::UI_ITEM, "Blue", "Set blue color",
      proc { item_activated }, "toolbar/blue",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::PIXMAP_BOOK_BLUE, 0, 0, nil ],
    [ Gnome::App::UI_ITEM, "Yellow", "Set yellow color",
      proc { item_activated }, "toolbar/yellow",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::PIXMAP_BOOK_YELLOW, 0, 0, nil ]
  ]

  TOOLBAR = [
    [ Gnome::App::UI_ITEM, "New", "Create a new file",
      proc { item_activated }, "toolbar/new",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::PIXMAP_NEW, 0, 0, nil ],
    [ Gnome::App::UI_ITEM, "Open", "Open an existing file",
      proc { item_activated }, "toolbar/open",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::PIXMAP_OPEN, 0, 0, nil ],
    [ Gnome::App::UI_ITEM, "Save", "Save the current file",
      proc { item_activated }, "toolbar/save",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::PIXMAP_SAVE, 0, 0, nil ],
    [ Gnome::App::UI_ITEM, "Print", "Print the current file",
      proc { item_activated }, "toolbar/print",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::PIXMAP_PRINT, 0, 0, nil ],
    Gnome::UIInfo::separator,
    [ Gnome::App::UI_ITEM, "Undo", "Undo the last operation",
      proc { item_activated }, "toolbar/undo",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::PIXMAP_UNDO, 0, 0, nil ],
    [ Gnome::App::UI_ITEM, "Redo", "Redo the last undo-ed operation",
      proc { item_activated }, "toolbar/redo",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::PIXMAP_REDO, 0, 0, nil ],
    Gnome::UIInfo::separator,
    [ Gnome::App::UI_ITEM, "Cut", "Cut the selection to the clipboard",
      proc { item_activated }, "toolbar/cut",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::PIXMAP_CUT, 0, 0, nil ],
    [ Gnome::App::UI_ITEM, "Copy", "Copy the selection to the clipboard",
      item_activated, "toolbar/copy",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::PIXMAP_COPY, 0, 0, nil ],
    [ Gnome::App::UI_ITEM, "Paste", "Paste the contents of the clipboard",
      item_activated, "toolbar/paste",
      Gnome::App::PIXMAP_STOCK, Gnome::Stock::PIXMAP_PASTE, 0, 0, nil ],
    Gnome::UIInfo::separator,
    Gnome::UIInfo::radiolist(TOOLBAR_RADIO_ITEMS)
  ]

  def initialize
    super("testGNOME", "GnomeAppHelper test");
    create_menus(MAIN_MENU)
    create_toolbar(TOOLBAR)

    bar = Gnome::AppBar.new(false, true, Gnome::PREFERENCES_USER);
    set_statusbar(bar)

    install_appbar_menu_hints(MAIN_MENU)

    vbox = Gtk::VBox.new(false, Gnome::PAD_SMALL)
    vbox.border_width = Gnome::PAD_SMALL

    # Shared popup menu

    popup = Gnome::PopupMenu.new(helper_shared_popup);

    frame = gtk_frame_new ("Shared popup menu");
	gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 0);
	gtk_widget_show (frame);

	vbox2 = gtk_vbox_new (FALSE, GNOME_PAD_SMALL);
	gtk_container_set_border_width (GTK_CONTAINER (vbox2), GNOME_PAD_SMALL);
	gtk_container_add (GTK_CONTAINER (frame), vbox2);
	gtk_widget_show (vbox2);

	w = gtk_entry_new ();
	gtk_box_pack_start (GTK_BOX (vbox2), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	gnome_popup_menu_attach (popup, w, w);

	w = gtk_entry_new ();
	gtk_box_pack_start (GTK_BOX (vbox2), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	gnome_popup_menu_attach (popup, w, w);

	/* Popup menu explicitly popped */

	popup = gnome_popup_menu_new (helper_explicit_popup);

	frame = gtk_frame_new ("Explicit popup menu");
	gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 0);
	gtk_widget_show (frame);

	w = gnome_canvas_new ();
	gtk_widget_set_usize ((w), 200, 100);
	gnome_canvas_set_scroll_region (GNOME_CANVAS (w), 0.0, 0.0, 200.0, 100.0);
	gtk_container_add (GTK_CONTAINER (frame), w);
	gtk_widget_show (w);

	gtk_signal_connect (GTK_OBJECT (w), "destroy",
			    (GtkSignalFunc) delete_event,
			    popup);

	item = gnome_canvas_item_new (gnome_canvas_root (GNOME_CANVAS (w)),
				      gnome_canvas_ellipse_get_type (),
				      "x1", 5.0,
				      "y1", 5.0,
				      "x2", 95.0,
				      "y2", 95.0,
				      "fill_color", "white",
				      "outline_color", "black",
				      NULL);
	gtk_signal_connect (GTK_OBJECT (item), "event",
			    (GtkSignalFunc) item_event,
			    popup);

	item = gnome_canvas_item_new (gnome_canvas_root (GNOME_CANVAS (w)),
				      gnome_canvas_ellipse_get_type (),
				      "x1", 105.0,
				      "y1", 0.0,
				      "x2", 195.0,
				      "y2", 95.0,
				      "fill_color", "white",
				      "outline_color", "black",
				      NULL);
	gtk_signal_connect (GTK_OBJECT (item), "event",
			    (GtkSignalFunc) item_event,
			    popup);

	Gnome::App::set_contents (GNOME_APP (app), vbox);
	gtk_widget_show (app);
  end
end
