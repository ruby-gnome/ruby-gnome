# NEWS

## Ruby-GNOME 4.2.5: 2024-12-15

This is a minor bug fix release.

### Changes

#### Ruby/GLib2

  * Fixes

    * Windows: Fixed a bug that `GType` conversion may be failed.
      * GH-1621
      * Reported by Eric Cunningham


#### Ruby/GObjectIntrospection

  * Improvements

    * Added support for transfer everything struct C array.

    * Added support for converting fixed size number array to `Array`.

#### Ruby/GTK4

  * Improvements

    * `Gtk::GestureStylus#backlog`: Improved API.

### Thanks

  * Eric Cunningham

## Ruby-GNOME 4.2.4: 2024-09-24

This is a WebKitGTK support improvement release.

### Changes

#### Ruby/WebKitGTK

  * Improvements

    * Changed to require GTK 4 and `webkitgtk-6.0`/`WebKit-6.0`.

#### Ruby/WebKit2GTK

  * Improvements

    * Added support for WebKitGTK 2.44.0 or later.

## Ruby-GNOME 4.2.3: 2024-09-19

This is a GTK 4 support improvement release.

### Changes

#### All

  * Improvements

    * Update project website URL.
      * GH-1611
      * GH-1615
      * Patch by Kazuhiro NISHIYAMA

    * Dropped support for CentOS 7.

#### Ruby/GLib2

  * Improvements

    * Added `GLib::VariantDict`.

    * Added `GLib::OptionArg`.

    * Added `GLib::OptionFlags`.

    * Added `GLib.get_os_info`.

    * `GLib::Param::*.new`: Added support for flag names.

    * `GLib::Param::*.new`: Added support for `nil` name and blurb.

#### Ruby/GObjectIntrospection

  * Improvements

    * Added support auto setter generation for class methods.

    * Added support auto setter generation for base module methods.

    * Added support for `nullable` of callback return value.

    * Added support for `nullable` `GHashTable`.

#### Ruby/GIO2

  * Improvements

    * Made `Gio::ListModel` enumerable.

    * Added `Gio::ListModel#[]`.

#### Ruby/GdkPixbuf2

  * Improvements

    * Removed tests that use XPM.
      * GH-1614
      * Reported by Mamoru TASAKA

#### Ruby/GTK4

  * Improvements

    * Renamed `Gtk::License::0BSD` to `Gtk::License::BSD_0`.
      * GH-1618
      * Reported by rubyFeedback
      * Reported by kojix2

    * `Gtk::TextBuffer#begin_irreversible_action`: Added support for
      block.

### Thanks

  * Mamoru TASAKA

  * Kazuhiro NISHIYAMA

  * rubyFeedback

  * kojix2

## Ruby-GNOME 4.2.2: 2024-04-02

This is a GLib 2.80 support improvement release.

### Changes

#### Ruby/GIO2

  * Improvements

    * Fixed a load error with GLib 2.80.
      * GH-1610
      * Reported by Dmitry Vorotilin

#### Ruby/GTK4

  * Improvements

    * `Gtk::CssProvider#load(string:)`: Added.
      * GH-1608
      * Reported by Sami Sieranoja

    * `Gtk::CssProvider#load(bytes:)`: Added.
      * GH-1608
      * Reported by Sami Sieranoja

### Thanks

  * Sami Sieranoja

  * Dmitry Vorotilin

## Ruby-GNOME 4.2.1: 2024-02-23

This is a minor bug fix release.

### Changes

#### Ruby/GLib2

  * Improvements

    * Added support for showing `GType` name for anonymous `GType`
      based classes.
      * GH-1577
      * Reported by Sami Sieranoja

    * Added `RG_DEF_SALIAS()`.

    * Stopped providing unusable signal related methods to not signal
      ready objects such as `GLib::Instantiatable`.
      * GH-1589
      * Reported by Sami Sieranoja

    * Added `GLib::Type::INVALID`. `GLib::Type[nil]` is also accepted.
      * GH-1594
      * Reported by Sami Sieranoja

    * Added `GLib::TimeZone#identifier`.

    * Added `GLib::TimeZone.try_convert`.

#### Ruby/GObjectIntrospection

  * Improvements

    * Ignored `.hash(some_arguments)` methods. It may break something.

    * Ignored fundamental `GType`.
      * GH-1580
      * Reported by Sami Sieranoja

    * Added support for returning sized `GType` array.

#### Ruby/GDK4

  * Fixes

    * Fixed a GC related bug of `Gdk::MemoryTexture.new`.
      * GH-1596
      * Reported by HuBandiT

    * Fixed a GC related bug of `Gdk::Picture`.
      * GH-1596
      * Reported by HuBandiT

