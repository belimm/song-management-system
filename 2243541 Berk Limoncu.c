#include <stdio.h>					//*BERK LIMONCU 2243541	
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 50
#define TRUE 1
#define FALSE 0

struct dateTime	
{
    int	day;
	int month;
	int year;
	int hour;
	int minute;        
};

struct songNode
{
	int id;
	char name_song[MAX];
	char genre[MAX];
	char album_name[MAX];
	char artist_name[MAX];
	int released_date;
	struct dateTime *dt;
	struct songNode *next;
};

struct songRecord
{
	struct songNode *head;
	struct songNode *tail;
	int size;
};

typedef struct songRecord *song;

int EmptyChecker(song);
song initialiseSongs(void);
song addSong(song,char *,char *,char *,char *,int);
song deleteSong(song,int);
void printSongs(song);
void searchSongs(song);
void createPlayList(song);
void Overwrite(song);

int main()
{
	song mySongs;
	
	int exit=FALSE,option,id;
	char n_song[MAX],genre[MAX],n_album[MAX],n_artist[MAX];
	int year;
	
	fflush(stdin);
	
	mySongs=initialiseSongs(); 
	
	while(!exit)
	{
		printf("\n----- MENU-----\n\n");
		printf("1.Add Song\n2.Delete Song\n3.Print Songs\n4.Search Songs\n5.Create PlayList\n6.Exit\n");
		
		printf("\nEnter your option:");
		scanf("%d",&option);
		fflush(stdin);
		
		switch(option)
		{
			
			case 1:
				printf("Enter name of the song: ");
				gets(n_song);
				fflush(stdin);
				
				printf("Enter genre of the song: ");
				gets(genre);
				fflush(stdin);
				
				printf("Enter name of the album: ");
				gets(n_album);
				fflush(stdin);
				
				printf("Enter name of the artist: ");
				gets(n_artist);
				fflush(stdin);
				
				printf("Enter year of the song: ");
				scanf("%d",&year);
				
				mySongs=addSong(mySongs,n_song,genre,n_album,n_artist,year);
				break;
			case 2:
				printf("\nEnter the ID of the song you want to delete: ");
				scanf("%d",&id);
				
				mySongs=deleteSong(mySongs,id);
				
				break;
			case 3:
				printSongs(mySongs);
				break;
			case 4:
				searchSongs(mySongs);
				break;
			case 5:
				createPlayList(mySongs);
				break;
			case 6:
				exit=TRUE;
				Overwrite(mySongs);
				break;
			default:
				printf("\nInvalid Option Please Try Again!");
				break;
		}	
	}
	
	return 0;
	
}

int emptyChecker(song s)
{
	return(s->size);
}

song initialiseSongs(void)
{
	FILE *song_man;
	struct songNode *temp;
	song mySongs;
	
	song_man=fopen("SongMan.txt","r");
	
	if(song_man == NULL) //*To check whether the file can be readed or not
	{
		printf("Failed!\n");
		exit(0);
	}
	
	mySongs=(struct songRecord*)malloc(sizeof(struct songRecord));			//*For Dummy Node
	mySongs->head=(struct songNode*)malloc(sizeof(struct songNode));		//*For Dummy Node
	mySongs->head->dt=(struct dateTime*)malloc(sizeof(struct dateTime));	//*For Dummy Node		
		
	mySongs->tail=mySongs->head;
	mySongs->size=0;
	
	
	temp=mySongs->head;
	
	temp->next=(struct songNode*)malloc(sizeof(struct songNode));
	temp->next->dt=(struct dateTime*)malloc(sizeof(struct dateTime));
	temp->next->next=NULL;
	temp=temp->next;
	
	while(!feof(song_man))
	{
		fscanf(song_man,"%d;%[^;];%[^;];%[^;];%[^;];%d;%d/%d/%d %d:%d\n",&temp->id,temp->name_song,temp->genre,temp->album_name,temp->artist_name,&temp->released_date,&temp->dt->day,&temp->dt->month,&temp->dt->year,&temp->dt->hour,&temp->dt->minute);
		fflush(stdin);
		mySongs->size++;
		
		if(feof(song_man))
			break;
		
		temp->next=(struct songNode*)malloc(sizeof(struct songNode));
		temp->next->dt=(struct dateTime*)malloc(sizeof(struct dateTime));
		temp->next->next=NULL;
		temp=temp->next;
		mySongs->tail=temp;
	}
		
	printf("The SongMan.txt file has been loaded successfully\n");

	fclose(song_man);
	
			
	return mySongs;

}



