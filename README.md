# Simple Graphics Framework

- Prošle godine sam takođe radio projekat:
[SenseFX](https://github.com/kotur95/sensefx)

## Važna napomena
- Projekat ima 2 grane:
	1) master (Glavna grana)
	2) januar2 (Samo za ispitni rok januar2)
**Bitno:**  Na ovaj način glavna grana neće biti kompromitovana izmenama (za januar2) koje neće ući u finalnu verziju projekta.

## Januar 2
[Link projektu januar2 2018](https://github.com/MATF-RG18/RG13-simple-graphics-framework/tree/januar2/januar2)
- Grana januar2 sadrži projakat `Space Climbing` (Program implementiran ove godine pomoću starog 'framework'-a).
- Implementacija novog framework-a je u toku (biće, nadam se, u nekom od narednih rokova ili bar do kraja fakultetske godine).
- Pored nove simulacije (Space Climbing) tu je i stara simulacija od prošle godine (SenseFX).

## Novi Framework
### U novom frameworku za sada je uradjeno:
- Struktuiran kod - Krajnji korisnik nema predstavu da je iza scene framework u simbiozi sa OpenGL bibliotekom.
- Hijerarhija klasa i struktura projekta omogućava da framework bude prevezan na DirectX ili bilo koji drugi low level API.
- Sav kod koji je zavistan od `OpenGL` biblioteke se nalazi u `opengl` folderu, za sada samo jedan (hpp i cpp) fajl.
- Korisnik može proslediti funkcije i objekte framework-u koje obrađuju ulaz. Ti objekti moraju naslediti useKeyboard/useMouse klasu i implementirati odg. metode.

### Kako videti šta je do sada urađeno
- Za primere koji na praktičan način prikazuju korišćenje frameworka pogledati `demos` direktorijum.
- Demo direktorijum se build-uje sa `make demos`.

Za više informacija posetiti: [wiki](https://github.com/MATF-RG18/RG13-simple-graphics-framework/wiki) stranu projekta.
