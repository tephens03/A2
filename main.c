#include "defs.h"

// Static declares that this function can only be found in this file and not during linking
static void print_menu(int* choice);

int main(void) {
  RoomCollection  rooms   = { .size = 0 };
  EntryCollection entries = { .size = 0 };
  
  return 0;
}

void print_menu(int* choice) {
  int c = -1;
  int rc = 0;
  const int num_options = 7;

  printf("\nMAIN MENU\n");
  printf("  (1) Load sample data\n");
  printf("  (2) Print entries\n");
  printf("  (3) Print rooms\n");
  printf("  (4) Add room\n");
  printf("  (5) Add entry\n");
  printf("  (6) Test order\n");
  printf("  (7) Test room entries\n");
  printf("  (0) Exit\n\n");

  do {
    printf("Please enter a valid selection: ");
    // Check if they entered a non-integer
    rc = scanf("%d", &c);
    while (getchar() != '\n');
  } while (rc < 1 || c < 0 || c > num_options);

  *choice = c;
}