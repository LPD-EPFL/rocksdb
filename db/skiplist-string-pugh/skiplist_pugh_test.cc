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

typedef volatile TestNode TestNode_t;

__thread unsigned long* seeds;


strval_t f() {
	strval_t v("lol");
	TestNode_t* node = (TestNode_t*) malloc(sizeof(TestNode_t));
	
	strncpy((char*) node->val.val, "troll", STRING_LENGTH);	

	strncpy((char *) v.val, (char*) node->val.val, STRING_LENGTH);

	return v;
}

class PughTest {

public:
	PughTest() {
		// levelmax = floor_log_2((unsigned int) initial);
		levelmax = 3;
  		size_pad_32 = sizeof(sl_node_t) + (levelmax * sizeof(sl_node_t *));
  		while (size_pad_32 & 31)
    	{
     		size_pad_32++;
    	}

    	ssalloc_init();

	}

};

// TEST(PughTest, EmptyTest) {
// 	ASSERT_TRUE(true);
// 	strkey_t k1, k2;
// 	strncpy(k1.key, "lol", 4);
// 	strncpy(k2.key, "troll", 6);

// 	TestNode_t* node = (TestNode_t*) malloc(sizeof(TestNode_t));
	
// 	node->key = k1;
// 	printf("%d\n", strkey_compare(node->key, k2));
// 	strval_t* res = optimistic_find(nullptr, k1);
// 	res = res + 1 -1;
// }

TEST(PughTest, CreateEmpty) {
	sl_intset_t* set = nullptr;
	set = sl_set_new();
	ASSERT_TRUE(set != nullptr);
}

TEST(PughTest, EmptyContains) {
	sl_intset_t* set = nullptr;
	seeds = seed_rand();
	set = sl_set_new();
	ASSERT_TRUE(set != nullptr);


	// printf("Set head address %p key %ld value %ld\n", set->head, set->head->key, set->head->val);
	// printf("Set tail address %p key %ld value %ld\n", set->head->next[0], set->head->next[0]->key, set->head->next[0]->val);
	strkey_t k("oana");
	strval_t v("igor");
	
	ASSERT_TRUE(sl_contains(set, k) == nullptr);
	ASSERT_NE(sl_add(set, k, v),0);
	ASSERT_TRUE(sl_contains(set, k) != nullptr);
	printf("%s\n", sl_contains(set, k)->val);

	// const char* res_val =  sl_contains(set, "lol");
	// ASSERT_TRUE(strcmp(res_val,"abc")==0);

}


int main(int argc, char** argv) {
  return rocksdb::test::RunAllTests();
}