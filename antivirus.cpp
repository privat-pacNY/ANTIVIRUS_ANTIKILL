/********************************************

ƒемонстрац≥йний антив≥рус? ‘аг

дл€ в≥русу SVC? 1740.

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

/* –€док ≥мен≥ поточного п≥дкаталогу */

path[PATH_LEN],

/* –€док ≥мен≥ початкового м≥сц€ розташуванн€ */

old_path[PATH_LEN],

/* –€док ≥мен≥ необх≥дного пристрою */

drive[DRIVE_LEN],

/* ѕорожн€ стр≥чка */

blank[BLANK_LEN];

int

/*  ≥льк≥сть в≥дсканованих каталог≥в */

n_dir,

/*  ≥льк≥сть досл≥джен≥х файл≥в */

n_fil,

/*  ≥льк≥сть хворих ≥ зц≥лених файл≥в */

n_ill;

int

/* ƒовжина ≥мен≥ файлу */

l,

/* тимчасовий ≥ндекс */

i;

#include Фantilib.cФ

/* –екурсивна процедура обходу дерева каталог≥в */

walk()

{

int found_d, found_f;

struct find_t buf;

/* ѕошук каталог≥в */

found_d=_dos_findfirst(Ф*.*Ф,_A_SUBDIR ,&buf);

while (found_d == F_FOUND)

{

if ((buf.name[0] != Ф.Ф) && (buf.attrib & _A_SUBDIR ))

{

chdir(buf.name);

walk();

chdir(Ф..Ф);

}

found_d=_dos_findnext( &buf );

}

/* ƒо цього моменту не в≥дсканованих нижчих каталог≥в

б≥льше не залишилос€ - скануЇмо файли */

n_dir++;

getcwd( path, PATH_LEN );

/* ѕошук файл≥в */

found_f=_dos_findfirst(Ф*.*Ф,_A_NORMAL ,&buf);

while (found_f == F_FOUND)

{

l=strlen( buf.name );

if (((buf.name[l?3]==ФCФ)&&

(buf.name[l?2]==ФOФ)&&

(buf.name[l?1]==ФMФ))||

((buf.name[l?3]==ФEФ)&&

(buf.name[l?2]==ФXФ)&&

(buf.name[l?1]==ФEФ)))

{

n_fil++;

printf(Ф%c%sФ,13,blank);

printf(Ф%c%s\%s Ф,13,path,buf.name);

/* «найшли новий файл - треба перев≥рити, чи ≥нф≥кований в≥н.

якщо заражений, то л≥куЇмо */

if (infected(buf.name)==BAD) cure(buf.name);

}

found_f=_dos_findnext( &buf );

}

}

main( int argc, char *argv[] )

