#  -*- indent-tabs-mode: nil -*-

class CanvasSampleRichText < Gtk::VBox
  def setup_text(root)
    Gnome::CanvasRect.new(root,
                          {"x1" => -90.0,
                            "y1" => -50.0,
                            "x2" => 110.0,
                            "y2" => 50.0,
                            "fill_color" => "green",
                            "outline_color" => "green"})

    Gnome::CanvasRichText.new(root,
                              {"x" => -90.0,
                                "y" => -50.0,
                                "width" => 200.0,
                                "height" => 100.0,
                                "grow_height" => true,
                                "text" => <<EOS})
English is so boring because everyone uses it.
Here is something exciting:  
وقد بدأ ثلاث من أكثر المؤسسات تقدما في شبكة اكسيون برامجها كمنظمات لا تسعى للربح، ثم تحولت في السنوات الخمس الماضية إلى مؤسسات مالية منظمة، وباتت جزءا من النظام المالي في بلدانها، ولكنها تتخصص في خدمة قطاع المشروعات الصغيرة. وأحد أكثر هذه المؤسسات نجاحا هو »بانكوسول« في بوليفيا.
And here is some more plain, boring English.
EOS

    Gnome::CanvasEllipse.new(root,
                             {"x1" => -5.0,
                               "y1" => -5.0,
                               "x2" => 5.0,
                               "y2" => 5.0,
                               "fill_color" => "white"})

    Gnome::CanvasRect.new(root,
                          {"x1" => 100.0,
                            "y1" => -30.0,
                            "x2" => 200.0,
                            "y2" => 30.0,
                            "fill_color" => "yellow",
                            "outline_color" => "yellow"})

    Gnome::CanvasRichText.new(root,
                              {"x" => 100.0,
                                "y" => -30.0,
                                "width" => 100.0,
                                "height" => 60.0,
                                "text" => "The quick brown fox jumped over the lazy dog.\n",
                                "cursor_visible" => true,
                                "cursor_blink" => true,
                                "grow_height" => true})

    Gnome::CanvasRect.new(root,
                          {"x1" => 50.0,
                            "y1" => 70.0,
                            "x2" => 150.0,
                            "y2" => 100.0,
                            "fill_color" => "pink",
                            "outline_color" => "pink"})

    Gnome::CanvasRichText.new(root,
                              {"x" => 50.0,
                                "y" => 70.0,
                                "width" => 100.0,
                                "height" => 30.0,
                                "text" => "This is a test.\nI enjoy tests a great deal\nThree lines!",
                                "cursor_visible" => true,
                                "cursor_blink" => true})
  end

  def initialize
    super(false, 4)
    self.border_width = 4
    self.show()

    alignment = Gtk::Alignment.new(0.5, 0.5, 0.0, 0.0)
    self.pack_start(alignment, true, true, 0)
    alignment.show()

    frame = Gtk::Frame.new()
    frame.shadow_type = Gtk::SHADOW_IN
    alignment.add(frame)
    frame.show()

    # Create the canvas and board

    canvas = Gnome::Canvas.new()
    canvas.set_usize(600, 450)
    frame.add(canvas)
    canvas.show()

    setup_text(canvas.root)
  end
end
