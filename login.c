#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int choice;

    while (1) {
        printf("\n============================\n");
        printf("   Welcome to P2P Chat Server\n");
        printf("============================\n");
        printf("1) Create Server\n");
        printf("2) Join Server\n");
        printf("3) Been Here Before (List Servers)\n");
        printf("4) Exit\n");
        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input, try again.\n");
            continue;
        }

        switch (choice) {
            case 1:
                printf(">> [Placeholder] Create server selected.\n");
                break;
            case 2:
                printf(">> [Placeholder] Join server selected.\n");
                break;
            case 3:
                printf(">> [Placeholder] Show previously joined servers.\n");
                break;
            case 4:
                printf("Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice, please try again.\n");
        }
    }

    return 0;
}

