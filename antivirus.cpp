/********************************************

Демонстрационный антивирус?фаг

для вируса SVC?1740.

********************************************/

#include <stdio.h>

#include <dos.h>

#include <dir.h>

#include <str.h>

#include <process.h>

#include <errno.h>

#include <bios.h>

#include <io.h>

#include <fcntl.h>

#define F_FOUND 0

#define PATH_LEN 128

#define DRIVE_LEN 4

#define BLANK_LEN 80

#define BAD 1

#define GOOD 0

#define DBG

char

/* Строка имени текущего подкаталога */

path[PATH_LEN],

/* Строка имени начального места расположения */

old_path[PATH_LEN],

/* Строка имени требуемого устройства */

drive[DRIVE_LEN],

/* Пустая строка */

blank[BLANK_LEN];

int

/* Количество отсканированных каталогов */

n_dir,

/* Количество исследованных файлов */

n_fil,

/* Количество больных и исцеленных файлов */

n_ill;

int

/* Длина имени файла */

l,

/* Временный индекс */

i;

#include ”antilib.c”

/* Рекурсивная процедура обхода дерева каталогов */

walk()

{

int found_d, found_f;

struct find_t buf;

/* Поиск каталогов */

found_d=_dos_findfirst(”*.*”,_A_SUBDIR ,&buf);

while (found_d == F_FOUND)

{

if ((buf.name[0] != ”.”) && (buf.attrib & _A_SUBDIR ))

{

chdir(buf.name);

walk();

chdir(”..”);

}

found_d=_dos_findnext( &buf );

}

/* К этому моменту не отсканированных нижележащих каталогов

больше не осталось – сканируем файлы */

n_dir++;

getcwd( path, PATH_LEN );

/* Поиск файлов */

found_f=_dos_findfirst(”*.*”,_A_NORMAL ,&buf);

while (found_f == F_FOUND)

{

l=strlen( buf.name );

if (((buf.name[l?3]==”C”)&&

(buf.name[l?2]==”O”)&&

(buf.name[l?1]==”M”))||

((buf.name[l?3]==”E”)&&

(buf.name[l?2]==”X”)&&

(buf.name[l?1]==”E”)))

{

n_fil++;

printf(”%c%s”,13,blank);

printf(”%c%s\%s ”,13,path,buf.name);

/* Нашли новый файл – надо проверить, инфицирован ли он.

Если заражен, то лечим */

if (infected(buf.name)==BAD) cure(buf.name);

}

found_f=_dos_findnext( &buf );

}

}

main( int argc, char *argv[] )

{

puts(”ANTISVC – демонстрационный антивирус?фаг”);

if (argc < 2)

{ puts(”Введите имя диска в качестве параметра”); exit(2); }

if (((toupper(argv[1][0]))>”Z”)||((toupper(argv[1][0]))<”A”))

{ puts(”Неверно задано имя диска”); exit(3); }

drive[0]=argv[1][0]; drive[1]=”:”; drive[3]=”’;

for (i=0;i<BLANK_LEN;i++) blank[i]=” ”;blank[BLANK_LEN?1]=””;

n_dir=0; n_fil=0;

getcwd(old_path, PATH_LEN);

drive[2]=””; system(drive);

drive[2]=”\”; chdir(drive);

/* Запускаем рекурсивный обход дерева каталогов

для выбранного диска */

walk();

old_path[2]=”0”; system(old_path);

old_path[2]=”\”; chdir(old_path);

printf(” Каталогов : %d Файлов : %d Обнаружено больных

и излечено: %d”, n_dir, n_fil, n_ill);

if (n_ill) exit(1); else exit(0);

}

/*Файл «ANTILIB.C», включаемый в предыдущий:*/

/***************************************************************

Процедуры обнаружения и лечения

***************************************************************/

/* Сигнатура */

char sign[7]={ (char) 0xB4,

(char) 0x83,

(char) 0xCD,

(char) 0x21,

(char) 0x5E,

(char) 0x56,

””};

int infected( char *fn )

{

int f;

int r,q;

char buf[7]; /* Буфер под сигнатуру */

/* Открываем файл */

r=_dos_open( fn, O_RDONLY, &f );

if (r) { printf(” – ошибка открытия!”); return GOOD; }

/* Читаем 6 байт */

lseek( f, ?1724, SEEK_END );

r=_dos_read( f, buf, 6, &q ); buf[6]=””;

if ((r)||(q!=6)) {printf(” – ошибка чтения!”); _dos_close(f); return GOOD;

}

/* Закрываем файл */

_dos_close(f);

/* Сравниваем байты с сигнатурой */

if (strcmp( buf, sign)==0)

{ printf(” – был болен и...”); n_ill++; return BAD; } /* Болен !!! */

/* Годен к в/службе. П/пк мед. службы Орлов :?) */

return GOOD;

}

cure( char *fn )

{

int f;

int mz;

int r,q;

char buf[24]; /* Буфер под байты */

/* Открываем файл */

r=_dos_open( fn, O_RDWR, &f );

if (r) { printf(” – ошибка открытия!”); return; }

/* Читаем первые два байта для определения типа программы */

r=_dos_read( f, &mz, 2, &q );

if ((r)||(q!=2)) {printf(” – ошибка чтения!”); _dos_close(f); return; }

/* Читаем сохраненные вирусом 24 байта старого начала */

lseek( f, ?80, SEEK_END );

r=_dos_read( f, buf, 24, &q );

if ((r)||(q!=24)) {printf(” – ошибка чтения!”); _dos_close(f); return; }

/* Определяем тип программы */

if ((mz==0x4D5A)||(mz==0x5A4D))

{ /* Это exe */

/* Пишем правильные PartPag и PageCnt */

lseek( f, 2, SEEK_SET );

r=_dos_write( f, &buf[2], 4, &q );

if ((r)||(q!=4)) {printf(” – ошибка записи!”); _dos_close(f); return; }

/* Пишем правильные ReloSS и ExeSP */

lseek( f, 14, SEEK_SET );

r=_dos_write( f, &buf[14], 4, &q );

if ((r)||(q!=4)) {printf(” – ошибка записи!”); _dos_close(f); return; }

/* Пишем правильные ReloCS и ExeIP */

lseek( f, 20, SEEK_SET );

r=_dos_write( f, &buf[20], 4, &q );

if ((r)||(q!=4)) {printf(” – ошибка записи!”); _dos_close(f); return; }

}

else

{ /* Это com */

/* Восстанавливаем сохраненные 3 первые байта программы */

lseek( f, 0, SEEK_SET);

r=_dos_write( f, &buf[0], 3, &q );

if ((r)||(q!=3)) {printf(” – ошибка записи!”); _dos_close(f); return; }

}

/* Усекаем файл (переходим на начало вируса

и записываем 0 байт) */

lseek( f, ?1740, SEEK_END);

r=_dos_write( f, buf, 0, &q);

/* Закрываем файл */

_dos_close(f);

printf(”теперь исцелен! ”);

return;
}
