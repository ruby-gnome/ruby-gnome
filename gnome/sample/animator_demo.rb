# Simple demonstration for the `Gnome::Animator' widget.  For now, it
# requires the `gnome-default.png', `mailcheck/tux-anim.xpm' and
# `mailcheck/email.xpm' icons to be installed in the GNOME pixmap
# directory (these should be in the `gnome-core' module).

require 'gtk'
require 'gdk_imlib'
require 'gnome'

class AnimatorDemo < Gnome::App
  def initialize
    super("gnome-animator", "Test!")
    realize

    signal_connect("delete_event") { Gtk::main_quit }

    the_box = Gtk::VBox.new(true, 0)

    @the_animator = Gnome::Animator.new(100, 100)

    # The following code loads the default GNOME foot and creates a
    # simple animation by changing its size frame-by-frame.

    # Warning: This is slow, and should be avoided in real programs
    # with long animations.  Load the file once with Imlib and use
    # `Gnome::Animator#append_from_imlib()' instead.
    s = Gnome::pixmap_file("gnome-default.png") ||
      Gnome::pixmap_file("gnome-default-dlg.png")
    @the_animator.append_frame_from_file(s, 36, 36, 200, 24, 24)
    @the_animator.append_frame_from_file(s, 30, 30, 50, 36, 36)
    @the_animator.append_frame_from_file(s, 28, 28, 50, 40, 40)
    @the_animator.append_frame_from_file(s, 24, 24, 50, 48, 48)
    @the_animator.append_frame_from_file(s, 23, 23, 50, 51, 51)
    @the_animator.append_frame_from_file(s, 17, 17, 50, 62, 62)
    @the_animator.append_frame_from_file(s, 9, 9, 50, 78, 78)
    @the_animator.append_frame_from_file(s, 3, 3, 50, 90, 90)
    @the_animator.append_frame_from_file(s, -2, -2, 50, 100, 100)
    @the_animator.append_frame_from_file(s, -12, -12, 50, 120, 120)
    @the_animator.append_frame_from_file(s, -27, -27, 50, 150, 150)
    @the_animator.append_frame_from_file(s, -52, -52, 50, 200, 200)
    @the_animator.append_frame_from_file(s, -102, -102, 100, 300, 300)

    # This puts the foot animation into a GtkButton, to demonstrate how
    # the animator can behave (almost) like a shaped windows without
    # actually being one.  If we used a real shaped window, the
    # animation would be a lot less smooth.

    # Also notice that the window size is never larger than the
    # specified size: the foot is always clipped to fit into this size,
    # even if the button becomes larger.
    the_button = Gtk::Button.new
    the_button.border_width = 4

    tmp = Gtk::HBox.new(true, 0)
    the_button.add(tmp)
    tmp.pack_start(@the_animator, false, true, 2)
    tmp.show

    # Clicking on the foot starts/stops its animation.
    the_box.pack_start(the_button, false, true, 0)
    the_button.signal_connect("clicked") { toggle_start_stop_cb }

    the_second_animator = Gnome::Animator.new(100, 100)

#    # This is the mandatory "jumping Tux" animation.  The animation is
#    # extracted from a large image, made up by tiling the frames
#    # horizontally.  The frames are quite small, so we get a chance to
#    # show how easy it is to magnify them with
#    # `gnome_animator_append_frames_from_file_at_size()'.
#    s = Gnome::pixmap_file("mailcheck/tux-anim.png")
#    the_second_animator.append_frames_from_file_at_size(
#		s, 0, 0, 300, 48, 100, 100)
#
#    the_box.pack_start(the_second_animator, false, true, 0)

    the_third_animator = Gnome::Animator.new(48, 48)

    # ...And this is another animation, similiar to the Tux one, but
    # without magnification and no shape.
    s = Gnome::pixmap_file("mailcheck/email.png")
    the_third_animator.append_frames_from_file(s, 0, 0, 150, 48)

    the_box.pack_start(the_third_animator, false, false, 0)

    set_contents(the_box)

    @the_animator.show

    the_button.show
  
    # the_second_animator.show

    the_third_animator.show

    show

    @the_animator.set_loop_type(Gnome::Animator::LOOP_PING_PONG)
    @the_animator.start

    the_second_animator.set_loop_type(Gnome::Animator::LOOP_RESTART)
    # This makes the animation twice as fast than the specified
    # interval values.  This feature is useful in case one wants to
    # increase the speed of the animation according to some custom
    # parameter, without having to re-create a new animation.
    the_second_animator.set_playback_speed(2.0)
    the_second_animator.start

    the_third_animator.set_loop_type(Gnome::Animator::LOOP_PING_PONG)
    the_third_animator.set_playback_speed(1.0)
    the_third_animator.start
  end

  private
  def toggle_start_stop_cb
    status = @the_animator.get_status
    if status == Gnome::Animator::STATUS_STOPPED then
      @the_animator.start
    else
      @the_animator.stop
    end
  end
end

demo = AnimatorDemo.new
Gtk::main
