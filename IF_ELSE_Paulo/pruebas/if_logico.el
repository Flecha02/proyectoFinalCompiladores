MAIN
BEGIN
    edad:nota ALLOCATE;

    IF(edad >= 18 AND nota >= 60) THEN
    BEGIN
        OUTPUT nota;
    END

    IF(edad < 18 OR nota < 60) THEN
    BEGIN
        OUTPUT edad;
    END
END
