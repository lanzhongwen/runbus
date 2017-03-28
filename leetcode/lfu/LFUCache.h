/*********************************************
 * Author : Zhongwen(Alan) Lan
 * Created: 2017/03/28
 ********************************************/
#ifndef _LFUCACHE_H_
#define _LFUCACHE_H_

#include <assert.h>
#include <list>
#include <unordered_map>

using namespace std;

class LFUCache {
public:
    struct LFU {
        // frequency
        int freq;
        // value of key
        int value;
        // position of key in frequency hash value list
        list<int>::iterator pos;
        LFU(int f, int v) :
          freq(f),value(v) {
        }
    };

    LFUCache(int capacity) {
        cap_ = capacity;
        size_ = 0;
        min_freq_ = 0;
    }

    int get(int key) {
        auto iter = key_value_map_.find(key);
        if (iter == key_value_map_.end()) {
            return -1;
        }
        updateFreq(iter->second, key);
        return iter->second.value;
    }

    void put(int key, int value) {
        if (cap_ <= 0) {
            return;
        }
        auto iter = key_value_map_.find(key);
        if (iter == key_value_map_.end()) {
            // empty
            // 1. < cap insert directly
            // 2. == cap remove least recently used key and then insert
            // 3. update frequency map as min_freq_ to be set to 1

            // current size >= capacity: remove least recently used key first
            if (size_ >= cap_) {
                auto iter = freq_keylist_map_.find(min_freq_);
                assert(iter != freq_keylist_map_.end());
                auto& key_list = iter->second;
                int min_key = key_list.front();
                 key_list.pop_front();
                // check if key_list is empty then remove the entry from freq_keylist_map_
                if (key_list.empty()) {
                    freq_keylist_map_.erase(min_freq_);
                }

                /*
                int min_key = iter->second.front();
                iter->second.pop_front();
                // check if key list is empty then remove the entry from freq_keylist_map_
                if (iter->second.empty()) {
                    freq_keylist_map_.erase(min_freq_);
                }
                */
                key_value_map_.erase(min_key);
                --size_;
            }
            min_freq_ = 1;
            LFU lfu(min_freq_, value);
            freq_keylist_map_[min_freq_].push_back(key);
            lfu.pos = --freq_keylist_map_[min_freq_].end();
            key_value_map_.insert({key, lfu});
            ++size_;
        } else {
            // found
            // 1. update value
            // 2. update frequency
            updateFreq(iter->second, key);
            iter->second.value = value;
        }

    }

private:
    void updateFreq(struct LFU& lfu, int key) {
        int old_freq = lfu.freq;
        freq_keylist_map_[lfu.freq].erase(lfu.pos);
        ++lfu.freq;
        freq_keylist_map_[lfu.freq].push_back(key);
        lfu.pos = --freq_keylist_map_[lfu.freq].end();
        if (freq_keylist_map_[old_freq].empty()) {
            if (old_freq == min_freq_) {
                ++min_freq_;
            }
            freq_keylist_map_.erase(old_freq);
        }
    }

private:
    // capacity
    int cap_;
    // current size
    int size_;
    // current minimal frequency
    int min_freq_;
    unordered_map<int, struct LFU> key_value_map_;
    unordered_map<int, list<int> > freq_keylist_map_;
};
#endif
