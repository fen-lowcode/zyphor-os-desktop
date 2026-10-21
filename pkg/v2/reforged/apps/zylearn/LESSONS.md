# Writing lessons for zylearn

Lessons are plain text files in `lessons/`. No C++ needed.

## Files

    lessons/06-users.txt      the lesson (first line = "# Title")
    lessons/06-users.check    optional: automatic checks for `zylearn check 06-users`

Lesson names may only use `a-z`, `0-9`, `-` and `_`. Lessons are sorted by file name, so keep the number prefix.

## Check files

One check per line, fields separated by `|`. Lines starting with `#` are comments.

    description | type | arguments... | hint: help text shown on failure

Everything after `hint:` is the hint and may contain `|`.
A leading `~/` in a path means the student's home directory.

| type | arguments | passes when |
|---|---|---|
| `exists` | path | the file or folder exists |
| `dir` | path | the folder exists |
| `nonempty` | path | the file exists and is not empty |
| `mode` | path, octal | permissions match exactly (e.g. `600`) |
| `executable` | path | the owner can execute the file |
| `contains` | path, text | the file contains the text |
| `lines-at-least` | path, number | the file has at least that many lines |
| `symlink` | path, target | the path is a symlink to that target |
| `command` | name | the program is found in PATH |

Checks only read the filesystem; they never run commands. Keep every
exercise inside `~/zylearn-practice/` so students cannot damage anything.

## Testing a lesson without installing

    make
    ZYLEARN_LESSONS_DIR=$PWD/lessons ./build/zylearn lesson list
    ZYLEARN_LESSONS_DIR=$PWD/lessons ./build/zylearn check 06-users

## Releasing

    make deb      # builds build/zylearn_<version>-1_<arch>.deb
