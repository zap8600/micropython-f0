#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/text_box.h>
#include <storage/storage.h>
#include <stdlib.h>

typedef struct {
  Gui* gui;
  ViewDispatcher* view_dispatcher;
  TextBox* text_box;
  Storage* storage;
  File* file;
} MpApp;

typedef enum {
  mpViewTextBox,
} mpView;
