// #ifndef _INFINITE_NUMBER_HPP
// #define _INFINITE_NUMBER_HPP

// #include <iostream>

// namespace cpp_utils {


// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// class infinite_number;

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// bool operator == (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// bool operator != (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// bool operator < (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// bool operator <= (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// bool operator > (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// bool operator >= (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// infinite_number<NUM, MINUSINF, PLUSINF> operator + (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// infinite_number<NUM, MINUSINF, PLUSINF> operator + (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const NUM& b);

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// infinite_number<NUM, MINUSINF, PLUSINF> operator + (const NUM& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// infinite_number<NUM, MINUSINF, PLUSINF> operator - (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// infinite_number<NUM, MINUSINF, PLUSINF> operator - (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const NUM& b);

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// infinite_number<NUM, MINUSINF, PLUSINF> operator - (const NUM& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// infinite_number<NUM, MINUSINF, PLUSINF> operator * (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// infinite_number<NUM, MINUSINF, PLUSINF> operator * (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const NUM& b);

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// infinite_number<NUM, MINUSINF, PLUSINF> operator * (const NUM& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// infinite_number<NUM, MINUSINF, PLUSINF> operator / (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// infinite_number<NUM, MINUSINF, PLUSINF> operator / (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const NUM& b);

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// infinite_number<NUM, MINUSINF, PLUSINF> operator / (const NUM& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);

// /**
//  * @brief a number which has a well defined lower bound and upper bound.
//  * 
//  * If the number goes over one of the 2 values it immediately be set to either be lowerbound or the upperbound
//  * 
//  * The template parameters are as follows:
//  *  - the first is the underlying number type;
//  *  - the second is the lowerbound;
//  *  - the third is the upperbound
//  */

// /**
//  * @brief a number which has a lowerbound and upperbound (both included)
//  * 
//  * @tparam NUM 
//  * @tparam MINUSINF 
//  * @tparam PLUSINF 
//  */
// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
// class infinite_number {
//     friend bool operator == <>(const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);
//     friend bool operator != <>(const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);
//     friend bool operator > <>(const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);
//     friend bool operator >= <>(const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);
//     friend bool operator < <>(const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);
//     friend bool operator <= <>(const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);
//     friend infinite_number<NUM, MINUSINF, PLUSINF> operator + <>(const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);
//     friend infinite_number<NUM, MINUSINF, PLUSINF> operator + <>(const infinite_number<NUM, MINUSINF, PLUSINF>& a, const NUM& b);
//     friend infinite_number<NUM, MINUSINF, PLUSINF> operator + <>(const NUM& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);
//     friend infinite_number<NUM, MINUSINF, PLUSINF> operator - <>(const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);
//     friend infinite_number<NUM, MINUSINF, PLUSINF> operator - <>(const infinite_number<NUM, MINUSINF, PLUSINF>& a, const NUM& b);
//     friend infinite_number<NUM, MINUSINF, PLUSINF> operator - <>(const NUM& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);
//     friend infinite_number<NUM, MINUSINF, PLUSINF> operator * <>(const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);
//     friend infinite_number<NUM, MINUSINF, PLUSINF> operator * <>(const infinite_number<NUM, MINUSINF, PLUSINF>& a, const NUM& b);
//     friend infinite_number<NUM, MINUSINF, PLUSINF> operator * <>(const NUM& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);
//     friend infinite_number<NUM, MINUSINF, PLUSINF> operator / <>(const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);
//     friend infinite_number<NUM, MINUSINF, PLUSINF> operator / <>(const infinite_number<NUM, MINUSINF, PLUSINF>& a, const NUM& b);
//     friend infinite_number<NUM, MINUSINF, PLUSINF> operator / <>(const NUM& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b);
// private:
//     NUM value;
// public:
//     explicit infinite_number<NUM, MINUSINF, PLUSINF>(NUM value) : value{value} { 
//         this->restrainValue(); 
//     }
//     infinite_number<NUM, MINUSINF, PLUSINF>(const infinite_number<NUM, MINUSINF, PLUSINF>& value) : value{value.value} { 
//     }
//     explicit infinite_number<NUM, MINUSINF, PLUSINF>(infinite_number<NUM, MINUSINF, PLUSINF> && other) : value{other.value} { 
//     }
//     ~infinite_number() { }
// private:
//     void restrainValue() {
//         if (this->value > PLUSINF) {
//             this->value = PLUSINF;
//         } else if (this->value < MINUSINF) {
//             this->value = MINUSINF;
//         }
//     }
// public:
//     infinite_number& operator = (const infinite_number<NUM, MINUSINF, PLUSINF>& other) {
//         if (this != &other) {
//             this->value = other.value;
//         }
//         return *this;
//     }
    
