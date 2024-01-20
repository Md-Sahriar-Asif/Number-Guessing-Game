#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>

int maxnumber;

struct userProfile
{
    char userName[20];
    int bestScore;
};

void manageUserProfile(struct userProfile *p1)
{
    printf("Enter your surname: ");
    scanf("%s", p1->userName);
    FILE *fp = fopen("user_profiles.txt", "r");
    if(fp != NULL)
    {
       char tempUsername[20];
       while(fscanf(fp, "%s %d\n", tempUsername, &p1->bestScore) != EOF)
       {
           if(strcmp(p1->userName,tempUsername)==0)
           {
               fclose(fp);
               return;
           }
       }
       fclose(fp);
    }
    p1->bestScore = 999;
}

int setDifficulty()
{
    int choice;
    printf("Select a difficulty level:\n");
    printf("1 for Easy(1-25) level\n");
    printf("2 for Medium(1-50) level\n");
    printf("3 for Hard(1-100) level\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

int generateRadnum(int difficulty)
{
    switch(difficulty)
    {
    case 1:
        maxnumber = 25;
        break;
    case 2:
        maxnumber = 50;
        break;
    case 3:
        maxnumber = 100;
        break;
    default:
        maxnumber = 100;
        break;
    }
    return (rand() % maxnumber)+1;
}

void countdownAnimation()
{
    printf("Get ready to play!\n");
    for(int i=3; i>0; i--)
    {
        printf("%d...\n", i);
        usleep(1000000);
        system("cls");
    }
}

void updateBestScore(struct userProfile *p1, int score)
{
    int found = 0;
    struct userProfile tempProfiles[100];
    int count = 0;

    FILE *fp = fopen("user_profiles.txt", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%s %d\n", tempProfiles[count].userName, &tempProfiles[count].bestScore) != EOF)
        {
            if (strcmp(p1->userName, tempProfiles[count].userName) == 0)
            {
                if (score < tempProfiles[count].bestScore)
                {
                    tempProfiles[count].bestScore = score;
                }
                found = 1;
            }
            count++;
        }
        fclose(fp);
    }

    if (!found) {
        strcpy(tempProfiles[count].userName, p1->userName);
        tempProfiles[count].bestScore = score;
        count++;
    }

    fp = fopen("user_profiles.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s %d\n", tempProfiles[i].userName, tempProfiles[i].bestScore);
    }
    fclose(fp);

}

void DisplayScores()
{
    printf("\nBest scores of different players:\n");
    FILE *fp = fopen("user_profiles.txt", "r");
    if(fp != NULL)
    {
        struct userProfile p1;
        while(fscanf(fp, "%s %d\n", p1.userName, &p1.bestScore) != EOF)
        {
            printf("Name: %s - best score = %d\n", p1.userName, p1.bestScore);
        }
        fclose(fp);
    }
    else{
        printf("No scores found.\n");
    }
}

void  trackStatistics(char name[], int attempts, int isWin)
{
    FILE *file = fopen("user_statistics.txt", "a");
    if(file != NULL)
    {
        fprintf(file, "player: %s, Attempts: %d, and %s\n", name, attempts, (isWin) ? "Won" : "Lose");
        fclose(file);
    }
}

int main()
{
    srand(time(NULL));
    struct userProfile p1;
    manageUserProfile(&p1);

    int difficulty = setDifficulty();
    int secretNum = generateRadnum(difficulty);

    int attempts=0;
    int guess;
    int isWin=0;

    countdownAnimation();

    printf("Welcome %s to the number guessing game!\n", p1.userName);
    if(p1.bestScore==999)
    {
        printf("Your best score is 999, which is a default value. Try to achieve some score.\n");
    }
    else
    {
        if(p1.bestScore==1)
            printf("Your score is 1. That's the heighest score in the game. Try to achieve it again.\n");
        else
            printf("Your best score is %d. Try to beat it.\n", p1.bestScore);
    }
    printf("You have 5 chances to guess the secret number between 1 and %d\n", maxnumber);

    while(attempts < 5)
    {
        printf("\nEnter your guess: ");
        scanf("%d", &guess);
        attempts++;

        if(guess<secretNum)
        {
            printf("Try a higher number. You have %d chances left.\n", 5-attempts);
        }
        else if(guess>secretNum)
        {
            printf("Try a lower number. You have %d chances left.\n", 5-attempts);
        }
        else
        {
            printf("\nCongratulations! you got the right number in %d attempts.\n", attempts);
            isWin=1;
            updateBestScore(&p1, attempts);
            break;
        }
        if(attempts==5)
        {
            printf("\nSorry! you've run out of chances. The secret number was: %d.\n", secretNum);
        }
    }
    DisplayScores();
    trackStatistics(p1.userName, attempts, isWin);
    return 0;
}
