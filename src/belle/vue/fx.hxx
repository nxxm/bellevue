#pragma once

#include <belle/vue/dom.hxx>

namespace belle::vue::fx {
  using emscripten::val;
  using namespace std::string_literals;

  void fade_out(Element elem, std::function<void()> on_done = [](){}, double step = 0.01, size_t step_time = 10) {
  
    std::function<void()> on_interval;
    auto interval_id =  std::make_shared<size_t>(); 

    on_interval = [elem, interval_id, step, on_done](){
      if (elem.val_["style"]["opacity"].as<std::string>() == ""s) {
        elem.val_["style"].set("opacity", "1"s);
      }
      
      auto opacity = std::stod(elem.val_["style"]["opacity"].as<std::string>());
      if (opacity > 0) {
        elem.val_["style"].set("opacity", std::to_string(opacity - step));
      } else {
        clear_interval(*interval_id);
        on_done();
      }     
    };
    *interval_id = set_interval(on_interval, step_time);
  }

  void fade_in(Element elem, std::function<void()> on_done = [](){}, double step = 0.01, size_t step_time = 10) {
  
    std::function<void()> on_interval;
    auto interval_id =  std::make_shared<size_t>(); 

    on_interval = [elem, interval_id, step, on_done](){
      if (elem.val_["style"]["opacity"].as<std::string>() == ""s) {
        elem.val_["style"].set("opacity", "0"s);
      }
      
      auto opacity = std::stod(elem.val_["style"]["opacity"].as<std::string>());
      if (opacity < 1) {
        elem.val_["style"].set("opacity", std::to_string(opacity + step));
      } else {
        clear_interval(*interval_id);
        on_done();
      }     
    };
    *interval_id = set_interval(on_interval, step_time);
  }

}
