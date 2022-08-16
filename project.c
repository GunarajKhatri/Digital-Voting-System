/*
Project-Name: "Digital Voting System"
*/

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
time_t now;
#define ENTER 13
#define TAB 9
#define BCKSPC 8
FILE *fp, *cfp, *vfp;
int main();

// voter struct
struct user
{
	char fullName[50];
	char email[50];
	char password[50];
	char division[50];
	int age;
	char citizenship_no[50];
	char district[50];
	int voting_status;
	char sex;
};

// candidate struct
struct candidate
{
	char fullName[50];
	char email[50];
	char division[50];
	int age;
	int voter_num;
	char citizenship_no[50];
	char district[50];
	char party[50];
	char post[50];
	char sex;
	int province_num; // it should be integer
};

// votes struct
struct votes
{
	int voter_num;
	int total_votes;
};

void takeinput(char ch[50])
{
	fgets(ch, 50, stdin);
	ch[strlen(ch) - 1] = 0;
}

void takepassword(char pwd[50])
{

	int i=0;

	char ch;
	while (1)
	{
		ch = getch();
		if (ch == ENTER || ch == TAB)
		{
			pwd[i] = '\0';
			printf("\n");
			break;
		}
		else if (ch == BCKSPC)
		{
			if (i > 0)
			{
				i--;
				printf("\b \b");
			}
		}
		else
		{
			pwd[i++] = ch;
			printf("* \b");
		}
	}
}
void swap(struct votes *voteX, struct votes *voteY) // swap between higher and lower total_votes
{
	struct votes temp;
	temp = *voteX;
	*voteX = *voteY;
	*voteY = temp;
}

int no_of_rows() // gives total votes dropped in a nation
{
	FILE *ffp = fopen("Votes.dat", "rb+");
	struct votes vote;
	int count = 0;
	while (fread(&vote, sizeof(vote), 1, ffp) == 1)
	{
		count++;
	}
	return count;
	fclose(ffp);
}

void sort_votes(int k, struct votes *voteK) // sort out vote by DESC
{
	FILE *ffp = fopen("Votes.dat", "rb+");
	int n = no_of_rows();
	struct votes vote[n];
	for (int i = 0; i < n; i++)
	{
		fread(&vote[i], sizeof(vote), 1, ffp);
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (vote[i].total_votes < vote[j].total_votes)
			{
				swap(&vote[i], &vote[j]);
			}
		}
	}

	fread(&vote[k], sizeof(vote), 1, ffp);
	voteK->voter_num = vote[k].voter_num;
	voteK->total_votes = vote[k].total_votes;
	fclose(ffp);
}

int GoBack() // Let us to main menu after our action done
{
	char ans;
	printf("\n\nDo you want to go back to main menu?[y/n]:\t\n");
	scanf(" %c", &ans);
	if (ans == 'y')
	{
		system("cls");
		main();
		return 0;
	}
	else if (ans == 'n')
	{
		return 0;
	}
	else
	{
		printf("\nInvalid choice!! Try next time..");
		return 0;
	}
}

int MenuChoice()
{
	int opt;
	system("cls");
	printf("|---------------------------------------------------------------------------------------------------------|");
	printf("\n|\t\t\t\t\tWELCOME TO DIGITAL VOTING SYSTEM\t\t\t\t  |\n");
	printf("|---------------------------------------------------------------------------------------------------------|\n");
	printf("|\tThis is the small prototype of digital voting system which is possible to implement especially in |\n|\tcontext of Nepal for local level elections.\t\t\t\t\t\t\t  |\n");
	printf("|---------------------------------------------------------------------------------------------------------|\n\n");
	printf("\nPlease choose your operation:\n");
	printf("\n1.Register for voting eligibility\n");
	printf("\n2.Login for voting\n");
	printf("\n3.Register for candidancy\n");
	printf("\n4.Show All Voters\n");
	printf("\n5.Search Result\n");
	printf("\n6.Exit\n");
	printf("\n\nYour choice:\t");
	scanf("%d", &opt);
	fgetc(stdin);
	return opt;
}