#### Ruby/GTK4

  * Improvements

    * Added a menu bar sample.
      * GH-1581
      * GH-1582
      * GH-1584
      * Patch by Sami Sieranoja

    * Added support for `Gtk::DropTarget.new([type, ...], actions)`.
      * GH-1594
      * Reported by Sami Sieranoja

  * Fixes

    * Fixed deprecation message of `Gtk::Button.new`.
      * GH-1587
      * Reported by Sami Sieranoja

    * Fixed a GC related bug of `Gtk::Image`.

    * Fixed a GC related bug of `Gtk::ColumnView`.
      * GH-1600
      * Reported by HuBandiT

    * Fixed a GC related bug of `Gtk::SingleSelection`.
      * GH-1600
      * Reported by HuBandiT

#### Ruby/GStreamer

  * Improvements

    * Added a missing GC guard for `Gst::Element#bus`.
      * GH-1586
      * Reported by Sami Sieranoja

### Thanks

  * Sami Sieranoja

  * HuBandiT

## Ruby-GNOME 4.2.0: 2023-08-19

This is a minor bug fix release.

### Changes

#### Ruby/GTK3

  * Fixes

    * Suppressed a build time warning.

## Ruby-GNOME 4.1.9: 2023-08-11

This is a vte4 added release.

### Changes

#### Ruby/GObjectIntrospection

  * Improvements

    * Changed to use GLib memory allocation functions instead of
      Ruby's one when calling callbacks. Because callbacks may be
      called in out of the main Ruby thread.

#### Ruby/VTE3

  * Improvements

    * Ensured using GTK3.

#### Ruby/VTE4

  * Improvements

    * Added.

## Ruby-GNOME 4.1.8: 2023-06-25

This is a conda support release.

### Changes

#### All

  * Improvements

    * Added support for installing conda packages automatically.

## Ruby-GNOME 4.1.7: 2023-05-31

This is a minor improvement release for 4.1.6.

### Changes

#### Ruby/GLib2

  * Improvements

    * Added support for writer barrier.

#### Ruby/CairoGObject

  * Improvements

    * Added support for specifying rcairo's source directory by
      `RCAIRO_SOURCE_DIR`.

## Ruby-GNOME 4.1.6: 2023-05-21

This is a minor improvement release for 4.1.5.

### Changes

#### Ruby/GLib2

  * Improvements

    * Avoided closure related `TypeError` in GC at exit.
      [Reported by Matijs van Zuijlen]

### Thanks

  * Matijs van Zuijlen

## Ruby-GNOME 4.1.5: 2023-05-11

This is a bug fix release for 4.1.4.

### Changes

#### Ruby/GLib2

  * Improvements

    * Ignored `gmodule.h` explicitly for MacPorts.
      [Reported by Akira Ouchi]

### Thanks

  * Akira Ouchi

## Ruby-GNOME 4.1.4: 2023-05-04

This is a bug fix release for 4.1.3.

### Changes

#### Ruby/GObjectIntrospection

  * Improvements

    * Hid internal `INVOKERS` constant to improve backward compatibility.
      GH-1551
      [Reported by Mamoru TASAKA]

### Thanks

  * Mamoru TASAKA

## Ruby-GNOME 4.1.3: 2023-04-28

This is a bug fix release for 4.1.2.

### Changes

#### Ruby/GObjectIntrospection

  * Improvements

    * Added support for methods and functions for enum and flags.

    * Improved internal `GObjectIntrospection::Loader` APIs but some
      libraries may use these APIs. So this may be a backward
      incompatible change.

  * Fixes

    * Fixed a bug for nullable argument support.

#### Ruby/GTK4

  * Improvements

    * Added examples:
      * GH-1551
      * GH-1552
      * GH-1553
      * GH-1554
      * GH-1555
      * GH-1556
      * GH-1557
      * GH-1558
      [Patch by Toshio Sekiya]

### Thanks

  * Toshio Sekiya

## Ruby-GNOME 4.1.2: 2023-02-24

This is a release to improve support for the latest GNOME related libraries.

### Changes

#### Ruby/GLib2

  * Improvements

    * Added support for GLib 2.75 or later.
      [GH-1547]
      [Reported by Mamoru TASAKA]

#### Ruby/GObjectIntrospection

  * Improvements

    * Improved wrong arguments error message for `#initialize`.
      [GH-1545]
      [Reported by D-W-L]

#### Ruby/GTK4

  * Improvements

    * Added support for GTK 4.9.3 or later.
      [GH-1548]
      [Reported by Mamoru TASAKA]

    * Added an example.
      [GH-1549]
      [Patch by Toshio Sekiya]

### Thanks

  * D-W-L

  * Mamoru TASAKA

  * Toshio Sekiya

## Ruby-GNOME 4.1.1: 2023-02-09

This is a release to improve callback.

### Changes

#### Ruby/GLib2

  * Improvements

    * Added missing error check in `GLib::Object.type_register`.
      [GH-1543]
      [Reported by D-W-L]

    * Added support for `try_convert` protocol on converting a Ruby
      object to boxed value.

#### Ruby/GObjectIntrospection

  * Improvements

    * Relaxed return values from callback. Non `Array` value is
      accepted for callback that needs multiple return values. Missing
      return values are processed as `nil`.

    * Suppressed a warning that is reported when returning `nil` for
      boxed value.

### Thanks

  * D-W-L

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
