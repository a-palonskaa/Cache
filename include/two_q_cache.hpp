#pragma once

#include <list>
#include <unordered_map>
#include <iterator>
#include <utility>
#include <functional>
#include <cstdlib>

template <typename KeyT, typename ElemT>
class two_q_cache_t {
public:
    using FuncT = std::function<ElemT(KeyT)>;

    two_q_cache_t(size_t size, FuncT slow_get_page): slow_get_page_(std::move(slow_get_page)) {
        if (size < minimal_cap_) {
            std::cerr << "size must be more or equal to minimal capacity "<< minimal_cap_ << '\n';
            std::abort();
        }

        a_in_size_  = size / 4;
        a_m_size_   = size - a_in_size_;
        a_out_size_ = size;
    }

    bool lookup_update(const KeyT& key) {
        auto page_pos_m = a_m_pos_.find(key);
        if (page_pos_m != a_m_pos_.end()) {
            a_m_storage_.splice(a_m_storage_.begin(), a_m_storage_, page_pos_m->second);
            return true;
        }

        auto page_pos_out = a_out_pos_.find(key);
        if (page_pos_out != a_out_pos_.end()) {
            a_out_storage_.erase(page_pos_out->second);
            a_out_pos_.erase(page_pos_out);

            insert_to_a_m(key, slow_get_page_(key));
            return false;
        }

        auto page_pos_in = a_in_pos_.find(key);
        if (page_pos_in != a_in_pos_.end()) {
            auto page = page_pos_in->second;
            a_in_storage_.erase(page);
            a_in_pos_.erase(page_pos_in);

            insert_to_a_m(key, page->second);
            return true;
        }

        insert_to_a_in(key, slow_get_page_(key));
        return false;
    }

private:
    void insert_to_a_in(const KeyT& key, const ElemT& page) {
        if (a_in_storage_.size() == a_in_size_) {
            auto elem_key = a_in_storage_.back().first;
            a_in_storage_.pop_back();
            a_in_pos_.erase(elem_key);

            a_out_storage_.push_front(elem_key);
            a_out_pos_.emplace(elem_key, a_out_storage_.begin());

            if (a_out_storage_.size() >= a_out_size_) {
                a_out_pos_.erase(a_out_storage_.back());
                a_out_storage_.pop_back();
            }
        }
        a_in_storage_.emplace_front(key, page);
        a_in_pos_.emplace(key, a_in_storage_.begin());
    }

    void insert_to_a_m(const KeyT& key, const ElemT& page) {
        if (a_m_storage_.size() == a_m_size_) {
            a_m_pos_.erase(a_m_storage_.back().first);
            a_m_storage_.pop_back();
        }
        a_m_storage_.emplace_front(key, page);
        a_m_pos_.emplace(key, a_m_storage_.begin());
    }

private:
    using PageListT = std::list<std::pair<KeyT, ElemT>>;
    using PageIterT = PageListT::iterator;

    using KeyListT = std::list<KeyT>;
    using KeyIterT = std::list<KeyT>::iterator;

    static constexpr size_t minimal_cap_ = 4;

    FuncT slow_get_page_;

    size_t a_in_size_;
    size_t a_out_size_;
    size_t a_m_size_;

    PageListT a_m_storage_;
    std::unordered_map<KeyT, PageIterT> a_m_pos_;

    PageListT a_in_storage_;
    std::unordered_map<KeyT, PageIterT> a_in_pos_;

    KeyListT a_out_storage_;
    std::unordered_map<KeyT, KeyIterT> a_out_pos_;
};


