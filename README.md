# Proyecto Final Compiladores - IDE Web

Estructura del proyecto para el compilador basado en Flex y Bison con interfaz web.

## Estructura de Archivos
- `proyectoFinal.l`: Analizador léxico (Flex).
- `proyectoFinal.y`: Analizador sintáctico (Bison).
- `backend/`: Servidor para conectar la interfaz web con el ejecutable del compilador.
- `frontend/`: Interfaz visual tipo IDE (página web).

## Cómo compilar localmente (Flex y Bison)
1. Generar el parser con Bison:
   ```bash
   bison -d proyectoFinal.y