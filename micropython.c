#include "port/micropython_embed.h"
#include "micropython.h"

#define MICROPYTHON_APP_PATH_FOLDER STORAGE_APP_DATA_PATH_PREFIX
#define TAG "micropython"

// static const char *example_1 = "print('hello world!', list(x + 1 for x in range(10)), end='eol\\n')";

static char heap[8 * 1024];
static uint8_t mpyBuf[1 * 1024];

int32_t micropython_app() {
  MpApp* app = malloc(sizeof(MpApp));

  app->view_dispatcher = view_dispatcher_alloc();
  app->text_box = text_box_alloc();
  text_box_set_font(app->text_box, TextBoxFontText);

  app->gui = furi_record_open(RECORD_GUI);
  view_dispatcher_add_view(app->view_dispatcher, mpViewTextBox, text_box_get_view(app->text_box));
  view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);
  view_dispatcher_switch_to_view(app->view_dispatcher, mpViewTextBox);

  app->storage = furi_record_open(RECORD_STORAGE);
  app->stream = file_stream_alloc(app->storage);
  if(file_stream_open(app->stream, APP_DATA_PATH("hello.mpy"), FSAM_READ, FSOM_OPEN_EXISTING)) {
    stream_read(app->stream, mpyBuf, sizeof(mpyBuf));
  } else {
    FURI_LOG_E(TAG, "Failed to open file!");
  }

  mp_embed_init(&heap[0], sizeof(heap));
  mp_embed_exec_mpy(mpyBuf, sizeof(mpyBuf));
  mp_embed_deinit();

  furi_assert(app);
  view_dispatcher_remove_view(app->view_dispatcher, mpViewTextBox);
  text_box_free(app->text_box);
  view_dispatcher_free(app->view_dispatcher);
  furi_record_close(RECORD_GUI);
  app->gui = NULL;
  file_stream_close(app->stream);
  stream_free(app->stream);
  furi_record_close(RECORD_STORAGE);
  app->storage = NULL;
  free(app);
  return 0;
}

/* Use when printing mp to screen
void mp_display(int len, char str) {
  snprintf(app->buf, 512, "%.*s", len, str);
  text_box_text_set(app->text_box, app->buf);
}
*/
