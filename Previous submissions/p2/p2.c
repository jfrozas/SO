/*
 * SO LabAssignment2
 *
 * AUTHOR 1: Guillermo Fernández Sánchez | login: guillermo.fernandezs
 * AUTHOR 2: Javier Fernández Rozas      | login: j.frozas
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <pwd.h>
#include <errno.h>
#include <grp.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include "list.h"
#include "memlist.h"

#define MAXLINEA 1024
#define TAMANO 4096
#define NAMEMAX 32
#define st_atime st_atim.tv_sec
#define st_mtime st_mtim.tv_sec
#define LEERCOMPLETO ((ssize_t)-1)

/*GLOBAL VARIABLES*/
tList list;
bool iscmd;
memList memlist;

/*global variables for memoria command*/
int g1=1,g2=2,g3=3;

void ProcesarEntrada(char *tr[]);
int trocearcadena(char * cadena, char * trozos[]);
void printDir(bool, bool , bool , bool , int , char* );

void cmd_autores(char **);
void cmd_carpeta(char **);
void cmd_pid(char **);
void cmd_fin();
void cmd_fecha(char **);
void cmd_hist(char **);
void cmd_comando(char **);
void cmd_infosis();
void cmd_ayuda(char **);
void cmd_crear(char **);
void cmd_borrar(char **);
void cmd_borrarrec(char **);
void cmd_listfich(char **);
void cmd_listdir(char **);
void cmd_malloc(char **);
void cmd_mmap(char **);
void cmd_shared(char **);
void cmd_dealloc(char **);
void cmd_memoria(char **);
void cmd_volcarmem(char **);
void cmd_llenarmem(char **);
void cmd_recursiva(char **);
void cmd_e_s(char **);


struct CMD{
    char * cmdname;
    void (*func)(char **);
};


struct CMD C[]={
        {"fin", cmd_fin},
        {"bye", cmd_fin},
        {"salir", cmd_fin},
        {"carpeta", cmd_carpeta},
        {"autores", cmd_autores},
        {"pid", cmd_pid},
        {"fecha", cmd_fecha},
        {"hist", cmd_hist},
        {"comando", cmd_comando},
        {"infosis", cmd_infosis},
        {"ayuda", cmd_ayuda},
        {"crear", cmd_crear},
        {"borrar", cmd_borrar},
        {"borrarrec", cmd_borrarrec},
        {"listfich", cmd_listfich},
        {"listdir", cmd_listdir},
        {"malloc", cmd_malloc},
        {"mmap", cmd_mmap},
        {"shared", cmd_shared},
        {"dealloc", cmd_dealloc},
        {"memoria", cmd_memoria},
        {"volcarmem", cmd_volcarmem},
        {"llenarmem", cmd_llenarmem},
        {"recursiva", cmd_recursiva},
        {"e-s", cmd_e_s},
        {NULL ,NULL}
};


void cmd_autores (char *tr[]){
/*prints the authors logins and/or names depending on the command's argument*/
    if (tr[0]==NULL){
        printf("The authors are:\nGuillermo Fernandez: guillermo.fernandezs\nJavier Fernandez: j.frozas\n");
    } else if(!strcmp(tr[0],"-l")){
        printf("The authors are:\nguillermo.fernandezs\nj.frozas\n");
    } else if(!strcmp(tr[0],"-n")){
        printf("The authors are:\nGuillermo Fernandez\nJavier Fernandez\n");
    } else printf("Command not found\n");
}


void cmd_pid(char *tr[]){
/*prints the process id of the current shell process or the id of the process' parent*/
    if (tr[0]== NULL) {
        printf("Shell process pid: %d\n",getpid());
    } else if(!strcmp(tr[0],"-p")){
        printf("Shell parent process pid: %d\n",getppid());
    }	else{
        printf("Command not found\n");
    }
}


void cmd_carpeta(char *tr[]){
/*prints the current directory of the shell*/
    char dir[MAXLINEA];

    if(tr[0]== NULL)
        printf("%s\n", getcwd(dir, MAXLINEA));
    else{
        if(chdir(tr[0]) == -1){
            printf("Cannot change dir %s: %s\n", tr[0], strerror(errno));
        }
    }
}


