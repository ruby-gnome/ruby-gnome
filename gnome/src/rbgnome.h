#ifndef _RBGNOME_H_included
#define _RBGNOME_H_included

#define ENABLE_NLS 1

#include <sys/time.h>
#include <gnome.h>

#undef PATH_SEP
#undef _

#include "rbgtk.h"
#include <rubyio.h>

extern VALUE rb_progname;

struct timeval rb_time_timeval(VALUE);
void signal_callback();

extern VALUE mGnome;
extern VALUE mGnomeUIInfo;
extern VALUE mGnomeConfig;
extern VALUE mGnomeI18n;
extern VALUE mGnomeURL;

extern VALUE gDial;
extern VALUE gClock;
extern VALUE gPixmapMenuItem;

extern VALUE gnoAbout;
extern VALUE gnoAnimator;
extern VALUE gnoApp;
extern VALUE gnoAppBar;
extern VALUE gnoCalculator;
extern VALUE gnoCanvas;
extern VALUE gnoCanvasItem;
extern VALUE gnoCanvasLine;
extern VALUE gnoCanvasPolygon;
extern VALUE gnoCanvasRect;
extern VALUE gnoCanvasEllipse;
extern VALUE gnoCanvasText;
extern VALUE gnoCanvasImage;
extern VALUE gnoCanvasWidget;
extern VALUE gnoCanvasGroup;
extern VALUE gnoCanvasPoints;
extern VALUE gnoClient;
extern VALUE gnoColorPicker;
extern VALUE gnoDateEdit;
extern VALUE gnoDEntryEdit;
extern VALUE gnoDesktopEntry;
extern VALUE gnoDialog;
extern VALUE gnoDock;
extern VALUE gnoDockBand;
extern VALUE gnoDockItem;
extern VALUE gnoDockLayout;
extern VALUE gnoDockLayoutItem;
extern VALUE gnoEntry;
extern VALUE gnoFileEntry;
extern VALUE gnoFontPicker;
extern VALUE gnoHRef;
extern VALUE gnoIconEntry;
extern VALUE gnoIconList;
extern VALUE gnoIconSelection;
extern VALUE gnoLess;
extern VALUE gnoMessageBox;
extern VALUE gnoNumberEntry;
extern VALUE gnoPaperSelector;
extern VALUE gnoPixmap;
extern VALUE gnoPixmapEntry;
extern VALUE gnoPropertyBox;
extern VALUE gnoProcBar;
extern VALUE gnoScores;
extern VALUE gnoSpell;
extern VALUE gnoStock;

VALUE make_gnobject_auto_type(GtkObject*);
VALUE get_value_from_gno_obj(GtkObject*);

GnomeDesktopEntry* get_dentry(VALUE);
VALUE make_dentry(GnomeDesktopEntry*);

GnomeCanvasPoints* get_gnome_canvas_points(VALUE);
VALUE make_gnome_canvas_points(GnomeCanvasPoints*);

GdkImlibImage *get_gdkimlib_image(VALUE);

void rbgnome_register_citem_type(VALUE, GtkType);

void Init_gtk_dial();
void Init_gtk_clock();
void Init_gtk_pixmap_menu_item();

void Init_gnome_about();
void Init_gnome_animator();
void Init_gnome_app();
void Init_gnome_appbar();
void Init_gnome_app_helper();
void Init_gnome_calculator();
void Init_gnome_canvas();
void Init_gnome_canvas_item();
void Init_gnome_canvas_group();
void Init_gnome_canvas_util();
void Init_gnome_client();
void Init_gnome_color_picker();
void Init_gnome_dateedit();
void Init_gnome_desktop_entry();
void Init_gnome_dentry_edit();
void Init_gnome_dialog();
void Init_gnome_dock();
void Init_gnome_dock_band();
void Init_gnome_dock_item();
void Init_gnome_dock_layout();
void Init_gnome_entry();
void Init_gnome_file_entry();
void Init_gnome_font_picker();
void Init_gnome_href();
void Init_gnome_icon_entry();
void Init_gnome_icon_list();
void Init_gnome_icon_selection();
void Init_gnome_less();
void Init_gnome_messagebox();
void Init_gnome_number_entry();
void Init_gnome_paper_selector();
void Init_gnome_pixmap();
void Init_gnome_pixmap_entry();
void Init_gnome_property_box();
void Init_gnome_proc_bar();
void Init_gnome_scores();
void Init_gnome_spell();
void Init_gnome_stock();
void Init_gnome_config();
void Init_gnome_i18n();
void Init_gnome_uidefs();
void Init_gnome_url();
void Init_gnome_util();

#endif /* ! _RBGNOME_H_included */