{

puts(ФANTIKILL Ц демонстрац≥йний антив≥рус?фагФ);

if (argc < 2)

{ puts(Ф¬вед≥ть ≥м'€ диска в €кост≥ параметраФ); exit(2); }

if (((toupper(argv[1][0]))>ФZФ)||((toupper(argv[1][0]))<ФAФ))

{ puts(ФЌев≥рно задано ≥м'€ дискаФ); exit(3); }

drive[0]=argv[1][0]; drive[1]=Ф:Ф; drive[3]=ФТ;

for (i=0;i<BLANK_LEN;i++) blank[i]=Ф Ф;blank[BLANK_LEN?1]=ФФ;

n_dir=0; n_fil=0;

getcwd(old_path, PATH_LEN);

drive[2]=ФФ; system(drive);

drive[2]=Ф\Ф; chdir(drive);

/* «апускаЇмо рекурсивний обх≥д дерева каталог≥в

дл€ вибраного диска */

walk();

old_path[2]=Ф0Ф; system(old_path);

old_path[2]=Ф\Ф; chdir(old_path);

printf(Ф  аталог≥в:% d фото:% d ¬и€влено хворих

≥ вил≥кувано:% d ", n_dir, n_fil, n_ill);

if (n_ill) exit(1); else exit(0);

}

‘айл ЂANTILIB.Cї, включаЇтьс€ в попередн≥й:

/***************************************************************

ѕроцедури ви€вленн€ ≥ л≥куванн€

***************************************************************/

/* —игнатура */

char sign[7]={ (char) 0xB4,

(char) 0x83,

(char) 0xCD,

(char) 0x21,

(char) 0x5E,

(char) 0x56,

ФФ};

int infected( char *fn )

{

int f;

int r,q;

char buf[7]; /* Ѕуфер п≥д сигнатуру */

/* в≥дкриваЇмо файл */

r=_dos_open( fn, O_RDONLY, &f );

if (r) { printf(Ф Ц помилка в≥дкритт€!Ф); return GOOD; }

/* читаЇмо 6 байт */

lseek( f, ?1724, SEEK_END );

r=_dos_read( f, buf, 6, &q ); buf[6]=ФФ;

if ((r)||(q!=6)) {printf(Ф Ц помилка читанн€!Ф); _dos_close(f); return GOOD;

}

/* закриваЇмо файл */

_dos_close(f);

/* ѕор≥внюЇмо байти з сигнатурою */

if (strcmp( buf, sign)==0)

{ printf(Ф Ц був хворий ≥...Ф); n_ill++; return BAD; } /* ’ворий !!! */

/* ѕридатний до в / служб≥. ѕ / пк мед. служби ќрлов:?) */

return GOOD;

}

cure( char *fn )

{

int f;

int mz;

int r,q;

char buf[24]; /* Ѕуфер п≥д байти */

/* в≥дкриваЇмо файл */

r=_dos_open( fn, O_RDWR, &f );

if (r) { printf(Ф Ц помилка в≥дкритт€!Ф); return; }

/* „итаЇмо перш≥ два байта дл€ визначенн€ типу програми */

r=_dos_read( f, &mz, 2, &q );

if ((r)||(q!=2)) {printf(Ф Ц помилка читанн€!Ф); _dos_close(f); return; }

/* „итаЇмо збережен≥ в≥русом 24 байта старого початку */

lseek( f, ?80, SEEK_END );

r=_dos_read( f, buf, 24, &q );

if ((r)||(q!=24)) {printf(Ф Ц помилка читанн€!Ф); _dos_close(f); return; }

/* ¬изначаЇмо тип програми */

if ((mz==0x4D5A)||(mz==0x5A4D))

{ /* це exe */

/* пишемо правильн≥ PartPag ≥ PageCnt */

lseek( f, 2, SEEK_SET );

r=_dos_write( f, &buf[2], 4, &q );

if ((r)||(q!=4)) {printf(Ф Ц помилка запису!Ф); _dos_close(f); return; }

/* пишемо правильн≥ ReloSS ≥ ExeSP */

lseek( f, 14, SEEK_SET );

r=_dos_write( f, &buf[14], 4, &q );

if ((r)||(q!=4)) {printf(Ф Ц помилка запису!Ф); _dos_close(f); return; }

/* пишемо правильн≥ ReloCS ≥ ExeIP */

lseek( f, 20, SEEK_SET );

r=_dos_write( f, &buf[20], 4, &q );

if ((r)||(q!=4)) {printf(Ф Ц помилка запису!Ф); _dos_close(f); return; }

}

else

{ /* це com */

/* ¬≥дновлюЇмо збережен≥ 3 перш≥ байта програми */

lseek( f, 0, SEEK_SET);

r=_dos_write( f, &buf[0], 3, &q );

if ((r)||(q!=3)) {printf(Ф Ц помилка запису!Ф); _dos_close(f); return; }

}

/* ”с≥каЇтьс€ файл (переходимо на початок в≥русу

≥ записуЇмо 0 байт) */

lseek( f, ?1740, SEEK_END);

r=_dos_write( f, buf, 0, &q);

/* закриваЇмо файл */

_dos_close(f);

printf(Фтепер зц≥лений! Ф);

return;

}