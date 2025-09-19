#pragma once

#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>
#include <algorithm>
#include <iterator>
#include <functional>

inline constexpr size_t inf = std::numeric_limits<size_t>::max();

template <typename KeyT, typename ElemT>
class belady_cache_t {
public:
    using FuncT = std::function<ElemT(KeyT)>;

    belady_cache_t(size_t size, std::vector<ElemT> elements, FuncT slow_get_page) :
        size_(size), elements_(std::move(elements)), slow_get_page_(std::move(slow_get_page)) {
        for (size_t i = 0; i < elements_.size(); ++i) {
            occurance_table_[elements_[i]].push(i);
        }

        for (auto& [key, queue] : occurance_table_) {
            queue.push(inf);
        }
    }

    bool lookup_update(const KeyT& elem_key) {
        occurance_table_[elem_key].pop();
        size_t next_key_use = occurance_table_[elem_key].front();

        if (storage_.contains(elem_key)) {
            return true;
        }

        if (storage_.size() < size_) {
            storage_.emplace(elem_key, slow_get_page_(elem_key));
            return false;
        }

        if (next_key_use == inf) {
            return false;
        }

        KeyT farthest_key{};
        size_t farthest_pos = 0;
        for (auto& [key, elem] : storage_) {
            auto next_use = occurance_table_[key].front();
            if (next_use == inf) {
                farthest_key =  key;
                farthest_pos = inf;
                break;
            } else if (next_use > farthest_pos) {
                farthest_pos = next_use;
                farthest_key = key;
            }
        }

        if (next_key_use > farthest_pos) {
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
    std::vector<ElemT> elements_;
};
