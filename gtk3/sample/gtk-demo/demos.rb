module DEMOS
  child0 = [ 
    { :name =>  "css_accordion", :title =>  "CSS Accordion", :filename =>  "css_accordion.rb", :func =>  :do_css_accordion, :children =>  nil  },
    { :name =>  "css_basics", :title =>  "CSS Basics", :filename =>  "css_basics.rb", :func =>  :do_css_basics, :children =>  nil  },
    { :name =>  "css_multiplebgs", :title =>  "Multiple Backgrounds", :filename =>  "css_multiplebgs.rb", :func =>  :do_css_multiplebgs, :children =>  nil  },
    { :name =>  "css_pixbufs", :title =>  "Animated Backgrounds", :filename =>  "css_pixbufs.rb", :func =>  :do_css_pixbufs, :children =>  nil  },
    { :name =>  "css_shadows", :title =>  "Shadows", :filename =>  "css_shadows.rb", :func =>  :do_css_shadows, :children =>  nil  },
    { :name =>  "theming_style_classes", :title =>  "Style Classes", :filename =>  "theming_style_classes.rb", :func =>  :do_theming_style_classes, :children =>  nil  },
  ]

  child1 = [
    { :name =>  "editable_cells", :title =>  "Editable Cells", :filename =>  "editable_cells.rb", :func =>  :do_editable_cells, :children =>  nil  },
    { :name =>  "filtermodel", :title =>  "Filter Model", :filename =>  "filtermodel.rb", :func =>  :do_filtermodel, :children =>  nil  },
    { :name =>  "list_store", :title =>  "List Store", :filename =>  "list_store.rb", :func =>  :do_list_store, :children =>  nil  },
    { :name =>  "tree_store", :title =>  "Tree Store", :filename =>  "tree_store.rb", :func =>  :do_tree_store, :children =>  nil  },
  ]

  child2 = [
    { :name =>  "entry_buffer", :title =>  "Entry Buffer", :filename =>  "entry_buffer.rb", :func =>  :do_entry_buffer, :children =>  nil  },
    { :name =>  "entry_completion", :title =>  "Entry Completion", :filename =>  "entry_completion.rb", :func =>  :do_entry_completion, :children =>  nil  },
    { :name =>  "search_entry", :title =>  "Search Entry", :filename =>  "search_entry.rb", :func =>  :do_search_entry, :children =>  nil  },
    { :name =>  "search_entry2", :title =>  "Delayed Search Entry", :filename =>  "search_entry2.rb", :func =>  :do_search_entry2, :children =>  nil  },
  ]

  child3 = [
    { :name =>  "font_features", :title =>  "Font Features", :filename =>  "font_features.rb", :func =>  :do_font_features, :children =>  nil  },
    { :name =>  "rotated_text", :title =>  "Rotated Text", :filename =>  "rotated_text.rb", :func =>  :do_rotated_text, :children =>  nil  },
    { :name =>  "textmask", :title =>  "Text Mask", :filename =>  "textmask.rb", :func =>  :do_textmask, :children =>  nil  },
  ]

  child4 = [ 
    { :name =>  "hypertext", :title =>  "Hypertext", :filename =>  "hypertext.rb", :func =>  :do_hypertext, :children =>  nil  },
    { :name =>  "markup", :title =>  "Markup", :filename =>  "markup.rb", :func =>  :do_markup, :children =>  nil  },
    { :name =>  "textview", :title =>  "Multiple Views", :filename =>  "textview.rb", :func =>  :do_textview, :children =>  nil  },
    { :name =>  "textscroll", :title =>  "Automatic Scrolling", :filename =>  "textscroll.rb", :func =>  :do_textscroll, :children =>  nil  },
  ]

  child5 = [
    { :name =>  "iconview", :title =>  "Icon View Basics", :filename =>  "iconview.rb", :func =>  :do_iconview, :children =>  nil  },
    { :name =>  "iconview_edit", :title =>  "Editing and Drag-and-Drop", :filename =>  "iconview_edit.rb", :func =>  :do_iconview_edit, :children =>  nil  },
  ]
  child6 = [
    { :name =>  "offscreen_window", :title =>  "Rotated Button", :filename =>  "offscreen_window.rb", :func =>  :do_offscreen_window, :children =>  nil  },
    { :name =>  "offscreen_window2", :title =>  "Effects", :filename =>  "offscreen_window2.rb", :func =>  :do_offscreen_window2, :children =>  nil  },
  ]
  child7 = [
    { :name =>  "overlay", :title =>  "Interactive Overlay", :filename =>  "overlay.rb", :func =>  :do_overlay, :children =>  nil  },
    { :name =>  "overlay2", :title =>  "Decorative Overlay", :filename =>  "overlay2.rb", :func =>  :do_overlay2, :children =>  nil  },
    { :name =>  "transparent", :title =>  "Transparency", :filename =>  "transparent.rb", :func =>  :do_transparent, :children =>  nil  },
  ]

  child8 = [ 
    { :name =>  "printing", :title =>  "Printing", :filename =>  "printing.rb", :func =>  :do_printing, :children =>  nil  },
  ]

  LIST = [ 
    { :name =>  "application", :title =>  "Application Class", :filename =>  "application.rb", :func =>  :do_application, :children =>  nil  }, 
    { :name =>  "assistant", :title =>  "Assistant", :filename =>  "assistant.rb", :func =>  :do_assistant, :children =>  nil  }, 
    { :name =>  "builder", :title =>  "Builder", :filename =>  "builder.rb", :func =>  :do_builder, :children =>  nil  }, 
    { :name =>  "button_box", :title =>  "Button Boxes", :filename =>  "button_box.rb", :func =>  :do_button_box, :children =>  nil  }, 
    { :name =>  nil, :title =>  "CSS Theming", :filename =>  nil, :func =>  nil, :children =>  child0  }, 
    { :name =>  "changedisplay", :title =>  "Change Display", :filename =>  "changedisplay.rb", :func =>  :do_changedisplay, :children =>  nil  }, 
    { :name =>  "clipboard", :title =>  "Clipboard", :filename =>  "clipboard.rb", :func =>  :do_clipboard, :children =>  nil  }, 
    { :name =>  "colorsel", :title =>  "Color Chooser", :filename =>  "colorsel.rb", :func =>  :do_colorsel, :children =>  nil  }, 
    { :name =>  "combobox", :title =>  "Combo Boxes", :filename =>  "combobox.rb", :func =>  :do_combobox, :children =>  nil  }, 
    { :name =>  "cursors", :title =>  "Cursors", :filename =>  "cursors.rb", :func =>  :do_cursors, :children =>  nil  }, 
    { :name =>  "dialog", :title =>  "Dialogs and Message Boxes", :filename =>  "dialog.rb", :func =>  :do_dialog, :children =>  nil  }, 
    { :name =>  "drawingarea", :title =>  "Drawing Area", :filename =>  "drawingarea.rb", :func =>  :do_drawingarea, :children =>  nil  }, 
    { :name =>  nil, :title =>  "Entry", :filename =>  nil, :func =>  nil, :children =>  child2  }, 
    { :name =>  "event_axes", :title =>  "Event Axes", :filename =>  "event_axes.rb", :func =>  :do_event_axes, :children =>  nil  }, 
    { :name =>  "expander", :title =>  "Expander", :filename =>  "expander.rb", :func =>  :do_expander, :children =>  nil  }, 
    { :name =>  "flowbox", :title =>  "Flow Box", :filename =>  "flowbox.rb", :func =>  :do_flowbox, :children =>  nil  }, 
    { :name =>  "gestures", :title =>  "Gestures", :filename =>  "gestures.rb", :func =>  :do_gestures, :children =>  nil  }, 
    { :name =>  "headerbar", :title =>  "Header Bar", :filename =>  "headerbar.rb", :func =>  :do_headerbar, :children =>  nil  }, 
    { :name =>  nil, :title =>  "Icon View", :filename =>  nil, :func =>  nil, :children =>  child5  }, 
    { :name =>  "images", :title =>  "Images", :filename =>  "images.rb", :func =>  :do_images, :children =>  nil  }, 
    { :name =>  "infobar", :title =>  "Info Bars", :filename =>  "infobar.rb", :func =>  :do_infobar, :children =>  nil  }, 
    { :name =>  "links", :title =>  "Links", :filename =>  "links.rb", :func =>  :do_links, :children =>  nil  }, 
    { :name =>  "listbox", :title =>  "List Box", :filename =>  "listbox.rb", :func =>  :do_listbox, :children =>  nil  }, 
    { :name =>  "menus", :title =>  "Menus", :filename =>  "menus.rb", :func =>  :do_menus, :children =>  nil  }, 
    { :name =>  "modelbutton", :title =>  "Model Button", :filename =>  "modelbutton.rb", :func =>  :do_modelbutton, :children =>  nil  }, 
    { :name =>  nil, :title =>  "Offscreen Windows", :filename =>  nil, :func =>  nil, :children =>  child6  }, 
    { :name =>  "glarea", :title =>  "OpenGL Area", :filename =>  "glarea.rb", :func =>  :do_glarea, :children =>  nil  }, 
    { :name =>  nil, :title =>  "Overlay", :filename =>  nil, :func =>  nil, :children =>  child7  }, 
    { :name =>  "panes", :title =>  "Paned Widgets", :filename =>  "panes.rb", :func =>  :do_panes, :children =>  nil  }, 
    { :name =>  nil, :title =>  "Pango", :filename =>  nil, :func =>  nil, :children =>  child3  }, 
    { :name =>  "pickers", :title =>  "Pickers", :filename =>  "pickers.rb", :func =>  :do_pickers, :children =>  nil  }, 
    { :name =>  "pixbufs", :title =>  "Pixbufs", :filename =>  "pixbufs.rb", :func =>  :do_pixbufs, :children =>  nil  }, 
    { :name =>  "popover", :title =>  "Popovers", :filename =>  "popover.rb", :func =>  :do_popover, :children =>  nil  }, 
    { :name =>  nil, :title =>  "Printing", :filename =>  nil, :func =>  nil, :children =>  child8  }, 
    { :name =>  "revealer", :title =>  "Revealer", :filename =>  "revealer.rb", :func =>  :do_revealer, :children =>  nil  }, 
    { :name =>  "scale", :title =>  "Scale", :filename =>  "scale.rb", :func =>  :do_scale, :children =>  nil  }, 
    { :name =>  "sizegroup", :title =>  "Size Groups", :filename =>  "sizegroup.rb", :func =>  :do_sizegroup, :children =>  nil  }, 
    { :name =>  "spinbutton", :title =>  "Spin Button", :filename =>  "spinbutton.rb", :func =>  :do_spinbutton, :children =>  nil  }, 
    { :name =>  "spinner", :title =>  "Spinner", :filename =>  "spinner.rb", :func =>  :do_spinner, :children =>  nil  }, 
    { :name =>  "stack", :title =>  "Stack", :filename =>  "stack.rb", :func =>  :do_stack, :children =>  nil  }, 
    { :name =>  "sidebar", :title =>  "Stack Sidebar", :filename =>  "sidebar.rb", :func =>  :do_sidebar, :children =>  nil  }, 
    { :name =>  nil, :title =>  "Text View", :filename =>  nil, :func =>  nil, :children =>  child4  }, 
    { :name =>  "toolpalette", :title =>  "Tool Palette", :filename =>  "toolpalette.rb", :func =>  :do_toolpalette, :children =>  nil  }, 
    { :name =>  nil, :title =>  "Tree View", :filename =>  nil, :func =>  nil, :children =>  child1  },
  ]

end
