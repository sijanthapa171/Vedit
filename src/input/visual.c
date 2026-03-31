#include <stdlib.h>
#include <string.h>
#include "editor.h"
#include "input.h"
#include "core.h"
#include "ui.h"

void visualModeProcessKey(int c) {
    switch (c) {
        case '\x1b':
        case 'q':
        case 'v':
            E.mode = MODE_NORMAL;
            E.sel_sx = -1;
            E.sel_sy = -1;
            break;

        case 'h':
        case 'j':
        case 'k':
        case 'l':
        case ARROW_LEFT:
        case ARROW_RIGHT:
        case ARROW_UP:
        case ARROW_DOWN:
            editorMoveCursor(c);
            break;

        case MOUSE_EVENT:
            editorHandleMouseClick();
            break;

        case MOUSE_WHEEL_UP:
        case MOUSE_WHEEL_DOWN:
            editorHandleMouseScroll(c);
            break;
            
        case 'x':
        case 'd':
            editorSaveUndoState();
            E.mode = MODE_NORMAL;
            E.sel_sx = -1;
            E.sel_sy = -1;
            editorSetStatusMessage("Selection action not yet fully implemented");
            break;
    }
}
