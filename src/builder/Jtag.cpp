#include "Jtag.h"

namespace jtag {

tap_state_t Jtag::current_state = TAP_RESET;

Jtag::Jtag() {}

Jtag::~Jtag() {}

int Jtag::tap_get_tms_path(tap_state_t from, tap_state_t to) {
  return tms_seqs[tap_move_ndx(from)][tap_move_ndx(to)].bits;
}

int Jtag::tap_get_tms_path_len(tap_state_t from, tap_state_t to) {
  return tms_seqs[tap_move_ndx(from)][tap_move_ndx(to)].bit_count;
}

int Jtag::tap_move_ndx(tap_state_t state) {

  int ndx = -1;

  switch (state) {
  case TAP_RESET:
    ndx = 0;
    break;
  case TAP_IDLE:
    ndx = 1;
    break;
  case TAP_DRSHIFT:
    ndx = 2;
    break;
  case TAP_DRPAUSE:
    ndx = 3;
    break;
  case TAP_IRSHIFT:
    ndx = 4;
    break;
  case TAP_IRPAUSE:
    ndx = 5;
    break;
  default:
    ALERT("main", "Requested state not found in tap_move_ndx()");
  }

  return ndx;
}

const char *Jtag::tap_state_name(tap_state_t state) {

  unsigned int i;

  for (i = 0; i < ARRAY_SIZE(tap_name_mapping); i++) {
    if (tap_name_mapping[i].symbol == state)
      return tap_name_mapping[i].name;
  }
  return "???";
}
}
