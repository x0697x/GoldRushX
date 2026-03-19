//      ________       .__       ._____________             .__    ____  ___
//     /  _____/  ____ |  |    __| _/\______   \__ __  _____|  |__ \   \/  /
//    /   \  ___ /  _ \|  |   / __ |  |       _/  |  \/  ___/  |  \ \     / 
//    \    \_\  (  <_> )  |__/ /_/ |  |    |   \  |  /\___ \|   Y  \/     \ 
//     \______  /\____/|____/\____ |  |____|_  /____//____  >___|  /___/\  \
//            \/                  \/         \/           \/     \/      \_/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>

float appversion = 0.3;
#define MAX_HEALTH  100
#define MAX_STAMINA 100
#define FRAME_WIDTH 28

int  health = MAX_HEALTH;
int  stamina = MAX_STAMINA;
int  gold = 0;
int  experience = 0;
int  level = 1;
char name[50];

void saveGame();
void loadGame();
void pause();
void enterName();
void header();
void menu();
void checkHealth();
void checkStamina();
void checkGold();
void checkExperience();
void buy();
void mine();
void combat();
void combatBoss1();

/* -------------------------------------------------------------------------
   Utility
   ------------------------------------------------------------------------- */

void clearInputBuffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

/* -------------------------------------------------------------------------
   Entry point
   ------------------------------------------------------------------------- */

int main()
{
	srand(time(NULL));
	loadGame();

	if (name[0] == '\0')
	{
		enterName();
	}

	printf("Welcome to GoldRushX version %.1f, %s!\n\n", appversion, name);
	pause();
	menu();

	printf("\nThanks for playing GoldRushX!\n");
	printf("(Made with love by lonejul)\n\n");
	saveGame();
	return 0;
}

/* -------------------------------------------------------------------------
   Save / Load
   ------------------------------------------------------------------------- */

void encryptDecrypt(char* data)
{
	char key = 'x';
	for (int i = 0; i < strlen(data); i++) {
		data[i] = data[i] ^ key;
	}
}

void saveGame()
{
	FILE* file = fopen("save.txt", "w");
	if (file == NULL)
	{
		printf("Error saving game!\n\n");
		return;
	}

	char buffer[256];
	sprintf(buffer, "%s|%d|%d|%d|%d|%d", name, health, stamina, gold, experience, level);
	encryptDecrypt(buffer);
	fprintf(file, "%s", buffer);

	fclose(file);
}

void loadGame()
{
	FILE* file = fopen("save.txt", "r");
	if (file == NULL)
	{
		printf("No save file found.\n\n");
		name[0] = '\0';
		return;
	}

	char buffer[256];
	if (fgets(buffer, sizeof(buffer), file))
	{
		encryptDecrypt(buffer);
		sscanf(buffer, "%[^|]|%d|%d|%d|%d|%d", name, &health, &stamina, &gold, &experience, &level);
	}

	fclose(file);
	printf("Game loaded successfully!\n\n");
}

/* -------------------------------------------------------------------------
   UI helpers
   ------------------------------------------------------------------------- */

void pause()
{
	printf("\nPress Enter to continue...");
	getchar();
}

void header()
{
	char name_trunc[20];
	snprintf(name_trunc, sizeof(name_trunc), "%.15s", name);
	if (strlen(name) > 15)
		strcat(name_trunc, "...");

	printf("+------------------------------+\n");
	printf("| GOLDRUSHX v%-18.1f|\n", appversion);
	printf("+------------------------------+\n");
	printf("| Player : %-20s|\n", name_trunc);
	printf("| Gold   : %-20d|\n", gold);
	printf("+------------------------------+\n");
	printf("| Health : %03d  Stamina : %03d  |\n", health, stamina);
	printf("| XP     : %03d  Level   : %d    |\n", experience, level);
	printf("+------------------------------+\n\n");
}

