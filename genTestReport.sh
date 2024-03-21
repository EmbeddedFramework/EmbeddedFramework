#!/bin/bash

# Nombre del archivo de salida
INDEX_FILE="out/testReport/index.html"

# Encabezado del archivo HTML
cat <<EOF > "$INDEX_FILE"
<!DOCTYPE html>
<html>
<head>
    <title>Informes de Cobertura de Módulos</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 20px;
        }
        h1 {
            color: #333;
        }
        ul {
            list-style-type: none;
            padding: 0;
        }
        li {
            margin-bottom: 10px;
        }
        a {
            text-decoration: none;
            color: #007bff;
        }
        a:hover {
            text-decoration: underline;
        }
    </style>
</head>
<body>
    <h1>Informes de Cobertura de Módulos</h1>
    <ul>
EOF

# Iterar sobre los módulos y generar enlaces HTML
for module_dir in out/testReport/*/; do
    module_name=$(basename "$module_dir")
    coverage_report="${module_dir}coverage_report.html"
    if [ -f "$coverage_report" ]; then
        echo "    <li><a href=\"${module_name}/coverage_report.html\">$module_name</a></li>" >> "$INDEX_FILE"
    else
        echo "    <li>No se encontró el informe de cobertura para el módulo: $module_name</li>" >> "$INDEX_FILE"
    fi
done

# Cerrar el archivo HTML
echo "  </ul>" >> "$INDEX_FILE"
echo "</body>" >> "$INDEX_FILE"
echo "</html>" >> "$INDEX_FILE"
