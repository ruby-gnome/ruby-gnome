require 'gtk2'
Gtk.init

class MyButton < Gtk::Button
  type_register("MyButton")

  def initialize(label = nil)
    # XXX: type_registerするとsuperがGLib::Object#initialize相当になる
    super("label" => label)
    @fuga = 0
  end

  # 既存のシグナルのデフォルトハンドラをオーバーライド
  def signal_do_clicked(*args)
    puts "MyButton#signal_do_clicked enter"
    #p caller
    super
    puts "MyButton#signal_do_clicked leave"
  end

  # 新しいシグナルを定義
  signal_new("hoge",                  # 名前
             GLib::Signal::RUN_FIRST, # フラグ
             nil,                     # accumulator (XXX: 仕様が未確定)
             GLib::Type["void"],      # 返り値の型
             GLib::Type["gint"], GLib::Type["gint"] # 引数の型
             )
  # 新しいシグナルのデフォルトハンドラ
  def signal_do_hoge(a, b)
    puts "MyButton#signal_do_hoge enter"
    #p caller
    puts "MyButton#signal_do_hoge leave"
  end

  # 新しいプロパティの作成
  install_property(GLib::Param::Int.new("fuga", # name
                                        "Fuga", # nick
                                        "fuga hoge", # blurb
                                        0,     # min
                                        10000, # max
                                        0,     # default
                                        GLib::Param::READABLE |
                                        GLib::Param::WRITABLE))
  # プロパティの実装
  def fuga
    puts "MyButton#fuga is called"
    @fuga
  end
  def fuga=(arg)
    puts "MyButton#fuga= is called"
    @fuga = arg
  end
end

class MyButton2 < MyButton
  type_register("MyButton2")

  # clickedシグナルのデフォルトハンドラをオーバーライド
  def signal_do_clicked(*args)
    puts "MyButton2#signal_do_clicked enter"
    super(*args)
    puts "MyButton2#signal_do_clicked leave"
  end

  # hogeシグナルのデフォルトハンドラをオーバーライド
  def signal_do_hoge(a, b)
    puts "MyButton2#signal_do_hoge enter"
    #p caller
    super
    puts "MyButton2#signal_do_hoge leave"
  end
end

b = MyButton2.new("Hello")
p b
p b.label
p b.gtype
b.clicked
b.signal_emit("hoge", 1, 2)

b.signal_connect("notify"){|obj, pspec|
  puts "#{b} notify #{pspec}"
}

p b.get_property("fuga")
b.set_property("fuga", 1)
p b.get_property("fuga")

p MyButton2.ancestors
