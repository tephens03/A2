#include "defs.h"

/* ---- entry comparator -------------------------------------------
   Order: room name ASC, then type ASC by #define value, then timestamp ASC
   Returns <0 if a<b, >0 if a>b, 0 if equal.
----------------------------------------------------------------------------- */
int entry_cmp(const LogEntry *a, const LogEntry *b)
{
  int diff;
  // Compare room names ascendingly lexicographical
  // Return immediately if 2 logs aren't ≠ name-wise, value determined by strcmpr (> 0 if a > b; < 0 if a < b)
  if ((diff = strcmp(a->room->name, b->room->name)))
    return diff;

  // The difference of a and b's type will automatically demonstrate their hierarchy and return immediately if a ≠ b
  if ((diff = a->data.type - b->data.type))
    return diff;

  // Final comparision if a and b = so far. Compare by timestamp difference
  return a->timestamp - b->timestamp;
}

/* ---- rooms_find ------------------------------------------------------------
   Purpose: Find a room by name.
   Params:
    - rc (in): room collection
    - room_name (in): C-string room name
   Returns: pointer to room or NULL if not found or on error
----------------------------------------------------------------------------- */
Room *rooms_find(RoomCollection *rc, const char *room_name)
{
  for (int i = 0; i < rc->rooms->size; i++)
    if (strcmp(rc->rooms[i].name, room_name) == 0)
      return &(rc->rooms[i]);
  return NULL;
}

/* ---- rooms_add -------------------------------------------------------------
   Purpose: Add a room if it does not already exist.
   Params:
     - rc (in/out): room collection
     - room_name (in): C-string room name
   Returns: C_ERR_OK, C_ERR_NULL_PTR, C_ERR_DUPLICATE, C_ERR_FULL_ARRAY
----------------------------------------------------------------------------- */
int rooms_add(RoomCollection *rc, const char *room_name)
{
  if (rc->size == MAX_ARR)
    return C_ERR_FULL_ARRAY;

  for (int i = 0; i < rc->size; i++)
    if (strcmp(rc->rooms[i].name, room_name) == 0)
      return C_ERR_DUPLICATE;

  Room newRoom = {.size = 0};

  strcpy(newRoom.name, room_name);

  rc->rooms[rc->size++] = newRoom;

  return C_ERR_OK;
}

/* ---- entries_create -----------------------------------------------------------
   Purpose: Create a log entry and place it in the global entries (sorted),
            and attach a pointer to it in the owning room (append or sorted, either works)
   Params:
     - ec (in/out): entry collection (owns LogEntry storage)
     - room (in/out): room to attach entry to (must already exist)
     - type (in): TYPE_TEMP|TYPE_DB|TYPE_MOTION
     - value (in): union payload for reading
     - timestamp (in): simple int timestamp
   Returns: C_ERR_OK, C_ERR_NULL_PTR, C_ERR_FULL_ARRAY, C_ERR_INVALID
----------------------------------------------------------------------------- */
int entries_create(EntryCollection *ec, Room *room, int type, ReadingValue value, int timestamp)
{
  if (type != TYPE_TEMP && type != TYPE_DB && type != TYPE_MOTION)
    return C_ERR_INVALID;

  if (ec->size == MAX_ARR)
    return C_ERR_FULL_ARRAY;

  LogEntry newEntry = {.timestamp = timestamp,
                       .room = room,
                       .data = {.type = type, .value = value}};

  int i = ec->size;
  int status = 1;
  while (i > 0 && (status = entry_cmp(&ec->entries[i - 1], &newEntry)) > 0)
  {
    ec->entries[i] = ec->entries[i - 1];
    i--;
  }

  if (status == 0)
    return C_ERR_DUPLICATE;

  ec->entries[i] = newEntry;
  room->entries[room->size++] = &ec->entries[i];
  ec->size++;
  return C_ERR_OK;
}

/* ---- entry_print -----------------------------------------------------------
   Purpose: Print one entry in a formatted row.
   Params:
     - e (in): entry to print
   Returns: C_ERR_OK, C_ERR_NULL_PTR if e is NULL, C_ERR_INVALID if room is NULL
----------------------------------------------------------------------------- */
int entry_print(const LogEntry *e)
{

  return C_ERR_NOT_IMPLEMENTED; // Remove once implemented
}

/* ---- room_print ------------------------------------------------------------
   Purpose: Print a room header and all of its entries (already sorted).
   Params:
     - r (in): room to print
   Returns: C_ERR_OK, C_ERR_NULL_PTR if r is NULL
----------------------------------------------------------------------------- */
int room_print(const Room *r)
{
  printf("Room: %s (entries=%i)\n", r->name, r->size);
  printf("%-35s %-10s %2s %10s\n", "Room", "Timestamp", "Type", "Value");

  for (size_t i = 0; i < r->size; i++)
  {
    LogEntry *entry = r->entries[i];
    printf("%-35s %-10s %2s %10s\n", r->name, entry->timestamp, entry->data.type, entry->data.value);
  }
  printf("\n");

  return C_ERR_NOT_IMPLEMENTED; // Remove once implemented
}

void error_print(int code)
{
  switch (code)
  {
  case C_ERR_OK:
    printf("✅ All good, homie.\n");
    break;
  case C_ERR_NULL_PTR:
    printf("⚠️ Null pointer hit, watch yo pointers.\n");
    break;
  case C_ERR_FULL_ARRAY:
    printf("🚫 Array full, can't fit no more.\n");
    break;
  case C_ERR_NOT_FOUND:
    printf("❌ Couldn’t find that, bruh.\n");
    break;
  case C_ERR_DUPLICATE:
    printf("⚠️ Duplicate detected, ain’t allowed.\n");
    break;
  case C_ERR_INVALID:
    printf("❌ Invalid input, check yo code.\n");
    break;
  case C_ERR_NOT_IMPLEMENTED:
    printf("🛑 Not implemented, homie.\n");
    break;
  default:
    printf("🤔 Unknown error code: %d\n", code);
    break;
  }
}
