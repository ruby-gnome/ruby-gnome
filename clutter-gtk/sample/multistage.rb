#!/usr/bin/env ruby
#
# This sample code is a port of clutter-gtk/examples/gtk-clutter-multistage.c.
# It is licensed under the terms of the GNU Lesser General Public
# License, version 2.1 or (at your option) later.
#
# Copyright (C) 2013  Ruby-GNOME2 Project Team
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

require "clutter-gtk"
require "gdk_pixbuf2"

window = Gtk::Window.new
window.set_default_size(600, 400)
window.title = "Multiple GtkClutterEmbed"
window.signal_connect("destroy") do
  Gtk.main_quit
end

notebook = Gtk::Notebook.new
window.add(notebook)

clutter0 = ClutterGtk::Embed.new
notebook.append_page(clutter0, Gtk::Label.new("One stage"))
stage0 = clutter0.stage
stage0.background_color = Clutter::Color.new(0xdd, 0xff, 0xdd, 0xff)

vbox = Gtk::Box.new(:vertical, 6)
notebook.append_page(vbox, Gtk::Label.new("Two stages"))

clutter1 = ClutterGtk::Embed.new
clutter1.set_size_request(320, 240)
stage1 = clutter1.stage
stage1.background_color = Clutter::Color.new(0xff, 0xff, 0xff, 0xff)
texture1 = ClutterGtk::Texture.new
texture1.set_from_stock(clutter1, Gtk::Stock::DIALOG_INFO.to_s, :dialog)
texture1.set_anchor_point(texture1.width / 2,
                          texture1.height / 2)
texture1.set_position(160, 120)
stage1.add_child(texture1)
texture1.show

vbox.add(clutter1)

clutter2 = ClutterGtk::Embed.new
clutter2.set_size_request(320, 120)
stage2 = clutter2.stage
stage2.background_color = Clutter::Color.new(0x00, 0x00, 0x00, 0xff)
texture2 = ClutterGtk::Texture.new
texture2.set_from_icon_name(clutter1, "user-info", :button)
texture2.set_anchor_point(texture2.width / 2,
                          texture2.height / 2)
texture2.set_position(160, 60)
stage2.add_child(texture2)

vbox.add(clutter2)

stage2.signal_connect("allocation-changed") do |actor, allocation, flags|
  texture2.set_position((allocation.x2 - allocation.x1) / 2,
                        (allocation.y2 - allocation.y1) / 2)
end

window.show_all

Gtk.main

__END__
#include <gtk/gtk.h>
#include <clutter/clutter.h>

#include <clutter-gtk/clutter-gtk.h>

static void
on_stage2_allocation_changed (ClutterActor           *stage_2,
                              const ClutterActorBox  *allocation,
                              ClutterAllocationFlags  flags,
                              ClutterActor           *texture_2)
{
  clutter_actor_set_position (texture_2,
                              (allocation->x2 - allocation->x1) / 2,
                              (allocation->y2 - allocation->y1) / 2);
}

int
main (int argc, char *argv[])
{
  ClutterActor *stage0, *stage1, *stage2, *tex1, *tex2;
  GtkWidget *window, *clutter0, *clutter1, *clutter2;
  GtkWidget *notebook, *vbox;
  ClutterColor col0 = { 0xdd, 0xff, 0xdd, 0xff };
  ClutterColor col1 = { 0xff, 0xff, 0xff, 0xff };
  ClutterColor col2 = {    0,    0,    0, 0xff };

  if (gtk_clutter_init (&argc, &argv) != CLUTTER_INIT_SUCCESS)
    g_error ("Unable to initialize GtkClutter");

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);
  gtk_window_set_title (GTK_WINDOW (window), "Multiple GtkClutterEmbed");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  notebook = gtk_notebook_new ();
  gtk_container_add (GTK_CONTAINER (window), notebook);

  clutter0 = gtk_clutter_embed_new ();
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook), clutter0,
                            gtk_label_new ("One stage"));
  stage0 = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (clutter0));
  clutter_actor_set_background_color (stage0, &col0);

  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook), vbox,
                            gtk_label_new ("Two stages"));

  clutter1 = gtk_clutter_embed_new ();
  gtk_widget_set_size_request (clutter1, 320, 240);
  stage1 = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (clutter1));
  clutter_actor_set_background_color (stage1, &col1);
  tex1 = gtk_clutter_texture_new ();
  gtk_clutter_texture_set_from_stock (GTK_CLUTTER_TEXTURE (tex1),
                                      clutter1,
                                      GTK_STOCK_DIALOG_INFO,
                                      GTK_ICON_SIZE_DIALOG,
                                      NULL);
  clutter_actor_set_anchor_point (tex1,
                                  clutter_actor_get_width (tex1) / 2,
                                  clutter_actor_get_height (tex1) / 2);
  clutter_actor_set_position (tex1, 160, 120);
  clutter_actor_add_child (stage1, tex1); 
  clutter_actor_show (tex1);

  gtk_container_add (GTK_CONTAINER (vbox), clutter1);

  clutter2 = gtk_clutter_embed_new ();
  gtk_widget_set_size_request (clutter2, 320, 120);
  stage2 = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (clutter2));
  clutter_actor_set_background_color (stage2, &col2);
  tex2 = gtk_clutter_texture_new ();
  gtk_clutter_texture_set_from_icon_name (GTK_CLUTTER_TEXTURE (tex2),
                                          clutter1,
                                          "user-info",
                                          GTK_ICON_SIZE_BUTTON,
                                          NULL);
  clutter_actor_set_anchor_point (tex2,
                                  clutter_actor_get_width (tex2) / 2,
                                  clutter_actor_get_height (tex2) / 2);
  clutter_actor_set_position (tex2, 160, 60);
  clutter_actor_add_child (stage2, tex2);

  gtk_container_add (GTK_CONTAINER (vbox), clutter2);

  g_signal_connect (stage2, "allocation-changed",
                    G_CALLBACK (on_stage2_allocation_changed),
                    tex2);

  gtk_widget_show_all (window);

  gtk_main();

  return 0;
}
