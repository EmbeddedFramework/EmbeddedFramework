#!/bin/bash

# Lista de directorios de módulos que contienen scripts de pruebas
MODULE_DIRS=("modules/template" "modules/efHal")

# Crear directorio para los informes de prueba
mkdir -p out/testReport

# Itera sobre cada directorio de módulo y ejecuta su script de pruebas
for module_dir in "${MODULE_DIRS[@]}"; do
    if [ -f "${module_dir}/run_tests.sh" ]; then
        echo "Ejecutando pruebas para el módulo: ${module_dir}..."
        (./$module_dir/run_tests.sh)
        
        # Copiar carpeta build/html a out/testReport/
        if [ -d "${module_dir}/uTest/build/html" ]; then
            cp -r "${module_dir}/uTest/build/html" out/testReport/$(basename $module_dir)
        else
            echo "No se encontró la carpeta build/html para el módulo: ${module_dir}"
        fi
        
    else
        echo "No se encontró el script de pruebas para el módulo: ${module_dir}"
    fi
done

./genTestReport.sh

