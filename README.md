# NFCJokeGenerator

Na posielanie vtipov cez NFC pomocou STM32 a SD karty na ukladanie vtipov budeme musieť integrovať viaceré komponenty. Rozvrh práce pre 4-členný tím, kde sa zameriame na rovnomerné rozdelenie úloh v rámci 4 týždňov. Cieľom bude efektívne naplánovať prácu tak, aby sa minimalizovali kolízie a aby všetci členovia tímu mohli paralelne pracovať na rôznych častiach projektu. Z každého týžńa vytvoríme stručnú dokumentáciu, ktorú aj odprezentujeme.

Prehľad úloh projektu

1. Práca s SD kartou a súborovým systémom (FatFS) – umožní uloženie vtipov do súboru na SD karte a ich následné čítanie.


2. Integrácia NFC modulu (SHIELD-M24SR-ADV) – prenášanie dát cez NFC.


3. Návrh a implementácia logiky programu – obsluha vtipov, náhodné vyberanie a odosielanie cez NFC.


4. Testovanie a ladenie – kontrola funkčnosti a odstraňovanie chýb.


Rozdelenie úloh a harmonogram pre 4-členný tím
členovia tímu:
David Cauner
Matej Horský
Peter Palček
Samuel Petrík 

Každý týždeň si rozdelíme úlohy A, B, C a D.

Týždeň 1 – Príprava a základná konfigurácia

A (SD karta a FatFS): Nainštaluje a nastaví FatFS na STM32. Skontroluje, že súborový systém dokáže korektne vytvárať a čítať súbory na SD karte.

B (NFC modul): Nainštaluje a základne nastaví SHIELD-M24SR-ADV, pripraví testovaciu aplikáciu na kontrolu NFC komunikácie.

C (Programová logika): Napíše základnú kostru programu na čítanie vtipov zo súboru, vrátane formátu na ukladanie vtipov.

D (Test a dokumentácia): Vytvorí základ dokumentácie projektu a začne pracovať na jednoduchých testovacích skriptoch na testovanie čítania a zapisovania na SD kartu.


Týždeň 2 – Implementácia a prvotné prepojenie

A (SD karta a FatFS): Pripraví súbor s vtipmi na SD karte a vyvinie funkciu na náhodné vyberanie vtipu z databázy.

B (NFC modul): Napíše základné funkcie pre odosielanie dát cez NFC a skontroluje, že NFC modul korektne funguje s mobilným zariadením.

C (Programová logika): Dokončí logiku pre výber vtipu z databázy a pripraví funkciu na odoslanie vtipu cez NFC (spolupracuje s B).

D (Test a dokumentácia): Začne testovanie implementovaných častí – testuje čítanie zo súboru a prenos cez NFC, a zaznamená priebežné výsledky.


Týždeň 3 – Integrácia a hlavné testovanie

A (SD karta a FatFS): Zabezpečí optimalizáciu prístupu k súborom na SD karte, vrátane optimalizácie rýchlosti a spoľahlivosti.

B (NFC modul): Implementuje pokročilé funkcie NFC, aby bolo možné správne odosielať a prijímať celé vtipy.

C (Programová logika): Prepojí celý systém do jedného programu, zabezpečí synchronizáciu medzi čítaním zo súboru a odosielaním cez NFC.

D (Test a dokumentácia): Spustí integrované testy na kompletnom systéme a začne písať finálnu dokumentáciu.


Týždeň 4 – Finálne úpravy a prezentácia

A (SD karta a FatFS): Zabezpečí záverečné ladenie, vrátane chýb a optimalizácie pri čítaní zo súboru.

B (NFC modul): Testuje NFC komunikáciu s rôznymi mobilnými zariadeniami a robí úpravy pre kompatibilitu.

C (Programová logika): Pridá finálne funkcie, ako sú rôzne módy pre odosielanie vtipov (napríklad náhodné alebo podľa poradia).

D (Test a dokumentácia): Dokončí dokumentáciu, vytvorí finálnu správu a pripraví krátku prezentáciu pre tím.


Dodržaním tohto postupu by sa nám malo podariť splniť zadanie načas a v dobrej kvalite.
