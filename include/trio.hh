// trio.hh - Trio Template
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef TRIO_HH
#define TRIO_HH

template <typename T1, typename T2, typename T3> struct trio {
    T1 first;
    T2 second;
    T3 third;

    trio() : first(), second(), third() { }

    trio(const T1& a, const T2& b, const T3& c) 
        : first(a), second(b), third(c) { }

    template <typename U1, typename U2, typename U3>
    trio(const trio<U1, U2, U3>& t) 
        : first(t.first), second(t.second), third(t.third) { }
};

// template <typename T1, typename T2, typename T3>
// inline bool operator==(const trio<T1, T2, T3>& x, const trio<T1, T2, T3>& y) { 
//     return x.first == y.first && x.second == y.second && x.third == y.third;
// }
// 
// template <typename T1, typename T2, typename T3>
// inline bool operator!=(const trio<T1, T2, T3>& x, const trio<T1, T2, T3>& y) { 
//   return !(x == y);
// }
// 
// template <typename T1, typename T2, typename T3> 
// inline trio<T1, T2, T3> make_trio(T1 x, T2 y, T3 z) {
//     return trio<T1, T2, T3>(x, y, z);
// }

#endif // End of: #ifndef TRIO_HH
