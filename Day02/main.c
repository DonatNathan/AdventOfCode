#include <stdio.h>
#include <stdlib.h>
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

int count_score_part2(char **array)
{
    int global_score = 0;

    for (int i = 0; array[i]; i += 1) {
        if (array[i][2] == 'X') {
            if (array[i][0] == 'A')
                global_score += 3;
            if (array[i][0] == 'B')
                global_score += 1;
            if (array[i][0] == 'C')
                global_score += 2;
        }
        if (array[i][2] == 'Y') {
            global_score += 3;
            if (array[i][0] == 'A')
                global_score += 1;
            if (array[i][0] == 'B')
                global_score += 2;
            if (array[i][0] == 'C')
                global_score += 3;
        }
        if (array[i][2] == 'Z') {
            global_score += 6;
            if (array[i][0] == 'A')
                global_score += 2;
            if (array[i][0] == 'B')
                global_score += 3;
            if (array[i][0] == 'C')
                global_score += 1;
        }
    }
    return global_score;
}

int count_score_part1(char **array)
{
    int global_score = 0;

    for (int i = 0; array[i]; i += 1) {
        if (array[i][2] == 'X') {
            global_score += 1;
            if (array[i][0] == 'A')
                global_score += 3;
            if (array[i][0] == 'C')
                global_score += 6;
        }
        if (array[i][2] == 'Y') {
            global_score += 2;
            if (array[i][0] == 'B')
                global_score += 3;
            if (array[i][0] == 'A')
                global_score += 6;
        }
        if (array[i][2] == 'Z') {
            global_score += 3;
            if (array[i][0] == 'C')
                global_score += 3;
            if (array[i][0] == 'B')
                global_score += 6;
        }
    }
    return global_score;
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
    int global_score;

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
            global_score = count_score_part1(array);
        else
            global_score = count_score_part2(array);
        printf("%d\n", global_score);
        free_all(array, file);
    }
    return 0;
}