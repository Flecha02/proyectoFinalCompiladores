"""Pruebas del compilador real y ejecucion de su codigo de tres direcciones."""
from pathlib import Path
import operator
import re
import subprocess
import sys

COMPILADOR = str(Path(sys.argv[1]).resolve())
CARPETA = Path(__file__).parent
OPERACIONES = {
    '+': operator.add, '-': operator.sub, '*': operator.mul,
    '/': operator.truediv, '**': operator.pow,
    '<': operator.lt, '>': operator.gt, '<=': operator.le,
    '>=': operator.ge, '==': operator.eq, '<>': operator.ne,
}


def compilar(fuente):
    return subprocess.run([COMPILADOR], input=fuente, text=True,
                          capture_output=True, check=False)


def ejecutar(codigo, entradas=()):
    """Interprete solo para pruebas; una division por cero hace fallar el caso."""
    lineas = codigo.splitlines()
    etiquetas = {}
    for posicion, linea in enumerate(lineas):
        if re.fullmatch(r'L\d+:', linea):
            nombre = linea[:-1]
            assert nombre not in etiquetas, f'Etiqueta repetida: {nombre}'
            etiquetas[nombre] = posicion
    for destino in re.findall(r'goto (L\d+)', codigo):
        assert destino in etiquetas, f'Etiqueta sin definir: {destino}'
    variables = {}
    datos = iter(entradas)
    salida = []

    def valor(nombre):
        return int(nombre) if nombre.isdigit() else variables[nombre]

    posicion = 0
    pasos = 0
    while posicion < len(lineas):
        pasos += 1
        assert pasos < 10000, 'Posible salto infinito'
        partes = lineas[posicion].split()
        posicion += 1
        if partes[0].endswith(':'):
            continue
        if partes[0] == 'allocate':
            variables[partes[1]] = 0
        elif partes[0] == 'input':
            variables[partes[1]] = next(datos)
        elif partes[0] == 'output':
            salida.append(valor(partes[1]))
        elif partes[0] == 'goto':
            posicion = etiquetas[partes[1]]
        elif partes[0] == 'if':
            assert len(partes) == 6 and partes[4] == 'goto'
            if OPERACIONES[partes[2]](valor(partes[1]), valor(partes[3])):
                posicion = etiquetas[partes[5]]
        else:
            assert partes[1] == '=' and len(partes) in (3, 5), partes
            resultado = valor(partes[2])
            if len(partes) == 5:
                resultado = OPERACIONES[partes[3]](resultado, valor(partes[4]))
            variables[partes[0]] = resultado
    return salida


cantidad = 0


def comprobar(nombre, fuente, esperado, entradas=()):
    global cantidad
    resultado = compilar(fuente)
    assert resultado.returncode == 0, (nombre, resultado.stderr)
    assert not resultado.stderr, (nombre, resultado.stderr)
    salida = ejecutar(resultado.stdout, entradas)
    assert salida == esperado, (nombre, salida, esperado, resultado.stdout)
    # Los contadores deben comenzar igual en cada ejecucion.
    assert compilar(fuente).stdout == resultado.stdout
    cantidad += 1
    print(f'OK: {nombre}')


def programa(cuerpo):
    return f'MAIN BEGIN {cuerpo} END'


# Se conservan y se ejecutan los ejemplos de la rama original de Paulo.
comprobar('if_else verdadero', (CARPETA / 'if_else.el').read_text(), [1], [11])
comprobar('if_else falso', (CARPETA / 'if_else.el').read_text(), [0], [10])
for edad, nota, esperado in [(18, 60, 60), (18, 59, 0), (17, 99, 17)]:
    comprobar(f'anidado {edad}/{nota}', (CARPETA / 'if_anidado.el').read_text(),
              [esperado], [edad, nota])
comprobar('ejemplo logico original', (CARPETA / 'if_logico.el').read_text(), [0])

for x in (0, 1):
    comprobar(f'IF sin ELSE {x}', programa(f'''
        x:y ALLOCATE; x := {x}; y := 7;
        IF(x == 1) THEN BEGIN y := 9; END OUTPUT y;
    '''), [9 if x else 7])

