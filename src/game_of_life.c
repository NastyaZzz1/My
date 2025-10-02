#include <stdio.h>
#include <unistd.h> // Для usleep

#define NMAX 25
#define MMAX 80

int input_matrix(int matrix[NMAX][MMAX]);
void input_mask(int mask[NMAX][MMAX]);

int search_count(int matrix[NMAX][MMAX], int i, int j);

void die_or_live(int matrix[NMAX][MMAX], int mask[NMAX][MMAX],
                 int index_revival, int index_death);
void result_in_matrix(int matrix[NMAX][MMAX], int mask[NMAX][MMAX],
                      int index_revival, int index_death);

void output_matrix(int matrix[NMAX][MMAX]);

int main() {
  int matrix[NMAX][MMAX];
  int mask[NMAX][MMAX];

  int index_revival = 6;
  int index_death = 3;

  int delay = 100000;

  if (input_matrix(matrix) == 0) {
    printf("n/a");
  } else {
    input_mask(mask);

    while (1) {
      printf("\033[H\033[J");

      die_or_live(matrix, mask, index_revival, index_death);

      result_in_matrix(matrix, mask, index_revival, index_death);
      output_matrix(matrix);

      // char c = getchar();
      // // ПЕРЕМЕЩЕНИЕ РАКЕТОК
      // if (c != ERR) {
      //     switch (c) {
      //         case 'a': {delay -= 10000; break;}
      //         case 'z': {delay += 10000; break;}
      //     }
      // }

      usleep(delay);
    }
  }
  return 0;
}

int input_matrix(int matrix[NMAX][MMAX]) {
  int flag = 1;
  for (int i = 0; i < NMAX; i++) {
    for (int j = 0; j < MMAX; j++) {
      if (scanf("%d", &matrix[i][j]) != 1) {
        flag = 0;
      }
    }
  }
  return flag;
}

void input_mask(int mask[NMAX][MMAX]) {
  for (int i = 0; i < NMAX; i++) {
    for (int j = 0; j < MMAX; j++) {
      mask[i][j] = 0;
    }
  }
}

int search_count(int matrix[NMAX][MMAX], int i, int j) {

  int count = 0;
  int i_last = i - 1;
  int j_last = j - 1;
  int i_next = i + 1;
  int j_next = j + 1;

  if (i_last < 0)
    i_last = NMAX - 1;
  if (i_next > NMAX - 1)
    i_next = i_next % NMAX;

  if (j_last < 0)
    j_last = MMAX - 1;
  if (j_next > MMAX - 1)
    j_next = j_next % MMAX;

  count += matrix[i_last][j_last];
  count += matrix[i_last][j];

  count += matrix[i_last][j_next];
  count += matrix[i][j_last];

  count += matrix[i][j_next];
  count += matrix[i_next][j_last];

  count += matrix[i_next][j];
  count += matrix[i_next][j_next];

  return count;
}

void die_or_live(int matrix[NMAX][MMAX], int mask[NMAX][MMAX],
                 int index_revival, int index_death) {

  for (int i = 0; i < NMAX; i++) {
    for (int j = 0; j < MMAX; j++) {

      // если мертвая клетка имеет 3 соседа, она возраждается
      if (matrix[i][j] == 0) {
        if (search_count(matrix, i, j) == 3) {
          mask[i][j] = index_revival;
        }

        // если живая клетка имеет меньше двух соседей или больше трёх, она
        // умирает
      } else if (matrix[i][j] == 1) {
        if (search_count(matrix, i, j) < 2 || search_count(matrix, i, j) > 3) {
          mask[i][j] = index_death;
        }

      } else {
        printf("n/a");
      }
    }
  }
}

// записываем изменения в изначальную маитрицу
void result_in_matrix(int matrix[NMAX][MMAX], int mask[NMAX][MMAX],
                      int index_revival, int index_death) {

  for (int i = 0; i < NMAX; i++) {
    for (int j = 0; j < MMAX; j++) {

      if (mask[i][j] == index_revival) {
        matrix[i][j] = 1;

      } else if (mask[i][j] == index_death) {
        matrix[i][j] = 0;
      }
    }
  }
}

void output_matrix(int matrix[NMAX][MMAX]) {
  for (int i = 0; i < NMAX; i++) {
    if (i != 0) {
      printf("%c", '\n');
    }
    for (int j = 0; j < MMAX; j++) {
      if (j != 0) {
        printf(" ");
      }
      printf("%d", matrix[i][j]);
    }
  }
}
