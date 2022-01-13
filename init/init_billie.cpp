/*
 * Copyright (C) 2020 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstdlib>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <sys/sysinfo.h>

#include <android-base/properties.h>
#include "property_service.h"
#include "vendor_init.h"

using android::base::GetProperty;
using std::string;

void property_override(string prop, string value)
{
    auto pi = (prop_info*) __system_property_find(prop.c_str());

    if (pi != nullptr)
        __system_property_update(pi, value.c_str(), value.size());
    else
        __system_property_add(prop.c_str(), prop.size(), value.c_str(), value.size());
}

void vendor_load_properties()
{

    string multiprop;
    string simpleprop;
    string model = GetProperty("ro.boot.product.hardware.sku", "");

    // Check whether model is a single or dual SIM variant and override properties accordingly
    if ((model == "8" || model == "9")) {
        // Model is a Single-SIM Variant
        multiprop = "ssss"; // Set Multi-SIM config to ssss (single-sim)
        simpleprop = "true"; // Disable connectivity simplification behavior (Android Default)
    } else {
        // Model is a Dual-SIM Variant
        multiprop = "dsds"; // Set Multi-SIM config to dsds (dual-sim)
        simpleprop = "false"; // Enable connectivity simplification behavior (Allows switching Data between SIM cards)
    }

    // Set multisim config
    property_override("persist.radio.multisim.config", multiprop);
    property_override("persist.sys.fflag.override.settings_provider_model", simpleprop);
}
