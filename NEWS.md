# NEWS

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
