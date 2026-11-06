# PG210-2025-1269 — Combos alternantes en un videojuego

- **Estudiante:** Kelvin Lorenzo Olivero
- **Matrícula:** 2025-1269
- **Correo institucional:** 20251269@itla.edu.do
- **Asignatura:** Programación para Mecatrónicos · Challenge individual (25 puntos)

## Contenido
| Archivo / carpeta | Descripción |
|---|---|
| `main.c`, `reto.h` | Solución en C11 (guardas de inclusión, prototipos, constantes) |
| `plan.md` | Análisis, pseudocódigo y reglas de validación |
| `evaluar.py`, `publicas.json` | Ejecutor del banco público (seis casos públicos) |
| `pruebas/publicas/` | `g1..g6` (`.in` / `.out`) |
| `pruebas/propias/` | 5 casos válidos nuevos (`p01..p06`) y 14 inválidos (`i01..i14`) |
| `pruebas/obtenidos/` | Salidas reales del programa para cada caso |
| `pruebas/ejecutar_pruebas.sh`, `pruebas/propias.json` | Ejecución automática de todo y evidencia (`resultado_ejecucion.txt`) |

## Modelo y decisiones principales
- Clase de cada muestra: `tipo=0` paridad (`x % 2`), `tipo=1` umbral (`x >= p`).
- `calcular` recorre el vector una sola vez con una **racha** de alternancias consecutivas:
  la racha en la posición `i` es el número de segmentos alternantes de longitud >= 2 que terminan en `i`,
  así que `score` es la suma de rachas. `estado = 1` cuando alguna racha llega a 2 (segmento de 3 elementos).
  Una racha maximal de longitud L aporta L(L-1)/2 segmentos.
- `racha`, `score` y `estado` son locales de `calcular`: nada se arrastra entre unidades.
- `estado` se devuelve **por referencia** (`int *estado`); el `score` es el valor de retorno.
- `total_rec` suma recursivamente con caso base `n = 0`.
- `ordenar` usa **inserción** sobre una copia (por score descendente y, en empate, id ascendente);
  con el criterio `ORDEN_POR_ID` ordena otra copia para la **búsqueda binaria** (`buscar`).
  El arreglo original no se altera, por lo que las líneas de resultados conservan el orden de entrada.
  No se usa `qsort` ni `bsearch`.
- Toda la entrada se lee y valida **antes** de imprimir; ante cualquier error solo se imprime `ERROR` y el programa termina con código 0.
- La conversión a mayúsculas es manual (ASCII), independiente del idioma del sistema. El nombre no interviene en la regla del reto; `calcular` lo recibe por requisito de firma.
- Sin memoria dinámica, sin `gets`, `system` ni `conio.h`. Capacidades definidas con `#define`.

## Compilación y ejecución
```
gcc -std=c11 -Wall -Wextra -Wpedantic main.c -o reto      # compila sin advertencias
./reto < caso.in                                          # Linux / macOS
reto.exe < caso.in                                        # Windows (cmd)
python3 evaluar.py publicas.json ./reto                   # banco público (Windows: py evaluar.py publicas.json reto.exe)
sh pruebas/ejecutar_pruebas.sh                            # públicas + propias, guarda pruebas/obtenidos/
```

## Resultados
Banco público (`python3 evaluar.py publicas.json ./reto`):

| Grupo | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|
| Resultado | OK | OK | OK | OK | OK | OK |

`Casos correctos: 6 / 6`. Pruebas propias: `Casos correctos: 20 / 20`
(ver `pruebas/resultado_ejecucion.txt`: 26/26 entre públicas y propias).
También se ejecutaron todos los casos con `-fsanitize=address,undefined` sin reportes.

### Pruebas propias (esperado vs obtenido: coinciden en todas)
| Caso | Qué comprueba | Esperado = Obtenido |
|---|---|---|
| p01_normal | 3 unidades, ambos tipos, empate de score (15) y FIND presente | Sí |
| p02_borde_id_max | n=1, m=1, id=9999, x=p=100 | Sí |
| p03_borde_m3_p0 | m=3, p=0 (todo clase 1), score 3/1/0, desempate por id | Sí |
| p04_q_ausente | `FIND 77 -1` | Sí |
| p05_reinicio | unidad alternante seguida de unidad sin alternancia (sin arrastre) | Sí |
| p06_capacidad_20x8 | n=20, m=8; esperado calculado con enumeración de segmentos independiente | Sí |
| i01 – i14 | x negativo, id repetido, entrada incompleta, tipo=2, p=101, n=0, n=21, m=9, q=0, id=0, nombre de 16 caracteres, nombre con guion medio, entrada vacía, error en el último registro (sin salida parcial) | Solo `ERROR` |

Los esperados de p01–p05 se obtuvieron a mano (traza abajo) y se verificaron contra el programa.

### Traza (grupo 1, `Libro9`, tipo 1, p = 20, muestras 20 20 25 10 80 5)
Clases (x >= 20): `1 1 1 0 1 0`

| i | pareja | ¿distinta? | racha | score acumulado |
|---|---|---|---|---|
| 1 | 1,1 | no | 0 | 0 |
| 2 | 1,1 | no | 0 | 0 |
| 3 | 1,0 | sí | 1 | 1 |
| 4 | 0,1 | sí | 2 | 3 |
| 5 | 1,0 | sí | 3 | 6 |

Resultado: score 6; como la racha llegó a 2, estado 1. Coincide con `LIBRO9 6 1`.

### Media de scores (solo documentación; no se imprime en stdout)
Expresión C: `double media = total / (double)n;` (se muestra con `%.2f`).
Con `gcc -std=c11 -DMOSTRAR_MEDIA main.c -o reto_media` se escribe por **stderr** únicamente para evidencia.

| Caso | total | n | media |
|---|---|---|---|
| g1 | 6 | 2 | 3.00 |
| g2 | 0 | 1 | 0.00 |
| g3 | 0 | 3 | 0.00 |
| g4 | 22 | 2 | 11.00 |
| g5 | 6 | 2 | 3.00 |
| p01_normal | 37 | 3 | 12.33 |
| p02_borde_id_max | 0 | 1 | 0.00 |
| p03_borde_m3_p0 | 4 | 4 | 1.00 |
| p04_q_ausente | 6 | 2 | 3.00 |
| p05_reinicio | 20 | 3 | 6.67 |
| p06_capacidad_20x8 | 116 | 20 | 5.80 |

### Revisión y correcciones
Registre aquí los errores reales que encontró durante su desarrollo y cómo los corrigió
(síntoma, causa, cambio realizado y prueba que lo confirma).

## Fuentes consultadas
- Corona Nakamura, M. A. y Ancona Valdez, M. de los Á. (2011). *Diseño de algoritmos y su codificación en lenguaje C*. McGraw-Hill (cap. 5 recursividad; cap. 6 estructuras anidadas; cap. 7 ordenamiento, búsqueda binaria y apuntadores).
- Documentación de la biblioteca estándar de C11 (`scanf`, `strlen`, `strcpy`, `memcpy`, `printf`).

## Historial de commits (anteriores al final)
| # | Hash completo | Avance |
|---|---|---|
| 1 | _(completar)_ | Estructura, interpretación del problema y plan |
| 2 | _(completar)_ | Desarrollo parcial verificable |
| 3 | _(completar)_ | Pruebas, validación y revisión |
| 4 | _(completar)_ | Correcciones finales y documentación |

El hash del commit final se entrega aparte, junto con el enlace del repositorio, para evitar la referencia circular.
