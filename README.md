# Vedit - A Vim-like C Terminal Text Editor

Vedit is a lightweight, terminal-based text editor written entirely in C. It implements modal editing with a UI inspired by Vim.

## Features

- **Modal Editing**:
  - `NORMAL` mode: Navigation and command entry.
  - `INSERT` mode: Text entry and editing.
  - `COMMAND` mode: File saving and quitting.
- **Vim-like Keybindings**: Use `h`, `j`, `k`, `l` to move, `i` to enter insert mode, and `:` to enter command mode.
- **Commands**:
  - `:w` - Save to the current file.
  - `:w <filename>` - Save as a new file.
  - `:q` - Quit the editor.
  - `:wq` - Save and quit.
  - `:q!` - Force quit without saving.
- **No external UI libraries**: Everything is drawn using VT100 escape sequences.
- **Portability**: Uses standard C99 and POSIX terminal standards.

## Development Environment Setup

This project provides a reproducible development environment via Nix.

1. Enable flakes in your Nix installation.
2. Run `nix develop` in the project root to launch the development shell. This environment includes `gcc`, `gdb`, `make`, and `pkg-config`.

## Build and Run

Inside the shell, run the following to compile the project:
```bash
make
```

To run the editor:
```bash
./bin/vedit [optional_filename]
```

# TOOD:
