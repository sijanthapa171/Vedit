#include <stdlib.h>
#include "editor.h"
#include "input.h"

void editorHandleMouseClick(void) {
    int filerow = E.mouseY + E.rowoff;
    if (filerow >= 0 && filerow < E.numrows) {
        E.cy = filerow;
        int filecol = E.mouseX - E.ln_width + E.coloff;
        if (filecol < 0) filecol = 0;
        
        int cur_rx = 0;
        int i;
        for (i = 0; i < E.row[E.cy].size; i++) {
            if (E.row[E.cy].chars[i] == '\t')
                cur_rx += (KILO_TAB_STOP - 1) - (cur_rx % KILO_TAB_STOP);
            cur_rx++;
            
            if (cur_rx > filecol) break;
        }
        E.cx = i;
    } else if (filerow >= E.numrows) {
        E.cy = E.numrows > 0 ? E.numrows - 1 : 0;
        if (E.numrows > 0) E.cx = E.row[E.cy].size;
        else E.cx = 0;
    }
}

void editorHandleMouseScroll(int key) {
    if (key == MOUSE_WHEEL_UP) {
        if (E.rowoff > 0) E.rowoff--;
    } else if (key == MOUSE_WHEEL_DOWN) {
        if (E.rowoff + E.screenrows < E.numrows) E.rowoff++;
    }
}
