#!/bin/bash

# Lista de directorios de módulos que contienen scripts de pruebas
MODULE_DIRS=("modules/template")

# Itera sobre cada directorio de módulo y ejecuta su script de pruebas
for module_dir in "${MODULE_DIRS[@]}"; do
    if [ -f "${module_dir}/test/run_tests.sh" ]; then
        echo "Ejecutando pruebas para el módulo: ${module_dir}..."
        (./$module_dir/test/run_tests.sh)
    else
        echo "No se encontró el script de pruebas para el módulo: ${module_dir}"
    fi
done
