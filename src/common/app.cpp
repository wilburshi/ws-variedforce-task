#include "app.hpp"
#include "common/ws.hpp"
#include <GLFW/glfw3.h>

#define ENABLE_RENDER_WIN_COPY (0)

namespace ws {

int App::run() {
  if (!ws::initialize_glfw()) {
    printf("Failed to initialize glfw.\n");
    return 0;
  }

  auto gui_win_res = ws::create_glfw_context();
  if (!gui_win_res) {
    return 0;
  }

  auto render_win_res = ws::create_glfw_context();
  if (!render_win_res) {
    return 0;
  }

#if ENABLE_RENDER_WIN_COPY
  auto render_win_res_copy = ws::create_glfw_context();
  if (!render_win_res_copy) {
    return 0;
  }
#endif

  auto gui_win = gui_win_res.value();
  auto render_win = render_win_res.value();
#if ENABLE_RENDER_WIN_COPY
  auto render_win_copy = render_win_res_copy.value();
#endif

  auto gui_res = ws::create_imgui_context(gui_win.window);
  if (!gui_res) {
    ws::terminate_glfw();
    ws::destroy_glfw_context(&gui_win);
    ws::destroy_glfw_context(&render_win);
#if ENABLE_RENDER_WIN_COPY
    ws::destroy_glfw_context(&render_win_copy);
#endif
    return 0;
  }
  auto imgui_context = gui_res.value();

  auto* lever_sys = ws::lever::get_global_lever_system();
  ws::lever::initialize(lever_sys, 2, levers.data());

  glfwMakeContextCurrent(render_win.window);
  ws::gfx::init_rendering();
  ws::audio::init_audio();

#if ENABLE_RENDER_WIN_COPY
  glfwMakeContextCurrent(render_win_copy.window);
  ws::gfx::init_rendering();
#endif

  setup();

#if ENABLE_RENDER_WIN_COPY
  while (!glfwWindowShouldClose(gui_win.window) && !glfwWindowShouldClose(render_win.window) && !glfwWindowShouldClose(render_win_copy.window)) {
#else
  while (!glfwWindowShouldClose(gui_win.window) && !glfwWindowShouldClose(render_win.window)) {
#endif
    glfwPollEvents();

    ws::lever::update(lever_sys);
    ws::pump::submit_commands();

    always_update();

    {
      glfwMakeContextCurrent(gui_win.window);
      ws::update_framebuffer_dimensions(&gui_win);
      ws::new_frame(&imgui_context, gui_win.framebuffer_width, gui_win.framebuffer_height);

      gui_update();

      ws::render(&imgui_context);
      glfwSwapBuffers(gui_win.window);

    }

    if (!start_render)
    {
      glfwMakeContextCurrent(render_win.window);
      ws::update_framebuffer_dimensions(&render_win);
      ws::gfx::new_frame(render_win.framebuffer_width, render_win.framebuffer_height);

      glfwSwapBuffers(render_win.window);

#if ENABLE_RENDER_WIN_COPY
      glfwMakeContextCurrent(render_win_copy.window);
      ws::update_framebuffer_dimensions(&render_win_copy);
      ws::gfx::new_frame(render_win_copy.framebuffer_width, render_win_copy.framebuffer_height);

      glfwSwapBuffers(render_win_copy.window);
#endif
    }


    if (start_render) {

#if ENABLE_RENDER_WIN_COPY
      glfwMakeContextCurrent(render_win_copy.window);
      ws::update_framebuffer_dimensions(&render_win_copy);
      ws::gfx::new_frame(render_win_copy.framebuffer_width, render_win_copy.framebuffer_height);

      task_update();

      ws::gfx::submit_frame();
      glfwSwapBuffers(render_win_copy.window);
#endif

      glfwMakeContextCurrent(render_win.window);
      ws::update_framebuffer_dimensions(&render_win);
      ws::gfx::new_frame(render_win.framebuffer_width, render_win.framebuffer_height);

      task_update();

      ws::gfx::submit_frame();
      glfwSwapBuffers(render_win.window);

    }
  }

  shutdown();

  ws::audio::terminate_audio();
  ws::gfx::terminate_rendering();
  ws::lever::terminate(lever_sys);
  ws::pump::terminate_pump_system();
  ws::destroy_imgui_context(&imgui_context);
  ws::destroy_glfw_context(&gui_win);
  ws::destroy_glfw_context(&render_win);
#if ENABLE_RENDER_WIN_COPY
  ws::destroy_glfw_context(&render_win_copy);
#endif
  ws::terminate_glfw();
  return 0;
}

}
