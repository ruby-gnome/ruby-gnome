# NEWS

## Ruby-GNOME 4.1.0: 2023-01-31

This is a bug fix release of 4.0.9.

### Changes

#### Ruby/GLib2

  * Fixes

    * Fixed a memory leak introduced in 4.0.9.

#### Ruby/Handy

  * Improvements

    * Renamed gem name to `libhandy` from `handy` because `handy` gem
      already exists.

## Ruby-GNOME 4.0.9: 2023-01-31

This is a bug fix release of 4.0.8.

### Changes

#### Ruby/GLib2

  * Fixes

    * Fixed a crash bug that is related to closure and GC.

## Ruby-GNOME 4.0.8: 2023-01-15

This is a bug fix release of 4.0.7.

### Changes

#### Ruby/GLib2

  * Fixes

    * Fixed a bug that gems that depend on glib2 gem can't find
      `glib-enum-types.h`.

## Ruby-GNOME 4.0.7: 2023-01-15

This is a release for Ruby 3.2 on Windows again.

### Changes

#### Ruby/GLib2

  * Improvements

    * Really added support for Ruby 3.2 on Windows.
      [GH-1535]
      [Reported by dsisnero]

### Thanks

  * dsisnero

## Ruby-GNOME 4.0.6: 2023-01-08

This is a release for Ruby 3.2 on Windows.

### Changes

#### Ruby/GLib2

  * Improvements

    * Added support for Ruby 3.2 on Windows.
      [GH-1533]
      [Reported by dsisnero]

### Thanks

  * dsisnero

## Ruby-GNOME 4.0.5: 2022-12-12

This is a bug fix release of 4.0.4.

### Changes

#### Ruby/GObjectIntrospection

  * Fixes

    * Fixed a memory leak that `GDestroyNotify` argument's metadata
      isn't freed. For example, this is happen with `GLib::Timeout.add`.

## Ruby-GNOME 4.0.4: 2022-12-09

This is a release that adds more new libraries: Ruby/Adwaita,
Ruby/GtkSourceView3 and Ruby/Handy.

### Changes

