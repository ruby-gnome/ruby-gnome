except_targets = ["Init_gtk2()", "Init_gtk_gdk()", "Init_gtk_gtk()"]
dependencies = {
  "Init_gtk_gdk_gc()" => ["Init_gtk_gdk_draw()"],
}

def print_data(array, type, defs, extern = false)
  if array[type]
    extern_def = "extern void" if extern
    print "##{defs} #{type}\n" if defs
    sorted_array = array[type].dup
    dependencies.each do |key, values|
      key_index = sorted_array.index(key)
      values.each do |value|
        value_index = sorted_array.index(value)
        next if value_index.nil?
        sorted_array.delete(value)
        sorted_array[key_index - 1, 1] = value
        key_index = sorted_array.index(key)
      end
    end
    sorted_array.each do |val|
	   print "#{extern_def}   #{val};\n"
    end
    print "#endif\n" if defs
  end
end

inits = Hash.new

while gets
  if ~ /^(Init_.*\(\))(.*)/
	init = $1
	unless except_targets.include?(init)
	  flag = $2
	  if flag.size > 0
		if flag =~ /\/\*\s*(\w*)\s*\*\//
		  inits[$1] = Array.new unless inits[$1]
		  inits[$1] << init
		end
	  else
		inits[""] = Array.new unless inits[""]
		inits[""] << init
	  end
	end
  end
end

print_data(inits, "", nil, true)
print_data(inits, "GTK_DISABLE_DEPRECATED", "ifndef", true)
print_data(inits, "GTK_ENABLE_BROKEN", "ifdef", true)

print "void Init_gtk_inits()\n"
print "{\n"
print_data(inits, "", nil)
print_data(inits, "GTK_DISABLE_DEPRECATED", "ifndef")
print_data(inits, "GTK_ENABLE_BROKEN", "ifdef")
print "}\n"