void enterName()
{
	printf("What is your name?\n\n");
	fgets(name, sizeof(name), stdin);

	size_t len = strlen(name);
	if (len > 0 && name[len - 1] == '\n')
		name[len - 1] = '\0';

	saveGame();
	printf("\n");
}

/* -------------------------------------------------------------------------
   Stat checks
   ------------------------------------------------------------------------- */

void checkHealth()
{
	if (health > MAX_HEALTH)
	{
		health = MAX_HEALTH;
	}
	if (health <= 0)
	{
		printf("\n\nYou have died. Game ended\n\n");
		remove("save.txt");
		pause();
		exit(0);
	}
}

void checkStamina()
{
	if (stamina > MAX_STAMINA)
	{
		stamina = MAX_STAMINA;
	}
	if (stamina < 0)
	{
		stamina = 0;
	}
}

void checkGold()
{
	if (gold <= 0)
	{
		gold = 0;
	}
}

void checkExperience()
{
	if (experience < 0)
	{
		experience = 0;
	}
	if (experience >= 100)
	{
		experience = 0;
		level += 1;
		printf("\n\nCongratulations! You leveled up!\n\n");
		pause();
	}
}

/* -------------------------------------------------------------------------
   Menu
   ------------------------------------------------------------------------- */

void menu()
{
	int choice;
	while (1)
	{
		system("cls");
		header();
		printf("1. Mine for Gold\n");
		printf("2. Fight a Monster\n");
		printf("3. Fight Boss 1\n");
		printf("4. Buy Potions\n");
		printf("0. Exit Game\n\n");

		if (scanf("%d", &choice) != 1)
		{
			printf("\nInvalid input. Please enter a number.\n\n");
			clearInputBuffer();
			pause();
			continue;
		}
		clearInputBuffer();

		switch (choice)
		{
		case 1:
			mine();
			break;
		case 2:
			combat();
			break;
		case 3:
			combatBoss1();
			break;
		case 4:
			buy();
			break;
		case 0:
			return;
		default:
			printf("\nInvalid choice.\n\n");
			pause();
		}
	}
}

/* -------------------------------------------------------------------------
   Shop
   ------------------------------------------------------------------------- */

void buy()
{
	int choice = -1;

	while (choice != 0)
	{
		system("cls");
		header();
		printf("--- SHOP ---\n");
		printf("1. Buy 10 Health  (10 Gold)\n");
		printf("2. Buy 10 Stamina (10 Gold)\n");
		printf("0. Exit Shop\n\n");

		if (scanf("%d", &choice) != 1)
		{
			printf("Invalid input.\n\n");
			clearInputBuffer();
			pause();
			choice = -1;
			continue;
		}
		clearInputBuffer();

		if (choice == 1)
		{
			if (gold >= 10)
			{
				if (health >= MAX_HEALTH)
				{
					printf("\nYou are already full health!\n\n");
					pause();
				}
				else
				{
					gold -= 10;
					health += 10;
					checkHealth();
					saveGame();
					printf("\nYou bought a health potion! Your health is now %d.\n", health);
					pause();
				}
			}
			else
			{
				printf("\n\nYou are broke. Get a life\n\n");
				pause();
			}
		}
		else if (choice == 2)
		{
			if (gold >= 10)
			{
				if (stamina >= MAX_STAMINA)
				{
					printf("\nYou are already full stamina!\n\n");
					pause();
				}
				else
				{
					gold -= 10;
					stamina += 10;
					checkStamina();
					saveGame();
					printf("\nYou bought a stamina potion! Your stamina is now %d.\n", stamina);
					pause();
				}
			}
			else
			{
				printf("\n\nYou are broke. Get a life\n\n");
				pause();
			}
		}
	}
}

/* -------------------------------------------------------------------------
   Actions
   ------------------------------------------------------------------------- */

