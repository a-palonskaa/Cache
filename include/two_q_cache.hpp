#pragma once

#include <list>
#include <unordered_map>

template <typename ElemT, typename KeyT>
struct two_q_cache_t {
    size_t a_in_size_;
    size_t a_out_size_;
    size_t a_m_size_;

    std::list<std::pair<KeyT, ElemT>> a_m_storage_;
    std::unordered_map<KeyT, typename std::list<std::pair<KeyT, ElemT>>::iterator> a_m_pos_;

    std::list<std::pair<KeyT, ElemT>> a_in_storage_;
    std::unordered_map<KeyT, typename std::list<std::pair<KeyT, ElemT>>::iterator> a_in_pos_;

    std::list<KeyT> a_out_storage_;
    std::unordered_map<KeyT, typename std::list<KeyT>::iterator> a_out_pos_;

    two_q_cache_t(size_t size) {
        if (size < 4) {
            size = 4;
        }

        a_in_size_  = size / 4;
        a_m_size_   = size - a_in_size_;
        a_out_size_ = size;
    };

    template <typename FuncT>
    bool lookup_update(KeyT key, FuncT slow_get_page) {
        if (auto page_pos_m = a_m_pos_.find(key); page_pos_m != a_m_pos_.end()) {
            a_m_storage_.splice(a_m_storage_.begin(), a_m_storage_, page_pos_m->second);
            return true;
        }

        if (auto page_pos_out = a_out_pos_.find(key); page_pos_out != a_out_pos_.end()) {
            a_out_storage_.erase(page_pos_out->second);
            a_out_pos_.erase(page_pos_out);

            insert_to_a_m(key, slow_get_page(key));
            return false;
        }

        if (auto page_pos_in = a_in_pos_.find(key); page_pos_in != a_in_pos_.end()) {
            ElemT page = page_pos_in->second->second;
            a_in_storage_.erase(page_pos_in->second);
            a_in_pos_.erase(page_pos_in);

            insert_to_a_m(key, page);
            return true;
        }

        insert_to_a_in(key, slow_get_page(key));
        return false;
    }

    void insert_to_a_in(KeyT key, ElemT page) {
        if (a_in_storage_.size() >= a_in_size_) {
            auto elem = a_in_storage_.back();
            a_in_storage_.pop_back();
            a_in_pos_.erase(elem.first);

            a_out_storage_.push_front(elem.first);
            a_out_pos_[elem.first] = a_out_storage_.begin();

            if (a_out_storage_.size() >= a_out_size_) {
                auto last_key = a_out_storage_.back();
                a_out_storage_.pop_back();
                a_out_pos_.erase(last_key);
            }
        }
        a_in_storage_.emplace_front(key, page);
        a_in_pos_[key] = a_in_storage_.begin();
    }

    void insert_to_a_m(KeyT key, ElemT page) {
        if (a_m_storage_.size() >= a_m_size_) {
            auto elem = a_m_storage_.back();
            a_m_storage_.pop_back();
            a_m_pos_.erase(elem.first);
        }
        a_m_storage_.emplace_front(key, page);
        a_m_pos_[key] = a_m_storage_.begin();
    }
};
