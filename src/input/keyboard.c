#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "input.h"
#include "utils.h"
#include "editor.h"

static int readOneChar(char *c) {
    int nread;
    while ((nread = read(STDIN_FILENO, c, 1)) != 1) {
        if (nread == -1) die("read");
        if (nread == 0) return 0; 
    }
    return 1;
}

int editorReadKey(void) {
    char c;
    while (readOneChar(&c) == 0);
    
    if (c == '\x1b') {
        char seq[3];
        if (readOneChar(&seq[0]) == 0) return '\x1b';
        if (readOneChar(&seq[1]) == 0) return '\x1b';
        
        if (seq[0] == '[') {
            if (seq[1] == '<') {
                int button, x, y;
                char m = '\0';
                char buf[64];
                int i = 0;
                while (i < 63) {
                    if (readOneChar(&buf[i]) == 0) break;
                    if (buf[i] == 'M' || buf[i] == 'm') {
                        m = buf[i];
                        buf[i] = '\0';
                        break;
                    }
                    i++;
                }
                if (sscanf(buf, "%d;%d;%d", &button, &x, &y) == 3) {
                    E.mouseX = x - 1;
                    E.mouseY = y - 1;
                    E.mouseButton = button;
                    if (m == 'M') return MOUSE_EVENT;
                }
                return '\x1b';
            }
            if (seq[1] >= '0' && seq[1] <= '9') {
                if (readOneChar(&seq[2]) == 0) return '\x1b';
                if (seq[2] == '~') {
                    switch (seq[1]) {
                        case '1': return HOME_KEY;
                        case '3': return DEL_KEY;
                        case '4': return END_KEY;
                        case '5': return PAGE_UP;
                        case '6': return PAGE_DOWN;
                        case '7': return HOME_KEY;
                        case '8': return END_KEY;
                    }
                }
            } else {
                switch (seq[1]) {
                    case 'A': return ARROW_UP;
                    case 'B': return ARROW_DOWN;
                    case 'C': return ARROW_RIGHT;
                    case 'D': return ARROW_LEFT;
                    case 'H': return HOME_KEY;
                    case 'F': return END_KEY;
                }
            }
        } else if (seq[0] == 'O') {
            switch (seq[1]) {
                case 'H': return HOME_KEY;
                case 'F': return END_KEY;
            }
        }
        return '\x1b';
    } else {
        return c;
    }
}
