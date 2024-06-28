Dla obu sposobów implementacji należy przeprowadzić pomiar czasu wykonywania. Wyniki należy przedstawić w formie pliku ```pomiar_zad_2.txt```.

# Zadanie 1 
Napisz program, który kopiuje zawartość jednego pliku do drugiego, odwróconą bajt po bajcie.

Wybrany wariant należy opracować na dwa sposoby:
- Czytanie po 1 znaku.
- Czytanie bloków po 1024 znaki (plik wynikowy powinien być identyczny jak w wariancie 1.)

W zadaniu 1 można wybrać do zaimplementowania tylko jeden wariant:
- albo fopen(), fseek(), fread(), fwrite(), fclose()
- albo open(), lseek(), read(), write(), close()



# Zadanie 2
 Napisz program, który będzie przeglądał bieżący katalog, korzystając z funkcji ```opendir()```, ```readdir()``` i ```stat()```. Dla każdego znalezionego pliku, który nie jest katalogiem, czyli ```!S_ISDIR(bufor_stat.st_mode)```, należy wypisać rozmiar i nazwę pliku. Ponadto na koniec należy wypisać sumaryczny rozmiar wszystkich plików. Nie należy przeglądać podkatalogów! Sumaryczny rozmiar plików należy przechowywać w zmiennej typu ```long long``` i wypisywać ją przez format ```%lld```.

Działanie programu porównaj z działaniem polecenia ```wc --bytes *```.
