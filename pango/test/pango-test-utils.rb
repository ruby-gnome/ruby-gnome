module PangoTestUtils
 private
 def only_pango_version(major, minor, micro=nil)
   micro ||= 0
   unless Pango::Version.or_later?(major, minor, micro)
     omit("Require Pango >= #{major}.#{minor}.#{micro}")
   end
 end

 def only_gi_version(major, minor, micro=nil)
   micro ||= 0
   if (GObjectIntrospection::BUILD_VERSION <=> [major, minor, micro]) < 0
     omit("Require GObject Introspection >= #{major}.#{minor}.#{micro}")
   end
 end
end
