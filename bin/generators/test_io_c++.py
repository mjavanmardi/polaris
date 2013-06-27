from generate_io_code import *

def re_patterns():
    assert p_ptr.search("std::vector<weak_ptr<Instance> >") is None
    assert p_ptr1.search("std::vector<weak_ptr<Instance> >") is not None
    assert p_ptr.search("weak_ptr<Instance>") is not None
    assert p_ptr.search("shared_ptr< Instance >") is not None
    assert p_ptr.search(" int *") is not None
    assert p_ptr.search(" float*") is not None
def re_container_patterns():
    assert p_container.search("std::vector<std::string>") is not None
    assert p_container.search("vector<name>") is not None
    assert p_container.search("std::set<std::string> ") is not None
    m =  p_container.search("std::vector<shared_ptr<int> >") 
    assert m.group(3) == "shared_ptr<int> "
    assert p_container.search("float") is None
    assert p_container.search("sahred_ptr<float>") is None
  
def singularize():
    assert my_singularize("instances") == "instanc"
    assert my_singularize("actions") == "action"
    assert my_singularize("setKeys") == "setKey"
re_patterns()
re_container_patterns()
singularize()
  
    
    