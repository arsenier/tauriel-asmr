#pragma once

#include <inttypes.h>
#include <Arduino.h>

#include "Config.h"
#include "MotionControl.h"

/**
 * @brief Sensor data
 * time - time in seconds since the start of the cyclogram
 */
struct Sensors
{
    float time;
};

/**
 * @brief Motion state
 * v_f0 - forward speed
 * theta_i0 - angular speed
 * is_completed - is the cyclogram completed
 */
struct MotionState
{
    float v_f0;
    float theta_i0;
    bool is_completed;
};

/**
 * @brief Cyclogram
 * Typedef so that we can use it as a function pointer
 */
typedef void (*Cyclogram)(MotionState*, Sensors);
/**
 * @brief Cyclogram macro
 * Macro to create a cyclogram function
 */
#define CYCLOGRAM(name) void name(MotionState *ms, Sensors s)

/*===CYCLOGRAMS===*/
/**
 * @brief Full stop
 */
CYCLOGRAM(STOP)
{
    ms->is_completed = false;
    ms->v_f0 = 0;
    ms->theta_i0 = 0;
}

/**
 * @brief Idle
 * Do nothing and skip to the next cyclogram
 */
CYCLOGRAM(IDLE)
{
    ms->is_completed = true;
    ms->v_f0 = 0;
    ms->theta_i0 = 0;
}

/**
 * @brief Move forward one cell
 */
CYCLOGRAM(FWD)
{
    // Математика
    ms->v_f0 = FORW_SPEED;
    ms->theta_i0 = 0;

    // Логика перехода
    if (s.time >= 1.8)
    // ЗАМЕНИТЕ 1.8 НА ПЕРЕСЧЕТ ПО СКОРОСТИ И РАЗМЕРУ ЯЧЕЙКИ
    {
        ms->is_completed = true;
    }
}

/**
 * @brief Straight-to-straight 90 degrees explore left turn
 */
CYCLOGRAM(SS90EL)
{
    int dir = 1;

    // Математика
    ms->v_f0 = 0.1;

    if (s.time >= 0.225 && s.time < 1.285)
    {
        ms->theta_i0 = 1.48 * dir;
    }
    else
    {
        ms->theta_i0 = 0;
    }

    // Логика перехода
    if (s.time >= 1.51)
    {
        ms->is_completed = true;
    }
}
/*===END OF CYCLOGRAMS===*/

/**
 * @brief Cyclogram program interpreter
 * @note ASMR is combination from ASM (assembly) and R (robot). It's like assembly language for robots, feeding imagination from scheme of operation microcontrollers use. Each action is an instruction, which is executed in a sequence, controlled by a program counter. The program is stored in the memory in the form of an array of instructions. So robot movement can be split too into smaller parts: cyclograms, and each part can be executed separately.
 */
class ASMR
{
private:
    MotionControl *motion_control;

    Cyclogram cycProgram[CYC_PROG_BUF_SIZE] = {IDLE};
    int cycProgCounter = 0;
    int cycEnd = 0;

    uint32_t lastProgStart = 0;

    int rotmod(int in)
    {
        return in % CYC_PROG_BUF_SIZE;
    }

    void incProgCounter()
    {
        cycProgCounter = rotmod(cycProgCounter + 1);
    }

public:
    void addAction(Cyclogram cyc)
    {
        cycEnd = rotmod(cycEnd + 1);
        cycProgram[cycEnd] = cyc;
    }

    void exec()
    {
        Sensors s = {0};
        MotionState ms = {0};

        s.time = millis() / 1000.0 - lastProgStart;

        Serial.print(s.time);
        Serial.print(" ");

        cycProgram[cycProgCounter](&ms, s);

        if(ms.is_completed)
        {
            incProgCounter();
            lastProgStart = millis() / 1000.0;
        }

        // Drive at speeds
        motion_control->tick(ms.v_f0, ms.theta_i0);
        Serial.println(String(ms.v_f0) + " " + String(ms.theta_i0) + " " + String(ms.is_completed));
    }
};



