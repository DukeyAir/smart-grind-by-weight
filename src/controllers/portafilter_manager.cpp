#include "portafilter_manager.h"
#include <Preferences.h>
#include <cstdio>
#include <cmath>

static const char* const kPortafilterNames[PortafilterManager::PF_COUNT] = {"Single", "Double"};
static const char* const kNvsNamespace = "portafilter";

const char* PortafilterManager::get_name(int index) {
    if (index < 0 || index >= PF_COUNT) return "Unknown";
    return kPortafilterNames[index];
}

void PortafilterManager::init() {
    for (auto& e : entries_) {
        e.ref_weight_g = 0.0f;
    }
    load();
}

void PortafilterManager::load() {
    Preferences prefs;
    prefs.begin(kNvsNamespace, true);
    for (int i = 0; i < PF_COUNT; ++i) {
        char key[12];
        snprintf(key, sizeof(key), "pf%d_ref", i);
        entries_[i].ref_weight_g = prefs.getFloat(key, 0.0f);
    }
    prefs.end();
}

void PortafilterManager::save(int index) {
    if (index < 0 || index >= PF_COUNT) return;
    Preferences prefs;
    prefs.begin(kNvsNamespace, false);
    char key[12];
    snprintf(key, sizeof(key), "pf%d_ref", index);
    prefs.putFloat(key, entries_[index].ref_weight_g);
    prefs.end();
}

void PortafilterManager::set_reference_weight(int index, float weight_g) {
    if (index < 0 || index >= PF_COUNT) return;
    entries_[index].ref_weight_g = weight_g;
}

float PortafilterManager::get_reference_weight(int index) const {
    if (index < 0 || index >= PF_COUNT) return 0.0f;
    return entries_[index].ref_weight_g;
}

bool PortafilterManager::is_configured(int index) const {
    return index >= 0 && index < PF_COUNT && entries_[index].ref_weight_g > 0.0f;
}

bool PortafilterManager::any_configured() const {
    for (int i = 0; i < PF_COUNT; ++i) {
        if (entries_[i].ref_weight_g > 0.0f) return true;
    }
    return false;
}

int PortafilterManager::detect(float weight_g) const {
    for (int i = 0; i < PF_COUNT; ++i) {
        if (entries_[i].ref_weight_g <= 0.0f) continue;
        float tolerance = entries_[i].ref_weight_g * PF_DETECT_TOLERANCE_PCT;
        if (fabsf(weight_g - entries_[i].ref_weight_g) <= tolerance) {
            return i;
        }
    }
    return -1;
}

void PortafilterManager::clear(int index) {
    if (index < 0 || index >= PF_COUNT) return;
    entries_[index].ref_weight_g = 0.0f;
    save(index);
}
