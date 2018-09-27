#pragma once

#include <emscripten/val.h>

#include <optional>
#include <js/bind.hpp>

namespace belle::vue {
  using namespace std::string_literals;
  using emscripten::val;


  struct Element {
    Element(const val& v) : val_(v) {}
    Element() : val_(val::global("document")) {}

    bool hasAttribute(const std::string& attr) const {
      return val_.call<bool>("hasAttribute", attr);
    }

    std::string getAttribute(const std::string& attr) const {
      return val_.call<std::string>("getAttribute", attr);
    }

    void setAttribute(const std::string& attr, const std::string& value) {
      val_.set(attr, value);
    }

    std::string tagName() const {
      return val_["tagName"].as<std::string>();
    }

    std::string innerHTML() const {
      return val_["innerHTML"].as<std::string>();
    }

    void innerHTML(const std::string& html) {
      val_.set("innerHTML", html);
    }

    Element parentElement() const {
      return val_["parentElement"].as<val>();
    }

    Element replaceChild(Element newnode, Element oldnode) {
      return val_.call<val>("replaceChild", newnode.val_, oldnode.val_);
    }

    std::size_t compareDocumentPosition(Element node) {
      return val_.call<std::size_t>("compareDocumentPosition", node.val_);
    }

    std::vector<Element> children() const {
      auto v = val_["children"];
      auto l = v["length"].as<unsigned>();

      std::vector<Element> rv;
      for(unsigned i = 0; i < l; ++i) {
          rv.push_back(v[i].as<val>());
      }

      return rv;
    }

    std::optional<Element> insertAdjacentElement(const std::string& position, const Element& el) {
      return val_.call<val>("insertAdjacentElement", position, el.val_);
    }

    Element removeChild(const Element& child) {
      return val_.call<val>("removeChild", child.val_);
    }

    Element appendChild(const Element& child) {
      return val_.call<val>("appendChild", child.val_);
    }

    Element cloneNode(bool deep = false) const {
      return val_.call<val>("cloneNode", deep);
    }

    std::string toString() const {
      return val_.call<std::string>("toString");
    }


    operator val() const { return val_; }

    val val_;
  };

  inline Element get_element_by_id(const std::string& id) {
    return val::global("document").call<val>("getElementById", id);
  }

  inline std::vector<Element> get_elements_by_class_name(const std::string& class_name) {
    val elements = val::global("document").call<val>("getElementsByClassName", class_name);

    auto l = elements["length"].as<unsigned>();

    std::vector<Element> rv;
    for(unsigned i = 0; i < l; ++i) {
        rv.push_back(elements[i].as<val>());
    }

    return rv;

  }

  //! return an id than can be used to unregister the interval
  inline std::size_t set_interval(std::function<void()>& on_interval, std::size_t interval) {
    return val::global("setInterval")(js::bind(on_interval), interval).as<std::size_t>();
  }

  inline void clear_interval(std::size_t interval_id) {
    val::global("clearInterval")(interval_id);
  }
}

