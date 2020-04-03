#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <time.h> 


pthread_t tid[3];
int stop=0;
char listPokemon[7][15];
int rarity[3];
int ap[7];
int item[3] = {0,20,0};
int capture_mode = 0,search_mode = 0;
int state = 0,escaped = 0,ketangkap = 0,myMoney = 100;
char* namaPokemon;
char* rarityPokemon; 
int* escapePokemon;
int* capturePokemon;
int* pokedollarmon;


//srand

void menu()
{
    pid_t child_id;
    int status;
   // int *ptr[2];
    child_id = fork();
    if(child_id == 0)
    {
             
        char *argv[] = {"clear", NULL};
        execv("/usr/bin/clear", argv);
    }
    else
    {    
        while ((wait(&status)) > 0);
        if(state != 1) printf("1. Cari Pokemon\n");
        else if(capture_mode == 1) printf("1. Capture\n");
        else printf("1. Berhenti Mencari\n");
        printf("2. Pokedex\n3. Shop\n");
        if(escaped == 1) printf("\n Pokemon melarikan diri\n");
        
    }
   
}


void pokedex()
{
    int i;
    int pilDex;
    pid_t child_id;
    int status;
    while (1)
    {
    
    child_id = fork();
    if(child_id == 0)
    {
             
        char *argv[] = {"clear", NULL};
        execv("/usr/bin/clear", argv);
    }
    while ((wait(&status)) > 0);
    for(i=0; i<7; i++)
    {
        if(strcmp(listPokemon[i],"1")!=-1) printf("%d. %s %d\n",i+1,listPokemon[i],ap[i]);
        else break;
    }
        if(strcmp(listPokemon[0],"1")==-1) printf("Anda tidak memiliki pokemon\n3. Kembali\n");
        else
        printf("\n1. Lepas Pokemon\n2. Beri Berry\n3. Kembali\n");
        scanf("%d",&pilDex);
        if(pilDex == 1)
        {
            int lepas;
            printf("Pilih index Pokemon:\n");
            for(i = 0; i<7; i++)
            {
                if(strcmp(listPokemon[i],"1")!=-1) printf("%d. %s %d\n",i+1,listPokemon[i],ap[i]);
            }
            scanf("%d",&lepas);
//check???
            for(i = lepas - 1; i<7-1; i++) 
            {
                strcpy(listPokemon[i],listPokemon[i+1]);
                ap[i] = ap[i+1];
            }
            strcpy(listPokemon[6],"0");
        }
        else if(pilDex == 2)
        {
            for(i = 0; i<7; i++) if(strcmp(listPokemon[i],"1")==-1) ap[i]+=10;
            else break;
        }
        else if (pilDex == 3)
        {
            state = 0;
             break;
             
        }
    }
    
}

void* apDecrement(void* arg)
{
    int i,index;
    for(i = 6; i<= 0;i--) if(strcmp(listPokemon[i],"1")==-1)
    {
        index = i;
        break;
    } 
    pthread_t id = pthread_self();
    if(pthread_equal(id,tid[index]))
    {
        while (1)
        {
            if(capture_mode == 0)
            {
            sleep(1);
            ap[index]-=10;
            // int status;      
            // pid_t child_id;
            // child_id = fork();
            // if(child_id == 0)
            // {       
            //     char *argv[] = {"clear", NULL};
            //     execv("/usr/bin/clear", argv);
            // }
            // while ((wait(&status)) > 0);
            // pokedex();
            if(ap[index] <= 0)
            {
                int lepas = rand() % 100;
                if(lepas < 90)
                {
                    for(i = index - 1; i<7-1; i++) 
                    {
                        strcpy(listPokemon[i],listPokemon[i+1]);
                        ap[i] = ap[i+1];
                    }
                }
                else if(lepas>=0 && lepas <10)
                {
                    ap[index] = 50;
                }
            }
            }
        }
    }

}

void* tangkap(void *arg)
{
    pthread_t id = pthread_self();
    int capturing = *capturePokemon;
    if(pthread_equal(id,tid[0]))
    {
        int chance;
        chance = rand() %10;
        if(chance < capturing)
        {
            ketangkap = 1;
        }
    }
}

void capture()
{
    ketangkap = 0;
    char PokemonTertangkap[15] = " ";
    char rarityTertangkap[10]= " ";
    strcpy(PokemonTertangkap,namaPokemon);
    strcpy(rarityTertangkap,rarityPokemon);
    int pilihan_capture;
    int status,i;
    pid_t child_id = fork();
    if(child_id == 0)
    {
             
        char *argv[] = {"clear", NULL};
        execv("/usr/bin/clear", argv);
    }
    while ((wait(&status)) > 0);
    printf("----Capturing----\n");
    printf("(%s) %s\n\n",rarityPokemon,namaPokemon);
    printf("1. Tangkap\n2. Item\n3. Keluar\n");
    while (1)
    {
        if(escaped == 0)
        {
            scanf("%d",&pilihan_capture);
            if(pilihan_capture == 1)
            {
                if(item[1] > 0)
                {
                    item[1]--;
                    pthread_create(&(tid[0]),NULL,&tangkap,NULL);
                    if(ketangkap == 1)
                    {
                        printf("\n(%s) %s berhasil ditangkap\n",rarityTertangkap,PokemonTertangkap);
                        for(i = 0; i<7; i++)
                        {
                            if(strcmp(listPokemon[i],"1")==-1) 
                            {
                                strcpy(listPokemon[i],PokemonTertangkap);
                                ap[i] = 100;
                                pthread_create(&(tid[i]),NULL,&apDecrement,NULL);

                                break;
                            }
                        }
                        capture_mode = 0;
                        stop = 1;
                        state = 0;
                        //escaped == 0;
                        printf("\nTekan enter untuk kembali ke menu\n");
                        getchar();
                        getchar();
                        break;
                    }
                    printf("Sisa pokeball %d\n",item[1]);

    //tangkap
                }
                else {
                    printf("pokeball telah habis\n");
                }
            }
            else if(pilihan_capture == 2)
            {
                int pilihan_item;
                printf("Item yang bisa digunakan:\n1. lullaby powder");
                scanf("%d",&pilihan_item);
                if(pilihan_item == 1)
                {
                    item[0]--;
        //efek
                }
            }
            else if(pilihan_capture == 3)
            {
                capture_mode = 0;
                stop = 1;
                state = 0;
                menu(state);
                
                break;
            }
        }
        else if(escaped == 1)
        {
            capture_mode = 0;
            stop = 1;
            state = 0;
            //escaped == 0;
            menu(state);
            
            break;
        }
    }
    return;
}