void RegisterVoter()
{
	struct user user;
	char password2[50];
	system("cls");
	printf("\nEnter your full name:\t");
	takeinput(user.fullName);
	printf("\nEnter your email:\t");
	takeinput(user.email);
	printf("\nEnter your age(must be >=16):\t");
	scanf("%d%*c", &user.age);
	if (user.age < 16)
	{
		printf("Sorry,You aren't elligible for voting currently!!\n");
	}
	else
	{
		printf("\nEnter your citizenship-no:\t");
		takeinput(user.citizenship_no);
		printf("\nEnter your sex [m|f]:\t");
		scanf("%c%*c", &user.sex);
		printf("\nEnter your district:\t");
		takeinput(user.district);
		printf("\nEnter your [municipality/sub-metro/metro]'s name:\t");
		takeinput(user.division);
		printf("\nEnter your password:\t");
		takepassword(user.password);
		printf("\nConfirm your password:\t");
		takepassword(password2);
		user.voting_status = 0;

		if (!strcmp(user.password, password2))
		{

			fp = fopen("Users.dat", "ab+");
			fwrite(&user, sizeof(user), 1, fp);
			if (fwrite != 0)
			{
				printf("\n\nVoter resgistration success,Best wishes for voting!!");
				fclose(fp);
			}
			else
			{
				printf("\n\nSorry! Something went wrong :");
			}
		}
		else
		{
			printf("\n\nPassword donot matched");
		}
	}
	GoBack();
}

void LoginVoter()
{
	int usrFound = 0;
	int voter_nums[100] = {};
	int j = 0;
	char citizenship_key[50], pword[50];
	struct user usr;
	int i = 0;
	system("cls");
	printf("\nEnter your citizenship-no:\t");
	takeinput(citizenship_key);
	printf("Enter your voter-key(password):\t");
	takepassword(pword);
	fp = fopen("Users.dat", "rb+");
	struct candidate candi;
	cfp = fopen("Candidates.dat", "rb+");
	rewind(fp);
	struct votes display_votes;
	while (fread(&usr, sizeof(usr), 1, fp) == 1)
	{
		if (!strcmp(usr.citizenship_no, citizenship_key))
		{
			if (!strcmp(usr.password, pword))
			{
				usrFound = 1;
				system("cls");

				while (fread(&candi, sizeof(struct candidate), 1, cfp))
				{

					if (strcmp(usr.district, candi.district) == 0 && strcmp(usr.division, candi.division) == 0)
					{
						i++;
						if (i == 1)
						{
							printf("|--------------------------------------------|\n");
							printf("|General Information:\n");
							printf("|--------------------\n");
							printf("|Voter's name: %s\n", usr.fullName);
							printf("|Email: %s\n", usr.email);
							printf("|Citizenship-no: %d\n", usr.citizenship_no);
							printf("|Age: %d\n", usr.age);
							printf("|District: %s\n", usr.district);
							printf("|--------------------------------------------|\n\n");
							printf("|----------------------------------------------------------------------------------------------------------|\n");
							printf("|Voter-Num\tCandidate\t\t\tParty\t\t\t\tPost                       |\n");
							printf("|----------------------------------------------------------------------------------------------------------|\n");
						}
						vfp = fopen("Votes.dat", "rb+");
						rewind(vfp);
						while (fread(&display_votes, sizeof(struct votes), 1, vfp))
						{

							if (candi.voter_num == display_votes.voter_num)
							{

								voter_nums[j] = display_votes.voter_num;
								printf("|%-16d%-32s%-32s%-26s|", display_votes.voter_num, candi.fullName, candi.party, candi.post);
								j++;
							}
						}
						fclose(vfp);
						printf("\n");
					}
				}
				printf("------------------------------------------------------------------------------------------------------------");

				if (usr.voting_status == 0)
				{
					printf("\n\n Enter the voter-num of respective candidate:\t");
					int num;
					int sum = 0;
					usr.voting_status = 1;
					fseek(fp, -(long)sizeof(usr), SEEK_CUR);
					fwrite(&usr, sizeof(usr), 1, fp);
					fclose(fp);
					scanf("%d", &num);

					vfp = fopen("Votes.dat", "rb+");
					rewind(vfp);
					while (fread(&display_votes, sizeof(struct votes), 1, vfp))
					{
						if (display_votes.voter_num == num)
						{
							display_votes.total_votes++;
							display_votes.voter_num = num;
							fseek(vfp, -(long)sizeof(display_votes), SEEK_CUR);
							fwrite(&display_votes, sizeof(display_votes), 1, vfp);
							if (fwrite)
							{
								sum = 0;
								printf("\nThank You For Voting!! May The Best Win..\n");
							}
							else
							{
								printf("Something went wrong!!");
							}
							fclose(vfp);
						}
					}
				}
				else
				{
					printf("\nYou have already voted!!\n");
				}
			}
			else
			{
				printf("\n\nInvalid Password!");
			}
		}
	}
	fclose(cfp);

	if (usrFound == 0)
	{
		Beep(800, 300);
		printf("\nSorry!! Couldn't find you..");
	}

	if (i == 0 && usrFound == 1)
	{
		printf("\nSorry!!,No Candidates Has Given Candidancy In Your district yet");
	}

	GoBack();
}

