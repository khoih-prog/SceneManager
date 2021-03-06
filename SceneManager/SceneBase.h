#pragma once
#ifndef ARDUINO_SCENE_MANAGER_SCENE_BASE_H
#define ARDUINO_SCENE_MANAGER_SCENE_BASE_H

#include <Arduino.h>
#include "util/PollingTimer/FrameRateCounter.h"

namespace arduino {
namespace scene {

class Manager;

class Base : public FrameRateCounter
{
    friend class Manager;

protected:

    String name;
    bool b_prev_running {false};

public:

    Base(const String& name, const double fps) : FrameRateCounter(fps), name(name) {};
    Base(const Base&) = default;
    Base& operator= (const Base&) = default;
    Base(Base&&) noexcept = default;
    Base& operator = (Base&&) noexcept = default;
    virtual ~Base() noexcept = default;

    virtual void begin() {};
    virtual void enter() {};
    virtual void update() = 0;
    virtual void exit() {};
    virtual void reset() {};

    bool hasEnter() const { return !b_prev_running && isRunning(); }
    bool hasExit() const { return b_prev_running && !isRunning(); }

    const String& getName() const { return name; }

protected:

    void callUpdate()
    {
        if (hasEnter())
        {
            b_prev_running = isRunning();
            enter();
        }

        if (isRunning()) update();

        if (hasExit())
        {
            b_prev_running = isRunning();
            exit();
        }
    }
};

} // scene
} // arduino

#endif // ARDUINO_SCENE_MANAGER_SCENE_BASE_H
