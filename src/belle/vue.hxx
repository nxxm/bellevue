#pragma once

#include <belle/vue/dom.hxx>
#include <belle/vue/fx.hxx>
#include <pre/fusion/for_each_member.hpp>
#include <pre/json/traits/is_container.hpp>

#include <mstch/mstch.hpp>

#include <memory>

#include <belle/vue/observable.hxx>

namespace belle::vue {

  using namespace std::string_literals;

  template<class T>
  struct vfor_observer {
    Element parent_element;
    Element insertion_parent;
    Element vfor_node_template;
 
    
    using observed_t = typename T::observed_t;


    vfor_observer(Element& child) {
      parent_element = child.parentElement();
      insertion_parent = parent_element.parentElement();
      vfor_node_template = child;
    }

    //! Called on data change
    void operator()(const observed_t& collection) {
      std::cout << "here we are ! " << std::endl;
      std::cout << "collection : " << collection.size() << std::endl;
      auto tpl_html = vfor_node_template.innerHTML();

      std::cout << "parent_element : " << parent_element.toString() << std::endl;
      std::cout << "parent_element - children : " << parent_element.children().size() << std::endl;
      auto new_parent = parent_element.cloneNode(true);
      new_parent.innerHTML("");

      std::cout << "insertion_parent : " << insertion_parent.toString() <<  insertion_parent.innerHTML() << std::endl;
      std::cout << "new_parent : " << new_parent.toString() <<  new_parent.innerHTML() << std::endl;
      std::cout << "parent_element: " << parent_element.toString() << parent_element.innerHTML() << std::endl;

      Element previous_child = vfor_node_template.cloneNode(true);
      auto new_child = previous_child;

      new_parent.appendChild(new_child);


      for (auto iter = collection.begin(); iter != collection.end(); ++iter) {
        const auto& elem = *iter;
        //std::cout << "elem : " << elem.name << std::endl;

        mstch::map m;
        pre::fusion::for_each_member(elem.get_proxied(), [&](const char* n, const auto& v) {
          std::stringstream ss; ss << v.get_proxied();
          m.insert_or_assign(n, ss.str()); //TODO: here we have to forward to another observer : templateable_dom_observer instead of passing get_proxied().
        });

        new_child.innerHTML(mstch::render(tpl_html, m));

        previous_child = *previous_child.insertAdjacentElement("afterend"s, new_child);
        new_child = vfor_node_template.cloneNode(true);
      }
      
      std::cout << "ALL ADDED !" << std::endl;
      std::cout << "insertion_parent : " << insertion_parent.toString() <<  insertion_parent.innerHTML() << std::endl;
      std::cout << "new_parent : " << new_parent.toString() <<  new_parent.innerHTML() << std::endl;
      std::cout << "parent_element: " << parent_element.toString() << parent_element.innerHTML() << std::endl;

      insertion_parent// for a list replace in the parent of the list.
        .replaceChild(new_parent, parent_element);
      parent_element = new_parent;
      
      std::cout << "parent_element after replace: " << parent_element.toString() << parent_element.innerHTML() << std::endl;
      //insertion_parent = new_parent.parentElement();

      std::cout << "parent element should have been replaced" << std::endl;
    }
  };

  template <class T>
  inline void search_vfor(Element parent_element, T& datamodel) {

    std::cout << parent_element.tagName() << std::endl;
    std::cout <<  parent_element.hasAttribute("v-for") << std::endl;

    for (auto child : parent_element.children()) {

      std::cout << child.tagName() << std::endl;

      auto has_vfor = child.hasAttribute("v-for");

      if (has_vfor) {
        std::cout << "\t has_vfor" << std::endl;
       
        std::string from_model = child.getAttribute("v-for"); 

        pre::fusion::for_each_member(datamodel.get_proxied(), [&](const char* name, auto& value) {
          
          if (name == from_model) {
          
            if constexpr (pre::json::traits::is_container<typename std::decay_t<decltype(value)>::observed_t >::value) {
              // So it exists in datamodel, now keep track of it
              vfor_observer<typename std::decay_t<decltype(value)>> vfor_observer{child};
              
              // We register for containers insertion/deletion
              vfor_observer(value.get_proxied());
              value.observe_onchange(vfor_observer);

              /*auto observer_for_current = [&](const typename std::decay_t<decltype(value)>::observed_t& observed) { 
                auto previous_child = child;
                for (auto iter = value->begin(); iter != value->end(); ++iter) {
                  auto& elem = *iter;
                  std::cout << "elem : " << elem.name << std::endl;
                  auto new_child = child.cloneNode();
                  std::cout << new_child.toString() << std::endl;

                  mstch::map m;
                  pre::fusion::for_each_member(elem, [&](const char* n, const auto& v) {
                    m.insert_or_assign(n, v);
                  });

                  auto tpl_html = child.innerHTML();
                  new_child.innerHTML(mstch::render(tpl_html, m));
                  previous_child = *previous_child.insertAdjacentElement("afterend"s, new_child);
                  previous_child = new_child;
                }*/
                
                

              }
            } else {
              std::cout << name << " is not container " << std::endl;
            }
          });

      }

      search_vfor(child, datamodel);

    }
  }
  
  template <class T>
  struct app {

    app(const std::string& id, observable<T>& d) 
      : d_(d) {
      std::cout << "init vue on : " << id << std::endl; 
      app_dom_rendered = get_element_by_id(id);
      app_dom_untemplated = app_dom_rendered.cloneNode(true);

      std::cout << "Compare Doc pos : " << app_dom_untemplated.compareDocumentPosition(app_dom_rendered) << std::endl;

      std::cout << "init vue on : " << app_dom_rendered.tagName() << std::endl; 
      search_vfor(app_dom_rendered, d_);
    }

   /* void datamodel_init(observable<T>&& d) {
      d_ = d;
      
      //auto rendering_node = app_dom_untemplated.cloneNode(true);
      search_vfor(app_dom_rendered, d_);

      //app_dom_rendered.parentElement().replaceChild(rendering_node, app_dom_rendered);
      //no - app_dom_rendered.val_.set("childNodes", rendering_node.val_["childNodes"]);
    }*/

    observable<T>& d_;

    Element app_dom_rendered;
    Element app_dom_untemplated;
  };


}
