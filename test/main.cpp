

#include <screen/screen.h>
#include <iostream>

int main() {
  screen_context_t screen_pctx;
  screen_create_context(&screen_pctx, SCREEN_APPLICATION_CONTEXT);
  /* Create the producer's stream */
  screen_stream_t stream_p;
  auto failed = screen_create_stream(&stream_p, screen_pctx);
  if (failed == -1) {
    std::cout << "failed to create stream \n";
    return -1;
  }

  int buffer_size[2] = {720, 720};
  screen_set_stream_property_iv(stream_p, SCREEN_PROPERTY_BUFFER_SIZE,
                                buffer_size);
  screen_set_stream_property_iv(stream_p, SCREEN_PROPERTY_FORMAT,
                                (const int[]){SCREEN_FORMAT_RGBA8888});
  screen_set_stream_property_iv(
      stream_p, SCREEN_PROPERTY_USAGE,
      (const int[]){SCREEN_USAGE_OPENGL_ES2 | SCREEN_USAGE_WRITE |
                    SCREEN_USAGE_NATIVE});

  failed = screen_create_stream_buffers(stream_p, 2);
  if (failed == -1) {
    std::cout << "failed to create stream buffer\n";
    return -1;
  }

  int permissions;
  screen_get_stream_property_iv(stream_p, SCREEN_PROPERTY_PERMISSIONS,
                                &permissions);
  /* Allow processes in the same group to access the stream */
  permissions |= SCREEN_PERMISSION_IRGRP;
  screen_set_stream_property_iv(stream_p, SCREEN_PROPERTY_PERMISSIONS,
                                &permissions);

  while (1) {
    std::cout << "post stream! \n";
    screen_buffer_t stream_buf = nullptr;
    failed = screen_get_stream_property_pv(  // buffers property
        /* A handle to the buffer or buffers available for rendering. */
        stream_p, SCREEN_PROPERTY_RENDER_BUFFERS, (void**)&stream_buf);
    //  stream_p, SCREEN_PROPERTY_BUFFERS, (void**)&stream_buf);
    //  stream_p, SCREEN_PROPERTY_FRONT_BUFFERS, (void**)&stream_buf);
    if (failed == -1) {
      std::cout << "failed to get  stream buffer\n";
      return -1;
    }

    if (stream_buf == nullptr) {
      std::cout << "failed to get stream buffer form stream\n";
      return -1;
    }

    screen_post_stream(stream_p, stream_buf, 0, /* lect cnt is zero */
                       nullptr, SCREEN_WAIT_IDLE);
  }
}
