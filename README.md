# Shpenzime GUI

A desktop expense-management application written in C with a raylib/raygui
interface. The app lets an administrator manage users and categories, while
regular users can record expenses, record income, and view a simple financial
summary.

The project keeps the original file-based business logic and adds a graphical
interface on top of it.

## Features

- Administrator login with fixed credentials from `struktura.h`
- User login from `user.txt`
- Add and delete users
- View and manage expense categories
- Record user expenses with category, amount, date, and description
- Record user income with source, amount, and date
- View expense and income lists per user
- View totals and balance report
- Local text-file storage, no database required

## Tech Stack

- C99
- raylib
- raygui
- Makefile build system
- Text files for persistence

## Project Structure

```text
.
|-- main.c                 # raylib window setup and screen routing
|-- gui_state.h            # AppState, cache, form state, helpers
|-- gui_panels.h           # GUI screens and tab panels
|-- funksione.c            # file I/O, validation, and original app logic
|-- struktura.h            # structs, constants, and function declarations
|-- raylib_mingw_compat.c  # Windows/MinGW compatibility helpers
|-- Makefile               # cross-platform build commands
|-- raylib.h               # bundled raylib header
|-- raygui.h               # bundled raygui header
|-- libraylib.a            # bundled Windows/MinGW raylib static library
|-- Figtree-Regular.ttf    # UI font
|-- user.txt               # user data
`-- shpenzime.txt          # expense data
```

The app may also create or read these data files at runtime:

```text
teArdhura.txt
kategorite.txt
```

## Requirements

For Windows with MinGW, this repository already includes the raylib header,
raygui header, and `libraylib.a`.

For Linux or macOS, install raylib or provide a compatible raylib library in
your compiler/library path.

Examples:

```bash
# Ubuntu/Debian
sudo apt install build-essential libraylib-dev

# macOS with Homebrew
brew install raylib
```

## Build

From the project directory:

```bash
make
```

The output binary is:

```text
shpenzime.exe   # Windows
shpenzime       # Linux/macOS
```

Manual Windows/MinGW build:

```bash
gcc -std=c99 -Wall -Wextra -Wno-unused-parameter -I. \
    main.c funksione.c raylib_mingw_compat.c \
    -o shpenzime.exe -L. -lraylib -lgdi32 -lwinmm -mwindows
```

Manual Linux build:

```bash
gcc -std=c99 -Wall -Wextra -Wno-unused-parameter -I. \
    main.c funksione.c raylib_mingw_compat.c \
    -o shpenzime -lraylib -lm -lpthread -ldl -lGL
```

Manual macOS build:

```bash
gcc -std=c99 -Wall -Wextra -Wno-unused-parameter -I. \
    main.c funksione.c raylib_mingw_compat.c \
    -o shpenzime -lraylib -lm \
    -framework OpenGL -framework Cocoa -framework IOKit
```

## Run

```bash
./shpenzime
```

On Windows:

```bash
./shpenzime.exe
```

Run the app from the project directory so it can find the local data files.

## Default Admin Login

```text
Username: admin
Password: Admin1!
```

These values are defined in `struktura.h`.

## Data Format

The project stores data in plain text files using `|` as a separator.

```text
user.txt:
id_user|emri|username|password|buxheti_mujor

shpenzime.txt:
id_shpenzim|id_user|pershkrim|id_kategori|kategori|pershkrimi_kategorise|shuma|data

teArdhura.txt:
id_hyrje|id_user|burimi|shuma|data

kategorite.txt:
id_kategori|emertimi|pershkrimi
```

Dates are expected in `DD/MM/YYYY` format.

## Notes for Publishing on Git

Recommended files to commit:

- Source files: `*.c`, `*.h`
- `Makefile`
- `README.md`
- Font/assets used by the app
- Small sample data files, if you want the app to open with demo data

Recommended files to ignore:

- Build outputs such as `shpenzime.exe` or `shpenzime`
- Temporary editor files
- Private real user data, if the `.txt` files contain sensitive information

Example `.gitignore`:

```gitignore
shpenzime
shpenzime.exe
*.o
*.obj
.vscode/
```

## License

Add a license before publishing if this project will be shared publicly.
MIT is a common choice for small open-source projects.