void cmd_fecha(char *tr[]){
/*prints the system time/date*/

    time_t t;
    t = time(NULL);
    struct tm tm;
    tm = *localtime(&t);

    if(tr[0]==NULL){
        printf("%02d/%02d/%d %02d:%02d:%02d\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    } else if(!strcmp(tr[0],"-d")){
        printf("%02d/%02d/%02d\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    } else if(!strcmp(tr[0],"-h")){
        printf("%02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
    }
}


void cmd_infosis() {
/*prints the system information*/
    struct utsname infosis;
    uname(&infosis);

    printf("%s (%s), OS: %s-%s-%s\n",infosis.nodename,infosis.machine,infosis.sysname,infosis.release, infosis.version);
}


void cmd_ayuda(char *tr[]){
/*prints help for different commands*/
    if(tr[0]==NULL) printf("Avaiable commands:\n-> autores\n-> ayuda\n-> borrar\n-> borrarrec\n-> bye\n-> carpeta\n-> comando\n-> crear\n-> dealloc\n-> e-s\n-> fecha\n-> fin\n-> hist\n-> infosis\n-> listdir\n-> listfich\n-> llenarmem\n-> malloc\n-> memoria\n-> mmap\n-> pid\n-> recursiva\n-> salir\n-> shared\n-> volcarmem\n");
    else if(!strcmp(tr[0],"autores")) printf("autores [-l|-n]: Prints the names and logins of the program authors.\n");
    else if(!strcmp(tr[0],"pid")) printf("pid [-p]: Prints the pid of the process or parent process of the shell.\n");
    else if(!strcmp(tr[0],"carpeta")) printf("carpeta [dir]: Changes (or shows) the current working directory of the shell to direct.\n");
    else if(!strcmp(tr[0],"fecha")) printf("fecha [-d] [-h]: Prints the current date and/or the current time.\n");
    else if(!strcmp(tr[0],"hist")) printf("hist [-c][-N]: Shows the historic of commands executed by this shell in order. hist -c clears the historic.\n");
    else if(!strcmp(tr[0],"comando")) printf("comando N: Repeats command number N.\n");
    else if(!strcmp(tr[0],"infosis")) printf("infosis: Prints information on the machine running the shell.\n");
    else if(!strcmp(tr[0],"ayuda")) printf("ayuda [cmd]: ayuda displays a list of available commands.\n");
    else if(!strcmp(tr[0],"fin")) printf("fin: Ends the shell.\n");
    else if(!strcmp(tr[0],"bye")) printf("bye: Ends the shell.\n");
    else if(!strcmp(tr[0],"salir")) printf("salir: Ends the shell.\n");
    else if(!strcmp(tr[0],"crear")) printf("crear [-f] [name]: Creates a directory or file in the file system.\n");
    else if(!strcmp(tr[0],"borrar")) printf("borrar name1 name2...: Deletes files and/or empty directories.\n");
    else if(!strcmp(tr[0],"borrarrec")) printf("borrarrec name1 name2...: Deletes files and/or non empty directories. If the directory is not empty it is deleted with all its contents.\n");
    else if(!strcmp(tr[0],"listfich"))printf("listfich [-long] [-link] [-acc] name1 name2 name3...: Gives info on files or directories entered.\n");
    else if(!strcmp(tr[0],"listdir")) printf("listdir [-reca] [-recb] [-hid] [-long] [-link] [-acc] name1 name2...: Lists the contents of directories with names name1, name2...\n");
    else if(!strcmp(tr[0],"malloc")) printf("malloc [-free] tam: The shell allocates (or deallocates) tam bytes of memory in the program.\n");
    else if(!strcmp(tr[0],"mmap ")) printf("mmap [-free] fich perm : maps (or unmaps) files in the directory space of the process.\n");
    else if(!strcmp(tr[0],"shared")) printf("shared [-free|-create|-delkey ] cl tam: Allocates(or deallocates shared memory in the program\n");
    else if(!strcmp(tr[0],"dealloc")) printf("dealloc [-malloc|-shared|-mmap]: deallocates one of the memory blocks allocated with the command malloc, mmap or shared\n");
    else if(!strcmp(tr[0],"memoria")) printf("memoria [-blocks] [-vars] [-funcs] [-all] [-pmap]: Shows details of the process memory.\n");
    else if(!strcmp(tr[0],"volcarmem")) printf("volcarmem addr cont: dumps in screen the contents (cont bytes) of the memory position addr.\n");
    else if(!strcmp(tr[0],"llenarmem")) printf("llenarmem addr cont byte: Fills cont bytes of memory starting at address addr with the value byte\n");
    else if(!strcmp(tr[0],"recursiva")) printf("recursiva n: calls the recursive function n times\n");
    else if(!strcmp(tr[0],"e-s")){
        printf("      read fich addr cont: reads cont bytes from file fich into memory address addr\n");
        printf("write [-o] fich addr cont: writes cont bytes frommemory address addr into file fich\n");
    }
    else printf("%s is not a command\n",tr[0]);
}


void cmd_fin() {
/*exits the shell*/
    exit(0);
}


void cmd_hist(char *tr[]){
/*prints the list of the previous commands*/
    LPos pos;
    tItemL item;

    if(tr[0]!=NULL && !strcmp(tr[0],"-c")){
        clearList(&list);
        iscmd=false;
    } else{

        int i=1, n;
        if(tr[0]==NULL){
            n=-MAXLINEA;
        } else n= strtol(tr[0], NULL, 10);

        if(!isEmptyList(list) && n<0){

            pos= first(list);
            while (pos!=LNULL && n<=-i){
                item  = getItem(pos, list);
                printf("%d->", i);
                puts(item.cmdline);
                pos = next(pos, list);
                i++;
            }
        } else printf("The historic is empty or number of commands to print is 0\n");
    }
}

void cmd_comando(char *nchar[]){
/*Repeats the command indicated*/

    int i, N = strtol(nchar[0], NULL, 10);
    if((N==0) | isEmptyList(list)){
        printf("command number not valid\n");
    } else{

        char linea[MAXLINEA];
        char *tr[MAXLINEA / 2];

        tItemL item;
        LPos pos;

        pos= first(list);

        for(i=1; i<N;i++){

            if(next(pos, list)!=NULL){
                pos= next(pos, list);
            } else {
                printf("command number not valid\n");
                pos=NULL;
                break;
            }

        }
        if(pos!=NULL){
            item = getItem(pos, list);
            printf("Repeating command %d:",i);
            puts(item.cmdline);

            strcpy(linea, item.cmdline);
            trocearcadena(linea, tr);
            ProcesarEntrada(tr);
        }
    }
    iscmd=false;
}


void cmd_crear(char *tr[]){
/*creates a file or a directory with name specified*/
    int f;

    if(tr[0]==NULL) cmd_carpeta(tr);//if no name is inserted, the current directory is printed
    else if(!strcmp(tr[0],"-f")){
        if (tr[1]==NULL) cmd_carpeta(tr+1);//if no name is inserted, the current directory is printed
        else{
            if ((f = open(tr[1],O_CREAT | O_EXCL, 0664))==-1){//permissions -rw-rw-r--
                printf("Unable to create file %s: %s\n",tr[1],strerror(errno));
            }
            close(f);
        }
    }
    else {
        if(mkdir(tr[0],0775)==-1){ //permissions drwxrwxr-x
            printf("Unable to create directory %s: %s\n",tr[0],strerror(errno));
        }
    }
}


bool isDir(const char *path){
/*aux function to check if a path corresponds to a directory*/
    struct stat str;
    if(lstat(path, &str)!=-1) {
        if(S_ISDIR(str.st_mode))
            return true;
        else return false;
    }
    else return false;


}


void cmd_borrar(char *tr[]){
/*Deletes an element*/
    int i=0;

    if (tr[i]==NULL) {
        cmd_carpeta(tr);
        return;
    }

    while (tr[i]!=NULL){

        if (isDir(tr[i])){
            if (rmdir(tr[i])==-1) printf("Unable to delete %s: %s\n", tr[i], strerror(errno));
        } else{
            if (unlink(tr[i])==-1) printf("Unable to delete %s: %s\n", tr[i], strerror(errno));
        }
        i++;
    }
}


void deleteDir(const char *path){
/*
 * function deletes recursively a directory with all its files
 * */
    int i=0;
    DIR *d;
    struct dirent *dirStruct;
    char path2[MAXLINEA];

    d = opendir(path);

    if (d) {
        while ((dirStruct = readdir(d)) != NULL) {
            if (strcmp(dirStruct->d_name,".")!=0 && strcmp(dirStruct->d_name,"..")!=0){

                sprintf(path2,"%s/%s",path, dirStruct->d_name);
                i++;
                if (isDir(path2)){
                    deleteDir(path2);
                }else{
                    if (unlink(path2)==-1) printf("Unable to delete %s: %s",path2, strerror(errno));
                }
            }
        }
        closedir(d);
    }

    if(rmdir(path)==-1){
        printf("Unable to delete %s: %s\n",path, strerror(errno));
    }

}


void cmd_borrarrec(char *tr[]){
/*Function to delete a file/directory, if its a non-empty directory, calls deleteDir to delete it*/

    int i=0;

    if (tr[i]==NULL) {
        cmd_carpeta(tr);
        return;
    }

    while (tr[i]!=NULL){

        if(isDir(tr[i])){
            deleteDir(tr[i]);
        } else if (unlink(tr[i])==-1) printf("Unable to delete %s: %s\n",tr[i], strerror(errno));

        i++;
    }
}

char LetraTF (mode_t m){
/*Auxiliar function to ConvierteModo*/
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato, 0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* regular file*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directory */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?';/*unknown (shouldn't appear)*/
    }
}


char * ConvierteModo (mode_t m){
/*Auxiliar function to return the permisions of a file passing its mode_t as argument*/

    static char permisos2[12];
    strcpy (permisos2,"---------- ");
    permisos2[0]=LetraTF(m);
    if (m&S_IRUSR) permisos2[1]='r'; /*owner*/
    if (m&S_IWUSR) permisos2[2]='w';
    if (m&S_IXUSR) permisos2[3]='x';
    if (m&S_IRGRP) permisos2[4]='r'; /*group*/
    if (m&S_IWGRP) permisos2[5]='w';
    if (m&S_IXGRP) permisos2[6]='x';
    if (m&S_IROTH) permisos2[7]='r'; /*rest*/
    if (m&S_IWOTH) permisos2[8]='w';
    if (m&S_IXOTH) permisos2[9]='x';
    if (m&S_ISUID) permisos2[3]='s'; /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos2[6]='s';
    if (m&S_ISVTX) permisos2[9]='t';
    return (permisos2);
}

void printTimeFormat(time_t t, int i){
    struct tm tm;
    tm = *localtime(&t);
    if (i==1){
        printf("%04d/%02d/%02d-%02d:%02d ",tm.tm_year+1900,tm.tm_mon+1, tm.tm_mday,  tm.tm_hour, tm.tm_min);
    } else if (i==2){
        printf("%s", ctime(&t));
    }
}


void printFile(bool longListing, bool link, bool acc, char* name){
/*prints the information of a file depending on the options specified*/

    struct stat fileData;
    struct passwd *userInfo;
    struct group *groupInfo;

    char linkName[MAXLINEA]="", userName[NAMEMAX], groupName[NAMEMAX];

    if (!lstat(name, &fileData)){

        if(longListing){
            if(acc){
                printTimeFormat(fileData.st_atime,1);
            } else {
                printTimeFormat(fileData.st_mtime,1);
            }

            if((userInfo = getpwuid(fileData.st_uid))!=NULL) sprintf(userName,"%s",userInfo->pw_name);
            else sprintf(userName,"%d",fileData.st_uid);

            if((groupInfo = getgrgid(fileData.st_gid))!=NULL) sprintf(groupName,"%s",groupInfo->gr_name);
            else sprintf(groupName,"%d",fileData.st_gid);

            printf("%2lu (%lu)",fileData.st_nlink, fileData.st_ino);
            printf(" %8s %8s ",userName, groupName);
            printf("%s ", ConvierteModo(fileData.st_mode));
        }

        printf("%9ld %s",fileData.st_size, basename(name));

        if(link && longListing && S_ISLNK(fileData.st_mode)){

            if ((readlink(name, linkName, sizeof(linkName)-1)) != -1){
                printf("->%s", basename(linkName));
            } else
                printf("Cannot access link: %s", strerror(errno));
        }

        printf("\n");
    } else printf("cannot access %s: %s\n",name,strerror(errno));
}


void cmd_listfich(char *tr[]){
/*function to perform the listfich comand*/

    bool longListing=false, link=false, acc=false;
    int i,names=0;

    for (i = 0; i < 3; ++i) {

        if(tr[i]==NULL){
            cmd_carpeta(tr+i);
            return;
        }
        else if(!strcmp(tr[i],"-long")){
            longListing=true;
        } else if (!strcmp(tr[i],"-link")){
            link=true;
        } else if (!strcmp(tr[i],"-acc")){
            acc=true;
        } else{
            break;
        }
    }

    while (tr[i]!=NULL){
        printFile(longListing, link, acc, tr[i]);
        i++;
        names++;
    }

    if (names==0) {
        cmd_carpeta(tr+i);
    }
}


void printSubDirs(bool longlisting, bool link, bool acc, bool hid, int rec, char* path){
/*Auxiliar function to printDir*/

    DIR *d2;
    struct dirent *dirStruct2;
    char path2[MAXLINEA];

    d2 = opendir(path);

    if (d2) {
        while ((dirStruct2 = readdir(d2)) != NULL) {

            sprintf(path2,"%s/%s",path,dirStruct2->d_name);

            if((strcmp(dirStruct2->d_name,".")!=0 && strcmp(dirStruct2->d_name,"..")!=0) && (isDir(path2)) && (hid || dirStruct2->d_name[0]!='.')){
                printDir(longlisting, link, acc,hid,rec, path2);
            }
        }
        closedir(d2);
    }
}


void printDir(bool longlisting, bool link, bool acc, bool hid, int rec, char* path){
/*Function to print the contents from a directory, auxiliar to cmd_listdir*/

    DIR *d;
    struct dirent *dirStruct;
    char path2[MAXLINEA];

    d = opendir(path);

    if (d) {
        if(rec==2){
            printSubDirs(longlisting, link, acc, hid, rec, path);
        }

        printf("************%s\n", path);
        while ((dirStruct = readdir(d)) != NULL) {
            if (hid || dirStruct->d_name[0]!='.'){
                sprintf(path2,"%s/%s",path,dirStruct->d_name);
                printFile(longlisting, link, acc, path2);
            }
        }

        if(rec==1){
            printSubDirs(longlisting, link, acc, hid, rec, path);
        }
    }
    closedir(d);
}


void cmd_listdir(char *tr[]){
/*function to perform the listdir command*/

    bool longListing=false, link=false, acc=false, hid=false;
    int i,names=0,rec=0;
    /*rec=0 if no recursion
     *rec=1 if reca
     *rec=2 if recb
     */

    for (i = 0; i < 5; ++i) {

        if(tr[i]==NULL){
            cmd_carpeta(tr+i);
            return;
        }
        else if(!strcmp(tr[i],"-long") && !longListing){
            longListing=true;
        } else if (!strcmp(tr[i],"-link") && !link){
            link=true;
        } else if (!strcmp(tr[i],"-acc") && !acc){
            acc=true;
        } else if (!strcmp(tr[i],"-hid") && !hid){
            hid=true;
        } else if (!strcmp(tr[i],"-reca") && rec==0){
            rec=1;
        } else if (!strcmp(tr[i],"-recb") && rec==0){
            rec=2;
        } else{
            break;
        }
    }
    while (tr[i]!=NULL){

        if(!isDir(tr[i]))
            printFile(longListing,link,acc,tr[i]);
        else{
            printDir(longListing, link, acc, hid, rec, tr[i]);
        }

        i++;
        names++;
    }

    if (names==0) {
        cmd_carpeta(tr+i);
    }
}

/* LAB ASSIGNMENT 2 FUNCTIONS*/

//Malloc functions and auxiliary functions
void mallocPrint(){
/*prints the list of blocks allocated with malloc*/
    tItemMem item;
    memPos p = memFirst(memlist);

    while (p!=NULL){

        item= getMemItem(p,memlist);
        if (item.type=='1'){
            printf("%p: size:%zu. malloc ",item.address,item.size);
            printTimeFormat(item.time,2);
        }

        p= memNext(p,memlist);
    }
}

void mallocFree(char *tr[]){
/*frees a block of memory allocated with malloc*/
    memPos p;
    tItemMem it;
    size_t size;
    bool deleted = false;

    if (tr[0]==NULL) { //no arguments: cannot free, prints the list of allocated blocks
        printf("******Lista de bloques asignados malloc para el proceso %d\n",getpid());
        mallocPrint();
        return;
    }

    size = strtol(tr[0], NULL, 10);
    p= memFirst(memlist);
    while (p!=NULL){

        it= getMemItem(p,memlist);
        if (it.type=='1'){
            if (it.size==size){
                printf("deallocated %zu bytes at %p\n",size,it.address);
                free(it.address); //address is freed
                deleteAtMemPosition(p,&memlist); //item is deleted from the list
                deleted=true;
                break;
            }
        }
        p = memNext(p,memlist);
    }
    if(!deleted){ //if no block allocated with that size, prints the list
        printf("******Lista de bloques asignados malloc para el proceso %d\n",getpid());
        mallocPrint();
    }

}

void cmd_malloc(char *tr[]){
/* The shell allocates bytes with malloc and shows the memory address returned by malloc.*/
    size_t size;
    tItemMem item;

    void *address;

    if(tr[0]==NULL){ // If no arguments are given, the list of blocks is printed
        printf("******Lista de bloques asignados malloc para el proceso %d\n",getpid());
        mallocPrint();
    } else if (!strcmp(tr[0],"-free")){
        mallocFree(tr+1);
    } else{
        size = strtol(tr[0], NULL, 10);
        if (size==0) {
            printf("No se asignan bloques de 0 bytes\n");
            return;
        }

        if ( (address = malloc(size)) == NULL )
            printf("Imposible obtener memoria con malloc: %s\n", strerror(errno));
        else{
            printf("allocated %zu at %p\n",size,address);

            item.address=address;
            item.time=time(NULL);
            item.type='1';
            item.size=size;

            insertMemItem(item,&memlist);
        }
    }
}


//Mmap functions and auxiliary functions
void mMapPrint(){
/*Prints the list of mapped files*/
    tItemMem item;
    memPos p = memFirst(memlist);

    while (p!=NULL){

        item= getMemItem(p,memlist);
        if (item.type=='2'){
            printf("%p: size:%zu. mmap %s (fd:%d) ",item.address,item.size,item.data.fileName,item.data.fd);
            printTimeFormat(item.time,2);

        }

        p= memNext(p,memlist);
    }
}

void * MmapFile (char * fichero, int protection){
/*auxiliary function to cmd_mmap which maps a file*/

    int df, map=MAP_PRIVATE, modo=O_RDONLY;
    struct stat s;
    void *p;

    tItemMem item;
    if (protection&PROT_WRITE) modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
        return NULL;
    if ( (p=mmap(NULL,s.st_size, protection,map,df,0) )==MAP_FAILED)
        return NULL;

    /*the item is created and inserted into the list*/
    item.address=p;
    item.size=s.st_size;
    item.type='2';
    item.time=time(NULL);
    item.data.fd=df;
    strcpy(item.data.fileName,fichero);

    insertMemItem(item,&memlist);

    return p; //returns the address where the file was mapped
}

void mmapFree(char *tr[]){
/*unmaps a file*/
    tItemMem item;
    memPos pos;
    bool deleted = false;

    if (tr[0]==NULL){
        printf("******Lista de bloques asignados mmap para el proceso %d\n",getpid());
        mMapPrint();
        return;
    }
    pos= memFirst(memlist);
    while (pos!=NULL){

        item= getMemItem(pos,memlist);
        if (item.type=='2'){
            if (!strcmp(item.data.fileName,tr[0])){

                if (munmap(item.address,item.size)==-1){
                    printf("Cannot unmap: %s", strerror(errno));
                }
                close(item.data.fd); //file descriptor is closed

                deleteAtMemPosition(pos,&memlist);//element is deleted from the list
                deleted=true;
                break;
            }
        }
        pos = memNext(pos,memlist);
    }
    if(!deleted){
        mMapPrint();
        printf("******Lista de bloques asignados mmap para el proceso %d\n",getpid());
    }

}

void cmd_mmap (char *tr[]){
/*Function to perform the mmap command*/

    char *perm;
    void *p;
    int protection=0;


    if (tr[0]==NULL){
        printf("******Lista de bloques asignados mmap para el proceso %d\n",getpid());
        mMapPrint();
        return;
    }
    if  (!strcmp(tr[0],"-free")){
        mmapFree(tr+1);
        return;
    }

    if ((perm=tr[1])!=NULL && strlen(perm)<4) { //gets permisions from argument
        if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
        if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
        if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
    }
    if ((p=MmapFile(tr[0],protection))==NULL)
        printf("Impossible to map file: %s\n", strerror(errno));
    else{
        printf ("file %s mapped in %p\n", tr[0], p);
    }
}

//Shared functions and auxiliary functions
void sharedPrint(){
/*prints the list of blocks allocated with shared*/

    tItemMem item;
    memPos p = memFirst(memlist);

    while (p!=NULL){

        item= getMemItem(p,memlist);
        if (item.type=='4'){
            printf("%p: size:%zu. shared memory (key:%d) ",item.address,item.size,item.data.key);
            printTimeFormat(item.time,2);
        }
        p= memNext(p,memlist);
    }
}


void * ObtainMemShmget (key_t key, size_t size){
/*Obtains a pointer to a shared memory space*/
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;
    tItemMem item;

    if (size)
        flags=flags | IPC_CREAT | IPC_EXCL;
    /*If size is 0, it is assumed that the key exists, if size>0 it tries to create it*/

    if (key==IPC_PRIVATE){ /*If private key, we cannot get that shared memory space*/
        errno=EINVAL;
        return NULL;
    }

    if ((id=shmget(key, size, flags))==-1)
        return NULL;
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno; /*if created but unable to map, it is deleted*/
        if (size)
            shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return NULL;
    }
    shmctl (id,IPC_STAT,&s);

    /*item is created and inserted into the list*/
    item.address=p;
    item.type='4';
    item.size=s.shm_segsz;
    item.time=s.shm_atime;
    item.data.key=key;

    insertMemItem(item,&memlist);

    return (p);
}


