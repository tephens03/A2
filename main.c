#include "defs.h"

// Static declares that this function can only be found in this file and not during linking
static void print_menu(int *choice);

int main(void)
{
  RoomCollection rooms = {.size = 0};
  EntryCollection entries = {.size = 0};
  while (1)
  {
    int choice;
    int status = 1000;
    char buffer[MAX_STR];

    print_menu(&choice);
    if (choice == 1)
    {
      if (!freopen("./inputs.txt", "r", stdin))
      {
        perror("freopen");
        status = C_ERR_NULL_PTR;
      }
      else
      {
        printf("Loaded input script from file.\n");
      }
    }

    else if (choice == 2)
    {
      printf("%-15s  %10s  %-6s    %15s\n", "Room", "Timestamp", "Type", "Value");
      for (int i = 0; i < entries.size; i++)
        status = entry_print(&entries.entries[i]);
    }
    else if (choice == 3)
    {
      printf("Here are list of (%i) rooms:\n", rooms.size);
      for (int i = 0; i < rooms.size; i++)
        status = room_print(&rooms.rooms[i]);
    }

    else if (choice == 4)
    {
      printf("Enter room name: ");
      fgets(buffer, MAX_STR, stdin);
      buffer[strcspn(buffer, "\n")] = '\0';
      status = rooms_add(&rooms, buffer); // main operation
    }

    else if (choice == 5)
    {
      printf("== Adding New Entry. \n");

      // get room to add for
      printf("Room name: ");
      fgets(buffer, MAX_STR, stdin);
      buffer[strcspn(buffer, "\n")] = '\0';
      Room *room = rooms_find(&rooms, buffer);

      printf("Timestamp (int): ");
      int timestamp;
      scanf("%d", &timestamp);

      printf("Type (1=TEMP, 2=DB, 3=MOTION): ");
      int type;
      scanf("%d", &type);
      
      ReadingValue reading;
      get_entry_value(&reading, type);

      status = entries_create(&entries, room, type, reading, timestamp);
    }

    else if (choice == 6)
    {
    }
    else if (choice == 7)
    {
    }
    else if (choice == 0)
    {
      break;
    }

    error_print(status);
  }

  return 0;
}

void print_menu(int *choice)

{
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

  do
  {
    printf("Please enter a valid selection: ");
    // Check if they entered a non-integer
    rc = scanf("%d", &c);
    while (getchar() != '\n')
      ;
  } while (rc < 1 || c < 0 || c > num_options);

  *choice = c;
}
