#pragma once
#include "../config/grind_control.h"

// Manages up to 2 portafilter reference weights for auto-detection.
// Each portafilter maps to a profile tab index (0 = Single, 1 = Double).
// Detection compares the current settled scale weight against stored reference
// weights using a percentage tolerance (PF_DETECT_TOLERANCE_PCT).

struct PortafilterEntry {
    float ref_weight_g;  // 0.0 = not configured
};

class PortafilterManager {
public:
    static constexpr int PF_COUNT = 2;

    static const char* get_name(int index);

    void init();   // loads from NVS
    void load();
    void save(int index);

    void  set_reference_weight(int index, float weight_g);
    float get_reference_weight(int index) const;
    bool  is_configured(int index) const;
    bool  any_configured() const;

    // Returns profile tab index (0 or 1) matching weight_g, or -1 if no match.
    int detect(float weight_g) const;

    void clear(int index);

private:
    PortafilterEntry entries_[PF_COUNT] = {};
};