song addSong(song s,char *n_song,char *genre,char *n_album,char *n_artist,int r_date)		
{
	if(emptyChecker==0) //*To check whether the list is empty or not
		printf("\nEmpty List!\n");
	else
	{
		time_t ti = time(NULL);
		struct tm t = *localtime(&ti);
		struct songNode *newSong,*temp;
		
		newSong=(struct songNode*)malloc(sizeof(struct songNode));
		newSong->dt=(struct dateTime*)malloc(sizeof(struct dateTime));
		newSong->next=NULL;
	
		strcpy(newSong->name_song,n_song);
		strcpy(newSong->genre,genre);
		strcpy(newSong->album_name,n_album);
		strcpy(newSong->artist_name,n_artist);
		newSong->released_date=r_date;
		newSong->id=s->size+1;
		
		newSong->dt->day=t.tm_mday;
		newSong->dt->month=t.tm_mon+1;
		newSong->dt->year=t.tm_year+1900;
		newSong->dt->hour=t.tm_hour;
		newSong->dt->minute=t.tm_min;
	
		temp=s->tail;	
	
		newSong->next=temp->next;
		temp->next=newSong;
		s->tail=temp->next;
	
		s->size++;
	
		return s;
	}
}

song deleteSong(song l,int id)
{
	struct songNode *deletedNode;
	struct songNode	*iter=l->head;
	int i;
	
	for(i=1;i<id;i++)	//*To find the previous node of deletedNode
		iter=iter->next;
		
	deletedNode=iter->next;
	
	iter->next=iter->next->next;	
	free(deletedNode);
	l->size--;
	printf("\nSong with ID %d has been deleted from your list!!!\n\n",id);
	
	return l;
}

void printSongs(song l)						
{
	struct songNode *iter=l->head->next;
	
	printf("\nSongs in your database:\n-----------------------\n");
	
	while(iter!=NULL)
	{
		printf("ID: %d\nSong Name: %s\nSong Genre: %s\nAlbum Name: %s\nArtist Name:%s\nYear: %d\nDate: %02d/%02d/%04d\nTime: %02d:%02d\n\n",iter->id,iter->name_song,iter->genre,iter->album_name,iter->artist_name,iter->released_date,iter->dt->day,iter->dt->month,iter->dt->year,iter->dt->hour,iter->dt->minute);
		iter=iter->next;
	}

}

void searchSongs(song l)
{
	struct songNode* iter=l->head;
	int option,counter=0;
	char name[MAX];
	
	printf("---- Searching by----\n(1) song name\n(2) artist name\n(3) genre\nEnter search by option: ");
	scanf("%d",&option);
	
	switch(option)
	{
		case 1:
			printf("Enter Song Name: ");
			fflush(stdin);
			gets(name);

			while(iter!=NULL)
			{
				if(strnicmp(iter->name_song,name,4)==0)  //*I assume that if the first +4 character entered by the user then u can find the sound.For example,if you search "unut" or "unutm" ... you can find the song in the list but if u write "unu" you can't find the song in the list
				{
					if(counter==0)									//*If searched song is in the list to print "Result:" I use counter==0 because in every loop it prints "Result:" I want to print just once
						printf("Results:\n---------------------\n");
					printf("ID: %d\nSong Name: %s\nSong Genre: %s\nAlbum Name: %s\nArtist Name: %s\nYear: %d\nDate: %02d/%02d/%04d\nTime: %02d:%02d\n",iter->id,iter->name_song,iter->genre,iter->album_name,iter->artist_name,iter->released_date,iter->dt->day,iter->dt->month,iter->dt->year,iter->dt->hour,iter->dt->minute);
					counter++;
				}
				iter=iter->next;
			}
			
			if(counter==0)		//*If counter==0 there is no song with searched name
				printf("\nNo song has this name!\n");
			break;
		case 2:
			printf("Enter Artist Name: ");
			fflush(stdin);
			gets(name);
			

			while(iter!=NULL)
			{
				if(stricmp(iter->artist_name,name)==0)				//*I used stricmp(string1,string2) function because it works the regardless of lower or upper case
				{
					if(counter==0)									//*If searched song is in the list to print "Result:" I use counter==0 because in every loop it prints "Result:" I want to print just once
						printf("Results:\n---------------------\n");
					printf("ID: %d\nSong Name: %s\nSong Genre: %s\nAlbum Name: %s\nArtist Name: %s\nYear: %d\nDate: %02d/%02d/%04d\nTime: %02d:%02d\n",iter->id,iter->name_song,iter->genre,iter->album_name,iter->artist_name,iter->released_date,iter->dt->day,iter->dt->month,iter->dt->year,iter->dt->hour,iter->dt->minute);
					counter++;
				}
				iter=iter->next;
			}
			
			if(counter==0)										//*If counter==0 there is no song with searched name
				printf("\nNo song has this artist name!\n");
			break;
		case 3:
			printf("Enter Song Genre: ");
			fflush(stdin);
			gets(name);
			
			while(iter!=NULL)
			{
				if(stricmp(iter->genre,name)==0)			//*I used stricmp(string1,string2) function because it works the regardless of lower or upper case
				{
					if(counter==0)							//*If searched song is in the list to print "Result:" I use counter==0 because in every loop it prints "Result:" I want to print just once
						printf("Results:\n---------------------\n");
					printf("\nID: %d\nSong Name: %s\nSong Genre: %s\nAlbum Name: %s\nArtist Name: %s\nYear: %d\nDate: %02d/%02d/%04d\nTime: %02d:%02d\n",iter->id,iter->name_song,iter->genre,iter->album_name,iter->artist_name,iter->released_date,iter->dt->day,iter->dt->month,iter->dt->year,iter->dt->hour,iter->dt->minute);
					counter++;
				}
				iter=iter->next;
			}
			if(counter==0)								//*If counter==0 there is no song with searched name
				printf("\nNo song has this genre!\n");
			break;
	}
}

