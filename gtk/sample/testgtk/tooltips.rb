=begin

  tooltips.rb - a part of testgtk.c rewritten in ruby-gtk

=end

require 'sample'

class TooltipsSample < SampleWindow
  def initialize
    super("Tooltips")
    set_policy true, false, true
    set_usize 200, 0

    tooltips = Gtk::Tooltips::new()

    button = Gtk::ToggleButton::new("button1")
    button.show
    tooltips.set_tip(button,"This is button 1", "ContextHelp/buttons/1")

    button2 = Gtk::ToggleButton::new("button2")
    button2.show
    tooltips.set_tip(button2,
		     "This is button 2. This is also a really long tooltip which probably won't fit on a single line and will therefore need to be wrapped. Hopefully the wrapping will work correctly.",
		     "ContextHelp/buttons/2_long")

    toggle = Gtk::ToggleButton::new("Override TipsQuery Label")
    toggle.show
    tooltips.set_tip(toggle, "Toggle TipsQuery view.", "Hi msw! ;)")

    tips_query = Gtk::TipsQuery::new()

    button3 = Gtk::Button::new("[?]")
    button3.show
    button3.signal_connect("clicked") do tips_query.start end
    tooltips.set_tip(button3,
		     "Start the Tooltips Inspector",
		     "ContextHelp/buttons/?")    
    tips_query.show
    tips_query.set_caller(button3)
    tips_query.signal_connect("widget_entered") do |tq, w, tip, tip_private|
      if (toggle.active?)
	tips_query.set(tip ? "There is a Tip!" : "There is no Tip!")
	tips_query.signal_emit_stop("widget_entered")
      end
    end
    tips_query.signal_connect("widget_selected") do |tq, w, tip, tip_private, b|
      if w
	print "Help \"#{tip_private ? tip_private : 'None'}\" requested for <#{w.type}>\n"
      end
      true
    end

    box3 = Gtk::VBox::new(false, 5)
    box3.border_width(5)
    box3.show
    box3.pack_start(button3, false, false, 0)
    box3.pack_start(tips_query, false, false, 0)

    frame = Gtk::Frame::new("ToolTips Inspector")
    frame.set_label_align(0.5, 0)
    frame.border_width(0)
    frame.add(box3)
    frame.show

    box2 = Gtk::VBox::new(false, 10)
    box2.border_width(10)
    box2.show
    box2.pack_start(button, true, true, 0)
    box2.pack_start(button2, true, true, 0)
    box2.pack_start(toggle, true, true, 0)
    box2.pack_start(frame, true, true, 10)

    separator = Gtk::HSeparator::new()
    separator.show

    button4 = Gtk::Button::new("close")
    button4.signal_connect("clicked") do destroy end
    button4.set_flags(Gtk::Widget::CAN_DEFAULT)
    button4.grab_default
    button4.show
    tooltips.set_tip(button4, "Push this button to close window", "ContextHelp/buttons/Close")

    box4 = Gtk::VBox::new(false, 10)
    box4.border_width(10)
    box4.show
    box4.pack_start(button4, true, true, 0)
    
    box1 = Gtk::VBox::new(false, 0)
    box1.show
    box1.pack_start(box2, true, true, 0)
    box1.pack_start(separator, false, true, 0)
    box1.pack_start(box4, false, true, 0)
    add(box1)
  end
end
