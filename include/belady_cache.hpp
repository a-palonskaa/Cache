#pragma once

#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <iterator>
#include <functional>

template <typename KeyT, typename ElemT>
class belady_cache_t {
public:
    using FuncT = std::function<ElemT(KeyT)>;

    belady_cache_t(size_t size, std::vector<ElemT> elements, FuncT slow_get_page) :
        size_(size), elements_(std::move(elements)), slow_get_page_(std::move(slow_get_page)) {}

    bool lookup_update(const KeyT& key) {
        if (!elements_.empty()) {
            elements_.erase(elements_.begin());
        }

        if (storage_.contains(key)) {
            return true;
        }

        if (storage_.size() < size_) {
            storage_.emplace(key, slow_get_page_(key));
            return false;
        }

        KeyT farthest_key{};
        for (size_t farthest_pos = 0; auto& elem : storage_) {
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
        storage_.emplace(key, slow_get_page_(key));
        return false;
    }
private:
    FuncT slow_get_page_;
    size_t size_;
    std::unordered_map<KeyT, ElemT> storage_;
    std::vector<ElemT> elements_;
};
