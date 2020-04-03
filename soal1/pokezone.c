#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <wait.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <string.h>
#include <time.h> 

pthread_t tid[5];
int item[3] = {100,100,100};
int capture,notEscape = 0,escape = 0;
int rarity[3],get_pokedollar = 0;
char get_rarity[10];
char get_pokemonName[15];


// void shutdown()
// {
//     pid_t child_id1,child_id2;
//     child_id1 = fork();
//     if(child_id1 == 0)
//     {
//         char* argv[] {"kill", "pid", NULL};
//         execv("/bin/kill", argv);
//     }
//     child_id2 = fork();
//     if(child_id2 = 0)
//     {
//         char* argv[] {"kill", "pid", NULL};
//         execv("/bin/kill", argv);
//     }
// }

//kurang list item
void* effectItem(void *arg)
{
//gagenah
    int i;
    pthread_t id = pthread_self();
    char* namaItem[3]={"Lullaby Powder","Pokeball","Berry"};
    char* deskripsiItem[3]={"+20% Capture rate, 0% Escape rate kedua efek bertahan 10 detik","Menangkap pokemon","+10 AP semua pokemon"};
    int hargaItem[3]={60,5,15};
    if(pthread_equal(id,tid[0]))
    {
        while (1)
        {
            sleep(10);
            for(i = 0;i<3;i)
            {
                if(item[i] < 200) item[i]+10;
            }
        }
    }
    //Lullaby Powder
    if(pthread_equal(id,tid[1]))
    {
        capture+=20;
        notEscape=1;
        sleep(10);
        notEscape=0;
        capture-=20;
    }
    if(pthread_equal(id,tid[2]))
    {
        for(i = 0;i<3;i++)
        {
            printf("%d. %s %s %d %d\n",i+1,namaItem[i],deskripsiItem[i],hargaItem[i],item[i]);
        }
    }
}
//cek
void* pokemon(void *argv)
{
    char pokemonName[15][15] = {"Bulbasaur","Charmander","Squirtle","Rattata","Caterpie",
                            "Pikachu","Eevee","Jigglypuff","Snorlax","Dragonite",
                            "Mew","Mewtwo","Moltres","Zapdos","Articuno"};
    char rarityName[3][10] = {"Normal","Rare","Legendary"};
    int escapeRate[3] = {5,10,20};
    char temp[2] = "";
    int batasRarity[4] = {0,80,95,100};
    int captureChance[3] = {70,50,30};
    int pokedollar[3] = {80,100,200};
    int i,j,k,encounter,index_pokemonName;
    pthread_t id = pthread_self();
    if(pthread_equal(id,tid[0]))
    {
        
        while (1)
        {
           // i = 2;
            encounter = rand() % 100;
            //printf("%d ",encounter);
               // temp = temp / (j+1)+1*j;
                //printf("%d",temp);
            for(i = 0; i <=3; i++)
            {
                if(encounter < batasRarity[i+1] && encounter >= batasRarity[i])
                {
                    rarity[i] = 1;
                    escape = escapeRate[i]; 
                    capture = captureChance[i];
                    get_pokedollar = pokedollar[i];
                    strcpy(get_rarity,rarityName[i]);
                    int cek_shiny = rand () % 8000;
                    if(cek_shiny < 1)
                    {
                        get_pokedollar+=5000;
                        capture-=20;
                        rarity[i] * 10;
                        strcpy(temp,"*");
                        escape +=5;
                    }
                   //printf("%d ",rarity[i]);
                    int pokemonName_rate = rand() % 100;
                    for(j = 20,k=0; j<= 100; j+=20,k++)
                    {
                        //printf("%d ",pokemonName_rate);
                        if(pokemonName_rate < j) 
                        {
                            strcpy(get_pokemonName,pokemonName[k+5*i]);
                            if(rarity[i]=10) strcat(get_pokemonName, temp);
                            //printf("%s ",get_pokemonName);
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
}

int main()
{
    srand(time(NULL)); 
    //int i=0;
	// while(i<3) // loop sejumlah thread
	// // {
    key_t key1 = 1,key2 = 2,key3 = 3,key4 = 4,key5 = 5;
     
    while (1)
    {
    int *escapePokemon;
    int *capturePokemon;
    int *pokedollarmon;
	pthread_create(&(tid[0]),NULL,&pokemon,NULL);
	pthread_join(tid[0],NULL);
   // printf("%d",escape);

    int shmid1 = shmget (key1, sizeof(int), IPC_CREAT | 0666);
    int shmid2 = shmget (key2, sizeof(int), IPC_CREAT | 0666);
    int shmid3 = shmget (key3, sizeof(int), IPC_CREAT | 0666);
    int shmid4 = shmget (key4, sizeof(int), IPC_CREAT | 0666);
    int shmid5 = shmget (key5, sizeof(int), IPC_CREAT | 0666);

    char *send_namaPokemon = (char *) shmat(shmid1, NULL, 0);
    char *send_rarity = (char *) shmat(shmid2, NULL, 0);
    escapePokemon = shmat(shmid3, NULL, 0);
    capturePokemon = shmat(shmid4, NULL, 0);
    pokedollarmon = shmat(shmid5, NULL,0);

    strcpy(send_namaPokemon,get_pokemonName);
    strcpy(send_rarity,get_rarity);
    *escapePokemon = escape;
    *capturePokemon = capture;
    *pokedollarmon = get_pokedollar;

    shmdt(pokedollarmon);
    shmdt(capturePokemon);
    shmdt(send_namaPokemon);
    shmdt(send_rarity);
    shmdt(escapePokemon);

    sleep(1);
    }
   // shmdt(get_pokemonName);
   

	//pthread_join(tid[1],NULL);
	//exit(0);
	//return 0;
}