void mine()
{
	int choice;
	printf("\nDo you want to mine for gold? (1 for Yes, 0 for No)\n\n");

	if (scanf("%d", &choice) != 1) {
		clearInputBuffer();
		return;
	}
	clearInputBuffer();

	if (choice == 1)
	{
		if (stamina >= 20)
		{
			printf("\nMining in progress...");
			for (int i = 5; i > 0; i--)
			{
				printf("\r%2d seconds remaining...           ", i);
				fflush(stdout);
				Sleep(1000);
			}

			int staminaLoss = rand() % 26 + 5;
			stamina -= staminaLoss;
			int loot = rand() % 50 + 1;
			gold += loot;
			int xpGain = rand() % 21 + 5;
			experience += xpGain;
			checkGold();
			checkStamina();
			printf("\n\n");
			printf("\nYou found %d gold, lost %d stamina and gain %d experience!\n\n", loot, staminaLoss, xpGain);
			pause();
			checkExperience();
			saveGame();
		}
		else
		{
			printf("\nYou need 20 stamina to mine.\n\n");
			pause();
		}
	}
}

void combat()
{
	int choice;
	printf("\nYou encounter a monster! Do you want to fight it? (1 for Yes, 0 for No)\n\n");

	if (scanf("%d", &choice) != 1) {
		clearInputBuffer();
		return;
	}
	clearInputBuffer();

	if (choice == 1)
	{
		if (health >= 20 && stamina >= 30)
		{
			printf("\nFighting in progress...");
			for (int i = 5; i > 0; i--)
			{
				printf("\r%2d seconds remaining...           ", i);
				fflush(stdout);
				Sleep(1000);
			}

			int damage = rand() % 26 + 5;
			health -= damage;
			int staminaLoss = rand() % 31 + 10;
			stamina -= staminaLoss;
			int loot = rand() % 91 + 10;
			gold += loot;
			int xpGain = rand() % 31 + 10;
			experience += xpGain;
			checkHealth();
			checkStamina();
			printf("\n");
			printf("\nVictory! You found %d gold, lost %d health and %d stamina and gain %d experience!\n\n", loot, damage, staminaLoss, xpGain);
			pause();
			checkExperience();
			saveGame();
		}
		else
		{
			printf("\nYou need at least 20 health and 30 stamina to fight.\n\n");
			pause();
		}
	}
}

void combatBoss1()
{
	int choice;
	printf("\nYou encounter the Boss 1! Do you want to fight it? (1 for Yes, 0 for No)\n\n");

	if (scanf("%d", &choice) != 1) {
		clearInputBuffer();
		return;
	}
	clearInputBuffer();

	if (choice == 1)
	{
		if (health >= 30 && stamina >= 45 && level >= 3)
		{
			printf("\nFighting the Boss in progress...");
			for (int i = 10; i > 0; i--)
			{
				printf("\r%2d seconds remaining...            ", i);
				fflush(stdout);
				Sleep(1000);
			}

			int damage = rand() % 39 + 7;
			health -= damage;
			int staminaLoss = rand() % 46 + 15;
			stamina -= staminaLoss;
			int loot = rand() % 136 + 15;
			gold += loot;
			int xpGain = rand() % 51 + 20;
			experience += xpGain;
			checkHealth();
			checkStamina();
			printf("\n");
			printf("\nVictory! You defeated the Boss 1 and found %d gold, lost %d health, %d stamina and gain %d experience!\n\n", loot, damage, staminaLoss, xpGain);
			pause();
			checkExperience();
			saveGame();
		}
		else
		{
			printf("\nYou need at least 30 health, 45 stamina and level 3 to fight the Boss 1.\n\n");
			pause();
		}
	}
}

//    .__                            __      .__   
//    |  |   ____   ____   ____     |__|__ __|  |  
//    |  |  /  _ \ /    \_/ __ \    |  |  |  \  |  
//    |  |_(  <_> )   |  \  ___/    |  |  |  /  |__
//    |____/\____/|___|  /\___  >\__|  |____/|____/
//                     \/     \/\______|           