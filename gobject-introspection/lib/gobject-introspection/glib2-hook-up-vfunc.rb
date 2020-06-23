module GLib
  class Instantiatable
    class << self
      alias _method_added method_added
      def method_added(name)
        _method_added(name)

        name.to_s =~ /^do_/ or return
        vfunc_name = name.to_s.sub /^do_/, ''
        repository = GObjectIntrospection::Repository.default
        klass = ancestors.find do |klass|
          klass.respond_to? :gtype or next
          info = repository.find(klass.gtype) or next
          info.respond_to? :get_vfunc or next
          info.get_vfunc(vfunc_name)
        end or return
        vfunc_info = repository.find(klass.gtype).get_vfunc(vfunc_name)

        puts "hook up virtual method #{klass.name}##{vfunc_name}" \
          " to #{self.name}##{name}"
        GObjectIntrospection.hook_up_vfunc(name, vfunc_info, gtype)
      end
    end
  end
end