void RegisterCandidate()
{
	int opt1;
	struct candidate candidate;
	system("cls");
	printf("\nEnter your full name:\t");
	takeinput(candidate.fullName);
	printf("\nEnter your email:\t");
	takeinput(candidate.email);
	printf("\nEnter your age(must be >=21):\t");
	scanf("%d%*c", &candidate.age);
	printf("\nEnter your citizenship-no:\t");
	takeinput(candidate.citizenship_no);
	printf("\nEnter your sex [m|f]:\t");
	scanf("%c%*c", &candidate.sex);
	printf("\nEnter province-no you lived on(eg:1,2):\t");
	scanf("%d%*c", &candidate.province_num);
	printf("\nEnter your district:\t");
	takeinput(candidate.district);
	printf("\nEnter your [municipality/sub-metro/metro]'s name:\t");
	takeinput(candidate.division);
	printf("\nEnter your political party to be stand out:\t");
	takeinput(candidate.party);
	printf("\nChoose your post for candidancy:");
	char *posts[] = {"Mayor", "Deputy-Mayor", "Province-head", "Province Sub-Head"};
	for (int i = 0; i < 4; i++)
	{
		printf("\n%d.%s", i + 1, posts[i]);
	}
	printf("\n\nYour Operation:\t");
	scanf("%d", &opt1);

	switch (opt1)
	{
	case 1:
		strcpy(candidate.post, posts[0]);
		break;
	case 2:
		strcpy(candidate.post, posts[1]);
		break;
	case 3:
		strcpy(candidate.post, posts[2]);
		break;
	case 4:
		strcpy(candidate.post, posts[3]);
		break;
	default:
		break;
	}
	printf("\nEnter your voting-number(must be unique):\t");
	scanf("%d%*c", &candidate.voter_num);

	cfp = fopen("Candidates.dat", "ab+");
	vfp = fopen("Votes.dat", "ab+");
	struct votes vote;
	vote.voter_num = candidate.voter_num;
	vote.total_votes = 0;
	if (fwrite(&candidate, sizeof(candidate), 1, cfp) == 1 && fwrite(&vote, sizeof(vote), 1, vfp) == 1)
	{

		printf("\n\nCandidancy resgistration success,Best wishes for election..");
	}
	else
	{
		printf("\n\nSorry! Something went wrong :(");
	}
	fclose(cfp);
	fclose(vfp);
	GoBack();
}


void ListOfVoters()
{
	int i = 0;
	struct user usr;
	fp = fopen("Users.dat", "rb+");
	while (fread(&usr, sizeof(usr), 1, fp))
	{
		i++;
		if (i == 1)
		{
			system("cls");
			printf("|--------------------|\n");
			printf("|Lists Of All Voters:|\n");
			printf("|--------------------|\n");
		}
		printf("\n\n|Full Name:\t%s", usr.fullName);
		printf("\n|Email:\t\t%s", usr.email);
		printf("\n|Citizenship_no:\t\t%s", usr.citizenship_no);
		printf("\n|District:\t\t%s", usr.district);
		printf("\n|Age:\t%d", usr.age);
		if (usr.voting_status == 0)
		{
			printf("\n|voting_status:\t%s", "Not Voted");
		}
		else
		{
			printf("\n|voting_status:\t%s", "Voted");
		}
	}
	fclose(fp);

	if (i == 0)
	{
		Beep(800, 300);
		printf("\nSorry!! There are no currently registered voters.\n");
	}
	else
	{
		printf("\n\nTotal Voters: %d\n", i);
	}
	GoBack();
}

