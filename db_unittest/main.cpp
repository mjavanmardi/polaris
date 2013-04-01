#include <iostream>
#include <gtest/gtest.h>

extern char* db_path; 

GTEST_API_ int main(int argc, char **argv) {
  std::cout << "Running main() from main.cpp\n";
  testing::InitGoogleTest(&argc, argv);
  if (argc != 2) 
  {
	  std::cout << "Usage: " << argv[0] << "[gtest arguments] partial_path_to_database\n";
	  return 1;
  }
  ::db_path = argv[1];
  return RUN_ALL_TESTS();
}
