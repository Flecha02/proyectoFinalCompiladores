# Compiladores EasyLang

Proyecto con analizadores Flex/Bison en C e inicio de una interfaz web.

El generador de código de tres direcciones de la rama
`codigo-tres-direcciones-paulo` amplía el analizador existente en
`IF_ELSE_Paulo/`: soporta IF, ELSE, anidamiento, AND/OR con cortocircuito,
expresiones, potencia, asignaciones y entrada/salida.

Desde la raíz, con Flex, Bison, GCC y GNU Make instalados:

```bash
make -C IF_ELSE_Paulo
IF_ELSE_Paulo/build/easylang < IF_ELSE_Paulo/pruebas/if_else.el
make -C IF_ELSE_Paulo test
```

Las pruebas requieren Python 3. Los archivos generados quedan en
`IF_ELSE_Paulo/build/`, que está ignorado por Git.

Consulta la [revisión de ramas, funcionamiento y pendientes de integración](docs/codigo-tres-direcciones.md).

- `IF_ELSE_Paulo/`: analizador ampliado y generador de código intermedio.
- `proyectoFinal.l`, `proyectoFinal.y`, `proyectoF.y`: versiones anteriores;
  todavía no están unificadas con el generador.
- `frontend/proyecto-final/`: plantilla Next.js, sin conexión al compilador.
- `backend/`: pendiente de implementación.
