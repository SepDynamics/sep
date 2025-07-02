#pragma once

// When compiling without the standard library the build system defines
// `SEP_NO_STDLIB`.  Default builds rely on the system standard library and
// should not define this macro.

// Use C headers instead of C++ headers
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>  // For snprintf
#include <stddef.h> // For ptrdiff_t
#include <cstdlib>
#include <functional>
#include <ostream>
#include <queue>
#include <vector>

namespace sep {
namespace shim {

  // Minimal string implementation
  class string {
  private:
    char* data_;
    size_t size_;
    size_t capacity_;
  public:
    string() : data_(nullptr), size_(0), capacity_(0) {}
    string(const char* s) : data_(nullptr), size_(0), capacity_(0) {
      if (s) {
        size_ = strlen(s);
        capacity_ = size_ + 1;
        data_ = static_cast<char*>(malloc(capacity_));
        if (data_) {
          memcpy(data_, s, size_ + 1);
        } else {
          data_ = nullptr;
          size_ = capacity_ = 0;
        }
      }
    }
    // Construct from std::string
    string(const std::string& other) : string(other.c_str()) {}
    // Move construc
    // t from std::string
    string(std::string&& other) : string(other.c_str()) {}
    string(const string& other) : data_(nullptr), size_(0), capacity_(0) {
      if (other.data_) {
        size_ = other.size_;
        capacity_ = size_ + 1;
        data_ = static_cast<char*>(malloc(capacity_));
        if (data_) {
          memcpy(data_, other.data_, size_ + 1);
        } else {
          size_ = capacity_ = 0;
        }
      }
    }
    string& operator=(const string& other) {
      if (this != &other) {
        if (data_) free(data_);
        data_ = nullptr;
        size_ = capacity_ = 0;
        if (other.data_) {
          size_ = other.size_;
          capacity_ = size_ + 1;
          data_ = static_cast<char*>(malloc(capacity_));
          if (data_) {
            memcpy(data_, other.data_, size_ + 1);
          } else {
            size_ = capacity_ = 0;
          }
        }
      }
      return *this;
    }
    string& operator=(const char* s) {
      if (data_) free(data_);
      data_ = nullptr;
      size_ = capacity_ = 0;
      if (s) {
        size_ = strlen(s);
        capacity_ = size_ + 1;
        data_ = static_cast<char*>(malloc(capacity_));
        if (data_) {
          memcpy(data_, s, size_ + 1);
        } else {
          size_ = capacity_ = 0;
        }
      }
      return *this;
    }
    // Assign from std::string
    string& operator=(const std::string& other) {
      *this = string(other.c_str());
      return *this;
    }
    // Move assign from std::string
    string& operator=(std::string&& other) {
      *this = string(other.c_str());
      return *this;
    }
    // Implicit conversion to std::string
    operator std::string() const { return std::string(c_str()); }
    ~string() {
      if (data_) {
        free(data_);
        data_ = nullptr;
      }
    }
    // Iterator support for compatibility with nlohmann::json
    using value_type = char;
    using size_type = size_t;
    using const_iterator = const char*;
    using iterator = char*;

    const_iterator begin() const { return data_; }
    const_iterator end() const { return data_ ? data_ + size_ : nullptr; }
    iterator begin() { return data_; }
    iterator end() { return data_ ? data_ + size_ : nullptr; }

    // Data access helpers
    const char* data() const { return c_str(); }
    size_t length() const { return size_; }

    // For JSON parsing - provide data() and size() consistently
    const char* c_str() const { return data_ ? data_ : ""; }
    size_t size() const { return size_; }

    // Ensure empty() is defined
    bool empty() const { return size_ == 0; }

    // Character access helpers
    char operator[](size_t pos) const {
        return (pos < size_) ? data_[pos] : '\0';
    }

    char& operator[](size_t pos) { return data_[pos]; }
    
    // Comparison operators
    bool operator==(const char* s) const {
      if (!s) return size_ == 0;
      return strcmp(c_str(), s) == 0;
    }
    bool operator!=(const char* s) const { return !(*this == s); }
    
    // Find operation (basic implementation)
    size_t find(char c, size_t pos = 0) const {
        if (!data_ || pos >= size_) return npos;
        for (size_t i = pos; i < size_; ++i) {
            if (data_[i] == c) return i;
        }
        return npos;
    }

