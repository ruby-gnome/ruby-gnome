# -*- ruby -*-

require './../glib2/lib/gnome2-raketask'

package = GNOME2Package.new do |_package|
  _package.summary = "Ruby/Pango is a Ruby binding of pango-1.x."
  _package.description = "Ruby/Pango is a Ruby binding of pango-1.x."
  _package.dependency.gem.runtime = [["cairo", ">= 1.10.0"], "glib2"]
  _package.win32.packages = ["pango"]
  _package.win32.dependencies = []
end
package.define_tasks

