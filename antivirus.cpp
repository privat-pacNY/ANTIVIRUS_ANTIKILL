/********************************************

Äåìîíñòðàö³éíèé àíòèâ³ðóñ? Ôàã

äëÿ â³ðóñó SVC? 1740.

********************************************/
*/ UTF-8

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

/* Ðÿäîê ³ìåí³ ïîòî÷íîãî ï³äêàòàëîãó */

path[PATH_LEN],

/* Ðÿäîê ³ìåí³ ïî÷àòêîâîãî ì³ñöÿ ðîçòàøóâàííÿ */

old_path[PATH_LEN],

/* Ðÿäîê ³ìåí³ íåîáõ³äíîãî ïðèñòðîþ */

drive[DRIVE_LEN],

/* Ïîðîæíÿ ñòð³÷êà */

blank[BLANK_LEN];

int

/* Ê³ëüê³ñòü â³äñêàíîâàíèõ êàòàëîã³â */

n_dir,

/* Ê³ëüê³ñòü äîñë³äæåí³õ ôàéë³â */

n_fil,

/* Ê³ëüê³ñòü õâîðèõ ³ çö³ëåíèõ ôàéë³â */

n_ill;

int

/* Äîâæèíà ³ìåí³ ôàéëó */

l,

/* òèì÷àñîâèé ³íäåêñ */

i;

#include ”antilib.c”

/* Ðåêóðñèâíà ïðîöåäóðà îáõîäó äåðåâà êàòàëîã³â */

walk()

{

int found_d, found_f;

struct find_t buf;

/* Ïîøóê êàòàëîã³â */

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

/* Äî öüîãî ìîìåíòó íå â³äñêàíîâàíèõ íèæ÷èõ êàòàëîã³â

á³ëüøå íå çàëèøèëîñÿ - ñêàíóºìî ôàéëè */

n_dir++;

getcwd( path, PATH_LEN );

/* Ïîøóê ôàéë³â */

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

/* Çíàéøëè íîâèé ôàéë - òðåáà ïåðåâ³ðèòè, ÷è ³íô³êîâàíèé â³í.

ßêùî çàðàæåíèé, òî ë³êóºìî */

if (infected(buf.name)==BAD) cure(buf.name);

}

found_f=_dos_findnext( &buf );

}

}

main( int argc, char *argv[] )

