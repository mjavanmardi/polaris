from generate_io_code import *

def re_patterns():
    assert p_ptr.search("std::vector<weak_ptr<Instance> >") is None
    assert p_ptr.search("weak_ptr<Instance>") is not None
    assert p_ptr.search("shared_ptr< Instance >") is not None
    assert p_ptr.search(" int *") is not None
    assert p_ptr.search(" float*") is not None
    
re_patterns()
  
    
    