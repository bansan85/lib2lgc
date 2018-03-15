# lib2lgcgdb

This library helps you to manipulate gdb to extract backtrace of a crash and to compare a bunch of backtrace files.

## Export the backtrace to file

Use static function in `Gdb` class. `RunBtFull` is to extract backtrace from a single file. `RunBtFullRecursive` for folder.

For a single file, you will need the command line that generate a crash.
In the command line, the file name must be replaced by `@@`.

For a folder, you will need to say if you allow multiple parallel instance of `gdb` and eventually, a regex (javascript compliant) to filter filename.

## Sort backtrace files

Create a `SetStack` class with parameters to sort backtraces.

Add backtrace files with `Add` and `AddRecursive` functions.
You are allow to keep only unique backtrace (based on sort parameters). In this case, it reduces a lot memory usage.

Finally, show the result in terminal by calling `Print` function.
