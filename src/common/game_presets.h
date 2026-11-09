// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "common/settings.h"

namespace Settings {

inline constexpr u64 MarioKartWorldProgramId = 0x0400C3F00006E000ULL;

/// Hosts at or below this much physical memory get the Potato Mode preset automatically.
inline constexpr u64 PotatoModeMemoryLimit = 8ULL * 1024 * 1024 * 1024;

inline void SetMarioKartWorldRecommendedSettings(Values& settings, bool global) {
    const auto session = [global](auto& setting, auto value) {
        setting.SetGlobal(global);
        setting.SetValue(value);
    };
    session(settings.renderer_backend, RendererBackend::Vulkan);
    session(settings.resolution_setup, ResolutionSetup::Res1X);
    session(settings.gpu_accuracy, GpuAccuracy::Low);
    session(settings.use_asynchronous_shaders, true);
    session(settings.astc_recompression, AstcRecompression::Bc3);
    session(settings.vram_usage_mode, VramUsageMode::Conservative);
    session(settings.use_reactive_flushing, false);
    session(settings.vsync_mode, VSyncMode::FifoRelaxed);
    session(settings.use_docked_mode, ConsoleMode::Handheld);
    session(settings.scaling_filter, ScalingFilter::ScaleForce);
    session(settings.anti_aliasing, AntiAliasing::Fxaa);
}

inline bool ApplyMarioKartWorldPreset(Values& settings, u64 program_id, bool accepted) {
    if (!accepted || program_id != MarioKartWorldProgramId) {
        return false;
    }
    SetMarioKartWorldRecommendedSettings(settings, false);
    return true;
}

/// Low-end defaults: fewer pixels for the host GPU to shade, the cheaper cache and sync paths,
/// and cheap display filtering. Nothing here changes emulation accuracy for the guest CPU.
inline void SetPotatoRecommendedSettings(Values& settings, bool global) {
    const auto session = [global](auto& setting, auto value) {
        setting.SetGlobal(global);
        setting.SetValue(value);
    };
    session(settings.renderer_backend, RendererBackend::Vulkan);
    session(settings.resolution_setup, ResolutionSetup::Res3_4X);
    session(settings.gpu_accuracy, GpuAccuracy::Low);
    session(settings.use_asynchronous_shaders, true);
    session(settings.astc_recompression, AstcRecompression::Bc3);
    session(settings.vram_usage_mode, VramUsageMode::Conservative);
    session(settings.use_reactive_flushing, false);
    session(settings.vsync_mode, VSyncMode::FifoRelaxed);
    session(settings.use_docked_mode, ConsoleMode::Handheld);
    session(settings.scaling_filter, ScalingFilter::Bilinear);
    session(settings.anti_aliasing, AntiAliasing::None);
}

/// Whether the preset applies. A reported memory size of zero means the host did not tell us, and
/// Auto stays out of the way rather than guessing.
inline bool PotatoModeShouldApply(PotatoMode mode, u64 total_physical_memory) {
    if (mode == PotatoMode::Off) {
        return false;
    }
    if (mode == PotatoMode::On) {
        return true;
    }
    return total_physical_memory != 0 && total_physical_memory <= PotatoModeMemoryLimit;
}

inline bool ApplyPotatoPreset(Values& settings, PotatoMode mode, u64 total_physical_memory) {
    if (!PotatoModeShouldApply(mode, total_physical_memory)) {
        return false;
    }
    SetPotatoRecommendedSettings(settings, false);
    return true;
}

} // namespace Settings
