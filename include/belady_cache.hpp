#pragma once

#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>
#include <iterator>
#include <functional>
#include <iostream>

template <typename KeyT, typename ElemT>
class belady_cache_t {
public:
    using FuncT = std::function<ElemT(KeyT)>;

    belady_cache_t(size_t size, std::vector<KeyT> elements, FuncT slow_get_page) :
        size_(size), slow_get_page_(std::move(slow_get_page)) {
        for (size_t i = 0; i < elements.size(); i++) {
            occurance_table_[elements[i]].push(i);
        }
    }

    bool lookup_update(const KeyT& elem_key) {
        occurance_table_[elem_key].pop();

        if (storage_.contains(elem_key)) {
            return true;
        }

        if (occurance_table_[elem_key].empty()) {
            return false;
        }

        if (storage_.size() < size_) {
            storage_.emplace(elem_key, slow_get_page_(elem_key));
            return false;
        }

        KeyT farthest_key{};
        size_t farthest_pos = 0;
        bool never_used = false;
        for (auto& [key, _] : storage_) {
            if (occurance_table_[key].empty()) {
                farthest_key = key;
                never_used = true;
                break;
            }

            auto next_key_use = occurance_table_[key].front();
            if (next_key_use > farthest_pos) {
                farthest_pos = next_key_use;
                farthest_key = key;
            }
        }

        if (!never_used && occurance_table_[elem_key].front() > farthest_pos) {
            return false;
        }

        storage_.erase(farthest_key);
        storage_.emplace(elem_key, slow_get_page_(elem_key));

        return false;
    }
private:
    FuncT slow_get_page_;
    size_t size_;
    std::unordered_map<KeyT, ElemT> storage_;
    std::unordered_map<KeyT, std::queue<size_t>> occurance_table_;
};