    static const size_t npos = static_cast<size_t>(-1);

    // Substr operation
    string substr(size_t pos = 0, size_t count = npos) const {
        if (pos >= size_) return string();
        size_t rcount = (count == npos || pos + count > size_) ?
                        size_ - pos : count;
        char* temp = static_cast<char*>(malloc(rcount + 1));
        if (temp) {
            memcpy(temp, data_ + pos, rcount);
            temp[rcount] = '\0';
            string result(temp);
            free(temp);
            return result;
        }
        return string();
    }

    // Append operations
    string& append(const char* s) {
        if (s) {
            size_t slen = strlen(s);
            if (size_ + slen + 1 > capacity_) {
                size_t new_cap = (size_ + slen + 1) * 2;
                char* new_data = static_cast<char*>(realloc(data_, new_cap));
                if (new_data) {
                    data_ = new_data;
                    capacity_ = new_cap;
                } else {
                    return *this; // Failed to allocate
                }
            }
            memcpy(data_ + size_, s, slen + 1);
            size_ += slen;
        }
        return *this;
    }

    string& operator+=(const char* s) {
        return append(s);
    }

    string& operator+=(const string& other) {
        return append(other.c_str());
    }

    // Comparison operators
    bool operator==(const string& other) const {
        if (size_ != other.size_) return false;
        if (!data_ && !other.data_) return true;
        if (!data_ || !other.data_) return false;
        return memcmp(data_, other.data_, size_) == 0;
    }

    bool operator!=(const string& other) const {
        return !(*this == other);
    }

    bool operator<(const string& other) const {
        size_t min_size = (size_ < other.size_) ? size_ : other.size_;
        if (data_ && other.data_) {
            int cmp = memcmp(data_, other.data_, min_size);
            if (cmp != 0) return cmp < 0;
        }
        return size_ < other.size_;
    }
  };

  // String concatenation
  inline string operator+(const string& lhs, const string& rhs) {
      string result(lhs);
      result += rhs;
      return result;
  }

  inline string operator+(const string& lhs, const char* rhs) {
      string result(lhs);
      result += rhs;
      return result;
  }

  inline string operator+(const char* lhs, const string& rhs) {
      string result(lhs);
      result += rhs;
      return result;
  }

  inline std::ostream& operator<<(std::ostream& os, const string& s) {
    os << s.c_str();
    return os;
  }

  // Exception classes
  class exception {
  public:
    virtual const char* what() const { return "unknown exception"; }
    virtual ~exception() {}
  };
  
  class runtime_error : public exception {
  private:
    string msg_;
  public:
    runtime_error(const char* msg) : msg_(msg) {}
    const char* what() const override { return msg_.c_str(); }
  };
  
  class system_error : public runtime_error {
  public:
    system_error(int, const char* msg) : runtime_error(msg) {}
  };
  
  // Error handling
  class error_code {
  public:
    error_code() {}
    error_code(int) {}
  };
  
  class error_condition {
  public:
    error_condition() {}
  };
  
  class error_category {
  public:
    virtual const char* name() const { return "sep"; }
    virtual ~error_category() {}
  };
  
  inline const error_category& generic_category() {
    static error_category cat;
    return cat;
  }
  
  inline const error_category& system_category() {
    static error_category cat;
    return cat;
  }
  
  inline error_code make_error_code(int) {
    return error_code();
  }
  
  inline error_condition make_error_condition(int) {
    return error_condition();
  }
  
  // String conversions
  inline int stoi(const string& s) { return atoi(s.c_str()); }
  inline long stol(const string& s) { return atol(s.c_str()); }
  inline long long stoll(const string& s) { return atoll(s.c_str()); }
  inline double stod(const string& s) { return atof(s.c_str()); }
  inline float stof(const string& s) { return (float)atof(s.c_str()); }
  
  inline string to_string(int val) {
    char buf[32];
    (void)snprintf(buf, sizeof(buf), "%d", val);
    return string(buf);
  }
  
  // Type traits
  struct true_type { static constexpr bool value = true; };
  struct false_type { static constexpr bool value = false; };
  
