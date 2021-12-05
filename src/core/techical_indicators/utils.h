
template <typename T, size_t size = 10>
class CircularBuffer
{
    T buffer[size] = {T(0)};
    size_t iterator = 0;

public:
    void update(const T value)
    {
        buffer[iterator % size] = value;
        iterator++;
    }
    T get_oldest_value() { return buffer[iterator % size]; }
};
