#ifndef LEVELS_H
#define LEVELS_H

template <unsigned int ... VALUES>
struct VALUE_LIST;

template <typename T>
struct sizeoflist;
template <unsigned int ... VS>
struct sizeoflist<VALUE_LIST<VS...>>
{
    enum { value = sizeof...(VS) };
};


template <unsigned int ... SEQ>
struct sequence_impl;

template <unsigned int N, unsigned int ... NS>
struct sequence_impl<N, NS...> : public sequence_impl<N-1,N-1,NS...>
{
};

template <>
struct sequence_impl<0>
{
    typedef VALUE_LIST<0> type;
};

template <unsigned int ... SEQ>
struct sequence_impl<1,SEQ...>
{
    typedef VALUE_LIST<0,SEQ...> type;
};

template <unsigned int ... SEQ>
using sequence = typename sequence_impl<SEQ...>::type;


template <unsigned int N, typename T>
struct sum_first_impl;
template <unsigned int N, template <unsigned int...a> class T>
struct sum_first_impl<N, T<>>
{
    enum { value = 0 };
};
template <template <unsigned int...a> class T, unsigned int V, size_t ... VS>
struct sum_first_impl<0,T<V,VS...>>
{
    enum { value = 0 };
};
template <unsigned int N, template <unsigned int...a> class T, unsigned int V, unsigned int ... VS>
struct sum_first_impl<N, T<V,VS...>>
{
    enum { value = V + sum_first_impl<N-1,VALUE_LIST<VS...>>::value };
};

template <size_t N, typename T>
constexpr auto sum_first = sum_first_impl<N,T>::value;

template <typename T, typename SEQ>
struct accumulate_impl;

template <typename T, unsigned int ... SEQ>
struct accumulate_impl<T,VALUE_LIST<SEQ...>>
{
    typedef VALUE_LIST<sum_first<SEQ,T>...> type;
};

template <typename T>
using accumulate = typename accumulate_impl<T,sequence<sizeoflist<T>::value>>::type;

template <unsigned int V>
struct int_type;

template <size_t N, typename ... TS>
struct get_element_at_impl
{
    static_assert(sizeof...(TS) >= N, "Element at index does not exist. Index too large.\n");
};

template <typename T, typename ... TS>
struct get_element_at_impl<0, T, TS...>
{
    typedef T type;
};

template <unsigned int N, unsigned int ... VS>
using get_element_at_i = typename detail::get_element_at_impl<N,int_type<VS>...>::type;

typedef VALUE_LIST<
    400,    // 1
    900,    // 2
    1400,   // 3
    2100,   // 4
    2800,   // 5
    3600,   // 6
    4500,   // 7
    5400,   // 8
    6500,   // 9
    7600,   // 10
    8800,   // 11
    10100,  // 12
    11400,  // 13
    12900,  // 14
    14400,  // 15
    16000,  // 16
    17700,  // 17
    19400,  // 18
    21300,  // 19
    23200,  // 20
    25200,  // 21
    27300,  // 22
    29400,  // 23
    31700,  // 24
    34000,  // 25
    36400,  // 26
    38900,  // 27
    41400,  // 28
    44300,  // 29
    47400,  // 30
    50800,  // 31
    54500,  // 32
    58600,  // 33
    62800,  // 34
    67100,  // 35
    71600,  // 36
    76100,  // 37
    80800,  // 38
    85800,  // 39
    90700,  // 40
    95800,  // 41
    101000, // 42
    106300, // 43
    111800, // 44
    117500, // 45
    123200, // 46
    129100, // 47
    135100, // 48
    141200, // 49
    147500, // 50
    153900, // 51
    160400, // 52
    167100, // 53
    173900, // 54
    180800, // 55
    187900, // 56
    195000, // 57
    202300, // 58
    209800  // 59
    > XP_PER_LEVEL;

typedef accumulate<XP_PER_LEVEL> TOTAL_XP_TO_LEVEL;

template <typename XP_PER_LEVEL, typename TOTAL_XP_TO_LEVEL>
struct values_data;

template <unsigned int ... XP_PER_LEVEL, unsigned int ... TOTAL_XP_TO_LEVEL>
struct values_data<VALUE_LIST<XP_PER_LEVEL...>, VALUE_LIST<TOTAL_XP_TO_LEVEL...>>
{
    static constexpr unsigned int xp_per_level[] {XP_PER_LEVEL...};
private:
    static constexpr unsigned int total_xp_to_level_d[] {TOTAL_XP_TO_LEVEL...};
public:
    static constexpr unsigned int total_xp_to_level(unsigned int i)
    {
        return (i <= 60 && i >= 2) ? total_xp_to_level_d[i-1] : 0;
    }
    static constexpr bool is_correct() { return total_xp_to_level(59) == 4084700; }
};

typedef values_data<XP_PER_LEVEL, TOTAL_XP_TO_LEVEL> levels_data;

#endif // LEVELS_H
