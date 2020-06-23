module GLib
  module Interface
    def included(klass)
      $stderr.puts "#{klass.name} implements interface #{name}"
      repository = GObjectIntrospection::Repository.default
      interface_info = repository.find(gtype)
      GObjectIntrospection.add_interface(klass.gtype, gtype, interface_info)
    end
  end
end
