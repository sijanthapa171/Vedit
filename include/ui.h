#ifndef UI_H
#define UI_H

#include "utils.h"

void enableRawMode(void);
void disableRawMode(void);
int getWindowSize(int *rows, int *cols);
void editorRefreshScreen(void);
void editorDrawStatusBar(struct abuf *ab);
void editorDrawMessageBar(struct abuf *ab);
void editorScroll(void);

#endif
