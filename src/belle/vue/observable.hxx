#pragma once

#include <boost/preprocessor/seq/for_each.hpp> 
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/comparison.hpp>
#include <boost/fusion/adapted/struct/detail/preprocessor/is_seq.hpp>

#include <belle/vue/observable_proxy.hxx>

#include <boost/tti/has_type.hpp>


namespace belle::vue {

  BOOST_TTI_HAS_TYPE(observed_t)

  using boost::fusion::operator==;
  using boost::fusion::operator!=;
  template<class T>
  struct observable_aggregate;

  template<class T>
  struct observable : public observable_proxy<T> {

    using observable_proxy<T>::observable_proxy; 

  };

  template <typename... Ts>
  struct observable<std::vector<Ts...> > 
    : public observable_proxy< std::vector< observable<typename std::vector<Ts...>::value_type> > > {
    
    //observable<std::vector<Ts...> >(
    //  std::initializer_list< observable<typename std::vector<Ts...>::value_type> > l ) {
    //  proxied = observed_t(l);
    //}
      

    using observable_proxy< std::vector< observable<typename std::vector<Ts...>::value_type> > >::observable_proxy;

  };

}

#define BELLE_VUE_OBSERVABLE_GEN(r, STRUCT_NAME, elem)                     \
  belle::vue::observable<                                                  \
      decltype(STRUCT_NAME::elem)                                          \
  > elem{};                                                       \

 #define BELLE_VUE_OBSERVABLE_GEN_CTOR(r, STRUCT_NAME, elem)                     \
  , belle::vue::observable< decltype(STRUCT_NAME::elem) > elem

#define BELLE_VUE_OBSERVABLE_GEN_CTOR_FIRST_ARG(r, STRUCT_NAME, elem) \
  belle::vue::observable< decltype(STRUCT_NAME::elem) > elem


//    observable<STRUCT_NAME>( \
//        BOOST_PP_SEQ_FOR_EACH(BELLE_VUE_OBSERVABLE_GEN_CTOR_FIRST_ARG, STRUCT_NAME, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(ATTRIBUTE_SEQ))) \
//      BOOST_PP_SEQ_FOR_EACH(BELLE_VUE_OBSERVABLE_GEN_CTOR, STRUCT_NAME,           \
//        BOOST_PP_SEQ_TAIL(BOOST_PP_SEQ_TAIL(ATTRIBUTE_SEQ)))                 \
//        ) {}                                   


#define BELLE_VUE_OBSERVABLE_BASE(STRUCT_NAME, ATTRIBUTE_SEQ)              \
namespace belle::vue {                                                     \
  template<>\
  struct observable_aggregate<STRUCT_NAME> {\
    BOOST_PP_SEQ_FOR_EACH(BELLE_VUE_OBSERVABLE_GEN, STRUCT_NAME,           \
       BOOST_PP_SEQ_TAIL(ATTRIBUTE_SEQ))                                   \
  }; \
\
  template<>                                                               \
  struct observable<STRUCT_NAME> : public observable_proxy<observable_aggregate<STRUCT_NAME>>  { \
    \
    using observable_proxy<observable_aggregate<STRUCT_NAME>>::observable_proxy; \
        \
        \
  };                                                                       \
}                                                                          \
                                                                           \
  


#define BELLE_VUE_OBSERVABLE_FILLER_0(X)                                   \
    BELLE_VUE_OBSERVABLE_WRAP_ATTR(X)                                      \
    BELLE_VUE_OBSERVABLE_FILLER_1

#define BELLE_VUE_OBSERVABLE_FILLER_1(X)                                   \
    BELLE_VUE_OBSERVABLE_WRAP_ATTR(X)                                      \
    BELLE_VUE_OBSERVABLE_FILLER_0

#define BELLE_VUE_OBSERVABLE_FILLER_0_END
#define BELLE_VUE_OBSERVABLE_FILLER_1_END

#define BELLE_VUE_OBSERVABLE_WRAP_ATTR(X)                                  \
    (X)


#   define BELLE_VUE_OBSERVABLE_ATTRIBUTES_FILLER_OP(r, unused, elem)      \
        BOOST_PP_IF(BOOST_FUSION_PP_IS_SEQ(elem),                          \
            BOOST_PP_CAT( BELLE_VUE_OBSERVABLE_FILLER_0 elem ,_END),       \
            BELLE_VUE_OBSERVABLE_WRAP_ATTR(elem))                          \


#   define BELLE_VUE_ATTRIBUTES_FILLER(...)                                \
        BOOST_PP_SEQ_PUSH_FRONT(                                           \
            BOOST_PP_SEQ_FOR_EACH(                                         \
                BELLE_VUE_OBSERVABLE_ATTRIBUTES_FILLER_OP,                 \
                unused, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))             \
            ,(0))


#   define BELLE_VUE_OBSERVABLE(STRUCT_NAME, ...)                          \
        BELLE_VUE_OBSERVABLE_BASE(                                         \
            STRUCT_NAME,                                                   \
            BELLE_VUE_ATTRIBUTES_FILLER(__VA_ARGS__))                      \
    BOOST_FUSION_ADAPT_STRUCT(belle::vue::observable_aggregate<STRUCT_NAME>,         \
        __VA_ARGS__)                                                       \
    BOOST_FUSION_ADAPT_STRUCT(STRUCT_NAME,         \
        __VA_ARGS__)                                                       \