{

puts(”ANTIKILL – äåìîíñòðàö³éíèé àíòèâ³ðóñ?ôàã”);

if (argc < 2)

{ puts(”Ââåä³òü ³ì'ÿ äèñêà â ÿêîñò³ ïàðàìåòðà”); exit(2); }

if (((toupper(argv[1][0]))>”Z”)||((toupper(argv[1][0]))<”A”))

{ puts(”Íåâ³ðíî çàäàíî ³ì'ÿ äèñêà”); exit(3); }

drive[0]=argv[1][0]; drive[1]=”:”; drive[3]=”’;

for (i=0;i<BLANK_LEN;i++) blank[i]=” ”;blank[BLANK_LEN?1]=””;

n_dir=0; n_fil=0;

getcwd(old_path, PATH_LEN);

drive[2]=””; system(drive);

drive[2]=”\”; chdir(drive);

/* Çàïóñêàºìî ðåêóðñèâíèé îáõ³ä äåðåâà êàòàëîã³â

äëÿ âèáðàíîãî äèñêà */

walk();

old_path[2]=”0”; system(old_path);

old_path[2]=”\”; chdir(old_path);

printf(” Êàòàëîã³â:% d ôîòî:% d Âèÿâëåíî õâîðèõ

³ âèë³êóâàíî:% d ", n_dir, n_fil, n_ill);

if (n_ill) exit(1); else exit(0);

}

Ôàéë «ANTILIB.C», âêëþ÷àºòüñÿ â ïîïåðåäí³é:

/***************************************************************

Ïðîöåäóðè âèÿâëåííÿ ³ ë³êóâàííÿ

***************************************************************/

/* Ñèãíàòóðà */

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

char buf[7]; /* Áóôåð ï³ä ñèãíàòóðó */

/* â³äêðèâàºìî ôàéë */

r=_dos_open( fn, O_RDONLY, &f );

if (r) { printf(” – ïîìèëêà â³äêðèòòÿ!”); return GOOD; }

/* ÷èòàºìî 6 áàéò */

lseek( f, ?1724, SEEK_END );

r=_dos_read( f, buf, 6, &q ); buf[6]=””;

if ((r)||(q!=6)) {printf(” – ïîìèëêà ÷èòàííÿ!”); _dos_close(f); return GOOD;

}

/* çàêðèâàºìî ôàéë */

_dos_close(f);

/* Ïîð³âíþºìî áàéòè ç ñèãíàòóðîþ */

if (strcmp( buf, sign)==0)

{ printf(” – áóâ õâîðèé ³...”); n_ill++; return BAD; } /* Õâîðèé !!! */

/* Ïðèäàòíèé äî â / ñëóæá³. Ï / ïê ìåä. ñëóæáè Îðëîâ:?) */

return GOOD;

}

cure( char *fn )

{

int f;

int mz;

int r,q;

char buf[24]; /* Áóôåð ï³ä áàéòè */

/* â³äêðèâàºìî ôàéë */

r=_dos_open( fn, O_RDWR, &f );

if (r) { printf(” – ïîìèëêà â³äêðèòòÿ!”); return; }

/* ×èòàºìî ïåðø³ äâà áàéòà äëÿ âèçíà÷åííÿ òèïó ïðîãðàìè */

r=_dos_read( f, &mz, 2, &q );

if ((r)||(q!=2)) {printf(” – ïîìèëêà ÷èòàííÿ!”); _dos_close(f); return; }

/* ×èòàºìî çáåðåæåí³ â³ðóñîì 24 áàéòà ñòàðîãî ïî÷àòêó */

lseek( f, ?80, SEEK_END );

r=_dos_read( f, buf, 24, &q );

if ((r)||(q!=24)) {printf(” – ïîìèëêà ÷èòàííÿ!”); _dos_close(f); return; }

/* Âèçíà÷àºìî òèï ïðîãðàìè */

if ((mz==0x4D5A)||(mz==0x5A4D))

{ /* öå exe */

/* ïèøåìî ïðàâèëüí³ PartPag ³ PageCnt */

lseek( f, 2, SEEK_SET );

r=_dos_write( f, &buf[2], 4, &q );

if ((r)||(q!=4)) {printf(” – ïîìèëêà çàïèñó!”); _dos_close(f); return; }

/* ïèøåìî ïðàâèëüí³ ReloSS ³ ExeSP */

lseek( f, 14, SEEK_SET );

r=_dos_write( f, &buf[14], 4, &q );

if ((r)||(q!=4)) {printf(” – ïîìèëêà çàïèñó!”); _dos_close(f); return; }

/* ïèøåìî ïðàâèëüí³ ReloCS ³ ExeIP */

lseek( f, 20, SEEK_SET );

r=_dos_write( f, &buf[20], 4, &q );

if ((r)||(q!=4)) {printf(” – ïîìèëêà çàïèñó!”); _dos_close(f); return; }

}

else

{ /* öå com */

/* Â³äíîâëþºìî çáåðåæåí³ 3 ïåðø³ áàéòà ïðîãðàìè */

lseek( f, 0, SEEK_SET);

r=_dos_write( f, &buf[0], 3, &q );

if ((r)||(q!=3)) {printf(” – ïîìèëêà çàïèñó!”); _dos_close(f); return; }

}

/* Óñ³êàºòüñÿ ôàéë (ïåðåõîäèìî íà ïî÷àòîê â³ðóñó

³ çàïèñóºìî 0 áàéò) */

lseek( f, ?1740, SEEK_END);

r=_dos_write( f, buf, 0, &q);

/* çàêðèâàºìî ôàéë */

_dos_close(f);

printf(”òåïåð çö³ëåíèé! ”);

return;

}
