#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_RECORDS 256

typedef struct {
  uint8_t id;
  const char *data;
  uint32_t length;
  bool active;
} Record;

typedef struct {
  Record records[MAX_RECORDS];
  uint8_t next_id;
} Database;

void init_database(Database *db) {
  for (uint8_t i = 0; i < MAX_RECORDS; i++) {
    db->records[i].active = false;
  }
  db->next_id = 1;
}

uint8_t create_record(Database *db, const char *data, uint32_t length) {
  uint8_t id = db->next_id++;
  Record *record = &db->records[id];
  record->id = id;
  record->data = data;
  record->length = length;
  record->active = true;
  return id;
}

Record *read_record(Database *db, uint8_t id) { return &db->records[id]; }

void update_record(Database *db, uint8_t id, const char *new_data,
                   uint32_t length) {
  Record *record = &db->records[id];
  record->data = new_data;
  record->length = length;
}

void delete_record(Database *db, uint8_t id) { db->records[id].active = false; }

const char *get_record_data(Database *db, uint8_t id) {
  return db->records[id].data;
}

bool is_record_active(Database *db, uint8_t id) {
  return db->records[id].active;
}

int main() {
  Database db;
  init_database(&db);

  const char data1[] = "First record";
  const char data2[] = "Second record";

  uint8_t id1 = create_record(&db, data1, sizeof(data1) - 1);
  uint8_t id2 = create_record(&db, data2, sizeof(data2) - 1);

  const char *read_data1 = get_record_data(&db, id1);
  const char *read_data2 = get_record_data(&db, id2);

  const char updated_data[] = "Updated first";
  update_record(&db, id1, updated_data, sizeof(updated_data) - 1);

  delete_record(&db, id2);

  bool active1 = is_record_active(&db, id1);
  bool active2 = is_record_active(&db, id2);

  printf("%d", active1);
  return 0;
}
