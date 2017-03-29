/*********************************************
 * Author : Zhongwen(Alan) Lan
 * Created: 2017/03/28
 ********************************************/
#ifndef _LRUCACHE_H_
#define _LRUCACHE_H_

#include <assert.h>
#include <iostream>
#include <list>
#include <unordered_map>

using namespace std;

class LRUCache {
public:
    struct LRU {
        // value of key
        int value;
        // position of key in time order list
        // for update or delete
        list<int>::iterator pos;
        LRU(int v) :
          value(v) {
        }
    };

    LRUCache(int capacity) {
        cap_ = capacity;
        size_ = 0;
    }

    int get(int key) {
        auto iter = key_value_map_.find(key);
        if (iter == key_value_map_.end()) {
            return -1;
        }
        updateOrder(iter->second, key);
        return iter->second.value;
    }

    void put(int key, int value) {
        cout << "-------put(" << key << ")----------\n";
        if (cap_ <= 0) {
            return;
        }
        auto iter = key_value_map_.find(key);
        if (iter == key_value_map_.end()) {
            // empty
            // 1. < cap insert directly
            // 2. == cap remove least recently used key and then insert
            // current size >= capacity: remove least recently used key first
            if (size_ >= cap_) {
                // remove oldest key from time order list
                int key_removed = time_order_list_.front();
                time_order_list_.pop_front();
                // remove oldest key from key value map
                key_value_map_.erase(key_removed);
                cout << "erase key = " << key_removed << endl;
                --size_;
            }
            LRU lru(value);
            time_order_list_.push_back(key);
            lru.pos = --time_order_list_.end();
            key_value_map_.insert({key, lru});
            ++size_;
        } else {
            // found
            // 1. update value
            // 2. update time order list
            updateOrder(iter->second, key);
            iter->second.value = value;
        }

    }

private:
    void updateOrder(struct LRU& lru, int key) {
        time_order_list_.erase(lru.pos);
        time_order_list_.push_back(key);
        lru.pos = --time_order_list_.end();
    }

private:
    // capacity
    int cap_;
    // current size
    int size_;
    unordered_map<int, struct LRU> key_value_map_;
    // oldest at first
    list<int> time_order_list_;
};
#endif
