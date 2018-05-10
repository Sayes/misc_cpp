ls *.cc | xargs dos2unix
ls *.c | xargs clang-format -style="{BasedOnStyle: google, ColumnLimit: 80}" -i
ls *.cc | xargs clang-format -style="{BasedOnStyle: google, ColumnLimit: 80}" -i
