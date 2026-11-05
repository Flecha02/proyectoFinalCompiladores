*** Ejemplo 2: Condicional IF-ELSE con AND ***
MAIN
BEGIN
    x:y:res ALLOCATE;
    INPUT x;
    INPUT y;
    
    IF (x < 10 AND y > 0) THEN
    BEGIN
        res := 1;
    END
    ELSE
    BEGIN
        res := 0;
    END
    
    OUTPUT res;
END