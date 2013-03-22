require 'gobject-introspection'
require 'gtk2'

base_dir = Pathname.new(__FILE__).dirname.dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_environment_path(vendor_bin_dir)

module WebKitGtk2
	@initialized = false
	class << self
		def init
			return if @initialized
			@initialized = true
			loader = Loader.new(self)
			loader.load("WebKit","1.0")
		end
	end

	class Loader < GObjectIntrospection::Loader
		def load(namespace,version=nil)
			repository = GObjectIntrospection::Repository.default
			repository.require(namespace,version)
			pre_load(repository,namespace)
			repository.each(namespace) do |info|
				load_info(info)
			end
			post_load(repository,namespace)
		end
	end
end
