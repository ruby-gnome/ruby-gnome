= Release

This document describes how to release Ruby-GNOME2.

== Packages

Here are packages to be released:

  * tar.gz: For distribution packagers such as pkgsrc.
  * gem: For normal users.
  * gem for Windows: For Windows users.

== How to create and confirm packages

=== tar.gz

The following command generates ruby-gtk2-X.Y.Z.tar.gz and
ruby-gnome2-all-X.Y.Z.tar.gz:

  % rake dist

Here are confirmation steps:

  % cp *.tar.gz /tmp
  % cd /tmp
  % tar xvf ruby-gtk2-X.Y.Z.tar.gz
  % cd ruby-gtk2-X.Y.Z
  % ruby extconf.rb
  % make
  % make install DESTDIR=/tmp/ruby
  % cd gtk2/sample/gtk-demo
  % ruby -I/tmp/ruby/usr/local/lib/site_ruby/1.8/{,x86_64-linux} main.rb
  (...try demo application...)
  % cd /tmp
  % rm -rf ruby
  % tar xvf ruby-gnome2-all-X.Y.Z.tar.gz
  % cd ruby-gnome2-all-X.Y.Z
  % ruby extconf.rb
  % make
  % make install DESTDIR=/tmp/ruby
  % cd gtk2/sample/gtk-demo
  % ruby -I/tmp/ruby/usr/local/lib/site_ruby/1.8/{,x86_64-linux} main.rb
  (...try demo application...)

=== gem

The following command generates gem files to */pkg/:

  % rake gem:build

The following command install all generated gems to user
locally:

  % rake gem:install

Here are confirmation steps:

  % cd gtk2/sample/gtk-demo
  % ruby -rubygems main.rb
  (...try demo application...)
  % cd -

The following command uninstall all gems installed by 'rake
gem:install':

  % rake gem:uninstall

=== gem for Windows

NOTE: This steps are confirmed on Debian GNU/Linux
sid. (2011-09-18)

Ruby-GNOME2 gems bundled Windows binary are created by
cross-compiling. It means that you require GNU/Linux not
Windows for releasing.

The following steps are needed only the first setup:

  % sudo gem install rake-compiler
  % sudo aptitude install -y mingw-w64
  % rake-compiler cross-ruby VERSION=1.8.7-p352 EXTS=--without-extensions
  % rake-compiler cross-ruby VERSION=1.9.2-p290 EXTS=--without-extensions

The following command downloads GTK+ related binaries for
Windows:

  % rake gem:windows:download

The following command rebuilds binary gems for Windows:

  % rake gem:windows:clean gem:windows:build

