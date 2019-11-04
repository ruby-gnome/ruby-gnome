# Getting started with GTK+ with the ruby-gnome2 Gtk3 module.

This is a ruby adaptation of the official tutorial for the C language that you can find at https://developer.gnome.org/gtk3/stable/gtk-getting-started.html.

GTK+ is a widget toolkit. Each user interface created by GTK+ consists of widgets. The Gtk3 module of the ruby-gnome2 project is an implementation of the ruby bindings for GTK+.

With Gtk3, Widgets are organized in a hierachy. The Gtk::Window widget is the main container. The user interface is then built by adding buttons, drop-down menus, input fields, and other widgets to the window.

If you are creating complex user interfaces it is recommended to use Gtk::Builder and its GTK-specific markup description language, instead of assembling the interface manually. You can also use a visual user interface editor, like Glade.

GTK+ is event-driven. The toolkit listens for events such as a click on a button, and passes the event to your application.

Here is the most basic example that illustrate the principles of widget hierarchy and events management:

```ruby
require "gtk3"

window = Gtk::Window.new("First example")
window.set_size_request(400, 400)
window.set_border_width(10)

button = Gtk::Button.new(:label => "Say hello")
button.signal_connect "clicked" do |_widget|
  puts "Hello World!!"
end

window.add(button)
window.signal_connect("delete-event") { |_widget| Gtk.main_quit }
window.show_all

Gtk.main
```
This tutorial will mainly be focused on the use of Gtk::Application, which is the best way to create an application.

