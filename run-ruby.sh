#!/bin/sh

GDB=${GDB:-}
RUBY=${RUBY:-ruby}

base_dir=$(cd $(dirname $0) && pwd)

${GDB} ${RUBY} \
  -I "${base_dir}/glib2/lib" \
  -I "${base_dir}/glib2/ext/glib2" \
  -I "${base_dir}/gobject-introspection/lib" \
  -I "${base_dir}/gobject-introspection/ext/gobject-introspection" \
  -I "${base_dir}/gio2/lib" \
  -I "${base_dir}/gio2/ext/gio2" \
  -I "${base_dir}/cairo-gobject/lib" \
  -I "${base_dir}/cairo-gobject/ext/cairo-gobject" \
  -I "${base_dir}/atk/lib" \
  -I "${base_dir}/pango/lib" \
  -I "${base_dir}/pango/ext/pango" \
  -I "${base_dir}/gdk_pixbuf2/lib" \
  -I "${base_dir}/gdk_pixbuf2/ext/gdk_pixbuf2" \
  -I "${base_dir}/gtk2/lib" \
  -I "${base_dir}/gtk2/ext/gtk2" \
  -I "${base_dir}/gdk3/lib" \
  -I "${base_dir}/gtk3/lib" \
  -I "${base_dir}/gtk3/ext/gtk3" \
  -I "${base_dir}/rsvg2/lib" \
  -I "${base_dir}/poppler/lib" \
  -I "${base_dir}/gstreamer/lib" \
  -I "${base_dir}/gstreamer/ext/gstreamer" \
  -I "${base_dir}/vte3/lib" \
  -I "${base_dir}/clutter/lib" \
  -I "${base_dir}/clutter-gdk/lib" \
  -I "${base_dir}/clutter-gtk/lib" \
  -I "${base_dir}/clutter-gstreamer/lib" \
  -I "${base_dir}/webkit-gtk/lib" \
  -I "${base_dir}/webkit-gtk2/lib" \
  -I "${base_dir}/webkit2-gtk/lib" \
  -I "${base_dir}/gtksourceview2/lib" \
  -I "${base_dir}/gtksourceview2/ext/gtksourceview2" \
  -I "${base_dir}/gtksourceview3/lib" \
  -I "${base_dir}/gsf/lib" \
  -I "${base_dir}/goffice/lib" \
  -I "${base_dir}/gnumeric/lib" \
  "$@"
