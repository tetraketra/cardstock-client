#!/bin/bash

# Fix weird bugs.
sudo sysctl vm.mmap_rnd_bits=28 > /dev/null 2>&1
sudo sysctl vm.mmap_rnd_compat_bits=8 > /dev/null 2>&1

# GCC args.
FILES=$(find . -print | grep -i "\.c" | tr -s '\n' ' ')
WARNS="-W -Wall -Wextra -Wno-multichar -Wno-comment -Wno-misleading-indentation -Wno-uninitialized"
FSANS="-fsanitize=address -fsanitize=undefined -fsanitize-address-use-after-scope" # FOR RELEASE, CLEAR THIS AND REMOVE THE DLCLOSE EXPORT
LINKS="-lGL -lm -lc -lSDL3"
FLAGS="-Og -g3 -D BUILDFLAG_DEBUG -D BUILDFLAG_LINUX" # BUILDFLAG_WINDOWS, BUILDFLAG_MAC
INCLD="-iquote ./src/modules"

echo "\n\nExecuting with..."

echo "FILES: $FILES"
echo "WARNS: $WARNS"
echo "FSANS: $FSAN"
echo "LINKS: $LINKS"
echo "FLAGS: $FLAGS"
echo "INCLD: $INCLD"

# Line counts.
echo "\n\nReporting lines..."

if command -v scc &> /dev/null; then
    scc \
        --not-match="(.gitignore|.gitattributes|dummy.txt|LICENSE|ext/*|ext)" \
        --no-large --large-line-count 3000
else
    find ./ -type f \( -iname \*.c -o -iname \*.h \) \
        | xargs wc -l \
        | sort -nr
fi

# Build and build time(s).
echo "\nBuilding and reporting build time(s)..."

export ASAN_OPTIONS=detect_leaks=1:halt_on_error=0:quarantine_size_mb=128

BUILD_START=$(date +%s.%N)
mkdir -p ./bin
gcc-12 $FILES -o ./bin/cardstockclient \
    $WARNS $LINKS $FLAGS $FSANS $INCLD \
    -ftime-report \
    > time.txt 2>&1
BUILD_END=$(date +%s.%N)

cat time.txt \
    | grep -E --color=never '^(Time variable| [[:alnum:]])' \
    | cut -c1-36,69-79 \
    | sed 's/   wall/ /' \
    | awk -v list="$FILES" 'BEGIN { file_index=1 }
        {
            if (gsub("Time variable", (file_index <= split(list, arr) ? arr[file_index] : "File"))) {
                file_index = (file_index < length(arr) ? file_index + 1 : 1)
            }
            print 
        }'

DIFF=$(echo "$BUILD_END $BUILD_START - p" | dc | xargs printf "%.2f")
echo "./everything"
echo " TOTAL                              $DIFF (100%)"

# Warnings and errors. 
echo "\n\nReporting build warnings and errors..."

cat time.txt \
    | grep -v -E '^(Time variable| [[:alnum:]])' \
    | grep -v '^$' \
    > errors.txt
rm time.txt

[ -s errors.txt ] && cat errors.txt || echo "Nothing! d^.^b" 
rm errors.txt

# Make executable, well, executable.
find ./bin -type f -print -quit | xargs chmod a+x

echo "\n\nExecute \"sh debug_run.sh\" to start the program."