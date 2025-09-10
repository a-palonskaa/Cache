#pragma once

#include <vector>
#include <unordered_map>

template <typename ElemT, typename KeyT>
struct belady_cache_t {
    size_t size_;
    std::unordered_map<KeyT, ElemT> storage_;

    std::vector<ElemT> elements_;

    belady_cache_t(size_t size, std::vector<ElemT> elements) {
        size_ = size;
        elements_ = std::move(elements);
    };

    template <typename FuncT>
    bool lookup_update(KeyT key, FuncT slow_get_page) {
        if (!elements_.empty()) {
            elements_.erase(elements_.begin());
        }

        if (auto page_pos = storage_.find(key); page_pos != storage_.end()) {
            return true;
        }

        if (storage_.size() < size_) {
            storage_[key] = slow_get_page(key);
            return false;
        }

        KeyT farthest_key = 0;
        size_t farthest_pos = 0;
        for (auto& elem : storage_) {
            size_t next_use = 0;
            if (auto elem_pos = std::find(elements_.begin(), elements_.end(), elem.first); elem_pos != elements_.end()) {
                next_use = std::distance(elements_.begin(), elem_pos);
                if (next_use > farthest_pos) {
                    farthest_pos = next_use;
                    farthest_key = elem.first;
                }
            } else {
                next_use = std::numeric_limits<size_t>::max();
                farthest_key = elem.first;
                break;
            }
        }

        storage_.erase(farthest_key);
        storage_[key] = slow_get_page(key);
        return false;
    }
};
