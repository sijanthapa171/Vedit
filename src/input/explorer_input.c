#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "editor.h"
#include "input.h"
#include "utils.h"
#include "commands.h"

void explorerModeProcessKey(int c) {
    switch (c) {
        case '\r':
            editorSelectEntry();
            break;

        case 'o':
            editorSelectEntry();
            break;

        case 'q':
        case '\x1b':
            E.mode = MODE_NORMAL;
            editorSetStatusMessage("Returned to Normal Mode");
            break;

        case 'h':
        case ARROW_LEFT:
            {
                char next_path[1024];
                char *last_slash = strrchr(E.filename, '/');
                if (last_slash) {
                    if (last_slash == E.filename) strcpy(next_path, "/");
                    else {
                        size_t path_len = last_slash - E.filename;
                        strncpy(next_path, E.filename, path_len);
                        next_path[path_len] = '\0';
                    }
                    editorOpen(next_path);
                }
            }
            break;

        case 'l':
        case ARROW_RIGHT:
            editorSelectEntry();
            break;

        case 'j':
        case ARROW_DOWN:
            if (E.cy < E.numrows - 1) E.cy++;
            break;

        case 'k':
        case ARROW_UP:
            if (E.cy > 4) E.cy--;
            break;

        case 'g':
            if (E.pending_key == 'g') {
                E.cy = 4;
                E.pending_key = 0;
            } else {
                E.pending_key = 'g';
            }
            break;

        case 'G':
            E.cy = E.numrows - 1;
            break;

        case 'a':
            explorerCreateFile();
            break;

        case 'A':
            explorerCreateFolder();
            break;

        case 'r':
            explorerRename();
            break;

        case 'd':
            explorerDelete();
            break;

        case 'y':
            explorerCopy();
            break;

        case 'p':
            explorerPaste();
            break;

        case 'x':
            explorerCut();
            break;

        case '.':
            explorerToggleHidden();
            break;

        case 'R':
            explorerRefresh();
            break;

        case '/':
            {
                char *pattern = editorPrompt("Search: /%s");
                if (pattern) {
                    strncpy(E.explorer_search_pattern, pattern, sizeof(E.explorer_search_pattern) - 1);
                    free(pattern);
                    // Jump to first match from current position
                    int i;
                    for (i = E.cy; i < E.numrows; i++) {
                        if (strstr(E.row[i].chars, E.explorer_search_pattern)) {
                            E.cy = i;
                            break;
                        }
                    }
                }
            }
            break;

        case 'n':
            {
                if (E.explorer_search_pattern[0] == '\0') break;
                int i;
                for (i = E.cy + 1; i < E.numrows; i++) {
                    if (strstr(E.row[i].chars, E.explorer_search_pattern)) {
                        E.cy = i;
                        break;
                    }
                }
            }
            break;

        case 'N':
            {
                if (E.explorer_search_pattern[0] == '\0') break;
                int i;
                for (i = E.cy - 1; i >= 4; i--) {
                    if (strstr(E.row[i].chars, E.explorer_search_pattern)) {
                        E.cy = i;
                        break;
                    }
                }
            }
            break;

        default:
            E.pending_key = 0;
            break;
    }
}
