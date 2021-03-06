//
//  deep_sleep_component.h
//  esphomelib
//
//  Created by Otto Winter on 15.04.18.
//  Copyright © 2018 Otto Winter. All rights reserved.
//

#ifndef ESPHOMELIB_DEEP_SLEEP_COMPONENT_H
#define ESPHOMELIB_DEEP_SLEEP_COMPONENT_H

#include "esphomelib/component.h"
#include "esphomelib/helpers.h"
#include "esphomelib/defines.h"

#ifdef USE_DEEP_SLEEP

ESPHOMELIB_NAMESPACE_BEGIN

#ifdef ARDUINO_ARCH_ESP32

/** The values of this enum define what should be done if deep sleep is set up with a wakeup pin on the ESP32
 * and the scenario occurs that the wakeup pin is already in the wakeup state.
 */
enum WakeupPinMode {
  WAKEUP_PIN_MODE_IGNORE = 0, ///< Ignore the fact that we will wake up when going into deep sleep.
  WAKEUP_PIN_MODE_KEEP_AWAKE, ///< As long as the wakeup pin is still in the wakeup state, keep awake.

  /** Automatically invert the wakeup level. For example if we were set up to wake up on HIGH, but the pin
   * is already high when attempting to enter deep sleep, re-configure deep sleep to wake up on LOW level.
   */
  WAKEUP_PIN_MODE_INVERT_WAKEUP,
};

#endif

/** This component allows setting up the node to go into deep sleep mode to conserve battery.
 *
 * To set this component up, first set *when* the deep sleep should trigger using set_run_cycles
 * and set_run_duration, then set how long the deep sleep should last using set_sleep_duration and optionally
 * on the ESP32 set_wakeup_pin.
 */
class DeepSleepComponent : public Component {
 public:
  /// Set the duration in ms the component should sleep once it's in deep sleep mode.
  void set_sleep_duration(uint32_t time_ms);
#ifdef ARDUINO_ARCH_ESP32
  /** Set the pin to wake up to on the ESP32 once it's in deep sleep mode.
   * Use the inverted property to set the wakeup level.
   */
  void set_wakeup_pin(GPIOInputPin pin);

  void set_wakeup_pin_mode(WakeupPinMode wakeup_pin_mode);
#endif
  /// Set the number of loop cycles after which the node should go into deep sleep mode.
  void set_run_cycles(uint32_t cycles);
  /// Set a duration in ms for how long the code should run before entering deep sleep mode.
  void set_run_duration(uint32_t time_ms);

  void setup() override;
  void loop() override;
  float get_loop_priority() const override;
  float get_setup_priority() const override;

 protected:
  /// Helper to enter deep sleep mode
  void begin_sleep();

  optional<uint64_t> sleep_duration_;
#ifdef ARDUINO_ARCH_ESP32
  optional<GPIOInputPin> wakeup_pin_;
  WakeupPinMode  wakeup_pin_mode_{WAKEUP_PIN_MODE_IGNORE};
  bool next_enter_deep_sleep_{false};
#endif
  optional<uint32_t> loop_cycles_;
  uint32_t at_loop_cycle_{0};
  optional<uint32_t> run_duration_;
};

ESPHOMELIB_NAMESPACE_END

#endif //USE_DEEP_SLEEP

#endif //ESPHOMELIB_DEEP_SLEEP_COMPONENT_H
