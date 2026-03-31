#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "editor.h"
#include "input.h"
#include "commands.h"
#include "core.h"
#include "utils.h"

void editorMoveCursor(int key) {
    erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
    
    switch (key) {
        case ARROW_LEFT:
        case 'h':
            if (E.cx > 0) {
                E.cx--;
            } else if (E.cy > 0) {
                E.cy--;
                E.cx = E.row[E.cy].size;
            }
            break;
        case ARROW_RIGHT:
        case 'l':
            if (row && E.cx < row->size) {
                E.cx++;
            } else if (row && E.cx == row->size) {
                E.cy++;
                E.cx = 0;
            }
            break;
        case ARROW_UP:
        case 'k':
            if (E.cy > 0) E.cy--;
            break;
        case ARROW_DOWN:
        case 'j':
            if (E.cy < E.numrows) E.cy++;
            break;
    }
    
    row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
    int rowlen = row ? row->size : 0;
    if (E.cx > rowlen) {
        E.cx = rowlen;
    }
}

void normalModeProcessKey(int c) {
    switch (c) {
        case 'i':
            if (E.mode == MODE_EXPLORER) break;
            E.command_count = 0;
            editorSaveUndoState();
            E.mode = MODE_INSERT;
            editorSetStatusMessage("-- INSERT --");
            break;
            
        case 'u':
            E.command_count = 0;
            editorUndo();
            break;
            
        case ':':
            E.command_count = 0;
            E.mode = MODE_COMMAND;
            commandModeProcess();
            break;
            
        case 'v':
            E.command_count = 0;
            E.mode = MODE_VISUAL;
            E.sel_sx = E.cx;
            E.sel_sy = E.cy;
            editorSetStatusMessage("-- VISUAL --");
            break;
            
        case '\r':
            E.command_count = 0;
            break;
            
        case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            E.command_count = E.command_count * 10 + (c - '0');
            break;
            
        case 'h':
        case 'j':
        case 'k':
        case 'l':
        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_LEFT:
        case ARROW_RIGHT:
            {
                int times = E.command_count ? E.command_count : 1;
                while (times--) {
                    editorMoveCursor(c);
                }
                E.command_count = 0;
            }
            break;
            
        case PAGE_UP:
        case PAGE_DOWN:
            {
                E.command_count = 0;
                int times = E.screenrows;
                while (times--)
                    editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
            }
            break;
            
        case HOME_KEY:
        case '0':
            if (c == '0' && E.command_count > 0) {
                E.command_count = E.command_count * 10;
            } else {
                E.cx = 0;
                E.command_count = 0;
            }
            break;
            
        case END_KEY:
        case '$':
            E.command_count = 0;
            if (E.cy < E.numrows)
                E.cx = E.row[E.cy].size;
            break;
            
        case 'x':
            E.command_count = 0;
            if (E.cy < E.numrows && E.row[E.cy].size > 0) {
                editorSaveUndoState();
                editorRowDelChar(&E.row[E.cy], E.cx);
            }
            break;
            
        case 'd':
            E.command_count = 0;
            if (E.pending_key == 'd') {
                E.pending_key = 0;
                if (E.numrows == 0) break;
                editorDelRow(E.cy);
                if (E.cy >= E.numrows && E.cy > 0) E.cy--;
                E.cx = 0;
                editorSetStatusMessage("");
            } else {
                E.pending_key = 'd';
                editorSetStatusMessage("d");
            }
            break;

        case 'g':
            if (E.pending_key == 'g') {
                E.cy = 0;
                E.cx = 0;
                E.pending_key = 0;
                editorSetStatusMessage("");
            } else {
                E.pending_key = 'g';
                editorSetStatusMessage("g");
            }
            break;

        case 'G':
            E.command_count = 0;
            if (E.numrows > 0) E.cy = E.numrows - 1;
            else E.cy = 0;
            E.cx = 0;
            break;

        case '/':
            {
                char *pattern = editorPrompt("Search: /%s (Append [!] for global)");
                if (pattern) {
                    int global = 0;
                    size_t plen = strlen(pattern);
                    if (plen > 0 && pattern[plen-1] == '!') {
                        pattern[plen-1] = '\0';
                        global = 1;
                    }

                    strncpy(E.search_pattern, pattern, sizeof(E.search_pattern) - 1);
                    free(pattern);
                    
                    if (global) {
                        editorOpenSearchResults(E.search_pattern);
                        return;
                    }

                    int found = 0;
                    for (int i = 0; i < E.numrows; i++) {
                        int filerow = (E.cy + i) % E.numrows;
                        char *match = strstr(E.row[filerow].chars, E.search_pattern);
                        if (match) {
                            E.cy = filerow;
                            E.cx = match - E.row[filerow].chars;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) editorSetStatusMessage("Pattern not found: %s", E.search_pattern);
                }
            }
            break;

        case 'n':
            if (E.search_pattern[0] != '\0') {
                int found = 0;
                for (int i = 1; i <= E.numrows; i++) {
                    int filerow = (E.cy + i) % E.numrows;
                    char *match = strstr(E.row[filerow].chars, E.search_pattern);
                    if (match) {
                        E.cy = filerow;
                        E.cx = match - E.row[filerow].chars;
                        found = 1;
                        break;
                    }
                }
                if (!found) editorSetStatusMessage("Pattern not found: %s", E.search_pattern);
            }
            break;

        case 'N':
            if (E.search_pattern[0] != '\0') {
                int found = 0;
                for (int i = 1; i <= E.numrows; i++) {
                    int filerow = (E.cy - i + E.numrows) % E.numrows;
                    char *match = strstr(E.row[filerow].chars, E.search_pattern);
                    if (match) {
                        E.cy = filerow;
                        E.cx = match - E.row[filerow].chars;
                        found = 1;
                        break;
                    }
                }
                if (!found) editorSetStatusMessage("Pattern not found: %s", E.search_pattern);
            }
            break;

        case CTRL_KEY('q'):
            E.command_count = 0;
            E.pending_key = 0;
            if (E.dirty && E.quit_times > 0) {
                editorSetStatusMessage("WARNING!!! File has unsaved changes. "
                                       "Press Ctrl-Q %d more times to quit.", E.quit_times);
                E.quit_times--;
                return;
            }
            if (write(STDOUT_FILENO, "\x1b[2J", 4) == -1) {}
            if (write(STDOUT_FILENO, "\x1b[H", 3) == -1) {}
            exit(0);
            break;
            
        case MOUSE_EVENT:
            E.command_count = 0;
            E.pending_key = 0;
            editorHandleMouseClick();
            break;

        case MOUSE_WHEEL_UP:
        case MOUSE_WHEEL_DOWN:
            E.command_count = 0;
            E.pending_key = 0;
            editorHandleMouseScroll(c);
            break;

        default:
            E.command_count = 0;
            E.pending_key = 0;
            break;
    }
}
