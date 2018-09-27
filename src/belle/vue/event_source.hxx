#pragma once

#include <vector>

#include <js/bind.hpp>
#include <emscripten/val.h>


namespace belle::vue {
  using emscripten::val;
  using namespace std::placeholders;
  using namespace std::string_literals;
 

  /**
   * \brief Event returned by the EventSource Server Sent Event system.
   */
  struct event {
    event(val& val) 
      : this_(val) {}
  
    std::string data() const {
      return this_["data"].as<std::string>();
    }

    val target() const {
      return this_["target"].as<val>();
    }

  private:
    val this_;
  };

  /**
   * \brief Server Sent EventSource
   */
  class EventSource : public std::enable_shared_from_this<EventSource> {
  public:

    EventSource(const std::string& url) 
      : this_(val::global("EventSource").new_(url))
    {}

    ~EventSource() {
      std::cout << "Destroying event source" << std::endl;
    }

    void init() {
      this_.call<val>("addEventListener", "open"s, js::bind(&EventSource::_on_open, shared_from_this(), _1), false);
      this_.call<val>("addEventListener", "error"s, js::bind(&EventSource::_on_error, shared_from_this(), _1), false);
      this_.call<val>("addEventListener", "connecttime"s, js::bind(&EventSource::_on_connecttime, shared_from_this(), _1), false);
      this_.call<val>("addEventListener", "message"s, js::bind(&EventSource::_on_message, shared_from_this(), _1), false);
    }

    enum ready_state_t {
      CONNECTING = 0
      , OPEN = 1 
      , CLOSED = 2
    };

    ready_state_t ready_state() {
      return static_cast<ready_state_t>(this_["readyState"].as<int>());
    }
    
    //! Sets the callbacks called when connection opens
    void on_open(std::function<void(const event&)> callback) {
      open_.push_back(callback);
    }

    //! Sets the callbacks called on error 
    void on_error(std::function<void(const event&)> callback) {
      error_.push_back(callback);
    }
    
    //! Sets the callbacks called on connecttime 
    void on_connecttime(std::function<void(const event&)> callback) {
      connecttime_.push_back(callback);
    }
    
    //! Sets the callbacks called on message 
    void on_message(std::function<void(const event&)> callback) {
      message_.push_back(callback);
    }
   

  private:
    val this_;

    std::vector<std::function<void(const event&)>> open_;
    std::vector<std::function<void(const event&)>> error_;
    std::vector<std::function<void(const event&)>> connecttime_;
    std::vector<std::function<void(const event&)>> message_;

    void _on_open(val evt) {
      std::cout << "DEBUG: _on_open" << std::endl;
      std::for_each(open_.begin(), open_.end(), 
        [&](auto& func) { func(evt); });
    }

    void _on_error(val evt) {
      std::cout << "DEBUG: _on_error" << std::endl;
      std::for_each(error_.begin(), error_.end(), 
        [&](auto& func) { func(evt); });
    }

    void _on_connecttime(val evt) {
      std::cout << "DEBUG: _on_connecttime" << std::endl;
      std::for_each(connecttime_.begin(), connecttime_.end(),
        [&](auto& func) { func(evt); });
    }

    void _on_message(val evt) {
      std::cout << "DEBUG: _on_message" << std::endl;
      std::for_each(message_.begin(), message_.end(), 
        [&](auto& func) { func(evt); });
    }

  };

}
