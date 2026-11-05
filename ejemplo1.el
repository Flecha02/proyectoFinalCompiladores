*** Ejemplo 1: Aritmetica, IO y Potencia ***
MAIN
BEGIN
    base:expo:resultado ALLOCATE;
    INPUT base;
    INPUT expo;
    
    resultado := expo ** base;
    OUTPUT resultado;
END