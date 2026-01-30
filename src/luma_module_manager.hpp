// ============================================================================
// luma_module_manager.hpp - C/C++ Bridge Header
// ============================================================================
// This header provides the interface between:
// - C code (luma_linux.c)
// - C++ code (luma_module_manager.cpp)
//
// The extern "C" linkage ensures that C++ name mangling is disabled,
// allowing C code to call C++ functions directly.
// ============================================================================

#pragma once

#ifndef LUMA_MODULE_MANAGER_HPP
#define LUMA_MODULE_MANAGER_HPP

// ============================================================================
// C++ GUARD
// ============================================================================
// When compiled by a C++ compiler, this disables name mangling for the
// enclosed function declarations, making them callable from C code.
#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// EXPORTED FUNCTIONS (Implemented in luma_module_manager.cpp)
// ============================================================================

/**
 * Initialize the Luma module manager and ImGui context
 * 
 * This function must be called once during mod initialization, before any
 * other Luma functions are used. It sets up:
 * - ImGui context
 * - OpenGL ES 3 backend
 * - Module registry
 * - UI theme
 * 
 * Thread Safety: Must be called from the main thread
 */
void init_luma_manager(void);

/**
 * Toggle the visibility of the Luma menu
 * 
 * This function is typically called when the user presses the menu hotkey
 * (default: 'K' key). It alternates between showing and hiding the UI.
 * 
 * Thread Safety: Must be called from the main thread
 */
void toggle_luma_menu(void);

/**
 * Execute one frame of UI rendering
 * 
 * This function should be called every frame from the game's render loop.
 * It handles:
 * - ImGui frame setup
 * - UI rendering
 * - Input processing
 * - Drawing the final frame
 * 
 * If the menu is not open, this function returns immediately without
 * performing any rendering.
 * 
 * Thread Safety: Must be called from the rendering thread
 */
void run_luma_tick(void);

// ============================================================================
// CLOSE C++ GUARD
// ============================================================================
#ifdef __cplusplus
}
#endif

#endif // LUMA_MODULE_MANAGER_HPP