void shop()
{
    ///
}


void* cariPokemon(void *arg)
{
    pthread_t id = pthread_self();
    if(pthread_equal(id,tid[0]))
        {
//tes 
            while (1)
            {
                int get;
                    sleep(1);
                    get = rand() %100;
                   // printf("%d",get);
                if(stop != 1)
                {
                    if(get<60)
                    {
                        printf("o");
                        fflush(stdout);

//bool???               
                        capture_mode =1;
                        search_mode = 0;
                        stop = 1;
                        menu();
                        //fflush(stdin);
                        //capture();
                    }
                    else
                    {
                        printf("-");
                        fflush(stdout);
                    }
                }
                else
                {
                    break;
                   // pthread_exit(&get);
                    
                }
                
               
            }
            
            //sleep(1);            
        }
        return NULL;
}

void* escape(void *arg)
{
    int escapeRate = *escapePokemon;
    escaped == 0;
    pthread_t id = pthread_self();
    if(pthread_equal(id,tid[0]))
    {
        if(capture_mode == 1)
        {
            while(1)
            {
//check          
                sleep(20);
                int rate;
                rate = rand() %100;
                if(rate < escapeRate) 
                {
                    escaped = 1;
//kurang input interupt
                    capture_mode = 0;
                    break;
                }    
            }
        }
    }
}



void main(void)
{
    srand(time(NULL)); 
    key_t key1 = 1, key2 = 2,key3 = 3, key4 =4,key5 = 5;
//int *tes;
    int shmid1= shmget(key1, sizeof(int) , IPC_CREAT | 0666);
    int shmid2= shmget(key2, sizeof(int) , IPC_CREAT | 0666);
    int shmid3= shmget(key3, sizeof(int) , IPC_CREAT | 0666);
    int shmid4 = shmget (key4, sizeof(int), IPC_CREAT | 0666);
    int shmid5 = shmget (key5, sizeof(int), IPC_CREAT | 0666);

    namaPokemon = (char*)shmat(shmid1, NULL, 0);
    rarityPokemon = (char*)shmat(shmid2, NULL, 0);
    escapePokemon = shmat(shmid3, NULL, 0);
    capturePokemon = shmat(shmid4, NULL, 0);
    pokedollarmon = shmat(shmid5, NULL, 0);

    //printf("%s %s %d %d %d",rarityPokemon, namaPokemon,*escapePokemon,*capturePokemon,*pokedollarmon);

    // // //int state =0 ;
    memset(listPokemon,'0',sizeof(listPokemon));
    //printf("%d ",strcmp(listPokemon[1],"1"));
    while (1) 
    {
        menu(state);
        if(state == 0)
        scanf("%d",&state);
        if(state == 1)
        {
            menu(state);
            //printf("----Searching----\n");
            stop = 0;
            int ss;
            search_mode = 1;
            if(stop !=1)
            pthread_create(&(tid[0]),NULL,&cariPokemon,NULL);
            //pthread_join(tid[0],NULL);
            capture_mode == 0;
            if(search_mode ==1)
            {
                scanf("%d",&ss);
                //else capture();
                while(1)
                {
                    if(capture_mode == 0)
                    {
                        if(ss==1) 
                        {
                            stop =1;
                            menu(state);
                            state=0;
                            search_mode = 0;
                            break;
                        }
                    }
                    else if(capture_mode == 1)
                    {
                        if(ss == 1)
                        {
                            stop =1;
                            pthread_create(&(tid[0]),NULL,&escape,NULL);
                            state=0;
                            search_mode = 0;
                            capture();
                            break;
                        }
                    }
                }
            }
            //else capture();
        }
        else if(state == 2)
        {
            pokedex();
        }
        else if(state == 3)
        {
            shop();
        }
    }

    shmdt(rarityPokemon);
    shmdt(namaPokemon);
    shmdt(escapePokemon);
    shmdt(capturePokemon);
    shmdt(pokedollarmon);

    shmctl(shmid1, IPC_RMID, NULL);
    shmctl(shmid2, IPC_RMID, NULL);
    shmctl(shmid3, IPC_RMID, NULL);
    shmctl(shmid4, IPC_RMID, NULL);
    shmctl(shmid5, IPC_RMID, NULL);
}