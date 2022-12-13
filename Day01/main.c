#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int get_file_len(char *filepath)
{
  int len = 0;
  char buff[1];

  int fd = open(filepath, O_RDONLY);
  for (int i = 1; i != 0; len += 1)
    i = read(fd, buff, 1);
  close(fd);
  return (len);
}

char *read_file(int fd, int len, char *file)
{
  read(fd, file, len);
  file[len - 1] = '\0';
  return file;
}

int error_handling(int argc, char **argv)
{
  if (argc != 3)
    return 1;
  if (strcmp(argv[1], "part1") != 0 && strcmp(argv[1], "part2") != 0) 
    return 1;  
  if (strlen(argv[2]) < 1)
    return 1;
  return 0;
}

int get_line_len(char *str, int i)
{
  int cmpt = 0;

  while (str[i] != '\n') {
    i += 1;
    cmpt += 1;
  }
  cmpt += 1;
  return cmpt;
}

char **str_to_array(char *str, int len)
{
  char **array;
  int nb_ligne = 0;
  int line = 0;
  int cmpt = 0;
  int chara = 0;

  for (int i = 0; i < len; i += 1)
    if (str[i] == '\n')
      nb_ligne += 1;
  array = malloc(sizeof(char *) * (nb_ligne + 1) + 1);
  cmpt = get_line_len(str, 0);
  array[line] = malloc(sizeof(char) * cmpt + 1);
  for (int i = 0; i < len; i += 1) {
    if (str[i] == '\n' && str[i + 1]) {
      array[line][chara] = '\n';
      array[line][chara + 1] = '\0';
      line += 1;
      chara = 0;
      cmpt = get_line_len(str, i + 1);
      array[line] = malloc(sizeof(char) * cmpt + 1);
    } else {
      array[line][chara] = str[i];
      chara += 1;
    }
  }
  array[line + 1] = NULL;
  return array;
}

void print_array(char **array)
{
  for (int i = 0; array[i]; i += 1)
    printf("%s", array[i]);
}

int count_cal_part1(char **array)
{
  int most_calories = 0;
  int elves_calories = 0;

  for (int line = 0; array[line]; line += 1) {
    if (atoi(array[line]) == 0) {
      if (elves_calories > most_calories) {
        most_calories = elves_calories;
      }
      elves_calories = 0;
    } else {
      elves_calories += atoi(array[line]);
    }
  }
  return most_calories;
}

int get_nb_elves(char **array)
{
  int nb = 0;

  for (int i = 0; array[i]; i += 1)
    if (atoi(array[i]) == 0)
      nb += 1;
  return nb - 1;
}

int *sort_int_tab(int *cal_per_elves, int size, int *sorted_list)
{
  int biggest;
  int cmpt;

  for (int i = 0; i < size; i += 1) {
    biggest = 0;
    cmpt = -1;
    for (int y = 0; y < size; y += 1) {
      if (cal_per_elves[y] >= biggest) {
        biggest = cal_per_elves[y];
        cmpt = y;
      }
    }
    sorted_list[i] = biggest;
    cal_per_elves[cmpt] = -1;
  }
  return sorted_list;
}

int count_cal_part2(char **array)
{
  int elves_calories = 0;
  int nb_elves = get_nb_elves(array);
  int cal_per_elves[nb_elves];
  int *sorted_list = malloc(sizeof(int) * nb_elves);
  int elf = 0;
  int top_three;

  for (int line = 1; array[line]; line += 1) {
    if (atoi(array[line]) == 0) {
      cal_per_elves[elf] = elves_calories;
      elves_calories = 0;
      elf += 1;
    } else {
      elves_calories += atoi(array[line]);
    }
  }
  sorted_list = sort_int_tab(cal_per_elves, nb_elves, sorted_list);
  top_three = sorted_list[0] + sorted_list[1] + sorted_list[2];
  free(sorted_list);
  return top_three;
}

void free_all(char **array, char *file)
{
  int i;

  free(file);
  for (i = 0; array[i]; i += 1)
    free(array[i]);
  free(array);
}

int main(int argc, char **argv)
{
  int fd = open(argv[2], O_RDONLY);
  int len;
  char *file;
  char **array;
  int biggest_calories;

  if (error_handling(argc, argv) == 1)
    return 1;

  if (fd == -1) {
    write(2, "reading file failed\n", 20);
  } else {
    len = get_file_len(argv[2]);
    file = malloc(sizeof(char) * len);
    file = read_file(fd, len, file);
    array = str_to_array(file, len);
    if (strcmp(argv[1], "part1") == 0)
      biggest_calories = count_cal_part1(array);
    else
      biggest_calories = count_cal_part2(array);
    printf("%d\n", biggest_calories);
    free_all(array, file);
  }
  return 0;
}
