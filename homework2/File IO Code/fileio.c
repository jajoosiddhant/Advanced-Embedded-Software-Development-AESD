#include <stdio.h>
#include <stdlib.h>

int main()
{

char *arr;
FILE *testfile;
printf("My name is Siddhant Jajoo\n");

//create a file
testfile = fopen("testfile.txt","w");
if(testfile == NULL)
{

printf("Testfile creation failed.\n");
exit (1);

}
else
{
printf("Testfile Created\n");
}

//add text in the file
fprintf (testfile, "Welcome to the testfile\n");
fclose(testfile);

//Dynamically allocate array of size 50
arr = (char *) malloc(sizeof(char)*50);

printf("Enter text to store in the text file\n");
scanf("%s", arr);

//append user data in file
testfile = fopen("testfile.txt","a");
fprintf (testfile, "Input From user: %s\n",arr);

//flush and close file
fflush(testfile);
fclose(testfile);

//Open and read a char and then a string from file 
testfile = fopen("testfile.txt","r");
char k = fgetc(testfile);
printf("%c\n",k);
fgets(arr,50,testfile);
printf("%s\n",arr);

//close file
fclose(testfile);
//free allocated memory
free(arr);

return (0);
}
