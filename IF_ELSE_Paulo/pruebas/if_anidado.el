MAIN
BEGIN
    edad:nota ALLOCATE;

    INPUT edad;
    INPUT nota;

    IF(edad >= 18) THEN
    BEGIN
        IF(nota >= 60) THEN
        BEGIN
            OUTPUT nota;
        END
        ELSE
        BEGIN
            nota := 0;
            OUTPUT nota;
        END
    END
    ELSE
    BEGIN
        OUTPUT edad;
    END
END
