#include "util/testharness.h"
#include "intset.h"

class HerlihyLFTest {};

TEST(HerlihyLFTest, CreateEmpty) {
	sl_intset_t* set = nullptr;
	ssalloc_init();
	set = sl_set_new();
	ASSERT_TRUE(set != nullptr);
}

TEST(HerlihyLFTest, EmptyContains) {
	sl_intset_t* set = nullptr;
	ssalloc_init();
	seeds = seed_rand();
	set = sl_set_new();
	ASSERT_TRUE(set != nullptr);


	printf("Set head address %p key %ld value %ld\n", set->head, set->head->key, set->head->val);
	printf("Set tail address %p key %ld value %ld\n", set->head->next[0], set->head->next[0]->key, set->head->next[0]->val);
	
	
	// sval_t res_val =  sl_contains(set, 2);
	// ASSERT_TRUE(res_val == 0);

	// int res = sl_add(set, "lol", "abc");
	// ASSERT_TRUE(res != 0);

	// const char* res_val =  sl_contains(set, "lol");
	// ASSERT_TRUE(strcmp(res_val,"abc")==0);

}


int main(int argc, char** argv) {
  return rocksdb::test::RunAllTests();
}