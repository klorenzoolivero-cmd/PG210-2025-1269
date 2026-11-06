#!/bin/sh
# Uso (desde la raiz del repo): sh pruebas/ejecutar_pruebas.sh
# Compila, ejecuta cada .in, guarda la salida en pruebas/obtenidos/ y compara con .out
gcc -std=c11 -Wall -Wextra -Wpedantic main.c -o "${RETO_BIN:-./reto}" || exit 1
mkdir -p pruebas/obtenidos
ok=0; total=0
for entrada in pruebas/publicas/*.in pruebas/propias/*.in; do
    nombre=$(basename "$entrada" .in)
    esperado="${entrada%.in}.out"
    obtenido="pruebas/obtenidos/$nombre.out"
    "${RETO_BIN:-./reto}" < "$entrada" > "$obtenido"
    total=$((total + 1))
    if [ "$(tr -s ' \n' '  ' < "$esperado")" = "$(tr -s ' \n' '  ' < "$obtenido")" ]; then
        ok=$((ok + 1)); echo "OK     $nombre"
    else
        echo "FALLO  $nombre"
    fi
done
echo "Correctos: $ok / $total"
[ "$ok" -eq "$total" ]
