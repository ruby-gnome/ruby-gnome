except_targets = ["Init_gtk2()", "Init_gtk_gdk()", "Init_gtk_gtk()"]

def print_data(array, type, defs, extern = false)
  extern_def = "extern void" if extern
  print "##{defs} #{type}\n" if defs
  array[type].each do |val|
	print "#{extern_def}   #{val};\n"
  end
  print "#endif\n" if defs
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
