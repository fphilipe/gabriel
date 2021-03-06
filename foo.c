#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

enum direction {
  left = 1,
  up,
  right,
  down
};

enum cell_type {
  wall   = '#',
  path   = ' ',
  player = 'K',
  target = 'O'
};

enum keys {
  keyLeft  = 'a',
  keyUp    = 'w',
  keyRight = 'd',
  keyDown  = 's',
  keyQuit  = 'q'
};

typedef struct {
  int width;
  int height;
  char **cells;
} Map;

typedef struct {
  int x;
  int y;
} Position;

/*-----------------FUNCTIONS-------------------*/
int prompt_size(char *label);

void print_map(Map *map);

void alloc_map(Map *map, int width, int height);

void free_map(Map *map);

void generate_labyrinth(Map *map);

int random_digging_direction(Map *map, Position *pos);

int can_dig(int dir, Map *map, Position *pos);

void dig(int dir, Map *map, Position *pos);

int can_move(int dir, Map *map, Position *pos);

int reaches_target(int dir, Map *map, Position *pos);

void explore(Map *map);

int directionForKey(char key);

void move(int dir, Map *map, Position *pos);

/* -------------------MAIN---------------------*/

int main() {
  srand(time(NULL));

  int width = prompt_size("Width");
  int height = prompt_size("Height");
  Map map;
  alloc_map(&map, width, height);
  generate_labyrinth(&map);

  explore(&map);

  free_map(&map);
  return 0;
}

/*---------------------------------------------*/
int prompt_size(char *label) {
  int size;
  while (1) {
    printf("%s of the map? (odd number)\n", label);
    scanf("%d%*c", &size);
    if (size % 2 == 1) {
      return size;
    } else {
      printf("Even number entered!\n");
    }
  }
}

void print_map(Map *map) {
  for (int y = 0; y < map->height; y++) {
    for (int x = 0; x < map->width; x++) {
      printf("%c", map->cells[y][x]);
    }
    printf("\n");
  }
}

void alloc_map(Map *map, int width, int height) {
  map->width = width;
  map->height = height;

  map->cells = malloc(sizeof(char *) * height);
  for (int y = 0; y < height; y++) {
    map->cells[y] = malloc(sizeof(char) * width);
    for (int x = 0; x < width; x++) {
      map->cells[y][x] = wall;
    }
  }
}

void free_map(Map *map) {
  for (int y = 0; y < map->height; y++) {
    free(map->cells[y]);
  }
  free(map->cells);
}

void generate_labyrinth(Map *map) {
  Position pos = { .x = 1, .y = 1 };
  map->cells[pos.x][pos.y] = player;

  while (1) {
    int dir = random_digging_direction(map, &pos);
    if (!dir) break;
    // We always dig two cells at a time:
    dig(dir, map, &pos);
    dig(dir, map, &pos);
  }

  map->cells[pos.y][pos.x] = target;
}

int random_digging_direction(Map *map, Position *pos) {
  int count = 0;
  int options[4];
  for (int dir = left; dir <= down; dir++) {
    if (can_dig(dir, map, pos)) {
      options[count] = dir;
      count += 1;
    }
  }

  if (count == 0) {
    return 0;
  } else {
    return options[rand() % count];
  }
}

int can_dig(int dir, Map *map, Position *pos) {
  switch (dir) {
    case left:  return (pos->x-2 > 0)           && map->cells[pos->y][pos->x-2] == wall;
    case up:    return (pos->y-2 > 0)           && map->cells[pos->y-2][pos->x] == wall;
    case right: return (pos->x+2 < map->width)  && map->cells[pos->y][pos->x+2] == wall;
    case down:  return (pos->y+2 < map->height) && map->cells[pos->y+2][pos->x] == wall;
    default: abort();
  }
}

void dig(int dir, Map *map, Position *pos) {
  switch (dir) {
    case left:  pos->x -= 1; break;
    case up:    pos->y -= 1; break;
    case right: pos->x += 1; break;
    case down:  pos->y += 1; break;
    default: abort();
  }
  map->cells[pos->y][pos->x] = path;
}

int can_move(int dir, Map *map, Position *pos) {
  switch (dir) {
    case left:  return (pos->x-1 > 0)           && map->cells[pos->y][pos->x-1] == path;
    case up:    return (pos->y-1 > 0)           && map->cells[pos->y-1][pos->x] == path;
    case right: return (pos->x+1 < map->width)  && map->cells[pos->y][pos->x+1] == path;
    case down:  return (pos->y+1 < map->height) && map->cells[pos->y+1][pos->x] == path;
    default: abort();
  }
}

int reaches_target(int dir, Map *map, Position *pos) {
  switch (dir) {
    case left:  return map->cells[pos->y][pos->x-1] == target;
    case up:    return map->cells[pos->y-1][pos->x] == target;
    case right: return map->cells[pos->y][pos->x+1] == target;
    case down:  return map->cells[pos->y+1][pos->x] == target;
    default: abort();
  }
}

void explore(Map *map) {
  char input;
  Position pos = {1, 1};
  while (input != 'q') {
    print_map(map);
    printf("\nChoose your next action (wasd - q to exit):\n");
    scanf("\n");
    scanf("%1c", &input);
    switch (input) {
      case keyLeft:
      case keyUp:
      case keyRight:
      case keyDown:
        {
          int dir = directionForKey(input);
          if (can_move(dir, map, &pos)) {
            move(dir, map, &pos);
          } else if (reaches_target(dir, map, &pos)) {
            move(dir, map, &pos);
            print_map(map);
            printf("Congrats!\n");
            return;
          }
          break;
        }
      case keyQuit: break;
      default: continue;
    }
  }
  return;
}

int directionForKey(char key) {
  switch (key) {
    case keyLeft:  return left;
    case keyUp:    return up;
    case keyRight: return right;
    case keyDown:  return down;
    default: abort();
  }
}

void move(int dir, Map *map, Position *pos) {
  switch (dir) {
    case left:
      {
        pos->x -= 1;
        map->cells[pos->y][pos->x+1] = path;
        map->cells[pos->y][pos->x] = player;
        break;
      }
    case up:
      {
        pos->y -= 1;
        map->cells[pos->y+1][pos->x] = path;
        map->cells[pos->y][pos->x] = player;
        break;
      }
    case right:
      {
        pos->x += 1;
        map->cells[pos->y][pos->x-1] = path;
        map->cells[pos->y][pos->x] = player;
        break;
      }
    case down:
      {
        pos->y += 1;
        map->cells[pos->y-1][pos->x] = path;
        map->cells[pos->y][pos->x] = player;
        break;
      }
  }
}
