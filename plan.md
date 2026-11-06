# plan.md — Análisis y algoritmo (PG210, Combos alternantes)

**Estudiante:** Kelvin Lorenzo Olivero · **Matrícula:** 2025-1269 · **Correo:** 20251269@itla.edu.do

## 1. Análisis del problema
- **Entrada:** `n m q`, luego `n` registros `id nombre tipo p x1..xm`.
- **Salida (entrada válida):** una línea `id NOMBRE score estado` por unidad (orden original), `TOTAL`, `ORDER`, `FIND`.
- **Salida (entrada inválida):** únicamente `ERROR`, sin resultados parciales.
- **Clase de una muestra:** `tipo=0` → `x % 2` (par/impar); `tipo=1` → `x >= p` (1) o `x < p` (0).
- **score:** cantidad de segmentos contiguos de longitud >= 2 donde cada pareja vecina tiene clases distintas.
- **estado:** 1 si existe un segmento alternante de longitud >= 3, si no 0.

## 2. Idea clave
Sea `racha[i]` la cantidad de alternancias consecutivas que terminan en la posición `i`.
Esa racha es exactamente el número de segmentos alternantes de longitud >= 2 que terminan en `i`
(un segmento por cada inicio posible dentro de la racha). Por tanto:

```
score  = suma de racha[i] para i = 1..m-1
estado = 1 si algún racha[i] >= 2   (3 elementos alternantes = 2 alternancias seguidas)
```
Equivalente: una racha maximal de longitud L aporta L(L-1)/2 segmentos.
Todo se reinicia en cada unidad (variables locales de `calcular`).

## 3. Pseudocódigo
```
calcular(v, m, tipo, p, nombre, *estado):
    score <- 0; racha <- 0; *estado <- 0
    para i desde 1 hasta m-1:
        si clase(v[i]) != clase(v[i-1]) entonces racha <- racha + 1
        sino racha <- 0
        score <- score + racha
        si racha >= 2 entonces *estado <- 1
    devolver score

total_rec(r, n):
    si n = 0 devolver 0
    devolver r[n-1].score + total_rec(r, n-1)

ordenar(r, n, criterio):          // inserción
    para i desde 1 hasta n-1:
        clave <- r[i]; j <- i-1
        mientras j >= 0 y antes(clave, r[j], criterio): r[j+1] <- r[j]; j <- j-1
        r[j+1] <- clave
    // antes: por id asc, o por score desc y en empate id asc

buscar(r, n, q):                  // r ordenado por id asc
    ini <- 0; fin <- n-1
    mientras ini <= fin:
        mitad <- ini + (fin-ini)/2
        si r[mitad].id = q devolver mitad
        si r[mitad].id < q entonces ini <- mitad+1 sino fin <- mitad-1
    devolver -1

main:
    si no leer_entrada(...) → imprimir ERROR y terminar
    para cada unidad: score, estado <- calcular(...)
    total <- total_rec(reg, n)
    imprimir líneas originales y TOTAL
    copia <- reg; ordenar(copia, POR_SCORE); imprimir ORDER
    copia <- reg; ordenar(copia, POR_ID); pos <- buscar(copia, n, q); imprimir FIND
```

## 4. Validación (antes de imprimir cualquier cosa)
`n` en 1..20, `m` en 1..8, `q` en 1..9999, `id` en 1..9999, nombre de 1 a 15 caracteres
`[A-Za-z0-9_]`, `tipo` en {0,1}, `p` en 0..100, `x` en 0..100, id únicos, sin datos faltantes.
Cualquier falla → `ERROR`.

## 5. Estructuras
`int muestras[20][8]`; `Registro { id, nombre[16], Config {tipo, p}, score, estado }`.
