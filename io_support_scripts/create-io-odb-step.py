def add_custom_step(name):
    s = """
    <CustomBuild Include="$name$.h">
      <Message>Running odb compiler on $name$.h</Message>
      <Command>odb.exe --std c++11 --database sqlite --generate-query --generate-schema --generate-session --default-pointer std::tr1::shared_ptr --schema-name $name$ $name$.h</Command>
      <Outputs>$name$-odb.hxx;$name$-odb.ixx;$name$-odb.cxx;%(Outputs)</Outputs>
    </CustomBuild>"""
    return s.replace("$name$", name)

 
custom_steps = """<?xml version="1.0" encoding=\"utf-8\"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup>"""
include_group = "\n  <ItemGroup>\n"
compile_group = "  <ItemGroup>\n"
none_group = "  <ItemGroup>\n"
with open("polaris_io\db-inventory.txt") as fh:
    for line in fh:
        item = line.strip("\n")
        custom_steps += add_custom_step(item)
        include_group += "    <ClInclude Include=\"%s-odb.hxx\" />\n"%item
        compile_group += "    <ClCompile Include=\"%s-odb.cxx\" />\n"%item
        none_group += "    <None Include=\"%s-odb.ixx\" />\n"%item
custom_steps += "\n  </ItemGroup>"
include_group += "  </ItemGroup>\n"
compile_group += "  </ItemGroup>\n"
none_group += "  </ItemGroup>\n"

with open("io-odb-step.props",'w') as fh:
    fh.write(custom_steps)
    fh.write(include_group)
    fh.write(compile_group)
    fh.write(none_group)
    fh.write("""</Project>""")