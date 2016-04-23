#include <Arduino.h>
#include "FS.h"

String loginPage() {
  char* html;
  size_t size = 0;
  snprintf(html, size, "");
  return String(html);
}