for operador, pares in {
    '<': [(1, 2), (2, 1)], '>': [(2, 1), (1, 2)],
    '<=': [(2, 2), (3, 2)], '>=': [(2, 2), (1, 2)],
    '==': [(2, 2), (1, 2)], '<>': [(1, 2), (2, 2)],
}.items():
    for izquierda, derecha in pares:
        esperado = int(OPERACIONES[operador](izquierda, derecha))
        comprobar(f'comparacion {izquierda} {operador} {derecha}', programa(f'''
            r ALLOCATE;
            IF({izquierda} {operador} {derecha}) THEN BEGIN r := 1; END
            ELSE BEGIN r := 0; END OUTPUT r;
        '''), [esperado])

for a in (0, 1):
    for b in (0, 1):
        for c in (0, 1):
            for condicion, esperado in [
                ('a == 1 OR b == 1 AND c == 1', a or (b and c)),
                ('(a == 1 OR b == 1) AND c == 1', (a or b) and c),
            ]:
                comprobar(f'precedencia {a}{b}{c}: {condicion}', programa(f'''
                    a:b:c:r ALLOCATE; a := {a}; b := {b}; c := {c};
                    IF({condicion}) THEN BEGIN r := 1; END ELSE BEGIN r := 0; END
                    OUTPUT r;
                '''), [int(esperado)])

comprobar('AND evita division por cero', programa('''
    x:r ALLOCATE; x := 0; r := 7;
    IF(x <> 0 AND 10 / x > 1) THEN BEGIN r := 8; END OUTPUT r;
'''), [7])
comprobar('OR evita division por cero', programa('''
    x:r ALLOCATE; x := 0; r := 7;
    IF(x == 0 OR 10 / x > 1) THEN BEGIN r := 8; END OUTPUT r;
'''), [8])
comprobar('AND evalua derecha', programa('''
    r ALLOCATE;
    IF(1 == 1 AND 0 == 1) THEN BEGIN r := 1; END ELSE BEGIN r := 2; END
    OUTPUT r;
'''), [2])
comprobar('OR evalua derecha', programa('''
    r ALLOCATE;
    IF(0 == 1 OR 1 == 1) THEN BEGIN r := 1; END ELSE BEGIN r := 2; END
    OUTPUT r;
'''), [1])
comprobar('aritmetica, potencia y nombres de temporales', programa('''
    _x:t1:r ALLOCATE; t1 := 5;
    _x := 2 + 3 * 2 ** 3; r := 2 ** 3 ** 2;
    OUTPUT _x; OUTPUT r;
    r := (t1 + 1) * (9 - 3) / 2; OUTPUT r;
    IF((_x + 1) >= (r * 1) AND (t1 == 5 OR r < 0)) THEN
    BEGIN OUTPUT t1; END
'''), [26, 64, 18, 5])
comprobar('bloques vacios y ELSE vacio', programa('''
    IF(1 == 1) THEN BEGIN END ELSE BEGIN END
    IF(1 == 0) THEN BEGIN END
'''), [])
comprobar('programa vacio', programa(''), [])
comprobar('comentario multilínea', programa('''
    *** comentario
    con * y ** dentro ***
    x ALLOCATE; x := 3; OUTPUT x;
'''), [3])
comprobar('tres niveles y ELSE exterior', programa('''
    r ALLOCATE; r := 0;
    IF(1 == 1) THEN BEGIN
        IF(1 == 0) THEN BEGIN r := 1; END ELSE BEGIN
            IF(2 > 1) THEN BEGIN r := 2; END ELSE BEGIN r := 3; END
        END
    END ELSE BEGIN r := 4; END OUTPUT r;
'''), [2])

invalidos = [
    (CARPETA / 'if_error.el').read_text(),
    programa('x := 1; @'),
    programa('IF(1 < 2) THEN BEGIN x := 1;'),
    programa('x := ;'),
    programa('IF(1 < 2 AND) THEN BEGIN END'),
    programa('ELSE BEGIN END'),
    programa('') + ' basura',
    programa('*** comentario sin cerrar'),
]
for numero, fuente in enumerate(invalidos):
    resultado = compilar(fuente)
    assert resultado.returncode != 0, fuente
    assert resultado.stdout == '', resultado.stdout
    assert 'linea' in resultado.stderr, resultado.stderr
    cantidad += 1
    print(f'OK: error {numero + 1} sin codigo parcial')
resultado = compilar('MAIN\nBEGIN\nx := ;\nEND')
assert 'linea 3' in resultado.stderr, resultado.stderr
print(f'\n{cantidad} pruebas correctas.')
