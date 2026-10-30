# Revisión e implementación del código de tres direcciones

Revisión realizada el 30 de octubre de 2026. Se ejecutó `git fetch origin`
(solo descarga de referencias; sin push). Los cambios de trabajo están únicamente
en `codigo-tres-direcciones-paulo`, que parte de `0dd2baa`.

## Funcionalidades encontradas antes de programar

Se revisaron los árboles e historial de todas las ramas locales y remotas.
Las versiones iguales se compararon con Git para evitar revisar como distintos
archivos que contienen exactamente el mismo código.

| Rama | Revisión | Trabajo existente y problemas encontrados |
| --- | --- | --- |
| `main`, `origin/main`, `origin/VARIABLES-Soch` | `5a318ed` | Declaraciones `a:b ALLOCATE;`, expresiones, potencia y seis comparaciones con AND/OR. No hay tabla de símbolos ni acciones para generar código. El lexer no incluye la cabecera de tokens, devuelve `COMENTARIO` no declarado y usa `BEGIN`, que choca con Flex. Bison y Flex generan archivos, pero GCC no compila el conjunto. |
| `origin/Potencia-Javier` | `d862950` | Incluye cabecera de tokens y cambia BEGIN por `TK_BEGIN`. Agrega `MAIN ... BEGIN ... END`, asignaciones, INPUT/OUTPUT y organización de sentencias. La potencia ya existía y se conserva. Compila, pero devuelve éxito incluso al detectar un error de sintaxis. |
| `origin/Interfaz-maria` | `3128c2f` | Agrega FOR, DO-WHILE y una aplicación Next.js. El parser raíz vuelve a aceptar instrucciones aisladas: rechaza un programa con MAIN. Tiene cinco conflictos shift/reduce en la lista de sentencias y devuelve éxito tras errores. La expresión regular de ID exige al menos dos caracteres. Frontend aún es la plantilla inicial; backend contiene solo `.gitkeep`. |
| `IF-ELSE-Paulo`, `origin/IF-ELSE-Paulo` | `b14be16` | Añade `IF_ELSE_Paulo/` con parser completo para MAIN, declaraciones, asignación, E/S, potencia, IF, ELSE y anidamiento; cuatro ejemplos. Compila y devuelve error cuando falla la sintaxis. AND y OR tienen la misma precedencia; solo reconoce sintaxis, sin generación. El parser raíz sigue siendo el de María. |
| `prueba`, `origin/prueba`, base de `codigo-tres-direcciones-paulo` | `0dd2baa` | Reúne la carpeta de Paulo y la versión raíz/interfaz de María. Agrega archivos generados y ejecutable al control de versiones; no integra ambos analizadores ni genera código intermedio. |
| `origin/HEAD` | `5a318ed` | Alias de `origin/main`. |

También se revisó `proyectoF.y`: es una gramática alternativa con mensajes de
reconocimiento para declaraciones y comparaciones, sin un compilador integrado.
El README original tenía incompleta la receta de compilación. `entrada.txt`
contiene FOR con `to` y `do` en minúsculas, aunque el lexer reconoce palabras
reservadas en mayúsculas. No hay servidor de compilación, análisis semántico ni
conexión entre la web y el compilador. Los archivos generados de la raíz no son
la fuente que debe editarse.

## Decisión y alcance

Se amplió el parser existente de `IF_ELSE_Paulo/` porque ya reúne la estructura
completa del programa y el trabajo de IF/ELSE. No se creó otro lexer/parser ni
se copiaron las reglas de los compañeros a una tercera gramática. Se conservaron
los cuatro ejemplos originales. Los analizadores raíz y `proyectoF.y` siguen
sin cambios para evitar mezclar el trabajo de ciclos con esta implementación.

Antes faltaban valores semánticos, representación de instrucciones, temporales,
etiquetas, saltos y generación de código. Ahora se implementan para:

- Declaraciones, asignaciones, INPUT y OUTPUT.
- Suma, resta, multiplicación, división, potencia y paréntesis aritméticos.
- Las seis comparaciones: `<`, `>`, `<=`, `>=`, `==`, `<>`.
- IF, IF/ELSE y anidamiento en ambos bloques, incluyendo bloques vacíos.
- AND/OR con cortocircuito y paréntesis lógicos; AND tiene prioridad sobre OR.

## Archivos y funcionamiento

- `IF_ELSE_Paulo/proyectoFinal.l`: mantiene las palabras y símbolos existentes;
  conserva el texto de números e identificadores en nodos. Permite identificadores
  de un carácter y `_` inicial. Cuenta líneas e ignora comentarios `*** ... ***`.
- `IF_ELSE_Paulo/proyectoFinal.y`: conserva la estructura existente y construye un
  árbol sencillo usando acciones de Bison. Separa las reglas AND y OR para darles
  precedencia y permite agrupar condiciones. Genera solo después de que `yyparse`
  acepte el archivo completo, incluyendo fin de entrada.
- `IF_ELSE_Paulo/codigo.h` y `codigo.c`: estructuras y funciones pequeñas en C
  para crear nodos, liberar memoria y emitir las instrucciones.
- `IF_ELSE_Paulo/Makefile`: genera todo en `build/` y compila desde los fuentes.
- `IF_ELSE_Paulo/pruebas/probar.py`: pruebas de compilación y ejecución del código
  emitido con un intérprete pequeño, usado exclusivamente para verificar resultados.

Cada operación aritmética genera un temporal `$t1`, `$t2`, etc. El prefijo `$`
no puede aparecer en una variable EasyLang, evitando colisiones con nombres como
`t1`. Cada IF reserva etiquetas de verdadero y falso; con ELSE reserva también
una etiqueta de salida para saltar el bloque alternativo. Cada IF anidado genera
sus propias etiquetas a partir del mismo contador.

