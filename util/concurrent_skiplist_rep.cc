//  Copyright (c) 2013, Facebook, Inc.  All rights reserved.
//  This source code is licensed under the BSD-style license found in the
//  LICENSE file in the root directory of this source tree. An additional grant
//  of patent rights can be found in the PATENTS file in the same directory.
//
#include "rocksdb/memtablerep.h"
#include "db/memtable.h"
#include "db/skiplist.h"
#include "db/skiplist-string-pugh/intset.h"
#include <iostream>
#include <fstream>
#include <string.h>

// ADD HERE -- IMITATE THIS

// extern __thread unsigned long* seeds;


namespace rocksdb {
namespace {
class ConcurrentSkipListRep : public MemTableRep {
  SkipList<const char*, const MemTableRep::KeyComparator&> skip_list_;
  sl_intset_t* intset;

public:
  explicit ConcurrentSkipListRep(const MemTableRep::KeyComparator& compare, Arena* arena)
    : skip_list_(compare, arena) {
      // Allocate and instantiate intset
      levelmax = 3;
      size_pad_32 = sizeof(sl_node_t) + (levelmax * sizeof(sl_node_t *));
      while (size_pad_32 & 31)
      {
        size_pad_32++;
      }

      ssalloc_init();
      intset = sl_set_new();
  }

  // Insert key into the list.
  // REQUIRES: nothing that compares equal to key is currently in the list.
  virtual void Insert(const char* key) override {
  }

  virtual void Insert(const char* key, const char* val) override {

    strkey_t k(key);
    // std::cout << "key being inserted " << key << " " << strcmp("Igor", key) << std::endl;
    // std::cout << "value being inserted " << val << " " << std::endl;


    // if the value to be inserted is a null pointer , we interpret it as 
    // a deletion
    if (val == nullptr) {
      sl_remove(intset, k);
      return;
    }
    strval_t v(val);
    sl_add(intset, k, v);
  }


  // Returns true iff an entry that compares equal to key is in the list.
  virtual bool Contains(const char* key) const override {
    // return skip_list_.Contains(key);
    strkey_t k(key);

    return (sl_contains(intset, k) != nullptr);
  }

  virtual void FlushToDisk(const char* filename) override {
    // TODO
    using namespace std;
    ofstream output_file(filename, ios::out | ios::trunc);

    if (!output_file.is_open()) {
      printf("Error opening file\n");
      return;
    }

    sl_node_t* n = intset->head->next[0];
    while(n->next[0] != NULL) {

      output_file << (char *) n->key.key << endl << (char *) n->val.val << endl;
      n = n->next[0];
    }
    output_file.close();
  }

  virtual size_t ApproximateMemoryUsage() override {
    // All memory is allocated through arena; nothing to report here
    return 0;
  }

  virtual void Get(const LookupKey& k, void* callback_args,
                   bool (*callback_func)(void* arg,
                                         const char* entry)) override {
    // ConcurrentSkipListRep::Iterator iter(&skip_list_);

    // TODO optimize here to avoid such a long function chain
    strkey_t key(k.user_key().ToString().c_str());
    strval_t* res = sl_contains(intset, key);
    // if (res != nullptr) {
    //   std::cout << "FOUND: " << key.key << " - " << res->val << std::endl;
    // }

    if (res == nullptr) {
      callback_func(callback_args, nullptr);
    } else {
      callback_func(callback_args, res->val);
    }

    // printf("internal key %s\n", k.internal_key().ToString());
    // printf("user key %s\n", k.user_key().data());
    // Slice dummy_slice;
    // for (iter.Seek(dummy_slice, k.memtable_key().data());
    //      iter.Valid() && callback_func(callback_args, iter.key());
    //      iter.Next()) {
    // }

  }

  virtual ~ConcurrentSkipListRep() override { }

  // Iteration over the contents of a skip list

  // ADD HERE - STUPID ITERATOR
  class Iterator : public MemTableRep::Iterator {
    //SkipList<const char*, const MemTableRep::KeyComparator&>::Iterator iter_;
   public:
    // Initialize an iterator over the specified list.
    // The returned iterator is not valid.
    explicit Iterator() {}

    virtual ~Iterator() override { }

    // Returns true iff the iterator is positioned at a valid node.
    virtual bool Valid() const override {
      //return iter_.Valid();
      return false;
    }

    // Returns the key at the current position.
    // REQUIRES: Valid()
    virtual const char* key() const override {
      //return iter_.key();
      return nullptr;
    }

    // Advances to the next position.
    // REQUIRES: Valid()
    virtual void Next() override {
      //iter_.Next();
    }

    // Advances to the previous position.
    // REQUIRES: Valid()
    virtual void Prev() override {
      //iter_.Prev();
    }

    // Advance to the first entry with a key >= target
    virtual void Seek(const Slice& user_key, const char* memtable_key)
        override {
      // if (memtable_key != nullptr) {
      //   iter_.Seek(memtable_key);
      // } else {
      //   iter_.Seek(EncodeKey(&tmp_, user_key));
      // }
    }

    // Position at the first entry in list.
    // Final state of iterator is Valid() iff list is not empty.
    virtual void SeekToFirst() override {
      // iter_.SeekToFirst();
    }

    // Position at the last entry in list.
    // Final state of iterator is Valid() iff list is not empty.
    virtual void SeekToLast() override {
      // iter_.SeekToLast();
    }
   //protected:
    //std::string tmp_;       // For passing to EncodeKey
  };

  // Unhide default implementations of GetIterator
  using MemTableRep::GetIterator;

  virtual MemTableRep::Iterator* GetIterator() override {
    // return new ConcurrentSkipListRep::Iterator(&skip_list_);
    return nullptr;

  }
};
}

MemTableRep* ConcurrentSkipListFactory::CreateMemTableRep(
    const MemTableRep::KeyComparator& compare, Arena* arena,
    const SliceTransform*) {
  return new ConcurrentSkipListRep(compare, arena);
}

} // namespace rocksdb