  // Math functions
  inline double exp(double x) { return ::exp(x); }
  inline float exp(float x) { return ::expf(x); }
  
#ifndef isfinite
  inline bool isfinite(double x) {
    // Check if x is finite (not infinite and not NaN)
    return x == x && x != 1.0/0.0 && x != -1.0/0.0;
  }
  inline bool isfinite(float x) {
    return x == x && x != 1.0f/0.0f && x != -1.0f/0.0f;
  }
  inline bool isfinite(long double x) {
    return x == x && x != 1.0L/0.0L && x != -1.0L/0.0L;
  }
#endif
  
#ifndef isinf
  inline bool isinf(double x) {
    // Check if x is positive or negative infinity
    return x == 1.0/0.0 || x == -1.0/0.0;
  }
  inline bool isinf(float x) {
    return x == 1.0f/0.0f || x == -1.0f/0.0f;
  }
  inline bool isinf(long double x) {
    return x == 1.0L/0.0L || x == -1.0L/0.0L;
  }
#endif
  
#ifndef isnan
  inline bool isnan(double x) {
    // NaN is the only value that is not equal to itself
    return x != x;
  }
  inline bool isnan(float x) {
    return x != x;
  }
  inline bool isnan(long double x) {
    return x != x;
  }
#endif
  
  inline double lgamma(double x) { return ::lgamma(x); }
  inline float lgamma(float x) { return ::lgammaf(x); }
  
  inline double log(double x) { return ::log(x); }
  inline float log(float x) { return ::logf(x); }
  
  inline double nextafter(double x, double y) { return ::nextafter(x, y); }
  inline float nextafter(float x, float y) { return ::nextafterf(x, y); }
  
  inline double pow(double x, double y) { return ::pow(x, y); }
  inline float pow(float x, float y) { return ::powf(x, y); }
  
#ifndef signbit
  inline bool signbit(double x) {
    // Check if the sign bit is set (negative number or negative zero)
    union { double d; uint64_t i; } u;
    u.d = x;
    return (u.i & 0x8000000000000000ULL) != 0;
  }
  inline bool signbit(float x) {
    union { float f; uint32_t i; } u;
    u.f = x;
    return (u.i & 0x80000000U) != 0;
  }
  inline bool signbit(long double x) {
    return x < 0.0L || (x == 0.0L && 1.0L / x < 0.0L);
  }
#endif
  
  inline double abs(double x) { return ::fabs(x); }
  inline float abs(float x) { return ::fabsf(x); }
  inline int abs(int x) { return x < 0 ? -x : x; }
  inline long abs(long x) { return x < 0 ? -x : x; }
  
  inline double sqrt(double x) { return ::sqrt(x); }
  inline float sqrt(float x) { return ::sqrtf(x); }
  
  // Memory management - minimal implementations
  inline void* aligned_alloc(size_t alignment, size_t size) {
    (void)alignment;  // alignment parameter unused in fallback implementation
    return ::malloc(size); // Not properly aligned but simple fallback
  }
  
  // Smart pointer helpers were previously defined here when the standard
  // library was unavailable.  With full std support enabled these minimal
  // implementations are removed.
  
  // Containers - minimal stub aliases to std implementations
  template<typename T>
  using allocator = std::allocator<T>;

  using mutex = std::mutex;

  template<typename T>
  using lock_guard = std::lock_guard<T>;

  template<typename T>
  using vector = std::vector<T>;
  
  template<typename K, typename V, typename... Args>
  class map {
  public:
    map() {}
  };
  
  // The project previously defined a very small unordered_map to avoid pulling
  // in the standard library.  With std available this container is no longer
  // needed and has been removed.

  template<typename T>
  class queue {
  public:
    queue() {}
    void push(const T& value) { data_.push_back(value); }
  private:
    vector<T> data_;
  };
  
  template<typename T, size_t N>
  class array {
  public:
    array() {}
  };
  
  // Minimal optional implementation removed now that std::optional is available.
  
  namespace chrono {
    // Enhanced chrono implementation
    using seconds = int;
    using milliseconds = int;
    using microseconds = int;
    using hours = int;
    
    template<typename Rep, typename Period = int>
    class duration {
    private:
      Rep count_;
    public:
      duration() : count_(0) {}
      duration(Rep count) : count_(count) {}
      
      // Conversion constructor from other duration types
      template<typename Rep2, typename Period2>
      duration(const duration<Rep2, Period2>& other) : count_(static_cast<Rep>(other.count())) {}
      
