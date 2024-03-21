#!/bin/bash

# prepara nombres de directorios
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
HTML_DIR="uTest/build/html"
SUPPORT_DIR="uTest/build/support"

# se para sobre el directorio test
cd "$SCRIPT_DIR" || exit

if [ ! -d "$HTML_DIR" ]; then
    mkdir -p "$HTML_DIR"
fi

if [ ! -d "$SUPPORT_DIR" ]; then
    mkdir -p "$SUPPORT_DIR"
fi

ceedling test:all
ceedling gcov:all
gcovr -r . --html --html-details -o $HTML_DIR/coverage_report.html --exclude '.*test_.*'
