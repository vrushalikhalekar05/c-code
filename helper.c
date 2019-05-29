#include "packer.h"
BOOL FilePack(char *DirName,char *Filename)
{
  DIR *dir;//DIR is a structure //when file gets opened then all info of directory will copied into DIR structure
  struct dirent *entry;//structure dirent
  struct stat filestat;//structure stat for displaying info about file
  char name[100]={'\0'};
  char buff[256]={'\0'};//set all indexes to empty
  char *ptr=NULL;

  int fd,fdread,retread,ret,fdnew,temp,icnt=0;
  int i=0;
  char key;
  FILEINFO obj;        //2nd  step is to create file bcz if directory is available then only we can extract all the files into another file.so that 1st check if directory and then create file
  if((dir=opendir(DirName))==NULL)
    {
      printf("error:unable to open specified directory\n");
      return ERR_ACCESS;
    }
  fd=creat(Filename,0666);//0666 is a permission readwrite
  if(fd==-1)
    {
      printf("error:unable to create file\n");
      return ERR_ACCESS;
    }
  key=strlen(Filename);

  write(fd,"Marvellous21",strlen("Marvellous21"));
  write(fd,&key,1);

  printf("key written\n");
  while((entry=readdir(dir))!=NULL)
    {
      sprintf(name,"%s/%s",DirName,entry->d_name);
      stat(name,&filestat);
      if(S_ISREG(filestat.st_mode))
	{
	  fdread=open(name,O_RDONLY);
	  strcpy(obj.name,entry->d_name);
	  obj.size=filestat.st_size;

	  write(fd,&obj,sizeof(obj));
	  while((retread=read(fdread,buff,sizeof(buff)))!=0)
	    {
	      i=retread;
	      printf("%d\n",i);
	      temp=retread;
	      // printf("%d",retread);
	      while(temp!=0)
		{
		  // if(buff[temp]!=' ')
		  // {
		  buff[temp]+=key;
		  // }
		  temp--;
		}
	      write(fd,buff,retread);
	    }
	  close(fdread);
	  icnt++;
	}
    }
  closedir(dir);
  printf("%d files successfully combined\n",icnt);
  return SUCCESS;
}
BOOL FileUnpack(char *FileName)
{
  DIR *dir;
  struct dirent *entry;
  struct stat filestat;
  char name[100]={'\0'};
  char buff[256]={'\0'};
  char *ptr=NULL;

  int fd,fdread,retread,ret,fdnew,temp,icnt=0;
  char key;
  FILEINFO obj;
  fd=open(FileName,O_RDONLY);
  if(fd==-1)
    {
      printf("error:unable to open file\n");
      return ERR_ACCESS;
    }
  ret=read(fd,buff,strlen("Marvellous21"));
  if(ret<=0)
    {
      printf("error:unable to open file\n");
      return ERR_INVALID;
    }
  read(fd,&key,1);
  while((ret=read(fd,&obj,sizeof(obj)))!=0)
    {
      fdnew=creat(obj.name,0666);

      ptr=(char*)malloc(obj.size*sizeof(char));
      read(fd,ptr,obj.size);

      temp=obj.size;
      while(temp!=0)
	{
	  ptr[temp]-=key;
	  temp--;
	}
      write(fdnew,ptr,obj.size);
      close(fdnew);
      free(ptr);
      icnt++;
    }
  close(fd);
  printf("%dfiles successfully recreated\n",icnt);
  return SUCCESS;
}
void DisplayHelp()
{

  printf("to pack files\n");
}
void DisplayError(BOOL ret)
{
  switch(ret)
    {
    case ERR_INVALID:
      printf("error:invalid file for unpacking");
      break;
    case ERR_ACCESS:
      printf("error:access denied");
      break;
    }
}
