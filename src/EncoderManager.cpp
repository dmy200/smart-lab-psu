//EncoderManager.cpp
#include "EncoderManager.h"
#include "Globals.h"
#include "Config.h"

namespace EncoderManager {

RotaryEncoder encoder(ENC_DT, ENC_CLK);

int encLastPos = 0;
bool editingFuse = false;

static float* editTarget = nullptr;
static bool* editFlag = nullptr;

static constexpr float EDIT_STEP = 0.1f;

static constexpr unsigned long LONG_PRESS_MS = 600;
static constexpr unsigned long MULTICLICK_MS = 400;
static constexpr unsigned long INACTIVITY_TIMEOUT_MS = 30000;

void begin() {
    pinMode(ENC_SW, INPUT_PULLUP);
    encoder.setPosition(0);
    encLastPos = 0;
}

void beginEdit(float* target, bool* flagEditing, bool fuse) {
    editTarget = target;
    editFlag = flagEditing;
    *editFlag = true;
    editingFuse = fuse;
}

void confirmEdit() {
    if (editFlag) *editFlag = false;
}

void cancelEdit() {
    if (editFlag) *editFlag = false;
}

void update() {
    static bool prevBtn = HIGH;
    static uint8_t clickCount = 0;
    static unsigned long lastClickTime = 0;
    static unsigned long pressStartTime = 0;
    static bool longPressHandled = false;
    static unsigned long lastActivityTime = millis();

    encoder.tick();
    int newPos = encoder.getPosition();

    if (newPos != encLastPos) {
        int delta = newPos - encLastPos;
        encLastPos = newPos;
        lastActivityTime = millis();

        if (editTarget && editFlag && *editFlag) {
            *editTarget += delta * EDIT_STEP;
            *editTarget = constrain(*editTarget, 0.0f, 10.0f);
        }
    }

    bool btn = digitalRead(ENC_SW);

    if (prevBtn == HIGH && btn == LOW) {
        pressStartTime = millis();
        longPressHandled = false;
    } else if (prevBtn == LOW && btn == HIGH) {
        unsigned long pressDuration = millis() - pressStartTime;

        if (pressDuration >= LONG_PRESS_MS && !longPressHandled) {
            longPressHandled = true;
            clickCount = 0;
            if (editFlag && *editFlag) cancelEdit();
        } else {
            clickCount++;
            lastClickTime = millis();
        }
        lastActivityTime = millis();
    }
    prevBtn = btn;

    if (clickCount > 0 && millis() - lastClickTime > MULTICLICK_MS) {
        switch (clickCount) {
            case 1:
                if (!(editFlag && *editFlag))
                    beginEdit(editTarget, editFlag, false);
                else
                    confirmEdit();
                break;
            case 2:
                if (editFlag && *editFlag)
                    cancelEdit();
                else
                    beginEdit(editTarget, editFlag, false);
                break;
            case 3:
                // TODO: тройной клик
                break;
        }
        clickCount = 0;
        lastActivityTime = millis();
    }

    if (editFlag && *editFlag && millis() - lastActivityTime > INACTIVITY_TIMEOUT_MS) {
        confirmEdit();
        lastActivityTime = millis();
    }
}

} // namespace EncoderManager