void SharedCreate (char *tr[]){
/*Function to create/get a shared memory address*/
    key_t k;
    size_t tam;
    void *p;

    if (tr[0]==NULL || tr[1]==NULL){ //if no arguments, the list of blocks is printed
        printf("******Lista de bloques asignados shared para el proceso %d\n",getpid());
        sharedPrint();
        return;
    }
    k=(key_t) atoi(tr[0]);
    tam=(size_t) atoll(tr[1]);

    if ((p=ObtainMemShmget(k,tam))==NULL)
        printf("Impossible to get shmget memory: %s\n", strerror(errno));
    else{
        printf ("Allocated shared memory (key %d) at %p\n",k,p);
    }
}

void sharedFree(char *tr[]){
/*Detaches the shared memory block with key specified*/
    tItemMem item;
    memPos pos;
    bool deleted = false;
    key_t k;

    if (tr[0]==NULL){
        printf("******Lista de bloques asignados shared para el proceso %d\n",getpid());
        sharedPrint();
        return;
    }
    k=(key_t) atoi(tr[0]);


    pos= memFirst(memlist);
    while (pos!=NULL){

        item= getMemItem(pos,memlist);
        if (item.type=='4'){
            if (item.data.key==k){
                if(shmdt(item.address)==-1){
                    printf("Cannot detach: %s", strerror(errno));
                }
                deleteAtMemPosition(pos,&memlist);
                deleted=true;
                break;
            }
        }
        pos = memNext(pos,memlist);
    }
    if(!deleted){
        printf("******Lista de bloques asignados shared para el proceso %d\n",getpid());
        sharedPrint();
    }
}

