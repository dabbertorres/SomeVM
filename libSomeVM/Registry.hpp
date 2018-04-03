#pragma once

#include <iterator>
#include <mutex>
#include <type_traits>
#include <vector>

#include "Register.hpp"
#include "Value.hpp"

namespace svm
{
    class Registry
    {
    public:
        using Iterator = std::vector<Value>::iterator;
        using ConstIterator = std::vector<Value>::const_iterator;

        Registry() = default;
        Registry(size_t size);

        Registry(const Registry& other);
        Registry& operator=(const Registry& other);

        Registry(Registry&& other);
        Registry& operator=(Registry&& other);

        ~Registry() = default;

        Iterator begin();
        Iterator end();

        ConstIterator begin() const;
        ConstIterator end() const;

        template<typename It, typename = std::enable_if_t<std::is_same_v<std::iterator_traits<It>::value_type, Value>>>
        void load(It begin, It end);

        Value get(Register idx) const;
        void set(Register idx, Value v);

    private:
        std::vector<Value> values;
        mutable std::recursive_mutex mut;

        using LockType = std::lock_guard<decltype(mut)>;
    };

    template<typename It, typename>
    inline void Registry::load(It begin, It end)
    {
        LockType lock{ mut };
        values.resize(std::distance(begin, end));
        std::copy(begin, end, values.begin());
    }
}
