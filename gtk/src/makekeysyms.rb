while gets
  if ~ /^#define\s+(GDK_\w+)\s+\d+/
    print %{rb_define_const(mGdkKeyval, "#$1", INT2FIX(#$1));\n}
  end
end
