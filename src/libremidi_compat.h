// Lightweight compatibility shim to expose a small subset of the older
// libremidi (RtMidi-like) API used by this project on top of the
// libremidi v4 API bundled as a submodule.
#pragma once
#include "libremidi/include/libremidi/libremidi.hpp"
#include <memory>
#include <functional>
#include <string>
#include <vector>

namespace libremidi_compat
{
class midi_in
{
public:
  midi_in() = default;
  ~midi_in() { cancel_callback(); }

  unsigned int get_port_count()
  {
    libremidi::observer obs{};
    return static_cast<unsigned int>(obs.get_input_ports().size());
  }

  std::string get_port_name(unsigned int idx)
  {
    libremidi::observer obs{};
    const auto ports = obs.get_input_ports();
    if (idx < ports.size()) {
      if (!ports[idx].display_name.empty())
        return ports[idx].display_name;
      if (!ports[idx].device_name.empty())
        return ports[idx].device_name;
      return ports[idx].port_name;
    }
    return std::string();
  }

  // set the message callback using the older signature expected by the
  // project (const libremidi::message&). Internally we create a
  // libremidi::input_configuration and a libremidi::midi_in instance.
  void set_callback(std::function<void(const libremidi::message &)> cb)
  {
    message_cb = std::move(cb);
    libremidi::input_configuration conf{};
    conf.on_message = [this](libremidi::message &&m) {
      if (message_cb)
        message_cb(m);
    };
    conf.on_error = [this](libremidi::midi_error e, std::string_view s) {
      if (error_cb)
        error_cb(e, s);
    };
    // Create the underlying midi_in instance with this configuration.
    in = std::make_unique<libremidi::midi_in>(conf);
  }

  void set_error_callback(std::function<void(libremidi::midi_error, std::string_view)> cb)
  {
    error_cb = std::move(cb);
    // If the underlying instance already exists, we don't re-create it here.
  }

  void cancel_callback() { in.reset(); }

  void open_port(int idx)
  {
    if (!in)
      return; // expects set_callback() to have been called first
    libremidi::observer obs{};
    const auto ports = obs.get_input_ports();
    if (idx >= 0 && static_cast<size_t>(idx) < ports.size()) {
      in->open_port(ports[idx]);
    }
  }

  void close_port()
  {
    if (in)
      in->close_port();
  }

  bool is_port_open() const { return in ? in->is_port_open() : false; }

  bool is_port_connected() const { return in ? in->is_port_connected() : false; }

  void set_port_name(const std::string &name)
  {
    if (in)
      in->set_port_name(name);
  }

private:
  std::unique_ptr<libremidi::midi_in> in;
  std::function<void(const libremidi::message &)> message_cb;
  std::function<void(libremidi::midi_error, std::string_view)> error_cb;
};

class midi_out
{
public:
  midi_out() = default;
  ~midi_out() = default;

  unsigned int get_port_count()
  {
    libremidi::observer obs{};
    return static_cast<unsigned int>(obs.get_output_ports().size());
  }

  std::string get_port_name(unsigned int idx)
  {
    libremidi::observer obs{};
    const auto ports = obs.get_output_ports();
    if (idx < ports.size()) {
      if (!ports[idx].display_name.empty())
        return ports[idx].display_name;
      if (!ports[idx].device_name.empty())
        return ports[idx].device_name;
      return ports[idx].port_name;
    }
    return std::string();
  }

private:
};
} // namespace libremidi_compat