#### Documents

  * Improvements

    * Added gdk4 and gtk4.
      [[GitHub#1514](https://github.com/ruby-gnome/ruby-gnome/issues/1514)]
      [Reported by codart]

#### All

  * Improvements

    * Added support for macOS 12/Xcode 14.
      [[GitHub#1516](https://github.com/ruby-gnome/ruby-gnome/issues/1516)]
      [Reported by Apoorv Sohal]

#### Ruby/GLib2

  * Improvements

    * Added support for instantiating `GLib::Pointer` and getting
      address of it.

    * Renamed `gnome2/` to `gnome/` in internal library path.

    * Suppressed `connected_closures` isn't initialized warning.

    * Re-added `GLib::ValueArray` because GStreamer still uses it
      without replacement.
      [[GitHub#1520](https://github.com/ruby-gnome/ruby-gnome/issues/1520)]
      [Reported by Matt Palmer]

    * Set missing `rb_data_type_t::wrap_struct_name`.
      [[GitHub#1522](https://github.com/ruby-gnome/ruby-gnome/issues/1522)]
      [Patch by Peter Zhu]

    * Added support for more `GSpawnFlags`.

  * Fixes

    * Fixed a bug that `GLib::Pointer.gtype` and `GLib::Pointer#gtype`
      use the wrong number of arguments.

    * Fixed a memory leak in `GLib::Object` based classes.
      [[GitHub#1523](https://github.com/ruby-gnome/ruby-gnome/issues/1523)]
      [Patch by Peter Zhu]

#### Ruby/GObjectIntrospection

  * Improvements

    * Added support for `length` in field.
      [[GitHub#1524](https://github.com/ruby-gnome/ruby-gnome/issues/1524)]
      [Reported by jvmf1]

#### Ruby/GIO2

  * Improvements

    * Added support for getting `GLib::Mount` by `Gio::Volume#mount`.
      [[GitHub#604](https://github.com/ruby-gnome/ruby-gnome/issues/604)]

#### Ruby/GTK4

  * Improvements

    * [sample] Updated.
      [[GitHub#1512](https://github.com/ruby-gnome/ruby-gnome/issues/1512)]
      [[GitHub#1515](https://github.com/ruby-gnome/ruby-gnome/issues/1515)]
      [Patch by kojix2]
      [[GitHub#1526](https://github.com/ruby-gnome/ruby-gnome/issues/1526)]
      [[GitHub#1527](https://github.com/ruby-gnome/ruby-gnome/issues/1527)]
      [Patch by Daniel Mircea]

  * Fixes

    * Fixed a bug that `Gtk::ScrolledWindow` can't be created.
      [[GitHub#1504](https://github.com/ruby-gnome/ruby-gnome/issues/1504)]
      [Reported by rubyFeedback]

    * Fixed a typo in `Gtk::Image.new`.
      [[GitHub#1504](https://github.com/ruby-gnome/ruby-gnome/issues/1504)]
      [Reported by rubyFeedback]

    * Fixed a bug that `Gtk::CellRendereText` may be GC-ed.
      [[GitHub#1518](https://github.com/ruby-gnome/ruby-gnome/issues/1518)]
      [Reported by galtgenod]

    * Fixed a crash bug that is caused when GLib based class defined
      by Ruby is used.
      [[GitHub#1444](https://github.com/ruby-gnome/ruby-gnome/issues/1444)]
      [Reported by HuBandiT]

#### Ruby/libsecret

  * Fixes

    * Fixed wrong MSYS2 package name.

#### Ruby/GtkSourceView4

  * Fixes

    * Fixed package ID.
      [[GitHub#1525](https://github.com/ruby-gnome/ruby-gnome/issues/1525)]
      [Reported by Daniel Mircea]

#### Ruby/GtkSourceView5

  * Added.
    [[GitHub#1511](https://github.com/ruby-gnome/ruby-gnome/issues/1511)]
    [Reported by galtgendo]

#### Ruby/Adwaita

  * Added.
    [[GitHub#1510](https://github.com/ruby-gnome/ruby-gnome/issues/1510)]
    [Reported by rubyFeedback]

#### Ruby/Handy

  * Added.
    [[GitHub#1434](https://github.com/ruby-gnome/ruby-gnome/issues/1434)]
    [Reported by Jacob Michalskie]

### Thanks

  * rubyFeedback

  * kojix2

  * codart

  * Apoorv Sohal

  * galtgendo

  * Matt Palmer

  * Peter Zhu

  * HuBandiT

  * jvmf1

  * Daniel Mircea

  * Jacob Michalskie

## Ruby-GNOME 4.0.3: 2022-09-13

This is a bug fix release of 4.0.2.

### Changes

#### Ruby/GTK4

  * Fixes

    * Fixed a bug that `ext/**/*.*` aren't included in `.gem`.
      [GitHub#1498][Reported by Anatol Pomozov]

### Thanks

  * Anatol Pomozov

## Ruby-GNOME 4.0.2: 2022-09-05

This is a bug fix release of 4.0.1.

### Changes

#### Ruby/GLib2

  * Fixes

    * Fixed a bug that closure may be GC-ed.
      [GitHub#1495][Reported by Matijs van Zuijlen]

#### Ruby/GObjectIntrospection

  * Fixes

    * Fixed a bug that callback doesn't work with GObject
      Introspection 1.72.0 or later.
      [GitHub#1496][Reported by Mamoru TASAKA]

### Thanks

  * Mamoru TASAKA

  * Matijs van Zuijlen

## Ruby-GNOME 4.0.1: 2022-09-02

This is a bug fix release of 4.0.0.

### Changes

#### Ruby/GLib2

  * Fixes

    * Fixed wrong false constant value.

## Ruby-GNOME 4.0.0: 2022-09-01

This is a release that supports GTK 4.

### Changes

#### All

  * windows: Removed a needless workaround for old libintl-8.dll.

  * doc: Added support for generating API documents.
    [GitHub#1468][Patch by kojix2]

  * doc: Added Rake tasks to generate documents.
    [GitHub#1482][Patch by Sean Champ]

  * Use Bundler API to load (({Gemfile.local})).
    [GitHub#1483][Patch by Sean Champ]

#### Ruby/GLib2

  * Improvements

    * Changed to use `g_warning()` from `rb_warn()` for
      already destroyed closure warning.

    * Dropped support for GLib 2.48 or earlier.

    * Added `GLib::DateTime#format_iso8601`.

    * Added `rbg_mGLib()`.

    * Added `rbg_mGLibObject()`.

    * Deprecated `mGLib`.

    * Added support for auto native package installation on Gentoo Linux.

    * Added `GLib.malloc`.

    * Added `GLib.malloc0`.

    * Added `GLib.free`.

    * Added `GLib::Variant#to_s`.
      [GitHub#1491][Reported by Sean Champ]

    * Added `GLib::Variant#inspect`.

    * Added `GLib::Variant.parse`.
      [GitHub#1491][Reported by Sean Champ]

  * Fixes

    * closure: Fixed a reference count problem.

#### Ruby/GObjectIntrospection

  * Improvements

    * Don't raise an exception for `object == nil`.
      [GitHub#1472][Reported by Hirokazu SUZUKI]

    * Added support for returning a fixed size C array of `GObject`.
      [GitHub#1481][Reported by Sean Champ]

    * Added support for GObject Introspection 1.72.0 or later.

    * Added support for returning `GClosure` from virtual
      method.

    * Added support for `transfer full` boxed type returned by closure.

  * Fixes

    * Fixed a bug that `gpointer` to `VALUE` conversion is
      broken on 32bit environment.
      [GitHub#1462][Reported by Mamoru TASAKA]

    * Fixed a wrong `GHashTable` value conversion in Ruby ->
      `GHashTable`.
      [GitHub#1481][Reported by Sean Champ]

    * Fixed a memory leak of
      `GObjectIntrospection::CallableInfo#return_type`.

    * Fixed a memory leak for async callback.

#### Ruby/GTK3

  * Improvements

    * Required GTK+ 3.22 or later.

    * Added support for `Gtk::AccelMap.each(filter: true)`.
      [GitHub#1487][Reported by Sean Champ]

  * Fixes

    * Fixed wrong {`Gtk::RadioToolButton.new` usage in demo.
      [GitHub#1475][Reported by Sami Sieranoja]

    * Fixed a GC related bug of `Gtk::Widget#insert_action_group`.
      [GitHub#1490][Reported by Sean Champ]

#### Ruby/GDK4

  * Re-implemented based on the current Ruby/GDK3.

#### Ruby/GTK4

  * Re-implemented based on the current Ruby/GTK4.

#### Ruby/Poppler

  * Improvements

    * Added support for `Poppler::Document.new(GLib::Bytes)`.

#### Ruby/RSVG2

  * Fixes

    * doc: Fixed a link.
      [GitHub#1469][Patch by Blake Gearin]

#### Ruby/GStreamer

  * Fixes

    * example: Updated to work with the latest GStreamer.
      [GitHub#1478][Patch by kojix2]

#### Ruby/libsecret

  * Improvements

    * Added support for `Secret::Schema#attributes`.

#### Ruby/VTE3

  * Improvements

    * Removed needless `Vte::Terminal#feed_child` wrapper.
      [GitHub#1484][Patch by Sean Champ]

### Thanks

  * Mamoru TASAKA

  * kojix2

  * Blake Gearin

  * Hirokazu SUZUKI

  * Sami Sieranoja

  * Sean Champ

## Old news

See [NEWS.rd](NEWS.rd).
