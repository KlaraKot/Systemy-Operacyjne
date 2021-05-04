/*
  PLIKI - ZADANIE 1
  Napisz program kopiujący zawartośćpliku o nazwie podanej jako pierwszy
  parametr do pliku którego nazwa podana jest jako drugi parametr
  Użycie: ./ex1 f1 f2
  Kopiowanie zawartości pliku f1 do pliku f2.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void die(char *message);

int main(int argc, char *argv[]) {

  if(argc <= 2) die("Podano za mało argumentów");

  int file_src, file_dst;
  if((file_src = open(argv[1], O_RDONLY)) < 0 ) die("Błąd przy otwieraniu pliku źródła");
  if((file_dst = creat(argv[2], 0664)) < 0 ) die("Błąd przy otwieraniu pliku przeznaczenia");
  char buffer[1024];
  int byte_len;
  while((byte_len = read(file_src, buffer, 1024)) > 0) {
    if(write(file_dst, buffer, byte_len) < 0) die("Błąd przy zapisie do pliku!");
  }
  if(byte_len < 0) die("Błąd przy odczycie pliku!");

  if((close(file_src) < 0) || (close(file_dst) < 0)) die("Błąd przy zamknięciu pliku!");

  return EXIT_SUCCESS;
}

void die(char *message) {

  if(errno != 0)
    perror(message);
  else
    fprintf(stderr, "ERROR: %s\n", message);
  exit(1);

}
 ex2.c
/*
  PLIKI - ZADANIE 2
  Napisz program zmieniający kolejność znaków w każdej linii pliku o nazwie
  podanej jako parametr.
  Użycie: ./ex2 f1
  Zamiana kolejności znaków w liniach pliku f1.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void die(char *message);

int main(int argc, char *argv[]) {

  if(argc <= 1) die("Podano za mało argumentów");

  int file, tmp_file;
  if((file = open(argv[1], O_RDONLY)) < 0 ) die("Błąd przy otwarciu pliku");
  if((tmp_file = creat(".tmp_file", 0664)) < 0 ) die("Błąd przy tworzeniu pliku tymczasowego");

  int line_len = 0;
  char line_buffer[1024];
  char buffer;

  while(read(file, &buffer, sizeof(buffer)) > 0) {
    line_len++;
    if(buffer == '\n') {
      for(int i = line_len - 1; i > 0; i--) {
        if(write(tmp_file, &line_buffer[i], 1) < 0) die("Błąd przy zapisie do pliku!");
      }
      if(write(tmp_file, "\n", 1) < 0) die("Błąd przy zapisie do pliku!");
      line_len = 0;
    }
    line_buffer[line_len] = buffer;
  }

  if(close(file) < 0) die("Błąd przy zamknięciu pliku!");
  if(close(tmp_file) < 0) die("Błąd przy zamknięciu pliku!");

  if((file = open(argv[1], O_WRONLY)) < 0 ) die("Błąd przy otwarciu pliku");
  if((tmp_file = open(".tmp_file", O_RDONLY)) < 0 ) die("Błąd przy otwarciu pliku");

  lseek(tmp_file, 0, SEEK_SET);
  char cp_buffer[1024];
  int byte_len;
  while((byte_len = read(tmp_file, cp_buffer, 1024)) > 0) {
    if(write(file, cp_buffer, byte_len) < 0) die("Błąd przy zapisie do pliku!");
  }
  if(byte_len < 0) die("Błąd przy odczycie pliku!");


  if(close(file) < 0) die("Błąd przy zamknięciu pliku!");
  if(unlink(".tmp_file") < 0) die("Błąd przy usuwaniu pliku!");

  return EXIT_SUCCESS;
}

void die(char *message) {

  if(errno != 0)
    perror(message);
  else
    fprintf(stderr, "ERROR: %s\n", message);
  exit(1);

}
 ex3.c
/*
  PLIKI - ZADANIE 3
  Napisz program wyszukujący najdłuższą linię w pliku i podający ilość znaków
  w tej linii.
  Użycie: ./ex3 f1
  Podaje długość najdłuższej linii w f1.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void die(char *message);

int main(int argc, char *argv[]) {
  if(argc <= 1) die("Podano za mało argumentów");

  int file;
  if((file = open(argv[1], O_RDONLY)) < 0 ) die("Błąd przy otwarciu pliku");

  int line_len = 0;
  int max_line_len = 0;
  char buffer;

  while(read(file, &buffer, sizeof(buffer)) > 0) {
    line_len++;
    if(buffer == '\n') {
      if(line_len > max_line_len)
        max_line_len = line_len-1;
      line_len = 0;
    }
  }

  if(close(file) < 0) die("Błąd przy zamknięciu pliku!");
  printf("%d\n", max_line_len);

  return EXIT_SUCCESS;

}

void die(char *message) {

  if(errno != 0)
    perror(message);
  else
    fprintf(stderr, "ERROR: %s\n", message);
  exit(1);

}
 ex4.c
/*
  PLIKI - ZADANIE 4
  Napisz program określający rozmiar pliku ( kilku plików o nazwach podanych
  jako parametry wejściowy)
  Użycie: ./ex4 f1 f2 f3 ... fn
  Wyświetla rozmiar plików f1, f2, f3, ..., fn
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void die(char *message);

int main(int argc, char *argv[]) {

  if(argc <= 1) die("Podano za mało argumentów");

  for(int i = 1; i <argc; i++) {
    int file;
    if((file = open(argv[i], O_RDONLY)) < 0 ) die("Błąd przy otwieraniu pliku źródła");
    char buffer[1024];
    int byte_len;
    int whole_len = 0;
    while((byte_len = read(file, buffer, 1024)) > 0) {
      whole_len += byte_len;
    }
    if(byte_len < 0) die("Błąd przy odczycie pliku!");
    if(close(file) < 0) die("Błąd przy zamknięciu pliku!");
    printf("%d\n", whole_len);
  }

  return EXIT_SUCCESS;
}

void die(char *message) {

  if(errno != 0)
    perror(message);
  else
    fprintf(stderr, "ERROR: %s\n", message);
  exit(1);

}