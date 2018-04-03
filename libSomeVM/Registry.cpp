#include "Registry.hpp"

namespace svm
{
    Registry::Registry(size_t size)
        : values{ size, Value{} }
    {}

    Registry::Registry(const Registry& other)
    {
        LockType otherLock{ other.mut };
        std::copy(other.values.begin(), other.values.end(), values.begin());
    }

    Registry& Registry::operator=(const Registry& other)
    {
        LockType lock{ mut };
        LockType otherLock{ other.mut };
        std::copy(other.values.begin(), other.values.end(), values.begin());

        return *this;
    }

    Registry::Registry(Registry&& other)
    {
        LockType otherLock{ other.mut };
        values = std::move(other.values);
    }

    Registry& Registry::operator=(Registry&& other)
    {
        LockType lock{ mut };
        LockType otherLock{ other.mut };
        values = std::move(other.values);

        return *this;
    }

    Registry::Iterator Registry::begin()
    {
        return values.begin();
    }

    Registry::Iterator Registry::end()
    {
        return values.end();
    }

    Registry::ConstIterator Registry::begin() const
    {
        return values.begin();
    }

    Registry::ConstIterator Registry::end() const
    {
        return values.end();
    }

    Value Registry::get(Register idx) const
    {
        LockType lock{ mut };
        return values[idx];
    }

    void Registry::set(Register idx, Value v)
    {
        LockType lock{ mut };
        values[idx] = v;
    }
}