void sharedDeletekey(char *tr[]){
/*Removes the shared memory region specified*/
    key_t k;
    int id, flags=0777;

    if (tr[0]==NULL){//if no key given, prints the list of blocks
        printf("******Lista de bloques asignados shared para el proceso %d\n",getpid());
        sharedPrint();
        return;
    }
    k=(key_t) atoi(tr[0]);

    if ((id=shmget(k, 0, flags))==-1){
        printf("Cannot get shared memory of key %d: %s",k,strerror(errno));
        return;
    }
    shmctl(id,IPC_RMID,NULL);
}


void cmd_shared(char *tr[]){
/*gets shared memory of key cl, maps it in the process address space and shows the memory address*/

    if (tr[0]==NULL){
        printf("******Lista de bloques asignados shared para el proceso %d\n",getpid());
        sharedPrint();
        return;
    }
    if (!strcmp(tr[0],"-create")){

        if(tr[2]!=NULL && !strcmp(tr[2],"0"))
            printf("No se asignan bloques de 0 bytes\n");
        else
            SharedCreate(tr+1);

    } else if (!strcmp(tr[0],"-free")){
        sharedFree(tr+1);
    } else if (!strcmp(tr[0],"-delkey")){
        sharedDeletekey(tr+1);
    } else {
        tr[1]="0";
        SharedCreate(tr);
    }

}

