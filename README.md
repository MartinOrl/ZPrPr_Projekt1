# Projekt 2021 C

### Autor: Martin Orlej
### Deadline: 19.11.2021
### Formát súboru: CSV

## Github repozitár: [GitHub - Martin Orlej](https://github.com/MartinOrl/ZPrPr_Projekt1)

## Vysvetlenie a zdroje niektorých funkcií a zápisov

`#define MAXCHAR 256`
>Toto je preprocesorova direktíva, ktorá compileru povie, aby všetky miesta, kde sa daný názov nachádza zmenil za číslo. V tomto prípade všade kde je MAXCHAR zmení na 256, čo je maximálny limit pri načítavaní riadku. Tento limit by mal vždy pokryť každý riadok

```
segment = strtok(row, ";,");
while(segment != NULL){
    ...code; <br />
    segment = strtok(NULL,"+,");
}
```
>Funkcia `strtok()` bola nájdená v dokumentácií knižnice `<string.h>` na stránke [TutorialsPoint](https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm).\
Táto funkcia rozdelí string, v tomto prípade riadok, na časti cez delimitere (2. parameter funkcie). `NULL` sa zadáva preto, aby v tomto rozdeľovaní pokračoval, až kým sa nedostane na koniec riadka, kedy vráti NULL a cyklus sa uzavrie

```
char *last;
last = strrch(segment, 32);
if(strstr(last+1, name)){
    ...code;
}
```
>Funkcia `strrchr()` bola nájdená v dokumentácií knižnice `<string.h>` na stránke [TutorialsPoint](https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm).\
Táto funkcia nájde pozíciu posledného výskytu 2. parametru, v tomto prípade 32 (ASCII tabuľka - medzera), a po pričítaní 1 sa vráti string priezviska, ktorý je ďalej použitý pri porovnávaní so vstupným priezviskom

`
strcpy(DESTINATION,ORIGIN);
`
>Funkcia `strcpy()` bola nájdená v dokumentácií knižnice `<string.h>` na stránke [TutorialsPoint](https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm).\
Táto funkcia skopíruje string z ORIGINU do DESTINATION, a vráti daný string

## Zoznam funkcií

### Pomocné funkcie
`int checkIntegrity()`
>Prejde každý riadok súboru a ak je niekde chyba vo formátovaní súboru, tak ju označí, vypíše a vráti premennú `integrityStatus = 0`, ktorá ukončí program.
Táto funkcia nedovolí spustiť program, pokiaľ nie je formát riadkov v poriadku.
Pri nájdení chyby, zavolá funkciu `getSegmentError()`, do ktorej vloží argumenty pozície riadku, segmentu a nájdeného neplatného stringu

`void getSegmentError(int segmentIndex, int rowIndex, char resultString[128])`
>Pomocou switchu zistí, v ktorej časti bola chyba vo formátovaní a príslušne ju vypíše.