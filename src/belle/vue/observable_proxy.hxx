#pragma once

#include <iostream>
#include <functional>
#include <vector>

namespace belle::vue {

template<class T>
struct observable_proxy {


  public:
    using observed_t = T;
    using observers_t = std::vector<std::function<void(const T&)>>;

    class proxy {
      public:
        proxy ( observers_t& observers, T *v) 
          : observers_(observers),
            proxied (v), 
            before_change(*v) {
          //std::cout << "Before size is: " << proxied->size () << std::endl;
        }
        T * operator -> () {
          return proxied;
        }
        ~proxy () {
          //std::cout << "After size is: " << proxied->size () << std::endl;

          for (auto& obs : observers_) { if (before_change != *proxied) obs(*proxied); }
        }
      private:
        observers_t& observers_;
        T * proxied; 
        T before_change;
    };

    observable_proxy () {}    


    template<class... Ts> 
    observable_proxy(Ts...args) : proxied{args...} {}


    //observable_proxy(std::initializer_list<T> l) : proxied(l) {}

//    observable_proxy (T t) : proxied(t) {}    

//    observable_proxy (T& t) : proxied(t) {}    

    observable_proxy (const observable_proxy<T>& other) {
      proxied = other.proxied;
      observers_ = other.observers_;
    }

    observable_proxy<T>& operator=( const observable_proxy<T> &other ) {
      proxied = other.proxied;
      observers_ = other.observers_;
      return *this;
    }

    proxy operator -> () {
       return proxy (observers_, std::addressof(proxied));
    }

    T* operator -> () const {
       return &proxied;
    }
    
    T& get_proxied() {
       return proxied;
    }

    const T& get_proxied() const {
       return proxied;
    }

    std::function<void(const T&)>& observe_onchange(std::function<void(const T&)> observer) {
      observers_.push_back(observer);
      return observers_.back();
    }

    void notify_observers() const {
      for (auto& obs : observers_) { obs(proxied); }
    }


  
    inline friend bool operator==(const observable_proxy<T>& lhs, const observable_proxy<T>& rhs) {
      return lhs.proxied == rhs.proxied;
    }
    inline friend bool operator!=(const observable_proxy<T>& lhs, const observable_proxy<T>& rhs) {
      return lhs.proxied != rhs.proxied;
    }


  private:
    T proxied;
    observers_t observers_;
};


  //template<class T>
  //observable_proxy (T& v) -> observable_proxy<T>;

  //template<class T>
  //observable_proxy (const observable_proxy<T>& v) -> observable_proxy<T>;

}