      // Constructor from time point difference
      template<typename TimePoint>
      duration(const TimePoint& tp1, const TimePoint& tp2) : count_(static_cast<Rep>(tp1.time_since_epoch().count() - tp2.time_since_epoch().count())) {}
      
      Rep count() const { return count_; }

      duration operator-(const duration& other) const {
        return duration(count_ - other.count_);
      }
      duration operator+(const duration& other) const {
        return duration(count_ + other.count_);
      }
      duration& operator+=(const duration& other) {
        count_ += other.count_;
        return *this;
      }
    };
    
    template<typename ToDuration, typename Rep, typename Period>
    ToDuration duration_cast(const duration<Rep, Period>& d) {
      return ToDuration(d.count());
    }
    
    struct time_point {
      duration<int> d;
      time_point() : d(0) {}
      explicit time_point(duration<int> dur) : d(dur) {}
      duration<int> time_since_epoch() const { return d; }
    };

    inline duration<int> operator-(const time_point& a, const time_point& b) {
      return a.d - b.d;
    }

    inline bool operator<(const time_point& a, const time_point& b) {
      return a.time_since_epoch().count() < b.time_since_epoch().count();
    }
    inline bool operator<=(const time_point& a, const time_point& b) {
      return a.time_since_epoch().count() <= b.time_since_epoch().count();
    }
    inline bool operator>(const time_point& a, const time_point& b) {
      return b < a;
    }
    inline bool operator>=(const time_point& a, const time_point& b) {
      return !(a < b);
    }
    inline bool operator==(const time_point& a, const time_point& b) {
      return a.time_since_epoch().count() == b.time_since_epoch().count();
    }
    inline bool operator!=(const time_point& a, const time_point& b) {
      return !(a == b);
    }

    inline time_point operator-(const time_point& tp, const duration<int>& d) {
      return time_point(tp.d - d);
    }

    class system_clock {
    public:
      using time_point = chrono::time_point;
      static time_point now() { return time_point(duration<int>(0)); }
    };
    
    class steady_clock {
    public:
      using time_point = chrono::time_point;
      static time_point now() { return time_point(duration<int>(0)); }
    };
  }
  
  namespace chrono_literals {
    inline chrono::milliseconds operator""_ms(unsigned long long ms) {
      return chrono::milliseconds(static_cast<int>(ms));
    }
  }
  
  // Add missing std types that are needed
  class once_flag {
  public:
    once_flag() {}
  };
  
  template<typename Callable, typename... Args>
  void call_once(once_flag&, Callable&&, Args&&...) {
    // Minimal implementation - just call once
  }
  
  // Add algorithm functions
  template<typename InputIt, typename T>
  InputIt remove(InputIt first, InputIt last, const T& value) {
    InputIt result = first;
    for (; first != last; ++first) {
      if (!(*first == value)) {
        *result++ = *first;
      }
    }
    return result;
  }
  
  namespace chrono_literals {
    // Empty implementation
  }
}  // namespace shim
}  // namespace sep

// Previously a specialization of std::hash was provided for sep::shim::string.
// Prefer a dedicated functor to avoid injecting into the std namespace.
namespace sep {
namespace shim {
struct string_hash {
    size_t operator()(const string& s) const {
      size_t hash_val = 5381;
      const char* str = s.c_str();
      for (size_t i = 0; i < s.size(); ++i) {
        hash_val = ((hash_val << 5) + hash_val) +
                   static_cast<unsigned char>(str[i]);
      }
      return hash_val;
    }
  };
}  // namespace shim
}  // namespace sep

namespace std {
    template<>
    struct hash<sep::shim::string> {
        size_t operator()(const sep::shim::string& s) const {
            size_t h = 0;
            const char* data = s.data();
            size_t len = s.size();
            for (size_t i = 0; i < len; ++i) {
                h = h * 31 + static_cast<size_t>(data[i]);
            }
            return h;
        }
    };
}

// Previously shim types were injected into the std namespace when SEP_NO_STDLIB
// was defined. That mapping has been removed; use sep::shim directly instead.
// Previously shim types were injected into the `std` namespace when
// `SEP_NO_STDLIB` was defined.  Those aliases conflicted with the real
// standard library.  Consumers should reference shim types via `sep::shim`.
