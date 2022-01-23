#pragma once

#include "pactree.h"
#include "tree_api.h"
#include <iostream>
#include <map>
#include <thread>

// used to define the interface of all benchmarking trees
class pac_wrapper : public tree_api {
public:
  pac_wrapper() { my_tree = new pactree(1); }

  void bulk_load(char *key[], size_t k_sz[], char *value[], size_t v_sz[],
                 size_t num_kv) {
    my_tree->registerThread();
    for (size_t i = 0; i < num_kv; i++) {
      if (k_sz[i] > 256) {
        printf("Key length is too long %lld\n", k_sz[i]);
      }
      Key_t key;
      key.set(key[i], k_sz[i]);
      my_tree->insert(key, reinterpret_cast<uint64_t>(value[i]));
    }
    my_tree->unregisterThread();
  }

  bool insert(const char *key, size_t key_sz, const char *value,
              size_t value_sz, size_t thread_id) override {
    if (!is_registered) {
      my_tree->registerThread();
      is_registered = true;
    }

    Key_t str_key;
    str_key.set(key, key_sz);
    auto ret = my_tree->insert(str_key, reinterpret_cast<uint64_t>(value));
    return ret;
  }

  bool find(const char *key, size_t sz, char *value_out,
            size_t thread_id) override {
    if (!is_registered) {
      my_tree->registerThread();
      is_registered = true;
    }

    Key_t str_key;
    str_key.set(key, sz);
    auto ret = my_tree->lookup(str_key);
    memcpy(value_out, &ret, sizeof(ret));
    return true;
  }

  bool update(const char *key, size_t key_sz, const char *value,
              size_t value_sz, size_t thread_id) override {
    return true;
  }

  bool remove(const char *key, size_t key_sz, size_t thread_id) override {
    return true;
  }

  int scan(const char *key, size_t key_sz, int scan_sz, char *&values_out,
           size_t thread_id) override {

    return 0;
  }

private:
  thread_local inline static bool is_registered = false;
  pactree *my_tree; // Store the pointer to the tree instance
};