void allBlocksPrint(){
//prints all blocks
    mallocPrint();
    mMapPrint();
    sharedPrint();
}

//Dealloc command function and auxiliary functions

bool deallocAddress(char *tr[]){
/* Dealloc a block by getting its address
 * returns true if an address is deleted, false otherwise
 * */
    memPos pos;
    tItemMem item;

    pos= memFirst(memlist);
    void* address;

    address = (void*) strtol(tr[0],NULL,16);

    while (pos!=NULL){

        item= getMemItem(pos,memlist);
        if(address!=item.address){
            pos = memNext(pos,memlist);
            continue;
        }

        printf("block at address %p deallocated ",item.address);
        switch (item.type) {
            case '1': //malloc
                printf("(malloc)\n");
                free(item.address);
                deleteAtMemPosition(pos,&memlist);
                break;
            case '2': //mmap
                printf("(mmap)\n");
                munmap(item.address,item.size);
                close(item.data.fd);
                deleteAtMemPosition(pos,&memlist);
                break;
            case '4':
                printf("(shared)\n");
                shmdt(item.address);
                deleteAtMemPosition(pos,&memlist);
                break;
            default: //default option (shouldn't occur)
                printf("Error\n");
                break;
        }
        return true;
    }
    return false;
}

void cmd_dealloc(char *tr[]){
/*Function to perform the dealloc command*/

    if (tr[0]==NULL){
        printf("******List of blocks assigned for the process %d\n",getpid());
        allBlocksPrint();
    } else if (!strcmp(tr[0],"-malloc")){
        mallocFree(tr+1);
    } else if (!strcmp(tr[0],"-mmap")){
        mmapFree(tr+1);
    } else if (!strcmp(tr[0],"-shared")){
        sharedFree(tr+1);
    } else{
        if (!deallocAddress(tr)){ //if no element deleted, prints the list
            printf("******List of blocks assigned for the process %d\n",getpid());
            allBlocksPrint();
        }
    }
}