AND salta al segundo operando únicamente si el primero es verdadero; OR lo hace
únicamente si el primero es falso. Las operaciones aritméticas del segundo
operando se emiten dentro de ese camino. Por eso `x <> 0 AND 10 / x > 1` evita
la división cuando `x` vale cero. No se calculan ambos lados por adelantado.

Los nodos se registran en una lista de reservas y se liberan incluso si el parser
rechaza el programa. Los errores van a stderr con línea y texto cercano, devuelven
un estado distinto de cero y no producen código parcial en stdout.

La potencia conserva la asociación a izquierda de las gramáticas existentes:
`2 ** 3 ** 2` equivale a `(2 ** 3) ** 2`, es decir, 64. No se añadió menos unario,
reales ni palabras reservadas en minúsculas. El cambio deliberado de precedencia
lógica significa que `A OR B AND C` ahora se interpreta como `A OR (B AND C)`;
se pueden usar paréntesis si se desea `(A OR B) AND C`.

## Uso local

Requiere Flex, Bison, GCC y GNU Make con soporte de objetivos agrupados. Python 3
solo es necesario para las pruebas; el compilador está escrito en Flex, Bison y C.
Desde la raíz:

```bash
make -C IF_ELSE_Paulo
IF_ELSE_Paulo/build/easylang < IF_ELSE_Paulo/pruebas/if_else.el
make -C IF_ELSE_Paulo test
```

Para guardar el resultado:

```bash
IF_ELSE_Paulo/build/easylang < IF_ELSE_Paulo/pruebas/if_else.el > /tmp/if_else.tac
```

Ejemplo de entrada:

```text
MAIN BEGIN
    x ALLOCATE;
    INPUT x;
    IF(x > 10) THEN BEGIN
        x := x + 1;
    END ELSE BEGIN
        x := 0;
    END
    OUTPUT x;
END
```

Salida:

```text
allocate x
input x
if x > 10 goto L1
goto L2
L1:
$t1 = x + 1
x = $t1
goto L3
L2:
x = 0
L3:
output x
```

`allocate`, `input` y `output` son pseudoinstrucciones del código intermedio.
El resultado todavía no es código máquina ni un ejecutable del programa EasyLang.

## Verificación realizada

- Compilación desde las versiones originales en directorios temporales para
  comprobar los problemas de las ramas sin modificarlas.
- Generación con `bison -Wall -Werror`: sin conflictos ni advertencias.
- Compilación C con `-std=c11 -Wall -Wextra -Wpedantic`: sin advertencias.
- 53 pruebas correctas: ejemplos originales; ambos caminos de IF; tres niveles de
  anidamiento; seis operadores relacionales; combinaciones de tres booleanos con
  precedencia y agrupación; cortocircuito con división por cero; aritmética y
  potencia; E/S; bloques vacíos; identificadores; comentarios; errores y ausencia
  de salida parcial. El intérprete comprueba etiquetas únicas y destinos válidos.
- Las mismas pruebas pasaron con AddressSanitizer y UndefinedBehaviorSanitizer.
  Se usó `ASAN_OPTIONS=detect_leaks=0` porque LeakSanitizer no funciona bajo ptrace
  en este entorno. Por tanto, esa ejecución no verifica fugas de memoria.
- `git diff --check` sin problemas de espacios.

El intérprete inicializa las variables declaradas a cero para ejecutar los
fixtures; eso es una convención del intérprete de pruebas, no una definición de
inicialización de EasyLang ni una validación semántica del compilador.

## Pendientes para integrar el proyecto

1. Acordar un único lexer/parser de entrada, tomando esta estructura MAIN y los
   nombres de tokens consistentes. Actualmente el ejecutable raíz antiguo sigue
   siendo distinto de `IF_ELSE_Paulo/build/easylang`.
2. Incorporar las reglas de FOR y DO-WHILE de María a esa gramática, resolver sus
   conflictos y agregar nodos/generación con etiquetas de inicio y salida.
   Este generador todavía no acepta ciclos.
3. Añadir tabla de símbolos y análisis semántico: declaraciones duplicadas,
   variables sin declarar, inicialización, tipos y errores de operaciones.
   Por ahora una variable no declarada puede aparecer en el código generado.
4. Acordar semántica numérica, asociación de potencia y precedencia lógica con
   el equipo. El formato intermedio actual conserva `**` como operación.
5. Crear el backend que reciba fuente y ejecute el compilador; conectar la web
   para mostrar stdout/stderr y estado de salida. La interfaz aún no compila.
6. Unificar las instrucciones de compilación y dejar de versionar binarios y
   archivos generados de la raíz cuando se acuerde la integración.

En la revisión inicial no se hizo commit, merge ni push.

## Revisión final para entrega

Se revisaron los nueve archivos de la entrega y se ejecutó nuevamente
`make -C IF_ELSE_Paulo -B test`, regenerando el lexer y el parser con Flex y
Bison y recompilando con GCC. Las 53 pruebas volvieron a pasar, sin
advertencias de compilación ni conflictos de gramática. No fue necesario
cambiar la implementación.

Los archivos generados y el ejecutable de `IF_ELSE_Paulo/build/` están
ignorados y no se incluyen en el commit. Los binarios y archivos generados
antiguos de la raíz ya estaban en el historial y se mantienen intactos.
La entrega se prepara únicamente en `codigo-tres-direcciones-paulo`, con el
mensaje de commit «Implementación de código de tres direcciones - Paulo»,
para publicar exclusivamente esa rama, sin merge ni Pull Request.
Las otras ramas y el respaldo de git stash se conservan sin cambios.
