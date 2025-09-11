#pragma once

#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <iterator>

template <typename KeyT, typename ElemT>
class belady_cache_t {
public:
    belady_cache_t(size_t size, std::vector<ElemT> elements) :
        size_(size), elements_(elements) {}

    template <typename FuncT>
    bool lookup_update(const KeyT& key, FuncT slow_get_page) {
        if (!elements_.empty()) {
            elements_.erase(elements_.begin());
        }

        auto page_pos = storage_.find(key);
        if (page_pos != storage_.end()) {
            return true;
        }

        if (storage_.size() < size_) {
            storage_.emplace(key, slow_get_page(key));
            return false;
        }

        KeyT farthest_key{};
        size_t farthest_pos = 0;
        for (auto& elem : storage_) {
            auto elem_pos = std::find(elements_.begin(), elements_.end(), elem.first);
            if (elem_pos != elements_.end()) {
                auto next_use = std::distance(elements_.begin(), elem_pos);
                if (next_use > farthest_pos) {
                    farthest_pos = next_use;
                    farthest_key = elem.first;
                }
            } else {
                farthest_key = elem.first;
                break;
            }
        }

        storage_.erase(farthest_key);
        storage_.emplace(key, slow_get_page(key));
        return false;
    }
private:
    size_t size_;
    std::unordered_map<KeyT, ElemT> storage_;
    std::vector<ElemT> elements_;
};