//     infinite_number& operator = (infinite_number && other) {
//         this->value = other.value;
//         this->restrainValue();
//         return *this;
//     }

//     infinite_number& operator += (const NUM other) {
//         this->value += value;
//         this->restrainValue();
//         return *this;
//     }
//     infinite_number& operator += (const infinite_number<NUM, MINUSINF, PLUSINF>& other) {
//         this->value += other.value;
//         this->restrainValue();
//         return *this;
//     }

//     infinite_number& operator -= (const NUM other) {
//         this->value -= value;
//         this->restrainValue();
//         return *this;
//     }
//     infinite_number& operator -= (const infinite_number<NUM, MINUSINF, PLUSINF>& other) {
//         this->value -= other.value;
//         this->restrainValue();
//         return *this;
//     }

//     infinite_number& operator *= (const NUM other) {
//         this->value *= value;
//         this->restrainValue();
//         return *this;
//     }
//     infinite_number& operator *= (const infinite_number<NUM, MINUSINF, PLUSINF>& other) {
//         this->value *= other.value;
//         this->restrainValue();
//         return *this;
//     }

//     infinite_number& operator /= (const NUM other) {
//         this->value /= value;
//         this->restrainValue();
//         return *this;
//     }
//     infinite_number& operator /= (const infinite_number<NUM, MINUSINF, PLUSINF>& other) {
//         this->value /= other.value;
//         this->restrainValue();
//         return *this;
//     }

//     infinite_number operator -() {
//         return infinite_number{-this->value};
//     }

//     explicit operator NUM() {
//         return this->value;
//     }

//     std::ostream& operator<<(std::ostream& os) {
//         os << this->value;
//         return os;
//     }

// };

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  bool operator == (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b) {
//     return a.value == b.value;
// }
// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  bool operator != (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b) {
//     return a.value != b.value;
// }

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  bool operator < (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b) {
//     return a.value < b.value;
// }

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  bool operator <= (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b) {
//     return a.value <= b.value;
// }

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  bool operator > (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b) {
//     return a.value > b.value;
// }

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  bool operator >= (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b) {
//     return a.value >= b.value;
// }

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  infinite_number<NUM, MINUSINF, PLUSINF> operator + (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b) {
//     infinite_number<NUM, MINUSINF, PLUSINF> result{a.value + b.value};
//     return result;
// }

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  infinite_number<NUM, MINUSINF, PLUSINF> operator + (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const NUM& b) {
//     return infinite_number<NUM, MINUSINF, PLUSINF>{a.value + b};
// }

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  infinite_number<NUM, MINUSINF, PLUSINF> operator + (const NUM& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b) {
//     return infinite_number<NUM, MINUSINF, PLUSINF>{a + b.value};
// }

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  infinite_number<NUM, MINUSINF, PLUSINF> operator - (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b) {
//     return infinite_number<NUM, MINUSINF, PLUSINF>{a.value - b.value};
// }

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  infinite_number<NUM, MINUSINF, PLUSINF> operator - (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const NUM& b) {
//     return infinite_number<NUM, MINUSINF, PLUSINF>{a.value - b};
// }

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  infinite_number<NUM, MINUSINF, PLUSINF> operator - (const NUM& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b) {
//     return infinite_number<NUM, MINUSINF, PLUSINF>{a - b.value};
// }

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  infinite_number<NUM, MINUSINF, PLUSINF> operator * (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b) {
//     return infinite_number<NUM, MINUSINF, PLUSINF>{a.value * b.value};
// }

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  infinite_number<NUM, MINUSINF, PLUSINF> operator * (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const NUM& b) {
//     return infinite_number<NUM, MINUSINF, PLUSINF>{a.value * b};
// }

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  infinite_number<NUM, MINUSINF, PLUSINF> operator * (const NUM& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b) {
//     return infinite_number<NUM, MINUSINF, PLUSINF>{a * b.value};
// }

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  infinite_number<NUM, MINUSINF, PLUSINF> operator / (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b) {
//     return infinite_number<NUM, MINUSINF, PLUSINF>{a.value / b.value};
// }

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  infinite_number<NUM, MINUSINF, PLUSINF> operator / (const infinite_number<NUM, MINUSINF, PLUSINF>& a, const NUM& b) {
//     return infinite_number<NUM, MINUSINF, PLUSINF>{a.value / b};
// }

// template<typename NUM, NUM MINUSINF, NUM PLUSINF>
//  infinite_number<NUM, MINUSINF, PLUSINF> operator / (const NUM& a, const infinite_number<NUM, MINUSINF, PLUSINF>& b) {
//     return infinite_number<NUM, MINUSINF, PLUSINF>{a / b.value};
// }


// }
// #endif