void ShowVotes()
{
	int opt;
	int province_num;
	char district[50];
	char division[50];
	int j = 0;
	int total_votes_dropped = 0;
	int total_voter = 0;
	char *posts[] = {"Mayor", "Deputy-Mayor", "Province-head", "Province Sub-Head"};
start:
{
	system("cls");
	printf("Enter the province number:\t");
	scanf("%d%*c", &province_num);
	printf("\nEnter the name of district:\t");
	takeinput(district);
	printf("\nEnter the name of [municipality/sub-metro/metro]:\t");
	takeinput(division);
	printf("\nChoose Post:");
	for (int i = 0; i < 4; i++)
	{
		printf("\n%d.%s", i + 1, posts[i]);
	}
	printf("\n\nYour Operation:\t");
	scanf("%d", &opt);
	struct candidate showcandi;
	struct votes showvote;

	system("cls");
	int len = no_of_rows();
	struct votes vote;
	for (int i = 0; i < len; i++)
	{
		sort_votes(i, &vote);
		cfp = fopen("Candidates.dat", "rb+");
		rewind(cfp);
		while (fread(&showcandi, sizeof(struct candidate), 1, cfp))
		{
			if (vote.voter_num == showcandi.voter_num)
			{
				if (strcmp(district, showcandi.district) == 0 && strcmp(division, showcandi.division) == 0 && strcmp(showcandi.post, posts[opt - 1]) == 0)
				{
					j++;

					if (j == 1)
					{
						printf("|------------|\n");
						printf("|Province:%d  |\n", showcandi.province_num);
						printf("|---------------------------------------------------------------------------------------------------------------------|\n");
						printf("|Voter-Num\tCandidate\t\t\tParty\t\t\t\tPost\t\t\tTotal Votes   |\n");
						printf("|---------------------------------------------------------------------------------------------------------------------|\n");
						printf("|%-16d%-32s%-32s%-28s%-9d|[*Elected]", showcandi.voter_num, showcandi.fullName, showcandi.party, showcandi.post, vote.total_votes);
					}
					else
					{
						printf("|%-16d%-32s%-32s%-28s%-9d|", showcandi.voter_num, showcandi.fullName, showcandi.party, showcandi.post, vote.total_votes);
					}
					total_votes_dropped = total_votes_dropped + vote.total_votes;
					printf("\n");
				}
			}
		}
		fclose(cfp);
	}
}
	if (j == 0)
	{
		char input;
		printf("Sorry!! Couldn't found any candidates from a given data.\n");
		printf("Do you want to try again?[y|n]:\t");
		scanf(" %c", &input);
		if (input == 'y')
		{
			goto start;
		}
		else if (input == 'n')
		{
			exit(0);
		}
		else
		{
			printf("Wrong input!! Please enter appropriate input next time.\n");
		}
	}
	else
	{
		printf("|---------------------------------------------------------------------------------------------------------------------|");
		fp = fopen("Users.dat", "rb+");
		struct user voter;
		while (fread(&voter, sizeof(voter), 1, fp))
		{
			if (strcmp(district, voter.district) == 0 && strcmp(division, voter.division) == 0)
			{
				total_voter++;
			}
		}
		fclose(fp);
		printf("\nTotal Voters: %d", total_voter);
		printf("\nTotal Votes Dropped: %d", total_votes_dropped);
		GoBack();
	}
}

int p = 0;
int voting_start = 60; //  voter and candidancy registration duration
int voting_end = 240;	

int main()
{
	p++;
	if (p == 1)
	{
		now = time(NULL);
	}

	int opt = MenuChoice();

	switch (opt)
	{
	case 1:
	{
		time_t current_time = time(NULL);
		double diff = difftime(current_time, now);
		if (diff < voting_start)
		{
			RegisterVoter();
		}
		else
		{	
			Beep(800, 300);
			printf("\nSorry!!Voting registration time has already ended..\n\n");
			GoBack();
		}
		break;
	}
	case 2:
	{
		time_t current_time = time(NULL);		 
		double diff = difftime(current_time, now); 
		if (diff >= voting_start && diff <= voting_end)
		{
			system("cls");
			LoginVoter();
		}
		else if (diff > voting_end)
		{	
			Beep(800, 300);
			printf("\n\nSorry!!Voting has already ended..\n");
			GoBack();
		}
		else
		{	
			printf("\n\nSorry Voting hasn't opened till now!!\n");
			GoBack();
		}
		break;
	}
	case 3:
	{
		time_t current_time = time(NULL);
		double diff = difftime(current_time, now);
		if (diff < voting_start)
		{
			RegisterCandidate();
		}
		else
		{	
			Beep(800, 300);
			printf("\nSorry!!Candidancy registration time has already ended..\n\n");
			GoBack();
		}

		break;
	}
	case 4:
	{
		time_t current_time = time(NULL);
		double diff = difftime(current_time, now);
		if(diff>voting_start){
			ListOfVoters();
		}else{
			printf("\nVoting Registration hasn't finished yet!!\n\n");
			GoBack();
		}
		break;
	}

	case 5:
	{
		time_t current_time = time(NULL);
		double diff = difftime(current_time, now);
		if (diff > voting_end)
		{
			ShowVotes();
		}
		else if (diff >= voting_start)
		{
			printf("\nVoting is still happening,wait for few moments for result!!\n\n");
			GoBack();
		}
		else
		{	
			Beep(800, 300);
			printf("\nVoting has not started till now!!\n\n");
			GoBack();
		}
		break;
	}
	case 6:
	{
		exit(0);
		break;
	}

	default:
		break;
	}
	return 0;
}
