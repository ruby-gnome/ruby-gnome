while gets
  if ~ /^(GDK_(\w+))\s*=\s*\d+/
    print %{rb_define_const(gdkCursorConst, "#$2", INT2FIX(#$1));\n}
  end
end
