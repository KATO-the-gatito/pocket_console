#include "global.h"

int buttons_processing(unsigned cnt_btns){
  for (int i = 0; i < cnt_btns; i++)
  {
    flags_btns_state.set(i, !digitalRead(i));
    if (flags_btns_pstate.get(i) != flags_btns_state.get(i)) {  // состояние изменилось
      flags_btns_pstate.set(i, flags_btns_state.get(i));     // запомнить новое
      if (flags_btns_state.get(i)) 
        return i;
    }
  }
  return -1;
}