## Table of Contents
* [Basics](#basics)
* [Packing](#packing)
* [Building user interfaces](#building-user-interfaces)
* [Building Applications](#building-applications)
  * [A trivial application](#a-trivial-application)
  * [Populating the window](#populating-the-window)
    * [Link a template to a custom class widget](#link-a-template-to-a-custom-class-widget)
    * [Load a resource file](#load-a-resource-file)
  * [Opening files](#opening-files)
  * [An application menu](#an-application-menu)
    * [Adding the menu interface](#adding-the-menu-interface)
    * [Linking menu items to actions](#linking-menu-items-to-actions)
    * [Add accelerators for an action](#add-accelerators-for-action)
  * [A preference dialog](#a-preference-dialog)
    * [Define and store settings for an application with gschemas](#define-and-store-settings-for-an-application-with-gschemas)
    * [Configure the settings with a dialog window](#configure-the-settings-with-a-dialog-window)
  * [Adding a search bar](#adding-a-search-bar)
  * [Adding a sidebar](#adding-a-sidebar)
  * [Properties](#properties)
  * [Header Bar](#header-bar)

## Basics
https://developer.gnome.org/gtk3/stable/gtk-getting-started.html#id-1.2.3.5

To begin our introduction to GTK, we'll start with a simple signal-based Gtk application. This program will create an empty 200 × 200 pixel window.

*    example-0.rb
```ruby
require "gtk3"

app = Gtk::Application.new("org.gtk.example", :flags_none)

app.signal_connect "activate" do |application|
  window = Gtk::ApplicationWindow.new(application)
  window.set_title("Window")
  window.set_default_size(200, 200)
  window.show_all
end

puts app.run

```
When creating a Gtk::Application you need to pick an application identifier (a name) and input to `Gtk::Application#new` as parameter. For this example *org.gtk.example* is used but for choosing an identifier for your application see this [guide](https://wiki.gnome.org/HowDoI/ChooseApplicationID).

Lastly `Gtk::Application#new` takes a `Gio::ApplicationFlags` constants as input for your application, if your application would have special needs (those constants can be replaced by theirs respective symbol ie. `Gio::ApplicationFlags::NONE` == `:flags_none`). You must know that `GApplication` ignores arguments passed to `g_application_run()` on the Windows systems. It always uses command line arguments even when we pass an empty array to g_application_run().

If you plan to create a cross-platform application, it is recommanded to use the `:handles_command_line` flags and the *command-line* signal. (reference : https://github.com/ruby-gnome2/ruby-gnome2/issues/721 ).

Next we add instructions for the "activate" event of the `Gtk::Application` instance we created. The activate signal will be sent when your application is launched with the method `Gtk::Application#run` on the line below. This method also takes as arguments a ruby array of string. This allows GTK+ to parse specific command line arguments that control the behavior of GTK+ itself. The parsed arguments will be removed from the array, leaving the unrecognized ones for your application to parse.

Inside the "activate" event block, we want to construct our GTK window, so that a window is shown when the application is launched. The call to `Gtk::ApplicationWindow#new` will create a new `Gtk::Window`. The window will have a frame, a title bar, and window controls depending on the platform.

A window title is set using `Gtk::Window#set_title`. This function takes a string as input. Finally the window size is set using `Gtk::Window#set_default_size` and the window is then shown by GTK via `Gtk::Widget#show_all`.

When you exit the window, by for example pressing the X, the `Gtk::Application#run` in the main loop returns with a number which is the exit status.

While the program is running, GTK+ is receiving *events*. These are typically input events caused by the user interacting with your program, but also things like messages from the window manager or other applications. GTK+ processes these and as a result, signals may be emitted on your widgets. Connecting handlers for these signals is how you normally make your program do something in response to user input.
The following example is slightly more complex, and tries to showcase some of the capabilities of GTK+.

In the long tradition of programming languages and libraries, it is called *Hello, World*.
*    example-1.rb
```ruby
require "gtk3"
app = Gtk::Application.new("org.gtk.example", :flags_none)

app.signal_connect "activate" do |application|
  window = Gtk::ApplicationWindow.new(application)
  window.set_title("Window")
  window.set_default_size(200, 200)

  button_box = Gtk::ButtonBox.new(:horizontal)
  window.add(button_box)

  button = Gtk::Button.new(label: "Hello World")
  button.signal_connect "clicked" do |widget|
    puts "Hello World"
    window.destroy
  end

  button_box.add(button)

  window.show_all
end

# Gtk::Application#run need C style argv ([prog, arg1, arg2, ...,argn]).
# The ARGV ruby variable only contains the arguments ([arg1, arg2, ...,argb])
# and not the program name. We have to add it explicitly.

puts app.run([$0] + ARGV)
```
As seen above, example-1.rb builds further upon example-0.rb by adding a button to our window, with the label "Hello World". Two new variables are created to accomplish this, button and button_box.

The button_box variable stores a `Gtk::ButtonBox` object, which is GTK+'s way of controlling the size and layout of buttons. The `Gtk::ButtonBox` is created with the method `Gtk::ButtonBox#new` which takes a `Gtk::Orientation `constant as parameter or the related symbols (`:vertical` or `:horizontal`).

The buttons which this box will contain can either be stored horizontally or vertically but this does not matter in this particular case as we are dealing with only one button. After initializing button_box with horizontal orientation, the code adds the button_box widget to the window widget using `Gtk::ButtonBox#add`.

Next the button variable is initialized in similar manner. The method `Gtk::Button#new` is called which returns a GtkButton to be stored inside button. A label is set using a ruby hash as argument :`:label => "Hello World"`.

Afterwards button is added to our button_box. Using the method "Gtk::Button#signal_connect" we add instructions, so that when the button is clicked, a message will be displayed in the terminal if the GTK application was started from one.

After that, `Gtk::Window#destroy` is called. This method is herited from `Gtk::Widget`. This has the effect that when the button is clicked, the whole GTK window is destroyed. More information about creating buttons can be found [here](https://wiki.gnome.org/HowDoI/Buttons).
The rest of the code in example-1.rb is identical to example-0.rb. Next section will elaborate further on how to add several GtkWidgets to your GTK application.

## Packing
https://developer.gnome.org/gtk3/stable/ch01s02.html

When creating an application, you'll want to put more than one widget inside a window. When you want to put more than one widget into a window, it it becomes important to control how each widget is positioned and sized. This is where packing comes in.

GTK+ comes with a large variety of layout containers whose purpose it is to control the layout of the child widgets that are added to them. See [Layout Containers](https://developer.gnome.org/gtk3/stable/LayoutContainers.html) for an overview.

The following example shows how the `Gtk::Grid` container lets you arrange several buttons:
*    example-2.rb

```ruby
require "gtk3"

app = Gtk::Application.new("org.gtk.example", :flags_none)

app.signal_connect "activate" do |application|
  # create a new window, and set its title
  window = Gtk::ApplicationWindow.new(application)
  window.set_title("Window")
  window.set_border_width(10)

  # Here we construct the container that is going pack our buttons
  grid = Gtk::Grid.new

  # Pack the container in the window
  window.add(grid)

  button = Gtk::Button.new(:label => "Button 1")
  button.signal_connect("clicked") { puts "Hello World" }
  # Place the first button in the grid cell (0, 0), and make it fill
  # just 1 cell horizontally and vertically (ie no spanning)
  grid.attach(button, 0, 0, 1, 1)

  button = Gtk::Button.new(:label => "Button 2")
  button.signal_connect("clicked") { puts "Hello World" }
  # Place the second button in the grid cell (1, 0), and make it fill
  # just 1 cell horizontally and vertically (ie no spanning)
  grid.attach(button, 1, 0, 1, 1)

  button = Gtk::Button.new(:label => "Quit")
  button.signal_connect("clicked") { window.destroy }
  # Place the Quit button in the grid cell (0, 1), and make it
  # span 2 columns.
  grid.attach(button, 0, 1, 2, 1)

  # Now that we are done packing our widgets, we show them all
  # in one go, by calling Gtk::Widget#show_all on the window.
  # This call recursively calls Gtk::Widget#show on all widgets
  # that are contained in the window, directly or indirectly
  window.show_all
end

# Gtk::Application#run need C style argv ([prog, arg1, arg2, ...,argn]).
# The ARGV ruby variable only contains the arguments ([arg1, arg2, ...,argb])
# and not the program name. We have to add it explicitly.

status = app.run([$0] + ARGV)

puts status
```
## Building user interfaces
https://developer.gnome.org/gtk3/stable/ch01s03.html

When construcing a more complicated user interface, with dozens or hundreds of widgets, doing all the setup work in code is cumbersome, and making changes becomes next to impossible.
Thankfully, GTK+ supports the separation of user interface layout from your business logic, by using UI descriptions in an XML format that can be parsed by the `Gtk::Builder` class.

*    example-4.rb : Packing buttons with GtkBuilder

```ruby
require "gtk3"

builder_file = "#{File.expand_path(File.dirname(__FILE__))}/builder.ui"

# Construct a Gtk::Builder instance and load our UI description
builder = Gtk::Builder.new(:file => builder_file)

# Connect signal handlers to the constructed widgets
window = builder.get_object("window")
window.signal_connect("destroy") { Gtk.main_quit }

button = builder.get_object("button1")
button.signal_connect("clicked") { puts "Hello World" }

button = builder.get_object("button2")
button.signal_connect("clicked") { puts "Hello World" }

button = builder.get_object("quit")
button.signal_connect("clicked") { Gtk.main_quit }

Gtk.main
```

Here is the "builder.ui" file that describes the interface:

```xml
<interface>
  <object id="window" class="GtkWindow">
    <property name="visible">True</property>
    <property name="title">Grid</property>
    <property name="border-width">10</property>
    <child>
      <object id="grid" class="GtkGrid">
        <property name="visible">True</property>
        <child>
          <object id="button1" class="GtkButton">
            <property name="visible">True</property>
            <property name="label">Button 1</property>
          </object>
          <packing>
            <property name="left-attach">0</property>
            <property name="top-attach">0</property>
          </packing>
        </child>
        <child>
          <object id="button2" class="GtkButton">
            <property name="visible">True</property>
            <property name="label">Button 2</property>
          </object>
          <packing>
            <property name="left-attach">1</property>
            <property name="top-attach">0</property>
          </packing>
        </child>
        <child>
          <object id="quit" class="GtkButton">
            <property name="visible">True</property>
            <property name="label">Quit</property>
          </object>
          <packing>
            <property name="left-attach">0</property>
            <property name="top-attach">1</property>
            <property name="width">2</property>
          </packing>
        </child>
      </object>
      <packing>
      </packing>
    </child>
  </object>
</interface>
```

The usage of the `Gtk::Builder` is really easy, we just create an instance from the
file "builder.ui" with `Gtk::Builder.new(:file => builder_file)`. Then you can access every widget or part of the interface thanks to its name: `window = builder.get_object("window")`. Note that `Gtk::Builder` can also be used to construct objects that are not widgets, such as tree models, adjustments, etc.

The XML definition of the interface can be loaded from a file, a string or a path in a gresource binary. More informations related to this XML definition can be found [here](https://developer.gnome.org/gtk3/stable/GtkBuilder.html#BUILDER-UI). Those files are generally built with [glade](https://glade.gnome.org/).


## Building applications

### A trivial application
https://developer.gnome.org/gtk3/stable/ch01s04.html#id-1.2.3.12.5

*    exampleapp1/exampleapp.rb
```ruby
require "gtk3"

class ExampleAppWindow < Gtk::ApplicationWindow

  def open(file)

  end
end

class ExampleApp < Gtk::Application
  def initialize
    super("org.gtk.exampleapp", :handles_open)

    signal_connect "activate" do |application|
      window = ExampleAppWindow.new(application)
      window.present
    end
    signal_connect "open" do |application, files, hin|
      windows = application.windows
      win = nil
      unless windows.empty?
        win = windows.first
      else
        win = ExampleAppWindow.new(application)
      end

      files.each { |file| win.open(file) }

      win.present
    end
  end
end

app = ExampleApp.new

puts app.run([$0]+ARGV)
```

In this example we create a subclass of `Gtk::Application` called ExampleApp. In the `ExampleApp#initialize` method, we add instructions for two signals *activate* and *open*. Every `Gtk::Application` object or its subclass object can react to 4 signals:

*    startup : sets up the application when it first start
*    shutdown : preforms shutdown tasks
*    activate : shows the default first window of the application
*    open : opens files and shows them in a new window

For more informations, see [here](https://wiki.gnome.org/HowDoI/GtkApplication).
In this case, the signal "*activate*" will be triggered if no arguments are given to the `ExampleApp#run` method. And a default window will be created and will be presented to the user ( [`Gtk::Widget#present`](https://developer.gnome.org/gtk3/stable/GtkWindow.html#gtk-window-present)).

If file names are given to the `ExampleApp#run` method, then it is the "*open*" signal that is called.
Trought this event, you can manage the files that are stored in an array of `Gio::File` objects.

In this example, each files are used by an `ExampleAppWindow#open` method. The `ExampleAppWindow` class is derived from the `Gtk::ApplicationWindow`.

This does not look very impressive yet, but our application is already presenting itself on the session bus, and it accepts files as commandline arguments.


### Populating the window
https://developer.gnome.org/gtk3/stable/ch01s04.html#id-1.2.3.12.6

*    exampleapp2/exampleapp.rb

In this step, we use a [`Gtk::Builder`](https://developer.gnome.org/gtk3/stable/GtkBuilder.html) template to associate a [`Gtk::Builder`](https://developer.gnome.org/gtk3/stable/GtkBuilder.html) ui file with our application window class.
Our simple ui file puts a [`Gtk::HeaderBar`](https://developer.gnome.org/gtk3/stable/GtkHeaderBar.html) on top of a [`Gtk::Stack`](https://developer.gnome.org/gtk3/stable/GtkStack.html) widget. The header bar contains a [`Gtk::StackSwitcher`](https://developer.gnome.org/gtk3/stable/GtkStackSwitcher.html), which is a standalone widget to show a row of 'tabs' for the pages of a [`Gtk::Stack`](https://developer.gnome.org/gtk3/stable/GtkStack.html) .

Here is the "window.ui" file that contains the template of the window:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<interface>
  <!-- interface-requires gtk+ 3.8 -->
  <template class="ExampleAppWindow" parent="GtkApplicationWindow">
    <property name="title" translatable="yes">Example Application</property>
    <property name="default-width">600</property>
    <property name="default-height">400</property>
    <child>
      <object class="GtkBox" id="content_box">
        <property name="visible">True</property>
        <property name="orientation">vertical</property>
        <child>
          <object class="GtkHeaderBar" id="header">
            <property name="visible">True</property>
            <child type="title">
              <object class="GtkStackSwitcher" id="tabs">
                <property name="visible">True</property>
                <property name="margin">6</property>
                <property name="stack">stack</property>
              </object>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkStack" id="stack">
            <property name="visible">True</property>
          </object>
        </child>
      </object>
    </child>
  </template>
</interface>
```
Unlike regular interface descriptions, in template XML descriptions, a`<template>` tag is expected as a direct child of the toplevel `<interface>` tag. The `<template>` tag must specify the "*class*" attribute which must be the class name of the widget. Optionally, the "*parent*" attribute may be specified to indicate the direct parent class (superclass).

More informations can be found in the part [building composite widgets from template XML](https://developer.gnome.org/gtk3/stable/GtkWidget.html#GtkWidget.description) of the `Gtk::Widget` documentation.

#### Link a template to a custom class widget.

```ruby
class ExampleAppWindow < Gtk::ApplicationWindow
  type_register
  class << self
    def init
      set_template(:resource => "/org/gtk/exampleapp/window.ui")
    end
  end

  def initialize(application)
    super(:application => application)
  end

  def open(file)

  end
end
```

We create a subclass of Gtk::ApplicationWindow. Then we call the method `type_register` inherited from `GLib::Object` in order to register this class as a new [GType](https://developer.gnome.org/gobject/stable/chapter-gtype.html). See the file *ruby-gnome2/glib2/ext/glib2/rbgobj_object.c* for the C implementation. More informations on the gobject manipulation can be found [here](https://blogs.gnome.org/desrt/2012/02/26/a-gentle-introduction-to-gobject-construction/)

The template of the interface is bound to the class using the `init` singleton method. We just open the *eigenclass*  with `class << self` and define the method `init` in which we call the `Gtk::Widget#set_template` method.

After that, the `ExampleAppWindow#initialize` method must be overwritten. When `type_register` is used, *super* is equivalent to `GLib::Object#initialize` so you need to use properties style constructor (hash argument, see [here](https://github.com/ruby-gnome2/ruby-gnome2/issues/503))

#### Load a resource file.

You may have noticed that we used the `:resource => ` key as the argument of the method that sets a template. Now we need to use GLib's resource functionality to include the ui file in the binary. This is commonly done by listing all resources in a .gresource.xml file, such as this:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<gresources>
  <gresource prefix="/org/gtk/exampleapp">
    <file preprocess="xml-stripblanks">window.ui</file>
  </gresource>
</gresources>
```
In our script, we built the resource binary file with
```ruby
system("glib-compile-resources",
       "--target", gresource_bin,
       "--sourcedir", File.dirname(gresource_xml),
       gresource_xml)
```
Then we make sure that this file is deleted when the script is done :

```ruby
at_exit do
  FileUtils.rm_f(gresource_bin)
end
```
The resource binary file is loaded so that each resources in it can be accessed via theirs respective paths.

```ruby
resource = Gio::Resource.load(gresource_bin)
Gio::Resources.register(resource)
```

### Opening files
https://developer.gnome.org/gtk3/stable/ch01s04.html#id-1.2.3.12.7

*    exampleapp3/exampleapp.rb

In this step, we make our application show the content of all the files that it is given on the commandline.
To this end, we need to easily access widgets from the template. So we use the method `Gtk::Widget.bind_template_child` like in the code below:

```ruby
class ExampleAppWindow < Gtk::ApplicationWindow
  type_register
  class << self
    def init
      set_template(:resource => "/org/gtk/exampleapp/window.ui")
      bind_template_child("stack")
    end
  end
```
Most of this piece of code have been seen previously, a window widget interface is defined with a template. But the usage of `bind_template_child("stack")` will add a method, to each ExampleAppWindow instance, which name will be `ExampleAppWindow#stack` and that will return the corresponding child widget in the template.

The initial implementation of `Gtk::Widget.bind_template_child` have been done in this [pull request](https://github.com/ruby-gnome2/ruby-gnome2/pull/445)

This new way to access the stack widget is used in the following code. We have previously handled the *open* signal in our application like this:

```ruby
class ExampleAppWindow < Gtk::ApplicationWindow
  def open(file)

  end
end

class ExampleApp < Gtk::Application
  def initialize
    # ...

    signal_connect "open" do |application, files, hin|
      windows = application.windows
      win = nil
      unless windows.empty?
        win = windows.first
      else
        win = ExampleAppWindow.new(application)
      end

      files.each { |file| win.open(file) }

      win.present
    end
  end
end
```

The `open` method of the main window is called with for each file. Now we manage those files like this:

```ruby
def open(file)
  basename = file.basename
  scrolled = Gtk::ScrolledWindow.new
  scrolled.show
  scrolled.set_hexpand(true)
  scrolled.set_vexpand(true)
  view = Gtk::TextView.new
  view.set_editable(false)
  view.set_cursor_visible(false)
  view.show
  scrolled.add(view)
  stack.add_titled(scrolled, basename, basename)
  stream = file.read
  view.buffer.text = stream.read
end
```
Each file is opened and loaded in a `Gtk::TextView` with

```ruby
  stream = file.read
  view.buffer.text = stream.read
```

We get the basename, of the file in argument, that will be used as title for each tab of the stack widget:

```
stack.add_titled(scrolled, basename, basename)
```

In this line, given that `self` is `ExampleAppWindow` the usage of `stack` is a call to the method we have created previously. So here we add a tab with a `Gtk::ScrolledWindow` in the `Gtk::Stack` widget of our template and we display the file content.

### An application menu
https://developer.gnome.org/gtk3/stable/ch01s04.html#id-1.2.3.12.8

*    exampleapp4/exampleapp.rb

An application menu is shown by GNOME shell at the top of the screen. It is meant to collect infrequently used actions that affect the whole application.
Just like the window template, we specify our application menu in a ui file, and add it as a resource to our binary.

#### Adding the menu interface

*    app-menu.ui
```xml
<?xml version="1.0"?>
<interface>
  <!-- interface-requires gtk+ 3.0 -->
  <menu id="appmenu">
    <section>
      <item>
        <attribute name="label" translatable="yes">_Preferences</attribute>
        <attribute name="action">app.preferences</attribute>
      </item>
    </section>
    <section>
      <item>
        <attribute name="label" translatable="yes">_Quit</attribute>
        <attribute name="action">app.quit</attribute>
      </item>
    </section>
  </menu>
</interface>
```

This menu interface is loaded and added to our application with :

```ruby
builder = Gtk::Builder.new(:resource => "/org/gtk/exampleapp/app-menu.ui")
app_menu = builder.get_object("appmenu")
application.set_app_menu(app_menu)
```

With this, our application has a menu with two items that we can show when clicking on the application icon in the Gnome shell.

#### Linking menu items to actions.

All the actions initialization should be done during the "startup" step, which is guaranteed to be called once for each primary application instance.

The "quit" item in the menu is implemented with this:

```xml
<section>
  <item>
    <attribute name="label" translatable="yes">_Quit</attribute>
    <attribute name="action">app.quit</attribute>
  </item>
</section>
```

Then we just have to create an `Gio::SimpleAction` named "quit" and configure it in order to quit the application when this action is triggered.

```ruby
action = Gio::SimpleAction.new("quit")
action.signal_connect("activate") do |_action, parameter|
  application.quit
end
application.add_action(action)

```

#### Add accelerators for action.

An accelerator is just a keys combination that acts as a shortcut for an action.

```ruby
quit_accels = ["<Ctrl>Q"]
action = Gio::SimpleAction.new("quit")
action.signal_connect("activate") do |_action, parameter|
  application.quit
end
application.add_action(action)
application.set_accels_for_action("app.quit", quit_accels)
```

### A preference dialog
https://developer.gnome.org/gtk3/stable/ch01s04.html#id-1.2.3.12.9

#### Define and store settings for an application with gschemas
*    exampleapp5/exampleapp.rb

A typical application will have a some preferences that should be remembered from one run to the next. Even for our simple example application, we may want to change the font that is used for the content.
We are going to use `Gio::Settings` to store our preferences. `Gio::Settings` requires a schema that describes our settings:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<schemalist>
  <schema path="/org/gtk/exampleapp/" id="org.gtk.exampleapp">
    <key name="font" type="s">
      <default>'Monospace 12'</default>
      <summary>Font</summary>
      <description>The font to be used for content.</description>
    </key>
    <key name="transition" type="s">
      <choices>
        <choice value='none'/>
        <choice value='crossfade'/>
        <choice value='slide-left-right'/>
      </choices>
      <default>'none'</default>
      <summary>Transition</summary>
      <description>The transition to use when switching tabs.</description>
    </key>
  </schema>
</schemalist>
```

Before we can make use of this schema in our application, we need to compile it into the binary form that `Gio::Settings` expects.

```ruby
system("glib-compile-schemas", data_path)
```
In this command, the tool *glib-compile-schemas* searches all files with a name that ends with *.gschema.xml* and compiles it into a binary file called *gschemas.compiled*.

This binary file can be loaded and our custom schema installed  at the begining of our script with:

```ruby
Gio::SettingsSchemaSource.new(data_path,
                              Gio::SettingsSchemaSource.default,
                              false)

```
This is the corresponding method to the function [g_settings_schema_source_new_from_directory](https://developer.gnome.org/gio/stable/gio-GSettingsSchema-GSettingsSchemaSource.html#g-settings-schema-source-new-from-directory).

As an alternative, our schema  can just be loaded by using the `GSETTINGS_SCHEMA_DIR` environment variable.

```ruby
ENV["GSETTINGS_SCHEMA_DIR"] = data_path
```
More informations on the use of gschemas can be found [here](https://developer.gnome.org/gio/stable/gio-GSettingsSchema-GSettingsSchemaSource.html)

Next, we need to connect our settings to the widgets that they are supposed to control. One convenient way to do this is to use `Gio::Settings` bind functionality to bind settings keys to object properties, as we do here for the transition setting. The `Gio::Settings#bind` is the ruby method for the [g_settings_bind](https://developer.gnome.org/gio/stable/GSettings.html#g-settings-bind) fonction of `gio`

```ruby
class ExampleAppWindow < Gtk::ApplicationWindow
  type_register
  class << self
    def init
      set_template(:resource => "/org/gtk/exampleapp/window.ui")
      bind_template_child("stack")
    end
  end

  def initialize(application)
    super(:application => application)
    settings = Gio::Settings.new("org.gtk.exampleapp")
    settings.bind("transition",
                  stack,
                  "transition-type",
                  Gio::SettingsBindFlags::DEFAULT)
  end
...
```

#### Configure the settings with a dialog window

*    exampleapp6/exampleapp.rb

The code to connect the font setting is a little more involved, since there is no simple object property that it corresponds to, so we are not going to go into that here.
At this point, the application will already react if you change one of the settings, e.g. using the gsettings commandline tool. Of course, we expect the application to provide a preference dialog for these. So lets do that now. Our preference dialog will be a subclass of `Gtk::Dialog`, and we'll use the same techniques that we've already seen: templates, bind child widget name to method, settings bindings.

Lets start with the template.

```xml
<?xml version="1.0" encoding="UTF-8"?>
<interface>
  <!-- interface-requires gtk+ 3.8 -->
  <template class="ExampleAppPrefs" parent="GtkDialog">
    <property name="title" translatable="yes">Preferences</property>
    <property name="resizable">False</property>
    <property name="modal">True</property>
    <child internal-child="vbox">
      <object class="GtkBox" id="vbox">
        <child>
          <object class="GtkGrid" id="grid">
            <property name="visible">True</property>
            <property name="margin">6</property>
            <property name="row-spacing">12</property>
            <property name="column-spacing">6</property>
            <child>
              <object class="GtkLabel" id="fontlabel">
                <property name="visible">True</property>
                <property name="label">_Font:</property>
                <property name="use-underline">True</property>
                <property name="mnemonic-widget">font</property>
                <property name="xalign">1</property>
              </object>
              <packing>
                <property name="left-attach">0</property>
                <property name="top-attach">0</property>
              </packing>
            </child>
            <child>
              <object class="GtkFontButton" id="font">
                <property name="visible">True</property>
              </object>
              <packing>
                <property name="left-attach">1</property>
                <property name="top-attach">0</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="transitionlabel">
                <property name="visible">True</property>
                <property name="label">_Transition:</property>
                <property name="use-underline">True</property>
                <property name="mnemonic-widget">transition</property>
                <property name="xalign">1</property>
              </object>
              <packing>
                <property name="left-attach">0</property>
                <property name="top-attach">1</property>
              </packing>
            </child>
            <child>
              <object class="GtkComboBoxText" id="transition">
                <property name="visible">True</property>
                <items>
                  <item translatable="yes" id="none">None</item>
                  <item translatable="yes" id="crossfade">Fade</item>
                  <item translatable="yes" id="slide-left-right">Slide</item>
                </items>
              </object>
              <packing>
                <property name="left-attach">1</property>
                <property name="top-attach">1</property>
              </packing>
            </child>
          </object>
        </child>
      </object>
    </child>
  </template>
</interface>
```
Next comes the dialog subclass.

```ruby
class ExampleAppPrefs < Gtk::Dialog
  type_register
  class << self
    def init
      set_template(:resource => "/org/gtk/exampleapp/prefs.ui")
      bind_template_child("font")
      bind_template_child("transition")
    end
  end
  def initialize(args)
    parent = args[:transient_for]
    bar = args[:use_header_bar]
    super(:transient_for => parent, :use_header_bar => 1)
    settings = Gio::Settings.new("org.gtk.exampleapp")
    settings.bind("font",
                  font,
                  "font",
                  Gio::SettingsBindFlags::DEFAULT)
    settings.bind("transition",
                  transition,
                  "active-id",
                  Gio::SettingsBindFlags::DEFAULT)
  end
end
```

Nothing new here, it works like previously. The main difference in our application now is that we define what must be done when the *preferences* menu item is clicked.

```ruby
action = Gio::SimpleAction.new("preferences")
action.signal_connect("activate") do |_action, _parameter|
  win = application.windows.first

  prefs = ExampleAppPrefs.new(:transient_for => win,
                              :use_header_bar => true)
  prefs.present
end
```
Here we just says that when the user activate the *preferences* item, we create an `ExampleAppPrefs` instance and display it. The user can then specify the *font* and *transition* settings for the application. Those settings are used in the `ExampleAppWindow#open` method for example:

```ruby
def open(file)
  basename = file.basename
  scrolled = Gtk::ScrolledWindow.new
  scrolled.show
  scrolled.set_hexpand(true)
  scrolled.set_vexpand(true)
  view = Gtk::TextView.new
  view.set_editable(false)
  view.set_cursor_visible(false)
  view.show
  scrolled.add(view)
  stack.add_titled(scrolled, basename, basename)
  stream = file.read
  buffer = view.buffer
  buffer.text = stream.read
  tag = buffer.create_tag()
  @settings.bind("font", tag, "font", Gio::SettingsBindFlags::DEFAULT)
  buffer.apply_tag(tag, buffer.start_iter, buffer.end_iter)
end
```

### Adding a search bar
https://developer.gnome.org/gtk3/stable/ch01s04.html#id-1.2.3.12.10

*    exampleapp7/exampleapp.rb

We continue to flesh out the functionality of our application. For now, we add search. GTK+ supports this with `Gtk::SearchEntry` and `Gtk::SearchBar`. The search bar is a widget that can slide in from the top to present a search entry.
We add a toggle button to the header bar, which can be used to slide out the search bar below the header bar.

The new window.ui file:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<interface>
  <!-- interface-requires gtk+ 3.8 -->
  <template class="ExampleAppWindow" parent="GtkApplicationWindow">
    <property name="title" translatable="yes">Example Application</property>
    <property name="default-width">600</property>
    <property name="default-height">400</property>
    <child>
      <object class="GtkBox" id="content_box">
        <property name="visible">True</property>
        <property name="orientation">vertical</property>
        <child>
          <object class="GtkHeaderBar" id="header">
            <property name="visible">True</property>
            <child type="title">
              <object class="GtkStackSwitcher" id="tabs">
                <property name="visible">True</property>
                <property name="margin">6</property>
                <property name="stack">stack</property>
              </object>
            </child>
            <child>
              <object class="GtkToggleButton" id="search">
                <property name="visible">True</property>
                <property name="sensitive">False</property>
                <style>
                  <class name="image-button"/>
                </style>
                <child>
                  <object class="GtkImage" id="search-icon">
                    <property name="visible">True</property>
                    <property name="icon-name">edit-find-symbolic</property>
                    <property name="icon-size">1</property>
                  </object>
                </child>
              </object>
              <packing>
                <property name="pack-type">end</property>
              </packing>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkSearchBar" id="searchbar">
            <property name="visible">True</property>
            <child>
              <object class="GtkSearchEntry" id="searchentry">
                <signal name="search-changed" handler="search_text_changed"/>
                <property name="visible">True</property>
              </object>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkStack" id="stack">
            <signal name="notify::visible-child" handler="visible_child_changed"/>
            <property name="visible">True</property>
          </object>
        </child>
      </object>
    </child>
  </template>
</interface>
```
Implementing the search needs quite a few code changes that we are not going to completely go over here. The central piece of the search implementation is a signal handler that listens for text changes in the search entry.

```ruby

def init
  set_template(:resource => "/org/gtk/exampleapp/window.ui")
  bind_template_child("stack")
  bind_template_child("search")
  bind_template_child("searchbar")
  set_connect_func do |name|
    method(name)
  end
end

private

def search_text_changed(search_entry)
  text = search_entry.text
  return if text.empty?

  win = search_entry.toplevel
  tab = win.stack.visible_child
  view = tab.child
  buffer = view.buffer
  range = buffer.start_iter.forward_search(text, Gtk::TextSearchFlags::CASE_INSENSITIVE)
  return unless range
  buffer.select_range(range[0], range[1])
  view.scroll_to_iter(range[0], 0.0, false, 0.0, 0.0)
end
```

In this part of code, the use of the method `set_connect_func` will allow us to define private methods as callback that have been set in the handler attributs of the XML ui file. In the code above, we can see that we have defined a private method name `search_text_changed`. In the XML file, there is this line :

    <signal name="search-changed" handler="search_text_changed"/>

Those pieces together mean that the signal *search-changed* of the `Gtk::SearchEntry`, trigger the private method of `ExampleAppWindow` that is called `search_text_changed`.

### Adding a sidebar
https://developer.gnome.org/gtk3/stable/ch01s04.html#id-1.2.3.12.11

*    exampleapp8/exampleapp.rb

As another piece of functionality, we are adding a sidebar, which demonstrates `Gtk::MenuButton`, `Gtk::Revealer` and `Gtk::ListBox`.

window.ui :

```xml
<?xml version="1.0" encoding="UTF-8"?>
<interface>
  <!-- interface-requires gtk+ 3.8 -->
  <template class="ExampleAppWindow" parent="GtkApplicationWindow">
    <property name="title" translatable="yes">Example Application</property>
    <property name="default-width">600</property>
    <property name="default-height">400</property>
    <child>
      <object class="GtkBox" id="content_box">
        <property name="visible">True</property>
        <property name="orientation">vertical</property>
        <child>
          <object class="GtkHeaderBar" id="header">
            <property name="visible">True</property>
            <child type="title">
              <object class="GtkStackSwitcher" id="tabs">
                <property name="visible">True</property>
                <property name="margin">6</property>
                <property name="stack">stack</property>
              </object>
            </child>
            <child>
              <object class="GtkToggleButton" id="search">
                <property name="visible">True</property>
                <property name="sensitive">False</property>
                <style>
                  <class name="image-button"/>
                </style>
                <child>
                  <object class="GtkImage" id="search-icon">
                    <property name="visible">True</property>
                    <property name="icon-name">edit-find-symbolic</property>
                    <property name="icon-size">1</property>
                  </object>
                </child>
              </object>
              <packing>
                <property name="pack-type">end</property>
              </packing>
            </child>
            <child>
              <object class="GtkMenuButton" id="gears">
                <property name="visible">True</property>
                <property name="direction">none</property>
                <property name="use-popover">True</property>
                <style>
                  <class name="image-button"/>
                </style>
              </object>
              <packing>
                <property name="pack-type">end</property>
              </packing>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkSearchBar" id="searchbar">
            <property name="visible">True</property>
            <child>
              <object class="GtkSearchEntry" id="searchentry">
                <signal name="search-changed" handler="search_text_changed"/>
                <property name="visible">True</property>
              </object>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkBox" id="hbox">
            <property name="visible">True</property>
            <child>
              <object class="GtkRevealer" id="sidebar">
                <property name="visible">True</property>
                <property name="transition-type">slide-right</property>
                <child>
                 <object class="GtkScrolledWindow" id="sidebar-sw">
                   <property name="visible">True</property>
                   <property name="hscrollbar-policy">never</property>
                   <property name="vscrollbar-policy">automatic</property>
                   <child>
                     <object class="GtkListBox" id="words">
                       <property name="visible">True</property>
                       <property name="selection-mode">none</property>
                     </object>
                   </child>
                 </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkStack" id="stack">
                <signal name="notify::visible-child" handler="visible_child_changed"/>
                <property name="visible">True</property>
              </object>
            </child>
          </object>
        </child>
      </object>
    </child>
  </template>
</interface>
```

The code to populate the sidebar with buttons for the words found in each file is a little too involved to go into here. But we'll look at the code to add the gears menu.
As expected by now, the gears menu is specified in a GtkBuilder ui file.

gears-menu.ui

```xml
<?xml version="1.0"?>
<interface>
  <!-- interface-requires gtk+ 3.0 -->
  <menu id="menu">
    <section>
      <item>
        <attribute name="label" translatable="yes">_Words</attribute>
        <attribute name="action">win.show-words</attribute>
      </item>
    </section>
  </menu>
</interface>
```
To connect the menuitem to the show-words setting, we use a `Gio::SimpleAction` corresponding to the given `Gio::Settings` key.

```ruby
class ExampleAppWindow < Gtk::ApplicationWindow
  # some code
  def initialize(application)
    super(:application => application)
    @settings = Gio::Settings.new("org.gtk.exampleapp")
    @settings.bind("transition",
                   stack,
                   "transition-type",
                   Gio::SettingsBindFlags::DEFAULT)
    search.bind_property("active", searchbar, "search-mode-enabled", :bidirectional)
    @settings.bind("show-words",
                   sidebar,
                   "reveal-child",
                   Gio::SettingsBindFlags::DEFAULT)
    sidebar.signal_connect "notify::reveal-child" do |_sidebar, _gparamspec|
      update_words(self)
    end
    builder = Gtk::Builder.new(:resource => "/org/gtk/exampleapp/gears-menu.ui")
    menu = builder.get_object("menu")
    gears.set_menu_model(menu)
    action = @settings.create_action("show-words")
    add_action(action)
  end

  # some code
end
```

### Properties

https://developer.gnome.org/gtk3/stable/ch01s04.html#id-1.2.3.12.12

Widgets and other objects have many useful properties.

Here we show some ways to use them in new and flexible ways, by wrapping them in actions with `Gio::PropertyAction` or by binding them with `Gio::Binding`.
To set this up, we add two labels to the header bar in our window template, named *lines_label* and *lines*, and bind them to struct members in the private struct, as we've seen a couple of times by now.
We add a new "Lines" menu item to the gears menu, which triggers the show-lines action:

```xml
<?xml version="1.0"?>
<interface>
  <!-- interface-requires gtk+ 3.0 -->
  <menu id="menu">
    <section>
      <item>
        <attribute name="label" translatable="yes">_Words</attribute>
        <attribute name="action">win.show-words</attribute>
      </item>
      <item>
        <attribute name="label" translatable="yes">_Lines</attribute>
        <attribute name="action">win.show-lines</attribute>
      </item>
    </section>
  </menu>
</interface>
```

To make this menu item do something, we create a property action for the visible property of the lines label, and add it to the actions of the window. The effect of this is that the visibility of the label gets toggled every time the action is activated.
Since we want both labels to appear and disappear together, we bind the visible property of the lines_label widget to the same property of the lines widget.

*    exampleapp9/exampleapp.rb

```ruby
# ...
class ExampleAppWindow < Gtk::ApplicationWindow
# ...
  def initialize(application)
    super(:application => application)
# ...
    action = Gio::PropertyAction.new("show-lines", lines, "visible")
    add_action(action)
    lines.bind_property("visible", lines_label, "visible", :default)
  end
```
We also need a function that counts the lines of the currently active tab, and updates the lines label. See the full source if you are interested in the details.

### Header Bar

https://developer.gnome.org/gtk3/stable/ch01s04.html#id-1.2.3.12.13

Our application already uses a `Gtk::HeaderBar`, but so far it still gets a 'normal' window titlebar on top of that. This is a bit redundant, and we will now tell GTK+ to use the header bar as replacement for the titlebar. To do so, we move it around to be a direct child of the window, and set its type to be titlebar.

```xml
<?xml version="1.0" encoding="UTF-8"?>
<interface>
  <!-- interface-requires gtk+ 3.8 -->
  <template class="ExampleAppWindow" parent="GtkApplicationWindow">
    <property name="title" translatable="yes">Example Application</property>
    <property name="default-width">600</property>
    <property name="default-height">400</property>
        <child type="titlebar">
          <object class="GtkHeaderBar" id="header">
            <property name="visible">True</property>
            <property name="show-close-button">True</property>
            <child>
              <object class="GtkLabel" id="lines_label">
                <property name="visible">False</property>
                <property name="label" translatable="yes">Lines:</property>
              </object>
              <packing>
                <property name="pack-type">start</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="lines">
                <property name="visible">False</property>
              </object>
              <packing>
                <property name="pack-type">start</property>
              </packing>
            </child>
            <child type="title">
              <object class="GtkStackSwitcher" id="tabs">
                <property name="visible">True</property>
                <property name="margin">6</property>
                <property name="stack">stack</property>
              </object>
            </child>
            <child>
              <object class="GtkToggleButton" id="search">
                <property name="visible">True</property>
                <property name="sensitive">False</property>
                <style>
                  <class name="image-button"/>
                </style>
                <child>
                  <object class="GtkImage" id="search-icon">
                    <property name="visible">True</property>
                    <property name="icon-name">edit-find-symbolic</property>
                    <property name="icon-size">1</property>
                  </object>
                </child>
              </object>
              <packing>
                <property name="pack-type">end</property>
              </packing>
            </child>
            <child>
              <object class="GtkMenuButton" id="gears">
                <property name="visible">True</property>
                <property name="direction">none</property>
                <property name="use-popover">True</property>
                <style>
                  <class name="image-button"/>
                </style>
              </object>
              <packing>
                <property name="pack-type">end</property>
              </packing>
            </child>
          </object>
        </child>
    <child>
      <object class="GtkBox" id="content_box">
        <property name="visible">True</property>
        <property name="orientation">vertical</property>
        <child>
          <object class="GtkSearchBar" id="searchbar">
            <property name="visible">True</property>
            <child>
              <object class="GtkSearchEntry" id="searchentry">
                <signal name="search-changed" handler="search_text_changed"/>
                <property name="visible">True</property>
              </object>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkBox" id="hbox">
            <property name="visible">True</property>
            <child>
              <object class="GtkRevealer" id="sidebar">
                <property name="visible">True</property>
                <property name="transition-type">slide-right</property>
                <child>
                 <object class="GtkScrolledWindow" id="sidebar-sw">
                   <property name="visible">True</property>
                   <property name="hscrollbar-policy">never</property>
                   <property name="vscrollbar-policy">automatic</property>
                   <child>
                     <object class="GtkListBox" id="words">
                       <property name="visible">True</property>
                       <property name="selection-mode">none</property>
                     </object>
                   </child>
                 </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkStack" id="stack">
                <signal name="notify::visible-child" handler="visible_child_changed"/>
                <property name="visible">True</property>
              </object>
            </child>
          </object>
        </child>
      </object>
    </child>
  </template>
</interface>
```

A small extra bonus of using a header bar is that we get a fallback application menu for free.
