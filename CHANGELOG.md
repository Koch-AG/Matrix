### V1.3.1 (29.09.2023), Martin Burger
- Create new Release with all changes until now. Especially change from char to byte in font.h

### V1.3.0 (?)
- ?

### V1.2.0 (28.07.2015, Sven Gehring)
- Code neu formatiert, fehlerhafte Indents behoben
- Kommentare übersetzt, ergänzt und Funktionsheader hinzugefügt
- Klassenname wurde von `matrix_class` zu `rkag_matrix` geändert
- `matrix.taste_1`...`matrix.taste_3` zu `matrix.button_1`...`matrix.button_3` geändert
- Statischen Tastengrenzwert mit `BTN_THRESHOLD` ersetzt
- Mehrere Methoden mit Schleifen vereinfacht
- `matrix.font_write` mit `matrix.write_char` ersetzt
    - Parametertyp von `int` zu `char` geändert, da ein Zeichen geschrieben wird
- `matrix.pcf_write` Maskierung in definition `PCF_MASKBTN` ausgelagert



### V1.1.0 (09.03.2015, Benjamin Marty) 
- Es darf keine Instanz mehr im Sketch gebildet werden, wird nun direkt in der Library erledigt!

- Funktion taste_1...taste_3 entfernt.
- Neue Funktion `matrix.read_io()` um die `matrix.taste_1`...`matrix.taste_3` variablen zu aktualisieren, gleichzeitig wird auch `matrix.potentiometer_0` aktualisiert.
- Neue Funktion `matrix.write_array` um Array direkt auf Matrix ausgeben zu können
- Examples angepasst Library V1.1


### V1.0.1 (24.02.2015, Benjamin Marty)
- Include von Header File in Header File entfernt
- Neue Funktion taste_1, taste_2, taste_3 und potentiometer. Tastenfunktionen geben 0 bzw. 1 zurück, potentiometer einen Wert zwischen 0-1023

### V1.0.0
- Erste Softwareversion