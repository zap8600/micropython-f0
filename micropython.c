#include "port/micropython_embed.h"
#include "micropython.h"

static const char *example_1 = "print('hello world!', list(x + 1 for x in range(10)), end='eol\\n')";

static char heap[8 * 1024];

int32_t micropython_app() {
  MpApp* app = malloc(sizeof(MpApp));

  app->view_dispatcher = view_dispatcher_alloc();
  app->text_box = text_box_alloc();
  text_box_set_font(app->text_box, TextBoxFontText);

  app->gui = furi_record_open(RECORD_GUI);
  view_dispatcher_add_view(app->view_dispatcher, mpViewTextBox, text_box_get_view(app->text_box));
  view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);
  view_dispatcher_switch_to_view(app->view_dispatcher, mpViewTextBox);

  mp_embed_init(&heap[0], sizeof(heap));
  mp_embed_exec_str(example_1);
  mp_embed_deinit();

  furi_assert(app);
  view_dispatcher_remove_view(app->view_dispatcher, mpViewTextBox);
  text_box_free(app->text_box);
  view_dispatcher_free(app->view_dispatcher);
  furi_record_close(RECORD_GUI);
  app->gui = NULL;
  free(app);
  return 0;
}

/* Use when printing mp to screen
void mp_display(int len, char str) {
  snprintf(app->buf, 512, "%.*s", len, str);
  text_box_text_set(app->text_box, app->buf);
}
*/
