MAIN
BEGIN
    x:y ALLOCATE;

    INPUT x;

    IF(x > 10) THEN
    BEGIN
        y := 1;
    END
    ELSE
    BEGIN
        y := 0;
    END

    OUTPUT y;
END
