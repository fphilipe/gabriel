#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

enum direction { left, up, right, down };
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

/*-----------------FUNCTIONS-------------------*/
int prompt_size(char *label);

void print_map(Map *map);

Map *generate_map(int width, int height);

void dig(Map *map, int x, int y);

int can_dig(int dir, Map *map, int x, int y);

int can_move(int dir, Map *map, int x, int y);

void explore(Map *map);

int directionForKey(char key);

void move(int dir, Map *map, int *x, int *y);

/* -------------------MAIN---------------------*/

int main() {
  srand(time(NULL));

  int width = prompt_size("Width");
  int height = prompt_size("Height");
  Map *map = generate_map(width, height);

  explore(map);
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

Map *generate_map(int width, int height) {
  Map *map = malloc(sizeof(*map));

  map->width = width;
  map->height = height;

  map->cells = malloc(sizeof(char *) * height);
  for (int y = 0; y < height; y++) {
    map->cells[y] = malloc(sizeof(char) * width);
    for (int x = 0; x < width; x++) {
      map->cells[y][x] = wall;
    }
  }

  map->cells[1][1] = player;
  dig(map, 1, 1);

  return map;
}

void dig(Map *map, int x, int y) {
  int r, w;
  int n_to_dig = 0;

  int where_to_dig[4];

  for (int dir = 0; dir < 4; dir++) {
    if (can_dig(dir, map, x, y)) {
      where_to_dig[n_to_dig] = dir;
      n_to_dig += 1;
    }
  }
  if (n_to_dig == 0) { //no more possible adjacent possible holes to dig
    map->cells[y][x] = target;
    return;
  } else { //random choice among possible ways of digging
    r = rand() % n_to_dig;
    w = where_to_dig[r];
    //digging
    switch (w) {
      case left:  x -= 2; map->cells[y][x+1] = path; break;
      case up:    y -= 2; map->cells[y+1][x] = path; break;
      case right: x += 2; map->cells[y][x-1] = path; break;
      case down:  y += 2; map->cells[y-1][x] = path; break;
    }
    map->cells[y][x] = path;
    return dig(map, x, y);
  }
}

int can_dig(int dir, Map *map, int x, int y) {
  switch (dir) {
    case left:  return (x-2 > 0)           && map->cells[y][x-2] == wall;
    case up:    return (y-2 > 0)           && map->cells[y-2][x] == wall;
    case right: return (x+2 < map->width)  && map->cells[y][x+2] == wall;
    case down:  return (y+2 < map->height) && map->cells[y+2][x] == wall;
    default: abort();
  }
}

int can_move(int dir, Map *map, int x, int y) {
  switch (dir) {
    case left:  return (x-1 > 0)           && map->cells[y][x-1] == path;
    case up:    return (y-1 > 0)           && map->cells[y-1][x] == path;
    case right: return (x+1 < map->width)  && map->cells[y][x+1] == path;
    case down:  return (y+1 < map->height) && map->cells[y+1][x] == path;
    default: abort();
  }
}

void explore(Map *map) {
  char input;
  int x = 1;
  int y = 1;
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
          if (can_move(dir, map, x, y))
            move(dir, map, &x, &y);
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

void move(int dir, Map *map, int *x, int *y) {
  switch (dir) {
    case left:
      {
        *x -= 1;
        map->cells[*y][*x+1] = path;
        map->cells[*y][*x] = player;
        break;
      }
    case up:
      {
        *y -= 1;
        map->cells[*y+1][*x] = path;
        map->cells[*y][*x] = player;
        break;
      }
    case right:
      {
        *x += 1;
        map->cells[*y][*x-1] = path;
        map->cells[*y][*x] = player;
        break;
      }
    case down:
      {
        *y += 1;
        map->cells[*y-1][*x] = path;
        map->cells[*y][*x] = player;
        break;
      }
  }
}
