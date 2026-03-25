#include "input.h"
#include "editor.h"

void helpModeProcessKey(int c) {
    switch (c) {
        case '\r':
        case '\x1b':
        case 'q':
        case 'Q':
            E.mode = MODE_NORMAL;
            break;
            
        case ARROW_UP:
        case 'k':
            if (E.help_rowoff > 0) {
                E.help_rowoff--;
            }
            break;
            
        case ARROW_DOWN:
        case 'j':
            // E.help_rowoff max check is handled in render logic, or we can just scroll unbounded
            // but we'll limit it somewhat (like 100) or let render handle bounds.
            E.help_rowoff++;
            break;
            
        case PAGE_UP:
            if (E.help_rowoff > E.screenrows) {
                E.help_rowoff -= E.screenrows;
            } else {
                E.help_rowoff = 0;
            }
            break;
            
        case PAGE_DOWN:
            E.help_rowoff += E.screenrows;
            break;
            
        case HOME_KEY:
            E.help_rowoff = 0;
            break;
            
        case END_KEY:
            // Could set to huge number, render logic handles cap
            E.help_rowoff = 9999;
            break;
    }
}