Here are confirmation steps:

  (1) copy */pkg/*-X.Y.Z-x86-mingw32.gem to Windows.
  (2) install copied gems on Windows.
  (3) cd GEM_PATH/gtk2/sample/gtk-demo/.
  (4) run 'ruby main.rb' and try the demo application.

== How to release packages

=== tar.gz

The following command uploads ruby-gtk2-X.Y.Z.tar.gz and
ruby-gnome2-all-X.Y.Z.tar.gz to SF.net:

  % rake release SVN_USER=YOUR_SF_NET_USER_NAME

=== gem

The following command uploads */pkg/*-X.Y.Z.gem to rubygems.org:

  % rake gem:push

=== gem for Windows

The following command uploads */pkg/*-X.Y.Z-x86-mingw32.gem to rubygems.org:

  % rake gem:windows:push

== After releasing packages

=== Tags the current revision

  % rake tag

=== Increments the version

  % editor glib2/ext/glib2/rbglib.h
  (increments RBGLIB_*_VERSION.)

=== Announces the current release

  (1) post release announce to
      http://ruby-gnome2.sourceforge.jp/hiki.cgi?create_news

      Here is a template:
        Your name: kou
        Subject: Ruby-GNOME2 X.Y.Z released
        Body:
          SUMMARY OF THIS RELEASE.

          == Install

            % sudo gem install gtk2

          == Downloads

          : ((<ruby-gtk2-X.Y.Z.tar.gz|URL:http://downloads.sourceforge.net/ruby-gnome2/ruby-gtk2-X.Y.Z.tar.gz>))
            The minimum package which includes Ruby/GLib2, Ruby/GIO2, Ruby/ATK, Ruby/GdkPixbuf2, Ruby/Pango, Ruby/GTK2.

          : ((<ruby-gnome2-all-X.Y.Z.tar.gz|URL:http://downloads.sourceforge.net/ruby-gnome2/ruby-gnome2-all-X.Y.Z.tar.gz>))
             All of Ruby-GNOME2 libraries.

          == Changes

          COPIED CHANGES FROM NEWS.

  (2) post release announce to ruby-gnome2-devel-en@lists.sourceforge.net

      Here is a template:
        Hi,

        The Ruby-GNOME2 project released Ruby-GNOME2 X.Y.Z:
          http://ruby-gnome2.sourceforge.jp/hiki.cgi?News_20110713_1
          REPLACE THIS WITH POSTED URL AT (1).


        SUMMARY OF THIS RELEASE.

        === Install

          % gem install gtk2

        === Changes

        COPIED CHANGES FROM NEWS.

        YOUR SIGNATURE

  (3) post release announce to ruby-talk@ruby-lang.org
      (optional)

      Here is a template:
        Hi,

        The Ruby-GNOME2 project released Ruby-GNOME2 X.Y.Z.

        Web:
          http://ruby-gnome2.sourceforge.jp/

        Install:
          gem install gtk2

        = About Ruby-GNOME2

        Ruby-GNOME2 is a set of Ruby language bindings for the GTK+
        2.x and GNOME 2.x development environment.

        = About GTK+

        from http://www.gtk.org/

          GTK+ is a highly usable, feature rich toolkit for creating
          graphical user interfaces which boasts cross platform
          compatibility and an easy to use API. GTK+ it is written
          in C, but has bindings to many other popular programming
          languages such as C++, Python and C# among others. GTK+ is
          licensed under the GNU LGPL 2.1 allowing development of
          both free and proprietary software with GTK+ without any
          license fees or royalties.

        = About GNOME

        from http://www.gnome.org/about/

          The GNOME project provides two things: The GNOME desktop
          environment, an intuitive and attractive desktop for
          users, and the GNOME development platform, an extensive
          framework for building applications that integrate into
          the rest of the desktop.

        = About this release

        SUMMARY OF THIS RELEASE.


        YOUR SIGNATURE

  (4) post release announce to ruby-gnome2-devel-ja@lists.sourceforge.net
      (optional)

      Here is a template:
        「あなたの名前」

        Ruby-GNOME2プロジェクトはRuby-GNOME2 X.Y.Zをリリースしまし
        た。

        「このリリースの要約」

        === インストール方法

          % gem install gtk2

        === このリリースの概要

        「NEWSからコピーして翻訳する」

  (5) post release announce to ruby-list@ruby-lang.org
      (optional)

      Here is a template:
        「あなたの名前」

        Ruby-GNOME2プロジェクトはRuby-GNOME2 1.0.0をリリースしました。

        Web:
          http://ruby-gnome2.sourceforge.jp/

        インストール:
          gem install gtk2

        = Ruby-GNOME2について

        Ruby-GNOME2はGNOME 2.xで使用されるGTK+ 2.xなどの主要なライブ
        ラリのRubyバインディング集です。

        GTK+はマルチプラットフォーム対応のGUIツールキットなので、マ
        ルチプラットフォーム対応のGUIアプリケーションをRubyで書くこ
        とができます。

        サポートしているライブラリは以下の通りです。

        * Ruby/GLib2         - 基本的かつ便利なユーティリティクラス群。および、
                               GLib::Objectをはじめとする基本クラスを提供。他のライブラリ
                               から参照される
        * Ruby/ATK           - アクセシビリティ
        * Ruby/Pango         - 国際化されたテキスト・フォント処理
        * Ruby/GdkPixbuf2    - イメージ（画像）処理
        * Ruby/GTK2          - GUIコンポーネント
        * Ruby/GStreamer     - 動画・音声処理
        * Ruby/GooCanvas     - cairoで描画するキャンバスウィジェット
        * Ruby/GtkSourceView2- テキストエディタ向けの高度なテキスト表示コンポーネント
                               (言語毎にキーワードを強調する等)
                               (gtksourceview 2.x用)
        * Ruby/Poppler       - PDFレンダリング用のライブラリ
        * Ruby/RSVG          - SVG処理
        * Ruby/VTE           - ターミナルエミュレータ用のウィジェット

        = このリリースについて

        「このリリースの要約」

        = 各種リソース

        - メーリングリスト
          http://sourceforge.net/mail/?group_id=53614

        - Ruby-GNOME2 APIリファレンス
          http://ruby-gnome2.sourceforge.jp/ja/hiki.cgi?Ruby-GNOME2+API+Reference

        - チュートリアル
          http://ruby-gnome2.sourceforge.jp/ja/hiki.cgi?tutorials
