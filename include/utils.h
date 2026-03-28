#ifndef UTILS_H
#define UTILS_H

struct abuf {
    char *b;
    int len;
};

#define ABUF_INIT {NULL, 0}

void abAppend(struct abuf *ab, const char *s, int len);
void abFree(struct abuf *ab);

void die(const char *s);
void editorOpen(char *filename);
void editorSave(void);
void editorSelectEntry(void);
char *editorRowsToString(int *buflen);

void explorerCreateFile(void);
void explorerCreateFolder(void);
void explorerRename(void);
void explorerDelete(void);
void explorerCopy(void);
void explorerCut(void);
void explorerPaste(void);
void explorerRefresh(void);

void editorOpenSearchResults(char *query);
void explorerToggleHidden(void);

#endif
