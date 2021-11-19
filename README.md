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
>Funkcia `strrchr()` bola nájdená v dokumentácií knižnice `<string.h>` na stránke [TutorialsPoint](https://www.tutorialspoint.com/c_standard_library/c_function_strrchr.htm).\
Táto funkcia nájde pozíciu posledného výskytu 2. parametru, v tomto prípade 32 (ASCII tabuľka - medzera), a po pričítaní 1 sa vráti string priezviska, ktorý je ďalej použitý pri porovnávaní so vstupným priezviskom

`
strcpy(DESTINATION,ORIGIN);
`
>Funkcia `strcpy()` bola nájdená v dokumentácií knižnice `<string.h>` na stránke [TutorialsPoint](https://www.tutorialspoint.com/c_standard_library/c_function_strcpy.htm).\
Táto funkcia skopíruje string z ORIGINU do DESTINATION, a vráti daný string

## Pomocný opis niektorých funkcií

### Pomocné funkcie
`int checkIntegrity()`
>Prejde každý riadok súboru a ak je niekde chyba vo formátovaní súboru, tak ju označí, vypíše a vráti premennú `integrityStatus = 0`, ktorá ukončí program.
Táto funkcia nedovolí spustiť program, pokiaľ nie je formát riadkov v poriadku.
Pri nájdení chyby, zavolá funkciu `getSegmentError()`, do ktorej vloží argumenty pozície riadku, segmentu a nájdeného neplatného stringu

`void getSegmentError(int segmentIndex, int rowIndex, char resultString[128])`
>Pomocou switchu zistí, v ktorej časti bola chyba vo formátovaní a príslušne ju vypíše.

### Hlavné funkcie
`void rmdriver()`
>Funkcia si postupne načíta riadky, tie následnde rozdelí na segmenty. Z prvého segmentu si získa priezvisko, ktoré porovná so vstupom. Ak sa zhodujú, zmení hodnotu premenných `int found` a `int globalFound`  na hodnotu `1`. `int found` určuje, či sa našiel a po každom riadku sa resetne. `int globalFound` určuje či sa vôbec niekto našiel. Následne funkcia do pomocného súboru prepíše údaje všetkých jazdcov okrem nájdeného. Ak už niekoho pridalo, funkcia do súboru vpíše aj `\n`, aby každý jazdec bol na novom riadkU. Následne prepíše obsah pomocného súboru do súboru tabulka.csv

`void brand()`
>Funkcia zapisuje do premenných hodnoti najlepších kôl značiek. Každá značka má svoj index. Ferrari je 0, Buggati je 1, Porsche je 2, Honda je 3. Podľa toho si funkcia porovnáva kolá len s tými z jej značky. Ak najde lepšie kolo, prepíše meno jazdca a poradie kola. Následne vytlačí sumárny výpis pre každú značku.

`void driver()`
>Funkcia pracuje na rovnakom princípe ako `void rmdriver()`, až na to že namiesto vymazania jazdca ho vypíše do konzoly

`void under()`
>Funkcia si postupne načíta riadky, tie následne rozdelí na segmenty. Potom porovnáva, či daný jazdec má nejaké kolo lepšie ako zadaný čas. Ak má, uloží jeho hodnoty do premennej a tie následne vypíše.

`void average()`
>Funkcia si postupne načíta riadky, tie následne rozdelí na segmenty. Pri kolách si do premennej `float averageRound` pripočíta dané kolo, ktoré je predelené číslom 5.0 (celkový počet kôl). Následne kontroluje či existuje najlepšie kolo, ak nie priradí mu prvý averageRound. Následne porovnáva bestRound a averageRound a ak je averageRound lepší, tak prepíše najlepšie hodnoty.

`void newdriver()`
>Funkcia zapisuje hodnoty nového jazdca. Vždy kontroluje zadaný formát. Ak je formát zlý, ukončí funkciu. Taktiež kontroluje, ako boli zadané časy kôl. Ak je v nich namiesto bodkočiarky čiarka, upozorní používateľa a prepíše čiarku na bodkočiarku

`void menu()`
>Táto funkcia načítava príkazy a volá podľa nich funkcie. Taktiež kontroluje, čo bolo zadané. Ak bolo zadaných viacero znakov, vždy zoberie len ten prvý znak