//Function memoria and auxiliary functions
void dopmap (void) {
/*Calls the program pmap for the shell process*/

    pid_t pid;
    char elpid[32];
    char *argv[3]= {"pmap",elpid,NULL};

    sprintf (elpid,"%d", (int) getpid());

    if ((pid=fork())==-1){
        printf("Cannot create process: %s",strerror(errno));
        return;
    }
    if (pid==0){
        if (execvp(argv[0],argv)==-1)
            printf("cannot execute pmap: %s",strerror(errno));
        exit(1);
    }
    waitpid (pid,NULL,0);
}

void cmd_memoria(char *tr[]){
/*Function to perform the memoria command*/

    int l1=1,l2=2,l3=3; //local initialized variables for the memoria -vars command
    static int s1=1, s2=2, s3=3; //static initialized variables for the memoria -vars command

    if((tr[0]==NULL || !strcmp(tr[0],"-all")) || !strcmp(tr[0],"-vars")){
        printf("Local  Variables\t%p,\t%p,\t%p\n",&l1,&l2,&l3);
        printf("Global Variables\t%p,\t%p,\t%p\n",&g1,&g2,&g3);
        printf("Static Variables\t%p,\t%p,\t%p\n",&s1,&s2,&s3);
    }
    if((tr[0]==NULL || !strcmp(tr[0],"-all")) || !strcmp(tr[0],"-funcs")){
        printf("Program functions\t%p,\t%p,\t%p\n", cmd_autores, cmd_carpeta, cmd_pid);
        printf("Library functions\t%p,\t%p,\t%p\n", printf,strcpy, strcmp);
    }
    if((tr[0]==NULL || !strcmp(tr[0],"-all")) || !strcmp(tr[0],"-blocks")){
        if(!isEmptyMemList(memlist))
            printf("******List of blocks assigned for the process %d\n",getpid());
        allBlocksPrint();
    }

    if(tr[0]!=NULL && !strcmp(tr[0],"-pmap")){
        dopmap();
        return;
    }

    if(!(tr[0]==NULL || !strcmp(tr[0],"-all") || !strcmp(tr[0],"-blocks") || !strcmp(tr[0],"-vars") || !strcmp(tr[0],"-funcs")))
        printf("Error: option %s is not valid\n",tr[0]);
}


