#include "util/testharness.h"
#include "intset.h"
// #include "optimistic.h"
#include "db/synchrobench-utils/common.h"
#include <iostream>

struct TestNode
{
  strkey_t key;
  strval_t val; 
};

__thread unsigned long* seeds;

class HerlihyTest {};

TEST(HerlihyTest, VoidTest) {
	printf("OK \n");
}

int main(int argc, char** argv) {
  return rocksdb::test::RunAllTests();
}