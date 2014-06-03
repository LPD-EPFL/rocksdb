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
		// levelmax = 3;
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

TEST(PughTest, Casting) {
	int64_t v;
	char c[STRING_LENGTH] = "lol";
	char c2[STRING_LENGTH];

	v = *(int64_t *) c;
	printf("Int value of %s : " "%" PRId64 "\n", c, v);

	memcpy(c2, &v, STRING_LENGTH);
	printf("String value of " "%" PRId64 " : %s\n", v, c2);
}

TEST(PughTest, Limits) {

	int64_t m = ALT_KEY_MIN;
	int64_t M = ALT_KEY_MAX;
	strkey_t min, max, k;
	int64_to_char8(min.key, m);
	int64_to_char8(max.key, M);
	// strncpy(min.key, (char *)&m, STRING_LENGTH);
	// strncpy(max.key, (char *)&M, STRING_LENGTH);
	
	strncpy(k.key, "lol", STRING_LENGTH);
	printf("Comparing %s with min: " "%" PRId64 "\n", k.key, strkey_compare_haxxorz(k, min));
	printf("Comparing %s with max: " "%" PRId64 "\n", k.key, strkey_compare_haxxorz(k, max));

	strncpy(k.key, "", STRING_LENGTH);
	printf("Comparing %s with min: " "%" PRId64 "\n", k.key, strkey_compare_haxxorz(k, min));
	printf("Comparing %s with max: " "%" PRId64 "\n", k.key, strkey_compare_haxxorz(k, max));

	strncpy(k.key, "0", STRING_LENGTH);
	printf("Comparing %s with min: " "%" PRId64 "\n", k.key, strkey_compare_haxxorz(k, min));
	printf("Comparing %s with max: " "%" PRId64 "\n", k.key, strkey_compare_haxxorz(k, max));


	strncpy(k.key, "zzz", STRING_LENGTH);
	printf("Comparing %s with min: " "%" PRId64 "\n", k.key, strkey_compare_haxxorz(k, min));
	printf("Comparing %s with max: " "%" PRId64 "\n", k.key, strkey_compare_haxxorz(k, max));


}

TEST(PughTest, NewComparator) {
	strkey_t k1, k2;
	strncpy(k1.key, "lol", STRING_LENGTH);
	strncpy(k2.key, "troll", STRING_LENGTH);
	printf("k1 : %s " "%" PRId64 "\n", k1.key, *(int64_t *) k1.key );
	printf("k2 : %s " "%" PRId64 "\n", k2.key, *(int64_t *) k2.key );

	printf("Comparing %s and %s : " "%" PRId64 "\n", k1.key, k2.key, strkey_compare_haxxorz(k1, k2));
}

int main(int argc, char** argv) {
  return rocksdb::test::RunAllTests();
}