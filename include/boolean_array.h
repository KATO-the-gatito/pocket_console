#pragma once

template <typename T>
class boolean_array
{
public:
    boolean_array() : buffer(0)
    {}

    boolean_array set(T index, bool sign){
        buffer &= ~(1 << index);
        buffer |= sign << index;
        return *this;
    }
    bool get(T index){
        byte val = 1 << index;
        return (buffer & val) == val;
    }

    T buffer;
};