//Volcarmem function and auxiliary functions
void volcarmemaux(void *addr, int cont){
/*auxiliar function to volcarmem*/

    int i,j=0;
    unsigned char *ad = (unsigned char*) addr;
    unsigned char hex[25]="";

    while (j<cont) {
        for (i = 0; i < 25; i++) {
            if (j>=cont)
                break;

            if (ad[j] == 10){
                printf("\\n ");
            } else if((ad[j] < 32) || (ad[j] > 126)){
                printf("   ");
            }
            else printf(" %c ", ad[j]);

            hex[i]=ad[j];
            j++;
        }
        printf("\n");
        for (int k = 0; k < i; ++k) {
            printf("%02x ",hex[k]);
        }
        printf("\n");
    }
}



void cmd_volcarmem(char *tr[]){
/*Shows the contents of cont bytes starting at memory address addr*/

    int cont=25;
    void* addr;
    if (tr[0]==NULL)
        return;
    addr = (void*) strtol(tr[0], NULL, 16);

    if (tr[1]!=NULL)
        cont= atoi(tr[1]);

    printf("volcarmem %p %d bytes\n",addr,cont);
    volcarmemaux(addr,cont);
}

//Llenarmem function and auxiliary functions
void llenarmemaux(void *addr,int cont,unsigned char byte){
/*Auxiliar function to llenarmem*/

    int i;
    unsigned char *ad = (unsigned char*) addr;

    for (i = 0; i < cont; ++i) {
        ad[i]=byte;
    }

    printf("Llenando %d bytes de memoria con el byte ",cont);
    if (byte == 10){
        printf("\\n");
    } else if((byte < 32) || (byte > 126)){
        printf(" ");
    }else printf("%c", byte);
    printf("(%02x) a partir de la direccion %p\n",byte,addr);

}

void cmd_llenarmem(char *tr[]){
/*Fills cont bytes of memory starting at address addr with the value ’byte’*/

    int cont=128;
    unsigned char bytesel;

    void* addr;
    if (tr[0]==NULL )
        return;
    addr = (void*) strtol(tr[0], NULL, 16);

    if (tr[1]!=NULL)
        cont= atoi(tr[1]);

    if (tr[1]==NULL)
        printf("%s",tr[2]);

    if (tr[1]!=NULL && tr[2]!=NULL){
        if (tr[2][0]=='0'&&tr[2][1]=='x')
            bytesel= (unsigned char) strtol(tr[2], NULL, 16);
        else if (tr[2][0]=='\'' && tr[2][2]=='\'')
            bytesel= (unsigned char) tr[2][1];
        else
            bytesel= (unsigned char) strtol(tr[2], NULL, 10);
    }else bytesel = (unsigned char) 65;

    llenarmemaux(addr,cont,bytesel);
}

