first = true
while gets
  if first 
    first = false
    regexp = /^  (GDK_(\w+))\s*=\s*\d+/
  end

  if ~ regexp
      print %{rb_define_const(gdkCursor, "#$2", INT2FIX(#$1));\n}
  end
end
