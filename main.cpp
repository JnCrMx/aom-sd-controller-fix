#include <lime/entrypoint.hpp>
#include <lime/module.hpp>
#include <lime/hooks/hook.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "exports.hpp"

#include <windows.h>

enum class EInputActionOrigin {
    None = 0,
    SteamController_A = 1,
    SteamController_B = 2,
    SteamController_X = 3,
    SteamController_Y = 4,
    SteamController_LeftBumper = 5,
    SteamController_RightBumper = 6,
    SteamController_LeftGrip = 7,
    SteamController_RightGrip = 8,
    SteamController_Start = 9,
    SteamController_Back = 10,
    SteamController_LeftPad_Touch = 11,
    SteamController_LeftPad_Swipe = 12,
    SteamController_LeftPad_Click = 13,
    SteamController_LeftPad_DPadNorth = 14,
    SteamController_LeftPad_DPadSouth = 15,
    SteamController_LeftPad_DPadWest = 16,
    SteamController_LeftPad_DPadEast = 17,
    SteamController_RightPad_Touch = 18,
    SteamController_RightPad_Swipe = 19,
    SteamController_RightPad_Click = 20,
    SteamController_RightPad_DPadNorth = 21,
    SteamController_RightPad_DPadSouth = 22,
    SteamController_RightPad_DPadWest = 23,
    SteamController_RightPad_DPadEast = 24,
    SteamController_LeftTrigger_Pull = 25,
    SteamController_LeftTrigger_Click = 26,
    SteamController_RightTrigger_Pull = 27,
    SteamController_RightTrigger_Click = 28,
    SteamController_LeftStick_Move = 29,
    SteamController_LeftStick_Click = 30,
    SteamController_LeftStick_DPadNorth = 31,
    SteamController_LeftStick_DPadSouth = 32,
    SteamController_LeftStick_DPadWest = 33,
    SteamController_LeftStick_DPadEast = 34,
    SteamController_Gyro_Move = 35,
    SteamController_Gyro_Pitch = 36,
    SteamController_Gyro_Yaw = 37,
    SteamController_Gyro_Roll = 38,
};

namespace function_addresses {
    constexpr std::uintptr_t get_button_code = 0x140ac7560;
}

void lime::load() {
    auto new_logger = spdlog::basic_logger_mt("AoM SteamDeck Controller Fix", "aom-sd-controller-fix.log", true);
    new_logger->flush_on(spdlog::level::info);
    new_logger->set_level(spdlog::level::debug);
    spdlog::set_default_logger(new_logger);

    spdlog::info("Loaded.");

    char original[MAX_PATH];
    GetSystemDirectory(original, sizeof(original));
    strcat_s(original, "\\VERSION.dll");
    spdlog::info("Proxying VERSION.dll at {}", original);
    if(lime::proxy::setup(original)) {
        spdlog::info("Proxying successful.");
    } else {
        spdlog::error("Proxying failed.");
        return;
    }

    using hook_t = lime::hook<int(int, int*, long long), lime::convention::automatic>;
    hook_t::create(function_addresses::get_button_code, [](auto* hook, int x, int* y, long long z) -> int {
        int original = hook->original()(x, y, z);
        spdlog::trace("Hooked function called with x: {}, y: {}, z: {}. Original return value: {}", x, (void*)y, z, original);
        if(original >= 304) {
            switch(original) {
                case 305:
                    return static_cast<int>(EInputActionOrigin::SteamController_A);
                case 306:
                    return static_cast<int>(EInputActionOrigin::SteamController_B);
                case 307:
                    return static_cast<int>(EInputActionOrigin::SteamController_X);
                case 308:
                    return static_cast<int>(EInputActionOrigin::SteamController_Y);
                case 309:
                    return static_cast<int>(EInputActionOrigin::SteamController_LeftBumper);
                case 310:
                    return static_cast<int>(EInputActionOrigin::SteamController_RightBumper);
                case 311:
                    return static_cast<int>(EInputActionOrigin::SteamController_Start);
                case 327:
                    return static_cast<int>(EInputActionOrigin::SteamController_LeftTrigger_Pull);
                case 329:
                    return static_cast<int>(EInputActionOrigin::SteamController_RightTrigger_Pull);
                default:
                    original -= 302;
            }
        }
        if(original > 38) {
            spdlog::warn("Attempting to return out-of-bounds value: {}; returning 0 instead to prevent crash", original);
            return 0;
        }
        return original;
    });
}

void lime::unload() {
    spdlog::info("Unloaded.");
}
