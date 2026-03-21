#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#include "editor.h"

void editorMoveCursor(int key) {
  erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];

  switch (key) {
    case ARROW_LEFT:
      if (E.cx != 0) {
        E.cx--;
      } else if (E.cy > 0) {
        E.cy--;
        E.cx = E.row[E.cy].size;
      }
      break;
    case ARROW_RIGHT:
      if (row && E.cx < row->size) {
        E.cx++;
      } else if (row && E.cx == row->size) {
        E.cy++;
        E.cx = 0;
      }
      break;
    case ARROW_UP:
      if (E.cy != 0) E.cy--;
      break;
    case ARROW_DOWN:
      if (E.cy < E.numrows) E.cy++;
      break;
  }

  row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
  int rowlen = row ? row->size : 0;
  if (E.cx > rowlen) {
    E.cx = rowlen;
  }
}

void editorProcessKeypress(void) {
  static int quit_times = 3;

  int c = editorReadKey();

  switch (c) {
    case CTRL_KEY('q'):
      if (E.dirty && quit_times > 0) {
        editorSetStatusMessage("WARNING!!! File has unsaved changes. Press Ctrl-Q %d more times to quit.", quit_times);
        quit_times--;
        return;
      }
      if (write(STDOUT_FILENO, "\x1b[2J", 4) == -1) {
        die("write");
      }
      if (write(STDOUT_FILENO, "\x1b[H", 3) == -1) {
        die("write");
      }
      exit(0);
      break;
    case CTRL_KEY('s'):
      editorSave();
      break;

    case HOME_KEY:
      E.cx = 0;
      break;
    case END_KEY:
      if (E.cy < E.numrows) {
        E.cx = E.row[E.cy].size;
      }
      break;

    case CTRL_KEY('h'):
    case 127:
    case DEL_KEY:
      if (c == DEL_KEY) {
        editorMoveCursor(ARROW_RIGHT);
      }
      editorDelChar();
      break;

    case PAGE_UP:
    case PAGE_DOWN:
      if (c == PAGE_UP) {
        E.cy = E.rowoff;
      } else if (c == PAGE_DOWN) {
        E.cy = E.rowoff + E.screenrows - 1;
        if (E.cy > E.numrows) E.cy = E.numrows;
      }

      for (int times = E.screenrows; times > 0; times--) {
        editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
      }
      break;

    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
      editorMoveCursor(c);
      break;

    case '\r':
      editorInsertNewline();
      break;

    case CTRL_KEY('l'):
    case '\x1b':
      break;

    default:
      if (isprint(c)) {
        editorInsertChar(c);
      }
      break;
  }

  quit_times = 3;
}