//Recursiva function and auxiliary functions
void doRecursiva (int n){
/*Function to do the recursion of the command recursiva*/

    char automatico[TAMANO];
    static char estatico[TAMANO];
    printf ("parameter n:%3d(%p) ",n,&n);
    printf ("automatic array en %p, ",automatico);
    printf ("static array %p\n",estatico);
    n--;
    if (n>0)
        doRecursiva(n);
}

void cmd_recursiva(char *tr[]){
/*calls the recursive functions n times*/

    int n;
    if (tr[0]==NULL)
        return;
    n = atoi(tr[0]);

    doRecursiva(n);
}

//e-s command functions
ssize_t readFile (char *fich, void *p, ssize_t n){
/*reads n bytes from file fich to memory address p, aux to e_s_read*/

    ssize_t nread,tam=n; /*if n==1 reads the hole file*/
    int fd, aux;
    struct stat s;
    if (stat (fich,&s)==-1 || (fd=open(fich,O_RDONLY))==-1){
        return ((ssize_t)-1);
    }
    if (n==LEERCOMPLETO)
        tam=(ssize_t) s.st_size;
    if ((nread=read(fd,p, tam))==-1){
        aux=errno;
        close(fd);
        errno=aux;
        return ((ssize_t)-1);
    }
    close (fd);
    return (nread);
}

void e_s_read(char *tr[]){
/*function to execute the command e-s read*/

    void *addr;
    ssize_t cont=-1,aux;
    if(tr[0]==NULL || tr[1]==NULL){
        printf("missing parameters\n");
        return;
    }
    if (tr[2]!=NULL)
        cont = (ssize_t) strtol(tr[2],NULL,10);
    addr = (void*) strtol(tr[1],NULL,16);

    if ((aux=readFile(tr[0],addr,cont))==-1)
        printf("Imposible leer fichero %s: %s\n",tr[0], strerror(errno));
    else
        printf("read %zd bytes of %s in %p\n", aux,tr[0],addr);
}

ssize_t writeFile (char *fich, void *p, ssize_t n,bool overwrite) {
/* writes n bytes from address p to fich */

    ssize_t nwritten,tam=n; /*si n==-1 lee el fichero completo*/
    int fd, aux;
    int flags= O_CREAT | O_WRONLY;

    if (!overwrite)
        flags|=O_EXCL;
    else
        flags|=O_TRUNC;

    if ((fd=open(fich,flags,0664))==-1){
        return ((ssize_t)-1);
    }

    if ((nwritten=write(fd,p, tam))==-1){
        aux=errno;
        close(fd);
        errno=aux;
        return ((ssize_t)-1);
    }
    close (fd);
    return nwritten;
}

void e_s_write(char *tr[], bool overwrite){

    void *addr;
    ssize_t cont,aux;
    if(tr[0]==NULL || tr[1]==NULL || tr[2]==NULL){
        printf("missing parameters\n");
        return;
    }

    cont = (ssize_t) strtol(tr[2],NULL,10);
    addr = (void*) strtol(tr[1],NULL,16);

    if ((aux=writeFile(tr[0],addr,cont,overwrite))==-1)
        printf("Unable to write file %s: %s\n",tr[0], strerror(errno));
    else
        printf("writen %zd bytes to %s from %p\n", aux,tr[0],addr);
}

void cmd_e_s(char *tr[]){

    if (tr[0]==NULL)
        printf("uso: e-s [read|write] ......\n");
    else if (!strcmp(tr[0],"write")){
        if (tr[1]!=NULL && !strcmp(tr[1],"-o"))
            e_s_write(tr+2,true);
        else
            e_s_write(tr+1,false);
    }
    else if (!strcmp(tr[0],"read"))
        e_s_read(tr+1);
}


int trocearcadena(char * cadena, char * trozos[]){

    int i=1;

    if((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]= strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

void ProcesarEntrada(char *tr[]){

    iscmd=true;
    bool found=false;

    int i;
    if(tr[0]==NULL) return;
    for (i = 0; C[i].cmdname != NULL; i++) {

        if (!strcmp(tr[0],C[i].cmdname)){
            (*C[i].func)(tr+1);
            found=true;
        }
    }
    if(!found) printf("command not found\n");
}


int main (int argc, char*argv[]) {
    char linea[MAXLINEA];
    char aux[MAXLINEA];
    char *tr[MAXLINEA / 2];

    createList(&list);
    createMemList(&memlist);

    while(1){
        printf( "\x1b[32m" ">> " "\x1b[0m");
        fgets(linea, MAXLINEA, stdin);
        tItemL item;

        /*an aux array is created to store linea, then the content of aux is copied into the list*/
        strcpy(aux, linea);
        aux[strlen(aux) - 1] = '\0';

        if (trocearcadena(linea, tr) == 0)
            continue;
        ProcesarEntrada(tr);

        if(iscmd){
            strcpy(item.cmdline, aux);
            insertItem(item, &list);
        }
    }
}
