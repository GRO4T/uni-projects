CREATE TABLE "DKOLASKA"."TEST1" (
	"ID" NUMBER NOT NULL ENABLE, 
    "SELEKTYWNOSC20" NUMBER,
    "SELEKTYWNOSC2" NUMBER,
	CONSTRAINT "TEST1_PK" PRIMARY KEY ("ID")
);

CREATE TABLE "DKOLASKA"."TEST2" (
	"ID" NUMBER NOT NULL ENABLE, 
	"NAZWA" VARCHAR2(200 BYTE),
	CONSTRAINT "TEST1_PK" PRIMARY KEY ("ID")
);

create or replace PROCEDURE rowgen(no_row IN NUMBER)
AS
BEGIN
  FOR i IN 1..no_row
  LOOP
    INSERT INTO TEST1 VALUES (i, TRUNC(DBMS_RANDOM.value(1,6)), TRUNC(DBMS_RANDOM.value(1,51)));
  END LOOP;  
END rowgen;
/

create or replace PROCEDURE rowgen_2(no_row IN NUMBER)
AS
BEGIN
  FOR i IN 1..no_row
  LOOP
    INSERT INTO TEST1 VALUES (i, TRUNC(DBMS_RANDOM.value(1,6)), TRUNC(DBMS_RANDOM.value(1,51)));
  END LOOP;  
END rowgen;
/

CALL rowgen(10000);

DROP PROCEDURE "DKOLASKA"."ROWGEN";
DROP TABLE "DKOLASKA"."TEST1";