void createPlayList(song l)
{
	FILE *playlist;
	playlist=fopen("PlayList-1.txt!","w");
	
	struct songNode *iter=l->head->next;	//*For iteration part
	struct songNode *p=iter;				//*I also created a new pointer because I need after using loop in after in line 370
	
	
	char song_1[MAX],song_2[MAX],song_3[MAX]; //*Song are to be searched 
	int pos_1=0,pos_2=0,pos_3=0;		//*I assigned their values as 0 because I need in the loop
	
	if(playlist==NULL)			//*To check wheter the data can be writed on file or not
		printf("\nFailed to write\n");
	else
	{
		printf("\nEnter the names of songs for your playlist:\n");
		printf("Enter (1) song name: ");
		fflush(stdin);
		gets(song_1);
	
		printf("Enter (2) song name: ");
		fflush(stdin);
		gets(song_2);
	
		printf("Enter (3) song name: ");
		fflush(stdin);
		gets(song_3);
	
		while(iter!=NULL)
		{
			if(stricmp(iter->name_song,song_1)==0)		//*If the song are founded the volue of position(pos_1,pos_2,pos_3) to initialize to their ID number in the list.
				pos_1=iter->id;
		
			if(stricmp(iter->name_song,song_2)==0)
				pos_2=iter->id;
			
			if(stricmp(iter->name_song,song_3)==0)
				pos_3=iter->id;
		
			iter=iter->next;
		}
		printf("\n%d	%d		%d\n",pos_1,pos_2,pos_3);
	
		if((pos_1!=0) && (pos_2!=0) && (pos_3!=0))	//*The case the all songs are founded(If the sound can't be found in the list the value of pos_1 or pos_2 or pos_3 is must be 0 because I assigned to 0
		{
			while(p!=NULL)
			{
				if(stricmp(p->name_song,song_1)==0)
					fprintf(playlist,"%d;%s;%s;%s;%s;%d;%d/%d/%d %d:%d\n",p->id,&p->name_song,&p->genre,&p->album_name,&p->artist_name,p->released_date,p->dt->day,p->dt->month,p->dt->year,p->dt->hour,p->dt->minute);
				if(stricmp(p->name_song,song_2)==0)
					fprintf(playlist,"%d;%s;%s;%s;%s;%d;%d/%d/%d %d:%d\n",p->id,&p->name_song,&p->genre,&p->album_name,&p->artist_name,p->released_date,p->dt->day,p->dt->month,p->dt->year,p->dt->hour,p->dt->minute);
				if(stricmp(p->name_song,song_3)==0)
					fprintf(playlist,"%d;%s;%s;%s;%s;%d;%d/%d/%d %d:%d\n",p->id,&p->name_song,&p->genre,&p->album_name,&p->artist_name,p->released_date,p->dt->day,p->dt->month,p->dt->year,p->dt->hour,p->dt->minute);
				
				p=p->next;
			}
		}
		else
			printf("\nThe playList cannot be created! Not all songs found!!!\n");
	
		fclose(playlist);
	}
}

void Overwrite(song l)
{
	FILE *over_write;
	
	if(fopen==NULL)		//*To check wheter the data can be writed on file or not
		printf("\nFailed to write!\n");
	else
	{
		printf("The SonMan.txt file has been updated successfully!!");
		over_write=fopen("SongMan.txt","w+");
		l->head=l->head->next;
		while(l->head!=NULL)
		{
			fprintf(over_write,"%d;%s;%s;%s;%s;%d;%d/%d/%d %d:%d\n",l->head->id,&l->head->name_song,&l->head->genre,&l->head->album_name,&l->head->artist_name,l->head->released_date,l->head->dt->day,l->head->dt->month,l->head->dt->year,l->head->dt->hour,l->head->dt->minute);
			l->head=l->head->next;	
		}
	
		fclose(over_write);
	}
}																			//*BERK LIMONCU 2243541				
